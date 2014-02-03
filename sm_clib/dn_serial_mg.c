/*
Copyright (c) 2014, Dust Networks. All rights reserved.

HDLC library.

\license See attached DN_LICENSE.txt.
*/

#include "dn_serial_mg.h"
#include "dn_hdlc.h"

//=========================== variables =======================================

typedef struct {
   // admin
   uint8_t                   status;
   // packet IDs
   uint8_t                   txPacketId;
   bool                      rxPacketIdInit;
   uint8_t                   rxPacketId;
   // reply callback
   uint8_t                   replyCmdId;
   dn_serial_reply_cbt       replyCb;
   // callbacks
   dn_serial_request_cbt     requestCb;
   dn_status_change_cbt      statusChangeCb;
} dn_serial_mg_vars_t;

dn_serial_mg_vars_t dn_serial_mg_vars;

//=========================== prototype =======================================

dn_err_t dn_serial_sendRequestNoCheck(
   uint8_t              cmdId,
   bool                 isAck,
   bool                 shouldBeAcked,
   uint8_t*             payload,
   uint8_t              length,
   dn_serial_reply_cbt  replyCb
);
void     dn_serial_mg_rxHdlcFrame(uint8_t* rxFrame, uint8_t rxFrameLen);
void     dn_serial_mg_dispatch_response(uint8_t cmdId, uint8_t *payload, uint8_t length);

//=========================== public ==========================================

/**
\brief Setting up the instance.
*/
void dn_serial_mg_init(dn_serial_request_cbt requestCb, dn_status_change_cbt statusChangeCb) {
   // reset local variables
   memset(&dn_serial_mg_vars, 0, sizeof(dn_serial_mg_vars));
   
   // initialize variables
   dn_serial_mg_vars.txPacketId      = 0x00;
   dn_serial_mg_vars.rxPacketIdInit  = FALSE;
   dn_serial_mg_vars.rxPacketId      = 0x00;
   
   dn_serial_mg_vars.requestCb       = requestCb;
   dn_serial_mg_vars.statusChangeCb  = statusChangeCb;
   
   // initialize the HDLC module
   dn_hdlc_init(dn_serial_mg_rxHdlcFrame);
}

dn_err_t dn_serial_mg_initiateConnect() {
   uint8_t payload[3];
   
   // prepare hello packet
   payload[0] = DN_API_VERSION;             // version
   payload[1] = dn_serial_mg_vars.txPacketId;  // cliSeqNo
   payload[2] = 0;                          // mode
   
   // send hello packet
   dn_serial_sendRequestNoCheck(
      SERIAL_CMID_HELLO,     // cmdId
      FALSE,                 // isAck
      FALSE,                 // shouldBeAcked
      payload,               // payload
      sizeof(payload),       // length
      NULL                   // replyCb
   );
   
   // remember state
   dn_serial_mg_vars.status = DN_SERIAL_ST_HELLO_SENT;
   
   return DN_ERR_NONE;
}

dn_err_t dn_serial_mg_sendRequest(uint8_t cmdId, bool isAck, uint8_t* payload, uint8_t length, dn_serial_reply_cbt replyCb) {
   // abort if not connected
   if (dn_serial_mg_vars.status!=DN_SERIAL_ST_CONNECTED) {
      return DN_ERR_NOT_CONNECTED;
   }
   
   // send the request
   return dn_serial_sendRequestNoCheck(
      cmdId,                 // cmdId
      isAck,                 // isAck
      !isAck,                // shouldBeAcked
      payload,               // payload
      length,                // length
      replyCb                // replyCb
   );
}

//=========================== private =========================================

dn_err_t dn_serial_sendRequestNoCheck(uint8_t cmdId, bool isAck, bool shouldBeAcked, uint8_t* payload, uint8_t length, dn_serial_reply_cbt replyCb) {
   uint8_t i;
   uint8_t control;
   
   // register reply callback
   dn_serial_mg_vars.replyCmdId      = cmdId;
   dn_serial_mg_vars.replyCb         = replyCb;
   
   // create the control byte
   control         = 0;
   if (isAck==1) {
      control     |= DN_SERIAL_FLAG_ACK;
   } else {
      control     |= DN_SERIAL_FLAG_DATA;
   }
   if (shouldBeAcked==1) {
      control     |= DN_SERIAL_FLAG_ACKNOWLEDGED;
   } else {
      control     |= DN_SERIAL_FLAG_UNACKNOWLEDGED;
   }
   
   // send the frame over serial
   dn_hdlc_outputOpen();
   dn_hdlc_outputWrite(control);                      // Control
   dn_hdlc_outputWrite(cmdId);                        // Packet Type
   dn_hdlc_outputWrite(dn_serial_mg_vars.txPacketId);    // Seq. Number
   dn_hdlc_outputWrite(length);                       // Payload Length
   for (i=0; i<length; i++) {                         // Payload
      dn_hdlc_outputWrite(payload[i]);
   }
   dn_hdlc_outputClose();
   
   // increment the txPacketId
   dn_serial_mg_vars.txPacketId++;
   
   return DN_ERR_NONE;
}

void dn_serial_mg_rxHdlcFrame(uint8_t* rxFrame, uint8_t rxFrameLen) {
   // fields in the serial API header
   uint8_t  control;
   uint8_t  cmdId;
   uint8_t  seqNum;
   uint8_t  length;
   uint8_t* payload;
   // misc
   uint8_t  isAck;
   uint8_t  shouldAck;
   uint8_t  isRepeatId;
   
   // assert length is OK
   if (rxFrameLen<4) {
      return;
   }
   
   // parse header
   control    =  rxFrame[0];
   cmdId      =  rxFrame[1];
   seqNum     =  rxFrame[2];
   length     =  rxFrame[3];
   payload    = &rxFrame[4];
   isAck      = ((control & DN_SERIAL_FLAG_ACK)!=0);
   shouldAck  = ((control & DN_SERIAL_FLAG_ACKNOWLEDGED)!=0);
   
   // check if valid packet ID
   if (isAck) {
      // ACK, dispatch
      
      if (length>0) {
         dn_serial_mg_dispatch_response(cmdId,payload,length);
      }
   } else {
      // DATA
      
      if (dn_serial_mg_vars.rxPacketIdInit==TRUE && seqNum==dn_serial_mg_vars.rxPacketId) {
         isRepeatId                         = TRUE;
      } else {
         isRepeatId                         = FALSE;
         dn_serial_mg_vars.rxPacketIdInit   = TRUE;
         dn_serial_mg_vars.rxPacketId       = seqNum;
      }
      
      // ACK
      if (shouldAck) {
         dn_hdlc_outputOpen();
         dn_hdlc_outputWrite(DN_SERIAL_FLAG_ACK | DN_SERIAL_FLAG_UNACKNOWLEDGED); // Control
         dn_hdlc_outputWrite(cmdId);                                              // Packet Type
         dn_hdlc_outputWrite(dn_serial_mg_vars.rxPacketId);                       // Seq. Number
         dn_hdlc_outputWrite(1);                                                  // Payload Length
         dn_hdlc_outputWrite(0);                                                  // Payload (RC==0x00)
         dn_hdlc_outputClose();
      }
      
      switch (cmdId) {
         case SERIAL_CMID_HELLO_RESPONSE:
            if (
                  length>=5 &&
                  payload[HELLO_RESP_OFFS_RC]      == 0 &&
                  payload[HELLO_RESP_OFFS_VERSION] == DN_API_VERSION
               ) {
               // change state
               dn_serial_mg_vars.status = DN_SERIAL_ST_CONNECTED;
               // record manager sequence number
               dn_serial_mg_vars.rxPacketIdInit     = TRUE;
               dn_serial_mg_vars.rxPacketId         = payload[HELLO_RESP_OFFS_MGRSEQNO];
               // indicate state change
               if (dn_serial_mg_vars.statusChangeCb) {
                  dn_serial_mg_vars.statusChangeCb(dn_serial_mg_vars.status);
               }
            };
            break;
         case SERIAL_CMID_MGR_HELLO:
            if (
                  length>=2
               ) {
               // change state
               dn_serial_mg_vars.status = DN_SERIAL_ST_DISCONNECTED;
               // indicate state change
               if (dn_serial_mg_vars.statusChangeCb) {
                   dn_serial_mg_vars.statusChangeCb(dn_serial_mg_vars.status);
               }
            }
            break;
         default:
            // dispatch
            if (length>0 && dn_serial_mg_vars.requestCb!=NULL && isRepeatId==FALSE) {
               dn_serial_mg_vars.requestCb(cmdId,control,payload,length);
            }
            break;
      }
   }
}

void dn_serial_mg_dispatch_response(uint8_t cmdId, uint8_t* payload, uint8_t length) {
   uint8_t rc;
   
   rc = payload[0];
   if (cmdId==dn_serial_mg_vars.replyCmdId && dn_serial_mg_vars.replyCb!=NULL) {
      
      // call the callback
      (dn_serial_mg_vars.replyCb)(cmdId,rc,&payload[1],length-1);
      
      // reset
      dn_serial_mg_vars.replyCmdId   = 0x00;
      dn_serial_mg_vars.replyCb      = NULL;
   }
}

//=========================== helpers =========================================
