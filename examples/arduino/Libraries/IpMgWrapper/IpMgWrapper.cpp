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
#include "IpMgWrapper.h"
#include "dn_ipmg.h"
#include "dn_uart.h"
#include "dn_serial_mg.h"

//=========================== define ==========================================

//#define DEBUG_PRINT

//=========================== prototypes ======================================

#ifdef __cplusplus
 extern "C" {
#endif

void dn_ipmg_notif_cb(uint8_t cmdId, uint8_t subCmdId);
void dn_ipmg_reply_cb(uint8_t cmdId);
void dn_ipmg_status_cb(uint8_t newStatus);

#ifdef __cplusplus
}
#endif

//=========================== variables =======================================

extern IpMgWrapper ipmgwrapper;

typedef struct {
   // fsm
   TIME_T               fsmPreviousEvent;
   bool                 fsmArmed;
   TIME_T               fsmDelay;
   fsm_timer_callback   fsmCb;
   // module
   dn_uart_rxByte_cbt   ipmg_uart_rxByte_cb;
   // reply
   fsm_reply_callback   replyCb;
   // api
   uint8_t              currentMac[8];
   uint8_t              replyBuf[MAX_FRAME_LENGTH];        // holds notifications from ipmg
   uint8_t              notifBuf[MAX_FRAME_LENGTH];        // notifications buffer internal to ipmg
   uint8_t              ledState;
} app_vars_t;

app_vars_t              app_vars;

//=========================== public ==========================================

/**
\brief Constructor.
*/
IpMgWrapper::IpMgWrapper() {
}

/**
\brief Setting up the instance.
*/
void IpMgWrapper::setup() {
   // reset local variables
   memset(&app_vars,    0, sizeof(app_vars));
   
   // initialize variables
   app_vars.ledState = 1;
   
   // store params
   
   // initialize the serial port connected to the computer
   Serial.begin(BAUDRATE_CLI);
   
   // initialize the ipmg module
   dn_ipmg_init(
      dn_ipmg_notif_cb,                // notifCb
      app_vars.notifBuf,               // notifBuf
      sizeof(app_vars.notifBuf),       // notifBufLen
      dn_ipmg_reply_cb,                // replyCb
      dn_ipmg_status_cb                // statusCb
   );
   
   // print banner
   Serial.print("IpMgWrapper Library, version ");
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
   fsm_scheduleEvent(
      CMD_PERIOD,
      &IpMgWrapper::api_initiateConnect
   );
}

void IpMgWrapper::loop() {
   uint8_t byte;
   TIME_T  currentTime;
   
   // receive and react to HDLC frames
   while (Serial1.available()) {
      
      // read a serial byte
      byte = uint8_t(Serial1.read());
      
      // hand over byte to ipmg module
      app_vars.ipmg_uart_rxByte_cb(byte);
   }
   
   // kick the fsm
   currentTime = millis();
   if (app_vars.fsmArmed==TRUE && (currentTime-app_vars.fsmPreviousEvent>app_vars.fsmDelay)) {
      // cancel event
      app_vars.fsmArmed=FALSE;
      
      // handle event (by calling the callback)
      (this->*app_vars.fsmCb)();
   }
}

//=========================== private =========================================

//===== fsm

void IpMgWrapper::fsm_scheduleEvent(uint16_t delay, fsm_timer_callback cb) {
   app_vars.fsmArmed         = TRUE;
   app_vars.fsmPreviousEvent = millis();
   app_vars.fsmDelay         = delay;
   app_vars.fsmCb            = cb;
}

void IpMgWrapper::fsm_setCallback(fsm_reply_callback cb) {
   app_vars.replyCb     = cb;
}

void IpMgWrapper::api_response_timeout(void) {
      
   // log
   Serial.print("ERROR:    serial timeout!");
   
   // issue cancel command
   dn_ipmg_cancelTx();
   
   // schedule first event
   fsm_scheduleEvent(
      BACKOFF_AFTER_TIMEOUT,
      &IpMgWrapper::api_initiateConnect
   );
}

void IpMgWrapper::oap_response_timeout(void) {
      
   // log
   Serial.print("\nERROR:    OAP timeout!\n");
   
   // schedule first event
   fsm_scheduleEvent(
      BACKOFF_AFTER_TIMEOUT,
      &IpMgWrapper::api_getNextMoteConfig
   );
}

//===== api_initiateConnect

void IpMgWrapper::api_initiateConnect(void) {
   dn_err_t err;
      
   // log
   Serial.println("");
   Serial.print("INFO:     dn_ipmg_initiateConnect... returns ");
   
   // schedule
   err = dn_ipmg_initiateConnect();
   
   // log
   Serial.println(err);
   
   // schedule timeout event
   fsm_scheduleEvent(
      SERIAL_RESPONSE_TIMEOUT,
      &IpMgWrapper::api_response_timeout
   );
}

//===== api_subscribe

void IpMgWrapper::api_subscribe(void) {
   dn_err_t err;
      
   // log
#ifdef DEBUG_PRINT
   Serial.println("");
   Serial.print("INFO:     api_subscribe... returns ");
#endif
   
   // arm callback
   fsm_setCallback(&IpMgWrapper::api_subscribe_reply);
   
   // issue function
   err = dn_ipmg_subscribe(
      SUBSC_FILTER_DATA,                              // filter
      0x00000000,                                     // unackFilter
      (dn_ipmg_subscribe_rpt*)(app_vars.replyBuf)     // reply
   );
   
   // log
#ifdef DEBUG_PRINT
   Serial.println(err);
#endif
   
   // schedule timeout event
   fsm_scheduleEvent(
      SERIAL_RESPONSE_TIMEOUT,
      &IpMgWrapper::api_response_timeout
   );
}

void IpMgWrapper::api_subscribe_reply() {
   dn_ipmg_subscribe_rpt* reply;
      
   // log
#ifdef DEBUG_PRINT
   Serial.println("INFO:     api_subscribe_reply");
#endif
   
   // cast reply
   reply = (dn_ipmg_subscribe_rpt*)app_vars.replyBuf;
   
   // log
#ifdef DEBUG_PRINT
   Serial.print("INFO:     RC=");
   Serial.println(reply->RC);
#endif
   
   // schedule next event
   ipmgwrapper.fsm_scheduleEvent(
      CMD_PERIOD,
      &IpMgWrapper::api_getNextMoteConfig
   );
}

//===== api_getNextMoteConfig

void IpMgWrapper::api_getNextMoteConfig(void) {
   dn_err_t err;
      
   // log
#ifdef DEBUG_PRINT
   Serial.println("");
   Serial.print("INFO:     api_getNextMoteConfig MAC=");
   printByteArray(app_vars.currentMac,sizeof(app_vars.currentMac));
   Serial.print("... returns ");
#endif
   
   // arm callback
   fsm_setCallback(&IpMgWrapper::api_getNextMoteConfig_reply);
   
   // issue function
   err = dn_ipmg_getMoteConfig(
      app_vars.currentMac,                            // macAddress
      TRUE,                                           // next
      (dn_ipmg_getMoteConfig_rpt*)(app_vars.replyBuf) // reply
   );
   
   // log
#ifdef DEBUG_PRINT
   Serial.println(err);
#endif
   
   // schedule timeout event
   fsm_scheduleEvent(
      SERIAL_RESPONSE_TIMEOUT,
      &IpMgWrapper::api_response_timeout
   );
}

void IpMgWrapper::api_getNextMoteConfig_reply() {
   dn_ipmg_getMoteConfig_rpt* reply;
      
   // log
#ifdef DEBUG_PRINT
   Serial.println("INFO:     api_getNextMoteConfig_reply");
#endif
   
   // cast reply
   reply = (dn_ipmg_getMoteConfig_rpt*)app_vars.replyBuf;
   
   // log
#ifdef DEBUG_PRINT
   Serial.print("INFO:     RC=");
   Serial.println(reply->RC);
   Serial.print("INFO:     MAC=");
   printByteArray(reply->macAddress,sizeof(reply->macAddress));
   Serial.print("\n");
   Serial.print("INFO:     state=");
   this->printState(reply->state);
   Serial.print("INFO:     isAP=");
   Serial.println(reply->isAP);
#endif
   
   if (reply->RC==0) {
      // remember current MAC
      memcpy(app_vars.currentMac,reply->macAddress,sizeof(reply->macAddress));
      
      if (reply->state==MOTE_STATE_OPERATIONAL && reply->isAP==FALSE) {
         // switch LED on
         
         app_vars.ledState=1;
         
         ipmgwrapper.fsm_scheduleEvent(
            CMD_PERIOD,
            &IpMgWrapper::api_toggleLed
         );
      } else {
         // schedule next event
         ipmgwrapper.fsm_scheduleEvent(
            CMD_PERIOD,
            &IpMgWrapper::api_getNextMoteConfig
         );
      }
   } else {
      // end of list or problem reading
      
      // reset next mote to ask
      memset(app_vars.currentMac,0,sizeof(app_vars.currentMac));
      
      // schedule next event
      ipmgwrapper.fsm_scheduleEvent(
         CMD_PERIOD,
         &IpMgWrapper::api_getNextMoteConfig
      );
   }
}

//===== api_toggleLed

void IpMgWrapper::api_toggleLed(void) {
   dn_err_t   err;
   uint8_t    payload[10];
   
   // log
#ifdef DEBUG_PRINT
   Serial.println("");
   Serial.print("INFO:     api_toggleLed MAC=");
   printByteArray(app_vars.currentMac,sizeof(app_vars.currentMac));
   Serial.print("... returns ");
#endif
   
   // arm callback
   fsm_setCallback(&IpMgWrapper::api_toggleLed_reply);
   
   // prepare OAP payload
   payload[0] = (1<<0) | (0<<1) | (1<<2);        // control byte (ACKed|request|resync)
   payload[1] = 0x00;                            // ID
   payload[2] = 0x02;                            // command (0x02==PUT)
   payload[3] = 0xff;                            // (T) address
   payload[4] = 2;                               // (L) address
   payload[5] = 3;                               // (V) address
   payload[6] = 2;                               // (V) address
   payload[7] = 0;                               // (T) value
   payload[8] = 1;                               // (L) value
   payload[9] = app_vars.ledState;               // (V) value
   
   // issue function
   err = dn_ipmg_sendData(
      app_vars.currentMac,                       // macAddress
      0x00,                                      // priority
      DN_UDP_PORT_OAP,                           // srcPort
      DN_UDP_PORT_OAP,                           // dstPort
      0x00,                                      // options
      payload,                                   // data
      sizeof(payload),                           // dataLen
      (dn_ipmg_sendData_rpt*)(app_vars.replyBuf) // reply
   );
   
   // log
#ifdef DEBUG_PRINT
   Serial.println(err);
#endif
   
   // schedule timeout event
   fsm_scheduleEvent(
      SERIAL_RESPONSE_TIMEOUT,
      &IpMgWrapper::api_response_timeout
   );
}

void IpMgWrapper::api_toggleLed_reply() {
   dn_ipmg_sendData_rpt* reply;
   
   // log
#ifdef DEBUG_PRINT
   Serial.println("INFO:     api_toggleLed_reply");
#endif
   
   // cast reply
   reply = (dn_ipmg_sendData_rpt*)app_vars.replyBuf;
   
   // log
#ifdef DEBUG_PRINT
   Serial.print("INFO:     RC=");
   Serial.println(reply->RC);
   Serial.print("INFO:     callbackId=");
   Serial.println(reply->callbackId);
#endif
   
   // schedule timeout event
   fsm_scheduleEvent(
      OAP_RESPONSE_TIMEOUT,
      &IpMgWrapper::oap_response_timeout
   );
}

//=========================== helpers =========================================

void IpMgWrapper::printState(uint8_t state) {
   switch (state) {
      case MOTE_STATE_LOST:
         Serial.println("MOTE_STATE_LOST");
         break;
      case MOTE_STATE_NEGOTIATING:
         Serial.println("MOTE_STATE_NEGOTIATING");
         break;
      case MOTE_STATE_OPERATIONAL:
         Serial.println("MOTE_STATE_OPERATIONAL");
         break;
      default:
         Serial.println("<unknown>");
         break;
   }
}

void IpMgWrapper::printByteArray(uint8_t* payload, uint8_t length) {
   uint8_t i;
   
   Serial.print(" ");
   for (i=0;i<length;i++) {
      Serial.print(payload[i]);
      if (i<length-1) {
         Serial.print("-");
      }
   }
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

//=========================== callback functions for ipmg =====================

void dn_ipmg_notif_cb(uint8_t cmdId, uint8_t subCmdId) {
   dn_ipmg_notifData_nt* notifData_notif;
   
   if (cmdId==DN_NOTIFID_NOTIFDATA) {
      
      notifData_notif = (dn_ipmg_notifData_nt*)app_vars.notifBuf;
      
      if (
         memcmp(notifData_notif->macAddress,app_vars.currentMac,8)==0     &&
         notifData_notif->srcPort==DN_UDP_PORT_OAP                        &&
         notifData_notif->dstPort==DN_UDP_PORT_OAP                        &&
         notifData_notif->data[0]==7                                      && // 7==b111 acknowledged, response,resync 
         notifData_notif->data[2]==2                                      && // 2==PUT
         notifData_notif->data[3]==0                                      && // RC
         notifData_notif->data[4]==255                                    && // (T) address 
         notifData_notif->data[5]==2                                      && // (L) address 
         notifData_notif->data[6]==3                                      && // (V) address 
         notifData_notif->data[7]==2                                         // (V) address 
      ) {
         // response from mote
         
         // log
#ifdef DEBUG_PRINT
         Serial.println("INFO:     toggleLed confirmation");
#endif
         
         if (app_vars.ledState==1) {
            // switch LED off
            
            app_vars.ledState=0;
            
            ipmgwrapper.fsm_scheduleEvent(
               INTER_FRAME_PERIOD,
               &IpMgWrapper::api_toggleLed
            );
         } else {
            // move to next mote
            
            ipmgwrapper.fsm_scheduleEvent(
               INTER_FRAME_PERIOD,
               &IpMgWrapper::api_getNextMoteConfig
            );
         }
      }
   }
}

extern "C" void dn_ipmg_reply_cb(uint8_t cmdId) {
   (ipmgwrapper.*app_vars.replyCb)();
}

extern "C" void dn_ipmg_status_cb(uint8_t newStatus) {
   switch (newStatus) {
      case DN_SERIAL_ST_CONNECTED:
         
         // log
         Serial.println("INFO:     connected");
         
         // schedule next event
         ipmgwrapper.fsm_scheduleEvent(
            INTER_FRAME_PERIOD,
            &IpMgWrapper::api_subscribe
         );
         break;
      case DN_SERIAL_ST_DISCONNECTED:
         // log
         Serial.println("WARNING:  disconnected");
         
         // schedule first event
         ipmgwrapper.fsm_scheduleEvent(
            INTER_FRAME_PERIOD,
            &IpMgWrapper::api_initiateConnect
         );
         
         break;
      default:
         Serial.println("ERROR:    unexpected newStatus");
   }
}

//=========================== port to Arduino =================================

//===== definition of interface declared in uart.h

extern "C" void dn_uart_init(dn_uart_rxByte_cbt rxByte_cb) {
   // remember function to call back
   app_vars.ipmg_uart_rxByte_cb = rxByte_cb;
   
   // open the serial 1 port on the Arduino Due
   Serial1.begin(BAUDRATE_API);
}

extern "C" void dn_uart_txByte(uint8_t byte) {
   // write to the serial 1 port on the Arduino Due
   Serial1.write(byte);
   Serial1.flush();
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
