/*
Copyright (c) 2014, Dust Networks.  All rights reserved.

Arduino library to connect to a SmartMesh IP manager.

This library is an Arduino "wrapper" around the generic SmartMesh C library.

This library will:
- Connect to the SmartMesh IP manager.
- Subscribe to data notifications.
- Get the MAC address of all nodes in the network.
- Send an OAP command to blink each node's LED in a round-robin fashion.
  
\license See attached DN_LICENSE.txt.
*/

#ifndef IPMGWRAPPER_H
#define IPMGWRAPPER_H

#include <Arduino.h>
#include "dn_ipmt.h"

//=========================== defines =========================================

#define TIME_T                    unsigned long

#define BAUDRATE_CLI              9600
#define BAUDRATE_API              115200

#define TRUE                      0x01
#define FALSE                     0x00

// mote state
#define MOTE_STATE_LOST           0x00
#define MOTE_STATE_NEGOTIATING    0x01
#define MOTE_STATE_OPERATIONAL    0x04

// service types
#define SERVICE_TYPE_BW           0x00

#define IPv6ADDR_LEN              16
#define DEFAULT_SOCKETID          22

// subscription
#define SUBSC_FILTER_EVENT        0x02
#define SUBSC_FILTER_LOG          0x02
#define SUBSC_FILTER_DATA         0x10
#define SUBSC_FILTER_IPDATA       0x20
#define SUBSC_FILTER_HR           0x40

#define DN_UDP_PORT_OAP           0xf0b9

//===== fsm

#define CMD_PERIOD                0          // number of ms between two commands being sent
#define INTER_FRAME_PERIOD        1          // min number of ms between two TX frames over serial port
#define SERIAL_RESPONSE_TIMEOUT   100        // max number of ms to wait for serial response
#define BACKOFF_AFTER_TIMEOUT     1000       // number of ms to back off after a timeout occurs
#define OAP_RESPONSE_TIMEOUT      10000      // max number of ms to wait for OAP response

//=========================== typedef =========================================

class IpMgWrapper; // forward declaration needed here

typedef void (IpMgWrapper::*fsm_timer_callback)(void);
typedef void (IpMgWrapper::*fsm_reply_callback)(void);
typedef void (*data_generator)(uint16_t* returnVal);

//=========================== IpMgWrapper object ==============================

class IpMgWrapper {
   public:
      //===== methods
      IpMgWrapper();
      void                   setup();
      void                   loop();
      //===== attributes
      //===== methods
      //=== fsm
      void                   fsm_scheduleEvent(
         uint16_t            delay,
         fsm_timer_callback  cb
      );
      void                   fsm_setCallback(fsm_reply_callback cb);
      //=== api
      void                   api_response_timeout();
      void                   oap_response_timeout();
      void                   api_initiateConnect();
      void                   api_subscribe();
      void                   api_subscribe_reply();
      void                   api_getNextMoteConfig();
      void                   api_getNextMoteConfig_reply();
      void                   api_toggleLed();
      void                   api_toggleLed_reply();
      //=== helpers
      void                   printState(uint8_t state);
      void                   printByteArray(uint8_t* payload, uint8_t length);
   private:
      //===== attributes
      
};

#endif
