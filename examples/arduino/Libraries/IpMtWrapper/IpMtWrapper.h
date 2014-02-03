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

#ifndef IPMTWRAPPER_H
#define IPMTWRAPPER_H

#include <Arduino.h>
#include "dn_ipmt.h"

//=========================== defines =========================================

#define TIME_T                    unsigned long

#define BAUDRATE_CLI              9600
#define BAUDRATE_API              115200

#define TRUE                      0x01
#define FALSE                     0x00

// mote state
#define MOTE_STATE_IDLE           0x01
#define MOTE_STATE_SEARCHING      0x02
#define MOTE_STATE_NEGOCIATING    0x03
#define MOTE_STATE_CONNECTED      0x04
#define MOTE_STATE_OPERATIONAL    0x05

// service types
#define SERVICE_TYPE_BW           0x00

#define IPv6ADDR_LEN              16
#define DEFAULT_SOCKETID          22

//===== fsm

#define CMD_PERIOD                1000      // number of ms between two commands being sent
#define SERIAL_RESPONSE_TIMEOUT    500      // max number of ms to wait for response

//=========================== typedef =========================================

class IpMtWrapper; // forward declaration needed here

typedef void (IpMtWrapper::*fsm_timer_callback)(void);
typedef void (IpMtWrapper::*fsm_reply_callback)(void);
typedef void (*data_generator)(uint16_t* returnVal);

//=========================== IpMtWrapper object ==============================

class IpMtWrapper {
   public:
      //===== methods
      IpMtWrapper();
      void                   setup(
         uint16_t            srcPort,
         uint8_t*            destAddr,
         uint16_t            destPort,
         TIME_T              dataPeriod,
         data_generator      dataGenerator
      );
      void                   loop();
      //===== attributes
      //===== methods
      //=== fsm
      void                   fsm_scheduleEvent(uint16_t delay, fsm_timer_callback cb);
      void                   fsm_cancelEvent();
      void                   fsm_setCallback(fsm_reply_callback cb);
      //=== api
      void                   api_response_timeout();
      void                   api_getMoteStatus();
      void                   api_getMoteStatus_reply();
      void                   api_openSocket();
      void                   api_openSocket_reply();
      void                   api_bindSocket();
      void                   api_bindSocket_reply();
      void                   api_join();
      void                   api_join_reply();
      void                   api_getServiceInfo();
      void                   api_getServiceInfo_reply();
      void                   api_requestService();
      void                   api_requestService_reply();
      void                   api_sendTo();
      void                   api_sendTo_reply();
      //=== helpers
      void                   printState(uint8_t state);
      void                   printByteArray(uint8_t* payload, uint8_t length);
   private:
      //===== attributes
      
};

#endif
