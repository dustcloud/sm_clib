/*
Copyright (c) 2015, Dust Networks. All rights reserved.

C library to connect to a SmartMesh WirelessHART Mote.

\license See attached DN_LICENSE.txt.
*/

#include "dn_whmt.h"
#include "dn_lock.h"
#include "dn_serial_mt.h"

//=========================== variables =======================================

typedef struct {
   // sending requests
   uint8_t              outputBuf[MAX_FRAME_LENGTH];
   bool                 busyTx;
   uint8_t              cmdId;
   uint8_t              paramId;
   // receiving replies
   dn_whmt_reply_cbt    replyCb;
   uint8_t*             replyContents;
   // receiving notifications
   dn_whmt_notif_cbt    notifCb;
   uint8_t*             notifBuf;
   uint8_t              notifBufLen;
} dn_whmt_vars_t;

dn_whmt_vars_t dn_whmt_vars;

//=========================== prototypes ======================================

// API
void dn_whmt_setParameter_txPower_reply(uint8_t cmdId, uint8_t rc, uint8_t* payload, uint8_t len);
void dn_whmt_setParameter_joinDutyCycle_reply(uint8_t cmdId, uint8_t rc, uint8_t* payload, uint8_t len);
void dn_whmt_setParameter_batteryLife_reply(uint8_t cmdId, uint8_t rc, uint8_t* payload, uint8_t len);
void dn_whmt_setParameter_service_reply(uint8_t cmdId, uint8_t rc, uint8_t* payload, uint8_t len);
void dn_whmt_setParameter_hartDeviceStatus_reply(uint8_t cmdId, uint8_t rc, uint8_t* payload, uint8_t len);
void dn_whmt_setParameter_hartDeviceInfo_reply(uint8_t cmdId, uint8_t rc, uint8_t* payload, uint8_t len);
void dn_whmt_setParameter_eventMask_reply(uint8_t cmdId, uint8_t rc, uint8_t* payload, uint8_t len);
void dn_whmt_setParameter_writeProtect_reply(uint8_t cmdId, uint8_t rc, uint8_t* payload, uint8_t len);
void dn_whmt_setParameter_lock_reply(uint8_t cmdId, uint8_t rc, uint8_t* payload, uint8_t len);
void dn_whmt_getParameter_joinDutyCycle_reply(uint8_t cmdId, uint8_t rc, uint8_t* payload, uint8_t len);
void dn_whmt_getParameter_service_reply(uint8_t cmdId, uint8_t rc, uint8_t* payload, uint8_t len);
void dn_whmt_getParameter_moteInfo_reply(uint8_t cmdId, uint8_t rc, uint8_t* payload, uint8_t len);
void dn_whmt_getParameter_networkInfo_reply(uint8_t cmdId, uint8_t rc, uint8_t* payload, uint8_t len);
void dn_whmt_getParameter_moteStatus_reply(uint8_t cmdId, uint8_t rc, uint8_t* payload, uint8_t len);
void dn_whmt_getParameter_time_reply(uint8_t cmdId, uint8_t rc, uint8_t* payload, uint8_t len);
void dn_whmt_getParameter_charge_reply(uint8_t cmdId, uint8_t rc, uint8_t* payload, uint8_t len);
void dn_whmt_getParameter_testRadioRxStats_reply(uint8_t cmdId, uint8_t rc, uint8_t* payload, uint8_t len);
void dn_whmt_getParameter_lock_reply(uint8_t cmdId, uint8_t rc, uint8_t* payload, uint8_t len);
void dn_whmt_setNVParameter_macAddress_reply(uint8_t cmdId, uint8_t rc, uint8_t* payload, uint8_t len);
void dn_whmt_setNVParameter_joinKey_reply(uint8_t cmdId, uint8_t rc, uint8_t* payload, uint8_t len);
void dn_whmt_setNVParameter_networkId_reply(uint8_t cmdId, uint8_t rc, uint8_t* payload, uint8_t len);
void dn_whmt_setNVParameter_txPower_reply(uint8_t cmdId, uint8_t rc, uint8_t* payload, uint8_t len);
void dn_whmt_setNVParameter_powerInfo_reply(uint8_t cmdId, uint8_t rc, uint8_t* payload, uint8_t len);
void dn_whmt_setNVParameter_ttl_reply(uint8_t cmdId, uint8_t rc, uint8_t* payload, uint8_t len);
void dn_whmt_setNVParameter_HARTantennaGain_reply(uint8_t cmdId, uint8_t rc, uint8_t* payload, uint8_t len);
void dn_whmt_setNVParameter_OTAPlockout_reply(uint8_t cmdId, uint8_t rc, uint8_t* payload, uint8_t len);
void dn_whmt_setNVParameter_hrCounterMode_reply(uint8_t cmdId, uint8_t rc, uint8_t* payload, uint8_t len);
void dn_whmt_setNVParameter_autojoin_reply(uint8_t cmdId, uint8_t rc, uint8_t* payload, uint8_t len);
void dn_whmt_setNVParameter_compliantMode_reply(uint8_t cmdId, uint8_t rc, uint8_t* payload, uint8_t len);
void dn_whmt_setNVParameter_lock_reply(uint8_t cmdId, uint8_t rc, uint8_t* payload, uint8_t len);
void dn_whmt_getNVParameter_macAddress_reply(uint8_t cmdId, uint8_t rc, uint8_t* payload, uint8_t len);
void dn_whmt_getNVParameter_networkId_reply(uint8_t cmdId, uint8_t rc, uint8_t* payload, uint8_t len);
void dn_whmt_getNVParameter_txPower_reply(uint8_t cmdId, uint8_t rc, uint8_t* payload, uint8_t len);
void dn_whmt_getNVParameter_powerInfo_reply(uint8_t cmdId, uint8_t rc, uint8_t* payload, uint8_t len);
void dn_whmt_getNVParameter_ttl_reply(uint8_t cmdId, uint8_t rc, uint8_t* payload, uint8_t len);
void dn_whmt_getNVParameter_HARTantennaGain_reply(uint8_t cmdId, uint8_t rc, uint8_t* payload, uint8_t len);
void dn_whmt_getNVParameter_OTAPlockout_reply(uint8_t cmdId, uint8_t rc, uint8_t* payload, uint8_t len);
void dn_whmt_getNVParameter_hrCounterMode_reply(uint8_t cmdId, uint8_t rc, uint8_t* payload, uint8_t len);
void dn_whmt_getNVParameter_autojoin_reply(uint8_t cmdId, uint8_t rc, uint8_t* payload, uint8_t len);
void dn_whmt_getNVParameter_compliantMode_reply(uint8_t cmdId, uint8_t rc, uint8_t* payload, uint8_t len);
void dn_whmt_getNVParameter_lock_reply(uint8_t cmdId, uint8_t rc, uint8_t* payload, uint8_t len);
void dn_whmt_send_reply(uint8_t cmdId, uint8_t rc, uint8_t* payload, uint8_t len);
void dn_whmt_join_reply(uint8_t cmdId, uint8_t rc, uint8_t* payload, uint8_t len);
void dn_whmt_disconnect_reply(uint8_t cmdId, uint8_t rc, uint8_t* payload, uint8_t len);
void dn_whmt_reset_reply(uint8_t cmdId, uint8_t rc, uint8_t* payload, uint8_t len);
void dn_whmt_lowPowerSleep_reply(uint8_t cmdId, uint8_t rc, uint8_t* payload, uint8_t len);
void dn_whmt_hartPayload_reply(uint8_t cmdId, uint8_t rc, uint8_t* payload, uint8_t len);
void dn_whmt_testRadioTx_reply(uint8_t cmdId, uint8_t rc, uint8_t* payload, uint8_t len);
void dn_whmt_testRadioRx_reply(uint8_t cmdId, uint8_t rc, uint8_t* payload, uint8_t len);
void dn_whmt_clearNV_reply(uint8_t cmdId, uint8_t rc, uint8_t* payload, uint8_t len);
void dn_whmt_search_reply(uint8_t cmdId, uint8_t rc, uint8_t* payload, uint8_t len);
void dn_whmt_testRadioTxExt_reply(uint8_t cmdId, uint8_t rc, uint8_t* payload, uint8_t len);
void dn_whmt_testRadioRxExt_reply(uint8_t cmdId, uint8_t rc, uint8_t* payload, uint8_t len);
void dn_whmt_zeroize_reply(uint8_t cmdId, uint8_t rc, uint8_t* payload, uint8_t len);
void dn_whmt_fileWrite_reply(uint8_t cmdId, uint8_t rc, uint8_t* payload, uint8_t len);
void dn_whmt_fileRead_reply(uint8_t cmdId, uint8_t rc, uint8_t* payload, uint8_t len);
void dn_whmt_fileOpen_reply(uint8_t cmdId, uint8_t rc, uint8_t* payload, uint8_t len);

// serial RX
void dn_whmt_rxSerialRequest(uint8_t cmdId, uint8_t flags, uint8_t* payload, uint8_t len);

//=========================== public ==========================================

//========== admin

/**
\brief Setting up the instance.
*/
void dn_whmt_init(dn_whmt_notif_cbt notifCb, uint8_t* notifBuf, uint8_t notifBufLen, dn_whmt_reply_cbt replyCb) {
   
   // reset local variables
   memset(&dn_whmt_vars,0,sizeof(dn_whmt_vars));
   
   // store params
   dn_whmt_vars.notifCb         = notifCb;
   dn_whmt_vars.notifBuf        = notifBuf;
   dn_whmt_vars.notifBufLen     = notifBufLen;
   dn_whmt_vars.replyCb         = replyCb;
   
   // initialize the serial connection
   dn_serial_mt_init(dn_whmt_rxSerialRequest);
}

void dn_whmt_cancelTx() {
   
   // lock the module
   dn_lock();
   
   dn_whmt_vars.busyTx=FALSE;
   
   // unlock the module
   dn_unlock();
}



//========== API

//===== setParameter_txPower

/**
The setParameter<txPower> command sets the mote conducted RF output power. 
Refer to product datasheets for supported RF output power values. For example, 
if the mote has a typical RF output power of +8 dBm when the Power Amplifier 
(PA) is enabled, set the txPower parameter to 8 to enable the PA. Similarly, if 
the mote has a typical RF output power of -2 dBm when the PA is disabled, then 
set the txPower parameter to -2 to turn off the PA. Note that this value is the 
RF output power coming out of the mote and not the radiated power coming out of 
the antenna. This command may be issued at any time and takes effect upon the 
next transmission. 
*/
dn_err_t dn_whmt_setParameter_txPower(int8_t txPower, dn_whmt_setParameter_txPower_rpt* reply) {
   uint8_t    extraFlags;
   dn_err_t   rc;
   
   // lock the module
   dn_lock();
   
   // verify no ongoing transmissions
   if (dn_whmt_vars.busyTx) {
      // unlock the module
      dn_unlock();
      
      // return
      return DN_ERR_BUSY;
   }
   
   // store callback information
   dn_whmt_vars.cmdId          = CMDID_SETPARAMETER;
   dn_whmt_vars.replyContents  = (uint8_t*)reply;
   dn_whmt_vars.paramId        = PARAMID_TXPOWER;
   
   // extraFlags
   extraFlags = 0x00;
   
   // build outputBuf
   dn_whmt_vars.outputBuf[0] = PARAMID_TXPOWER;
   dn_whmt_vars.outputBuf[DN_SETPARAMETER_TXPOWER_REQ_OFFS_TXPOWER] = (int8_t)txPower;
   
   // send outputBuf
   rc = dn_serial_mt_sendRequest(
      CMDID_SETPARAMETER,                                       // cmdId
      extraFlags,                                               // extraFlags
      dn_whmt_vars.outputBuf,                                   // payload
      DN_SETPARAMETER_TXPOWER_REQ_LEN,                          // length
      dn_whmt_setParameter_txPower_reply                        // replyCb
   );
   
   if (rc==DN_ERR_NONE) {
      // I'm now busy transmitting
      dn_whmt_vars.busyTx         = TRUE;
   }
   
   // unlock the module
   dn_unlock();
   
   return rc;
   
}

void dn_whmt_setParameter_txPower_reply(uint8_t cmdId, uint8_t rc, uint8_t* payload, uint8_t len) {
   dn_whmt_setParameter_txPower_rpt* reply;
   uint8_t paramId;
   
   // verify I'm expecting this answer
   if (dn_whmt_vars.busyTx==FALSE || dn_whmt_vars.cmdId!=cmdId) {
      return;
   }
   
   // verify I'm expecting this paramId
   paramId = payload[0];
   if (paramId!=dn_whmt_vars.paramId) {
      return;
   }
   
   // verify length
   if (rc==DN_SERIAL_RC_OK && len<DN_SETPARAMETER_TXPOWER_REPLY_LEN) {
      return;
   }
   
   // cast the replyContent
   reply = (dn_whmt_setParameter_txPower_rpt*)dn_whmt_vars.replyContents;
   
   // store RC
   reply->RC = rc;
   
   // parse returned value (iff RC==0)
   if (rc==DN_SERIAL_RC_OK) {
      
   }
   
   // call the callback
   dn_whmt_vars.replyCb(cmdId);
   
   // I'm not busy transmitting anymore
   dn_whmt_vars.busyTx=FALSE;
}

//===== setParameter_joinDutyCycle

/**
The setParameter<joinDutyCycle> command allows the microprocessor to control 
the join duty cycle the ratio of active listen time to doze time (a low-power 
radio state) during the period when the mote is searching for the network. The 
default duty cycle enables the mote to join the network at a reasonable rate 
without using excessive battery power. If you desire a faster join time at the 
cost of higher power consumption, use the setParameter<joinDutyCycle> command 
to increase the join duty cycle up to 100%. Note that the 
setParameter<joinDutyCycle> command is not persistent and stays in effect only 
until reset. For power consumption information, refer to the mote product datasheet.

This command may be issued multiple times during the joining process. This 
command is only effective when the mote is in the Idle and Searching states. 
*/
dn_err_t dn_whmt_setParameter_joinDutyCycle(uint8_t dutyCycle, dn_whmt_setParameter_joinDutyCycle_rpt* reply) {
   uint8_t    extraFlags;
   dn_err_t   rc;
   
   // lock the module
   dn_lock();
   
   // verify no ongoing transmissions
   if (dn_whmt_vars.busyTx) {
      // unlock the module
      dn_unlock();
      
      // return
      return DN_ERR_BUSY;
   }
   
   // store callback information
   dn_whmt_vars.cmdId          = CMDID_SETPARAMETER;
   dn_whmt_vars.replyContents  = (uint8_t*)reply;
   dn_whmt_vars.paramId        = PARAMID_JOINDUTYCYCLE;
   
   // extraFlags
   extraFlags = 0x00;
   
   // build outputBuf
   dn_whmt_vars.outputBuf[0] = PARAMID_JOINDUTYCYCLE;
   dn_whmt_vars.outputBuf[DN_SETPARAMETER_JOINDUTYCYCLE_REQ_OFFS_DUTYCYCLE] = dutyCycle;
   
   // send outputBuf
   rc = dn_serial_mt_sendRequest(
      CMDID_SETPARAMETER,                                       // cmdId
      extraFlags,                                               // extraFlags
      dn_whmt_vars.outputBuf,                                   // payload
      DN_SETPARAMETER_JOINDUTYCYCLE_REQ_LEN,                    // length
      dn_whmt_setParameter_joinDutyCycle_reply                  // replyCb
   );
   
   if (rc==DN_ERR_NONE) {
      // I'm now busy transmitting
      dn_whmt_vars.busyTx         = TRUE;
   }
   
   // unlock the module
   dn_unlock();
   
   return rc;
   
}

void dn_whmt_setParameter_joinDutyCycle_reply(uint8_t cmdId, uint8_t rc, uint8_t* payload, uint8_t len) {
   dn_whmt_setParameter_joinDutyCycle_rpt* reply;
   uint8_t paramId;
   
   // verify I'm expecting this answer
   if (dn_whmt_vars.busyTx==FALSE || dn_whmt_vars.cmdId!=cmdId) {
      return;
   }
   
   // verify I'm expecting this paramId
   paramId = payload[0];
   if (paramId!=dn_whmt_vars.paramId) {
      return;
   }
   
   // verify length
   if (rc==DN_SERIAL_RC_OK && len<DN_SETPARAMETER_JOINDUTYCYCLE_REPLY_LEN) {
      return;
   }
   
   // cast the replyContent
   reply = (dn_whmt_setParameter_joinDutyCycle_rpt*)dn_whmt_vars.replyContents;
   
   // store RC
   reply->RC = rc;
   
   // parse returned value (iff RC==0)
   if (rc==DN_SERIAL_RC_OK) {
      
   }
   
   // call the callback
   dn_whmt_vars.replyCb(cmdId);
   
   // I'm not busy transmitting anymore
   dn_whmt_vars.busyTx=FALSE;
}

//===== setParameter_batteryLife

/**
The setParameter<batteryLife> command allows the microprocessor to update the 
remaining battery life information that the mote reports to WirelessHART 
Gateway in Command 778. This parameter must be set during the Idle state prior 
to joining, and should be updated periodically throughout operation.This 
parameter is only used in WirelessHART-compliant devices.

Command 778 is deprecated in version 7.4 of the HART specification as most 
existing gateways do not use battery life information. 
*/
dn_err_t dn_whmt_setParameter_batteryLife(uint16_t batteryLife, uint8_t powerStatus, dn_whmt_setParameter_batteryLife_rpt* reply) {
   uint8_t    extraFlags;
   dn_err_t   rc;
   
   // lock the module
   dn_lock();
   
   // verify no ongoing transmissions
   if (dn_whmt_vars.busyTx) {
      // unlock the module
      dn_unlock();
      
      // return
      return DN_ERR_BUSY;
   }
   
   // store callback information
   dn_whmt_vars.cmdId          = CMDID_SETPARAMETER;
   dn_whmt_vars.replyContents  = (uint8_t*)reply;
   dn_whmt_vars.paramId        = PARAMID_BATTERYLIFE;
   
   // extraFlags
   extraFlags = 0x00;
   
   // build outputBuf
   dn_whmt_vars.outputBuf[0] = PARAMID_BATTERYLIFE;
   dn_write_uint16_t(&dn_whmt_vars.outputBuf[DN_SETPARAMETER_BATTERYLIFE_REQ_OFFS_BATTERYLIFE],batteryLife);
   dn_whmt_vars.outputBuf[DN_SETPARAMETER_BATTERYLIFE_REQ_OFFS_POWERSTATUS] = powerStatus;
   
   // send outputBuf
   rc = dn_serial_mt_sendRequest(
      CMDID_SETPARAMETER,                                       // cmdId
      extraFlags,                                               // extraFlags
      dn_whmt_vars.outputBuf,                                   // payload
      DN_SETPARAMETER_BATTERYLIFE_REQ_LEN,                      // length
      dn_whmt_setParameter_batteryLife_reply                    // replyCb
   );
   
   if (rc==DN_ERR_NONE) {
      // I'm now busy transmitting
      dn_whmt_vars.busyTx         = TRUE;
   }
   
   // unlock the module
   dn_unlock();
   
   return rc;
   
}

void dn_whmt_setParameter_batteryLife_reply(uint8_t cmdId, uint8_t rc, uint8_t* payload, uint8_t len) {
   dn_whmt_setParameter_batteryLife_rpt* reply;
   uint8_t paramId;
   
   // verify I'm expecting this answer
   if (dn_whmt_vars.busyTx==FALSE || dn_whmt_vars.cmdId!=cmdId) {
      return;
   }
   
   // verify I'm expecting this paramId
   paramId = payload[0];
   if (paramId!=dn_whmt_vars.paramId) {
      return;
   }
   
   // verify length
   if (rc==DN_SERIAL_RC_OK && len<DN_SETPARAMETER_BATTERYLIFE_REPLY_LEN) {
      return;
   }
   
   // cast the replyContent
   reply = (dn_whmt_setParameter_batteryLife_rpt*)dn_whmt_vars.replyContents;
   
   // store RC
   reply->RC = rc;
   
   // parse returned value (iff RC==0)
   if (rc==DN_SERIAL_RC_OK) {
      
   }
   
   // call the callback
   dn_whmt_vars.replyCb(cmdId);
   
   // I'm not busy transmitting anymore
   dn_whmt_vars.busyTx=FALSE;
}

//===== setParameter_service

/**
The setParameter<service> command is used to request new device-originated 
bandwidth services and modify existing device-initiated services (now called 
"Timetables" in WirelessHART 7.4).Calling thiscommand updates the motes 
internal service table, which later initiates a request to the network manager 
for bandwidth allocation. A subsequent serviceIndication notification will be 
sent indicating the response from the network manager. The 
getParameter<service> command may be used to read the service table, including 
the state of the service request.

The setParameter<service> command may be sent at any time. If the network 
manager rejects a service request, the microprocessor can try again by 
re-issuing the setParameter<service> command.

To delete a service, set the time field of the desired service to zero. Service 
request flags, application domain, and destination address are ignored by the 
mote when time equals zero.

Normally all service requests are compared against the power limits set with 
the setNVParameter<powerInfo> command. Services that would cause the device to 
exceed its power budget are denied.In Manager 4.1.1, a service request of 1 ms 
will result in the manager respecting the power limit for publish services, but 
will allow a block-transfer service requests (see the SmartMesh WirelessHART 
User's Guide section on Services)that would result in a fast pipe being 
activated. 
*/
dn_err_t dn_whmt_setParameter_service(uint8_t serviceId, uint8_t serviceReqFlags, uint8_t appDomain, uint16_t destAddr, uint32_t time, dn_whmt_setParameter_service_rpt* reply) {
   uint8_t    extraFlags;
   dn_err_t   rc;
   
   // lock the module
   dn_lock();
   
   // verify no ongoing transmissions
   if (dn_whmt_vars.busyTx) {
      // unlock the module
      dn_unlock();
      
      // return
      return DN_ERR_BUSY;
   }
   
   // store callback information
   dn_whmt_vars.cmdId          = CMDID_SETPARAMETER;
   dn_whmt_vars.replyContents  = (uint8_t*)reply;
   dn_whmt_vars.paramId        = PARAMID_SERVICE;
   
   // extraFlags
   extraFlags = 0x00;
   
   // build outputBuf
   dn_whmt_vars.outputBuf[0] = PARAMID_SERVICE;
   dn_whmt_vars.outputBuf[DN_SETPARAMETER_SERVICE_REQ_OFFS_SERVICEID] = serviceId;
   dn_whmt_vars.outputBuf[DN_SETPARAMETER_SERVICE_REQ_OFFS_SERVICEREQFLAGS] = serviceReqFlags;
   dn_whmt_vars.outputBuf[DN_SETPARAMETER_SERVICE_REQ_OFFS_APPDOMAIN] = appDomain;
   dn_write_uint16_t(&dn_whmt_vars.outputBuf[DN_SETPARAMETER_SERVICE_REQ_OFFS_DESTADDR],destAddr);
   dn_write_uint32_t(&dn_whmt_vars.outputBuf[DN_SETPARAMETER_SERVICE_REQ_OFFS_TIME],time);
   
   // send outputBuf
   rc = dn_serial_mt_sendRequest(
      CMDID_SETPARAMETER,                                       // cmdId
      extraFlags,                                               // extraFlags
      dn_whmt_vars.outputBuf,                                   // payload
      DN_SETPARAMETER_SERVICE_REQ_LEN,                          // length
      dn_whmt_setParameter_service_reply                        // replyCb
   );
   
   if (rc==DN_ERR_NONE) {
      // I'm now busy transmitting
      dn_whmt_vars.busyTx         = TRUE;
   }
   
   // unlock the module
   dn_unlock();
   
   return rc;
   
}

void dn_whmt_setParameter_service_reply(uint8_t cmdId, uint8_t rc, uint8_t* payload, uint8_t len) {
   dn_whmt_setParameter_service_rpt* reply;
   uint8_t paramId;
   
   // verify I'm expecting this answer
   if (dn_whmt_vars.busyTx==FALSE || dn_whmt_vars.cmdId!=cmdId) {
      return;
   }
   
   // verify I'm expecting this paramId
   paramId = payload[0];
   if (paramId!=dn_whmt_vars.paramId) {
      return;
   }
   
   // verify length
   if (rc==DN_SERIAL_RC_OK && len<DN_SETPARAMETER_SERVICE_REPLY_LEN) {
      return;
   }
   
   // cast the replyContent
   reply = (dn_whmt_setParameter_service_rpt*)dn_whmt_vars.replyContents;
   
   // store RC
   reply->RC = rc;
   
   // parse returned value (iff RC==0)
   if (rc==DN_SERIAL_RC_OK) {
      
      reply->numServices = payload[DN_SETPARAMETER_SERVICE_REPLY_OFFS_NUMSERVICES];
   }
   
   // call the callback
   dn_whmt_vars.replyCb(cmdId);
   
   // I'm not busy transmitting anymore
   dn_whmt_vars.busyTx=FALSE;
}

//===== setParameter_hartDeviceStatus

/**
The setParameter<hartDeviceStatus> command sets the current status of a 
WirelessHART device. The value passed in this parameter is used in all 
subsequent WirelessHART communications between the mote and the manager. This 
command is only required for WirelessHART-compliant devices. Refer to the HART 
Command Specifications for the appropriate value to use. 
*/
dn_err_t dn_whmt_setParameter_hartDeviceStatus(uint8_t hartDevStatus, dn_whmt_setParameter_hartDeviceStatus_rpt* reply) {
   uint8_t    extraFlags;
   dn_err_t   rc;
   
   // lock the module
   dn_lock();
   
   // verify no ongoing transmissions
   if (dn_whmt_vars.busyTx) {
      // unlock the module
      dn_unlock();
      
      // return
      return DN_ERR_BUSY;
   }
   
   // store callback information
   dn_whmt_vars.cmdId          = CMDID_SETPARAMETER;
   dn_whmt_vars.replyContents  = (uint8_t*)reply;
   dn_whmt_vars.paramId        = PARAMID_HARTDEVICESTATUS;
   
   // extraFlags
   extraFlags = 0x00;
   
   // build outputBuf
   dn_whmt_vars.outputBuf[0] = PARAMID_HARTDEVICESTATUS;
   dn_whmt_vars.outputBuf[DN_SETPARAMETER_HARTDEVICESTATUS_REQ_OFFS_HARTDEVSTATUS] = hartDevStatus;
   
   // send outputBuf
   rc = dn_serial_mt_sendRequest(
      CMDID_SETPARAMETER,                                       // cmdId
      extraFlags,                                               // extraFlags
      dn_whmt_vars.outputBuf,                                   // payload
      DN_SETPARAMETER_HARTDEVICESTATUS_REQ_LEN,                 // length
      dn_whmt_setParameter_hartDeviceStatus_reply               // replyCb
   );
   
   if (rc==DN_ERR_NONE) {
      // I'm now busy transmitting
      dn_whmt_vars.busyTx         = TRUE;
   }
   
   // unlock the module
   dn_unlock();
   
   return rc;
   
}

void dn_whmt_setParameter_hartDeviceStatus_reply(uint8_t cmdId, uint8_t rc, uint8_t* payload, uint8_t len) {
   dn_whmt_setParameter_hartDeviceStatus_rpt* reply;
   uint8_t paramId;
   
   // verify I'm expecting this answer
   if (dn_whmt_vars.busyTx==FALSE || dn_whmt_vars.cmdId!=cmdId) {
      return;
   }
   
   // verify I'm expecting this paramId
   paramId = payload[0];
   if (paramId!=dn_whmt_vars.paramId) {
      return;
   }
   
   // verify length
   if (rc==DN_SERIAL_RC_OK && len<DN_SETPARAMETER_HARTDEVICESTATUS_REPLY_LEN) {
      return;
   }
   
   // cast the replyContent
   reply = (dn_whmt_setParameter_hartDeviceStatus_rpt*)dn_whmt_vars.replyContents;
   
   // store RC
   reply->RC = rc;
   
   // parse returned value (iff RC==0)
   if (rc==DN_SERIAL_RC_OK) {
      
   }
   
   // call the callback
   dn_whmt_vars.replyCb(cmdId);
   
   // I'm not busy transmitting anymore
   dn_whmt_vars.busyTx=FALSE;
}

//===== setParameter_hartDeviceInfo

/**
The setParameter<hartDeviceInfo> command is used to set HART device information 
that the mote passes to gateway during join. This command must be issued prior 
to join. This command is only required for WirelessHART-compliant devices.Note 
that the contents of this command are not validated by mote. 
*/
dn_err_t dn_whmt_setParameter_hartDeviceInfo(uint8_t* hartCmd0, uint8_t* hartCmd20, dn_whmt_setParameter_hartDeviceInfo_rpt* reply) {
   uint8_t    extraFlags;
   dn_err_t   rc;
   
   // lock the module
   dn_lock();
   
   // verify no ongoing transmissions
   if (dn_whmt_vars.busyTx) {
      // unlock the module
      dn_unlock();
      
      // return
      return DN_ERR_BUSY;
   }
   
   // store callback information
   dn_whmt_vars.cmdId          = CMDID_SETPARAMETER;
   dn_whmt_vars.replyContents  = (uint8_t*)reply;
   dn_whmt_vars.paramId        = PARAMID_HARTDEVICEINFO;
   
   // extraFlags
   extraFlags = 0x00;
   
   // build outputBuf
   dn_whmt_vars.outputBuf[0] = PARAMID_HARTDEVICEINFO;
   memcpy(&dn_whmt_vars.outputBuf[DN_SETPARAMETER_HARTDEVICEINFO_REQ_OFFS_HARTCMD0],hartCmd0,22);
   memcpy(&dn_whmt_vars.outputBuf[DN_SETPARAMETER_HARTDEVICEINFO_REQ_OFFS_HARTCMD20],hartCmd20,32);
   
   // send outputBuf
   rc = dn_serial_mt_sendRequest(
      CMDID_SETPARAMETER,                                       // cmdId
      extraFlags,                                               // extraFlags
      dn_whmt_vars.outputBuf,                                   // payload
      DN_SETPARAMETER_HARTDEVICEINFO_REQ_LEN,                   // length
      dn_whmt_setParameter_hartDeviceInfo_reply                 // replyCb
   );
   
   if (rc==DN_ERR_NONE) {
      // I'm now busy transmitting
      dn_whmt_vars.busyTx         = TRUE;
   }
   
   // unlock the module
   dn_unlock();
   
   return rc;
   
}

void dn_whmt_setParameter_hartDeviceInfo_reply(uint8_t cmdId, uint8_t rc, uint8_t* payload, uint8_t len) {
   dn_whmt_setParameter_hartDeviceInfo_rpt* reply;
   uint8_t paramId;
   
   // verify I'm expecting this answer
   if (dn_whmt_vars.busyTx==FALSE || dn_whmt_vars.cmdId!=cmdId) {
      return;
   }
   
   // verify I'm expecting this paramId
   paramId = payload[0];
   if (paramId!=dn_whmt_vars.paramId) {
      return;
   }
   
   // verify length
   if (rc==DN_SERIAL_RC_OK && len<DN_SETPARAMETER_HARTDEVICEINFO_REPLY_LEN) {
      return;
   }
   
   // cast the replyContent
   reply = (dn_whmt_setParameter_hartDeviceInfo_rpt*)dn_whmt_vars.replyContents;
   
   // store RC
   reply->RC = rc;
   
   // parse returned value (iff RC==0)
   if (rc==DN_SERIAL_RC_OK) {
      
   }
   
   // call the callback
   dn_whmt_vars.replyCb(cmdId);
   
   // I'm not busy transmitting anymore
   dn_whmt_vars.busyTx=FALSE;
}

//===== setParameter_eventMask

/**
The setParameter<eventMask> command allows the microprocessor to subscribe to 
the types of events that may be sent in the motes events notification message. 
This command may be called at any time and takes effect at the next event 
notification. The mote includes an event in the notification message if the 
corresponding bit in <eventMask> is set to 1, and excludes the event if the bit 
is set to 0. At mote reset, the default value of <eventMask> is 1 for all events.

New event type may be added in future revisions of mote software. It is 
recommended that the client code only subscribe to known events and gracefully 
ignore all unknown events. 
*/
dn_err_t dn_whmt_setParameter_eventMask(uint32_t eventMask, dn_whmt_setParameter_eventMask_rpt* reply) {
   uint8_t    extraFlags;
   dn_err_t   rc;
   
   // lock the module
   dn_lock();
   
   // verify no ongoing transmissions
   if (dn_whmt_vars.busyTx) {
      // unlock the module
      dn_unlock();
      
      // return
      return DN_ERR_BUSY;
   }
   
   // store callback information
   dn_whmt_vars.cmdId          = CMDID_SETPARAMETER;
   dn_whmt_vars.replyContents  = (uint8_t*)reply;
   dn_whmt_vars.paramId        = PARAMID_EVENTMASK;
   
   // extraFlags
   extraFlags = 0x00;
   
   // build outputBuf
   dn_whmt_vars.outputBuf[0] = PARAMID_EVENTMASK;
   dn_write_uint32_t(&dn_whmt_vars.outputBuf[DN_SETPARAMETER_EVENTMASK_REQ_OFFS_EVENTMASK],eventMask);
   
   // send outputBuf
   rc = dn_serial_mt_sendRequest(
      CMDID_SETPARAMETER,                                       // cmdId
      extraFlags,                                               // extraFlags
      dn_whmt_vars.outputBuf,                                   // payload
      DN_SETPARAMETER_EVENTMASK_REQ_LEN,                        // length
      dn_whmt_setParameter_eventMask_reply                      // replyCb
   );
   
   if (rc==DN_ERR_NONE) {
      // I'm now busy transmitting
      dn_whmt_vars.busyTx         = TRUE;
   }
   
   // unlock the module
   dn_unlock();
   
   return rc;
   
}

void dn_whmt_setParameter_eventMask_reply(uint8_t cmdId, uint8_t rc, uint8_t* payload, uint8_t len) {
   dn_whmt_setParameter_eventMask_rpt* reply;
   uint8_t paramId;
   
   // verify I'm expecting this answer
   if (dn_whmt_vars.busyTx==FALSE || dn_whmt_vars.cmdId!=cmdId) {
      return;
   }
   
   // verify I'm expecting this paramId
   paramId = payload[0];
   if (paramId!=dn_whmt_vars.paramId) {
      return;
   }
   
   // verify length
   if (rc==DN_SERIAL_RC_OK && len<DN_SETPARAMETER_EVENTMASK_REPLY_LEN) {
      return;
   }
   
   // cast the replyContent
   reply = (dn_whmt_setParameter_eventMask_rpt*)dn_whmt_vars.replyContents;
   
   // store RC
   reply->RC = rc;
   
   // parse returned value (iff RC==0)
   if (rc==DN_SERIAL_RC_OK) {
      
   }
   
   // call the callback
   dn_whmt_vars.replyCb(cmdId);
   
   // I'm not busy transmitting anymore
   dn_whmt_vars.busyTx=FALSE;
}

//===== setParameter_writeProtect

/**
The setParameter<writeProtect> command allows the microprocessor to enable or 
disable access to selected WirelessHART commands via wireless or the 
hartPayload command. Refer to the SmartMesh WirelessHART User's Guide for the 
list of affected commands. If writeProtect is enabled and the mote receives any 
of these commands (either via wireless connection or via the hartPayload 
command), the command will have no effect, and the mote will return RC_7 (In 
Write Protect Mode). At mote boot, writeProtect is set to 0 (writes allowed). 
The current status of writeProtect may be read via the getParameter<moteStatus> 
command. This command is for WirelessHART-compliant devices only. 
*/
dn_err_t dn_whmt_setParameter_writeProtect(uint8_t writeProtect, dn_whmt_setParameter_writeProtect_rpt* reply) {
   uint8_t    extraFlags;
   dn_err_t   rc;
   
   // lock the module
   dn_lock();
   
   // verify no ongoing transmissions
   if (dn_whmt_vars.busyTx) {
      // unlock the module
      dn_unlock();
      
      // return
      return DN_ERR_BUSY;
   }
   
   // store callback information
   dn_whmt_vars.cmdId          = CMDID_SETPARAMETER;
   dn_whmt_vars.replyContents  = (uint8_t*)reply;
   dn_whmt_vars.paramId        = PARAMID_WRITEPROTECT;
   
   // extraFlags
   extraFlags = 0x00;
   
   // build outputBuf
   dn_whmt_vars.outputBuf[0] = PARAMID_WRITEPROTECT;
   dn_whmt_vars.outputBuf[DN_SETPARAMETER_WRITEPROTECT_REQ_OFFS_WRITEPROTECT] = writeProtect;
   
   // send outputBuf
   rc = dn_serial_mt_sendRequest(
      CMDID_SETPARAMETER,                                       // cmdId
      extraFlags,                                               // extraFlags
      dn_whmt_vars.outputBuf,                                   // payload
      DN_SETPARAMETER_WRITEPROTECT_REQ_LEN,                     // length
      dn_whmt_setParameter_writeProtect_reply                   // replyCb
   );
   
   if (rc==DN_ERR_NONE) {
      // I'm now busy transmitting
      dn_whmt_vars.busyTx         = TRUE;
   }
   
   // unlock the module
   dn_unlock();
   
   return rc;
   
}

void dn_whmt_setParameter_writeProtect_reply(uint8_t cmdId, uint8_t rc, uint8_t* payload, uint8_t len) {
   dn_whmt_setParameter_writeProtect_rpt* reply;
   uint8_t paramId;
   
   // verify I'm expecting this answer
   if (dn_whmt_vars.busyTx==FALSE || dn_whmt_vars.cmdId!=cmdId) {
      return;
   }
   
   // verify I'm expecting this paramId
   paramId = payload[0];
   if (paramId!=dn_whmt_vars.paramId) {
      return;
   }
   
   // verify length
   if (rc==DN_SERIAL_RC_OK && len<DN_SETPARAMETER_WRITEPROTECT_REPLY_LEN) {
      return;
   }
   
   // cast the replyContent
   reply = (dn_whmt_setParameter_writeProtect_rpt*)dn_whmt_vars.replyContents;
   
   // store RC
   reply->RC = rc;
   
   // parse returned value (iff RC==0)
   if (rc==DN_SERIAL_RC_OK) {
      
   }
   
   // call the callback
   dn_whmt_vars.replyCb(cmdId);
   
   // I'm not busy transmitting anymore
   dn_whmt_vars.busyTx=FALSE;
}

//===== setParameter_lock

/**
The setParameter<lock> command locks/unlocks select HART commands (ones that 
affect the configuration changed flag) to a specific master (GW or serial 
maintenance port) to prevent the other master from changing it. This command is 
intended for use when the lock is temporary, i.e. it does not persist through 
power cycle or reset. For nonvolatile locking, use the setNVParameter<lock> 
command.Note: This parameter is available in devices running mote software >= 
1.1.0 
*/
dn_err_t dn_whmt_setParameter_lock(uint8_t code, uint16_t master, dn_whmt_setParameter_lock_rpt* reply) {
   uint8_t    extraFlags;
   dn_err_t   rc;
   
   // lock the module
   dn_lock();
   
   // verify no ongoing transmissions
   if (dn_whmt_vars.busyTx) {
      // unlock the module
      dn_unlock();
      
      // return
      return DN_ERR_BUSY;
   }
   
   // store callback information
   dn_whmt_vars.cmdId          = CMDID_SETPARAMETER;
   dn_whmt_vars.replyContents  = (uint8_t*)reply;
   dn_whmt_vars.paramId        = PARAMID_LOCK;
   
   // extraFlags
   extraFlags = 0x00;
   
   // build outputBuf
   dn_whmt_vars.outputBuf[0] = PARAMID_LOCK;
   dn_whmt_vars.outputBuf[DN_SETPARAMETER_LOCK_REQ_OFFS_CODE] = code;
   dn_write_uint16_t(&dn_whmt_vars.outputBuf[DN_SETPARAMETER_LOCK_REQ_OFFS_MASTER],master);
   
   // send outputBuf
   rc = dn_serial_mt_sendRequest(
      CMDID_SETPARAMETER,                                       // cmdId
      extraFlags,                                               // extraFlags
      dn_whmt_vars.outputBuf,                                   // payload
      DN_SETPARAMETER_LOCK_REQ_LEN,                             // length
      dn_whmt_setParameter_lock_reply                           // replyCb
   );
   
   if (rc==DN_ERR_NONE) {
      // I'm now busy transmitting
      dn_whmt_vars.busyTx         = TRUE;
   }
   
   // unlock the module
   dn_unlock();
   
   return rc;
   
}

void dn_whmt_setParameter_lock_reply(uint8_t cmdId, uint8_t rc, uint8_t* payload, uint8_t len) {
   dn_whmt_setParameter_lock_rpt* reply;
   uint8_t paramId;
   
   // verify I'm expecting this answer
   if (dn_whmt_vars.busyTx==FALSE || dn_whmt_vars.cmdId!=cmdId) {
      return;
   }
   
   // verify I'm expecting this paramId
   paramId = payload[0];
   if (paramId!=dn_whmt_vars.paramId) {
      return;
   }
   
   // verify length
   if (rc==DN_SERIAL_RC_OK && len<DN_SETPARAMETER_LOCK_REPLY_LEN) {
      return;
   }
   
   // cast the replyContent
   reply = (dn_whmt_setParameter_lock_rpt*)dn_whmt_vars.replyContents;
   
   // store RC
   reply->RC = rc;
   
   // parse returned value (iff RC==0)
   if (rc==DN_SERIAL_RC_OK) {
      
   }
   
   // call the callback
   dn_whmt_vars.replyCb(cmdId);
   
   // I'm not busy transmitting anymore
   dn_whmt_vars.busyTx=FALSE;
}

//===== getParameter_joinDutyCycle

/**
The getParameter<joinDutyCycle> command return mote's join duty cycle, which 
determines the percentage of time the mote spends in radio receive mode while 
searching for network. The value of join duty cycle is expressed in increments 
of 1/255th of 100%, where 0 corresponds to 0% and 255 corresponds to 100%. 
*/
dn_err_t dn_whmt_getParameter_joinDutyCycle(dn_whmt_getParameter_joinDutyCycle_rpt* reply) {
   uint8_t    extraFlags;
   dn_err_t   rc;
   
   // lock the module
   dn_lock();
   
   // verify no ongoing transmissions
   if (dn_whmt_vars.busyTx) {
      // unlock the module
      dn_unlock();
      
      // return
      return DN_ERR_BUSY;
   }
   
   // store callback information
   dn_whmt_vars.cmdId          = CMDID_GETPARAMETER;
   dn_whmt_vars.replyContents  = (uint8_t*)reply;
   dn_whmt_vars.paramId        = PARAMID_JOINDUTYCYCLE;
   
   // extraFlags
   extraFlags = 0x00;
   
   // build outputBuf
   dn_whmt_vars.outputBuf[0] = PARAMID_JOINDUTYCYCLE;
   
   // send outputBuf
   rc = dn_serial_mt_sendRequest(
      CMDID_GETPARAMETER,                                       // cmdId
      extraFlags,                                               // extraFlags
      dn_whmt_vars.outputBuf,                                   // payload
      DN_GETPARAMETER_JOINDUTYCYCLE_REQ_LEN,                    // length
      dn_whmt_getParameter_joinDutyCycle_reply                  // replyCb
   );
   
   if (rc==DN_ERR_NONE) {
      // I'm now busy transmitting
      dn_whmt_vars.busyTx         = TRUE;
   }
   
   // unlock the module
   dn_unlock();
   
   return rc;
   
}

void dn_whmt_getParameter_joinDutyCycle_reply(uint8_t cmdId, uint8_t rc, uint8_t* payload, uint8_t len) {
   dn_whmt_getParameter_joinDutyCycle_rpt* reply;
   uint8_t paramId;
   
   // verify I'm expecting this answer
   if (dn_whmt_vars.busyTx==FALSE || dn_whmt_vars.cmdId!=cmdId) {
      return;
   }
   
   // verify I'm expecting this paramId
   paramId = payload[0];
   if (paramId!=dn_whmt_vars.paramId) {
      return;
   }
   
   // verify length
   if (rc==DN_SERIAL_RC_OK && len<DN_GETPARAMETER_JOINDUTYCYCLE_REPLY_LEN) {
      return;
   }
   
   // cast the replyContent
   reply = (dn_whmt_getParameter_joinDutyCycle_rpt*)dn_whmt_vars.replyContents;
   
   // store RC
   reply->RC = rc;
   
   // parse returned value (iff RC==0)
   if (rc==DN_SERIAL_RC_OK) {
      
      reply->joinDutyCycle = payload[DN_GETPARAMETER_JOINDUTYCYCLE_REPLY_OFFS_JOINDUTYCYCLE];
   }
   
   // call the callback
   dn_whmt_vars.replyCb(cmdId);
   
   // I'm not busy transmitting anymore
   dn_whmt_vars.busyTx=FALSE;
}

//===== getParameter_service

/**
The getParameter<service> command retrieves information about the service 
allocation that is currently available to the field device. Services (now 
called "Timetables" in WirelessHART 7.4) in the range 0x00-7F are those 
requested by the device, and those in the range 0x80-FF are assigned 
independently by the network manager. 
*/
dn_err_t dn_whmt_getParameter_service(uint8_t serviceId, dn_whmt_getParameter_service_rpt* reply) {
   uint8_t    extraFlags;
   dn_err_t   rc;
   
   // lock the module
   dn_lock();
   
   // verify no ongoing transmissions
   if (dn_whmt_vars.busyTx) {
      // unlock the module
      dn_unlock();
      
      // return
      return DN_ERR_BUSY;
   }
   
   // store callback information
   dn_whmt_vars.cmdId          = CMDID_GETPARAMETER;
   dn_whmt_vars.replyContents  = (uint8_t*)reply;
   dn_whmt_vars.paramId        = PARAMID_SERVICE;
   
   // extraFlags
   extraFlags = 0x00;
   
   // build outputBuf
   dn_whmt_vars.outputBuf[0] = PARAMID_SERVICE;
   dn_whmt_vars.outputBuf[DN_GETPARAMETER_SERVICE_REQ_OFFS_SERVICEID] = serviceId;
   
   // send outputBuf
   rc = dn_serial_mt_sendRequest(
      CMDID_GETPARAMETER,                                       // cmdId
      extraFlags,                                               // extraFlags
      dn_whmt_vars.outputBuf,                                   // payload
      DN_GETPARAMETER_SERVICE_REQ_LEN,                          // length
      dn_whmt_getParameter_service_reply                        // replyCb
   );
   
   if (rc==DN_ERR_NONE) {
      // I'm now busy transmitting
      dn_whmt_vars.busyTx         = TRUE;
   }
   
   // unlock the module
   dn_unlock();
   
   return rc;
   
}

void dn_whmt_getParameter_service_reply(uint8_t cmdId, uint8_t rc, uint8_t* payload, uint8_t len) {
   dn_whmt_getParameter_service_rpt* reply;
   uint8_t paramId;
   
   // verify I'm expecting this answer
   if (dn_whmt_vars.busyTx==FALSE || dn_whmt_vars.cmdId!=cmdId) {
      return;
   }
   
   // verify I'm expecting this paramId
   paramId = payload[0];
   if (paramId!=dn_whmt_vars.paramId) {
      return;
   }
   
   // verify length
   if (rc==DN_SERIAL_RC_OK && len<DN_GETPARAMETER_SERVICE_REPLY_LEN) {
      return;
   }
   
   // cast the replyContent
   reply = (dn_whmt_getParameter_service_rpt*)dn_whmt_vars.replyContents;
   
   // store RC
   reply->RC = rc;
   
   // parse returned value (iff RC==0)
   if (rc==DN_SERIAL_RC_OK) {
      
      reply->serviceId = payload[DN_GETPARAMETER_SERVICE_REPLY_OFFS_SERVICEID];
      reply->serviceState = payload[DN_GETPARAMETER_SERVICE_REPLY_OFFS_SERVICESTATE];
      reply->serviceFlags = payload[DN_GETPARAMETER_SERVICE_REPLY_OFFS_SERVICEFLAGS];
      reply->appDomain = payload[DN_GETPARAMETER_SERVICE_REPLY_OFFS_APPDOMAIN];
      dn_read_uint16_t(&reply->destAddr,&payload[DN_GETPARAMETER_SERVICE_REPLY_OFFS_DESTADDR]);
      dn_read_uint32_t(&reply->time,&payload[DN_GETPARAMETER_SERVICE_REPLY_OFFS_TIME]);
   }
   
   // call the callback
   dn_whmt_vars.replyCb(cmdId);
   
   // I'm not busy transmitting anymore
   dn_whmt_vars.busyTx=FALSE;
}

//===== getParameter_moteInfo

/**
The getParameter<moteInfo> command returns static information about the motes 
hardware and software. Note that network state-related information about the 
mote may be retrieved using getParameter<networkInfo>. 
*/
dn_err_t dn_whmt_getParameter_moteInfo(dn_whmt_getParameter_moteInfo_rpt* reply) {
   uint8_t    extraFlags;
   dn_err_t   rc;
   
   // lock the module
   dn_lock();
   
   // verify no ongoing transmissions
   if (dn_whmt_vars.busyTx) {
      // unlock the module
      dn_unlock();
      
      // return
      return DN_ERR_BUSY;
   }
   
   // store callback information
   dn_whmt_vars.cmdId          = CMDID_GETPARAMETER;
   dn_whmt_vars.replyContents  = (uint8_t*)reply;
   dn_whmt_vars.paramId        = PARAMID_MOTEINFO;
   
   // extraFlags
   extraFlags = 0x00;
   
   // build outputBuf
   dn_whmt_vars.outputBuf[0] = PARAMID_MOTEINFO;
   
   // send outputBuf
   rc = dn_serial_mt_sendRequest(
      CMDID_GETPARAMETER,                                       // cmdId
      extraFlags,                                               // extraFlags
      dn_whmt_vars.outputBuf,                                   // payload
      DN_GETPARAMETER_MOTEINFO_REQ_LEN,                         // length
      dn_whmt_getParameter_moteInfo_reply                       // replyCb
   );
   
   if (rc==DN_ERR_NONE) {
      // I'm now busy transmitting
      dn_whmt_vars.busyTx         = TRUE;
   }
   
   // unlock the module
   dn_unlock();
   
   return rc;
   
}

void dn_whmt_getParameter_moteInfo_reply(uint8_t cmdId, uint8_t rc, uint8_t* payload, uint8_t len) {
   dn_whmt_getParameter_moteInfo_rpt* reply;
   uint8_t paramId;
   
   // verify I'm expecting this answer
   if (dn_whmt_vars.busyTx==FALSE || dn_whmt_vars.cmdId!=cmdId) {
      return;
   }
   
   // verify I'm expecting this paramId
   paramId = payload[0];
   if (paramId!=dn_whmt_vars.paramId) {
      return;
   }
   
   // verify length
   if (rc==DN_SERIAL_RC_OK && len<DN_GETPARAMETER_MOTEINFO_REPLY_LEN) {
      return;
   }
   
   // cast the replyContent
   reply = (dn_whmt_getParameter_moteInfo_rpt*)dn_whmt_vars.replyContents;
   
   // store RC
   reply->RC = rc;
   
   // parse returned value (iff RC==0)
   if (rc==DN_SERIAL_RC_OK) {
      
      reply->apiVersion = payload[DN_GETPARAMETER_MOTEINFO_REPLY_OFFS_APIVERSION];
      memcpy(&reply->serialNum[0],&payload[DN_GETPARAMETER_MOTEINFO_REPLY_OFFS_SERIALNUM],8);
      reply->hwModel = payload[DN_GETPARAMETER_MOTEINFO_REPLY_OFFS_HWMODEL];
      reply->hwRev = payload[DN_GETPARAMETER_MOTEINFO_REPLY_OFFS_HWREV];
      reply->swMajorRev = payload[DN_GETPARAMETER_MOTEINFO_REPLY_OFFS_SWMAJORREV];
      reply->swMinorRev = payload[DN_GETPARAMETER_MOTEINFO_REPLY_OFFS_SWMINORREV];
      reply->swPatch = payload[DN_GETPARAMETER_MOTEINFO_REPLY_OFFS_SWPATCH];
      dn_read_uint16_t(&reply->swBuild,&payload[DN_GETPARAMETER_MOTEINFO_REPLY_OFFS_SWBUILD]);
   }
   
   // call the callback
   dn_whmt_vars.replyCb(cmdId);
   
   // I'm not busy transmitting anymore
   dn_whmt_vars.busyTx=FALSE;
}

//===== getParameter_networkInfo

/**
The getParameter<networkInfo> command may be used to retrieve the mote's 
network-related parameters. Note that static information about the motes 
hardware and software may be retrieved using getParameter<moteInfo>. 
*/
dn_err_t dn_whmt_getParameter_networkInfo(dn_whmt_getParameter_networkInfo_rpt* reply) {
   uint8_t    extraFlags;
   dn_err_t   rc;
   
   // lock the module
   dn_lock();
   
   // verify no ongoing transmissions
   if (dn_whmt_vars.busyTx) {
      // unlock the module
      dn_unlock();
      
      // return
      return DN_ERR_BUSY;
   }
   
   // store callback information
   dn_whmt_vars.cmdId          = CMDID_GETPARAMETER;
   dn_whmt_vars.replyContents  = (uint8_t*)reply;
   dn_whmt_vars.paramId        = PARAMID_NETWORKINFO;
   
   // extraFlags
   extraFlags = 0x00;
   
   // build outputBuf
   dn_whmt_vars.outputBuf[0] = PARAMID_NETWORKINFO;
   
   // send outputBuf
   rc = dn_serial_mt_sendRequest(
      CMDID_GETPARAMETER,                                       // cmdId
      extraFlags,                                               // extraFlags
      dn_whmt_vars.outputBuf,                                   // payload
      DN_GETPARAMETER_NETWORKINFO_REQ_LEN,                      // length
      dn_whmt_getParameter_networkInfo_reply                    // replyCb
   );
   
   if (rc==DN_ERR_NONE) {
      // I'm now busy transmitting
      dn_whmt_vars.busyTx         = TRUE;
   }
   
   // unlock the module
   dn_unlock();
   
   return rc;
   
}

void dn_whmt_getParameter_networkInfo_reply(uint8_t cmdId, uint8_t rc, uint8_t* payload, uint8_t len) {
   dn_whmt_getParameter_networkInfo_rpt* reply;
   uint8_t paramId;
   
   // verify I'm expecting this answer
   if (dn_whmt_vars.busyTx==FALSE || dn_whmt_vars.cmdId!=cmdId) {
      return;
   }
   
   // verify I'm expecting this paramId
   paramId = payload[0];
   if (paramId!=dn_whmt_vars.paramId) {
      return;
   }
   
   // verify length
   if (rc==DN_SERIAL_RC_OK && len<DN_GETPARAMETER_NETWORKINFO_REPLY_LEN) {
      return;
   }
   
   // cast the replyContent
   reply = (dn_whmt_getParameter_networkInfo_rpt*)dn_whmt_vars.replyContents;
   
   // store RC
   reply->RC = rc;
   
   // parse returned value (iff RC==0)
   if (rc==DN_SERIAL_RC_OK) {
      
      memcpy(&reply->macAddress[0],&payload[DN_GETPARAMETER_NETWORKINFO_REPLY_OFFS_MACADDRESS],8);
      dn_read_uint16_t(&reply->moteId,&payload[DN_GETPARAMETER_NETWORKINFO_REPLY_OFFS_MOTEID]);
      dn_read_uint16_t(&reply->networkId,&payload[DN_GETPARAMETER_NETWORKINFO_REPLY_OFFS_NETWORKID]);
   }
   
   // call the callback
   dn_whmt_vars.replyCb(cmdId);
   
   // I'm not busy transmitting anymore
   dn_whmt_vars.busyTx=FALSE;
}

//===== getParameter_moteStatus

/**
The getParameter<moteStatus> command is used to retrieve the mote's state and 
frequently changing information. Note that static information about the state 
of the mote hardware and software may be retrieved using 
getParameter<moteInfo>. 
*/
dn_err_t dn_whmt_getParameter_moteStatus(dn_whmt_getParameter_moteStatus_rpt* reply) {
   uint8_t    extraFlags;
   dn_err_t   rc;
   
   // lock the module
   dn_lock();
   
   // verify no ongoing transmissions
   if (dn_whmt_vars.busyTx) {
      // unlock the module
      dn_unlock();
      
      // return
      return DN_ERR_BUSY;
   }
   
   // store callback information
   dn_whmt_vars.cmdId          = CMDID_GETPARAMETER;
   dn_whmt_vars.replyContents  = (uint8_t*)reply;
   dn_whmt_vars.paramId        = PARAMID_MOTESTATUS;
   
   // extraFlags
   extraFlags = 0x00;
   
   // build outputBuf
   dn_whmt_vars.outputBuf[0] = PARAMID_MOTESTATUS;
   
   // send outputBuf
   rc = dn_serial_mt_sendRequest(
      CMDID_GETPARAMETER,                                       // cmdId
      extraFlags,                                               // extraFlags
      dn_whmt_vars.outputBuf,                                   // payload
      DN_GETPARAMETER_MOTESTATUS_REQ_LEN,                       // length
      dn_whmt_getParameter_moteStatus_reply                     // replyCb
   );
   
   if (rc==DN_ERR_NONE) {
      // I'm now busy transmitting
      dn_whmt_vars.busyTx         = TRUE;
   }
   
   // unlock the module
   dn_unlock();
   
   return rc;
   
}

void dn_whmt_getParameter_moteStatus_reply(uint8_t cmdId, uint8_t rc, uint8_t* payload, uint8_t len) {
   dn_whmt_getParameter_moteStatus_rpt* reply;
   uint8_t paramId;
   
   // verify I'm expecting this answer
   if (dn_whmt_vars.busyTx==FALSE || dn_whmt_vars.cmdId!=cmdId) {
      return;
   }
   
   // verify I'm expecting this paramId
   paramId = payload[0];
   if (paramId!=dn_whmt_vars.paramId) {
      return;
   }
   
   // verify length
   if (rc==DN_SERIAL_RC_OK && len<DN_GETPARAMETER_MOTESTATUS_REPLY_LEN) {
      return;
   }
   
   // cast the replyContent
   reply = (dn_whmt_getParameter_moteStatus_rpt*)dn_whmt_vars.replyContents;
   
   // store RC
   reply->RC = rc;
   
   // parse returned value (iff RC==0)
   if (rc==DN_SERIAL_RC_OK) {
      
      reply->state = payload[DN_GETPARAMETER_MOTESTATUS_REPLY_OFFS_STATE];
      reply->moteStateReason = payload[DN_GETPARAMETER_MOTESTATUS_REPLY_OFFS_MOTESTATEREASON];
      dn_read_uint16_t(&reply->changeCounter,&payload[DN_GETPARAMETER_MOTESTATUS_REPLY_OFFS_CHANGECOUNTER]);
      reply->numParents = payload[DN_GETPARAMETER_MOTESTATUS_REPLY_OFFS_NUMPARENTS];
      dn_read_uint32_t(&reply->moteAlarms,&payload[DN_GETPARAMETER_MOTESTATUS_REPLY_OFFS_MOTEALARMS]);
      reply->statusFlags = payload[DN_GETPARAMETER_MOTESTATUS_REPLY_OFFS_STATUSFLAGS];
   }
   
   // call the callback
   dn_whmt_vars.replyCb(cmdId);
   
   // I'm not busy transmitting anymore
   dn_whmt_vars.busyTx=FALSE;
}

//===== getParameter_time

/**
The getParameter<time> command is used to request the current time on the mote. 
*/
dn_err_t dn_whmt_getParameter_time(dn_whmt_getParameter_time_rpt* reply) {
   uint8_t    extraFlags;
   dn_err_t   rc;
   
   // lock the module
   dn_lock();
   
   // verify no ongoing transmissions
   if (dn_whmt_vars.busyTx) {
      // unlock the module
      dn_unlock();
      
      // return
      return DN_ERR_BUSY;
   }
   
   // store callback information
   dn_whmt_vars.cmdId          = CMDID_GETPARAMETER;
   dn_whmt_vars.replyContents  = (uint8_t*)reply;
   dn_whmt_vars.paramId        = PARAMID_TIME;
   
   // extraFlags
   extraFlags = 0x00;
   
   // build outputBuf
   dn_whmt_vars.outputBuf[0] = PARAMID_TIME;
   
   // send outputBuf
   rc = dn_serial_mt_sendRequest(
      CMDID_GETPARAMETER,                                       // cmdId
      extraFlags,                                               // extraFlags
      dn_whmt_vars.outputBuf,                                   // payload
      DN_GETPARAMETER_TIME_REQ_LEN,                             // length
      dn_whmt_getParameter_time_reply                           // replyCb
   );
   
   if (rc==DN_ERR_NONE) {
      // I'm now busy transmitting
      dn_whmt_vars.busyTx         = TRUE;
   }
   
   // unlock the module
   dn_unlock();
   
   return rc;
   
}

void dn_whmt_getParameter_time_reply(uint8_t cmdId, uint8_t rc, uint8_t* payload, uint8_t len) {
   dn_whmt_getParameter_time_rpt* reply;
   uint8_t paramId;
   
   // verify I'm expecting this answer
   if (dn_whmt_vars.busyTx==FALSE || dn_whmt_vars.cmdId!=cmdId) {
      return;
   }
   
   // verify I'm expecting this paramId
   paramId = payload[0];
   if (paramId!=dn_whmt_vars.paramId) {
      return;
   }
   
   // verify length
   if (rc==DN_SERIAL_RC_OK && len<DN_GETPARAMETER_TIME_REPLY_LEN) {
      return;
   }
   
   // cast the replyContent
   reply = (dn_whmt_getParameter_time_rpt*)dn_whmt_vars.replyContents;
   
   // store RC
   reply->RC = rc;
   
   // parse returned value (iff RC==0)
   if (rc==DN_SERIAL_RC_OK) {
      
      memcpy(&reply->utcTime[0],&payload[DN_GETPARAMETER_TIME_REPLY_OFFS_UTCTIME],8);
      memcpy(&reply->asn[0],&payload[DN_GETPARAMETER_TIME_REPLY_OFFS_ASN],5);
      dn_read_uint16_t(&reply->asnOffset,&payload[DN_GETPARAMETER_TIME_REPLY_OFFS_ASNOFFSET]);
   }
   
   // call the callback
   dn_whmt_vars.replyCb(cmdId);
   
   // I'm not busy transmitting anymore
   dn_whmt_vars.busyTx=FALSE;
}

//===== getParameter_charge

/**
The getParameter<charge> command retrieves estimated charge consumption of the 
mote since the last reset, as well as the mote uptime and last measured 
temperature. 
*/
dn_err_t dn_whmt_getParameter_charge(dn_whmt_getParameter_charge_rpt* reply) {
   uint8_t    extraFlags;
   dn_err_t   rc;
   
   // lock the module
   dn_lock();
   
   // verify no ongoing transmissions
   if (dn_whmt_vars.busyTx) {
      // unlock the module
      dn_unlock();
      
      // return
      return DN_ERR_BUSY;
   }
   
   // store callback information
   dn_whmt_vars.cmdId          = CMDID_GETPARAMETER;
   dn_whmt_vars.replyContents  = (uint8_t*)reply;
   dn_whmt_vars.paramId        = PARAMID_CHARGE;
   
   // extraFlags
   extraFlags = 0x00;
   
   // build outputBuf
   dn_whmt_vars.outputBuf[0] = PARAMID_CHARGE;
   
   // send outputBuf
   rc = dn_serial_mt_sendRequest(
      CMDID_GETPARAMETER,                                       // cmdId
      extraFlags,                                               // extraFlags
      dn_whmt_vars.outputBuf,                                   // payload
      DN_GETPARAMETER_CHARGE_REQ_LEN,                           // length
      dn_whmt_getParameter_charge_reply                         // replyCb
   );
   
   if (rc==DN_ERR_NONE) {
      // I'm now busy transmitting
      dn_whmt_vars.busyTx         = TRUE;
   }
   
   // unlock the module
   dn_unlock();
   
   return rc;
   
}

void dn_whmt_getParameter_charge_reply(uint8_t cmdId, uint8_t rc, uint8_t* payload, uint8_t len) {
   dn_whmt_getParameter_charge_rpt* reply;
   uint8_t paramId;
   
   // verify I'm expecting this answer
   if (dn_whmt_vars.busyTx==FALSE || dn_whmt_vars.cmdId!=cmdId) {
      return;
   }
   
   // verify I'm expecting this paramId
   paramId = payload[0];
   if (paramId!=dn_whmt_vars.paramId) {
      return;
   }
   
   // verify length
   if (rc==DN_SERIAL_RC_OK && len<DN_GETPARAMETER_CHARGE_REPLY_LEN) {
      return;
   }
   
   // cast the replyContent
   reply = (dn_whmt_getParameter_charge_rpt*)dn_whmt_vars.replyContents;
   
   // store RC
   reply->RC = rc;
   
   // parse returned value (iff RC==0)
   if (rc==DN_SERIAL_RC_OK) {
      
      dn_read_uint32_t(&reply->charge,&payload[DN_GETPARAMETER_CHARGE_REPLY_OFFS_CHARGE]);
      dn_read_uint32_t(&reply->uptime,&payload[DN_GETPARAMETER_CHARGE_REPLY_OFFS_UPTIME]);
      reply->temperature = (int8_t)payload[DN_GETPARAMETER_CHARGE_REPLY_OFFS_TEMPERATURE];
      reply->fractionalTemp = payload[DN_GETPARAMETER_CHARGE_REPLY_OFFS_FRACTIONALTEMP];
   }
   
   // call the callback
   dn_whmt_vars.replyCb(cmdId);
   
   // I'm not busy transmitting anymore
   dn_whmt_vars.busyTx=FALSE;
}

//===== getParameter_testRadioRxStats

/**
The getParameter<testRadioRxStats> command retrieves statistics for the latest 
radio reception test performed using the testRadioRx command. The statistics 
show the number of good and bad packets (CRC failures) received during the 
test. 
*/
dn_err_t dn_whmt_getParameter_testRadioRxStats(dn_whmt_getParameter_testRadioRxStats_rpt* reply) {
   uint8_t    extraFlags;
   dn_err_t   rc;
   
   // lock the module
   dn_lock();
   
   // verify no ongoing transmissions
   if (dn_whmt_vars.busyTx) {
      // unlock the module
      dn_unlock();
      
      // return
      return DN_ERR_BUSY;
   }
   
   // store callback information
   dn_whmt_vars.cmdId          = CMDID_GETPARAMETER;
   dn_whmt_vars.replyContents  = (uint8_t*)reply;
   dn_whmt_vars.paramId        = PARAMID_TESTRADIORXSTATS;
   
   // extraFlags
   extraFlags = 0x00;
   
   // build outputBuf
   dn_whmt_vars.outputBuf[0] = PARAMID_TESTRADIORXSTATS;
   
   // send outputBuf
   rc = dn_serial_mt_sendRequest(
      CMDID_GETPARAMETER,                                       // cmdId
      extraFlags,                                               // extraFlags
      dn_whmt_vars.outputBuf,                                   // payload
      DN_GETPARAMETER_TESTRADIORXSTATS_REQ_LEN,                 // length
      dn_whmt_getParameter_testRadioRxStats_reply               // replyCb
   );
   
   if (rc==DN_ERR_NONE) {
      // I'm now busy transmitting
      dn_whmt_vars.busyTx         = TRUE;
   }
   
   // unlock the module
   dn_unlock();
   
   return rc;
   
}

void dn_whmt_getParameter_testRadioRxStats_reply(uint8_t cmdId, uint8_t rc, uint8_t* payload, uint8_t len) {
   dn_whmt_getParameter_testRadioRxStats_rpt* reply;
   uint8_t paramId;
   
   // verify I'm expecting this answer
   if (dn_whmt_vars.busyTx==FALSE || dn_whmt_vars.cmdId!=cmdId) {
      return;
   }
   
   // verify I'm expecting this paramId
   paramId = payload[0];
   if (paramId!=dn_whmt_vars.paramId) {
      return;
   }
   
   // verify length
   if (rc==DN_SERIAL_RC_OK && len<DN_GETPARAMETER_TESTRADIORXSTATS_REPLY_LEN) {
      return;
   }
   
   // cast the replyContent
   reply = (dn_whmt_getParameter_testRadioRxStats_rpt*)dn_whmt_vars.replyContents;
   
   // store RC
   reply->RC = rc;
   
   // parse returned value (iff RC==0)
   if (rc==DN_SERIAL_RC_OK) {
      
      dn_read_uint16_t(&reply->rxOk,&payload[DN_GETPARAMETER_TESTRADIORXSTATS_REPLY_OFFS_RXOK]);
      dn_read_uint16_t(&reply->rxFailed,&payload[DN_GETPARAMETER_TESTRADIORXSTATS_REPLY_OFFS_RXFAILED]);
   }
   
   // call the callback
   dn_whmt_vars.replyCb(cmdId);
   
   // I'm not busy transmitting anymore
   dn_whmt_vars.busyTx=FALSE;
}

//===== getParameter_lock

/**
The getParameter<lock> command returns the current (RAM resident) lock code and 
locking master. To determine what the lock status will be after reset, use the 
getNVParameter<lock> command.Note: This parameter is available in devices 
running mote software >= 1.1.0 
*/
dn_err_t dn_whmt_getParameter_lock(dn_whmt_getParameter_lock_rpt* reply) {
   uint8_t    extraFlags;
   dn_err_t   rc;
   
   // lock the module
   dn_lock();
   
   // verify no ongoing transmissions
   if (dn_whmt_vars.busyTx) {
      // unlock the module
      dn_unlock();
      
      // return
      return DN_ERR_BUSY;
   }
   
   // store callback information
   dn_whmt_vars.cmdId          = CMDID_GETPARAMETER;
   dn_whmt_vars.replyContents  = (uint8_t*)reply;
   dn_whmt_vars.paramId        = PARAMID_LOCK;
   
   // extraFlags
   extraFlags = 0x00;
   
   // build outputBuf
   dn_whmt_vars.outputBuf[0] = PARAMID_LOCK;
   
   // send outputBuf
   rc = dn_serial_mt_sendRequest(
      CMDID_GETPARAMETER,                                       // cmdId
      extraFlags,                                               // extraFlags
      dn_whmt_vars.outputBuf,                                   // payload
      DN_GETPARAMETER_LOCK_REQ_LEN,                             // length
      dn_whmt_getParameter_lock_reply                           // replyCb
   );
   
   if (rc==DN_ERR_NONE) {
      // I'm now busy transmitting
      dn_whmt_vars.busyTx         = TRUE;
   }
   
   // unlock the module
   dn_unlock();
   
   return rc;
   
}

void dn_whmt_getParameter_lock_reply(uint8_t cmdId, uint8_t rc, uint8_t* payload, uint8_t len) {
   dn_whmt_getParameter_lock_rpt* reply;
   uint8_t paramId;
   
   // verify I'm expecting this answer
   if (dn_whmt_vars.busyTx==FALSE || dn_whmt_vars.cmdId!=cmdId) {
      return;
   }
   
   // verify I'm expecting this paramId
   paramId = payload[0];
   if (paramId!=dn_whmt_vars.paramId) {
      return;
   }
   
   // verify length
   if (rc==DN_SERIAL_RC_OK && len<DN_GETPARAMETER_LOCK_REPLY_LEN) {
      return;
   }
   
   // cast the replyContent
   reply = (dn_whmt_getParameter_lock_rpt*)dn_whmt_vars.replyContents;
   
   // store RC
   reply->RC = rc;
   
   // parse returned value (iff RC==0)
   if (rc==DN_SERIAL_RC_OK) {
      
      reply->code = payload[DN_GETPARAMETER_LOCK_REPLY_OFFS_CODE];
      dn_read_uint16_t(&reply->master,&payload[DN_GETPARAMETER_LOCK_REPLY_OFFS_MASTER]);
   }
   
   // call the callback
   dn_whmt_vars.replyCb(cmdId);
   
   // I'm not busy transmitting anymore
   dn_whmt_vars.busyTx=FALSE;
}

//===== setNVParameter_macAddress

/**
The setNVParameter<macAddress> command may be used to supersede the 
factory-configured MAC address of the mote. 
*/
dn_err_t dn_whmt_setNVParameter_macAddress(uint8_t memory, uint8_t* macAddr, dn_whmt_setNVParameter_macAddress_rpt* reply) {
   uint8_t    extraFlags;
   dn_err_t   rc;
   
   // lock the module
   dn_lock();
   
   // verify no ongoing transmissions
   if (dn_whmt_vars.busyTx) {
      // unlock the module
      dn_unlock();
      
      // return
      return DN_ERR_BUSY;
   }
   
   // store callback information
   dn_whmt_vars.cmdId          = CMDID_SETNVPARAMETER;
   dn_whmt_vars.replyContents  = (uint8_t*)reply;
   dn_whmt_vars.paramId        = PARAMID_MACADDRESS;
   
   // extraFlags
   extraFlags = 0x00;
   if (memory==DN_MEMORY_NV_RAM) {
      extraFlags            |= (1<<7);
   }
   
   // build outputBuf
   dn_whmt_vars.outputBuf[0] = 0; // reserved
   dn_whmt_vars.outputBuf[1] = 0; // reserved
   dn_whmt_vars.outputBuf[2] = 0; // reserved
   dn_whmt_vars.outputBuf[3] = 0; // reserved
   dn_whmt_vars.outputBuf[4] = PARAMID_MACADDRESS;
   memcpy(&dn_whmt_vars.outputBuf[DN_SETNVPARAMETER_MACADDRESS_REQ_OFFS_MACADDR],macAddr,8);
   
   // send outputBuf
   rc = dn_serial_mt_sendRequest(
      CMDID_SETNVPARAMETER,                                     // cmdId
      extraFlags,                                               // extraFlags
      dn_whmt_vars.outputBuf,                                   // payload
      DN_SETNVPARAMETER_MACADDRESS_REQ_LEN,                     // length
      dn_whmt_setNVParameter_macAddress_reply                   // replyCb
   );
   
   if (rc==DN_ERR_NONE) {
      // I'm now busy transmitting
      dn_whmt_vars.busyTx         = TRUE;
   }
   
   // unlock the module
   dn_unlock();
   
   return rc;
   
}

void dn_whmt_setNVParameter_macAddress_reply(uint8_t cmdId, uint8_t rc, uint8_t* payload, uint8_t len) {
   dn_whmt_setNVParameter_macAddress_rpt* reply;
   uint8_t paramId;
   
   // verify I'm expecting this answer
   if (dn_whmt_vars.busyTx==FALSE || dn_whmt_vars.cmdId!=cmdId) {
      return;
   }
   
   // verify I'm expecting this paramId
   paramId = payload[0];
   if (paramId!=dn_whmt_vars.paramId) {
      return;
   }
   
   // verify length
   if (rc==DN_SERIAL_RC_OK && len<DN_SETNVPARAMETER_MACADDRESS_REPLY_LEN) {
      return;
   }
   
   // cast the replyContent
   reply = (dn_whmt_setNVParameter_macAddress_rpt*)dn_whmt_vars.replyContents;
   
   // store RC
   reply->RC = rc;
   
   // parse returned value (iff RC==0)
   if (rc==DN_SERIAL_RC_OK) {
      
   }
   
   // call the callback
   dn_whmt_vars.replyCb(cmdId);
   
   // I'm not busy transmitting anymore
   dn_whmt_vars.busyTx=FALSE;
}

//===== setNVParameter_joinKey

/**
The setNVParameter<joinKey> command may be used to set the join key. Upon 
receiving this request, the mote stores the new join key in its persistent 
storage. Using the write RAM option will only have an effect if the command is 
called while the mote is in Idle state. Otherwise, the new value will be used 
after the next mote boot. 
*/
dn_err_t dn_whmt_setNVParameter_joinKey(uint8_t memory, uint8_t* joinKey, dn_whmt_setNVParameter_joinKey_rpt* reply) {
   uint8_t    extraFlags;
   dn_err_t   rc;
   
   // lock the module
   dn_lock();
   
   // verify no ongoing transmissions
   if (dn_whmt_vars.busyTx) {
      // unlock the module
      dn_unlock();
      
      // return
      return DN_ERR_BUSY;
   }
   
   // store callback information
   dn_whmt_vars.cmdId          = CMDID_SETNVPARAMETER;
   dn_whmt_vars.replyContents  = (uint8_t*)reply;
   dn_whmt_vars.paramId        = PARAMID_JOINKEY;
   
   // extraFlags
   extraFlags = 0x00;
   if (memory==DN_MEMORY_NV_RAM) {
      extraFlags            |= (1<<7);
   }
   
   // build outputBuf
   dn_whmt_vars.outputBuf[0] = 0; // reserved
   dn_whmt_vars.outputBuf[1] = 0; // reserved
   dn_whmt_vars.outputBuf[2] = 0; // reserved
   dn_whmt_vars.outputBuf[3] = 0; // reserved
   dn_whmt_vars.outputBuf[4] = PARAMID_JOINKEY;
   memcpy(&dn_whmt_vars.outputBuf[DN_SETNVPARAMETER_JOINKEY_REQ_OFFS_JOINKEY],joinKey,16);
   
   // send outputBuf
   rc = dn_serial_mt_sendRequest(
      CMDID_SETNVPARAMETER,                                     // cmdId
      extraFlags,                                               // extraFlags
      dn_whmt_vars.outputBuf,                                   // payload
      DN_SETNVPARAMETER_JOINKEY_REQ_LEN,                        // length
      dn_whmt_setNVParameter_joinKey_reply                      // replyCb
   );
   
   if (rc==DN_ERR_NONE) {
      // I'm now busy transmitting
      dn_whmt_vars.busyTx         = TRUE;
   }
   
   // unlock the module
   dn_unlock();
   
   return rc;
   
}

void dn_whmt_setNVParameter_joinKey_reply(uint8_t cmdId, uint8_t rc, uint8_t* payload, uint8_t len) {
   dn_whmt_setNVParameter_joinKey_rpt* reply;
   uint8_t paramId;
   
   // verify I'm expecting this answer
   if (dn_whmt_vars.busyTx==FALSE || dn_whmt_vars.cmdId!=cmdId) {
      return;
   }
   
   // verify I'm expecting this paramId
   paramId = payload[0];
   if (paramId!=dn_whmt_vars.paramId) {
      return;
   }
   
   // verify length
   if (rc==DN_SERIAL_RC_OK && len<DN_SETNVPARAMETER_JOINKEY_REPLY_LEN) {
      return;
   }
   
   // cast the replyContent
   reply = (dn_whmt_setNVParameter_joinKey_rpt*)dn_whmt_vars.replyContents;
   
   // store RC
   reply->RC = rc;
   
   // parse returned value (iff RC==0)
   if (rc==DN_SERIAL_RC_OK) {
      
   }
   
   // call the callback
   dn_whmt_vars.replyCb(cmdId);
   
   // I'm not busy transmitting anymore
   dn_whmt_vars.busyTx=FALSE;
}

//===== setNVParameter_networkId

/**
The setNVParameter<networkId> command may be used to set the persistent Network 
ID of the mote. The networkId is used to separate networks, and can be set 
during manufacturing or in the field.The mote reads this value from persistent 
storage at boot time. Note: while the mote is in Idle state, it is possible to 
update the value of mote's in-RAM Network ID by using the RAM flag in the 
header of this command. This avoids the extra reset that is needed to start 
using the Network ID. Network ID can also be set over the air using HART 
command 773 in a WirelessHART-compliant network.

As of version 1.1.1, a network ID of 0xFFFF can be used to indicate that the 
mote should join the first network heard.

0xFFFF is never used over the air as a valid HART network ID - do not set the 
Manager's network ID to 0xFFFF. 
*/
dn_err_t dn_whmt_setNVParameter_networkId(uint8_t memory, uint16_t networkId, dn_whmt_setNVParameter_networkId_rpt* reply) {
   uint8_t    extraFlags;
   dn_err_t   rc;
   
   // lock the module
   dn_lock();
   
   // verify no ongoing transmissions
   if (dn_whmt_vars.busyTx) {
      // unlock the module
      dn_unlock();
      
      // return
      return DN_ERR_BUSY;
   }
   
   // store callback information
   dn_whmt_vars.cmdId          = CMDID_SETNVPARAMETER;
   dn_whmt_vars.replyContents  = (uint8_t*)reply;
   dn_whmt_vars.paramId        = PARAMID_NETWORKID;
   
   // extraFlags
   extraFlags = 0x00;
   if (memory==DN_MEMORY_NV_RAM) {
      extraFlags            |= (1<<7);
   }
   
   // build outputBuf
   dn_whmt_vars.outputBuf[0] = 0; // reserved
   dn_whmt_vars.outputBuf[1] = 0; // reserved
   dn_whmt_vars.outputBuf[2] = 0; // reserved
   dn_whmt_vars.outputBuf[3] = 0; // reserved
   dn_whmt_vars.outputBuf[4] = PARAMID_NETWORKID;
   dn_write_uint16_t(&dn_whmt_vars.outputBuf[DN_SETNVPARAMETER_NETWORKID_REQ_OFFS_NETWORKID],networkId);
   
   // send outputBuf
   rc = dn_serial_mt_sendRequest(
      CMDID_SETNVPARAMETER,                                     // cmdId
      extraFlags,                                               // extraFlags
      dn_whmt_vars.outputBuf,                                   // payload
      DN_SETNVPARAMETER_NETWORKID_REQ_LEN,                      // length
      dn_whmt_setNVParameter_networkId_reply                    // replyCb
   );
   
   if (rc==DN_ERR_NONE) {
      // I'm now busy transmitting
      dn_whmt_vars.busyTx         = TRUE;
   }
   
   // unlock the module
   dn_unlock();
   
   return rc;
   
}

void dn_whmt_setNVParameter_networkId_reply(uint8_t cmdId, uint8_t rc, uint8_t* payload, uint8_t len) {
   dn_whmt_setNVParameter_networkId_rpt* reply;
   uint8_t paramId;
   
   // verify I'm expecting this answer
   if (dn_whmt_vars.busyTx==FALSE || dn_whmt_vars.cmdId!=cmdId) {
      return;
   }
   
   // verify I'm expecting this paramId
   paramId = payload[0];
   if (paramId!=dn_whmt_vars.paramId) {
      return;
   }
   
   // verify length
   if (rc==DN_SERIAL_RC_OK && len<DN_SETNVPARAMETER_NETWORKID_REPLY_LEN) {
      return;
   }
   
   // cast the replyContent
   reply = (dn_whmt_setNVParameter_networkId_rpt*)dn_whmt_vars.replyContents;
   
   // store RC
   reply->RC = rc;
   
   // parse returned value (iff RC==0)
   if (rc==DN_SERIAL_RC_OK) {
      
   }
   
   // call the callback
   dn_whmt_vars.replyCb(cmdId);
   
   // I'm not busy transmitting anymore
   dn_whmt_vars.busyTx=FALSE;
}

//===== setNVParameter_txPower

/**
The setNVParameter<txPower> command sets the mote output power. Refer to 
product datasheets for supported RF output power values. For example, if the 
mote has a typical RF output power of +8 dBm when the Power Amplifier (PA) is 
enabled, then set the txPower parameter to 8 to enable the PA. Similarly, if 
the mote has a typical RF output power of -2 dBm when the PA is disabled, then 
set the txPower parameter to -2 to turn off the PA. This command may be issued 
at any time and takes effect at the next mote boot. To change the transmit 
power immediately, use the write RAM option of this command, which can also be 
used at any time. 
*/
dn_err_t dn_whmt_setNVParameter_txPower(uint8_t memory, int8_t txPower, dn_whmt_setNVParameter_txPower_rpt* reply) {
   uint8_t    extraFlags;
   dn_err_t   rc;
   
   // lock the module
   dn_lock();
   
   // verify no ongoing transmissions
   if (dn_whmt_vars.busyTx) {
      // unlock the module
      dn_unlock();
      
      // return
      return DN_ERR_BUSY;
   }
   
   // store callback information
   dn_whmt_vars.cmdId          = CMDID_SETNVPARAMETER;
   dn_whmt_vars.replyContents  = (uint8_t*)reply;
   dn_whmt_vars.paramId        = PARAMID_TXPOWER;
   
   // extraFlags
   extraFlags = 0x00;
   if (memory==DN_MEMORY_NV_RAM) {
      extraFlags            |= (1<<7);
   }
   
   // build outputBuf
   dn_whmt_vars.outputBuf[0] = 0; // reserved
   dn_whmt_vars.outputBuf[1] = 0; // reserved
   dn_whmt_vars.outputBuf[2] = 0; // reserved
   dn_whmt_vars.outputBuf[3] = 0; // reserved
   dn_whmt_vars.outputBuf[4] = PARAMID_TXPOWER;
   dn_whmt_vars.outputBuf[DN_SETNVPARAMETER_TXPOWER_REQ_OFFS_TXPOWER] = (int8_t)txPower;
   
   // send outputBuf
   rc = dn_serial_mt_sendRequest(
      CMDID_SETNVPARAMETER,                                     // cmdId
      extraFlags,                                               // extraFlags
      dn_whmt_vars.outputBuf,                                   // payload
      DN_SETNVPARAMETER_TXPOWER_REQ_LEN,                        // length
      dn_whmt_setNVParameter_txPower_reply                      // replyCb
   );
   
   if (rc==DN_ERR_NONE) {
      // I'm now busy transmitting
      dn_whmt_vars.busyTx         = TRUE;
   }
   
   // unlock the module
   dn_unlock();
   
   return rc;
   
}

void dn_whmt_setNVParameter_txPower_reply(uint8_t cmdId, uint8_t rc, uint8_t* payload, uint8_t len) {
   dn_whmt_setNVParameter_txPower_rpt* reply;
   uint8_t paramId;
   
   // verify I'm expecting this answer
   if (dn_whmt_vars.busyTx==FALSE || dn_whmt_vars.cmdId!=cmdId) {
      return;
   }
   
   // verify I'm expecting this paramId
   paramId = payload[0];
   if (paramId!=dn_whmt_vars.paramId) {
      return;
   }
   
   // verify length
   if (rc==DN_SERIAL_RC_OK && len<DN_SETNVPARAMETER_TXPOWER_REPLY_LEN) {
      return;
   }
   
   // cast the replyContent
   reply = (dn_whmt_setNVParameter_txPower_rpt*)dn_whmt_vars.replyContents;
   
   // store RC
   reply->RC = rc;
   
   // parse returned value (iff RC==0)
   if (rc==DN_SERIAL_RC_OK) {
      
   }
   
   // call the callback
   dn_whmt_vars.replyCb(cmdId);
   
   // I'm not busy transmitting anymore
   dn_whmt_vars.busyTx=FALSE;
}

//===== setNVParameter_powerInfo

/**
The setNVParameter<powerInfo> command specifies the average current that is 
available to the mote. Using the write RAM option will only have an effect if 
the command is called while the mote is in Idle state. Otherwise, the new value 
will be used after the next mote boot. 
*/
dn_err_t dn_whmt_setNVParameter_powerInfo(uint8_t memory, uint8_t powerSource, uint16_t dischargeCur, uint32_t dischargeTime, uint32_t recoverTime, dn_whmt_setNVParameter_powerInfo_rpt* reply) {
   uint8_t    extraFlags;
   dn_err_t   rc;
   
   // lock the module
   dn_lock();
   
   // verify no ongoing transmissions
   if (dn_whmt_vars.busyTx) {
      // unlock the module
      dn_unlock();
      
      // return
      return DN_ERR_BUSY;
   }
   
   // store callback information
   dn_whmt_vars.cmdId          = CMDID_SETNVPARAMETER;
   dn_whmt_vars.replyContents  = (uint8_t*)reply;
   dn_whmt_vars.paramId        = PARAMID_POWERINFO;
   
   // extraFlags
   extraFlags = 0x00;
   if (memory==DN_MEMORY_NV_RAM) {
      extraFlags            |= (1<<7);
   }
   
   // build outputBuf
   dn_whmt_vars.outputBuf[0] = 0; // reserved
   dn_whmt_vars.outputBuf[1] = 0; // reserved
   dn_whmt_vars.outputBuf[2] = 0; // reserved
   dn_whmt_vars.outputBuf[3] = 0; // reserved
   dn_whmt_vars.outputBuf[4] = PARAMID_POWERINFO;
   dn_whmt_vars.outputBuf[DN_SETNVPARAMETER_POWERINFO_REQ_OFFS_POWERSOURCE] = powerSource;
   dn_write_uint16_t(&dn_whmt_vars.outputBuf[DN_SETNVPARAMETER_POWERINFO_REQ_OFFS_DISCHARGECUR],dischargeCur);
   dn_write_uint32_t(&dn_whmt_vars.outputBuf[DN_SETNVPARAMETER_POWERINFO_REQ_OFFS_DISCHARGETIME],dischargeTime);
   dn_write_uint32_t(&dn_whmt_vars.outputBuf[DN_SETNVPARAMETER_POWERINFO_REQ_OFFS_RECOVERTIME],recoverTime);
   
   // send outputBuf
   rc = dn_serial_mt_sendRequest(
      CMDID_SETNVPARAMETER,                                     // cmdId
      extraFlags,                                               // extraFlags
      dn_whmt_vars.outputBuf,                                   // payload
      DN_SETNVPARAMETER_POWERINFO_REQ_LEN,                      // length
      dn_whmt_setNVParameter_powerInfo_reply                    // replyCb
   );
   
   if (rc==DN_ERR_NONE) {
      // I'm now busy transmitting
      dn_whmt_vars.busyTx         = TRUE;
   }
   
   // unlock the module
   dn_unlock();
   
   return rc;
   
}

void dn_whmt_setNVParameter_powerInfo_reply(uint8_t cmdId, uint8_t rc, uint8_t* payload, uint8_t len) {
   dn_whmt_setNVParameter_powerInfo_rpt* reply;
   uint8_t paramId;
   
   // verify I'm expecting this answer
   if (dn_whmt_vars.busyTx==FALSE || dn_whmt_vars.cmdId!=cmdId) {
      return;
   }
   
   // verify I'm expecting this paramId
   paramId = payload[0];
   if (paramId!=dn_whmt_vars.paramId) {
      return;
   }
   
   // verify length
   if (rc==DN_SERIAL_RC_OK && len<DN_SETNVPARAMETER_POWERINFO_REPLY_LEN) {
      return;
   }
   
   // cast the replyContent
   reply = (dn_whmt_setNVParameter_powerInfo_rpt*)dn_whmt_vars.replyContents;
   
   // store RC
   reply->RC = rc;
   
   // parse returned value (iff RC==0)
   if (rc==DN_SERIAL_RC_OK) {
      
   }
   
   // call the callback
   dn_whmt_vars.replyCb(cmdId);
   
   // I'm not busy transmitting anymore
   dn_whmt_vars.busyTx=FALSE;
}

//===== setNVParameter_ttl

/**
The setNVParameter<ttl> command sets the mote's persistent packet Time To Live 
(TTL) value. TTL specifies the maximum number of hops a packet may traverse 
before it is discarded from the network. A mote sets the initial value of the 
TTL field in the packets it generates to this value.The mote reads the value 
from persistent storage at boot time. To change the TTL used currently, this 
command may be issued with the RAM option.

The mote defaults TTL to 127. For compliant devices, the HART specification 
currently defaults to 32, but this will change to 249 in spec version 7.4, as 
will the mote default. We suggest not changing the mote default unless HART 
specifically raises it as a compliance issue when you submit your device for 
testing. 
*/
dn_err_t dn_whmt_setNVParameter_ttl(uint8_t memory, uint8_t timeToLive, dn_whmt_setNVParameter_ttl_rpt* reply) {
   uint8_t    extraFlags;
   dn_err_t   rc;
   
   // lock the module
   dn_lock();
   
   // verify no ongoing transmissions
   if (dn_whmt_vars.busyTx) {
      // unlock the module
      dn_unlock();
      
      // return
      return DN_ERR_BUSY;
   }
   
   // store callback information
   dn_whmt_vars.cmdId          = CMDID_SETNVPARAMETER;
   dn_whmt_vars.replyContents  = (uint8_t*)reply;
   dn_whmt_vars.paramId        = PARAMID_TTL;
   
   // extraFlags
   extraFlags = 0x00;
   if (memory==DN_MEMORY_NV_RAM) {
      extraFlags            |= (1<<7);
   }
   
   // build outputBuf
   dn_whmt_vars.outputBuf[0] = 0; // reserved
   dn_whmt_vars.outputBuf[1] = 0; // reserved
   dn_whmt_vars.outputBuf[2] = 0; // reserved
   dn_whmt_vars.outputBuf[3] = 0; // reserved
   dn_whmt_vars.outputBuf[4] = PARAMID_TTL;
   dn_whmt_vars.outputBuf[DN_SETNVPARAMETER_TTL_REQ_OFFS_TIMETOLIVE] = timeToLive;
   
   // send outputBuf
   rc = dn_serial_mt_sendRequest(
      CMDID_SETNVPARAMETER,                                     // cmdId
      extraFlags,                                               // extraFlags
      dn_whmt_vars.outputBuf,                                   // payload
      DN_SETNVPARAMETER_TTL_REQ_LEN,                            // length
      dn_whmt_setNVParameter_ttl_reply                          // replyCb
   );
   
   if (rc==DN_ERR_NONE) {
      // I'm now busy transmitting
      dn_whmt_vars.busyTx         = TRUE;
   }
   
   // unlock the module
   dn_unlock();
   
   return rc;
   
}

void dn_whmt_setNVParameter_ttl_reply(uint8_t cmdId, uint8_t rc, uint8_t* payload, uint8_t len) {
   dn_whmt_setNVParameter_ttl_rpt* reply;
   uint8_t paramId;
   
   // verify I'm expecting this answer
   if (dn_whmt_vars.busyTx==FALSE || dn_whmt_vars.cmdId!=cmdId) {
      return;
   }
   
   // verify I'm expecting this paramId
   paramId = payload[0];
   if (paramId!=dn_whmt_vars.paramId) {
      return;
   }
   
   // verify length
   if (rc==DN_SERIAL_RC_OK && len<DN_SETNVPARAMETER_TTL_REPLY_LEN) {
      return;
   }
   
   // cast the replyContent
   reply = (dn_whmt_setNVParameter_ttl_rpt*)dn_whmt_vars.replyContents;
   
   // store RC
   reply->RC = rc;
   
   // parse returned value (iff RC==0)
   if (rc==DN_SERIAL_RC_OK) {
      
   }
   
   // call the callback
   dn_whmt_vars.replyCb(cmdId);
   
   // I'm not busy transmitting anymore
   dn_whmt_vars.busyTx=FALSE;
}

//===== setNVParameter_HARTantennaGain

/**
The setNVParameter<HARTantennaGain> command stores value of the antenna gain in 
the mote's persistent storage.This value is added to the conducted output power 
of the mote when replying to HART command 797 (Write Radio Power Output) and to 
HART command 798 (Read Radio Output Power). The antenna gain should take into 
account both the gain of the antenna and any loss (for example, attenuation 
from a long coax cable) between the mote and the antenna. By default, this 
value is 2, assuming a +2 dBi antenna gain. To change the transmit power 
immediately, use the write RAM option of this command. 
*/
dn_err_t dn_whmt_setNVParameter_HARTantennaGain(uint8_t memory, int8_t antennaGain, dn_whmt_setNVParameter_HARTantennaGain_rpt* reply) {
   uint8_t    extraFlags;
   dn_err_t   rc;
   
   // lock the module
   dn_lock();
   
   // verify no ongoing transmissions
   if (dn_whmt_vars.busyTx) {
      // unlock the module
      dn_unlock();
      
      // return
      return DN_ERR_BUSY;
   }
   
   // store callback information
   dn_whmt_vars.cmdId          = CMDID_SETNVPARAMETER;
   dn_whmt_vars.replyContents  = (uint8_t*)reply;
   dn_whmt_vars.paramId        = PARAMID_HARTANTENNAGAIN;
   
   // extraFlags
   extraFlags = 0x00;
   if (memory==DN_MEMORY_NV_RAM) {
      extraFlags            |= (1<<7);
   }
   
   // build outputBuf
   dn_whmt_vars.outputBuf[0] = 0; // reserved
   dn_whmt_vars.outputBuf[1] = 0; // reserved
   dn_whmt_vars.outputBuf[2] = 0; // reserved
   dn_whmt_vars.outputBuf[3] = 0; // reserved
   dn_whmt_vars.outputBuf[4] = PARAMID_HARTANTENNAGAIN;
   dn_whmt_vars.outputBuf[DN_SETNVPARAMETER_HARTANTENNAGAIN_REQ_OFFS_ANTENNAGAIN] = (int8_t)antennaGain;
   
   // send outputBuf
   rc = dn_serial_mt_sendRequest(
      CMDID_SETNVPARAMETER,                                     // cmdId
      extraFlags,                                               // extraFlags
      dn_whmt_vars.outputBuf,                                   // payload
      DN_SETNVPARAMETER_HARTANTENNAGAIN_REQ_LEN,                // length
      dn_whmt_setNVParameter_HARTantennaGain_reply              // replyCb
   );
   
   if (rc==DN_ERR_NONE) {
      // I'm now busy transmitting
      dn_whmt_vars.busyTx         = TRUE;
   }
   
   // unlock the module
   dn_unlock();
   
   return rc;
   
}

void dn_whmt_setNVParameter_HARTantennaGain_reply(uint8_t cmdId, uint8_t rc, uint8_t* payload, uint8_t len) {
   dn_whmt_setNVParameter_HARTantennaGain_rpt* reply;
   uint8_t paramId;
   
   // verify I'm expecting this answer
   if (dn_whmt_vars.busyTx==FALSE || dn_whmt_vars.cmdId!=cmdId) {
      return;
   }
   
   // verify I'm expecting this paramId
   paramId = payload[0];
   if (paramId!=dn_whmt_vars.paramId) {
      return;
   }
   
   // verify length
   if (rc==DN_SERIAL_RC_OK && len<DN_SETNVPARAMETER_HARTANTENNAGAIN_REPLY_LEN) {
      return;
   }
   
   // cast the replyContent
   reply = (dn_whmt_setNVParameter_HARTantennaGain_rpt*)dn_whmt_vars.replyContents;
   
   // store RC
   reply->RC = rc;
   
   // parse returned value (iff RC==0)
   if (rc==DN_SERIAL_RC_OK) {
      
   }
   
   // call the callback
   dn_whmt_vars.replyCb(cmdId);
   
   // I'm not busy transmitting anymore
   dn_whmt_vars.busyTx=FALSE;
}

//===== setNVParameter_OTAPlockout

/**
The setNVParameter<OTAPlockout> command specifies whether the mote's firmware 
can be updated over the air. Over-The-Air-Programming (OTAP) is allowed by 
default. The mote reads the OTAPlockout value from persistent storage at boot 
time. To change the value used currently, this command may be issued with RAM option.

Dust Networks recommends that OEMs allow their devices to receive firmware 
updates, either by leaving the OTAPlockout parameter at its default value, or 
by making OTAPlockout settable using a WirelessHART command that is available 
both over the air and through its wired maintenance port. OEMs have the option 
of making such a command password protected. 
*/
dn_err_t dn_whmt_setNVParameter_OTAPlockout(uint8_t memory, uint8_t otapLockout, dn_whmt_setNVParameter_OTAPlockout_rpt* reply) {
   uint8_t    extraFlags;
   dn_err_t   rc;
   
   // lock the module
   dn_lock();
   
   // verify no ongoing transmissions
   if (dn_whmt_vars.busyTx) {
      // unlock the module
      dn_unlock();
      
      // return
      return DN_ERR_BUSY;
   }
   
   // store callback information
   dn_whmt_vars.cmdId          = CMDID_SETNVPARAMETER;
   dn_whmt_vars.replyContents  = (uint8_t*)reply;
   dn_whmt_vars.paramId        = PARAMID_OTAPLOCKOUT;
   
   // extraFlags
   extraFlags = 0x00;
   if (memory==DN_MEMORY_NV_RAM) {
      extraFlags            |= (1<<7);
   }
   
   // build outputBuf
   dn_whmt_vars.outputBuf[0] = 0; // reserved
   dn_whmt_vars.outputBuf[1] = 0; // reserved
   dn_whmt_vars.outputBuf[2] = 0; // reserved
   dn_whmt_vars.outputBuf[3] = 0; // reserved
   dn_whmt_vars.outputBuf[4] = PARAMID_OTAPLOCKOUT;
   dn_whmt_vars.outputBuf[DN_SETNVPARAMETER_OTAPLOCKOUT_REQ_OFFS_OTAPLOCKOUT] = otapLockout;
   
   // send outputBuf
   rc = dn_serial_mt_sendRequest(
      CMDID_SETNVPARAMETER,                                     // cmdId
      extraFlags,                                               // extraFlags
      dn_whmt_vars.outputBuf,                                   // payload
      DN_SETNVPARAMETER_OTAPLOCKOUT_REQ_LEN,                    // length
      dn_whmt_setNVParameter_OTAPlockout_reply                  // replyCb
   );
   
   if (rc==DN_ERR_NONE) {
      // I'm now busy transmitting
      dn_whmt_vars.busyTx         = TRUE;
   }
   
   // unlock the module
   dn_unlock();
   
   return rc;
   
}

void dn_whmt_setNVParameter_OTAPlockout_reply(uint8_t cmdId, uint8_t rc, uint8_t* payload, uint8_t len) {
   dn_whmt_setNVParameter_OTAPlockout_rpt* reply;
   uint8_t paramId;
   
   // verify I'm expecting this answer
   if (dn_whmt_vars.busyTx==FALSE || dn_whmt_vars.cmdId!=cmdId) {
      return;
   }
   
   // verify I'm expecting this paramId
   paramId = payload[0];
   if (paramId!=dn_whmt_vars.paramId) {
      return;
   }
   
   // verify length
   if (rc==DN_SERIAL_RC_OK && len<DN_SETNVPARAMETER_OTAPLOCKOUT_REPLY_LEN) {
      return;
   }
   
   // cast the replyContent
   reply = (dn_whmt_setNVParameter_OTAPlockout_rpt*)dn_whmt_vars.replyContents;
   
   // store RC
   reply->RC = rc;
   
   // parse returned value (iff RC==0)
   if (rc==DN_SERIAL_RC_OK) {
      
   }
   
   // call the callback
   dn_whmt_vars.replyCb(cmdId);
   
   // I'm not busy transmitting anymore
   dn_whmt_vars.busyTx=FALSE;
}

//===== setNVParameter_hrCounterMode

/**
The setNVParameter<hrCounterMode> command may be used to control how the mote 
increments statistics counters reported via HART health reports. The two 
options are "saturating" (i.e. stop counting at maximum value) and "rollover" 
(i.e. continue counting through rollover). The default value of "saturating" is 
required for compatibility with Dust Wireless HART managers. This parameter 
takes effect upon mote reset.

Note: This parameter is available in devices running mote software >=1.1.0 
*/
dn_err_t dn_whmt_setNVParameter_hrCounterMode(uint8_t memory, uint8_t hrCounterMode, dn_whmt_setNVParameter_hrCounterMode_rpt* reply) {
   uint8_t    extraFlags;
   dn_err_t   rc;
   
   // lock the module
   dn_lock();
   
   // verify no ongoing transmissions
   if (dn_whmt_vars.busyTx) {
      // unlock the module
      dn_unlock();
      
      // return
      return DN_ERR_BUSY;
   }
   
   // store callback information
   dn_whmt_vars.cmdId          = CMDID_SETNVPARAMETER;
   dn_whmt_vars.replyContents  = (uint8_t*)reply;
   dn_whmt_vars.paramId        = PARAMID_HRCOUNTERMODE;
   
   // extraFlags
   extraFlags = 0x00;
   if (memory==DN_MEMORY_NV_RAM) {
      extraFlags            |= (1<<7);
   }
   
   // build outputBuf
   dn_whmt_vars.outputBuf[0] = 0; // reserved
   dn_whmt_vars.outputBuf[1] = 0; // reserved
   dn_whmt_vars.outputBuf[2] = 0; // reserved
   dn_whmt_vars.outputBuf[3] = 0; // reserved
   dn_whmt_vars.outputBuf[4] = PARAMID_HRCOUNTERMODE;
   dn_whmt_vars.outputBuf[DN_SETNVPARAMETER_HRCOUNTERMODE_REQ_OFFS_HRCOUNTERMODE] = hrCounterMode;
   
   // send outputBuf
   rc = dn_serial_mt_sendRequest(
      CMDID_SETNVPARAMETER,                                     // cmdId
      extraFlags,                                               // extraFlags
      dn_whmt_vars.outputBuf,                                   // payload
      DN_SETNVPARAMETER_HRCOUNTERMODE_REQ_LEN,                  // length
      dn_whmt_setNVParameter_hrCounterMode_reply                // replyCb
   );
   
   if (rc==DN_ERR_NONE) {
      // I'm now busy transmitting
      dn_whmt_vars.busyTx         = TRUE;
   }
   
   // unlock the module
   dn_unlock();
   
   return rc;
   
}

void dn_whmt_setNVParameter_hrCounterMode_reply(uint8_t cmdId, uint8_t rc, uint8_t* payload, uint8_t len) {
   dn_whmt_setNVParameter_hrCounterMode_rpt* reply;
   uint8_t paramId;
   
   // verify I'm expecting this answer
   if (dn_whmt_vars.busyTx==FALSE || dn_whmt_vars.cmdId!=cmdId) {
      return;
   }
   
   // verify I'm expecting this paramId
   paramId = payload[0];
   if (paramId!=dn_whmt_vars.paramId) {
      return;
   }
   
   // verify length
   if (rc==DN_SERIAL_RC_OK && len<DN_SETNVPARAMETER_HRCOUNTERMODE_REPLY_LEN) {
      return;
   }
   
   // cast the replyContent
   reply = (dn_whmt_setNVParameter_hrCounterMode_rpt*)dn_whmt_vars.replyContents;
   
   // store RC
   reply->RC = rc;
   
   // parse returned value (iff RC==0)
   if (rc==DN_SERIAL_RC_OK) {
      
   }
   
   // call the callback
   dn_whmt_vars.replyCb(cmdId);
   
   // I'm not busy transmitting anymore
   dn_whmt_vars.busyTx=FALSE;
}

//===== setNVParameter_autojoin

/**
The setNVParameter<autojoin> command allows the microprocessor to change 
between automatic and manual joining by the mote's networking stack. In manual 
mode, an explicit join command from the application is required to initiate 
joining. This setting is persistent and takes effect after mote reset. 
(Available Mote >= 1.1) Note that auto join mode must not be set if the 
application is also configured to join (e.g combining 'auto join' with 'master' 
mode will result in mote not joining). 
*/
dn_err_t dn_whmt_setNVParameter_autojoin(uint8_t memory, uint32_t reserved, uint8_t nvParamId, uint8_t autojoin, dn_whmt_setNVParameter_autojoin_rpt* reply) {
   uint8_t    extraFlags;
   dn_err_t   rc;
   
   // lock the module
   dn_lock();
   
   // verify no ongoing transmissions
   if (dn_whmt_vars.busyTx) {
      // unlock the module
      dn_unlock();
      
      // return
      return DN_ERR_BUSY;
   }
   
   // store callback information
   dn_whmt_vars.cmdId          = CMDID_SETNVPARAMETER;
   dn_whmt_vars.replyContents  = (uint8_t*)reply;
   dn_whmt_vars.paramId        = PARAMID_AUTOJOIN;
   
   // extraFlags
   extraFlags = 0x00;
   if (memory==DN_MEMORY_NV_RAM) {
      extraFlags            |= (1<<7);
   }
   
   // build outputBuf
   dn_whmt_vars.outputBuf[0] = 0; // reserved
   dn_whmt_vars.outputBuf[1] = 0; // reserved
   dn_whmt_vars.outputBuf[2] = 0; // reserved
   dn_whmt_vars.outputBuf[3] = 0; // reserved
   dn_whmt_vars.outputBuf[4] = PARAMID_AUTOJOIN;
   dn_write_uint32_t(&dn_whmt_vars.outputBuf[DN_SETNVPARAMETER_AUTOJOIN_REQ_OFFS_RESERVED],reserved);
   dn_whmt_vars.outputBuf[DN_SETNVPARAMETER_AUTOJOIN_REQ_OFFS_NVPARAMID] = nvParamId;
   dn_whmt_vars.outputBuf[DN_SETNVPARAMETER_AUTOJOIN_REQ_OFFS_AUTOJOIN] = autojoin;
   
   // send outputBuf
   rc = dn_serial_mt_sendRequest(
      CMDID_SETNVPARAMETER,                                     // cmdId
      extraFlags,                                               // extraFlags
      dn_whmt_vars.outputBuf,                                   // payload
      DN_SETNVPARAMETER_AUTOJOIN_REQ_LEN,                       // length
      dn_whmt_setNVParameter_autojoin_reply                     // replyCb
   );
   
   if (rc==DN_ERR_NONE) {
      // I'm now busy transmitting
      dn_whmt_vars.busyTx         = TRUE;
   }
   
   // unlock the module
   dn_unlock();
   
   return rc;
   
}

void dn_whmt_setNVParameter_autojoin_reply(uint8_t cmdId, uint8_t rc, uint8_t* payload, uint8_t len) {
   dn_whmt_setNVParameter_autojoin_rpt* reply;
   uint8_t paramId;
   
   // verify I'm expecting this answer
   if (dn_whmt_vars.busyTx==FALSE || dn_whmt_vars.cmdId!=cmdId) {
      return;
   }
   
   // verify I'm expecting this paramId
   paramId = payload[0];
   if (paramId!=dn_whmt_vars.paramId) {
      return;
   }
   
   // verify length
   if (rc==DN_SERIAL_RC_OK && len<DN_SETNVPARAMETER_AUTOJOIN_REPLY_LEN) {
      return;
   }
   
   // cast the replyContent
   reply = (dn_whmt_setNVParameter_autojoin_rpt*)dn_whmt_vars.replyContents;
   
   // store RC
   reply->RC = rc;
   
   // parse returned value (iff RC==0)
   if (rc==DN_SERIAL_RC_OK) {
      
      reply->nvParamId = payload[DN_SETNVPARAMETER_AUTOJOIN_REPLY_OFFS_NVPARAMID];
   }
   
   // call the callback
   dn_whmt_vars.replyCb(cmdId);
   
   // I'm not busy transmitting anymore
   dn_whmt_vars.busyTx=FALSE;
}

//===== setNVParameter_compliantMode

/**
The setNVParameter<compliantMode> command may be used to force strict 
compliance to HART specification requirements, specifically:

- join timeouts (faster in non-compliant mode)
- Keepalive interval (adapts to synch quality in non-compliant mode)
- Health report format (uses saturating counters in non-compliant mode)

Note: This parameter is available in devices running mote software >= 1.1.0 
*/
dn_err_t dn_whmt_setNVParameter_compliantMode(uint8_t memory, uint8_t compliantMode, dn_whmt_setNVParameter_compliantMode_rpt* reply) {
   uint8_t    extraFlags;
   dn_err_t   rc;
   
   // lock the module
   dn_lock();
   
   // verify no ongoing transmissions
   if (dn_whmt_vars.busyTx) {
      // unlock the module
      dn_unlock();
      
      // return
      return DN_ERR_BUSY;
   }
   
   // store callback information
   dn_whmt_vars.cmdId          = CMDID_SETNVPARAMETER;
   dn_whmt_vars.replyContents  = (uint8_t*)reply;
   dn_whmt_vars.paramId        = PARAMID_COMPLIANTMODE;
   
   // extraFlags
   extraFlags = 0x00;
   if (memory==DN_MEMORY_NV_RAM) {
      extraFlags            |= (1<<7);
   }
   
   // build outputBuf
   dn_whmt_vars.outputBuf[0] = 0; // reserved
   dn_whmt_vars.outputBuf[1] = 0; // reserved
   dn_whmt_vars.outputBuf[2] = 0; // reserved
   dn_whmt_vars.outputBuf[3] = 0; // reserved
   dn_whmt_vars.outputBuf[4] = PARAMID_COMPLIANTMODE;
   dn_whmt_vars.outputBuf[DN_SETNVPARAMETER_COMPLIANTMODE_REQ_OFFS_COMPLIANTMODE] = compliantMode;
   
   // send outputBuf
   rc = dn_serial_mt_sendRequest(
      CMDID_SETNVPARAMETER,                                     // cmdId
      extraFlags,                                               // extraFlags
      dn_whmt_vars.outputBuf,                                   // payload
      DN_SETNVPARAMETER_COMPLIANTMODE_REQ_LEN,                  // length
      dn_whmt_setNVParameter_compliantMode_reply                // replyCb
   );
   
   if (rc==DN_ERR_NONE) {
      // I'm now busy transmitting
      dn_whmt_vars.busyTx         = TRUE;
   }
   
   // unlock the module
   dn_unlock();
   
   return rc;
   
}

void dn_whmt_setNVParameter_compliantMode_reply(uint8_t cmdId, uint8_t rc, uint8_t* payload, uint8_t len) {
   dn_whmt_setNVParameter_compliantMode_rpt* reply;
   uint8_t paramId;
   
   // verify I'm expecting this answer
   if (dn_whmt_vars.busyTx==FALSE || dn_whmt_vars.cmdId!=cmdId) {
      return;
   }
   
   // verify I'm expecting this paramId
   paramId = payload[0];
   if (paramId!=dn_whmt_vars.paramId) {
      return;
   }
   
   // verify length
   if (rc==DN_SERIAL_RC_OK && len<DN_SETNVPARAMETER_COMPLIANTMODE_REPLY_LEN) {
      return;
   }
   
   // cast the replyContent
   reply = (dn_whmt_setNVParameter_compliantMode_rpt*)dn_whmt_vars.replyContents;
   
   // store RC
   reply->RC = rc;
   
   // parse returned value (iff RC==0)
   if (rc==DN_SERIAL_RC_OK) {
      
   }
   
   // call the callback
   dn_whmt_vars.replyCb(cmdId);
   
   // I'm not busy transmitting anymore
   dn_whmt_vars.busyTx=FALSE;
}

//===== setNVParameter_lock

/**
The setNVParameter<lock> command persistently locks/unlocks select HART 
commands (ones that affect the configuration changed flag) to a specific master 
(GW or serial maintenance port) to prevent the other master from changing it. 
This command is intended for use when the lock persists through power cycle or 
reset. For temporary locking, use the setParameter<lock> command. Bit 7 in the 
flags field of the API header (see Packet Format) should be set (store in NV & 
RAM) when calling this command.Note: This parameter is available in devices 
running mote software >= 1.1.0 
*/
dn_err_t dn_whmt_setNVParameter_lock(uint8_t memory, uint8_t code, uint16_t master, dn_whmt_setNVParameter_lock_rpt* reply) {
   uint8_t    extraFlags;
   dn_err_t   rc;
   
   // lock the module
   dn_lock();
   
   // verify no ongoing transmissions
   if (dn_whmt_vars.busyTx) {
      // unlock the module
      dn_unlock();
      
      // return
      return DN_ERR_BUSY;
   }
   
   // store callback information
   dn_whmt_vars.cmdId          = CMDID_SETNVPARAMETER;
   dn_whmt_vars.replyContents  = (uint8_t*)reply;
   dn_whmt_vars.paramId        = PARAMID_LOCK;
   
   // extraFlags
   extraFlags = 0x00;
   if (memory==DN_MEMORY_NV_RAM) {
      extraFlags            |= (1<<7);
   }
   
   // build outputBuf
   dn_whmt_vars.outputBuf[0] = 0; // reserved
   dn_whmt_vars.outputBuf[1] = 0; // reserved
   dn_whmt_vars.outputBuf[2] = 0; // reserved
   dn_whmt_vars.outputBuf[3] = 0; // reserved
   dn_whmt_vars.outputBuf[4] = PARAMID_LOCK;
   dn_whmt_vars.outputBuf[DN_SETNVPARAMETER_LOCK_REQ_OFFS_CODE] = code;
   dn_write_uint16_t(&dn_whmt_vars.outputBuf[DN_SETNVPARAMETER_LOCK_REQ_OFFS_MASTER],master);
   
   // send outputBuf
   rc = dn_serial_mt_sendRequest(
      CMDID_SETNVPARAMETER,                                     // cmdId
      extraFlags,                                               // extraFlags
      dn_whmt_vars.outputBuf,                                   // payload
      DN_SETNVPARAMETER_LOCK_REQ_LEN,                           // length
      dn_whmt_setNVParameter_lock_reply                         // replyCb
   );
   
   if (rc==DN_ERR_NONE) {
      // I'm now busy transmitting
      dn_whmt_vars.busyTx         = TRUE;
   }
   
   // unlock the module
   dn_unlock();
   
   return rc;
   
}

void dn_whmt_setNVParameter_lock_reply(uint8_t cmdId, uint8_t rc, uint8_t* payload, uint8_t len) {
   dn_whmt_setNVParameter_lock_rpt* reply;
   uint8_t paramId;
   
   // verify I'm expecting this answer
   if (dn_whmt_vars.busyTx==FALSE || dn_whmt_vars.cmdId!=cmdId) {
      return;
   }
   
   // verify I'm expecting this paramId
   paramId = payload[0];
   if (paramId!=dn_whmt_vars.paramId) {
      return;
   }
   
   // verify length
   if (rc==DN_SERIAL_RC_OK && len<DN_SETNVPARAMETER_LOCK_REPLY_LEN) {
      return;
   }
   
   // cast the replyContent
   reply = (dn_whmt_setNVParameter_lock_rpt*)dn_whmt_vars.replyContents;
   
   // store RC
   reply->RC = rc;
   
   // parse returned value (iff RC==0)
   if (rc==DN_SERIAL_RC_OK) {
      
   }
   
   // call the callback
   dn_whmt_vars.replyCb(cmdId);
   
   // I'm not busy transmitting anymore
   dn_whmt_vars.busyTx=FALSE;
}

//===== getNVParameter_macAddress

/**
The getNVParameter<macAddress> command returns the MAC address stored in mote's 
persistent storage (i.e. set with setNVParameter<macAddress>).

This command returns 0000000000000000 if the macAddress has not been set 
previously - the mote will use its hardware MAC in this case, but it is not 
displayed with this command. 
*/
dn_err_t dn_whmt_getNVParameter_macAddress(dn_whmt_getNVParameter_macAddress_rpt* reply) {
   uint8_t    extraFlags;
   dn_err_t   rc;
   
   // lock the module
   dn_lock();
   
   // verify no ongoing transmissions
   if (dn_whmt_vars.busyTx) {
      // unlock the module
      dn_unlock();
      
      // return
      return DN_ERR_BUSY;
   }
   
   // store callback information
   dn_whmt_vars.cmdId          = CMDID_GETNVPARAMETER;
   dn_whmt_vars.replyContents  = (uint8_t*)reply;
   dn_whmt_vars.paramId        = PARAMID_MACADDRESS;
   
   // extraFlags
   extraFlags = 0x00;
   
   // build outputBuf
   dn_whmt_vars.outputBuf[0] = 0; // reserved
   dn_whmt_vars.outputBuf[1] = 0; // reserved
   dn_whmt_vars.outputBuf[2] = 0; // reserved
   dn_whmt_vars.outputBuf[3] = 0; // reserved
   dn_whmt_vars.outputBuf[4] = PARAMID_MACADDRESS;
   
   // send outputBuf
   rc = dn_serial_mt_sendRequest(
      CMDID_GETNVPARAMETER,                                     // cmdId
      extraFlags,                                               // extraFlags
      dn_whmt_vars.outputBuf,                                   // payload
      DN_GETNVPARAMETER_MACADDRESS_REQ_LEN,                     // length
      dn_whmt_getNVParameter_macAddress_reply                   // replyCb
   );
   
   if (rc==DN_ERR_NONE) {
      // I'm now busy transmitting
      dn_whmt_vars.busyTx         = TRUE;
   }
   
   // unlock the module
   dn_unlock();
   
   return rc;
   
}

void dn_whmt_getNVParameter_macAddress_reply(uint8_t cmdId, uint8_t rc, uint8_t* payload, uint8_t len) {
   dn_whmt_getNVParameter_macAddress_rpt* reply;
   uint8_t paramId;
   
   // verify I'm expecting this answer
   if (dn_whmt_vars.busyTx==FALSE || dn_whmt_vars.cmdId!=cmdId) {
      return;
   }
   
   // verify I'm expecting this paramId
   paramId = payload[0];
   if (paramId!=dn_whmt_vars.paramId) {
      return;
   }
   
   // verify length
   if (rc==DN_SERIAL_RC_OK && len<DN_GETNVPARAMETER_MACADDRESS_REPLY_LEN) {
      return;
   }
   
   // cast the replyContent
   reply = (dn_whmt_getNVParameter_macAddress_rpt*)dn_whmt_vars.replyContents;
   
   // store RC
   reply->RC = rc;
   
   // parse returned value (iff RC==0)
   if (rc==DN_SERIAL_RC_OK) {
      
      memcpy(&reply->macAddr[0],&payload[DN_GETNVPARAMETER_MACADDRESS_REPLY_OFFS_MACADDR],8);
   }
   
   // call the callback
   dn_whmt_vars.replyCb(cmdId);
   
   // I'm not busy transmitting anymore
   dn_whmt_vars.busyTx=FALSE;
}

//===== getNVParameter_networkId

/**
The getNVParameter<networkId> command returns the Network ID stored in mote's 
persistent storage. 
*/
dn_err_t dn_whmt_getNVParameter_networkId(dn_whmt_getNVParameter_networkId_rpt* reply) {
   uint8_t    extraFlags;
   dn_err_t   rc;
   
   // lock the module
   dn_lock();
   
   // verify no ongoing transmissions
   if (dn_whmt_vars.busyTx) {
      // unlock the module
      dn_unlock();
      
      // return
      return DN_ERR_BUSY;
   }
   
   // store callback information
   dn_whmt_vars.cmdId          = CMDID_GETNVPARAMETER;
   dn_whmt_vars.replyContents  = (uint8_t*)reply;
   dn_whmt_vars.paramId        = PARAMID_NETWORKID;
   
   // extraFlags
   extraFlags = 0x00;
   
   // build outputBuf
   dn_whmt_vars.outputBuf[0] = 0; // reserved
   dn_whmt_vars.outputBuf[1] = 0; // reserved
   dn_whmt_vars.outputBuf[2] = 0; // reserved
   dn_whmt_vars.outputBuf[3] = 0; // reserved
   dn_whmt_vars.outputBuf[4] = PARAMID_NETWORKID;
   
   // send outputBuf
   rc = dn_serial_mt_sendRequest(
      CMDID_GETNVPARAMETER,                                     // cmdId
      extraFlags,                                               // extraFlags
      dn_whmt_vars.outputBuf,                                   // payload
      DN_GETNVPARAMETER_NETWORKID_REQ_LEN,                      // length
      dn_whmt_getNVParameter_networkId_reply                    // replyCb
   );
   
   if (rc==DN_ERR_NONE) {
      // I'm now busy transmitting
      dn_whmt_vars.busyTx         = TRUE;
   }
   
   // unlock the module
   dn_unlock();
   
   return rc;
   
}

void dn_whmt_getNVParameter_networkId_reply(uint8_t cmdId, uint8_t rc, uint8_t* payload, uint8_t len) {
   dn_whmt_getNVParameter_networkId_rpt* reply;
   uint8_t paramId;
   
   // verify I'm expecting this answer
   if (dn_whmt_vars.busyTx==FALSE || dn_whmt_vars.cmdId!=cmdId) {
      return;
   }
   
   // verify I'm expecting this paramId
   paramId = payload[0];
   if (paramId!=dn_whmt_vars.paramId) {
      return;
   }
   
   // verify length
   if (rc==DN_SERIAL_RC_OK && len<DN_GETNVPARAMETER_NETWORKID_REPLY_LEN) {
      return;
   }
   
   // cast the replyContent
   reply = (dn_whmt_getNVParameter_networkId_rpt*)dn_whmt_vars.replyContents;
   
   // store RC
   reply->RC = rc;
   
   // parse returned value (iff RC==0)
   if (rc==DN_SERIAL_RC_OK) {
      
      dn_read_uint16_t(&reply->networkId,&payload[DN_GETNVPARAMETER_NETWORKID_REPLY_OFFS_NETWORKID]);
   }
   
   // call the callback
   dn_whmt_vars.replyCb(cmdId);
   
   // I'm not busy transmitting anymore
   dn_whmt_vars.busyTx=FALSE;
}

//===== getNVParameter_txPower

/**
The getNVParameter<txPower> command returns the transmit power value stored in 
mote's persistent storage. 
*/
dn_err_t dn_whmt_getNVParameter_txPower(dn_whmt_getNVParameter_txPower_rpt* reply) {
   uint8_t    extraFlags;
   dn_err_t   rc;
   
   // lock the module
   dn_lock();
   
   // verify no ongoing transmissions
   if (dn_whmt_vars.busyTx) {
      // unlock the module
      dn_unlock();
      
      // return
      return DN_ERR_BUSY;
   }
   
   // store callback information
   dn_whmt_vars.cmdId          = CMDID_GETNVPARAMETER;
   dn_whmt_vars.replyContents  = (uint8_t*)reply;
   dn_whmt_vars.paramId        = PARAMID_TXPOWER;
   
   // extraFlags
   extraFlags = 0x00;
   
   // build outputBuf
   dn_whmt_vars.outputBuf[0] = 0; // reserved
   dn_whmt_vars.outputBuf[1] = 0; // reserved
   dn_whmt_vars.outputBuf[2] = 0; // reserved
   dn_whmt_vars.outputBuf[3] = 0; // reserved
   dn_whmt_vars.outputBuf[4] = PARAMID_TXPOWER;
   
   // send outputBuf
   rc = dn_serial_mt_sendRequest(
      CMDID_GETNVPARAMETER,                                     // cmdId
      extraFlags,                                               // extraFlags
      dn_whmt_vars.outputBuf,                                   // payload
      DN_GETNVPARAMETER_TXPOWER_REQ_LEN,                        // length
      dn_whmt_getNVParameter_txPower_reply                      // replyCb
   );
   
   if (rc==DN_ERR_NONE) {
      // I'm now busy transmitting
      dn_whmt_vars.busyTx         = TRUE;
   }
   
   // unlock the module
   dn_unlock();
   
   return rc;
   
}

void dn_whmt_getNVParameter_txPower_reply(uint8_t cmdId, uint8_t rc, uint8_t* payload, uint8_t len) {
   dn_whmt_getNVParameter_txPower_rpt* reply;
   uint8_t paramId;
   
   // verify I'm expecting this answer
   if (dn_whmt_vars.busyTx==FALSE || dn_whmt_vars.cmdId!=cmdId) {
      return;
   }
   
   // verify I'm expecting this paramId
   paramId = payload[0];
   if (paramId!=dn_whmt_vars.paramId) {
      return;
   }
   
   // verify length
   if (rc==DN_SERIAL_RC_OK && len<DN_GETNVPARAMETER_TXPOWER_REPLY_LEN) {
      return;
   }
   
   // cast the replyContent
   reply = (dn_whmt_getNVParameter_txPower_rpt*)dn_whmt_vars.replyContents;
   
   // store RC
   reply->RC = rc;
   
   // parse returned value (iff RC==0)
   if (rc==DN_SERIAL_RC_OK) {
      
      reply->txPower = (int8_t)payload[DN_GETNVPARAMETER_TXPOWER_REPLY_OFFS_TXPOWER];
   }
   
   // call the callback
   dn_whmt_vars.replyCb(cmdId);
   
   // I'm not busy transmitting anymore
   dn_whmt_vars.busyTx=FALSE;
}

//===== getNVParameter_powerInfo

/**
The getNVParameter<powerInfo> command returns the power supply information 
stored in mote's persistent storage. 
*/
dn_err_t dn_whmt_getNVParameter_powerInfo(dn_whmt_getNVParameter_powerInfo_rpt* reply) {
   uint8_t    extraFlags;
   dn_err_t   rc;
   
   // lock the module
   dn_lock();
   
   // verify no ongoing transmissions
   if (dn_whmt_vars.busyTx) {
      // unlock the module
      dn_unlock();
      
      // return
      return DN_ERR_BUSY;
   }
   
   // store callback information
   dn_whmt_vars.cmdId          = CMDID_GETNVPARAMETER;
   dn_whmt_vars.replyContents  = (uint8_t*)reply;
   dn_whmt_vars.paramId        = PARAMID_POWERINFO;
   
   // extraFlags
   extraFlags = 0x00;
   
   // build outputBuf
   dn_whmt_vars.outputBuf[0] = 0; // reserved
   dn_whmt_vars.outputBuf[1] = 0; // reserved
   dn_whmt_vars.outputBuf[2] = 0; // reserved
   dn_whmt_vars.outputBuf[3] = 0; // reserved
   dn_whmt_vars.outputBuf[4] = PARAMID_POWERINFO;
   
   // send outputBuf
   rc = dn_serial_mt_sendRequest(
      CMDID_GETNVPARAMETER,                                     // cmdId
      extraFlags,                                               // extraFlags
      dn_whmt_vars.outputBuf,                                   // payload
      DN_GETNVPARAMETER_POWERINFO_REQ_LEN,                      // length
      dn_whmt_getNVParameter_powerInfo_reply                    // replyCb
   );
   
   if (rc==DN_ERR_NONE) {
      // I'm now busy transmitting
      dn_whmt_vars.busyTx         = TRUE;
   }
   
   // unlock the module
   dn_unlock();
   
   return rc;
   
}

void dn_whmt_getNVParameter_powerInfo_reply(uint8_t cmdId, uint8_t rc, uint8_t* payload, uint8_t len) {
   dn_whmt_getNVParameter_powerInfo_rpt* reply;
   uint8_t paramId;
   
   // verify I'm expecting this answer
   if (dn_whmt_vars.busyTx==FALSE || dn_whmt_vars.cmdId!=cmdId) {
      return;
   }
   
   // verify I'm expecting this paramId
   paramId = payload[0];
   if (paramId!=dn_whmt_vars.paramId) {
      return;
   }
   
   // verify length
   if (rc==DN_SERIAL_RC_OK && len<DN_GETNVPARAMETER_POWERINFO_REPLY_LEN) {
      return;
   }
   
   // cast the replyContent
   reply = (dn_whmt_getNVParameter_powerInfo_rpt*)dn_whmt_vars.replyContents;
   
   // store RC
   reply->RC = rc;
   
   // parse returned value (iff RC==0)
   if (rc==DN_SERIAL_RC_OK) {
      
      reply->powerSource = payload[DN_GETNVPARAMETER_POWERINFO_REPLY_OFFS_POWERSOURCE];
      dn_read_uint16_t(&reply->dischargeCur,&payload[DN_GETNVPARAMETER_POWERINFO_REPLY_OFFS_DISCHARGECUR]);
      dn_read_uint32_t(&reply->dischargeTime,&payload[DN_GETNVPARAMETER_POWERINFO_REPLY_OFFS_DISCHARGETIME]);
      dn_read_uint32_t(&reply->recoverTime,&payload[DN_GETNVPARAMETER_POWERINFO_REPLY_OFFS_RECOVERTIME]);
   }
   
   // call the callback
   dn_whmt_vars.replyCb(cmdId);
   
   // I'm not busy transmitting anymore
   dn_whmt_vars.busyTx=FALSE;
}

//===== getNVParameter_ttl

/**
The getNVParameter<ttl> command reads the Time To Live parameter from the 
mote's persistent storage. Time To Live is used when the mote sends a packet 
into the network, and specifies the maximum number of hops the packet may 
traverse before it is discarded from the network. 
*/
dn_err_t dn_whmt_getNVParameter_ttl(dn_whmt_getNVParameter_ttl_rpt* reply) {
   uint8_t    extraFlags;
   dn_err_t   rc;
   
   // lock the module
   dn_lock();
   
   // verify no ongoing transmissions
   if (dn_whmt_vars.busyTx) {
      // unlock the module
      dn_unlock();
      
      // return
      return DN_ERR_BUSY;
   }
   
   // store callback information
   dn_whmt_vars.cmdId          = CMDID_GETNVPARAMETER;
   dn_whmt_vars.replyContents  = (uint8_t*)reply;
   dn_whmt_vars.paramId        = PARAMID_TTL;
   
   // extraFlags
   extraFlags = 0x00;
   
   // build outputBuf
   dn_whmt_vars.outputBuf[0] = 0; // reserved
   dn_whmt_vars.outputBuf[1] = 0; // reserved
   dn_whmt_vars.outputBuf[2] = 0; // reserved
   dn_whmt_vars.outputBuf[3] = 0; // reserved
   dn_whmt_vars.outputBuf[4] = PARAMID_TTL;
   
   // send outputBuf
   rc = dn_serial_mt_sendRequest(
      CMDID_GETNVPARAMETER,                                     // cmdId
      extraFlags,                                               // extraFlags
      dn_whmt_vars.outputBuf,                                   // payload
      DN_GETNVPARAMETER_TTL_REQ_LEN,                            // length
      dn_whmt_getNVParameter_ttl_reply                          // replyCb
   );
   
   if (rc==DN_ERR_NONE) {
      // I'm now busy transmitting
      dn_whmt_vars.busyTx         = TRUE;
   }
   
   // unlock the module
   dn_unlock();
   
   return rc;
   
}

void dn_whmt_getNVParameter_ttl_reply(uint8_t cmdId, uint8_t rc, uint8_t* payload, uint8_t len) {
   dn_whmt_getNVParameter_ttl_rpt* reply;
   uint8_t paramId;
   
   // verify I'm expecting this answer
   if (dn_whmt_vars.busyTx==FALSE || dn_whmt_vars.cmdId!=cmdId) {
      return;
   }
   
   // verify I'm expecting this paramId
   paramId = payload[0];
   if (paramId!=dn_whmt_vars.paramId) {
      return;
   }
   
   // verify length
   if (rc==DN_SERIAL_RC_OK && len<DN_GETNVPARAMETER_TTL_REPLY_LEN) {
      return;
   }
   
   // cast the replyContent
   reply = (dn_whmt_getNVParameter_ttl_rpt*)dn_whmt_vars.replyContents;
   
   // store RC
   reply->RC = rc;
   
   // parse returned value (iff RC==0)
   if (rc==DN_SERIAL_RC_OK) {
      
      reply->timeToLive = payload[DN_GETNVPARAMETER_TTL_REPLY_OFFS_TIMETOLIVE];
   }
   
   // call the callback
   dn_whmt_vars.replyCb(cmdId);
   
   // I'm not busy transmitting anymore
   dn_whmt_vars.busyTx=FALSE;
}

//===== getNVParameter_HARTantennaGain

/**
The getNVParameter<HARTantennaGain> command reads the antenna gain value from 
the mote's persistent storage. This value is added to conducted output power of 
the Dust mote when replying to HART command 797 (Write Radio Power Output) and 
to HART command 798 (Read Radio Output Power). 
*/
dn_err_t dn_whmt_getNVParameter_HARTantennaGain(dn_whmt_getNVParameter_HARTantennaGain_rpt* reply) {
   uint8_t    extraFlags;
   dn_err_t   rc;
   
   // lock the module
   dn_lock();
   
   // verify no ongoing transmissions
   if (dn_whmt_vars.busyTx) {
      // unlock the module
      dn_unlock();
      
      // return
      return DN_ERR_BUSY;
   }
   
   // store callback information
   dn_whmt_vars.cmdId          = CMDID_GETNVPARAMETER;
   dn_whmt_vars.replyContents  = (uint8_t*)reply;
   dn_whmt_vars.paramId        = PARAMID_HARTANTENNAGAIN;
   
   // extraFlags
   extraFlags = 0x00;
   
   // build outputBuf
   dn_whmt_vars.outputBuf[0] = 0; // reserved
   dn_whmt_vars.outputBuf[1] = 0; // reserved
   dn_whmt_vars.outputBuf[2] = 0; // reserved
   dn_whmt_vars.outputBuf[3] = 0; // reserved
   dn_whmt_vars.outputBuf[4] = PARAMID_HARTANTENNAGAIN;
   
   // send outputBuf
   rc = dn_serial_mt_sendRequest(
      CMDID_GETNVPARAMETER,                                     // cmdId
      extraFlags,                                               // extraFlags
      dn_whmt_vars.outputBuf,                                   // payload
      DN_GETNVPARAMETER_HARTANTENNAGAIN_REQ_LEN,                // length
      dn_whmt_getNVParameter_HARTantennaGain_reply              // replyCb
   );
   
   if (rc==DN_ERR_NONE) {
      // I'm now busy transmitting
      dn_whmt_vars.busyTx         = TRUE;
   }
   
   // unlock the module
   dn_unlock();
   
   return rc;
   
}

void dn_whmt_getNVParameter_HARTantennaGain_reply(uint8_t cmdId, uint8_t rc, uint8_t* payload, uint8_t len) {
   dn_whmt_getNVParameter_HARTantennaGain_rpt* reply;
   uint8_t paramId;
   
   // verify I'm expecting this answer
   if (dn_whmt_vars.busyTx==FALSE || dn_whmt_vars.cmdId!=cmdId) {
      return;
   }
   
   // verify I'm expecting this paramId
   paramId = payload[0];
   if (paramId!=dn_whmt_vars.paramId) {
      return;
   }
   
   // verify length
   if (rc==DN_SERIAL_RC_OK && len<DN_GETNVPARAMETER_HARTANTENNAGAIN_REPLY_LEN) {
      return;
   }
   
   // cast the replyContent
   reply = (dn_whmt_getNVParameter_HARTantennaGain_rpt*)dn_whmt_vars.replyContents;
   
   // store RC
   reply->RC = rc;
   
   // parse returned value (iff RC==0)
   if (rc==DN_SERIAL_RC_OK) {
      
      reply->antennaGain = (int8_t)payload[DN_GETNVPARAMETER_HARTANTENNAGAIN_REPLY_OFFS_ANTENNAGAIN];
   }
   
   // call the callback
   dn_whmt_vars.replyCb(cmdId);
   
   // I'm not busy transmitting anymore
   dn_whmt_vars.busyTx=FALSE;
}

//===== getNVParameter_OTAPlockout

/**
The getNVParameter<OTAPlockout> command reads the OTAP lockout setting from the 
motes persistent storage. OTAP lockout specifies whether the mote can be 
Over-The-Air-Programmed (OTAP). 
*/
dn_err_t dn_whmt_getNVParameter_OTAPlockout(dn_whmt_getNVParameter_OTAPlockout_rpt* reply) {
   uint8_t    extraFlags;
   dn_err_t   rc;
   
   // lock the module
   dn_lock();
   
   // verify no ongoing transmissions
   if (dn_whmt_vars.busyTx) {
      // unlock the module
      dn_unlock();
      
      // return
      return DN_ERR_BUSY;
   }
   
   // store callback information
   dn_whmt_vars.cmdId          = CMDID_GETNVPARAMETER;
   dn_whmt_vars.replyContents  = (uint8_t*)reply;
   dn_whmt_vars.paramId        = PARAMID_OTAPLOCKOUT;
   
   // extraFlags
   extraFlags = 0x00;
   
   // build outputBuf
   dn_whmt_vars.outputBuf[0] = 0; // reserved
   dn_whmt_vars.outputBuf[1] = 0; // reserved
   dn_whmt_vars.outputBuf[2] = 0; // reserved
   dn_whmt_vars.outputBuf[3] = 0; // reserved
   dn_whmt_vars.outputBuf[4] = PARAMID_OTAPLOCKOUT;
   
   // send outputBuf
   rc = dn_serial_mt_sendRequest(
      CMDID_GETNVPARAMETER,                                     // cmdId
      extraFlags,                                               // extraFlags
      dn_whmt_vars.outputBuf,                                   // payload
      DN_GETNVPARAMETER_OTAPLOCKOUT_REQ_LEN,                    // length
      dn_whmt_getNVParameter_OTAPlockout_reply                  // replyCb
   );
   
   if (rc==DN_ERR_NONE) {
      // I'm now busy transmitting
      dn_whmt_vars.busyTx         = TRUE;
   }
   
   // unlock the module
   dn_unlock();
   
   return rc;
   
}

void dn_whmt_getNVParameter_OTAPlockout_reply(uint8_t cmdId, uint8_t rc, uint8_t* payload, uint8_t len) {
   dn_whmt_getNVParameter_OTAPlockout_rpt* reply;
   uint8_t paramId;
   
   // verify I'm expecting this answer
   if (dn_whmt_vars.busyTx==FALSE || dn_whmt_vars.cmdId!=cmdId) {
      return;
   }
   
   // verify I'm expecting this paramId
   paramId = payload[0];
   if (paramId!=dn_whmt_vars.paramId) {
      return;
   }
   
   // verify length
   if (rc==DN_SERIAL_RC_OK && len<DN_GETNVPARAMETER_OTAPLOCKOUT_REPLY_LEN) {
      return;
   }
   
   // cast the replyContent
   reply = (dn_whmt_getNVParameter_OTAPlockout_rpt*)dn_whmt_vars.replyContents;
   
   // store RC
   reply->RC = rc;
   
   // parse returned value (iff RC==0)
   if (rc==DN_SERIAL_RC_OK) {
      
      reply->otapLockout = payload[DN_GETNVPARAMETER_OTAPLOCKOUT_REPLY_OFFS_OTAPLOCKOUT];
   }
   
   // call the callback
   dn_whmt_vars.replyCb(cmdId);
   
   // I'm not busy transmitting anymore
   dn_whmt_vars.busyTx=FALSE;
}

//===== getNVParameter_hrCounterMode

/**
The getNVParameter<hrCounterMode> command may be used to retrieve the health 
report counter mode that is used by devices. This mode controls how the mote 
deals with statistics counters when they reach their maximum value.

Note: This parameter is available in devices running mote software >=1.1.0 
*/
dn_err_t dn_whmt_getNVParameter_hrCounterMode(dn_whmt_getNVParameter_hrCounterMode_rpt* reply) {
   uint8_t    extraFlags;
   dn_err_t   rc;
   
   // lock the module
   dn_lock();
   
   // verify no ongoing transmissions
   if (dn_whmt_vars.busyTx) {
      // unlock the module
      dn_unlock();
      
      // return
      return DN_ERR_BUSY;
   }
   
   // store callback information
   dn_whmt_vars.cmdId          = CMDID_GETNVPARAMETER;
   dn_whmt_vars.replyContents  = (uint8_t*)reply;
   dn_whmt_vars.paramId        = PARAMID_HRCOUNTERMODE;
   
   // extraFlags
   extraFlags = 0x00;
   
   // build outputBuf
   dn_whmt_vars.outputBuf[0] = 0; // reserved
   dn_whmt_vars.outputBuf[1] = 0; // reserved
   dn_whmt_vars.outputBuf[2] = 0; // reserved
   dn_whmt_vars.outputBuf[3] = 0; // reserved
   dn_whmt_vars.outputBuf[4] = PARAMID_HRCOUNTERMODE;
   
   // send outputBuf
   rc = dn_serial_mt_sendRequest(
      CMDID_GETNVPARAMETER,                                     // cmdId
      extraFlags,                                               // extraFlags
      dn_whmt_vars.outputBuf,                                   // payload
      DN_GETNVPARAMETER_HRCOUNTERMODE_REQ_LEN,                  // length
      dn_whmt_getNVParameter_hrCounterMode_reply                // replyCb
   );
   
   if (rc==DN_ERR_NONE) {
      // I'm now busy transmitting
      dn_whmt_vars.busyTx         = TRUE;
   }
   
   // unlock the module
   dn_unlock();
   
   return rc;
   
}

void dn_whmt_getNVParameter_hrCounterMode_reply(uint8_t cmdId, uint8_t rc, uint8_t* payload, uint8_t len) {
   dn_whmt_getNVParameter_hrCounterMode_rpt* reply;
   uint8_t paramId;
   
   // verify I'm expecting this answer
   if (dn_whmt_vars.busyTx==FALSE || dn_whmt_vars.cmdId!=cmdId) {
      return;
   }
   
   // verify I'm expecting this paramId
   paramId = payload[0];
   if (paramId!=dn_whmt_vars.paramId) {
      return;
   }
   
   // verify length
   if (rc==DN_SERIAL_RC_OK && len<DN_GETNVPARAMETER_HRCOUNTERMODE_REPLY_LEN) {
      return;
   }
   
   // cast the replyContent
   reply = (dn_whmt_getNVParameter_hrCounterMode_rpt*)dn_whmt_vars.replyContents;
   
   // store RC
   reply->RC = rc;
   
   // parse returned value (iff RC==0)
   if (rc==DN_SERIAL_RC_OK) {
      
      reply->hrCounterMode = payload[DN_GETNVPARAMETER_HRCOUNTERMODE_REPLY_OFFS_HRCOUNTERMODE];
   }
   
   // call the callback
   dn_whmt_vars.replyCb(cmdId);
   
   // I'm not busy transmitting anymore
   dn_whmt_vars.busyTx=FALSE;
}

//===== getNVParameter_autojoin

/**
The getNVParameter<autojoin> command returns the autojoin status stored in 
mote's persistent storage (i.e. set with setNVParameter<autojoin>). Autojoin 
can be used to cause a mote in slave mode to join on its own when booted. 
*/
dn_err_t dn_whmt_getNVParameter_autojoin(uint32_t reserved, uint8_t nvParamId, dn_whmt_getNVParameter_autojoin_rpt* reply) {
   uint8_t    extraFlags;
   dn_err_t   rc;
   
   // lock the module
   dn_lock();
   
   // verify no ongoing transmissions
   if (dn_whmt_vars.busyTx) {
      // unlock the module
      dn_unlock();
      
      // return
      return DN_ERR_BUSY;
   }
   
   // store callback information
   dn_whmt_vars.cmdId          = CMDID_GETNVPARAMETER;
   dn_whmt_vars.replyContents  = (uint8_t*)reply;
   dn_whmt_vars.paramId        = PARAMID_AUTOJOIN;
   
   // extraFlags
   extraFlags = 0x00;
   
   // build outputBuf
   dn_whmt_vars.outputBuf[0] = 0; // reserved
   dn_whmt_vars.outputBuf[1] = 0; // reserved
   dn_whmt_vars.outputBuf[2] = 0; // reserved
   dn_whmt_vars.outputBuf[3] = 0; // reserved
   dn_whmt_vars.outputBuf[4] = PARAMID_AUTOJOIN;
   dn_write_uint32_t(&dn_whmt_vars.outputBuf[DN_GETNVPARAMETER_AUTOJOIN_REQ_OFFS_RESERVED],reserved);
   dn_whmt_vars.outputBuf[DN_GETNVPARAMETER_AUTOJOIN_REQ_OFFS_NVPARAMID] = nvParamId;
   
   // send outputBuf
   rc = dn_serial_mt_sendRequest(
      CMDID_GETNVPARAMETER,                                     // cmdId
      extraFlags,                                               // extraFlags
      dn_whmt_vars.outputBuf,                                   // payload
      DN_GETNVPARAMETER_AUTOJOIN_REQ_LEN,                       // length
      dn_whmt_getNVParameter_autojoin_reply                     // replyCb
   );
   
   if (rc==DN_ERR_NONE) {
      // I'm now busy transmitting
      dn_whmt_vars.busyTx         = TRUE;
   }
   
   // unlock the module
   dn_unlock();
   
   return rc;
   
}

void dn_whmt_getNVParameter_autojoin_reply(uint8_t cmdId, uint8_t rc, uint8_t* payload, uint8_t len) {
   dn_whmt_getNVParameter_autojoin_rpt* reply;
   uint8_t paramId;
   
   // verify I'm expecting this answer
   if (dn_whmt_vars.busyTx==FALSE || dn_whmt_vars.cmdId!=cmdId) {
      return;
   }
   
   // verify I'm expecting this paramId
   paramId = payload[0];
   if (paramId!=dn_whmt_vars.paramId) {
      return;
   }
   
   // verify length
   if (rc==DN_SERIAL_RC_OK && len<DN_GETNVPARAMETER_AUTOJOIN_REPLY_LEN) {
      return;
   }
   
   // cast the replyContent
   reply = (dn_whmt_getNVParameter_autojoin_rpt*)dn_whmt_vars.replyContents;
   
   // store RC
   reply->RC = rc;
   
   // parse returned value (iff RC==0)
   if (rc==DN_SERIAL_RC_OK) {
      
      reply->nvParamId = payload[DN_GETNVPARAMETER_AUTOJOIN_REPLY_OFFS_NVPARAMID];
      reply->autojoin = payload[DN_GETNVPARAMETER_AUTOJOIN_REPLY_OFFS_AUTOJOIN];
   }
   
   // call the callback
   dn_whmt_vars.replyCb(cmdId);
   
   // I'm not busy transmitting anymore
   dn_whmt_vars.busyTx=FALSE;
}

//===== getNVParameter_compliantMode

/**
The getNVParameter<compliantMode> command may be used to retrieve the 
compliance mode that is used by devices.This mode controls strict compliance to 
HART specification requirements, specifically:

- join timeouts (faster in non-compliant mode)
- Keepalive interval (adapts to synch quality in non-compliant mode)
- Health report format (uses saturating counters in non-compliant mode)

Note: This parameter is available in devices running mote software >= 1.1.0 
*/
dn_err_t dn_whmt_getNVParameter_compliantMode(dn_whmt_getNVParameter_compliantMode_rpt* reply) {
   uint8_t    extraFlags;
   dn_err_t   rc;
   
   // lock the module
   dn_lock();
   
   // verify no ongoing transmissions
   if (dn_whmt_vars.busyTx) {
      // unlock the module
      dn_unlock();
      
      // return
      return DN_ERR_BUSY;
   }
   
   // store callback information
   dn_whmt_vars.cmdId          = CMDID_GETNVPARAMETER;
   dn_whmt_vars.replyContents  = (uint8_t*)reply;
   dn_whmt_vars.paramId        = PARAMID_COMPLIANTMODE;
   
   // extraFlags
   extraFlags = 0x00;
   
   // build outputBuf
   dn_whmt_vars.outputBuf[0] = 0; // reserved
   dn_whmt_vars.outputBuf[1] = 0; // reserved
   dn_whmt_vars.outputBuf[2] = 0; // reserved
   dn_whmt_vars.outputBuf[3] = 0; // reserved
   dn_whmt_vars.outputBuf[4] = PARAMID_COMPLIANTMODE;
   
   // send outputBuf
   rc = dn_serial_mt_sendRequest(
      CMDID_GETNVPARAMETER,                                     // cmdId
      extraFlags,                                               // extraFlags
      dn_whmt_vars.outputBuf,                                   // payload
      DN_GETNVPARAMETER_COMPLIANTMODE_REQ_LEN,                  // length
      dn_whmt_getNVParameter_compliantMode_reply                // replyCb
   );
   
   if (rc==DN_ERR_NONE) {
      // I'm now busy transmitting
      dn_whmt_vars.busyTx         = TRUE;
   }
   
   // unlock the module
   dn_unlock();
   
   return rc;
   
}

void dn_whmt_getNVParameter_compliantMode_reply(uint8_t cmdId, uint8_t rc, uint8_t* payload, uint8_t len) {
   dn_whmt_getNVParameter_compliantMode_rpt* reply;
   uint8_t paramId;
   
   // verify I'm expecting this answer
   if (dn_whmt_vars.busyTx==FALSE || dn_whmt_vars.cmdId!=cmdId) {
      return;
   }
   
   // verify I'm expecting this paramId
   paramId = payload[0];
   if (paramId!=dn_whmt_vars.paramId) {
      return;
   }
   
   // verify length
   if (rc==DN_SERIAL_RC_OK && len<DN_GETNVPARAMETER_COMPLIANTMODE_REPLY_LEN) {
      return;
   }
   
   // cast the replyContent
   reply = (dn_whmt_getNVParameter_compliantMode_rpt*)dn_whmt_vars.replyContents;
   
   // store RC
   reply->RC = rc;
   
   // parse returned value (iff RC==0)
   if (rc==DN_SERIAL_RC_OK) {
      
      reply->compliantMode = payload[DN_GETNVPARAMETER_COMPLIANTMODE_REPLY_OFFS_COMPLIANTMODE];
   }
   
   // call the callback
   dn_whmt_vars.replyCb(cmdId);
   
   // I'm not busy transmitting anymore
   dn_whmt_vars.busyTx=FALSE;
}

//===== getNVParameter_lock

/**
The getNVParameter < lock > command returns the persisted lock code and locking 
master (those to be used after reset). To determine the current lock status, 
use the getParameter<lock> command.Note: This parameter is available in devices 
running mote software >= 1.1.0 
*/
dn_err_t dn_whmt_getNVParameter_lock(dn_whmt_getNVParameter_lock_rpt* reply) {
   uint8_t    extraFlags;
   dn_err_t   rc;
   
   // lock the module
   dn_lock();
   
   // verify no ongoing transmissions
   if (dn_whmt_vars.busyTx) {
      // unlock the module
      dn_unlock();
      
      // return
      return DN_ERR_BUSY;
   }
   
   // store callback information
   dn_whmt_vars.cmdId          = CMDID_GETNVPARAMETER;
   dn_whmt_vars.replyContents  = (uint8_t*)reply;
   dn_whmt_vars.paramId        = PARAMID_LOCK;
   
   // extraFlags
   extraFlags = 0x00;
   
   // build outputBuf
   dn_whmt_vars.outputBuf[0] = 0; // reserved
   dn_whmt_vars.outputBuf[1] = 0; // reserved
   dn_whmt_vars.outputBuf[2] = 0; // reserved
   dn_whmt_vars.outputBuf[3] = 0; // reserved
   dn_whmt_vars.outputBuf[4] = PARAMID_LOCK;
   
   // send outputBuf
   rc = dn_serial_mt_sendRequest(
      CMDID_GETNVPARAMETER,                                     // cmdId
      extraFlags,                                               // extraFlags
      dn_whmt_vars.outputBuf,                                   // payload
      DN_GETNVPARAMETER_LOCK_REQ_LEN,                           // length
      dn_whmt_getNVParameter_lock_reply                         // replyCb
   );
   
   if (rc==DN_ERR_NONE) {
      // I'm now busy transmitting
      dn_whmt_vars.busyTx         = TRUE;
   }
   
   // unlock the module
   dn_unlock();
   
   return rc;
   
}

void dn_whmt_getNVParameter_lock_reply(uint8_t cmdId, uint8_t rc, uint8_t* payload, uint8_t len) {
   dn_whmt_getNVParameter_lock_rpt* reply;
   uint8_t paramId;
   
   // verify I'm expecting this answer
   if (dn_whmt_vars.busyTx==FALSE || dn_whmt_vars.cmdId!=cmdId) {
      return;
   }
   
   // verify I'm expecting this paramId
   paramId = payload[0];
   if (paramId!=dn_whmt_vars.paramId) {
      return;
   }
   
   // verify length
   if (rc==DN_SERIAL_RC_OK && len<DN_GETNVPARAMETER_LOCK_REPLY_LEN) {
      return;
   }
   
   // cast the replyContent
   reply = (dn_whmt_getNVParameter_lock_rpt*)dn_whmt_vars.replyContents;
   
   // store RC
   reply->RC = rc;
   
   // parse returned value (iff RC==0)
   if (rc==DN_SERIAL_RC_OK) {
      
      reply->code = payload[DN_GETNVPARAMETER_LOCK_REPLY_OFFS_CODE];
      dn_read_uint16_t(&reply->master,&payload[DN_GETNVPARAMETER_LOCK_REPLY_OFFS_MASTER]);
   }
   
   // call the callback
   dn_whmt_vars.replyCb(cmdId);
   
   // I'm not busy transmitting anymore
   dn_whmt_vars.busyTx=FALSE;
}

//===== send

/**
The send command allows a serial device to send a packet into the network 
through the mote's serial port. The mote forwards the packet to the network 
upon receiving it. The microprocessor must not attempt to send data at a rate 
that exceeds its allocated bandwidth. For a WirelessHART device, the payload of 
the packet must include the status byte and the extended status byte, followed 
by one or more sets of HART commands up to the maximum send payload size, as follows:

Request: Status|Extended Status|Cmd1|Length1|Data1|Cmd2|Length2|Data2...
Response: Status|Extended Status|Cmd1|Length1(includes response code)|RC1|Data1|Cmd2|Length2|RC2|Data2...

Prior to sending the payload into the network, the mote caches the value of 
Status and Extended Status to use in packets it originates locally. The send 
command is only valid when the mote is in the Operational state. If the mote 
receives this command when it is not in the Operational state, it returns the 
error RC_INV_STATE. Note: The serial device can receive a request while the 
mote is in the process of transition from the Connected state to the 
Operational state. 
*/
dn_err_t dn_whmt_send(bool tranType, bool tranDir, uint16_t destAddr, uint8_t serviceId, uint8_t appDomain, uint8_t priority, uint16_t reserved, uint8_t seqNum, uint8_t payloadLen, uint8_t* payload, dn_whmt_send_rpt* reply) {
   uint8_t    extraFlags;
   dn_err_t   rc;
   
   // lock the module
   dn_lock();
   
   // verify no ongoing transmissions
   if (dn_whmt_vars.busyTx) {
      // unlock the module
      dn_unlock();
      
      // return
      return DN_ERR_BUSY;
   }
   
   // store callback information
   dn_whmt_vars.cmdId          = CMDID_SEND;
   dn_whmt_vars.replyContents  = (uint8_t*)reply;
   
   // extraFlags
   extraFlags = 0x00;
   if (tranType==DN_TRANTYPE_ACKNOWLEDGED) {
      extraFlags            |= (1<<6);
   }
   if (tranDir==DN_TRANDIR_RESPONSE) {
      extraFlags            |= (1<<7);
   }
   
   // build outputBuf
   dn_write_uint16_t(&dn_whmt_vars.outputBuf[DN_SEND_REQ_OFFS_DESTADDR],destAddr);
   dn_whmt_vars.outputBuf[DN_SEND_REQ_OFFS_SERVICEID] = serviceId;
   dn_whmt_vars.outputBuf[DN_SEND_REQ_OFFS_APPDOMAIN] = appDomain;
   dn_whmt_vars.outputBuf[DN_SEND_REQ_OFFS_PRIORITY] = priority;
   dn_write_uint16_t(&dn_whmt_vars.outputBuf[DN_SEND_REQ_OFFS_RESERVED],reserved);
   dn_whmt_vars.outputBuf[DN_SEND_REQ_OFFS_SEQNUM] = seqNum;
   dn_whmt_vars.outputBuf[DN_SEND_REQ_OFFS_PAYLOADLEN] = payloadLen;
   memcpy(&dn_whmt_vars.outputBuf[DN_SEND_REQ_OFFS_PAYLOAD],payload,payloadLen);
   
   // send outputBuf
   rc = dn_serial_mt_sendRequest(
      CMDID_SEND,                                               // cmdId
      extraFlags,                                               // extraFlags
      dn_whmt_vars.outputBuf,                                   // payload
      DN_SEND_REQ_LEN+payloadLen,                               // length
      dn_whmt_send_reply                                        // replyCb
   );
   
   if (rc==DN_ERR_NONE) {
      // I'm now busy transmitting
      dn_whmt_vars.busyTx         = TRUE;
   }
   
   // unlock the module
   dn_unlock();
   
   return rc;
   
}

void dn_whmt_send_reply(uint8_t cmdId, uint8_t rc, uint8_t* payload, uint8_t len) {
   dn_whmt_send_rpt* reply;
   
   // verify I'm expecting this answer
   if (dn_whmt_vars.busyTx==FALSE || dn_whmt_vars.cmdId!=cmdId) {
      return;
   }
   
   // do NOT verify length (no return fields expected)
   
   // cast the replyContent
   reply = (dn_whmt_send_rpt*)dn_whmt_vars.replyContents;
   
   // store RC
   reply->RC = rc;
   
   // parse returned value (iff RC==0)
   if (rc==DN_SERIAL_RC_OK) {
      
   }
   
   // call the callback
   dn_whmt_vars.replyCb(cmdId);
   
   // I'm not busy transmitting anymore
   dn_whmt_vars.busyTx=FALSE;
}

//===== join

/**
The join command requests that a mote start searching for the network and 
attempt to join. The mote must be in the Idle state or the Promiscuous Listen 
state (see search) for this command to be valid. The join time is partly 
determined by the join duty cycle. For guidance on setting the join duty cycle, 
see setParameter<joinDutyCycle>. 
*/
dn_err_t dn_whmt_join(dn_whmt_join_rpt* reply) {
   uint8_t    extraFlags;
   dn_err_t   rc;
   
   // lock the module
   dn_lock();
   
   // verify no ongoing transmissions
   if (dn_whmt_vars.busyTx) {
      // unlock the module
      dn_unlock();
      
      // return
      return DN_ERR_BUSY;
   }
   
   // store callback information
   dn_whmt_vars.cmdId          = CMDID_JOIN;
   dn_whmt_vars.replyContents  = (uint8_t*)reply;
   
   // extraFlags
   extraFlags = 0x00;
   
   // build outputBuf
   
   // send outputBuf
   rc = dn_serial_mt_sendRequest(
      CMDID_JOIN,                                               // cmdId
      extraFlags,                                               // extraFlags
      dn_whmt_vars.outputBuf,                                   // payload
      DN_JOIN_REQ_LEN,                                          // length
      dn_whmt_join_reply                                        // replyCb
   );
   
   if (rc==DN_ERR_NONE) {
      // I'm now busy transmitting
      dn_whmt_vars.busyTx         = TRUE;
   }
   
   // unlock the module
   dn_unlock();
   
   return rc;
   
}

void dn_whmt_join_reply(uint8_t cmdId, uint8_t rc, uint8_t* payload, uint8_t len) {
   dn_whmt_join_rpt* reply;
   
   // verify I'm expecting this answer
   if (dn_whmt_vars.busyTx==FALSE || dn_whmt_vars.cmdId!=cmdId) {
      return;
   }
   
   // do NOT verify length (no return fields expected)
   
   // cast the replyContent
   reply = (dn_whmt_join_rpt*)dn_whmt_vars.replyContents;
   
   // store RC
   reply->RC = rc;
   
   // parse returned value (iff RC==0)
   if (rc==DN_SERIAL_RC_OK) {
      
   }
   
   // call the callback
   dn_whmt_vars.replyCb(cmdId);
   
   // I'm not busy transmitting anymore
   dn_whmt_vars.busyTx=FALSE;
}

//===== disconnect

/**
The disconnect command requests that the mote disconnect from the network. The 
mote will send an indication to its network neighbors that it is about to 
become unavailable. Just after the mote disconnects, it sends the 
microprocessor an events packet with the disconnected bit set, indicating it 
will reset. This command is only valid in when the mote is in the Connected or 
Operational state (see Mote State).

The OEM microprocessor should disconnect from the network if the device is 
going to power down, reset, or otherwise be unavailable for a long period.

A mote will reset itself after having sent the disconnect notification to the 
OEM microprocessor. The microprocessor should wait to acknowledge the boot 
event before shutting down. 
*/
dn_err_t dn_whmt_disconnect(dn_whmt_disconnect_rpt* reply) {
   uint8_t    extraFlags;
   dn_err_t   rc;
   
   // lock the module
   dn_lock();
   
   // verify no ongoing transmissions
   if (dn_whmt_vars.busyTx) {
      // unlock the module
      dn_unlock();
      
      // return
      return DN_ERR_BUSY;
   }
   
   // store callback information
   dn_whmt_vars.cmdId          = CMDID_DISCONNECT;
   dn_whmt_vars.replyContents  = (uint8_t*)reply;
   
   // extraFlags
   extraFlags = 0x00;
   
   // build outputBuf
   
   // send outputBuf
   rc = dn_serial_mt_sendRequest(
      CMDID_DISCONNECT,                                         // cmdId
      extraFlags,                                               // extraFlags
      dn_whmt_vars.outputBuf,                                   // payload
      DN_DISCONNECT_REQ_LEN,                                    // length
      dn_whmt_disconnect_reply                                  // replyCb
   );
   
   if (rc==DN_ERR_NONE) {
      // I'm now busy transmitting
      dn_whmt_vars.busyTx         = TRUE;
   }
   
   // unlock the module
   dn_unlock();
   
   return rc;
   
}

void dn_whmt_disconnect_reply(uint8_t cmdId, uint8_t rc, uint8_t* payload, uint8_t len) {
   dn_whmt_disconnect_rpt* reply;
   
   // verify I'm expecting this answer
   if (dn_whmt_vars.busyTx==FALSE || dn_whmt_vars.cmdId!=cmdId) {
      return;
   }
   
   // do NOT verify length (no return fields expected)
   
   // cast the replyContent
   reply = (dn_whmt_disconnect_rpt*)dn_whmt_vars.replyContents;
   
   // store RC
   reply->RC = rc;
   
   // parse returned value (iff RC==0)
   if (rc==DN_SERIAL_RC_OK) {
      
   }
   
   // call the callback
   dn_whmt_vars.replyCb(cmdId);
   
   // I'm not busy transmitting anymore
   dn_whmt_vars.busyTx=FALSE;
}

//===== reset

/**
Upon receiving this command, the mote resets itself after a short delay. The 
mote will always send a response packet before initiating the reset. To force 
the mote to gracefully leave the network, use the disconnect command. 
*/
dn_err_t dn_whmt_reset(dn_whmt_reset_rpt* reply) {
   uint8_t    extraFlags;
   dn_err_t   rc;
   
   // lock the module
   dn_lock();
   
   // verify no ongoing transmissions
   if (dn_whmt_vars.busyTx) {
      // unlock the module
      dn_unlock();
      
      // return
      return DN_ERR_BUSY;
   }
   
   // store callback information
   dn_whmt_vars.cmdId          = CMDID_RESET;
   dn_whmt_vars.replyContents  = (uint8_t*)reply;
   
   // extraFlags
   extraFlags = 0x00;
   
   // build outputBuf
   
   // send outputBuf
   rc = dn_serial_mt_sendRequest(
      CMDID_RESET,                                              // cmdId
      extraFlags,                                               // extraFlags
      dn_whmt_vars.outputBuf,                                   // payload
      DN_RESET_REQ_LEN,                                         // length
      dn_whmt_reset_reply                                       // replyCb
   );
   
   if (rc==DN_ERR_NONE) {
      // I'm now busy transmitting
      dn_whmt_vars.busyTx         = TRUE;
   }
   
   // unlock the module
   dn_unlock();
   
   return rc;
   
}

void dn_whmt_reset_reply(uint8_t cmdId, uint8_t rc, uint8_t* payload, uint8_t len) {
   dn_whmt_reset_rpt* reply;
   
   // verify I'm expecting this answer
   if (dn_whmt_vars.busyTx==FALSE || dn_whmt_vars.cmdId!=cmdId) {
      return;
   }
   
   // do NOT verify length (no return fields expected)
   
   // cast the replyContent
   reply = (dn_whmt_reset_rpt*)dn_whmt_vars.replyContents;
   
   // store RC
   reply->RC = rc;
   
   // parse returned value (iff RC==0)
   if (rc==DN_SERIAL_RC_OK) {
      
   }
   
   // call the callback
   dn_whmt_vars.replyCb(cmdId);
   
   // I'm not busy transmitting anymore
   dn_whmt_vars.busyTx=FALSE;
}

//===== lowPowerSleep

/**
The lowPowerSleep command shuts down all peripherals and places the mote in 
deep sleep mode. The lowPowerSleep command may be issued at any time and will 
cause the mote to interrupt all in-progress network operation. The command 
executes after the mote sends its response. The mote enters deep sleep within 
two seconds after the command executes.

The OEM microprocessor should put the mote into low power sleep when the mote 
needs to be offline for an extended period of time. In most cases, this will 
result in a lower current state of the mote than simply asserting /RST without 
putting the mote in low power sleep. To achieve a graceful disconnect, use the 
disconnect command before using the lowPowerSleep command. The mote can only be 
awakened from low power sleep by asserting a non-maskable interrupt, such as 
the /RST control line. For power consumption information, refer to the mote 
product datasheet. 
*/
dn_err_t dn_whmt_lowPowerSleep(dn_whmt_lowPowerSleep_rpt* reply) {
   uint8_t    extraFlags;
   dn_err_t   rc;
   
   // lock the module
   dn_lock();
   
   // verify no ongoing transmissions
   if (dn_whmt_vars.busyTx) {
      // unlock the module
      dn_unlock();
      
      // return
      return DN_ERR_BUSY;
   }
   
   // store callback information
   dn_whmt_vars.cmdId          = CMDID_LOWPOWERSLEEP;
   dn_whmt_vars.replyContents  = (uint8_t*)reply;
   
   // extraFlags
   extraFlags = 0x00;
   
   // build outputBuf
   
   // send outputBuf
   rc = dn_serial_mt_sendRequest(
      CMDID_LOWPOWERSLEEP,                                      // cmdId
      extraFlags,                                               // extraFlags
      dn_whmt_vars.outputBuf,                                   // payload
      DN_LOWPOWERSLEEP_REQ_LEN,                                 // length
      dn_whmt_lowPowerSleep_reply                               // replyCb
   );
   
   if (rc==DN_ERR_NONE) {
      // I'm now busy transmitting
      dn_whmt_vars.busyTx         = TRUE;
   }
   
   // unlock the module
   dn_unlock();
   
   return rc;
   
}

void dn_whmt_lowPowerSleep_reply(uint8_t cmdId, uint8_t rc, uint8_t* payload, uint8_t len) {
   dn_whmt_lowPowerSleep_rpt* reply;
   
   // verify I'm expecting this answer
   if (dn_whmt_vars.busyTx==FALSE || dn_whmt_vars.cmdId!=cmdId) {
      return;
   }
   
   // do NOT verify length (no return fields expected)
   
   // cast the replyContent
   reply = (dn_whmt_lowPowerSleep_rpt*)dn_whmt_vars.replyContents;
   
   // store RC
   reply->RC = rc;
   
   // parse returned value (iff RC==0)
   if (rc==DN_SERIAL_RC_OK) {
      
   }
   
   // call the callback
   dn_whmt_vars.replyCb(cmdId);
   
   // I'm not busy transmitting anymore
   dn_whmt_vars.busyTx=FALSE;
}

//===== hartPayload

/**
The hartPayload command allows the microprocessor to forward a HART payload to 
the mote. The format of the command must be as follows

16-bit command number | data length | data

The reply (if any) will be in the form of a HART response and sent in the 
payload of the acknowledgement. The RC_INVALID_VALUE response means that the 
hartPayload command was given a HART command that the mote does not terminate. 
*/
dn_err_t dn_whmt_hartPayload(uint8_t payloadLen, uint8_t* payload, dn_whmt_hartPayload_rpt* reply) {
   uint8_t    extraFlags;
   dn_err_t   rc;
   
   // lock the module
   dn_lock();
   
   // verify no ongoing transmissions
   if (dn_whmt_vars.busyTx) {
      // unlock the module
      dn_unlock();
      
      // return
      return DN_ERR_BUSY;
   }
   
   // store callback information
   dn_whmt_vars.cmdId          = CMDID_HARTPAYLOAD;
   dn_whmt_vars.replyContents  = (uint8_t*)reply;
   
   // extraFlags
   extraFlags = 0x00;
   
   // build outputBuf
   dn_whmt_vars.outputBuf[DN_HARTPAYLOAD_REQ_OFFS_PAYLOADLEN] = payloadLen;
   memcpy(&dn_whmt_vars.outputBuf[DN_HARTPAYLOAD_REQ_OFFS_PAYLOAD],payload,payloadLen);
   
   // send outputBuf
   rc = dn_serial_mt_sendRequest(
      CMDID_HARTPAYLOAD,                                        // cmdId
      extraFlags,                                               // extraFlags
      dn_whmt_vars.outputBuf,                                   // payload
      DN_HARTPAYLOAD_REQ_LEN+payloadLen,                        // length
      dn_whmt_hartPayload_reply                                 // replyCb
   );
   
   if (rc==DN_ERR_NONE) {
      // I'm now busy transmitting
      dn_whmt_vars.busyTx         = TRUE;
   }
   
   // unlock the module
   dn_unlock();
   
   return rc;
   
}

void dn_whmt_hartPayload_reply(uint8_t cmdId, uint8_t rc, uint8_t* payload, uint8_t len) {
   dn_whmt_hartPayload_rpt* reply;
   
   // verify I'm expecting this answer
   if (dn_whmt_vars.busyTx==FALSE || dn_whmt_vars.cmdId!=cmdId) {
      return;
   }
   
   // verify length
   if (rc==DN_SERIAL_RC_OK && len<DN_HARTPAYLOAD_REPLY_LEN) {
      return;
   }
   
   // cast the replyContent
   reply = (dn_whmt_hartPayload_rpt*)dn_whmt_vars.replyContents;
   
   // store RC
   reply->RC = rc;
   
   // parse returned value (iff RC==0)
   if (rc==DN_SERIAL_RC_OK) {
      
      reply->payloadLen = payload[DN_HARTPAYLOAD_REPLY_OFFS_PAYLOADLEN];
      memcpy(&reply->payload[0],&payload[DN_HARTPAYLOAD_REPLY_OFFS_PAYLOAD],len-DN_HARTPAYLOAD_REPLY_OFFS_PAYLOAD);
   }
   
   // call the callback
   dn_whmt_vars.replyCb(cmdId);
   
   // I'm not busy transmitting anymore
   dn_whmt_vars.busyTx=FALSE;
}

//===== testRadioTx

/**
The testRadioTx command initiates transmission over the radio. This command may 
only be issued prior to the mote joining the network. While executing this 
command the mote sends numPackets packets. Each packet consists of a payload of 
up to 125 bytes, and a 2-byte 802.15.4 CRC at the end. Bytes 0 and 1 contain 
the packet number (in big-endian format) that increments with every packet 
transmitted. Bytes 2..N contain a counter (from 0..N-2) that increments with 
every byte inside payload. Transmissions occur on the specified channel.

If number of packets parameter is set to 0x00, the mote will generate an 
unmodulatedtest tone on the selected channel. The test tone can only be stopped 
by resetting themote.



Channel numbering is 0-15, corresponding to IEEE 2.4 GHz channels 11-26.

Note: this command is deprecated and should not be used in new designs. The 
replacement command is testRadioTxExt. 
*/
dn_err_t dn_whmt_testRadioTx(uint8_t channel, uint16_t numPackets, dn_whmt_testRadioTx_rpt* reply) {
   uint8_t    extraFlags;
   dn_err_t   rc;
   
   // lock the module
   dn_lock();
   
   // verify no ongoing transmissions
   if (dn_whmt_vars.busyTx) {
      // unlock the module
      dn_unlock();
      
      // return
      return DN_ERR_BUSY;
   }
   
   // store callback information
   dn_whmt_vars.cmdId          = CMDID_TESTRADIOTX;
   dn_whmt_vars.replyContents  = (uint8_t*)reply;
   
   // extraFlags
   extraFlags = 0x00;
   
   // build outputBuf
   dn_whmt_vars.outputBuf[DN_TESTRADIOTX_REQ_OFFS_CHANNEL] = channel;
   dn_write_uint16_t(&dn_whmt_vars.outputBuf[DN_TESTRADIOTX_REQ_OFFS_NUMPACKETS],numPackets);
   
   // send outputBuf
   rc = dn_serial_mt_sendRequest(
      CMDID_TESTRADIOTX,                                        // cmdId
      extraFlags,                                               // extraFlags
      dn_whmt_vars.outputBuf,                                   // payload
      DN_TESTRADIOTX_REQ_LEN,                                   // length
      dn_whmt_testRadioTx_reply                                 // replyCb
   );
   
   if (rc==DN_ERR_NONE) {
      // I'm now busy transmitting
      dn_whmt_vars.busyTx         = TRUE;
   }
   
   // unlock the module
   dn_unlock();
   
   return rc;
   
}

void dn_whmt_testRadioTx_reply(uint8_t cmdId, uint8_t rc, uint8_t* payload, uint8_t len) {
   dn_whmt_testRadioTx_rpt* reply;
   
   // verify I'm expecting this answer
   if (dn_whmt_vars.busyTx==FALSE || dn_whmt_vars.cmdId!=cmdId) {
      return;
   }
   
   // do NOT verify length (no return fields expected)
   
   // cast the replyContent
   reply = (dn_whmt_testRadioTx_rpt*)dn_whmt_vars.replyContents;
   
   // store RC
   reply->RC = rc;
   
   // parse returned value (iff RC==0)
   if (rc==DN_SERIAL_RC_OK) {
      
   }
   
   // call the callback
   dn_whmt_vars.replyCb(cmdId);
   
   // I'm not busy transmitting anymore
   dn_whmt_vars.busyTx=FALSE;
}

//===== testRadioRx

/**
The testRadioRx command clears all previously collected statistics and 
initiates a test of radio reception for the specified channel and duration. 
During the test, the mote keeps statistics about the number of packets received 
(with and without error). The test results may be retrieved using the 
getParameter<testRadioRxStats> command. The mote must be reset (either hardware 
or software reset) after radio tests are complete and prior to joining.



Channel numbering is 0-15, corresponding to IEEE 2.4 GHz channels 11-26.

Note: this command is deprecated and should not be used in new designs. The 
replacement command is testRadioRxExt. 
*/
dn_err_t dn_whmt_testRadioRx(uint8_t channel, uint16_t time, dn_whmt_testRadioRx_rpt* reply) {
   uint8_t    extraFlags;
   dn_err_t   rc;
   
   // lock the module
   dn_lock();
   
   // verify no ongoing transmissions
   if (dn_whmt_vars.busyTx) {
      // unlock the module
      dn_unlock();
      
      // return
      return DN_ERR_BUSY;
   }
   
   // store callback information
   dn_whmt_vars.cmdId          = CMDID_TESTRADIORX;
   dn_whmt_vars.replyContents  = (uint8_t*)reply;
   
   // extraFlags
   extraFlags = 0x00;
   
   // build outputBuf
   dn_whmt_vars.outputBuf[DN_TESTRADIORX_REQ_OFFS_CHANNEL] = channel;
   dn_write_uint16_t(&dn_whmt_vars.outputBuf[DN_TESTRADIORX_REQ_OFFS_TIME],time);
   
   // send outputBuf
   rc = dn_serial_mt_sendRequest(
      CMDID_TESTRADIORX,                                        // cmdId
      extraFlags,                                               // extraFlags
      dn_whmt_vars.outputBuf,                                   // payload
      DN_TESTRADIORX_REQ_LEN,                                   // length
      dn_whmt_testRadioRx_reply                                 // replyCb
   );
   
   if (rc==DN_ERR_NONE) {
      // I'm now busy transmitting
      dn_whmt_vars.busyTx         = TRUE;
   }
   
   // unlock the module
   dn_unlock();
   
   return rc;
   
}

void dn_whmt_testRadioRx_reply(uint8_t cmdId, uint8_t rc, uint8_t* payload, uint8_t len) {
   dn_whmt_testRadioRx_rpt* reply;
   
   // verify I'm expecting this answer
   if (dn_whmt_vars.busyTx==FALSE || dn_whmt_vars.cmdId!=cmdId) {
      return;
   }
   
   // do NOT verify length (no return fields expected)
   
   // cast the replyContent
   reply = (dn_whmt_testRadioRx_rpt*)dn_whmt_vars.replyContents;
   
   // store RC
   reply->RC = rc;
   
   // parse returned value (iff RC==0)
   if (rc==DN_SERIAL_RC_OK) {
      
   }
   
   // call the callback
   dn_whmt_vars.replyCb(cmdId);
   
   // I'm not busy transmitting anymore
   dn_whmt_vars.busyTx=FALSE;
}

//===== clearNV

/**
The clearNV command resets the motes Non-Volatile (NV) memory to its 
factory-default state. Refer to the WirelessHART User Guide for table of 
default values. Note that since this command clears the mote's security join 
counter, the corresponding manager's Access Control List (ACL) entry may need 
to be cleared as well to allow joining. 
*/
dn_err_t dn_whmt_clearNV(dn_whmt_clearNV_rpt* reply) {
   uint8_t    extraFlags;
   dn_err_t   rc;
   
   // lock the module
   dn_lock();
   
   // verify no ongoing transmissions
   if (dn_whmt_vars.busyTx) {
      // unlock the module
      dn_unlock();
      
      // return
      return DN_ERR_BUSY;
   }
   
   // store callback information
   dn_whmt_vars.cmdId          = CMDID_CLEARNV;
   dn_whmt_vars.replyContents  = (uint8_t*)reply;
   
   // extraFlags
   extraFlags = 0x00;
   
   // build outputBuf
   
   // send outputBuf
   rc = dn_serial_mt_sendRequest(
      CMDID_CLEARNV,                                            // cmdId
      extraFlags,                                               // extraFlags
      dn_whmt_vars.outputBuf,                                   // payload
      DN_CLEARNV_REQ_LEN,                                       // length
      dn_whmt_clearNV_reply                                     // replyCb
   );
   
   if (rc==DN_ERR_NONE) {
      // I'm now busy transmitting
      dn_whmt_vars.busyTx         = TRUE;
   }
   
   // unlock the module
   dn_unlock();
   
   return rc;
   
}

void dn_whmt_clearNV_reply(uint8_t cmdId, uint8_t rc, uint8_t* payload, uint8_t len) {
   dn_whmt_clearNV_rpt* reply;
   
   // verify I'm expecting this answer
   if (dn_whmt_vars.busyTx==FALSE || dn_whmt_vars.cmdId!=cmdId) {
      return;
   }
   
   // do NOT verify length (no return fields expected)
   
   // cast the replyContent
   reply = (dn_whmt_clearNV_rpt*)dn_whmt_vars.replyContents;
   
   // store RC
   reply->RC = rc;
   
   // parse returned value (iff RC==0)
   if (rc==DN_SERIAL_RC_OK) {
      
   }
   
   // call the callback
   dn_whmt_vars.replyCb(cmdId);
   
   // I'm not busy transmitting anymore
   dn_whmt_vars.busyTx=FALSE;
}

//===== search

/**
The search command causes the mote to listen for network advertisements and 
notify the microprocessor about each advertisement it hears. This is referred 
to as the Promiscuous Listen state. Notifications are sent using the 
advReceived notification. The search command may only be issued prior to join. 
The mote stays in listen mode until the join command is received or the mote is 
reset. 
*/
dn_err_t dn_whmt_search(dn_whmt_search_rpt* reply) {
   uint8_t    extraFlags;
   dn_err_t   rc;
   
   // lock the module
   dn_lock();
   
   // verify no ongoing transmissions
   if (dn_whmt_vars.busyTx) {
      // unlock the module
      dn_unlock();
      
      // return
      return DN_ERR_BUSY;
   }
   
   // store callback information
   dn_whmt_vars.cmdId          = CMDID_SEARCH;
   dn_whmt_vars.replyContents  = (uint8_t*)reply;
   
   // extraFlags
   extraFlags = 0x00;
   
   // build outputBuf
   
   // send outputBuf
   rc = dn_serial_mt_sendRequest(
      CMDID_SEARCH,                                             // cmdId
      extraFlags,                                               // extraFlags
      dn_whmt_vars.outputBuf,                                   // payload
      DN_SEARCH_REQ_LEN,                                        // length
      dn_whmt_search_reply                                      // replyCb
   );
   
   if (rc==DN_ERR_NONE) {
      // I'm now busy transmitting
      dn_whmt_vars.busyTx         = TRUE;
   }
   
   // unlock the module
   dn_unlock();
   
   return rc;
   
}

void dn_whmt_search_reply(uint8_t cmdId, uint8_t rc, uint8_t* payload, uint8_t len) {
   dn_whmt_search_rpt* reply;
   
   // verify I'm expecting this answer
   if (dn_whmt_vars.busyTx==FALSE || dn_whmt_vars.cmdId!=cmdId) {
      return;
   }
   
   // do NOT verify length (no return fields expected)
   
   // cast the replyContent
   reply = (dn_whmt_search_rpt*)dn_whmt_vars.replyContents;
   
   // store RC
   reply->RC = rc;
   
   // parse returned value (iff RC==0)
   if (rc==DN_SERIAL_RC_OK) {
      
   }
   
   // call the callback
   dn_whmt_vars.replyCb(cmdId);
   
   // I'm not busy transmitting anymore
   dn_whmt_vars.busyTx=FALSE;
}

//===== testRadioTxExt

/**
The testRadioTxExt command allows the microprocessor to initiate a radio 
transmission test. This command may only be issued prior to the mote joining 
the network. Three types of transmission tests are supported:

- Packet transmission
- Continuous modulation
- Continuous wave (unmodulated signal)

In a packet transmission test, the mote generates a repeatCnt number of packet 
sequences. Each sequence consists of up to 10 packets with configurable size 
and delays. Each packet starts with a PHY preamble (5 bytes), followed by a PHY 
length field (1 byte), followed by data payload of up to 125 bytes, and finally 
a 2-byte 802.15.4 CRC at the end. Byte 0 of the payload contains stationId of 
the sender. Bytes 1 and 2 contain the packet number (in big-endian format) that 
increments with every packet transmitted. Bytes 3..N contain a counter (from 
0..N-3) that increments with every byte inside payload. Transmissions occur on 
the set of channels defined by chanMask , selected inpseudo-randomorder.

In a continuous modulation test, the mote generates continuous pseudo-random 
modulated signal, centered at the specified channel. The test is stopped by 
resetting the mote.

In a continuous wave test, the mote generates an unmodulated tone, centered at 
the specified channel. The test tone is stopped by resetting the mote.

The testRadioTxExt command may only be issued when the mote is in Idle mode, 
prior to its joining the network. The mote must be reset (either hardware or 
software reset) after radio tests are complete and prior to joining.

The station ID is a user selectable value. It is used in packet tests so that a 
receiver can identify packets from this device in cases where there may be 
multiple tests running in the same radio space. This field is not used for CM 
or CW tests. See testRadioRX (SmartMesh IP) or testRadioRxExt (SmartMesh WirelessHART).



Channel numbering is 0-15, corresponding to IEEE 2.4 GHz channels 11-26. 
*/
dn_err_t dn_whmt_testRadioTxExt(uint8_t testType, uint16_t chanMask, uint16_t repeatCnt, int8_t txPower, uint8_t seqSize, uint8_t pkLen_1, uint16_t delay_1, uint8_t pkLen_2, uint16_t delay_2, uint8_t pkLen_3, uint16_t delay_3, uint8_t pkLen_4, uint16_t delay_4, uint8_t pkLen_5, uint16_t delay_5, uint8_t pkLen_6, uint16_t delay_6, uint8_t pkLen_7, uint16_t delay_7, uint8_t pkLen_8, uint16_t delay_8, uint8_t pkLen_9, uint16_t delay_9, uint8_t pkLen_10, uint16_t delay_10, uint8_t stationId, dn_whmt_testRadioTxExt_rpt* reply) {
   uint8_t    extraFlags;
   dn_err_t   rc;
   
   // lock the module
   dn_lock();
   
   // verify no ongoing transmissions
   if (dn_whmt_vars.busyTx) {
      // unlock the module
      dn_unlock();
      
      // return
      return DN_ERR_BUSY;
   }
   
   // store callback information
   dn_whmt_vars.cmdId          = CMDID_TESTRADIOTXEXT;
   dn_whmt_vars.replyContents  = (uint8_t*)reply;
   
   // extraFlags
   extraFlags = 0x00;
   
   // build outputBuf
   dn_whmt_vars.outputBuf[DN_TESTRADIOTXEXT_REQ_OFFS_TESTTYPE] = testType;
   dn_write_uint16_t(&dn_whmt_vars.outputBuf[DN_TESTRADIOTXEXT_REQ_OFFS_CHANMASK],chanMask);
   dn_write_uint16_t(&dn_whmt_vars.outputBuf[DN_TESTRADIOTXEXT_REQ_OFFS_REPEATCNT],repeatCnt);
   dn_whmt_vars.outputBuf[DN_TESTRADIOTXEXT_REQ_OFFS_TXPOWER] = (int8_t)txPower;
   dn_whmt_vars.outputBuf[DN_TESTRADIOTXEXT_REQ_OFFS_SEQSIZE] = seqSize;
   dn_whmt_vars.outputBuf[DN_TESTRADIOTXEXT_REQ_OFFS_PKLEN_1] = pkLen_1;
   dn_write_uint16_t(&dn_whmt_vars.outputBuf[DN_TESTRADIOTXEXT_REQ_OFFS_DELAY_1],delay_1);
   dn_whmt_vars.outputBuf[DN_TESTRADIOTXEXT_REQ_OFFS_PKLEN_2] = pkLen_2;
   dn_write_uint16_t(&dn_whmt_vars.outputBuf[DN_TESTRADIOTXEXT_REQ_OFFS_DELAY_2],delay_2);
   dn_whmt_vars.outputBuf[DN_TESTRADIOTXEXT_REQ_OFFS_PKLEN_3] = pkLen_3;
   dn_write_uint16_t(&dn_whmt_vars.outputBuf[DN_TESTRADIOTXEXT_REQ_OFFS_DELAY_3],delay_3);
   dn_whmt_vars.outputBuf[DN_TESTRADIOTXEXT_REQ_OFFS_PKLEN_4] = pkLen_4;
   dn_write_uint16_t(&dn_whmt_vars.outputBuf[DN_TESTRADIOTXEXT_REQ_OFFS_DELAY_4],delay_4);
   dn_whmt_vars.outputBuf[DN_TESTRADIOTXEXT_REQ_OFFS_PKLEN_5] = pkLen_5;
   dn_write_uint16_t(&dn_whmt_vars.outputBuf[DN_TESTRADIOTXEXT_REQ_OFFS_DELAY_5],delay_5);
   dn_whmt_vars.outputBuf[DN_TESTRADIOTXEXT_REQ_OFFS_PKLEN_6] = pkLen_6;
   dn_write_uint16_t(&dn_whmt_vars.outputBuf[DN_TESTRADIOTXEXT_REQ_OFFS_DELAY_6],delay_6);
   dn_whmt_vars.outputBuf[DN_TESTRADIOTXEXT_REQ_OFFS_PKLEN_7] = pkLen_7;
   dn_write_uint16_t(&dn_whmt_vars.outputBuf[DN_TESTRADIOTXEXT_REQ_OFFS_DELAY_7],delay_7);
   dn_whmt_vars.outputBuf[DN_TESTRADIOTXEXT_REQ_OFFS_PKLEN_8] = pkLen_8;
   dn_write_uint16_t(&dn_whmt_vars.outputBuf[DN_TESTRADIOTXEXT_REQ_OFFS_DELAY_8],delay_8);
   dn_whmt_vars.outputBuf[DN_TESTRADIOTXEXT_REQ_OFFS_PKLEN_9] = pkLen_9;
   dn_write_uint16_t(&dn_whmt_vars.outputBuf[DN_TESTRADIOTXEXT_REQ_OFFS_DELAY_9],delay_9);
   dn_whmt_vars.outputBuf[DN_TESTRADIOTXEXT_REQ_OFFS_PKLEN_10] = pkLen_10;
   dn_write_uint16_t(&dn_whmt_vars.outputBuf[DN_TESTRADIOTXEXT_REQ_OFFS_DELAY_10],delay_10);
   dn_whmt_vars.outputBuf[DN_TESTRADIOTXEXT_REQ_OFFS_STATIONID] = stationId;
   
   // send outputBuf
   rc = dn_serial_mt_sendRequest(
      CMDID_TESTRADIOTXEXT,                                     // cmdId
      extraFlags,                                               // extraFlags
      dn_whmt_vars.outputBuf,                                   // payload
      DN_TESTRADIOTXEXT_REQ_LEN,                                // length
      dn_whmt_testRadioTxExt_reply                              // replyCb
   );
   
   if (rc==DN_ERR_NONE) {
      // I'm now busy transmitting
      dn_whmt_vars.busyTx         = TRUE;
   }
   
   // unlock the module
   dn_unlock();
   
   return rc;
   
}

void dn_whmt_testRadioTxExt_reply(uint8_t cmdId, uint8_t rc, uint8_t* payload, uint8_t len) {
   dn_whmt_testRadioTxExt_rpt* reply;
   
   // verify I'm expecting this answer
   if (dn_whmt_vars.busyTx==FALSE || dn_whmt_vars.cmdId!=cmdId) {
      return;
   }
   
   // do NOT verify length (no return fields expected)
   
   // cast the replyContent
   reply = (dn_whmt_testRadioTxExt_rpt*)dn_whmt_vars.replyContents;
   
   // store RC
   reply->RC = rc;
   
   // parse returned value (iff RC==0)
   if (rc==DN_SERIAL_RC_OK) {
      
   }
   
   // call the callback
   dn_whmt_vars.replyCb(cmdId);
   
   // I'm not busy transmitting anymore
   dn_whmt_vars.busyTx=FALSE;
}

//===== testRadioRxExt

/**
The testRadioRxExt command clears all previously collected statistics and 
initiates a test of radio reception for the specified channel and duration. 
During the test, the mote keeps statistics about the number of packets received 
(with and without error). The test results may be retrieved using the 
getParameter<testRadioRxStats> command.The mote must be reset (either hardware 
or software reset) after radio tests are complete and prior to joining.

Station ID is available in IP mote >= 1.4, and WirelessHART mote >= 1.1.2. The 
station ID is a user selectable value used to isolate traffic if multiple tests 
are running in the same radio space. It must be set to match the station ID 
used by the transmitter.



Channel numbering is 0-15, corresponding to IEEE 2.4 GHz channels 11-26. 
*/
dn_err_t dn_whmt_testRadioRxExt(uint16_t channelMask, uint16_t time, uint8_t stationId, dn_whmt_testRadioRxExt_rpt* reply) {
   uint8_t    extraFlags;
   dn_err_t   rc;
   
   // lock the module
   dn_lock();
   
   // verify no ongoing transmissions
   if (dn_whmt_vars.busyTx) {
      // unlock the module
      dn_unlock();
      
      // return
      return DN_ERR_BUSY;
   }
   
   // store callback information
   dn_whmt_vars.cmdId          = CMDID_TESTRADIORXEXT;
   dn_whmt_vars.replyContents  = (uint8_t*)reply;
   
   // extraFlags
   extraFlags = 0x00;
   
   // build outputBuf
   dn_write_uint16_t(&dn_whmt_vars.outputBuf[DN_TESTRADIORXEXT_REQ_OFFS_CHANNELMASK],channelMask);
   dn_write_uint16_t(&dn_whmt_vars.outputBuf[DN_TESTRADIORXEXT_REQ_OFFS_TIME],time);
   dn_whmt_vars.outputBuf[DN_TESTRADIORXEXT_REQ_OFFS_STATIONID] = stationId;
   
   // send outputBuf
   rc = dn_serial_mt_sendRequest(
      CMDID_TESTRADIORXEXT,                                     // cmdId
      extraFlags,                                               // extraFlags
      dn_whmt_vars.outputBuf,                                   // payload
      DN_TESTRADIORXEXT_REQ_LEN,                                // length
      dn_whmt_testRadioRxExt_reply                              // replyCb
   );
   
   if (rc==DN_ERR_NONE) {
      // I'm now busy transmitting
      dn_whmt_vars.busyTx         = TRUE;
   }
   
   // unlock the module
   dn_unlock();
   
   return rc;
   
}

void dn_whmt_testRadioRxExt_reply(uint8_t cmdId, uint8_t rc, uint8_t* payload, uint8_t len) {
   dn_whmt_testRadioRxExt_rpt* reply;
   
   // verify I'm expecting this answer
   if (dn_whmt_vars.busyTx==FALSE || dn_whmt_vars.cmdId!=cmdId) {
      return;
   }
   
   // do NOT verify length (no return fields expected)
   
   // cast the replyContent
   reply = (dn_whmt_testRadioRxExt_rpt*)dn_whmt_vars.replyContents;
   
   // store RC
   reply->RC = rc;
   
   // parse returned value (iff RC==0)
   if (rc==DN_SERIAL_RC_OK) {
      
   }
   
   // call the callback
   dn_whmt_vars.replyCb(cmdId);
   
   // I'm not busy transmitting anymore
   dn_whmt_vars.busyTx=FALSE;
}

//===== zeroize

/**
The zeroize (zeroise) command erases flash area that is used to store 
configuration parameters, such as join keys. This command is intended to 
satisfy the zeroization requirement of the FIPS-140 standard. After the command 
executes, the mote should be reset. Available in mote >= 1.1.x

The zeroize command will render the mote inoperable. It must be re-programmed 
via SPI or JTAG in order to be useable. 
*/
dn_err_t dn_whmt_zeroize(dn_whmt_zeroize_rpt* reply) {
   uint8_t    extraFlags;
   dn_err_t   rc;
   
   // lock the module
   dn_lock();
   
   // verify no ongoing transmissions
   if (dn_whmt_vars.busyTx) {
      // unlock the module
      dn_unlock();
      
      // return
      return DN_ERR_BUSY;
   }
   
   // store callback information
   dn_whmt_vars.cmdId          = CMDID_ZEROIZE;
   dn_whmt_vars.replyContents  = (uint8_t*)reply;
   
   // extraFlags
   extraFlags = 0x00;
   
   // build outputBuf
   
   // send outputBuf
   rc = dn_serial_mt_sendRequest(
      CMDID_ZEROIZE,                                            // cmdId
      extraFlags,                                               // extraFlags
      dn_whmt_vars.outputBuf,                                   // payload
      DN_ZEROIZE_REQ_LEN,                                       // length
      dn_whmt_zeroize_reply                                     // replyCb
   );
   
   if (rc==DN_ERR_NONE) {
      // I'm now busy transmitting
      dn_whmt_vars.busyTx         = TRUE;
   }
   
   // unlock the module
   dn_unlock();
   
   return rc;
   
}

void dn_whmt_zeroize_reply(uint8_t cmdId, uint8_t rc, uint8_t* payload, uint8_t len) {
   dn_whmt_zeroize_rpt* reply;
   
   // verify I'm expecting this answer
   if (dn_whmt_vars.busyTx==FALSE || dn_whmt_vars.cmdId!=cmdId) {
      return;
   }
   
   // do NOT verify length (no return fields expected)
   
   // cast the replyContent
   reply = (dn_whmt_zeroize_rpt*)dn_whmt_vars.replyContents;
   
   // store RC
   reply->RC = rc;
   
   // parse returned value (iff RC==0)
   if (rc==DN_SERIAL_RC_OK) {
      
   }
   
   // call the callback
   dn_whmt_vars.replyCb(cmdId);
   
   // I'm not busy transmitting anymore
   dn_whmt_vars.busyTx=FALSE;
}

//===== fileWrite

/**
The fileWrite command may be used to read data stored in the scratchpad file in 
the mote filesystem. The size of the data read is limited by the size of a 
serial API transaction. 
*/
dn_err_t dn_whmt_fileWrite(int32_t descriptor, uint16_t offset, uint8_t length, uint8_t* data, uint8_t dataLen, dn_whmt_fileWrite_rpt* reply) {
   uint8_t    extraFlags;
   dn_err_t   rc;
   
   // lock the module
   dn_lock();
   
   // verify no ongoing transmissions
   if (dn_whmt_vars.busyTx) {
      // unlock the module
      dn_unlock();
      
      // return
      return DN_ERR_BUSY;
   }
   
   // store callback information
   dn_whmt_vars.cmdId          = CMDID_FILEWRITE;
   dn_whmt_vars.replyContents  = (uint8_t*)reply;
   
   // extraFlags
   extraFlags = 0x00;
   
   // build outputBuf
   dn_write_int32_t(&dn_whmt_vars.outputBuf[DN_FILEWRITE_REQ_OFFS_DESCRIPTOR],descriptor);
   dn_write_uint16_t(&dn_whmt_vars.outputBuf[DN_FILEWRITE_REQ_OFFS_OFFSET],offset);
   dn_whmt_vars.outputBuf[DN_FILEWRITE_REQ_OFFS_LENGTH] = length;
   memcpy(&dn_whmt_vars.outputBuf[DN_FILEWRITE_REQ_OFFS_DATA],data,dataLen);
   
   // send outputBuf
   rc = dn_serial_mt_sendRequest(
      CMDID_FILEWRITE,                                          // cmdId
      extraFlags,                                               // extraFlags
      dn_whmt_vars.outputBuf,                                   // payload
      DN_FILEWRITE_REQ_LEN+dataLen,                             // length
      dn_whmt_fileWrite_reply                                   // replyCb
   );
   
   if (rc==DN_ERR_NONE) {
      // I'm now busy transmitting
      dn_whmt_vars.busyTx         = TRUE;
   }
   
   // unlock the module
   dn_unlock();
   
   return rc;
   
}

void dn_whmt_fileWrite_reply(uint8_t cmdId, uint8_t rc, uint8_t* payload, uint8_t len) {
   dn_whmt_fileWrite_rpt* reply;
   
   // verify I'm expecting this answer
   if (dn_whmt_vars.busyTx==FALSE || dn_whmt_vars.cmdId!=cmdId) {
      return;
   }
   
   // verify length
   if (rc==DN_SERIAL_RC_OK && len<DN_FILEWRITE_REPLY_LEN) {
      return;
   }
   
   // cast the replyContent
   reply = (dn_whmt_fileWrite_rpt*)dn_whmt_vars.replyContents;
   
   // store RC
   reply->RC = rc;
   
   // parse returned value (iff RC==0)
   if (rc==DN_SERIAL_RC_OK) {
      
      dn_read_int32_t(&reply->length,&payload[DN_FILEWRITE_REPLY_OFFS_LENGTH]);
   }
   
   // call the callback
   dn_whmt_vars.replyCb(cmdId);
   
   // I'm not busy transmitting anymore
   dn_whmt_vars.busyTx=FALSE;
}

//===== fileRead

/**
The fileRead command may be used to read data stored in the scratchpad file in 
the mote filesystem. The size of the data read is limited by the size of a 
serial API transaction. 
*/
dn_err_t dn_whmt_fileRead(int32_t descriptor, uint16_t offset, uint8_t length, dn_whmt_fileRead_rpt* reply) {
   uint8_t    extraFlags;
   dn_err_t   rc;
   
   // lock the module
   dn_lock();
   
   // verify no ongoing transmissions
   if (dn_whmt_vars.busyTx) {
      // unlock the module
      dn_unlock();
      
      // return
      return DN_ERR_BUSY;
   }
   
   // store callback information
   dn_whmt_vars.cmdId          = CMDID_FILEREAD;
   dn_whmt_vars.replyContents  = (uint8_t*)reply;
   
   // extraFlags
   extraFlags = 0x00;
   
   // build outputBuf
   dn_write_int32_t(&dn_whmt_vars.outputBuf[DN_FILEREAD_REQ_OFFS_DESCRIPTOR],descriptor);
   dn_write_uint16_t(&dn_whmt_vars.outputBuf[DN_FILEREAD_REQ_OFFS_OFFSET],offset);
   dn_whmt_vars.outputBuf[DN_FILEREAD_REQ_OFFS_LENGTH] = length;
   
   // send outputBuf
   rc = dn_serial_mt_sendRequest(
      CMDID_FILEREAD,                                           // cmdId
      extraFlags,                                               // extraFlags
      dn_whmt_vars.outputBuf,                                   // payload
      DN_FILEREAD_REQ_LEN,                                      // length
      dn_whmt_fileRead_reply                                    // replyCb
   );
   
   if (rc==DN_ERR_NONE) {
      // I'm now busy transmitting
      dn_whmt_vars.busyTx         = TRUE;
   }
   
   // unlock the module
   dn_unlock();
   
   return rc;
   
}

void dn_whmt_fileRead_reply(uint8_t cmdId, uint8_t rc, uint8_t* payload, uint8_t len) {
   dn_whmt_fileRead_rpt* reply;
   
   // verify I'm expecting this answer
   if (dn_whmt_vars.busyTx==FALSE || dn_whmt_vars.cmdId!=cmdId) {
      return;
   }
   
   // verify length
   if (rc==DN_SERIAL_RC_OK && len<DN_FILEREAD_REPLY_LEN) {
      return;
   }
   
   // cast the replyContent
   reply = (dn_whmt_fileRead_rpt*)dn_whmt_vars.replyContents;
   
   // store RC
   reply->RC = rc;
   
   // parse returned value (iff RC==0)
   if (rc==DN_SERIAL_RC_OK) {
      
      dn_read_int32_t(&reply->descriptor,&payload[DN_FILEREAD_REPLY_OFFS_DESCRIPTOR]);
      dn_read_uint16_t(&reply->offset,&payload[DN_FILEREAD_REPLY_OFFS_OFFSET]);
      reply->length = payload[DN_FILEREAD_REPLY_OFFS_LENGTH];
      memcpy(&reply->data[0],&payload[DN_FILEREAD_REPLY_OFFS_DATA],len-DN_FILEREAD_REPLY_OFFS_DATA);
   }
   
   // call the callback
   dn_whmt_vars.replyCb(cmdId);
   
   // I'm not busy transmitting anymore
   dn_whmt_vars.busyTx=FALSE;
}

//===== fileOpen

/**
The fileOpen command may be used to open the scratchpad file in the mote 
filesystem. 
*/
dn_err_t dn_whmt_fileOpen(uint8_t* name, uint8_t options, uint16_t size, uint8_t mode, dn_whmt_fileOpen_rpt* reply) {
   uint8_t    extraFlags;
   dn_err_t   rc;
   
   // lock the module
   dn_lock();
   
   // verify no ongoing transmissions
   if (dn_whmt_vars.busyTx) {
      // unlock the module
      dn_unlock();
      
      // return
      return DN_ERR_BUSY;
   }
   
   // store callback information
   dn_whmt_vars.cmdId          = CMDID_FILEOPEN;
   dn_whmt_vars.replyContents  = (uint8_t*)reply;
   
   // extraFlags
   extraFlags = 0x00;
   
   // build outputBuf
   memcpy(&dn_whmt_vars.outputBuf[DN_FILEOPEN_REQ_OFFS_NAME],name,12);
   dn_whmt_vars.outputBuf[DN_FILEOPEN_REQ_OFFS_OPTIONS] = options;
   dn_write_uint16_t(&dn_whmt_vars.outputBuf[DN_FILEOPEN_REQ_OFFS_SIZE],size);
   dn_whmt_vars.outputBuf[DN_FILEOPEN_REQ_OFFS_MODE] = mode;
   
   // send outputBuf
   rc = dn_serial_mt_sendRequest(
      CMDID_FILEOPEN,                                           // cmdId
      extraFlags,                                               // extraFlags
      dn_whmt_vars.outputBuf,                                   // payload
      DN_FILEOPEN_REQ_LEN,                                      // length
      dn_whmt_fileOpen_reply                                    // replyCb
   );
   
   if (rc==DN_ERR_NONE) {
      // I'm now busy transmitting
      dn_whmt_vars.busyTx         = TRUE;
   }
   
   // unlock the module
   dn_unlock();
   
   return rc;
   
}

void dn_whmt_fileOpen_reply(uint8_t cmdId, uint8_t rc, uint8_t* payload, uint8_t len) {
   dn_whmt_fileOpen_rpt* reply;
   
   // verify I'm expecting this answer
   if (dn_whmt_vars.busyTx==FALSE || dn_whmt_vars.cmdId!=cmdId) {
      return;
   }
   
   // verify length
   if (rc==DN_SERIAL_RC_OK && len<DN_FILEOPEN_REPLY_LEN) {
      return;
   }
   
   // cast the replyContent
   reply = (dn_whmt_fileOpen_rpt*)dn_whmt_vars.replyContents;
   
   // store RC
   reply->RC = rc;
   
   // parse returned value (iff RC==0)
   if (rc==DN_SERIAL_RC_OK) {
      
      dn_read_int32_t(&reply->descriptor,&payload[DN_FILEOPEN_REPLY_OFFS_DESCRIPTOR]);
   }
   
   // call the callback
   dn_whmt_vars.replyCb(cmdId);
   
   // I'm not busy transmitting anymore
   dn_whmt_vars.busyTx=FALSE;
}

//========== serialRX

void dn_whmt_rxSerialRequest(uint8_t cmdId, uint8_t flags, uint8_t* payload, uint8_t len) {
   dn_whmt_timeIndication_nt* notif_timeIndication;
   dn_whmt_serviceIndication_nt* notif_serviceIndication;
   dn_whmt_events_nt* notif_events;
   dn_whmt_dataReceived_nt* notif_dataReceived;
   dn_whmt_advReceived_nt* notif_advReceived;
   dn_whmt_suspendStarted_nt* notif_suspendStarted;
   
   // parse notification
   switch(cmdId) {
      case CMDID_TIMEINDICATION:
         
         // verify length payload received
         if (len<15) {
            return;
         }
         
         // verify length notifBuf
         if (len>dn_whmt_vars.notifBufLen) {
            return;
         }
         
         // cast notifBuf
         notif_timeIndication = (dn_whmt_timeIndication_nt*)dn_whmt_vars.notifBuf;
         
         // parse the notification
         dn_read_uint32_t(&notif_timeIndication->utcSec,&payload[DN_TIMEINDICATION_NOTIF_OFFS_UTCSEC]);
         dn_read_uint32_t(&notif_timeIndication->utcMicroSec,&payload[DN_TIMEINDICATION_NOTIF_OFFS_UTCMICROSEC]);
         memcpy(&notif_timeIndication->asn[0],&payload[DN_TIMEINDICATION_NOTIF_OFFS_ASN],5);
         dn_read_uint16_t(&notif_timeIndication->asnOffset,&payload[DN_TIMEINDICATION_NOTIF_OFFS_ASNOFFSET]);
         break;
      case CMDID_SERVICEINDICATION:
         
         // verify length payload received
         if (len<12) {
            return;
         }
         
         // verify length notifBuf
         if (len>dn_whmt_vars.notifBufLen) {
            return;
         }
         
         // cast notifBuf
         notif_serviceIndication = (dn_whmt_serviceIndication_nt*)dn_whmt_vars.notifBuf;
         
         // parse the notification
         notif_serviceIndication->eventCode = payload[DN_SERVICEINDICATION_NOTIF_OFFS_EVENTCODE];
         notif_serviceIndication->netMgrCode = payload[DN_SERVICEINDICATION_NOTIF_OFFS_NETMGRCODE];
         notif_serviceIndication->serviceId = payload[DN_SERVICEINDICATION_NOTIF_OFFS_SERVICEID];
         notif_serviceIndication->serviceState = payload[DN_SERVICEINDICATION_NOTIF_OFFS_SERVICESTATE];
         notif_serviceIndication->serviceFlags = payload[DN_SERVICEINDICATION_NOTIF_OFFS_SERVICEFLAGS];
         notif_serviceIndication->appDomain = payload[DN_SERVICEINDICATION_NOTIF_OFFS_APPDOMAIN];
         dn_read_uint16_t(&notif_serviceIndication->destAddr,&payload[DN_SERVICEINDICATION_NOTIF_OFFS_DESTADDR]);
         dn_read_uint32_t(&notif_serviceIndication->time,&payload[DN_SERVICEINDICATION_NOTIF_OFFS_TIME]);
         break;
      case CMDID_EVENTS:
         
         // verify length payload received
         if (len<9) {
            return;
         }
         
         // verify length notifBuf
         if (len>dn_whmt_vars.notifBufLen) {
            return;
         }
         
         // cast notifBuf
         notif_events = (dn_whmt_events_nt*)dn_whmt_vars.notifBuf;
         
         // parse the notification
         dn_read_uint32_t(&notif_events->events,&payload[DN_EVENTS_NOTIF_OFFS_EVENTS]);
         notif_events->state = payload[DN_EVENTS_NOTIF_OFFS_STATE];
         dn_read_uint32_t(&notif_events->moteAlarms,&payload[DN_EVENTS_NOTIF_OFFS_MOTEALARMS]);
         break;
      case CMDID_DATARECEIVED:
         
         // verify length payload received
         if (len<4) {
            return;
         }
         
         // verify length notifBuf
         if (len>dn_whmt_vars.notifBufLen) {
            return;
         }
         
         // cast notifBuf
         notif_dataReceived = (dn_whmt_dataReceived_nt*)dn_whmt_vars.notifBuf;
         
         // parse flags
         if (flags & (1<<6)) {
            notif_dataReceived->tranType    = 0x01;
         } else {
            notif_dataReceived->tranType    = 0x00;
         }
         if (flags & (1<<7)) {
            notif_dataReceived->tranDir     = 0x01;
         } else {
            notif_dataReceived->tranDir     = 0x00;
         }
         
         // parse the notification
         dn_read_uint16_t(&notif_dataReceived->srcAddr,&payload[DN_DATARECEIVED_NOTIF_OFFS_SRCADDR]);
         notif_dataReceived->seqNum = payload[DN_DATARECEIVED_NOTIF_OFFS_SEQNUM];
         notif_dataReceived->pktLength = payload[DN_DATARECEIVED_NOTIF_OFFS_PKTLENGTH];
         memcpy(&notif_dataReceived->data[0],&payload[DN_DATARECEIVED_NOTIF_OFFS_DATA],len-DN_DATARECEIVED_NOTIF_OFFS_DATA);
         break;
      case CMDID_ADVRECEIVED:
         
         // verify length payload received
         if (len<6) {
            return;
         }
         
         // verify length notifBuf
         if (len>dn_whmt_vars.notifBufLen) {
            return;
         }
         
         // cast notifBuf
         notif_advReceived = (dn_whmt_advReceived_nt*)dn_whmt_vars.notifBuf;
         
         // parse the notification
         dn_read_uint16_t(&notif_advReceived->netId,&payload[DN_ADVRECEIVED_NOTIF_OFFS_NETID]);
         dn_read_uint16_t(&notif_advReceived->moteid,&payload[DN_ADVRECEIVED_NOTIF_OFFS_MOTEID]);
         notif_advReceived->rssi = (int8_t)payload[DN_ADVRECEIVED_NOTIF_OFFS_RSSI];
         notif_advReceived->joinPri = payload[DN_ADVRECEIVED_NOTIF_OFFS_JOINPRI];
         break;
      case CMDID_SUSPENDSTARTED:
         
         // verify length payload received
         if (len<4) {
            return;
         }
         
         // verify length notifBuf
         if (len>dn_whmt_vars.notifBufLen) {
            return;
         }
         
         // cast notifBuf
         notif_suspendStarted = (dn_whmt_suspendStarted_nt*)dn_whmt_vars.notifBuf;
         
         // parse the notification
         dn_read_uint32_t(&notif_suspendStarted->duration,&payload[DN_SUSPENDSTARTED_NOTIF_OFFS_DURATION]);
         break;
      default:
         // unknown cmdID
         return;
   }
   
   // call the callback
   dn_whmt_vars.notifCb(cmdId,DN_SUBCMDID_NONE);
}

//=========================== helpers =========================================

