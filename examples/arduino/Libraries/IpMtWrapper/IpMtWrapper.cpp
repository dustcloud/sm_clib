/*
Copyright (c) 2014, Dust Networks.  All rights reserved.

Arduino library to connect to a SmartMesh IP mote and periodically send data.

This library is an Arduino "wrapper" around the generic SmartMesh C library.

This library will:
- Connect to the SmartMesh IP mote over its serial port.
- Have the SmartMesh IP mote connect to a SmartMesh IP network, open and bind a
  UDP socket
- Periodically, invoke a data generation function and send the generated
  payload to the specified IPv6 address and UDP port.
  
\license See attached DN_LICENSE.txt.
*/

#include "Arduino.h"
#include "IpMtWrapper.h"
#include "dn_ipmt.h"
#include "dn_uart.h"

//=========================== prototypes ======================================

#ifdef __cplusplus
 extern "C" {
#endif

void dn_ipmt_notif_cb(uint8_t cmdId, uint8_t subCmdId);
void dn_ipmt_reply_cb(uint8_t cmdId);

#ifdef __cplusplus
}
#endif

//=========================== variables =======================================

extern IpMtWrapper ipmtwrapper;

typedef struct {
   // fsm
   TIME_T               fsmPreviousEvent;
   TIME_T               fsmDelay;
   fsm_timer_callback   fsmCb;
   // module
   dn_uart_rxByte_cbt   ipmt_uart_rxByte_cb;
   // reply
   fsm_reply_callback   replyCb;
   // api
   uint8_t              socketId;                          // ID of the mote's UDP socket
   uint16_t             srcPort;                           // UDP source port
   uint8_t              destAddr[IPv6ADDR_LEN];            // IPv6 destination address
   uint16_t             destPort;                          // UDP destination port
   TIME_T               dataPeriod;                        // number of ms between transmissions
   data_generator       dataGenerator;                     // data generating function
   uint8_t              replyBuf[MAX_FRAME_LENGTH];        // holds notifications from ipmt
   uint8_t              notifBuf[MAX_FRAME_LENGTH];        // notifications buffer internal to ipmt
} app_vars_t;

app_vars_t              app_vars;

//=========================== public ==========================================

/**
\brief Constructor.
*/
IpMtWrapper::IpMtWrapper() {
}

/**
\brief Setting up the instance.
*/
void IpMtWrapper::setup(
      uint16_t       srcPort,
      uint8_t*       destAddr,
      uint16_t       destPort,
      TIME_T         dataPeriod,
      data_generator dataGenerator
   ) {
   // reset local variables
   memset(&app_vars,    0, sizeof(app_vars));
   
   // store params
   app_vars.srcPort          = srcPort;
   memcpy(app_vars.destAddr,destAddr,IPv6ADDR_LEN);
   app_vars.destPort         = destPort;
   app_vars.dataPeriod       = dataPeriod;
   app_vars.dataGenerator    = dataGenerator;
   
   // initialize the serial port connected to the computer
   Serial.begin(BAUDRATE_CLI);
   
   // initialize the ipmt module
   dn_ipmt_init(
      dn_ipmt_notif_cb,                // notifCb
      app_vars.notifBuf,               // notifBuf
      sizeof(app_vars.notifBuf),       // notifBufLen
      dn_ipmt_reply_cb                 // replyCb
   );
   
   // print banner
   Serial.print("IpMtWrapper Library, version ");
   Serial.print(VER_MAJOR);
   Serial.print(".");
   Serial.print(VER_MINOR);
   Serial.print(".");
   Serial.print(VER_PATCH);
   Serial.print(".");
   Serial.print(VER_BUILD);
   Serial.println(" (c) Dust Networks, 2014.");
   Serial.println("");
   
   // schedule first event
   fsm_scheduleEvent(2*CMD_PERIOD, &IpMtWrapper::api_getMoteStatus);
}

void IpMtWrapper::loop() {
   uint8_t byte;
   TIME_T  currentTime;
   
   // receive and react to HDLC frames
   while (Serial1.available()) {
      
      // read a serial byte
      byte = uint8_t(Serial1.read());
      
      // hand over byte to ipmt module
      app_vars.ipmt_uart_rxByte_cb(byte);
   }
   
   // kick the fsm
   currentTime = millis();
   if (app_vars.fsmDelay>0 && (currentTime-app_vars.fsmPreviousEvent>app_vars.fsmDelay)) {
      // cancel event
      app_vars.fsmDelay=0;
      
      // handle event (by calling the callback)
      (this->*app_vars.fsmCb)();
   }
}

//=========================== private =========================================

//===== fsm

void IpMtWrapper::fsm_scheduleEvent(uint16_t delay, fsm_timer_callback cb) {
   app_vars.fsmDelay    = delay;
   app_vars.fsmCb       = cb;
}

void IpMtWrapper::fsm_cancelEvent() {
   app_vars.fsmDelay    = 0;
   app_vars.fsmCb       = NULL;
}

void IpMtWrapper::fsm_setCallback(fsm_reply_callback cb) {
   app_vars.replyCb     = cb;
}

void IpMtWrapper::api_response_timeout(void) {
   
   // log
   Serial.print("ERROR:    timeout!");
   
   // issue cancel command
   dn_ipmt_cancelTx();
   
   // schedule first event
   fsm_scheduleEvent(2*CMD_PERIOD, &IpMtWrapper::api_getMoteStatus);
}

//===== getMoteStatus

void IpMtWrapper::api_getMoteStatus(void) {
   dn_err_t err;
   
   // record time
   app_vars.fsmPreviousEvent = millis();
   
   // log
   Serial.println("");
   Serial.print("INFO:     api_getMoteStatus... returns ");
   
   // arm callback
   fsm_setCallback(&IpMtWrapper::api_getMoteStatus_reply);
   
   // issue function
   err = dn_ipmt_getParameter_moteStatus(
      (dn_ipmt_getParameter_moteStatus_rpt*)(app_vars.replyBuf)
   );
   
   // log
   Serial.println(err);
   
   // schedule timeout event
   fsm_scheduleEvent(SERIAL_RESPONSE_TIMEOUT, &IpMtWrapper::api_response_timeout);
}

void IpMtWrapper::api_getMoteStatus_reply() {
   dn_ipmt_getParameter_moteStatus_rpt* reply;
   
   // cancel timeout
   fsm_cancelEvent();
   
   // record time
   app_vars.fsmPreviousEvent = millis();
   
   Serial.println("INFO:     api_getMoteStatus_reply");
   
   reply = (dn_ipmt_getParameter_moteStatus_rpt*)app_vars.replyBuf;
   
   Serial.print("INFO:     state=");
   this->printState(reply->state);
   
   switch (reply->state) {
      case MOTE_STATE_IDLE:
         fsm_scheduleEvent(CMD_PERIOD, &IpMtWrapper::api_openSocket);
         break;
      case MOTE_STATE_OPERATIONAL:
         // the API currently does not allow to find out what the open sockets are
         app_vars.socketId = DEFAULT_SOCKETID;
         
         // mote already operational, send data
         fsm_scheduleEvent(CMD_PERIOD, &IpMtWrapper::api_getServiceInfo);
         break;
      default:
         fsm_scheduleEvent(CMD_PERIOD, &IpMtWrapper::api_getMoteStatus);
         break;
      }
}

//===== openSocket

void IpMtWrapper::api_openSocket(void) {
   dn_err_t err;
   
   // record time
   app_vars.fsmPreviousEvent = millis();
   
   // log
   Serial.println("");
   Serial.print("INFO:     api_openSocket... returns ");
   
   // arm callback
   fsm_setCallback(&IpMtWrapper::api_openSocket_reply);
   
   // issue function
   err = dn_ipmt_openSocket(
      0,                                              // protocol
      (dn_ipmt_openSocket_rpt*)(app_vars.replyBuf)    // reply
   );
   
   // log
   Serial.println(err);
   
   // schedule timeout event
   fsm_scheduleEvent(SERIAL_RESPONSE_TIMEOUT, &IpMtWrapper::api_response_timeout);
}

void IpMtWrapper::api_openSocket_reply() {
   dn_ipmt_openSocket_rpt* reply;
   
   // cancel timeout
   fsm_cancelEvent();
   
   // record time
   app_vars.fsmPreviousEvent = millis();
   
   Serial.println("INFO:     api_openSocket_reply");
   
   reply = (dn_ipmt_openSocket_rpt*)app_vars.replyBuf;
   
   Serial.print("INFO:     socketId=");
   Serial.println(reply->socketId);
   
   // store the socketID
   app_vars.socketId = reply->socketId;
   
   // schedule next event
   fsm_scheduleEvent(CMD_PERIOD, &IpMtWrapper::api_bindSocket);
}

//===== bindSocket

void IpMtWrapper::api_bindSocket(void) {
   dn_err_t err;
   
   // record time
   app_vars.fsmPreviousEvent = millis();
   
   // log
   Serial.println("");
   Serial.print("INFO:     api_bindSocket... returns ");
   
   // arm callback
   fsm_setCallback(&IpMtWrapper::api_bindSocket_reply);
   
   // issue function
   err = dn_ipmt_bindSocket(
      app_vars.socketId,                              // socketId
      app_vars.srcPort,                               // port
      (dn_ipmt_bindSocket_rpt*)(app_vars.replyBuf)    // reply
   );
   
   // log
   Serial.println(err);
   
   // schedule timeout event
   fsm_scheduleEvent(SERIAL_RESPONSE_TIMEOUT, &IpMtWrapper::api_response_timeout);
}

void IpMtWrapper::api_bindSocket_reply() {
   dn_ipmt_bindSocket_rpt* reply;
   
   // cancel timeout
   fsm_cancelEvent();
   
   // record time
   app_vars.fsmPreviousEvent = millis();
   
   Serial.println("INFO:     api_bindSocket_reply");
   
   reply = (dn_ipmt_bindSocket_rpt*)app_vars.replyBuf;
   
   Serial.print("INFO:     RC=");
   Serial.println(reply->RC);
   
   // schedule next event
   fsm_scheduleEvent(CMD_PERIOD, &IpMtWrapper::api_join);
}

//===== join

void IpMtWrapper::api_join(void) {
   dn_err_t err;
   
   // record time
   app_vars.fsmPreviousEvent = millis();
   
   // log
   Serial.println("");
   Serial.print("INFO:     api_join... returns ");
   
   // arm callback
   fsm_setCallback(&IpMtWrapper::api_join_reply);
   
   // issue function
   err = dn_ipmt_join(
      (dn_ipmt_join_rpt*)(app_vars.replyBuf)     // reply
   );
   
   // log
   Serial.println(err);
   
   // schedule timeout event
   fsm_scheduleEvent(SERIAL_RESPONSE_TIMEOUT, &IpMtWrapper::api_response_timeout);
}

void IpMtWrapper::api_join_reply() {
   dn_ipmt_join_rpt* reply;
   
   // cancel timeout
   fsm_cancelEvent();
   
   // record time
   app_vars.fsmPreviousEvent = millis();
   
   Serial.println("INFO:     api_join_reply");
   
   reply = (dn_ipmt_join_rpt*)app_vars.replyBuf;
   
   Serial.print("INFO:     RC=");
   Serial.println(reply->RC);
}

//===== getServiceInfo

void IpMtWrapper::api_getServiceInfo(void) {
   dn_err_t err;
   
   // record time
   app_vars.fsmPreviousEvent = millis();
   
   // log
   Serial.println("");
   Serial.print("INFO:     api_getServiceInfo... returns ");
   
   // arm callback
   fsm_setCallback(&IpMtWrapper::api_getServiceInfo_reply);
   
   // issue function
   err = dn_ipmt_getServiceInfo(
      0xfffe,                                              // destAddr (0xfffe==manager)
      SERVICE_TYPE_BW,                                     // type
      (dn_ipmt_getServiceInfo_rpt*)(app_vars.replyBuf)     // reply
   );
   
   // log
   Serial.println(err);
   
   // schedule timeout event
   fsm_scheduleEvent(SERIAL_RESPONSE_TIMEOUT, &IpMtWrapper::api_response_timeout);
}

void IpMtWrapper::api_getServiceInfo_reply() {
   dn_ipmt_getServiceInfo_rpt* reply;
   
   // cancel timeout
   fsm_cancelEvent();
   
   // record time
   app_vars.fsmPreviousEvent = millis();
   
   Serial.println("INFO:     api_getServiceInfo_reply");
   
   reply = (dn_ipmt_getServiceInfo_rpt*)app_vars.replyBuf;
   
   Serial.print("INFO:     RC=");
   Serial.println(reply->RC);
   
   Serial.print("INFO:     value=");
   Serial.println(reply->value);
   
   // schedule next event
   if (reply->RC!=0 || reply->value>app_vars.dataPeriod) {
      fsm_scheduleEvent(CMD_PERIOD, &IpMtWrapper::api_requestService);
   } else {
      fsm_scheduleEvent(CMD_PERIOD, &IpMtWrapper::api_sendTo);
   }
}

//===== requestService

void IpMtWrapper::api_requestService(void) {
   dn_err_t err;
   
   // record time
   app_vars.fsmPreviousEvent = millis();
   
   // log
   Serial.println("");
   Serial.print("INFO:     api_requestService... returns ");
   
   // arm callback
   fsm_setCallback(&IpMtWrapper::api_requestService_reply);
   
   // issue function
   err = dn_ipmt_requestService(
      0xfffe,                                              // destAddr (0xfffe==manager)
      SERVICE_TYPE_BW,                                     // serviceType
      app_vars.dataPeriod,                                 // value
      (dn_ipmt_requestService_rpt*)(app_vars.replyBuf)     // reply
   );
   
   // log
   Serial.println(err);
   
   // schedule timeout event
   fsm_scheduleEvent(SERIAL_RESPONSE_TIMEOUT, &IpMtWrapper::api_response_timeout);
}

void IpMtWrapper::api_requestService_reply() {
   dn_ipmt_requestService_rpt* reply;
   
   // cancel timeout
   fsm_cancelEvent();
   
   // record time
   app_vars.fsmPreviousEvent = millis();
   
   Serial.println("INFO:     api_requestService_reply");
   
   reply = (dn_ipmt_requestService_rpt*)app_vars.replyBuf;
   
   Serial.print("INFO:     RC=");
   Serial.println(reply->RC);
}

//===== sendTo

void IpMtWrapper::api_sendTo(void) {
   dn_err_t err;
   uint16_t dataVal;
   uint8_t  payload[2];
   uint8_t  lenWritten;
   
   // record time
   app_vars.fsmPreviousEvent = millis();
   
   // log
   Serial.println("");
   Serial.print("INFO:     api_sendTo... returns ");
   
   // arm callback
   fsm_setCallback(&IpMtWrapper::api_sendTo_reply);
   
   // create payload
   app_vars.dataGenerator(&dataVal);
   dn_write_uint16_t(payload, dataVal);
   
   // issue function
   err = dn_ipmt_sendTo(
      app_vars.socketId,                                   // socketId
      app_vars.destAddr,                                   // destIP
      app_vars.destPort,                                   // destPort
      SERVICE_TYPE_BW,                                     // serviceType
      0,                                                   // priority
      0xffff,                                              // packetId
      payload,                                             // payload
      sizeof(payload),                                     // payloadLen
      (dn_ipmt_sendTo_rpt*)(app_vars.replyBuf)             // reply
   );
   
   // log
   Serial.println(err);
   
   Serial.print("INFO:     sending value: ");
   Serial.println(dataVal);
   
   // schedule timeout event
   fsm_scheduleEvent(SERIAL_RESPONSE_TIMEOUT, &IpMtWrapper::api_response_timeout);
}

void IpMtWrapper::api_sendTo_reply() {
   dn_ipmt_sendTo_rpt* reply;
   
   // cancel timeout
   fsm_cancelEvent();
   
   // record time
   app_vars.fsmPreviousEvent = millis();
   
   Serial.println("INFO:     api_sendTo_reply");
   
   reply = (dn_ipmt_sendTo_rpt*)app_vars.replyBuf;
   
   Serial.print("INFO:     RC=");
   Serial.println(reply->RC);
   
   // schedule next transmission
   fsm_scheduleEvent(app_vars.dataPeriod, &IpMtWrapper::api_sendTo);
}

//=========================== helpers =========================================

void IpMtWrapper::printState(uint8_t state) {
   switch (state) {
      case MOTE_STATE_IDLE:
         Serial.println("MOTE_STATE_IDLE");
         break;
      case MOTE_STATE_SEARCHING:
         Serial.println("MOTE_STATE_SEARCHING");
         break;
      case MOTE_STATE_NEGOCIATING:
         Serial.println("MOTE_STATE_NEGOCIATING");
         break;
      case MOTE_STATE_CONNECTED:
         Serial.println("MOTE_STATE_CONNECTED");;
         break;
      case MOTE_STATE_OPERATIONAL:
         Serial.println("MOTE_STATE_OPERATIONAL");
         break;
      default:
         Serial.println("<unknown>");
         break;
   }
}

void IpMtWrapper::printByteArray(uint8_t* payload, uint8_t length) {
   uint8_t i;
   
   Serial.print(" ");
   for (i=0;i<length;i++) {
      Serial.print(payload[i]);
      if (i<length-1) {
         Serial.print("-");
      }
   }
   Serial.print("\n");
}

extern "C" void printBuf(uint8_t* buf, uint8_t bufLen) {
   uint8_t i;
   
   Serial.print(" ");
   for (i=0;i<bufLen;i++) {
      Serial.print(buf[i]);
      if (i<bufLen-1) {
         Serial.print("-");
      }
   }
   Serial.print("\n");
}

extern "C" void printPoi() {
   Serial.println("poipoi");
}

//=========================== callback functions for ipmt =====================

void dn_ipmt_notif_cb(uint8_t cmdId, uint8_t subCmdId) {
   
   dn_ipmt_events_nt* dn_ipmt_events_notif;
   
   switch (cmdId) {
      case CMDID_EVENTS:
         
         Serial.println("");
         Serial.println("INFO:     notif CMDID_EVENTS");
         
         dn_ipmt_events_notif = (dn_ipmt_events_nt*)app_vars.notifBuf;
         
         Serial.print("INFO:     state=");
         ipmtwrapper.printState(dn_ipmt_events_notif->state);
         
         switch (dn_ipmt_events_notif->state) {
            case MOTE_STATE_IDLE:
               ipmtwrapper.fsm_scheduleEvent(CMD_PERIOD,&IpMtWrapper::api_getMoteStatus);
               break;
            case MOTE_STATE_OPERATIONAL:
               ipmtwrapper.fsm_scheduleEvent(CMD_PERIOD,&IpMtWrapper::api_getServiceInfo);
               break;
            default:
               // nothing to do
               break;
         }
         break;
      default:
         // nothing to do
         break;
   }
}

extern "C" void dn_ipmt_reply_cb(uint8_t cmdId) {
   (ipmtwrapper.*app_vars.replyCb)();
}

//=========================== port to Arduino =================================

//===== definition of interface declared in uart.h

extern "C" void dn_uart_init(dn_uart_rxByte_cbt rxByte_cb) {
   // remember function to call back
   app_vars.ipmt_uart_rxByte_cb = rxByte_cb;
   
   // open the serial 1 port on the Arduino Due
   Serial1.begin(BAUDRATE_API);
}

extern "C" void dn_uart_txByte(uint8_t byte) {
   // write to the serial 1 port on the Arduino Due
   Serial1.write(byte);
}

extern "C" void dn_uart_txFlush() {
   // nothing to do since Arduino Due serial 1 driver is byte-oriented
}

//===== definition of interface declared in lock.h

extern "C" void dn_lock() {
   // this sample Arduino code is single threaded, no need to lock.
}

extern "C" void dn_unlock() {
   // this sample Arduino code is single threaded, no need to lock.
}

//===== definition of interface declared in endianness.h

extern "C" void dn_write_uint16_t(uint8_t* ptr, uint16_t val) {
   // arduino Due is a little-endian platform
   ptr[0]     = (val>>8)  & 0xff;
   ptr[1]     = (val>>0)  & 0xff;
}

extern "C" void dn_write_uint32_t(uint8_t* ptr, uint32_t val) {
   // arduino Due is a little-endian platform
   ptr[0]     = (val>>24) & 0xff;
   ptr[1]     = (val>>16) & 0xff;
   ptr[2]     = (val>>8)  & 0xff;
   ptr[3]     = (val>>0)  & 0xff;
}

extern "C" void dn_read_uint16_t(uint16_t* to, uint8_t* from) {
   // arduino Due is a little endian platform
   *to        = 0;
   *to       |= (from[1]<<0);
   *to       |= (from[0]<<8);
}
extern "C" void dn_read_uint32_t(uint32_t* to, uint8_t* from) {
   // arduino Due is a little endian platform
   *to        = 0;
   *to       |= (from[3]<<0);
   *to       |= (from[2]<<8);
   *to       |= (from[1]<<16);
   *to       |= (from[0]<<24);
}
