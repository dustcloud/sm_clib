/*
Copyright (c) 2015, Dust Networks. All rights reserved.

C library to connect to a SmartMesh IP Manager.

\license See attached DN_LICENSE.txt.
*/

#include "dn_ipmg.h"
#include "dn_lock.h"
#include "dn_serial_mg.h"

//=========================== variables =======================================

typedef struct {
   // sending requests
   uint8_t              outputBuf[MAX_FRAME_LENGTH];
   bool                 busyTx;
   uint8_t              cmdId;
   uint8_t              paramId;
   // receiving replies
   dn_ipmg_reply_cbt    replyCb;
   uint8_t*             replyContents;
   // receiving notifications
   dn_ipmg_notif_cbt    notifCb;
   uint8_t*             notifBuf;
   uint8_t              notifBufLen;
} dn_ipmg_vars_t;

dn_ipmg_vars_t dn_ipmg_vars;

//=========================== prototypes ======================================

// API
void dn_ipmg_reset_reply(uint8_t cmdId, uint8_t rc, uint8_t* payload, uint8_t len);
void dn_ipmg_subscribe_reply(uint8_t cmdId, uint8_t rc, uint8_t* payload, uint8_t len);
void dn_ipmg_getTime_reply(uint8_t cmdId, uint8_t rc, uint8_t* payload, uint8_t len);
void dn_ipmg_setNetworkConfig_reply(uint8_t cmdId, uint8_t rc, uint8_t* payload, uint8_t len);
void dn_ipmg_clearStatistics_reply(uint8_t cmdId, uint8_t rc, uint8_t* payload, uint8_t len);
void dn_ipmg_exchangeMoteJoinKey_reply(uint8_t cmdId, uint8_t rc, uint8_t* payload, uint8_t len);
void dn_ipmg_exchangeNetworkId_reply(uint8_t cmdId, uint8_t rc, uint8_t* payload, uint8_t len);
void dn_ipmg_radiotestTx_reply(uint8_t cmdId, uint8_t rc, uint8_t* payload, uint8_t len);
void dn_ipmg_radiotestRx_reply(uint8_t cmdId, uint8_t rc, uint8_t* payload, uint8_t len);
void dn_ipmg_getRadiotestStatistics_reply(uint8_t cmdId, uint8_t rc, uint8_t* payload, uint8_t len);
void dn_ipmg_setACLEntry_reply(uint8_t cmdId, uint8_t rc, uint8_t* payload, uint8_t len);
void dn_ipmg_getNextACLEntry_reply(uint8_t cmdId, uint8_t rc, uint8_t* payload, uint8_t len);
void dn_ipmg_deleteACLEntry_reply(uint8_t cmdId, uint8_t rc, uint8_t* payload, uint8_t len);
void dn_ipmg_pingMote_reply(uint8_t cmdId, uint8_t rc, uint8_t* payload, uint8_t len);
void dn_ipmg_getLog_reply(uint8_t cmdId, uint8_t rc, uint8_t* payload, uint8_t len);
void dn_ipmg_sendData_reply(uint8_t cmdId, uint8_t rc, uint8_t* payload, uint8_t len);
void dn_ipmg_startNetwork_reply(uint8_t cmdId, uint8_t rc, uint8_t* payload, uint8_t len);
void dn_ipmg_getSystemInfo_reply(uint8_t cmdId, uint8_t rc, uint8_t* payload, uint8_t len);
void dn_ipmg_getMoteConfig_reply(uint8_t cmdId, uint8_t rc, uint8_t* payload, uint8_t len);
void dn_ipmg_getPathInfo_reply(uint8_t cmdId, uint8_t rc, uint8_t* payload, uint8_t len);
void dn_ipmg_getNextPathInfo_reply(uint8_t cmdId, uint8_t rc, uint8_t* payload, uint8_t len);
void dn_ipmg_setAdvertising_reply(uint8_t cmdId, uint8_t rc, uint8_t* payload, uint8_t len);
void dn_ipmg_setDownstreamFrameMode_reply(uint8_t cmdId, uint8_t rc, uint8_t* payload, uint8_t len);
void dn_ipmg_getManagerStatistics_reply(uint8_t cmdId, uint8_t rc, uint8_t* payload, uint8_t len);
void dn_ipmg_setTime_reply(uint8_t cmdId, uint8_t rc, uint8_t* payload, uint8_t len);
void dn_ipmg_getLicense_reply(uint8_t cmdId, uint8_t rc, uint8_t* payload, uint8_t len);
void dn_ipmg_setLicense_reply(uint8_t cmdId, uint8_t rc, uint8_t* payload, uint8_t len);
void dn_ipmg_setCLIUser_reply(uint8_t cmdId, uint8_t rc, uint8_t* payload, uint8_t len);
void dn_ipmg_sendIP_reply(uint8_t cmdId, uint8_t rc, uint8_t* payload, uint8_t len);
void dn_ipmg_restoreFactoryDefaults_reply(uint8_t cmdId, uint8_t rc, uint8_t* payload, uint8_t len);
void dn_ipmg_getMoteInfo_reply(uint8_t cmdId, uint8_t rc, uint8_t* payload, uint8_t len);
void dn_ipmg_getNetworkConfig_reply(uint8_t cmdId, uint8_t rc, uint8_t* payload, uint8_t len);
void dn_ipmg_getNetworkInfo_reply(uint8_t cmdId, uint8_t rc, uint8_t* payload, uint8_t len);
void dn_ipmg_getMoteConfigById_reply(uint8_t cmdId, uint8_t rc, uint8_t* payload, uint8_t len);
void dn_ipmg_setCommonJoinKey_reply(uint8_t cmdId, uint8_t rc, uint8_t* payload, uint8_t len);
void dn_ipmg_getIPConfig_reply(uint8_t cmdId, uint8_t rc, uint8_t* payload, uint8_t len);
void dn_ipmg_setIPConfig_reply(uint8_t cmdId, uint8_t rc, uint8_t* payload, uint8_t len);
void dn_ipmg_deleteMote_reply(uint8_t cmdId, uint8_t rc, uint8_t* payload, uint8_t len);
void dn_ipmg_getMoteLinks_reply(uint8_t cmdId, uint8_t rc, uint8_t* payload, uint8_t len);

// serial RX
void dn_ipmg_rxSerialRequest(uint8_t cmdId, uint8_t flags, uint8_t* payload, uint8_t len);

//=========================== public ==========================================

//========== admin

/**
\brief Setting up the instance.
*/
void dn_ipmg_init(dn_ipmg_notif_cbt notifCb, uint8_t* notifBuf, uint8_t notifBufLen, dn_ipmg_reply_cbt replyCb, dn_ipmg_status_cbt statusCb) {
   
   // reset local variables
   memset(&dn_ipmg_vars,0,sizeof(dn_ipmg_vars));
   
   // store params
   dn_ipmg_vars.notifCb         = notifCb;
   dn_ipmg_vars.notifBuf        = notifBuf;
   dn_ipmg_vars.notifBufLen     = notifBufLen;
   dn_ipmg_vars.replyCb         = replyCb;
   
   // initialize the serial connection
   dn_serial_mg_init(dn_ipmg_rxSerialRequest, statusCb);
}

void dn_ipmg_cancelTx() {
   
   // lock the module
   dn_lock();
   
   dn_ipmg_vars.busyTx=FALSE;
   
   // unlock the module
   dn_unlock();
}


dn_err_t dn_ipmg_initiateConnect() {
   dn_err_t rc;
   
   // lock the module
   dn_lock();
   
   // have the serial module connect
   rc = dn_serial_mg_initiateConnect();
   
   // unlock the module
   dn_unlock();
   
   return rc;
}

//========== API

//===== reset

/**
The reset command is used to reset various objects. The command argument is an 
object type, and if the object is a mote the MAC address must be specified 
(otherwise that argument is ignored). 
*/
dn_err_t dn_ipmg_reset(uint8_t type, uint8_t* macAddress, dn_ipmg_reset_rpt* reply) {
   uint8_t    extraFlags;
   dn_err_t   rc;
   
   // lock the module
   dn_lock();
   
   // verify no ongoing transmissions
   if (dn_ipmg_vars.busyTx) {
      // unlock the module
      dn_unlock();
      
      // return
      return DN_ERR_BUSY;
   }
   
   // store callback information
   dn_ipmg_vars.cmdId          = CMDID_RESET;
   dn_ipmg_vars.replyContents  = (uint8_t*)reply;
   
   // extraFlags
   extraFlags = 0x00;
   
   // build outputBuf
   dn_ipmg_vars.outputBuf[DN_RESET_REQ_OFFS_TYPE] = type;
   memcpy(&dn_ipmg_vars.outputBuf[DN_RESET_REQ_OFFS_MACADDRESS],macAddress,8);
   
   // send outputBuf
   rc = dn_serial_mg_sendRequest(
      CMDID_RESET,                                              // cmdId
      extraFlags,                                               // extraFlags
      dn_ipmg_vars.outputBuf,                                   // payload
      DN_RESET_REQ_LEN,                                         // length
      dn_ipmg_reset_reply                                       // replyCb
   );
   
   if (rc==DN_ERR_NONE) {
      // I'm now busy transmitting
      dn_ipmg_vars.busyTx         = TRUE;
   }
   
   // unlock the module
   dn_unlock();
   
   return rc;
   
}

void dn_ipmg_reset_reply(uint8_t cmdId, uint8_t rc, uint8_t* payload, uint8_t len) {
   dn_ipmg_reset_rpt* reply;
   
   // verify I'm expecting this answer
   if (dn_ipmg_vars.busyTx==FALSE || dn_ipmg_vars.cmdId!=cmdId) {
      return;
   }
   
   // verify length
   if (rc==DN_SERIAL_RC_OK && len<DN_RESET_REPLY_LEN) {
      return;
   }
   
   // cast the replyContent
   reply = (dn_ipmg_reset_rpt*)dn_ipmg_vars.replyContents;
   
   // store RC
   reply->RC = rc;
   
   // parse returned value (iff RC==0)
   if (rc==DN_SERIAL_RC_OK) {
      
      memcpy(&reply->macAddress[0],&payload[DN_RESET_REPLY_OFFS_MACADDRESS],8);
   }
   
   // call the callback
   dn_ipmg_vars.replyCb(cmdId);
   
   // I'm not busy transmitting anymore
   dn_ipmg_vars.busyTx=FALSE;
}

//===== subscribe

/**
The subscribe command indicates that the manager should send the external 
application the specified notifications. It contains two filter fields:

- filter is a bitmask of flags indicating the types of notifications that the 
client wants to receive
- unackFilter allows the client to select which of the notifications selected 
in filter should be sent acknowledged. If a notification is sent as 
'acknowledged', thesubsequent notification packets will be queued while waiting 
for response.

Each subscription request overwrites the previous one. If an application is 
subscribed to data and then decides he also wants events he should send a 
subscribe command with both the data and event flags set. To clear all 
subscriptions, the client should send a subscribe command with the filter set 
to zero. When a session is initiated between the manager and a client, the 
subscription filter is initialized to zero.

The subscribe bitmap uses the values of the notification type enumeration. Some 
values are unused to provide backwards compatibility with earlier APIs. 
*/
dn_err_t dn_ipmg_subscribe(uint32_t filter, uint32_t unackFilter, dn_ipmg_subscribe_rpt* reply) {
   uint8_t    extraFlags;
   dn_err_t   rc;
   
   // lock the module
   dn_lock();
   
   // verify no ongoing transmissions
   if (dn_ipmg_vars.busyTx) {
      // unlock the module
      dn_unlock();
      
      // return
      return DN_ERR_BUSY;
   }
   
   // store callback information
   dn_ipmg_vars.cmdId          = CMDID_SUBSCRIBE;
   dn_ipmg_vars.replyContents  = (uint8_t*)reply;
   
   // extraFlags
   extraFlags = 0x00;
   
   // build outputBuf
   dn_write_uint32_t(&dn_ipmg_vars.outputBuf[DN_SUBSCRIBE_REQ_OFFS_FILTER],filter);
   dn_write_uint32_t(&dn_ipmg_vars.outputBuf[DN_SUBSCRIBE_REQ_OFFS_UNACKFILTER],unackFilter);
   
   // send outputBuf
   rc = dn_serial_mg_sendRequest(
      CMDID_SUBSCRIBE,                                          // cmdId
      extraFlags,                                               // extraFlags
      dn_ipmg_vars.outputBuf,                                   // payload
      DN_SUBSCRIBE_REQ_LEN,                                     // length
      dn_ipmg_subscribe_reply                                   // replyCb
   );
   
   if (rc==DN_ERR_NONE) {
      // I'm now busy transmitting
      dn_ipmg_vars.busyTx         = TRUE;
   }
   
   // unlock the module
   dn_unlock();
   
   return rc;
   
}

void dn_ipmg_subscribe_reply(uint8_t cmdId, uint8_t rc, uint8_t* payload, uint8_t len) {
   dn_ipmg_subscribe_rpt* reply;
   
   // verify I'm expecting this answer
   if (dn_ipmg_vars.busyTx==FALSE || dn_ipmg_vars.cmdId!=cmdId) {
      return;
   }
   
   // do NOT verify length (no return fields expected)
   
   // cast the replyContent
   reply = (dn_ipmg_subscribe_rpt*)dn_ipmg_vars.replyContents;
   
   // store RC
   reply->RC = rc;
   
   // parse returned value (iff RC==0)
   if (rc==DN_SERIAL_RC_OK) {
      
   }
   
   // call the callback
   dn_ipmg_vars.replyCb(cmdId);
   
   // I'm not busy transmitting anymore
   dn_ipmg_vars.busyTx=FALSE;
}

//===== getTime

/**
The getTime command returns the current manager UTC time and current absolute 
slot number (ASN). The time values returned by this command are delayed by 
queuing and transfer time over the serial connection. For additional precision, 
an external application should trigger the networkTime notification using the 
Time Pin. 
*/
dn_err_t dn_ipmg_getTime(dn_ipmg_getTime_rpt* reply) {
   uint8_t    extraFlags;
   dn_err_t   rc;
   
   // lock the module
   dn_lock();
   
   // verify no ongoing transmissions
   if (dn_ipmg_vars.busyTx) {
      // unlock the module
      dn_unlock();
      
      // return
      return DN_ERR_BUSY;
   }
   
   // store callback information
   dn_ipmg_vars.cmdId          = CMDID_GETTIME;
   dn_ipmg_vars.replyContents  = (uint8_t*)reply;
   
   // extraFlags
   extraFlags = 0x00;
   
   // build outputBuf
   
   // send outputBuf
   rc = dn_serial_mg_sendRequest(
      CMDID_GETTIME,                                            // cmdId
      extraFlags,                                               // extraFlags
      dn_ipmg_vars.outputBuf,                                   // payload
      DN_GETTIME_REQ_LEN,                                       // length
      dn_ipmg_getTime_reply                                     // replyCb
   );
   
   if (rc==DN_ERR_NONE) {
      // I'm now busy transmitting
      dn_ipmg_vars.busyTx         = TRUE;
   }
   
   // unlock the module
   dn_unlock();
   
   return rc;
   
}

void dn_ipmg_getTime_reply(uint8_t cmdId, uint8_t rc, uint8_t* payload, uint8_t len) {
   dn_ipmg_getTime_rpt* reply;
   
   // verify I'm expecting this answer
   if (dn_ipmg_vars.busyTx==FALSE || dn_ipmg_vars.cmdId!=cmdId) {
      return;
   }
   
   // verify length
   if (rc==DN_SERIAL_RC_OK && len<DN_GETTIME_REPLY_LEN) {
      return;
   }
   
   // cast the replyContent
   reply = (dn_ipmg_getTime_rpt*)dn_ipmg_vars.replyContents;
   
   // store RC
   reply->RC = rc;
   
   // parse returned value (iff RC==0)
   if (rc==DN_SERIAL_RC_OK) {
      
      dn_read_uint32_t(&reply->uptime,&payload[DN_GETTIME_REPLY_OFFS_UPTIME]);
      memcpy(&reply->utcSecs[0],&payload[DN_GETTIME_REPLY_OFFS_UTCSECS],8);
      dn_read_uint32_t(&reply->utcUsecs,&payload[DN_GETTIME_REPLY_OFFS_UTCUSECS]);
      memcpy(&reply->asn[0],&payload[DN_GETTIME_REPLY_OFFS_ASN],5);
      dn_read_uint16_t(&reply->asnOffset,&payload[DN_GETTIME_REPLY_OFFS_ASNOFFSET]);
   }
   
   // call the callback
   dn_ipmg_vars.replyCb(cmdId);
   
   // I'm not busy transmitting anymore
   dn_ipmg_vars.busyTx=FALSE;
}

//===== setNetworkConfig

/**
The setNetworkConfig command changes network configuration parameters. The 
response code indicates whether the changes were successfully applied. This 
change is persistent.

Generally, changes to network configuration will take effect when the manager 
reboots. Exceptions are detailed below:

- Max Motes: The new maxMotes value is used as soon as new motes try to join 
the network, but motes are not removed from the network if the value is set to 
a number lower than numMotes.
- Base Bandwidth: Changing baseBandwidth while the network is running does not 
reallocate bandwidth to Operational motes. 
*/
dn_err_t dn_ipmg_setNetworkConfig(uint16_t networkId, int8_t apTxPower, uint8_t frameProfile, uint16_t maxMotes, uint16_t baseBandwidth, uint8_t downFrameMultVal, uint8_t numParents, uint8_t ccaMode, uint16_t channelList, bool autoStartNetwork, uint8_t locMode, uint8_t bbMode, uint8_t bbSize, uint8_t isRadioTest, uint16_t bwMult, uint8_t oneChannel, dn_ipmg_setNetworkConfig_rpt* reply) {
   uint8_t    extraFlags;
   dn_err_t   rc;
   
   // lock the module
   dn_lock();
   
   // verify no ongoing transmissions
   if (dn_ipmg_vars.busyTx) {
      // unlock the module
      dn_unlock();
      
      // return
      return DN_ERR_BUSY;
   }
   
   // store callback information
   dn_ipmg_vars.cmdId          = CMDID_SETNETWORKCONFIG;
   dn_ipmg_vars.replyContents  = (uint8_t*)reply;
   
   // extraFlags
   extraFlags = 0x00;
   
   // build outputBuf
   dn_write_uint16_t(&dn_ipmg_vars.outputBuf[DN_SETNETWORKCONFIG_REQ_OFFS_NETWORKID],networkId);
   dn_ipmg_vars.outputBuf[DN_SETNETWORKCONFIG_REQ_OFFS_APTXPOWER] = (int8_t)apTxPower;
   dn_ipmg_vars.outputBuf[DN_SETNETWORKCONFIG_REQ_OFFS_FRAMEPROFILE] = frameProfile;
   dn_write_uint16_t(&dn_ipmg_vars.outputBuf[DN_SETNETWORKCONFIG_REQ_OFFS_MAXMOTES],maxMotes);
   dn_write_uint16_t(&dn_ipmg_vars.outputBuf[DN_SETNETWORKCONFIG_REQ_OFFS_BASEBANDWIDTH],baseBandwidth);
   dn_ipmg_vars.outputBuf[DN_SETNETWORKCONFIG_REQ_OFFS_DOWNFRAMEMULTVAL] = downFrameMultVal;
   dn_ipmg_vars.outputBuf[DN_SETNETWORKCONFIG_REQ_OFFS_NUMPARENTS] = numParents;
   dn_ipmg_vars.outputBuf[DN_SETNETWORKCONFIG_REQ_OFFS_CCAMODE] = ccaMode;
   dn_write_uint16_t(&dn_ipmg_vars.outputBuf[DN_SETNETWORKCONFIG_REQ_OFFS_CHANNELLIST],channelList);
   dn_ipmg_vars.outputBuf[DN_SETNETWORKCONFIG_REQ_OFFS_AUTOSTARTNETWORK] = autoStartNetwork;
   dn_ipmg_vars.outputBuf[DN_SETNETWORKCONFIG_REQ_OFFS_LOCMODE] = locMode;
   dn_ipmg_vars.outputBuf[DN_SETNETWORKCONFIG_REQ_OFFS_BBMODE] = bbMode;
   dn_ipmg_vars.outputBuf[DN_SETNETWORKCONFIG_REQ_OFFS_BBSIZE] = bbSize;
   dn_ipmg_vars.outputBuf[DN_SETNETWORKCONFIG_REQ_OFFS_ISRADIOTEST] = isRadioTest;
   dn_write_uint16_t(&dn_ipmg_vars.outputBuf[DN_SETNETWORKCONFIG_REQ_OFFS_BWMULT],bwMult);
   dn_ipmg_vars.outputBuf[DN_SETNETWORKCONFIG_REQ_OFFS_ONECHANNEL] = oneChannel;
   
   // send outputBuf
   rc = dn_serial_mg_sendRequest(
      CMDID_SETNETWORKCONFIG,                                   // cmdId
      extraFlags,                                               // extraFlags
      dn_ipmg_vars.outputBuf,                                   // payload
      DN_SETNETWORKCONFIG_REQ_LEN,                              // length
      dn_ipmg_setNetworkConfig_reply                            // replyCb
   );
   
   if (rc==DN_ERR_NONE) {
      // I'm now busy transmitting
      dn_ipmg_vars.busyTx         = TRUE;
   }
   
   // unlock the module
   dn_unlock();
   
   return rc;
   
}

void dn_ipmg_setNetworkConfig_reply(uint8_t cmdId, uint8_t rc, uint8_t* payload, uint8_t len) {
   dn_ipmg_setNetworkConfig_rpt* reply;
   
   // verify I'm expecting this answer
   if (dn_ipmg_vars.busyTx==FALSE || dn_ipmg_vars.cmdId!=cmdId) {
      return;
   }
   
   // do NOT verify length (no return fields expected)
   
   // cast the replyContent
   reply = (dn_ipmg_setNetworkConfig_rpt*)dn_ipmg_vars.replyContents;
   
   // store RC
   reply->RC = rc;
   
   // parse returned value (iff RC==0)
   if (rc==DN_SERIAL_RC_OK) {
      
   }
   
   // call the callback
   dn_ipmg_vars.replyCb(cmdId);
   
   // I'm not busy transmitting anymore
   dn_ipmg_vars.busyTx=FALSE;
}

//===== clearStatistics

/**
The clearStatistics command clears the accumulated network statistics. The 
command does not clear path quality or mote statistics. 
*/
dn_err_t dn_ipmg_clearStatistics(dn_ipmg_clearStatistics_rpt* reply) {
   uint8_t    extraFlags;
   dn_err_t   rc;
   
   // lock the module
   dn_lock();
   
   // verify no ongoing transmissions
   if (dn_ipmg_vars.busyTx) {
      // unlock the module
      dn_unlock();
      
      // return
      return DN_ERR_BUSY;
   }
   
   // store callback information
   dn_ipmg_vars.cmdId          = CMDID_CLEARSTATISTICS;
   dn_ipmg_vars.replyContents  = (uint8_t*)reply;
   
   // extraFlags
   extraFlags = 0x00;
   
   // build outputBuf
   
   // send outputBuf
   rc = dn_serial_mg_sendRequest(
      CMDID_CLEARSTATISTICS,                                    // cmdId
      extraFlags,                                               // extraFlags
      dn_ipmg_vars.outputBuf,                                   // payload
      DN_CLEARSTATISTICS_REQ_LEN,                               // length
      dn_ipmg_clearStatistics_reply                             // replyCb
   );
   
   if (rc==DN_ERR_NONE) {
      // I'm now busy transmitting
      dn_ipmg_vars.busyTx         = TRUE;
   }
   
   // unlock the module
   dn_unlock();
   
   return rc;
   
}

void dn_ipmg_clearStatistics_reply(uint8_t cmdId, uint8_t rc, uint8_t* payload, uint8_t len) {
   dn_ipmg_clearStatistics_rpt* reply;
   
   // verify I'm expecting this answer
   if (dn_ipmg_vars.busyTx==FALSE || dn_ipmg_vars.cmdId!=cmdId) {
      return;
   }
   
   // do NOT verify length (no return fields expected)
   
   // cast the replyContent
   reply = (dn_ipmg_clearStatistics_rpt*)dn_ipmg_vars.replyContents;
   
   // store RC
   reply->RC = rc;
   
   // parse returned value (iff RC==0)
   if (rc==DN_SERIAL_RC_OK) {
      
   }
   
   // call the callback
   dn_ipmg_vars.replyCb(cmdId);
   
   // I'm not busy transmitting anymore
   dn_ipmg_vars.busyTx=FALSE;
}

//===== exchangeMoteJoinKey

/**
The exchangeMoteJoinKey command triggers the manager to send a new join key to 
the specified mote and update the manager's ACL entry for the mote. The 
response contains a callbackId. A commandFinished event notification with this 
callbackId will be sent when the operation is complete. This change is 
persistent. 
*/
dn_err_t dn_ipmg_exchangeMoteJoinKey(uint8_t* macAddress, uint8_t* key, dn_ipmg_exchangeMoteJoinKey_rpt* reply) {
   uint8_t    extraFlags;
   dn_err_t   rc;
   
   // lock the module
   dn_lock();
   
   // verify no ongoing transmissions
   if (dn_ipmg_vars.busyTx) {
      // unlock the module
      dn_unlock();
      
      // return
      return DN_ERR_BUSY;
   }
   
   // store callback information
   dn_ipmg_vars.cmdId          = CMDID_EXCHANGEMOTEJOINKEY;
   dn_ipmg_vars.replyContents  = (uint8_t*)reply;
   
   // extraFlags
   extraFlags = 0x00;
   
   // build outputBuf
   memcpy(&dn_ipmg_vars.outputBuf[DN_EXCHANGEMOTEJOINKEY_REQ_OFFS_MACADDRESS],macAddress,8);
   memcpy(&dn_ipmg_vars.outputBuf[DN_EXCHANGEMOTEJOINKEY_REQ_OFFS_KEY],key,16);
   
   // send outputBuf
   rc = dn_serial_mg_sendRequest(
      CMDID_EXCHANGEMOTEJOINKEY,                                // cmdId
      extraFlags,                                               // extraFlags
      dn_ipmg_vars.outputBuf,                                   // payload
      DN_EXCHANGEMOTEJOINKEY_REQ_LEN,                           // length
      dn_ipmg_exchangeMoteJoinKey_reply                         // replyCb
   );
   
   if (rc==DN_ERR_NONE) {
      // I'm now busy transmitting
      dn_ipmg_vars.busyTx         = TRUE;
   }
   
   // unlock the module
   dn_unlock();
   
   return rc;
   
}

void dn_ipmg_exchangeMoteJoinKey_reply(uint8_t cmdId, uint8_t rc, uint8_t* payload, uint8_t len) {
   dn_ipmg_exchangeMoteJoinKey_rpt* reply;
   
   // verify I'm expecting this answer
   if (dn_ipmg_vars.busyTx==FALSE || dn_ipmg_vars.cmdId!=cmdId) {
      return;
   }
   
   // verify length
   if (rc==DN_SERIAL_RC_OK && len<DN_EXCHANGEMOTEJOINKEY_REPLY_LEN) {
      return;
   }
   
   // cast the replyContent
   reply = (dn_ipmg_exchangeMoteJoinKey_rpt*)dn_ipmg_vars.replyContents;
   
   // store RC
   reply->RC = rc;
   
   // parse returned value (iff RC==0)
   if (rc==DN_SERIAL_RC_OK) {
      
      dn_read_uint32_t(&reply->callbackId,&payload[DN_EXCHANGEMOTEJOINKEY_REPLY_OFFS_CALLBACKID]);
   }
   
   // call the callback
   dn_ipmg_vars.replyCb(cmdId);
   
   // I'm not busy transmitting anymore
   dn_ipmg_vars.busyTx=FALSE;
}

//===== exchangeNetworkId

/**
The exchangeNetworkId command triggers the manager to distribute a new network 
ID to all the motes in the network. A callbackId is returned in the response. A 
commandFinished notification with this callbackId will be sent when the 
operation is complete.This change is persistent. 
*/
dn_err_t dn_ipmg_exchangeNetworkId(uint16_t id, dn_ipmg_exchangeNetworkId_rpt* reply) {
   uint8_t    extraFlags;
   dn_err_t   rc;
   
   // lock the module
   dn_lock();
   
   // verify no ongoing transmissions
   if (dn_ipmg_vars.busyTx) {
      // unlock the module
      dn_unlock();
      
      // return
      return DN_ERR_BUSY;
   }
   
   // store callback information
   dn_ipmg_vars.cmdId          = CMDID_EXCHANGENETWORKID;
   dn_ipmg_vars.replyContents  = (uint8_t*)reply;
   
   // extraFlags
   extraFlags = 0x00;
   
   // build outputBuf
   dn_write_uint16_t(&dn_ipmg_vars.outputBuf[DN_EXCHANGENETWORKID_REQ_OFFS_ID],id);
   
   // send outputBuf
   rc = dn_serial_mg_sendRequest(
      CMDID_EXCHANGENETWORKID,                                  // cmdId
      extraFlags,                                               // extraFlags
      dn_ipmg_vars.outputBuf,                                   // payload
      DN_EXCHANGENETWORKID_REQ_LEN,                             // length
      dn_ipmg_exchangeNetworkId_reply                           // replyCb
   );
   
   if (rc==DN_ERR_NONE) {
      // I'm now busy transmitting
      dn_ipmg_vars.busyTx         = TRUE;
   }
   
   // unlock the module
   dn_unlock();
   
   return rc;
   
}

void dn_ipmg_exchangeNetworkId_reply(uint8_t cmdId, uint8_t rc, uint8_t* payload, uint8_t len) {
   dn_ipmg_exchangeNetworkId_rpt* reply;
   
   // verify I'm expecting this answer
   if (dn_ipmg_vars.busyTx==FALSE || dn_ipmg_vars.cmdId!=cmdId) {
      return;
   }
   
   // verify length
   if (rc==DN_SERIAL_RC_OK && len<DN_EXCHANGENETWORKID_REPLY_LEN) {
      return;
   }
   
   // cast the replyContent
   reply = (dn_ipmg_exchangeNetworkId_rpt*)dn_ipmg_vars.replyContents;
   
   // store RC
   reply->RC = rc;
   
   // parse returned value (iff RC==0)
   if (rc==DN_SERIAL_RC_OK) {
      
      dn_read_uint32_t(&reply->callbackId,&payload[DN_EXCHANGENETWORKID_REPLY_OFFS_CALLBACKID]);
   }
   
   // call the callback
   dn_ipmg_vars.replyCb(cmdId);
   
   // I'm not busy transmitting anymore
   dn_ipmg_vars.busyTx=FALSE;
}

//===== radiotestTx

/**
The radiotestTx command allows the user to initiate a radio transmission test. 
It may only be executed if the manager has been booted up in radiotest mode 
(see setNetworkConfig command). Three types of transmission tests are supported:

- Packet transmission
- Continuous modulation (CM)
- Continuous wave, i.e unmodulated signal (CW)

In a packet transmission test, the device generates a repeatCnt number of 
packet sequences. Each sequence consists of up to 10 packets with configurable 
size and delays. Each packet starts with a PHY preamble (5 bytes), followed by 
a PHY length field (1 byte), followed by data payload of up to 125 bytes, and 
finally a 2-byte 802.15.4 CRC at the end. Byte 0 of the payload contains 
stationId of the sender. Bytes 1 and 2 contain the packet number (in big-endian 
format) that increments with every packet transmitted. Bytes 3..N contain a 
counter (from 0..N-2) that increments with every byte inside payload. 
Transmissions occur on the set of channels defined by chanMask , selected inpseudo-randomorder.

In a continuous modulation test, the device generates continuous pseudo-random 
modulated signal, centered at the specified channel. The test is stopped by 
resetting the device.

In a continuous wave test, the device generates an unmodulated tone, centered 
at the specified channel. The test tone is stopped by resetting the device.



Channel numbering is 0-15, corresponding to IEEE 2.4 GHz channels 11-26.

The station ID is a user selectable value. It is used in packet tests so that a 
receiver (see radiotestRx) can identify packets from this device in cases where 
there may be multiple tests running in the same radio space. This field is not 
used for CM or CW tests. 
*/
dn_err_t dn_ipmg_radiotestTx(uint8_t testType, uint16_t chanMask, uint16_t repeatCnt, int8_t txPower, uint8_t seqSize, uint8_t pkLen_1, uint16_t delay_1, uint8_t pkLen_2, uint16_t delay_2, uint8_t pkLen_3, uint16_t delay_3, uint8_t pkLen_4, uint16_t delay_4, uint8_t pkLen_5, uint16_t delay_5, uint8_t pkLen_6, uint16_t delay_6, uint8_t pkLen_7, uint16_t delay_7, uint8_t pkLen_8, uint16_t delay_8, uint8_t pkLen_9, uint16_t delay_9, uint8_t pkLen_10, uint16_t delay_10, uint8_t stationId, dn_ipmg_radiotestTx_rpt* reply) {
   uint8_t    extraFlags;
   dn_err_t   rc;
   
   // lock the module
   dn_lock();
   
   // verify no ongoing transmissions
   if (dn_ipmg_vars.busyTx) {
      // unlock the module
      dn_unlock();
      
      // return
      return DN_ERR_BUSY;
   }
   
   // store callback information
   dn_ipmg_vars.cmdId          = CMDID_RADIOTESTTX;
   dn_ipmg_vars.replyContents  = (uint8_t*)reply;
   
   // extraFlags
   extraFlags = 0x00;
   
   // build outputBuf
   dn_ipmg_vars.outputBuf[DN_RADIOTESTTX_REQ_OFFS_TESTTYPE] = testType;
   dn_write_uint16_t(&dn_ipmg_vars.outputBuf[DN_RADIOTESTTX_REQ_OFFS_CHANMASK],chanMask);
   dn_write_uint16_t(&dn_ipmg_vars.outputBuf[DN_RADIOTESTTX_REQ_OFFS_REPEATCNT],repeatCnt);
   dn_ipmg_vars.outputBuf[DN_RADIOTESTTX_REQ_OFFS_TXPOWER] = (int8_t)txPower;
   dn_ipmg_vars.outputBuf[DN_RADIOTESTTX_REQ_OFFS_SEQSIZE] = seqSize;
   dn_ipmg_vars.outputBuf[DN_RADIOTESTTX_REQ_OFFS_PKLEN_1] = pkLen_1;
   dn_write_uint16_t(&dn_ipmg_vars.outputBuf[DN_RADIOTESTTX_REQ_OFFS_DELAY_1],delay_1);
   dn_ipmg_vars.outputBuf[DN_RADIOTESTTX_REQ_OFFS_PKLEN_2] = pkLen_2;
   dn_write_uint16_t(&dn_ipmg_vars.outputBuf[DN_RADIOTESTTX_REQ_OFFS_DELAY_2],delay_2);
   dn_ipmg_vars.outputBuf[DN_RADIOTESTTX_REQ_OFFS_PKLEN_3] = pkLen_3;
   dn_write_uint16_t(&dn_ipmg_vars.outputBuf[DN_RADIOTESTTX_REQ_OFFS_DELAY_3],delay_3);
   dn_ipmg_vars.outputBuf[DN_RADIOTESTTX_REQ_OFFS_PKLEN_4] = pkLen_4;
   dn_write_uint16_t(&dn_ipmg_vars.outputBuf[DN_RADIOTESTTX_REQ_OFFS_DELAY_4],delay_4);
   dn_ipmg_vars.outputBuf[DN_RADIOTESTTX_REQ_OFFS_PKLEN_5] = pkLen_5;
   dn_write_uint16_t(&dn_ipmg_vars.outputBuf[DN_RADIOTESTTX_REQ_OFFS_DELAY_5],delay_5);
   dn_ipmg_vars.outputBuf[DN_RADIOTESTTX_REQ_OFFS_PKLEN_6] = pkLen_6;
   dn_write_uint16_t(&dn_ipmg_vars.outputBuf[DN_RADIOTESTTX_REQ_OFFS_DELAY_6],delay_6);
   dn_ipmg_vars.outputBuf[DN_RADIOTESTTX_REQ_OFFS_PKLEN_7] = pkLen_7;
   dn_write_uint16_t(&dn_ipmg_vars.outputBuf[DN_RADIOTESTTX_REQ_OFFS_DELAY_7],delay_7);
   dn_ipmg_vars.outputBuf[DN_RADIOTESTTX_REQ_OFFS_PKLEN_8] = pkLen_8;
   dn_write_uint16_t(&dn_ipmg_vars.outputBuf[DN_RADIOTESTTX_REQ_OFFS_DELAY_8],delay_8);
   dn_ipmg_vars.outputBuf[DN_RADIOTESTTX_REQ_OFFS_PKLEN_9] = pkLen_9;
   dn_write_uint16_t(&dn_ipmg_vars.outputBuf[DN_RADIOTESTTX_REQ_OFFS_DELAY_9],delay_9);
   dn_ipmg_vars.outputBuf[DN_RADIOTESTTX_REQ_OFFS_PKLEN_10] = pkLen_10;
   dn_write_uint16_t(&dn_ipmg_vars.outputBuf[DN_RADIOTESTTX_REQ_OFFS_DELAY_10],delay_10);
   dn_ipmg_vars.outputBuf[DN_RADIOTESTTX_REQ_OFFS_STATIONID] = stationId;
   
   // send outputBuf
   rc = dn_serial_mg_sendRequest(
      CMDID_RADIOTESTTX,                                        // cmdId
      extraFlags,                                               // extraFlags
      dn_ipmg_vars.outputBuf,                                   // payload
      DN_RADIOTESTTX_REQ_LEN,                                   // length
      dn_ipmg_radiotestTx_reply                                 // replyCb
   );
   
   if (rc==DN_ERR_NONE) {
      // I'm now busy transmitting
      dn_ipmg_vars.busyTx         = TRUE;
   }
   
   // unlock the module
   dn_unlock();
   
   return rc;
   
}

void dn_ipmg_radiotestTx_reply(uint8_t cmdId, uint8_t rc, uint8_t* payload, uint8_t len) {
   dn_ipmg_radiotestTx_rpt* reply;
   
   // verify I'm expecting this answer
   if (dn_ipmg_vars.busyTx==FALSE || dn_ipmg_vars.cmdId!=cmdId) {
      return;
   }
   
   // do NOT verify length (no return fields expected)
   
   // cast the replyContent
   reply = (dn_ipmg_radiotestTx_rpt*)dn_ipmg_vars.replyContents;
   
   // store RC
   reply->RC = rc;
   
   // parse returned value (iff RC==0)
   if (rc==DN_SERIAL_RC_OK) {
      
   }
   
   // call the callback
   dn_ipmg_vars.replyCb(cmdId);
   
   // I'm not busy transmitting anymore
   dn_ipmg_vars.busyTx=FALSE;
}

//===== radiotestRx

/**
The radiotestRx command clears all previously collected statistics and 
initiates radio reception on the specified channel. It may only be executed if 
the manager has been booted up in radiotest mode (see setNetworkConfig 
command). During the test, the device keeps statistics about the number of 
packets received (with and without error). The test results may be retrieved 
using the getRadiotestStatistics command.

The station ID is a user selectable value. It must be set to match the station 
ID used by the transmitter. Station ID is used to isolate traffic if multiple 
tests are running in the same radio space.



Channel numbering is 0-15, corresponding to IEEE 2.4 GHz channels 11-26. 
*/
dn_err_t dn_ipmg_radiotestRx(uint16_t mask, uint16_t duration, uint8_t stationId, dn_ipmg_radiotestRx_rpt* reply) {
   uint8_t    extraFlags;
   dn_err_t   rc;
   
   // lock the module
   dn_lock();
   
   // verify no ongoing transmissions
   if (dn_ipmg_vars.busyTx) {
      // unlock the module
      dn_unlock();
      
      // return
      return DN_ERR_BUSY;
   }
   
   // store callback information
   dn_ipmg_vars.cmdId          = CMDID_RADIOTESTRX;
   dn_ipmg_vars.replyContents  = (uint8_t*)reply;
   
   // extraFlags
   extraFlags = 0x00;
   
   // build outputBuf
   dn_write_uint16_t(&dn_ipmg_vars.outputBuf[DN_RADIOTESTRX_REQ_OFFS_MASK],mask);
   dn_write_uint16_t(&dn_ipmg_vars.outputBuf[DN_RADIOTESTRX_REQ_OFFS_DURATION],duration);
   dn_ipmg_vars.outputBuf[DN_RADIOTESTRX_REQ_OFFS_STATIONID] = stationId;
   
   // send outputBuf
   rc = dn_serial_mg_sendRequest(
      CMDID_RADIOTESTRX,                                        // cmdId
      extraFlags,                                               // extraFlags
      dn_ipmg_vars.outputBuf,                                   // payload
      DN_RADIOTESTRX_REQ_LEN,                                   // length
      dn_ipmg_radiotestRx_reply                                 // replyCb
   );
   
   if (rc==DN_ERR_NONE) {
      // I'm now busy transmitting
      dn_ipmg_vars.busyTx         = TRUE;
   }
   
   // unlock the module
   dn_unlock();
   
   return rc;
   
}

void dn_ipmg_radiotestRx_reply(uint8_t cmdId, uint8_t rc, uint8_t* payload, uint8_t len) {
   dn_ipmg_radiotestRx_rpt* reply;
   
   // verify I'm expecting this answer
   if (dn_ipmg_vars.busyTx==FALSE || dn_ipmg_vars.cmdId!=cmdId) {
      return;
   }
   
   // do NOT verify length (no return fields expected)
   
   // cast the replyContent
   reply = (dn_ipmg_radiotestRx_rpt*)dn_ipmg_vars.replyContents;
   
   // store RC
   reply->RC = rc;
   
   // parse returned value (iff RC==0)
   if (rc==DN_SERIAL_RC_OK) {
      
   }
   
   // call the callback
   dn_ipmg_vars.replyCb(cmdId);
   
   // I'm not busy transmitting anymore
   dn_ipmg_vars.busyTx=FALSE;
}

//===== getRadiotestStatistics

/**
This command retrieves statistics from a previously run radiotestRx command.It 
may only be executed if the manager has been booted up in radiotest mode (see 
setNetworkConfig command). 
*/
dn_err_t dn_ipmg_getRadiotestStatistics(dn_ipmg_getRadiotestStatistics_rpt* reply) {
   uint8_t    extraFlags;
   dn_err_t   rc;
   
   // lock the module
   dn_lock();
   
   // verify no ongoing transmissions
   if (dn_ipmg_vars.busyTx) {
      // unlock the module
      dn_unlock();
      
      // return
      return DN_ERR_BUSY;
   }
   
   // store callback information
   dn_ipmg_vars.cmdId          = CMDID_GETRADIOTESTSTATISTICS;
   dn_ipmg_vars.replyContents  = (uint8_t*)reply;
   
   // extraFlags
   extraFlags = 0x00;
   
   // build outputBuf
   
   // send outputBuf
   rc = dn_serial_mg_sendRequest(
      CMDID_GETRADIOTESTSTATISTICS,                             // cmdId
      extraFlags,                                               // extraFlags
      dn_ipmg_vars.outputBuf,                                   // payload
      DN_GETRADIOTESTSTATISTICS_REQ_LEN,                        // length
      dn_ipmg_getRadiotestStatistics_reply                      // replyCb
   );
   
   if (rc==DN_ERR_NONE) {
      // I'm now busy transmitting
      dn_ipmg_vars.busyTx         = TRUE;
   }
   
   // unlock the module
   dn_unlock();
   
   return rc;
   
}

void dn_ipmg_getRadiotestStatistics_reply(uint8_t cmdId, uint8_t rc, uint8_t* payload, uint8_t len) {
   dn_ipmg_getRadiotestStatistics_rpt* reply;
   
   // verify I'm expecting this answer
   if (dn_ipmg_vars.busyTx==FALSE || dn_ipmg_vars.cmdId!=cmdId) {
      return;
   }
   
   // verify length
   if (rc==DN_SERIAL_RC_OK && len<DN_GETRADIOTESTSTATISTICS_REPLY_LEN) {
      return;
   }
   
   // cast the replyContent
   reply = (dn_ipmg_getRadiotestStatistics_rpt*)dn_ipmg_vars.replyContents;
   
   // store RC
   reply->RC = rc;
   
   // parse returned value (iff RC==0)
   if (rc==DN_SERIAL_RC_OK) {
      
      dn_read_uint16_t(&reply->rxOk,&payload[DN_GETRADIOTESTSTATISTICS_REPLY_OFFS_RXOK]);
      dn_read_uint16_t(&reply->rxFail,&payload[DN_GETRADIOTESTSTATISTICS_REPLY_OFFS_RXFAIL]);
   }
   
   // call the callback
   dn_ipmg_vars.replyCb(cmdId);
   
   // I'm not busy transmitting anymore
   dn_ipmg_vars.busyTx=FALSE;
}

//===== setACLEntry

/**
The setACLEntry command adds a new entry or updates an existing entry in the 
Access Control List (ACL).This change is persistent. 
*/
dn_err_t dn_ipmg_setACLEntry(uint8_t* macAddress, uint8_t* joinKey, dn_ipmg_setACLEntry_rpt* reply) {
   uint8_t    extraFlags;
   dn_err_t   rc;
   
   // lock the module
   dn_lock();
   
   // verify no ongoing transmissions
   if (dn_ipmg_vars.busyTx) {
      // unlock the module
      dn_unlock();
      
      // return
      return DN_ERR_BUSY;
   }
   
   // store callback information
   dn_ipmg_vars.cmdId          = CMDID_SETACLENTRY;
   dn_ipmg_vars.replyContents  = (uint8_t*)reply;
   
   // extraFlags
   extraFlags = 0x00;
   
   // build outputBuf
   memcpy(&dn_ipmg_vars.outputBuf[DN_SETACLENTRY_REQ_OFFS_MACADDRESS],macAddress,8);
   memcpy(&dn_ipmg_vars.outputBuf[DN_SETACLENTRY_REQ_OFFS_JOINKEY],joinKey,16);
   
   // send outputBuf
   rc = dn_serial_mg_sendRequest(
      CMDID_SETACLENTRY,                                        // cmdId
      extraFlags,                                               // extraFlags
      dn_ipmg_vars.outputBuf,                                   // payload
      DN_SETACLENTRY_REQ_LEN,                                   // length
      dn_ipmg_setACLEntry_reply                                 // replyCb
   );
   
   if (rc==DN_ERR_NONE) {
      // I'm now busy transmitting
      dn_ipmg_vars.busyTx         = TRUE;
   }
   
   // unlock the module
   dn_unlock();
   
   return rc;
   
}

void dn_ipmg_setACLEntry_reply(uint8_t cmdId, uint8_t rc, uint8_t* payload, uint8_t len) {
   dn_ipmg_setACLEntry_rpt* reply;
   
   // verify I'm expecting this answer
   if (dn_ipmg_vars.busyTx==FALSE || dn_ipmg_vars.cmdId!=cmdId) {
      return;
   }
   
   // do NOT verify length (no return fields expected)
   
   // cast the replyContent
   reply = (dn_ipmg_setACLEntry_rpt*)dn_ipmg_vars.replyContents;
   
   // store RC
   reply->RC = rc;
   
   // parse returned value (iff RC==0)
   if (rc==DN_SERIAL_RC_OK) {
      
   }
   
   // call the callback
   dn_ipmg_vars.replyCb(cmdId);
   
   // I'm not busy transmitting anymore
   dn_ipmg_vars.busyTx=FALSE;
}

//===== getNextACLEntry

/**
The getNextACLEntry command returns information about next mote entry in the 
access control list (ACL). To begin a search (find the first mote in ACL), a 
zero MAC address (0000000000000000) should be sent. There is no mechanism for 
reading the ACL entry of a specific mote. This call is an iterator. If you call 
getNextACLEntry with mote A as the argument, your response is the ACL entry for 
mote B, where B is the next mote in the ACL. 
*/
dn_err_t dn_ipmg_getNextACLEntry(uint8_t* macAddress, dn_ipmg_getNextACLEntry_rpt* reply) {
   uint8_t    extraFlags;
   dn_err_t   rc;
   
   // lock the module
   dn_lock();
   
   // verify no ongoing transmissions
   if (dn_ipmg_vars.busyTx) {
      // unlock the module
      dn_unlock();
      
      // return
      return DN_ERR_BUSY;
   }
   
   // store callback information
   dn_ipmg_vars.cmdId          = CMDID_GETNEXTACLENTRY;
   dn_ipmg_vars.replyContents  = (uint8_t*)reply;
   
   // extraFlags
   extraFlags = 0x00;
   
   // build outputBuf
   memcpy(&dn_ipmg_vars.outputBuf[DN_GETNEXTACLENTRY_REQ_OFFS_MACADDRESS],macAddress,8);
   
   // send outputBuf
   rc = dn_serial_mg_sendRequest(
      CMDID_GETNEXTACLENTRY,                                    // cmdId
      extraFlags,                                               // extraFlags
      dn_ipmg_vars.outputBuf,                                   // payload
      DN_GETNEXTACLENTRY_REQ_LEN,                               // length
      dn_ipmg_getNextACLEntry_reply                             // replyCb
   );
   
   if (rc==DN_ERR_NONE) {
      // I'm now busy transmitting
      dn_ipmg_vars.busyTx         = TRUE;
   }
   
   // unlock the module
   dn_unlock();
   
   return rc;
   
}

void dn_ipmg_getNextACLEntry_reply(uint8_t cmdId, uint8_t rc, uint8_t* payload, uint8_t len) {
   dn_ipmg_getNextACLEntry_rpt* reply;
   
   // verify I'm expecting this answer
   if (dn_ipmg_vars.busyTx==FALSE || dn_ipmg_vars.cmdId!=cmdId) {
      return;
   }
   
   // verify length
   if (rc==DN_SERIAL_RC_OK && len<DN_GETNEXTACLENTRY_REPLY_LEN) {
      return;
   }
   
   // cast the replyContent
   reply = (dn_ipmg_getNextACLEntry_rpt*)dn_ipmg_vars.replyContents;
   
   // store RC
   reply->RC = rc;
   
   // parse returned value (iff RC==0)
   if (rc==DN_SERIAL_RC_OK) {
      
      memcpy(&reply->macAddress[0],&payload[DN_GETNEXTACLENTRY_REPLY_OFFS_MACADDRESS],8);
      memcpy(&reply->joinKey[0],&payload[DN_GETNEXTACLENTRY_REPLY_OFFS_JOINKEY],16);
   }
   
   // call the callback
   dn_ipmg_vars.replyCb(cmdId);
   
   // I'm not busy transmitting anymore
   dn_ipmg_vars.busyTx=FALSE;
}

//===== deleteACLEntry

/**
The deleteACLEntry command deletes the specified mote from the access control 
list (ACL). If the macAddress parameter is set to all 0xFFs or all 0x00s, the 
entire ACL is cleared. This change is persistent. 
*/
dn_err_t dn_ipmg_deleteACLEntry(uint8_t* macAddress, dn_ipmg_deleteACLEntry_rpt* reply) {
   uint8_t    extraFlags;
   dn_err_t   rc;
   
   // lock the module
   dn_lock();
   
   // verify no ongoing transmissions
   if (dn_ipmg_vars.busyTx) {
      // unlock the module
      dn_unlock();
      
      // return
      return DN_ERR_BUSY;
   }
   
   // store callback information
   dn_ipmg_vars.cmdId          = CMDID_DELETEACLENTRY;
   dn_ipmg_vars.replyContents  = (uint8_t*)reply;
   
   // extraFlags
   extraFlags = 0x00;
   
   // build outputBuf
   memcpy(&dn_ipmg_vars.outputBuf[DN_DELETEACLENTRY_REQ_OFFS_MACADDRESS],macAddress,8);
   
   // send outputBuf
   rc = dn_serial_mg_sendRequest(
      CMDID_DELETEACLENTRY,                                     // cmdId
      extraFlags,                                               // extraFlags
      dn_ipmg_vars.outputBuf,                                   // payload
      DN_DELETEACLENTRY_REQ_LEN,                                // length
      dn_ipmg_deleteACLEntry_reply                              // replyCb
   );
   
   if (rc==DN_ERR_NONE) {
      // I'm now busy transmitting
      dn_ipmg_vars.busyTx         = TRUE;
   }
   
   // unlock the module
   dn_unlock();
   
   return rc;
   
}

void dn_ipmg_deleteACLEntry_reply(uint8_t cmdId, uint8_t rc, uint8_t* payload, uint8_t len) {
   dn_ipmg_deleteACLEntry_rpt* reply;
   
   // verify I'm expecting this answer
   if (dn_ipmg_vars.busyTx==FALSE || dn_ipmg_vars.cmdId!=cmdId) {
      return;
   }
   
   // do NOT verify length (no return fields expected)
   
   // cast the replyContent
   reply = (dn_ipmg_deleteACLEntry_rpt*)dn_ipmg_vars.replyContents;
   
   // store RC
   reply->RC = rc;
   
   // parse returned value (iff RC==0)
   if (rc==DN_SERIAL_RC_OK) {
      
   }
   
   // call the callback
   dn_ipmg_vars.replyCb(cmdId);
   
   // I'm not busy transmitting anymore
   dn_ipmg_vars.busyTx=FALSE;
}

//===== pingMote

/**
The pingMote command sends a ping (echo request) to the mote specified by MAC 
address. A unique callbackId is generated and returned with the response. When 
the response is received from the mote, the manager generates a pingResponse 
notification with the measured round trip delay and several other parameters. 
The request is sent using unacknowledged transport, so the mote is not 
guaranteed to receive the request. 
*/
dn_err_t dn_ipmg_pingMote(uint8_t* macAddress, dn_ipmg_pingMote_rpt* reply) {
   uint8_t    extraFlags;
   dn_err_t   rc;
   
   // lock the module
   dn_lock();
   
   // verify no ongoing transmissions
   if (dn_ipmg_vars.busyTx) {
      // unlock the module
      dn_unlock();
      
      // return
      return DN_ERR_BUSY;
   }
   
   // store callback information
   dn_ipmg_vars.cmdId          = CMDID_PINGMOTE;
   dn_ipmg_vars.replyContents  = (uint8_t*)reply;
   
   // extraFlags
   extraFlags = 0x00;
   
   // build outputBuf
   memcpy(&dn_ipmg_vars.outputBuf[DN_PINGMOTE_REQ_OFFS_MACADDRESS],macAddress,8);
   
   // send outputBuf
   rc = dn_serial_mg_sendRequest(
      CMDID_PINGMOTE,                                           // cmdId
      extraFlags,                                               // extraFlags
      dn_ipmg_vars.outputBuf,                                   // payload
      DN_PINGMOTE_REQ_LEN,                                      // length
      dn_ipmg_pingMote_reply                                    // replyCb
   );
   
   if (rc==DN_ERR_NONE) {
      // I'm now busy transmitting
      dn_ipmg_vars.busyTx         = TRUE;
   }
   
   // unlock the module
   dn_unlock();
   
   return rc;
   
}

void dn_ipmg_pingMote_reply(uint8_t cmdId, uint8_t rc, uint8_t* payload, uint8_t len) {
   dn_ipmg_pingMote_rpt* reply;
   
   // verify I'm expecting this answer
   if (dn_ipmg_vars.busyTx==FALSE || dn_ipmg_vars.cmdId!=cmdId) {
      return;
   }
   
   // verify length
   if (rc==DN_SERIAL_RC_OK && len<DN_PINGMOTE_REPLY_LEN) {
      return;
   }
   
   // cast the replyContent
   reply = (dn_ipmg_pingMote_rpt*)dn_ipmg_vars.replyContents;
   
   // store RC
   reply->RC = rc;
   
   // parse returned value (iff RC==0)
   if (rc==DN_SERIAL_RC_OK) {
      
      dn_read_uint32_t(&reply->callbackId,&payload[DN_PINGMOTE_REPLY_OFFS_CALLBACKID]);
   }
   
   // call the callback
   dn_ipmg_vars.replyCb(cmdId);
   
   // I'm not busy transmitting anymore
   dn_ipmg_vars.busyTx=FALSE;
}

//===== getLog

/**
The getLog command retrieves diagnostic logs from the manager or a mote 
specified by MAC address. 
*/
dn_err_t dn_ipmg_getLog(uint8_t* macAddress, dn_ipmg_getLog_rpt* reply) {
   uint8_t    extraFlags;
   dn_err_t   rc;
   
   // lock the module
   dn_lock();
   
   // verify no ongoing transmissions
   if (dn_ipmg_vars.busyTx) {
      // unlock the module
      dn_unlock();
      
      // return
      return DN_ERR_BUSY;
   }
   
   // store callback information
   dn_ipmg_vars.cmdId          = CMDID_GETLOG;
   dn_ipmg_vars.replyContents  = (uint8_t*)reply;
   
   // extraFlags
   extraFlags = 0x00;
   
   // build outputBuf
   memcpy(&dn_ipmg_vars.outputBuf[DN_GETLOG_REQ_OFFS_MACADDRESS],macAddress,8);
   
   // send outputBuf
   rc = dn_serial_mg_sendRequest(
      CMDID_GETLOG,                                             // cmdId
      extraFlags,                                               // extraFlags
      dn_ipmg_vars.outputBuf,                                   // payload
      DN_GETLOG_REQ_LEN,                                        // length
      dn_ipmg_getLog_reply                                      // replyCb
   );
   
   if (rc==DN_ERR_NONE) {
      // I'm now busy transmitting
      dn_ipmg_vars.busyTx         = TRUE;
   }
   
   // unlock the module
   dn_unlock();
   
   return rc;
   
}

void dn_ipmg_getLog_reply(uint8_t cmdId, uint8_t rc, uint8_t* payload, uint8_t len) {
   dn_ipmg_getLog_rpt* reply;
   
   // verify I'm expecting this answer
   if (dn_ipmg_vars.busyTx==FALSE || dn_ipmg_vars.cmdId!=cmdId) {
      return;
   }
   
   // do NOT verify length (no return fields expected)
   
   // cast the replyContent
   reply = (dn_ipmg_getLog_rpt*)dn_ipmg_vars.replyContents;
   
   // store RC
   reply->RC = rc;
   
   // parse returned value (iff RC==0)
   if (rc==DN_SERIAL_RC_OK) {
      
   }
   
   // call the callback
   dn_ipmg_vars.replyCb(cmdId);
   
   // I'm not busy transmitting anymore
   dn_ipmg_vars.busyTx=FALSE;
}

//===== sendData

/**
The sendData command sends a packet to a mote in the network. The response 
contains a callbackId. When the manager injects the packet into the network, it 
will generate a packetSent notification. It is the responsibility of the 
customer'sapplication layer at the mote to send a response. It is also the 
responsibility of thecustomer's application layer to timeout if no response is 
received at the manager if one is expected.

The sendData command should be used by applications that communicate directly 
with the manager. If end-to-end (application to mote) IP connectivity is 
required, the application should use the sendIP command. For a more 
comprehensive discussion of the distinction, see the SmartMesh IPNetwork User 
Guide. 
*/
dn_err_t dn_ipmg_sendData(uint8_t* macAddress, uint8_t priority, uint16_t srcPort, uint16_t dstPort, uint8_t options, uint8_t* data, uint8_t dataLen, dn_ipmg_sendData_rpt* reply) {
   uint8_t    extraFlags;
   dn_err_t   rc;
   
   // lock the module
   dn_lock();
   
   // verify no ongoing transmissions
   if (dn_ipmg_vars.busyTx) {
      // unlock the module
      dn_unlock();
      
      // return
      return DN_ERR_BUSY;
   }
   
   // store callback information
   dn_ipmg_vars.cmdId          = CMDID_SENDDATA;
   dn_ipmg_vars.replyContents  = (uint8_t*)reply;
   
   // extraFlags
   extraFlags = 0x00;
   
   // build outputBuf
   memcpy(&dn_ipmg_vars.outputBuf[DN_SENDDATA_REQ_OFFS_MACADDRESS],macAddress,8);
   dn_ipmg_vars.outputBuf[DN_SENDDATA_REQ_OFFS_PRIORITY] = priority;
   dn_write_uint16_t(&dn_ipmg_vars.outputBuf[DN_SENDDATA_REQ_OFFS_SRCPORT],srcPort);
   dn_write_uint16_t(&dn_ipmg_vars.outputBuf[DN_SENDDATA_REQ_OFFS_DSTPORT],dstPort);
   dn_ipmg_vars.outputBuf[DN_SENDDATA_REQ_OFFS_OPTIONS] = options;
   memcpy(&dn_ipmg_vars.outputBuf[DN_SENDDATA_REQ_OFFS_DATA],data,dataLen);
   
   // send outputBuf
   rc = dn_serial_mg_sendRequest(
      CMDID_SENDDATA,                                           // cmdId
      extraFlags,                                               // extraFlags
      dn_ipmg_vars.outputBuf,                                   // payload
      DN_SENDDATA_REQ_LEN+dataLen,                              // length
      dn_ipmg_sendData_reply                                    // replyCb
   );
   
   if (rc==DN_ERR_NONE) {
      // I'm now busy transmitting
      dn_ipmg_vars.busyTx         = TRUE;
   }
   
   // unlock the module
   dn_unlock();
   
   return rc;
   
}

void dn_ipmg_sendData_reply(uint8_t cmdId, uint8_t rc, uint8_t* payload, uint8_t len) {
   dn_ipmg_sendData_rpt* reply;
   
   // verify I'm expecting this answer
   if (dn_ipmg_vars.busyTx==FALSE || dn_ipmg_vars.cmdId!=cmdId) {
      return;
   }
   
   // verify length
   if (rc==DN_SERIAL_RC_OK && len<DN_SENDDATA_REPLY_LEN) {
      return;
   }
   
   // cast the replyContent
   reply = (dn_ipmg_sendData_rpt*)dn_ipmg_vars.replyContents;
   
   // store RC
   reply->RC = rc;
   
   // parse returned value (iff RC==0)
   if (rc==DN_SERIAL_RC_OK) {
      
      dn_read_uint32_t(&reply->callbackId,&payload[DN_SENDDATA_REPLY_OFFS_CALLBACKID]);
   }
   
   // call the callback
   dn_ipmg_vars.replyCb(cmdId);
   
   // I'm not busy transmitting anymore
   dn_ipmg_vars.busyTx=FALSE;
}

//===== startNetwork

/**
The startNetwork command tells the manager to allow the network to start 
forming (begin accepting join requests from devices). The external application 
must issue the startNetwork command if the autoStartNetwork flag is not set 
(see setNetworkConfig).

This command has been deprecated and should not be used in new designs. 
*/
dn_err_t dn_ipmg_startNetwork(dn_ipmg_startNetwork_rpt* reply) {
   uint8_t    extraFlags;
   dn_err_t   rc;
   
   // lock the module
   dn_lock();
   
   // verify no ongoing transmissions
   if (dn_ipmg_vars.busyTx) {
      // unlock the module
      dn_unlock();
      
      // return
      return DN_ERR_BUSY;
   }
   
   // store callback information
   dn_ipmg_vars.cmdId          = CMDID_STARTNETWORK;
   dn_ipmg_vars.replyContents  = (uint8_t*)reply;
   
   // extraFlags
   extraFlags = 0x00;
   
   // build outputBuf
   
   // send outputBuf
   rc = dn_serial_mg_sendRequest(
      CMDID_STARTNETWORK,                                       // cmdId
      extraFlags,                                               // extraFlags
      dn_ipmg_vars.outputBuf,                                   // payload
      DN_STARTNETWORK_REQ_LEN,                                  // length
      dn_ipmg_startNetwork_reply                                // replyCb
   );
   
   if (rc==DN_ERR_NONE) {
      // I'm now busy transmitting
      dn_ipmg_vars.busyTx         = TRUE;
   }
   
   // unlock the module
   dn_unlock();
   
   return rc;
   
}

void dn_ipmg_startNetwork_reply(uint8_t cmdId, uint8_t rc, uint8_t* payload, uint8_t len) {
   dn_ipmg_startNetwork_rpt* reply;
   
   // verify I'm expecting this answer
   if (dn_ipmg_vars.busyTx==FALSE || dn_ipmg_vars.cmdId!=cmdId) {
      return;
   }
   
   // do NOT verify length (no return fields expected)
   
   // cast the replyContent
   reply = (dn_ipmg_startNetwork_rpt*)dn_ipmg_vars.replyContents;
   
   // store RC
   reply->RC = rc;
   
   // parse returned value (iff RC==0)
   if (rc==DN_SERIAL_RC_OK) {
      
   }
   
   // call the callback
   dn_ipmg_vars.replyCb(cmdId);
   
   // I'm not busy transmitting anymore
   dn_ipmg_vars.busyTx=FALSE;
}

//===== getSystemInfo

/**
The getSystemInfo command returns system-level information about the hardware 
and software versions. 
*/
dn_err_t dn_ipmg_getSystemInfo(dn_ipmg_getSystemInfo_rpt* reply) {
   uint8_t    extraFlags;
   dn_err_t   rc;
   
   // lock the module
   dn_lock();
   
   // verify no ongoing transmissions
   if (dn_ipmg_vars.busyTx) {
      // unlock the module
      dn_unlock();
      
      // return
      return DN_ERR_BUSY;
   }
   
   // store callback information
   dn_ipmg_vars.cmdId          = CMDID_GETSYSTEMINFO;
   dn_ipmg_vars.replyContents  = (uint8_t*)reply;
   
   // extraFlags
   extraFlags = 0x00;
   
   // build outputBuf
   
   // send outputBuf
   rc = dn_serial_mg_sendRequest(
      CMDID_GETSYSTEMINFO,                                      // cmdId
      extraFlags,                                               // extraFlags
      dn_ipmg_vars.outputBuf,                                   // payload
      DN_GETSYSTEMINFO_REQ_LEN,                                 // length
      dn_ipmg_getSystemInfo_reply                               // replyCb
   );
   
   if (rc==DN_ERR_NONE) {
      // I'm now busy transmitting
      dn_ipmg_vars.busyTx         = TRUE;
   }
   
   // unlock the module
   dn_unlock();
   
   return rc;
   
}

void dn_ipmg_getSystemInfo_reply(uint8_t cmdId, uint8_t rc, uint8_t* payload, uint8_t len) {
   dn_ipmg_getSystemInfo_rpt* reply;
   
   // verify I'm expecting this answer
   if (dn_ipmg_vars.busyTx==FALSE || dn_ipmg_vars.cmdId!=cmdId) {
      return;
   }
   
   // verify length
   if (rc==DN_SERIAL_RC_OK && len<DN_GETSYSTEMINFO_REPLY_LEN) {
      return;
   }
   
   // cast the replyContent
   reply = (dn_ipmg_getSystemInfo_rpt*)dn_ipmg_vars.replyContents;
   
   // store RC
   reply->RC = rc;
   
   // parse returned value (iff RC==0)
   if (rc==DN_SERIAL_RC_OK) {
      
      memcpy(&reply->macAddress[0],&payload[DN_GETSYSTEMINFO_REPLY_OFFS_MACADDRESS],8);
      reply->hwModel = payload[DN_GETSYSTEMINFO_REPLY_OFFS_HWMODEL];
      reply->hwRev = payload[DN_GETSYSTEMINFO_REPLY_OFFS_HWREV];
      reply->swMajor = payload[DN_GETSYSTEMINFO_REPLY_OFFS_SWMAJOR];
      reply->swMinor = payload[DN_GETSYSTEMINFO_REPLY_OFFS_SWMINOR];
      reply->swPatch = payload[DN_GETSYSTEMINFO_REPLY_OFFS_SWPATCH];
      dn_read_uint16_t(&reply->swBuild,&payload[DN_GETSYSTEMINFO_REPLY_OFFS_SWBUILD]);
   }
   
   // call the callback
   dn_ipmg_vars.replyCb(cmdId);
   
   // I'm not busy transmitting anymore
   dn_ipmg_vars.busyTx=FALSE;
}

//===== getMoteConfig

/**
The getMoteConfig command returns a single mote description as the response. 
The command takes two arguments, a MAC Address and a flag indicating whether 
the MAC Address refers to the requested mote or to the next mote in managers 
memory. This command may be used to iterate through all motes known by the 
manager by starting with the macAddress parameter set to 0 and next set to 
true, and then using the MAC Address of that response as the input to the next call.

The mote MAC address is used in all query commands, but space constraints 
require the neighbor health reports to use the Mote ID for identification. 
Therefore, both identifiers are present in the mote structure. 
*/
dn_err_t dn_ipmg_getMoteConfig(uint8_t* macAddress, bool next, dn_ipmg_getMoteConfig_rpt* reply) {
   uint8_t    extraFlags;
   dn_err_t   rc;
   
   // lock the module
   dn_lock();
   
   // verify no ongoing transmissions
   if (dn_ipmg_vars.busyTx) {
      // unlock the module
      dn_unlock();
      
      // return
      return DN_ERR_BUSY;
   }
   
   // store callback information
   dn_ipmg_vars.cmdId          = CMDID_GETMOTECONFIG;
   dn_ipmg_vars.replyContents  = (uint8_t*)reply;
   
   // extraFlags
   extraFlags = 0x00;
   
   // build outputBuf
   memcpy(&dn_ipmg_vars.outputBuf[DN_GETMOTECONFIG_REQ_OFFS_MACADDRESS],macAddress,8);
   dn_ipmg_vars.outputBuf[DN_GETMOTECONFIG_REQ_OFFS_NEXT] = next;
   
   // send outputBuf
   rc = dn_serial_mg_sendRequest(
      CMDID_GETMOTECONFIG,                                      // cmdId
      extraFlags,                                               // extraFlags
      dn_ipmg_vars.outputBuf,                                   // payload
      DN_GETMOTECONFIG_REQ_LEN,                                 // length
      dn_ipmg_getMoteConfig_reply                               // replyCb
   );
   
   if (rc==DN_ERR_NONE) {
      // I'm now busy transmitting
      dn_ipmg_vars.busyTx         = TRUE;
   }
   
   // unlock the module
   dn_unlock();
   
   return rc;
   
}

void dn_ipmg_getMoteConfig_reply(uint8_t cmdId, uint8_t rc, uint8_t* payload, uint8_t len) {
   dn_ipmg_getMoteConfig_rpt* reply;
   
   // verify I'm expecting this answer
   if (dn_ipmg_vars.busyTx==FALSE || dn_ipmg_vars.cmdId!=cmdId) {
      return;
   }
   
   // verify length
   if (rc==DN_SERIAL_RC_OK && len<DN_GETMOTECONFIG_REPLY_LEN) {
      return;
   }
   
   // cast the replyContent
   reply = (dn_ipmg_getMoteConfig_rpt*)dn_ipmg_vars.replyContents;
   
   // store RC
   reply->RC = rc;
   
   // parse returned value (iff RC==0)
   if (rc==DN_SERIAL_RC_OK) {
      
      memcpy(&reply->macAddress[0],&payload[DN_GETMOTECONFIG_REPLY_OFFS_MACADDRESS],8);
      dn_read_uint16_t(&reply->moteId,&payload[DN_GETMOTECONFIG_REPLY_OFFS_MOTEID]);
      reply->isAP = payload[DN_GETMOTECONFIG_REPLY_OFFS_ISAP];
      reply->state = payload[DN_GETMOTECONFIG_REPLY_OFFS_STATE];
      reply->reserved = payload[DN_GETMOTECONFIG_REPLY_OFFS_RESERVED];
      reply->isRouting = payload[DN_GETMOTECONFIG_REPLY_OFFS_ISROUTING];
   }
   
   // call the callback
   dn_ipmg_vars.replyCb(cmdId);
   
   // I'm not busy transmitting anymore
   dn_ipmg_vars.busyTx=FALSE;
}

//===== getPathInfo

/**
The getPathInfo command returns parameters of requested path. 
*/
dn_err_t dn_ipmg_getPathInfo(uint8_t* source, uint8_t* dest, dn_ipmg_getPathInfo_rpt* reply) {
   uint8_t    extraFlags;
   dn_err_t   rc;
   
   // lock the module
   dn_lock();
   
   // verify no ongoing transmissions
   if (dn_ipmg_vars.busyTx) {
      // unlock the module
      dn_unlock();
      
      // return
      return DN_ERR_BUSY;
   }
   
   // store callback information
   dn_ipmg_vars.cmdId          = CMDID_GETPATHINFO;
   dn_ipmg_vars.replyContents  = (uint8_t*)reply;
   
   // extraFlags
   extraFlags = 0x00;
   
   // build outputBuf
   memcpy(&dn_ipmg_vars.outputBuf[DN_GETPATHINFO_REQ_OFFS_SOURCE],source,8);
   memcpy(&dn_ipmg_vars.outputBuf[DN_GETPATHINFO_REQ_OFFS_DEST],dest,8);
   
   // send outputBuf
   rc = dn_serial_mg_sendRequest(
      CMDID_GETPATHINFO,                                        // cmdId
      extraFlags,                                               // extraFlags
      dn_ipmg_vars.outputBuf,                                   // payload
      DN_GETPATHINFO_REQ_LEN,                                   // length
      dn_ipmg_getPathInfo_reply                                 // replyCb
   );
   
   if (rc==DN_ERR_NONE) {
      // I'm now busy transmitting
      dn_ipmg_vars.busyTx         = TRUE;
   }
   
   // unlock the module
   dn_unlock();
   
   return rc;
   
}

void dn_ipmg_getPathInfo_reply(uint8_t cmdId, uint8_t rc, uint8_t* payload, uint8_t len) {
   dn_ipmg_getPathInfo_rpt* reply;
   
   // verify I'm expecting this answer
   if (dn_ipmg_vars.busyTx==FALSE || dn_ipmg_vars.cmdId!=cmdId) {
      return;
   }
   
   // verify length
   if (rc==DN_SERIAL_RC_OK && len<DN_GETPATHINFO_REPLY_LEN) {
      return;
   }
   
   // cast the replyContent
   reply = (dn_ipmg_getPathInfo_rpt*)dn_ipmg_vars.replyContents;
   
   // store RC
   reply->RC = rc;
   
   // parse returned value (iff RC==0)
   if (rc==DN_SERIAL_RC_OK) {
      
      memcpy(&reply->source[0],&payload[DN_GETPATHINFO_REPLY_OFFS_SOURCE],8);
      memcpy(&reply->dest[0],&payload[DN_GETPATHINFO_REPLY_OFFS_DEST],8);
      reply->direction = payload[DN_GETPATHINFO_REPLY_OFFS_DIRECTION];
      reply->numLinks = payload[DN_GETPATHINFO_REPLY_OFFS_NUMLINKS];
      reply->quality = payload[DN_GETPATHINFO_REPLY_OFFS_QUALITY];
      reply->rssiSrcDest = (int8_t)payload[DN_GETPATHINFO_REPLY_OFFS_RSSISRCDEST];
      reply->rssiDestSrc = (int8_t)payload[DN_GETPATHINFO_REPLY_OFFS_RSSIDESTSRC];
   }
   
   // call the callback
   dn_ipmg_vars.replyCb(cmdId);
   
   // I'm not busy transmitting anymore
   dn_ipmg_vars.busyTx=FALSE;
}

//===== getNextPathInfo

/**
The getNextPathInfo command allows iteration across paths connected to a 
particular mote. The pathId parameter indicates the previous value in the 
iteration. Setting pathId to 0 returns the first path. A pathId can not be used 
as a unique identifier for a path. It is only valid when associated with a 
particular mote. 
*/
dn_err_t dn_ipmg_getNextPathInfo(uint8_t* macAddress, uint8_t filter, uint16_t pathId, dn_ipmg_getNextPathInfo_rpt* reply) {
   uint8_t    extraFlags;
   dn_err_t   rc;
   
   // lock the module
   dn_lock();
   
   // verify no ongoing transmissions
   if (dn_ipmg_vars.busyTx) {
      // unlock the module
      dn_unlock();
      
      // return
      return DN_ERR_BUSY;
   }
   
   // store callback information
   dn_ipmg_vars.cmdId          = CMDID_GETNEXTPATHINFO;
   dn_ipmg_vars.replyContents  = (uint8_t*)reply;
   
   // extraFlags
   extraFlags = 0x00;
   
   // build outputBuf
   memcpy(&dn_ipmg_vars.outputBuf[DN_GETNEXTPATHINFO_REQ_OFFS_MACADDRESS],macAddress,8);
   dn_ipmg_vars.outputBuf[DN_GETNEXTPATHINFO_REQ_OFFS_FILTER] = filter;
   dn_write_uint16_t(&dn_ipmg_vars.outputBuf[DN_GETNEXTPATHINFO_REQ_OFFS_PATHID],pathId);
   
   // send outputBuf
   rc = dn_serial_mg_sendRequest(
      CMDID_GETNEXTPATHINFO,                                    // cmdId
      extraFlags,                                               // extraFlags
      dn_ipmg_vars.outputBuf,                                   // payload
      DN_GETNEXTPATHINFO_REQ_LEN,                               // length
      dn_ipmg_getNextPathInfo_reply                             // replyCb
   );
   
   if (rc==DN_ERR_NONE) {
      // I'm now busy transmitting
      dn_ipmg_vars.busyTx         = TRUE;
   }
   
   // unlock the module
   dn_unlock();
   
   return rc;
   
}

void dn_ipmg_getNextPathInfo_reply(uint8_t cmdId, uint8_t rc, uint8_t* payload, uint8_t len) {
   dn_ipmg_getNextPathInfo_rpt* reply;
   
   // verify I'm expecting this answer
   if (dn_ipmg_vars.busyTx==FALSE || dn_ipmg_vars.cmdId!=cmdId) {
      return;
   }
   
   // verify length
   if (rc==DN_SERIAL_RC_OK && len<DN_GETNEXTPATHINFO_REPLY_LEN) {
      return;
   }
   
   // cast the replyContent
   reply = (dn_ipmg_getNextPathInfo_rpt*)dn_ipmg_vars.replyContents;
   
   // store RC
   reply->RC = rc;
   
   // parse returned value (iff RC==0)
   if (rc==DN_SERIAL_RC_OK) {
      
      dn_read_uint16_t(&reply->pathId,&payload[DN_GETNEXTPATHINFO_REPLY_OFFS_PATHID]);
      memcpy(&reply->source[0],&payload[DN_GETNEXTPATHINFO_REPLY_OFFS_SOURCE],8);
      memcpy(&reply->dest[0],&payload[DN_GETNEXTPATHINFO_REPLY_OFFS_DEST],8);
      reply->direction = payload[DN_GETNEXTPATHINFO_REPLY_OFFS_DIRECTION];
      reply->numLinks = payload[DN_GETNEXTPATHINFO_REPLY_OFFS_NUMLINKS];
      reply->quality = payload[DN_GETNEXTPATHINFO_REPLY_OFFS_QUALITY];
      reply->rssiSrcDest = (int8_t)payload[DN_GETNEXTPATHINFO_REPLY_OFFS_RSSISRCDEST];
      reply->rssiDestSrc = (int8_t)payload[DN_GETNEXTPATHINFO_REPLY_OFFS_RSSIDESTSRC];
   }
   
   // call the callback
   dn_ipmg_vars.replyCb(cmdId);
   
   // I'm not busy transmitting anymore
   dn_ipmg_vars.busyTx=FALSE;
}

//===== setAdvertising

/**
The setAdvertising command tells the manager to activate or deactivate 
advertising. The response is a callbackId. A commandFinished notification with 
the callbackId is generated when the command propagation is complete. 
*/
dn_err_t dn_ipmg_setAdvertising(uint8_t activate, dn_ipmg_setAdvertising_rpt* reply) {
   uint8_t    extraFlags;
   dn_err_t   rc;
   
   // lock the module
   dn_lock();
   
   // verify no ongoing transmissions
   if (dn_ipmg_vars.busyTx) {
      // unlock the module
      dn_unlock();
      
      // return
      return DN_ERR_BUSY;
   }
   
   // store callback information
   dn_ipmg_vars.cmdId          = CMDID_SETADVERTISING;
   dn_ipmg_vars.replyContents  = (uint8_t*)reply;
   
   // extraFlags
   extraFlags = 0x00;
   
   // build outputBuf
   dn_ipmg_vars.outputBuf[DN_SETADVERTISING_REQ_OFFS_ACTIVATE] = activate;
   
   // send outputBuf
   rc = dn_serial_mg_sendRequest(
      CMDID_SETADVERTISING,                                     // cmdId
      extraFlags,                                               // extraFlags
      dn_ipmg_vars.outputBuf,                                   // payload
      DN_SETADVERTISING_REQ_LEN,                                // length
      dn_ipmg_setAdvertising_reply                              // replyCb
   );
   
   if (rc==DN_ERR_NONE) {
      // I'm now busy transmitting
      dn_ipmg_vars.busyTx         = TRUE;
   }
   
   // unlock the module
   dn_unlock();
   
   return rc;
   
}

void dn_ipmg_setAdvertising_reply(uint8_t cmdId, uint8_t rc, uint8_t* payload, uint8_t len) {
   dn_ipmg_setAdvertising_rpt* reply;
   
   // verify I'm expecting this answer
   if (dn_ipmg_vars.busyTx==FALSE || dn_ipmg_vars.cmdId!=cmdId) {
      return;
   }
   
   // verify length
   if (rc==DN_SERIAL_RC_OK && len<DN_SETADVERTISING_REPLY_LEN) {
      return;
   }
   
   // cast the replyContent
   reply = (dn_ipmg_setAdvertising_rpt*)dn_ipmg_vars.replyContents;
   
   // store RC
   reply->RC = rc;
   
   // parse returned value (iff RC==0)
   if (rc==DN_SERIAL_RC_OK) {
      
      dn_read_uint32_t(&reply->callbackId,&payload[DN_SETADVERTISING_REPLY_OFFS_CALLBACKID]);
   }
   
   // call the callback
   dn_ipmg_vars.replyCb(cmdId);
   
   // I'm not busy transmitting anymore
   dn_ipmg_vars.busyTx=FALSE;
}

//===== setDownstreamFrameMode

/**
The setDownstreamFrameMode command tells the manager to shorten or extend the 
downstream slotframe. The base slotframe length will be multiplied by the 
downFrameMultVal for "normal" speed. For "fast" speed the downstream slotframe 
is the base length.Once this command is executed, the manager switches to 
manual mode and no longer changes slotframesize automatically. The response is 
a callbackId. A commandFinished notification with the callbackId is generated 
when the command propagation is complete. 
*/
dn_err_t dn_ipmg_setDownstreamFrameMode(uint8_t frameMode, dn_ipmg_setDownstreamFrameMode_rpt* reply) {
   uint8_t    extraFlags;
   dn_err_t   rc;
   
   // lock the module
   dn_lock();
   
   // verify no ongoing transmissions
   if (dn_ipmg_vars.busyTx) {
      // unlock the module
      dn_unlock();
      
      // return
      return DN_ERR_BUSY;
   }
   
   // store callback information
   dn_ipmg_vars.cmdId          = CMDID_SETDOWNSTREAMFRAMEMODE;
   dn_ipmg_vars.replyContents  = (uint8_t*)reply;
   
   // extraFlags
   extraFlags = 0x00;
   
   // build outputBuf
   dn_ipmg_vars.outputBuf[DN_SETDOWNSTREAMFRAMEMODE_REQ_OFFS_FRAMEMODE] = frameMode;
   
   // send outputBuf
   rc = dn_serial_mg_sendRequest(
      CMDID_SETDOWNSTREAMFRAMEMODE,                             // cmdId
      extraFlags,                                               // extraFlags
      dn_ipmg_vars.outputBuf,                                   // payload
      DN_SETDOWNSTREAMFRAMEMODE_REQ_LEN,                        // length
      dn_ipmg_setDownstreamFrameMode_reply                      // replyCb
   );
   
   if (rc==DN_ERR_NONE) {
      // I'm now busy transmitting
      dn_ipmg_vars.busyTx         = TRUE;
   }
   
   // unlock the module
   dn_unlock();
   
   return rc;
   
}

void dn_ipmg_setDownstreamFrameMode_reply(uint8_t cmdId, uint8_t rc, uint8_t* payload, uint8_t len) {
   dn_ipmg_setDownstreamFrameMode_rpt* reply;
   
   // verify I'm expecting this answer
   if (dn_ipmg_vars.busyTx==FALSE || dn_ipmg_vars.cmdId!=cmdId) {
      return;
   }
   
   // verify length
   if (rc==DN_SERIAL_RC_OK && len<DN_SETDOWNSTREAMFRAMEMODE_REPLY_LEN) {
      return;
   }
   
   // cast the replyContent
   reply = (dn_ipmg_setDownstreamFrameMode_rpt*)dn_ipmg_vars.replyContents;
   
   // store RC
   reply->RC = rc;
   
   // parse returned value (iff RC==0)
   if (rc==DN_SERIAL_RC_OK) {
      
      dn_read_uint32_t(&reply->callbackId,&payload[DN_SETDOWNSTREAMFRAMEMODE_REPLY_OFFS_CALLBACKID]);
   }
   
   // call the callback
   dn_ipmg_vars.replyCb(cmdId);
   
   // I'm not busy transmitting anymore
   dn_ipmg_vars.busyTx=FALSE;
}

//===== getManagerStatistics

/**
The getManagerStatistics command returns dynamic information and statistics 
about the manager API. The statistics counts are cleared together with all 
current statistics using clearStatistics. 
*/
dn_err_t dn_ipmg_getManagerStatistics(dn_ipmg_getManagerStatistics_rpt* reply) {
   uint8_t    extraFlags;
   dn_err_t   rc;
   
   // lock the module
   dn_lock();
   
   // verify no ongoing transmissions
   if (dn_ipmg_vars.busyTx) {
      // unlock the module
      dn_unlock();
      
      // return
      return DN_ERR_BUSY;
   }
   
   // store callback information
   dn_ipmg_vars.cmdId          = CMDID_GETMANAGERSTATISTICS;
   dn_ipmg_vars.replyContents  = (uint8_t*)reply;
   
   // extraFlags
   extraFlags = 0x00;
   
   // build outputBuf
   
   // send outputBuf
   rc = dn_serial_mg_sendRequest(
      CMDID_GETMANAGERSTATISTICS,                               // cmdId
      extraFlags,                                               // extraFlags
      dn_ipmg_vars.outputBuf,                                   // payload
      DN_GETMANAGERSTATISTICS_REQ_LEN,                          // length
      dn_ipmg_getManagerStatistics_reply                        // replyCb
   );
   
   if (rc==DN_ERR_NONE) {
      // I'm now busy transmitting
      dn_ipmg_vars.busyTx         = TRUE;
   }
   
   // unlock the module
   dn_unlock();
   
   return rc;
   
}

void dn_ipmg_getManagerStatistics_reply(uint8_t cmdId, uint8_t rc, uint8_t* payload, uint8_t len) {
   dn_ipmg_getManagerStatistics_rpt* reply;
   
   // verify I'm expecting this answer
   if (dn_ipmg_vars.busyTx==FALSE || dn_ipmg_vars.cmdId!=cmdId) {
      return;
   }
   
   // verify length
   if (rc==DN_SERIAL_RC_OK && len<DN_GETMANAGERSTATISTICS_REPLY_LEN) {
      return;
   }
   
   // cast the replyContent
   reply = (dn_ipmg_getManagerStatistics_rpt*)dn_ipmg_vars.replyContents;
   
   // store RC
   reply->RC = rc;
   
   // parse returned value (iff RC==0)
   if (rc==DN_SERIAL_RC_OK) {
      
      dn_read_uint16_t(&reply->serTxCnt,&payload[DN_GETMANAGERSTATISTICS_REPLY_OFFS_SERTXCNT]);
      dn_read_uint16_t(&reply->serRxCnt,&payload[DN_GETMANAGERSTATISTICS_REPLY_OFFS_SERRXCNT]);
      dn_read_uint16_t(&reply->serRxCRCErr,&payload[DN_GETMANAGERSTATISTICS_REPLY_OFFS_SERRXCRCERR]);
      dn_read_uint16_t(&reply->serRxOverruns,&payload[DN_GETMANAGERSTATISTICS_REPLY_OFFS_SERRXOVERRUNS]);
      dn_read_uint16_t(&reply->apiEstabConn,&payload[DN_GETMANAGERSTATISTICS_REPLY_OFFS_APIESTABCONN]);
      dn_read_uint16_t(&reply->apiDroppedConn,&payload[DN_GETMANAGERSTATISTICS_REPLY_OFFS_APIDROPPEDCONN]);
      dn_read_uint16_t(&reply->apiTxOk,&payload[DN_GETMANAGERSTATISTICS_REPLY_OFFS_APITXOK]);
      dn_read_uint16_t(&reply->apiTxErr,&payload[DN_GETMANAGERSTATISTICS_REPLY_OFFS_APITXERR]);
      dn_read_uint16_t(&reply->apiTxFail,&payload[DN_GETMANAGERSTATISTICS_REPLY_OFFS_APITXFAIL]);
      dn_read_uint16_t(&reply->apiRxOk,&payload[DN_GETMANAGERSTATISTICS_REPLY_OFFS_APIRXOK]);
      dn_read_uint16_t(&reply->apiRxProtErr,&payload[DN_GETMANAGERSTATISTICS_REPLY_OFFS_APIRXPROTERR]);
   }
   
   // call the callback
   dn_ipmg_vars.replyCb(cmdId);
   
   // I'm not busy transmitting anymore
   dn_ipmg_vars.busyTx=FALSE;
}

//===== setTime

/**
This command has been deprecated, and should not be used in new designs. When 
the Manager restarts, it will start counting from 20:00:00 UTC July 2, 2002.

The setTime command sets the UTC time on the manager. This command may only be 
executed when the network is not running. If the trigger flag is false, the 
manager sets the specified time as soon as it receives the setTime command. 
When the manager receives a Time Pin trigger, it temporarily stores the current 
time. If a setTime request is received within a short period of time following 
the trigger, the manager calculates the delay since the trigger and adjust the 
time such that the trigger was received at the specified time value. 
*/
dn_err_t dn_ipmg_setTime(uint8_t trigger, uint8_t* utcSecs, uint32_t utcUsecs, dn_ipmg_setTime_rpt* reply) {
   uint8_t    extraFlags;
   dn_err_t   rc;
   
   // lock the module
   dn_lock();
   
   // verify no ongoing transmissions
   if (dn_ipmg_vars.busyTx) {
      // unlock the module
      dn_unlock();
      
      // return
      return DN_ERR_BUSY;
   }
   
   // store callback information
   dn_ipmg_vars.cmdId          = CMDID_SETTIME;
   dn_ipmg_vars.replyContents  = (uint8_t*)reply;
   
   // extraFlags
   extraFlags = 0x00;
   
   // build outputBuf
   dn_ipmg_vars.outputBuf[DN_SETTIME_REQ_OFFS_TRIGGER] = trigger;
   memcpy(&dn_ipmg_vars.outputBuf[DN_SETTIME_REQ_OFFS_UTCSECS],utcSecs,8);
   dn_write_uint32_t(&dn_ipmg_vars.outputBuf[DN_SETTIME_REQ_OFFS_UTCUSECS],utcUsecs);
   
   // send outputBuf
   rc = dn_serial_mg_sendRequest(
      CMDID_SETTIME,                                            // cmdId
      extraFlags,                                               // extraFlags
      dn_ipmg_vars.outputBuf,                                   // payload
      DN_SETTIME_REQ_LEN,                                       // length
      dn_ipmg_setTime_reply                                     // replyCb
   );
   
   if (rc==DN_ERR_NONE) {
      // I'm now busy transmitting
      dn_ipmg_vars.busyTx         = TRUE;
   }
   
   // unlock the module
   dn_unlock();
   
   return rc;
   
}

void dn_ipmg_setTime_reply(uint8_t cmdId, uint8_t rc, uint8_t* payload, uint8_t len) {
   dn_ipmg_setTime_rpt* reply;
   
   // verify I'm expecting this answer
   if (dn_ipmg_vars.busyTx==FALSE || dn_ipmg_vars.cmdId!=cmdId) {
      return;
   }
   
   // do NOT verify length (no return fields expected)
   
   // cast the replyContent
   reply = (dn_ipmg_setTime_rpt*)dn_ipmg_vars.replyContents;
   
   // store RC
   reply->RC = rc;
   
   // parse returned value (iff RC==0)
   if (rc==DN_SERIAL_RC_OK) {
      
   }
   
   // call the callback
   dn_ipmg_vars.replyCb(cmdId);
   
   // I'm not busy transmitting anymore
   dn_ipmg_vars.busyTx=FALSE;
}

//===== getLicense

/**
The getLicense command returns the current license key. 
*/
dn_err_t dn_ipmg_getLicense(dn_ipmg_getLicense_rpt* reply) {
   uint8_t    extraFlags;
   dn_err_t   rc;
   
   // lock the module
   dn_lock();
   
   // verify no ongoing transmissions
   if (dn_ipmg_vars.busyTx) {
      // unlock the module
      dn_unlock();
      
      // return
      return DN_ERR_BUSY;
   }
   
   // store callback information
   dn_ipmg_vars.cmdId          = CMDID_GETLICENSE;
   dn_ipmg_vars.replyContents  = (uint8_t*)reply;
   
   // extraFlags
   extraFlags = 0x00;
   
   // build outputBuf
   
   // send outputBuf
   rc = dn_serial_mg_sendRequest(
      CMDID_GETLICENSE,                                         // cmdId
      extraFlags,                                               // extraFlags
      dn_ipmg_vars.outputBuf,                                   // payload
      DN_GETLICENSE_REQ_LEN,                                    // length
      dn_ipmg_getLicense_reply                                  // replyCb
   );
   
   if (rc==DN_ERR_NONE) {
      // I'm now busy transmitting
      dn_ipmg_vars.busyTx         = TRUE;
   }
   
   // unlock the module
   dn_unlock();
   
   return rc;
   
}

void dn_ipmg_getLicense_reply(uint8_t cmdId, uint8_t rc, uint8_t* payload, uint8_t len) {
   dn_ipmg_getLicense_rpt* reply;
   
   // verify I'm expecting this answer
   if (dn_ipmg_vars.busyTx==FALSE || dn_ipmg_vars.cmdId!=cmdId) {
      return;
   }
   
   // verify length
   if (rc==DN_SERIAL_RC_OK && len<DN_GETLICENSE_REPLY_LEN) {
      return;
   }
   
   // cast the replyContent
   reply = (dn_ipmg_getLicense_rpt*)dn_ipmg_vars.replyContents;
   
   // store RC
   reply->RC = rc;
   
   // parse returned value (iff RC==0)
   if (rc==DN_SERIAL_RC_OK) {
      
      memcpy(&reply->license[0],&payload[DN_GETLICENSE_REPLY_OFFS_LICENSE],13);
   }
   
   // call the callback
   dn_ipmg_vars.replyCb(cmdId);
   
   // I'm not busy transmitting anymore
   dn_ipmg_vars.busyTx=FALSE;
}

//===== setLicense

/**
The setLicense command validates and updates the software license key stored in 
flash. Features enabled or disabled by the license key change will take effect 
after the device is restarted.If the license parameter is set to all 0x0s, the 
manager restores the default license. This change is persistent. 
*/
dn_err_t dn_ipmg_setLicense(uint8_t* license, dn_ipmg_setLicense_rpt* reply) {
   uint8_t    extraFlags;
   dn_err_t   rc;
   
   // lock the module
   dn_lock();
   
   // verify no ongoing transmissions
   if (dn_ipmg_vars.busyTx) {
      // unlock the module
      dn_unlock();
      
      // return
      return DN_ERR_BUSY;
   }
   
   // store callback information
   dn_ipmg_vars.cmdId          = CMDID_SETLICENSE;
   dn_ipmg_vars.replyContents  = (uint8_t*)reply;
   
   // extraFlags
   extraFlags = 0x00;
   
   // build outputBuf
   memcpy(&dn_ipmg_vars.outputBuf[DN_SETLICENSE_REQ_OFFS_LICENSE],license,13);
   
   // send outputBuf
   rc = dn_serial_mg_sendRequest(
      CMDID_SETLICENSE,                                         // cmdId
      extraFlags,                                               // extraFlags
      dn_ipmg_vars.outputBuf,                                   // payload
      DN_SETLICENSE_REQ_LEN,                                    // length
      dn_ipmg_setLicense_reply                                  // replyCb
   );
   
   if (rc==DN_ERR_NONE) {
      // I'm now busy transmitting
      dn_ipmg_vars.busyTx         = TRUE;
   }
   
   // unlock the module
   dn_unlock();
   
   return rc;
   
}

void dn_ipmg_setLicense_reply(uint8_t cmdId, uint8_t rc, uint8_t* payload, uint8_t len) {
   dn_ipmg_setLicense_rpt* reply;
   
   // verify I'm expecting this answer
   if (dn_ipmg_vars.busyTx==FALSE || dn_ipmg_vars.cmdId!=cmdId) {
      return;
   }
   
   // do NOT verify length (no return fields expected)
   
   // cast the replyContent
   reply = (dn_ipmg_setLicense_rpt*)dn_ipmg_vars.replyContents;
   
   // store RC
   reply->RC = rc;
   
   // parse returned value (iff RC==0)
   if (rc==DN_SERIAL_RC_OK) {
      
   }
   
   // call the callback
   dn_ipmg_vars.replyCb(cmdId);
   
   // I'm not busy transmitting anymore
   dn_ipmg_vars.busyTx=FALSE;
}

//===== setCLIUser

/**
The setCLIUser command sets the password that must be used to log into the 
command line for a particular user role. The user roles are:

- Viewer - read-only access to non-sensitive information
- User - read-write access This change is persistent. 
*/
dn_err_t dn_ipmg_setCLIUser(uint8_t role, uint8_t* password, dn_ipmg_setCLIUser_rpt* reply) {
   uint8_t    extraFlags;
   dn_err_t   rc;
   
   // lock the module
   dn_lock();
   
   // verify no ongoing transmissions
   if (dn_ipmg_vars.busyTx) {
      // unlock the module
      dn_unlock();
      
      // return
      return DN_ERR_BUSY;
   }
   
   // store callback information
   dn_ipmg_vars.cmdId          = CMDID_SETCLIUSER;
   dn_ipmg_vars.replyContents  = (uint8_t*)reply;
   
   // extraFlags
   extraFlags = 0x00;
   
   // build outputBuf
   dn_ipmg_vars.outputBuf[DN_SETCLIUSER_REQ_OFFS_ROLE] = role;
   memcpy(&dn_ipmg_vars.outputBuf[DN_SETCLIUSER_REQ_OFFS_PASSWORD],password,16);
   
   // send outputBuf
   rc = dn_serial_mg_sendRequest(
      CMDID_SETCLIUSER,                                         // cmdId
      extraFlags,                                               // extraFlags
      dn_ipmg_vars.outputBuf,                                   // payload
      DN_SETCLIUSER_REQ_LEN,                                    // length
      dn_ipmg_setCLIUser_reply                                  // replyCb
   );
   
   if (rc==DN_ERR_NONE) {
      // I'm now busy transmitting
      dn_ipmg_vars.busyTx         = TRUE;
   }
   
   // unlock the module
   dn_unlock();
   
   return rc;
   
}

void dn_ipmg_setCLIUser_reply(uint8_t cmdId, uint8_t rc, uint8_t* payload, uint8_t len) {
   dn_ipmg_setCLIUser_rpt* reply;
   
   // verify I'm expecting this answer
   if (dn_ipmg_vars.busyTx==FALSE || dn_ipmg_vars.cmdId!=cmdId) {
      return;
   }
   
   // do NOT verify length (no return fields expected)
   
   // cast the replyContent
   reply = (dn_ipmg_setCLIUser_rpt*)dn_ipmg_vars.replyContents;
   
   // store RC
   reply->RC = rc;
   
   // parse returned value (iff RC==0)
   if (rc==DN_SERIAL_RC_OK) {
      
   }
   
   // call the callback
   dn_ipmg_vars.replyCb(cmdId);
   
   // I'm not busy transmitting anymore
   dn_ipmg_vars.busyTx=FALSE;
}

//===== sendIP

/**
The sendIP command sends a 6LoWPAN packet to a mote in the network. The 
response contains a callback Id. When the manager injects the packet into the 
network, it will generate a packetSent notification with the calllbackId. The 
application is responsible for constructing a valid 6LoWPAN packet. The packet 
is sent to the mote best-effort, so the application should deal with responses 
and timeouts, if any.

The sendIP command should be used by applications that require end-to-end IP 
connectivity. For applications that do not require end-to-end IP connectivity, 
the sendData command provides a simpler interface without requiring the 
application to understand 6LoWPAN encapsulation. For a more comprehensive 
discussion of the distinction, see the SmartMesh IP Network User Guide. 
*/
dn_err_t dn_ipmg_sendIP(uint8_t* macAddress, uint8_t priority, uint8_t options, uint8_t encryptedOffset, uint8_t* data, uint8_t dataLen, dn_ipmg_sendIP_rpt* reply) {
   uint8_t    extraFlags;
   dn_err_t   rc;
   
   // lock the module
   dn_lock();
   
   // verify no ongoing transmissions
   if (dn_ipmg_vars.busyTx) {
      // unlock the module
      dn_unlock();
      
      // return
      return DN_ERR_BUSY;
   }
   
   // store callback information
   dn_ipmg_vars.cmdId          = CMDID_SENDIP;
   dn_ipmg_vars.replyContents  = (uint8_t*)reply;
   
   // extraFlags
   extraFlags = 0x00;
   
   // build outputBuf
   memcpy(&dn_ipmg_vars.outputBuf[DN_SENDIP_REQ_OFFS_MACADDRESS],macAddress,8);
   dn_ipmg_vars.outputBuf[DN_SENDIP_REQ_OFFS_PRIORITY] = priority;
   dn_ipmg_vars.outputBuf[DN_SENDIP_REQ_OFFS_OPTIONS] = options;
   dn_ipmg_vars.outputBuf[DN_SENDIP_REQ_OFFS_ENCRYPTEDOFFSET] = encryptedOffset;
   memcpy(&dn_ipmg_vars.outputBuf[DN_SENDIP_REQ_OFFS_DATA],data,dataLen);
   
   // send outputBuf
   rc = dn_serial_mg_sendRequest(
      CMDID_SENDIP,                                             // cmdId
      extraFlags,                                               // extraFlags
      dn_ipmg_vars.outputBuf,                                   // payload
      DN_SENDIP_REQ_LEN+dataLen,                                // length
      dn_ipmg_sendIP_reply                                      // replyCb
   );
   
   if (rc==DN_ERR_NONE) {
      // I'm now busy transmitting
      dn_ipmg_vars.busyTx         = TRUE;
   }
   
   // unlock the module
   dn_unlock();
   
   return rc;
   
}

void dn_ipmg_sendIP_reply(uint8_t cmdId, uint8_t rc, uint8_t* payload, uint8_t len) {
   dn_ipmg_sendIP_rpt* reply;
   
   // verify I'm expecting this answer
   if (dn_ipmg_vars.busyTx==FALSE || dn_ipmg_vars.cmdId!=cmdId) {
      return;
   }
   
   // verify length
   if (rc==DN_SERIAL_RC_OK && len<DN_SENDIP_REPLY_LEN) {
      return;
   }
   
   // cast the replyContent
   reply = (dn_ipmg_sendIP_rpt*)dn_ipmg_vars.replyContents;
   
   // store RC
   reply->RC = rc;
   
   // parse returned value (iff RC==0)
   if (rc==DN_SERIAL_RC_OK) {
      
      dn_read_uint32_t(&reply->callbackId,&payload[DN_SENDIP_REPLY_OFFS_CALLBACKID]);
   }
   
   // call the callback
   dn_ipmg_vars.replyCb(cmdId);
   
   // I'm not busy transmitting anymore
   dn_ipmg_vars.busyTx=FALSE;
}

//===== restoreFactoryDefaults

/**
The restoreFactoryDefaults command restores the default configuration and 
clears the ACL. This command does not affect the license.This change is 
persistent. 
*/
dn_err_t dn_ipmg_restoreFactoryDefaults(dn_ipmg_restoreFactoryDefaults_rpt* reply) {
   uint8_t    extraFlags;
   dn_err_t   rc;
   
   // lock the module
   dn_lock();
   
   // verify no ongoing transmissions
   if (dn_ipmg_vars.busyTx) {
      // unlock the module
      dn_unlock();
      
      // return
      return DN_ERR_BUSY;
   }
   
   // store callback information
   dn_ipmg_vars.cmdId          = CMDID_RESTOREFACTORYDEFAULTS;
   dn_ipmg_vars.replyContents  = (uint8_t*)reply;
   
   // extraFlags
   extraFlags = 0x00;
   
   // build outputBuf
   
   // send outputBuf
   rc = dn_serial_mg_sendRequest(
      CMDID_RESTOREFACTORYDEFAULTS,                             // cmdId
      extraFlags,                                               // extraFlags
      dn_ipmg_vars.outputBuf,                                   // payload
      DN_RESTOREFACTORYDEFAULTS_REQ_LEN,                        // length
      dn_ipmg_restoreFactoryDefaults_reply                      // replyCb
   );
   
   if (rc==DN_ERR_NONE) {
      // I'm now busy transmitting
      dn_ipmg_vars.busyTx         = TRUE;
   }
   
   // unlock the module
   dn_unlock();
   
   return rc;
   
}

void dn_ipmg_restoreFactoryDefaults_reply(uint8_t cmdId, uint8_t rc, uint8_t* payload, uint8_t len) {
   dn_ipmg_restoreFactoryDefaults_rpt* reply;
   
   // verify I'm expecting this answer
   if (dn_ipmg_vars.busyTx==FALSE || dn_ipmg_vars.cmdId!=cmdId) {
      return;
   }
   
   // do NOT verify length (no return fields expected)
   
   // cast the replyContent
   reply = (dn_ipmg_restoreFactoryDefaults_rpt*)dn_ipmg_vars.replyContents;
   
   // store RC
   reply->RC = rc;
   
   // parse returned value (iff RC==0)
   if (rc==DN_SERIAL_RC_OK) {
      
   }
   
   // call the callback
   dn_ipmg_vars.replyCb(cmdId);
   
   // I'm not busy transmitting anymore
   dn_ipmg_vars.busyTx=FALSE;
}

//===== getMoteInfo

/**
The getMoteInfo command returns dynamic information for the specified mote. 
*/
dn_err_t dn_ipmg_getMoteInfo(uint8_t* macAddress, dn_ipmg_getMoteInfo_rpt* reply) {
   uint8_t    extraFlags;
   dn_err_t   rc;
   
   // lock the module
   dn_lock();
   
   // verify no ongoing transmissions
   if (dn_ipmg_vars.busyTx) {
      // unlock the module
      dn_unlock();
      
      // return
      return DN_ERR_BUSY;
   }
   
   // store callback information
   dn_ipmg_vars.cmdId          = CMDID_GETMOTEINFO;
   dn_ipmg_vars.replyContents  = (uint8_t*)reply;
   
   // extraFlags
   extraFlags = 0x00;
   
   // build outputBuf
   memcpy(&dn_ipmg_vars.outputBuf[DN_GETMOTEINFO_REQ_OFFS_MACADDRESS],macAddress,8);
   
   // send outputBuf
   rc = dn_serial_mg_sendRequest(
      CMDID_GETMOTEINFO,                                        // cmdId
      extraFlags,                                               // extraFlags
      dn_ipmg_vars.outputBuf,                                   // payload
      DN_GETMOTEINFO_REQ_LEN,                                   // length
      dn_ipmg_getMoteInfo_reply                                 // replyCb
   );
   
   if (rc==DN_ERR_NONE) {
      // I'm now busy transmitting
      dn_ipmg_vars.busyTx         = TRUE;
   }
   
   // unlock the module
   dn_unlock();
   
   return rc;
   
}

void dn_ipmg_getMoteInfo_reply(uint8_t cmdId, uint8_t rc, uint8_t* payload, uint8_t len) {
   dn_ipmg_getMoteInfo_rpt* reply;
   
   // verify I'm expecting this answer
   if (dn_ipmg_vars.busyTx==FALSE || dn_ipmg_vars.cmdId!=cmdId) {
      return;
   }
   
   // verify length
   if (rc==DN_SERIAL_RC_OK && len<DN_GETMOTEINFO_REPLY_LEN) {
      return;
   }
   
   // cast the replyContent
   reply = (dn_ipmg_getMoteInfo_rpt*)dn_ipmg_vars.replyContents;
   
   // store RC
   reply->RC = rc;
   
   // parse returned value (iff RC==0)
   if (rc==DN_SERIAL_RC_OK) {
      
      memcpy(&reply->macAddress[0],&payload[DN_GETMOTEINFO_REPLY_OFFS_MACADDRESS],8);
      reply->state = payload[DN_GETMOTEINFO_REPLY_OFFS_STATE];
      reply->numNbrs = payload[DN_GETMOTEINFO_REPLY_OFFS_NUMNBRS];
      reply->numGoodNbrs = payload[DN_GETMOTEINFO_REPLY_OFFS_NUMGOODNBRS];
      dn_read_uint32_t(&reply->requestedBw,&payload[DN_GETMOTEINFO_REPLY_OFFS_REQUESTEDBW]);
      dn_read_uint32_t(&reply->totalNeededBw,&payload[DN_GETMOTEINFO_REPLY_OFFS_TOTALNEEDEDBW]);
      dn_read_uint32_t(&reply->assignedBw,&payload[DN_GETMOTEINFO_REPLY_OFFS_ASSIGNEDBW]);
      dn_read_uint32_t(&reply->packetsReceived,&payload[DN_GETMOTEINFO_REPLY_OFFS_PACKETSRECEIVED]);
      dn_read_uint32_t(&reply->packetsLost,&payload[DN_GETMOTEINFO_REPLY_OFFS_PACKETSLOST]);
      dn_read_uint32_t(&reply->avgLatency,&payload[DN_GETMOTEINFO_REPLY_OFFS_AVGLATENCY]);
   }
   
   // call the callback
   dn_ipmg_vars.replyCb(cmdId);
   
   // I'm not busy transmitting anymore
   dn_ipmg_vars.busyTx=FALSE;
}

//===== getNetworkConfig

/**
The getNetworkConfig command returns general network configuration parameters, 
including the Network ID, bandwidth parameters and number of motes. 
*/
dn_err_t dn_ipmg_getNetworkConfig(dn_ipmg_getNetworkConfig_rpt* reply) {
   uint8_t    extraFlags;
   dn_err_t   rc;
   
   // lock the module
   dn_lock();
   
   // verify no ongoing transmissions
   if (dn_ipmg_vars.busyTx) {
      // unlock the module
      dn_unlock();
      
      // return
      return DN_ERR_BUSY;
   }
   
   // store callback information
   dn_ipmg_vars.cmdId          = CMDID_GETNETWORKCONFIG;
   dn_ipmg_vars.replyContents  = (uint8_t*)reply;
   
   // extraFlags
   extraFlags = 0x00;
   
   // build outputBuf
   
   // send outputBuf
   rc = dn_serial_mg_sendRequest(
      CMDID_GETNETWORKCONFIG,                                   // cmdId
      extraFlags,                                               // extraFlags
      dn_ipmg_vars.outputBuf,                                   // payload
      DN_GETNETWORKCONFIG_REQ_LEN,                              // length
      dn_ipmg_getNetworkConfig_reply                            // replyCb
   );
   
   if (rc==DN_ERR_NONE) {
      // I'm now busy transmitting
      dn_ipmg_vars.busyTx         = TRUE;
   }
   
   // unlock the module
   dn_unlock();
   
   return rc;
   
}

void dn_ipmg_getNetworkConfig_reply(uint8_t cmdId, uint8_t rc, uint8_t* payload, uint8_t len) {
   dn_ipmg_getNetworkConfig_rpt* reply;
   
   // verify I'm expecting this answer
   if (dn_ipmg_vars.busyTx==FALSE || dn_ipmg_vars.cmdId!=cmdId) {
      return;
   }
   
   // verify length
   if (rc==DN_SERIAL_RC_OK && len<DN_GETNETWORKCONFIG_REPLY_LEN) {
      return;
   }
   
   // cast the replyContent
   reply = (dn_ipmg_getNetworkConfig_rpt*)dn_ipmg_vars.replyContents;
   
   // store RC
   reply->RC = rc;
   
   // parse returned value (iff RC==0)
   if (rc==DN_SERIAL_RC_OK) {
      
      dn_read_uint16_t(&reply->networkId,&payload[DN_GETNETWORKCONFIG_REPLY_OFFS_NETWORKID]);
      reply->apTxPower = (int8_t)payload[DN_GETNETWORKCONFIG_REPLY_OFFS_APTXPOWER];
      reply->frameProfile = payload[DN_GETNETWORKCONFIG_REPLY_OFFS_FRAMEPROFILE];
      dn_read_uint16_t(&reply->maxMotes,&payload[DN_GETNETWORKCONFIG_REPLY_OFFS_MAXMOTES]);
      dn_read_uint16_t(&reply->baseBandwidth,&payload[DN_GETNETWORKCONFIG_REPLY_OFFS_BASEBANDWIDTH]);
      reply->downFrameMultVal = payload[DN_GETNETWORKCONFIG_REPLY_OFFS_DOWNFRAMEMULTVAL];
      reply->numParents = payload[DN_GETNETWORKCONFIG_REPLY_OFFS_NUMPARENTS];
      reply->ccaMode = payload[DN_GETNETWORKCONFIG_REPLY_OFFS_CCAMODE];
      dn_read_uint16_t(&reply->channelList,&payload[DN_GETNETWORKCONFIG_REPLY_OFFS_CHANNELLIST]);
      reply->autoStartNetwork = payload[DN_GETNETWORKCONFIG_REPLY_OFFS_AUTOSTARTNETWORK];
      reply->locMode = payload[DN_GETNETWORKCONFIG_REPLY_OFFS_LOCMODE];
      reply->bbMode = payload[DN_GETNETWORKCONFIG_REPLY_OFFS_BBMODE];
      reply->bbSize = payload[DN_GETNETWORKCONFIG_REPLY_OFFS_BBSIZE];
      reply->isRadioTest = payload[DN_GETNETWORKCONFIG_REPLY_OFFS_ISRADIOTEST];
      dn_read_uint16_t(&reply->bwMult,&payload[DN_GETNETWORKCONFIG_REPLY_OFFS_BWMULT]);
      reply->oneChannel = payload[DN_GETNETWORKCONFIG_REPLY_OFFS_ONECHANNEL];
   }
   
   // call the callback
   dn_ipmg_vars.replyCb(cmdId);
   
   // I'm not busy transmitting anymore
   dn_ipmg_vars.busyTx=FALSE;
}

//===== getNetworkInfo

/**
The getNetworkInfo command returns dynamic network information and statistics. 
*/
dn_err_t dn_ipmg_getNetworkInfo(dn_ipmg_getNetworkInfo_rpt* reply) {
   uint8_t    extraFlags;
   dn_err_t   rc;
   
   // lock the module
   dn_lock();
   
   // verify no ongoing transmissions
   if (dn_ipmg_vars.busyTx) {
      // unlock the module
      dn_unlock();
      
      // return
      return DN_ERR_BUSY;
   }
   
   // store callback information
   dn_ipmg_vars.cmdId          = CMDID_GETNETWORKINFO;
   dn_ipmg_vars.replyContents  = (uint8_t*)reply;
   
   // extraFlags
   extraFlags = 0x00;
   
   // build outputBuf
   
   // send outputBuf
   rc = dn_serial_mg_sendRequest(
      CMDID_GETNETWORKINFO,                                     // cmdId
      extraFlags,                                               // extraFlags
      dn_ipmg_vars.outputBuf,                                   // payload
      DN_GETNETWORKINFO_REQ_LEN,                                // length
      dn_ipmg_getNetworkInfo_reply                              // replyCb
   );
   
   if (rc==DN_ERR_NONE) {
      // I'm now busy transmitting
      dn_ipmg_vars.busyTx         = TRUE;
   }
   
   // unlock the module
   dn_unlock();
   
   return rc;
   
}

void dn_ipmg_getNetworkInfo_reply(uint8_t cmdId, uint8_t rc, uint8_t* payload, uint8_t len) {
   dn_ipmg_getNetworkInfo_rpt* reply;
   
   // verify I'm expecting this answer
   if (dn_ipmg_vars.busyTx==FALSE || dn_ipmg_vars.cmdId!=cmdId) {
      return;
   }
   
   // verify length
   if (rc==DN_SERIAL_RC_OK && len<DN_GETNETWORKINFO_REPLY_LEN) {
      return;
   }
   
   // cast the replyContent
   reply = (dn_ipmg_getNetworkInfo_rpt*)dn_ipmg_vars.replyContents;
   
   // store RC
   reply->RC = rc;
   
   // parse returned value (iff RC==0)
   if (rc==DN_SERIAL_RC_OK) {
      
      dn_read_uint16_t(&reply->numMotes,&payload[DN_GETNETWORKINFO_REPLY_OFFS_NUMMOTES]);
      dn_read_uint16_t(&reply->asnSize,&payload[DN_GETNETWORKINFO_REPLY_OFFS_ASNSIZE]);
      reply->advertisementState = payload[DN_GETNETWORKINFO_REPLY_OFFS_ADVERTISEMENTSTATE];
      reply->downFrameState = payload[DN_GETNETWORKINFO_REPLY_OFFS_DOWNFRAMESTATE];
      reply->netReliability = payload[DN_GETNETWORKINFO_REPLY_OFFS_NETRELIABILITY];
      reply->netPathStability = payload[DN_GETNETWORKINFO_REPLY_OFFS_NETPATHSTABILITY];
      dn_read_uint32_t(&reply->netLatency,&payload[DN_GETNETWORKINFO_REPLY_OFFS_NETLATENCY]);
      reply->netState = payload[DN_GETNETWORKINFO_REPLY_OFFS_NETSTATE];
      memcpy(&reply->ipv6Address[0],&payload[DN_GETNETWORKINFO_REPLY_OFFS_IPV6ADDRESS],16);
   }
   
   // call the callback
   dn_ipmg_vars.replyCb(cmdId);
   
   // I'm not busy transmitting anymore
   dn_ipmg_vars.busyTx=FALSE;
}

//===== getMoteConfigById

/**
The getMoteConfigById command returns a single mote description as the 
response. The command takes one argument, the short address of a mote (Mote 
ID). The command returns the same response structure as the getMoteConfig 
command. 
*/
dn_err_t dn_ipmg_getMoteConfigById(uint16_t moteId, dn_ipmg_getMoteConfigById_rpt* reply) {
   uint8_t    extraFlags;
   dn_err_t   rc;
   
   // lock the module
   dn_lock();
   
   // verify no ongoing transmissions
   if (dn_ipmg_vars.busyTx) {
      // unlock the module
      dn_unlock();
      
      // return
      return DN_ERR_BUSY;
   }
   
   // store callback information
   dn_ipmg_vars.cmdId          = CMDID_GETMOTECONFIGBYID;
   dn_ipmg_vars.replyContents  = (uint8_t*)reply;
   
   // extraFlags
   extraFlags = 0x00;
   
   // build outputBuf
   dn_write_uint16_t(&dn_ipmg_vars.outputBuf[DN_GETMOTECONFIGBYID_REQ_OFFS_MOTEID],moteId);
   
   // send outputBuf
   rc = dn_serial_mg_sendRequest(
      CMDID_GETMOTECONFIGBYID,                                  // cmdId
      extraFlags,                                               // extraFlags
      dn_ipmg_vars.outputBuf,                                   // payload
      DN_GETMOTECONFIGBYID_REQ_LEN,                             // length
      dn_ipmg_getMoteConfigById_reply                           // replyCb
   );
   
   if (rc==DN_ERR_NONE) {
      // I'm now busy transmitting
      dn_ipmg_vars.busyTx         = TRUE;
   }
   
   // unlock the module
   dn_unlock();
   
   return rc;
   
}

void dn_ipmg_getMoteConfigById_reply(uint8_t cmdId, uint8_t rc, uint8_t* payload, uint8_t len) {
   dn_ipmg_getMoteConfigById_rpt* reply;
   
   // verify I'm expecting this answer
   if (dn_ipmg_vars.busyTx==FALSE || dn_ipmg_vars.cmdId!=cmdId) {
      return;
   }
   
   // verify length
   if (rc==DN_SERIAL_RC_OK && len<DN_GETMOTECONFIGBYID_REPLY_LEN) {
      return;
   }
   
   // cast the replyContent
   reply = (dn_ipmg_getMoteConfigById_rpt*)dn_ipmg_vars.replyContents;
   
   // store RC
   reply->RC = rc;
   
   // parse returned value (iff RC==0)
   if (rc==DN_SERIAL_RC_OK) {
      
      memcpy(&reply->macAddress[0],&payload[DN_GETMOTECONFIGBYID_REPLY_OFFS_MACADDRESS],8);
      dn_read_uint16_t(&reply->moteId,&payload[DN_GETMOTECONFIGBYID_REPLY_OFFS_MOTEID]);
      reply->isAP = payload[DN_GETMOTECONFIGBYID_REPLY_OFFS_ISAP];
      reply->state = payload[DN_GETMOTECONFIGBYID_REPLY_OFFS_STATE];
      reply->reserved = payload[DN_GETMOTECONFIGBYID_REPLY_OFFS_RESERVED];
      reply->isRouting = payload[DN_GETMOTECONFIGBYID_REPLY_OFFS_ISROUTING];
   }
   
   // call the callback
   dn_ipmg_vars.replyCb(cmdId);
   
   // I'm not busy transmitting anymore
   dn_ipmg_vars.busyTx=FALSE;
}

//===== setCommonJoinKey

/**
The setCommonJoinKey command will set a new value for the common join key. The 
common join key is used to decrypt join messages only if the ACL is empty. 
*/
dn_err_t dn_ipmg_setCommonJoinKey(uint8_t* key, dn_ipmg_setCommonJoinKey_rpt* reply) {
   uint8_t    extraFlags;
   dn_err_t   rc;
   
   // lock the module
   dn_lock();
   
   // verify no ongoing transmissions
   if (dn_ipmg_vars.busyTx) {
      // unlock the module
      dn_unlock();
      
      // return
      return DN_ERR_BUSY;
   }
   
   // store callback information
   dn_ipmg_vars.cmdId          = CMDID_SETCOMMONJOINKEY;
   dn_ipmg_vars.replyContents  = (uint8_t*)reply;
   
   // extraFlags
   extraFlags = 0x00;
   
   // build outputBuf
   memcpy(&dn_ipmg_vars.outputBuf[DN_SETCOMMONJOINKEY_REQ_OFFS_KEY],key,16);
   
   // send outputBuf
   rc = dn_serial_mg_sendRequest(
      CMDID_SETCOMMONJOINKEY,                                   // cmdId
      extraFlags,                                               // extraFlags
      dn_ipmg_vars.outputBuf,                                   // payload
      DN_SETCOMMONJOINKEY_REQ_LEN,                              // length
      dn_ipmg_setCommonJoinKey_reply                            // replyCb
   );
   
   if (rc==DN_ERR_NONE) {
      // I'm now busy transmitting
      dn_ipmg_vars.busyTx         = TRUE;
   }
   
   // unlock the module
   dn_unlock();
   
   return rc;
   
}

void dn_ipmg_setCommonJoinKey_reply(uint8_t cmdId, uint8_t rc, uint8_t* payload, uint8_t len) {
   dn_ipmg_setCommonJoinKey_rpt* reply;
   
   // verify I'm expecting this answer
   if (dn_ipmg_vars.busyTx==FALSE || dn_ipmg_vars.cmdId!=cmdId) {
      return;
   }
   
   // do NOT verify length (no return fields expected)
   
   // cast the replyContent
   reply = (dn_ipmg_setCommonJoinKey_rpt*)dn_ipmg_vars.replyContents;
   
   // store RC
   reply->RC = rc;
   
   // parse returned value (iff RC==0)
   if (rc==DN_SERIAL_RC_OK) {
      
   }
   
   // call the callback
   dn_ipmg_vars.replyCb(cmdId);
   
   // I'm not busy transmitting anymore
   dn_ipmg_vars.busyTx=FALSE;
}

//===== getIPConfig

/**
The getIPConfig command returns the manager's IP configuration parameters, 
including the IPv6 address and mask. 
*/
dn_err_t dn_ipmg_getIPConfig(dn_ipmg_getIPConfig_rpt* reply) {
   uint8_t    extraFlags;
   dn_err_t   rc;
   
   // lock the module
   dn_lock();
   
   // verify no ongoing transmissions
   if (dn_ipmg_vars.busyTx) {
      // unlock the module
      dn_unlock();
      
      // return
      return DN_ERR_BUSY;
   }
   
   // store callback information
   dn_ipmg_vars.cmdId          = CMDID_GETIPCONFIG;
   dn_ipmg_vars.replyContents  = (uint8_t*)reply;
   
   // extraFlags
   extraFlags = 0x00;
   
   // build outputBuf
   
   // send outputBuf
   rc = dn_serial_mg_sendRequest(
      CMDID_GETIPCONFIG,                                        // cmdId
      extraFlags,                                               // extraFlags
      dn_ipmg_vars.outputBuf,                                   // payload
      DN_GETIPCONFIG_REQ_LEN,                                   // length
      dn_ipmg_getIPConfig_reply                                 // replyCb
   );
   
   if (rc==DN_ERR_NONE) {
      // I'm now busy transmitting
      dn_ipmg_vars.busyTx         = TRUE;
   }
   
   // unlock the module
   dn_unlock();
   
   return rc;
   
}

void dn_ipmg_getIPConfig_reply(uint8_t cmdId, uint8_t rc, uint8_t* payload, uint8_t len) {
   dn_ipmg_getIPConfig_rpt* reply;
   
   // verify I'm expecting this answer
   if (dn_ipmg_vars.busyTx==FALSE || dn_ipmg_vars.cmdId!=cmdId) {
      return;
   }
   
   // verify length
   if (rc==DN_SERIAL_RC_OK && len<DN_GETIPCONFIG_REPLY_LEN) {
      return;
   }
   
   // cast the replyContent
   reply = (dn_ipmg_getIPConfig_rpt*)dn_ipmg_vars.replyContents;
   
   // store RC
   reply->RC = rc;
   
   // parse returned value (iff RC==0)
   if (rc==DN_SERIAL_RC_OK) {
      
      memcpy(&reply->ipv6Address[0],&payload[DN_GETIPCONFIG_REPLY_OFFS_IPV6ADDRESS],16);
      memcpy(&reply->mask[0],&payload[DN_GETIPCONFIG_REPLY_OFFS_MASK],16);
   }
   
   // call the callback
   dn_ipmg_vars.replyCb(cmdId);
   
   // I'm not busy transmitting anymore
   dn_ipmg_vars.busyTx=FALSE;
}

//===== setIPConfig

/**
The setIPConfig command sets the IPv6 prefix of the mesh network. Only the 
upper 8 bytes of the IPv6 address are relevant: the lower 8 bytes of the IPv6 
address are ignored, and lower 8 bytes of the mask field are reserved and 
should be set to 0.This change is persistent. 
*/
dn_err_t dn_ipmg_setIPConfig(uint8_t* ipv6Address, uint8_t* mask, dn_ipmg_setIPConfig_rpt* reply) {
   uint8_t    extraFlags;
   dn_err_t   rc;
   
   // lock the module
   dn_lock();
   
   // verify no ongoing transmissions
   if (dn_ipmg_vars.busyTx) {
      // unlock the module
      dn_unlock();
      
      // return
      return DN_ERR_BUSY;
   }
   
   // store callback information
   dn_ipmg_vars.cmdId          = CMDID_SETIPCONFIG;
   dn_ipmg_vars.replyContents  = (uint8_t*)reply;
   
   // extraFlags
   extraFlags = 0x00;
   
   // build outputBuf
   memcpy(&dn_ipmg_vars.outputBuf[DN_SETIPCONFIG_REQ_OFFS_IPV6ADDRESS],ipv6Address,16);
   memcpy(&dn_ipmg_vars.outputBuf[DN_SETIPCONFIG_REQ_OFFS_MASK],mask,16);
   
   // send outputBuf
   rc = dn_serial_mg_sendRequest(
      CMDID_SETIPCONFIG,                                        // cmdId
      extraFlags,                                               // extraFlags
      dn_ipmg_vars.outputBuf,                                   // payload
      DN_SETIPCONFIG_REQ_LEN,                                   // length
      dn_ipmg_setIPConfig_reply                                 // replyCb
   );
   
   if (rc==DN_ERR_NONE) {
      // I'm now busy transmitting
      dn_ipmg_vars.busyTx         = TRUE;
   }
   
   // unlock the module
   dn_unlock();
   
   return rc;
   
}

void dn_ipmg_setIPConfig_reply(uint8_t cmdId, uint8_t rc, uint8_t* payload, uint8_t len) {
   dn_ipmg_setIPConfig_rpt* reply;
   
   // verify I'm expecting this answer
   if (dn_ipmg_vars.busyTx==FALSE || dn_ipmg_vars.cmdId!=cmdId) {
      return;
   }
   
   // do NOT verify length (no return fields expected)
   
   // cast the replyContent
   reply = (dn_ipmg_setIPConfig_rpt*)dn_ipmg_vars.replyContents;
   
   // store RC
   reply->RC = rc;
   
   // parse returned value (iff RC==0)
   if (rc==DN_SERIAL_RC_OK) {
      
   }
   
   // call the callback
   dn_ipmg_vars.replyCb(cmdId);
   
   // I'm not busy transmitting anymore
   dn_ipmg_vars.busyTx=FALSE;
}

//===== deleteMote

/**
The deleteMote command deletes a mote from the manager's list. A mote can only 
be deleted if it in the Lost or Unknown states. This change is persistent. 
*/
dn_err_t dn_ipmg_deleteMote(uint8_t* macAddress, dn_ipmg_deleteMote_rpt* reply) {
   uint8_t    extraFlags;
   dn_err_t   rc;
   
   // lock the module
   dn_lock();
   
   // verify no ongoing transmissions
   if (dn_ipmg_vars.busyTx) {
      // unlock the module
      dn_unlock();
      
      // return
      return DN_ERR_BUSY;
   }
   
   // store callback information
   dn_ipmg_vars.cmdId          = CMDID_DELETEMOTE;
   dn_ipmg_vars.replyContents  = (uint8_t*)reply;
   
   // extraFlags
   extraFlags = 0x00;
   
   // build outputBuf
   memcpy(&dn_ipmg_vars.outputBuf[DN_DELETEMOTE_REQ_OFFS_MACADDRESS],macAddress,8);
   
   // send outputBuf
   rc = dn_serial_mg_sendRequest(
      CMDID_DELETEMOTE,                                         // cmdId
      extraFlags,                                               // extraFlags
      dn_ipmg_vars.outputBuf,                                   // payload
      DN_DELETEMOTE_REQ_LEN,                                    // length
      dn_ipmg_deleteMote_reply                                  // replyCb
   );
   
   if (rc==DN_ERR_NONE) {
      // I'm now busy transmitting
      dn_ipmg_vars.busyTx         = TRUE;
   }
   
   // unlock the module
   dn_unlock();
   
   return rc;
   
}

void dn_ipmg_deleteMote_reply(uint8_t cmdId, uint8_t rc, uint8_t* payload, uint8_t len) {
   dn_ipmg_deleteMote_rpt* reply;
   
   // verify I'm expecting this answer
   if (dn_ipmg_vars.busyTx==FALSE || dn_ipmg_vars.cmdId!=cmdId) {
      return;
   }
   
   // do NOT verify length (no return fields expected)
   
   // cast the replyContent
   reply = (dn_ipmg_deleteMote_rpt*)dn_ipmg_vars.replyContents;
   
   // store RC
   reply->RC = rc;
   
   // parse returned value (iff RC==0)
   if (rc==DN_SERIAL_RC_OK) {
      
   }
   
   // call the callback
   dn_ipmg_vars.replyCb(cmdId);
   
   // I'm not busy transmitting anymore
   dn_ipmg_vars.busyTx=FALSE;
}

//===== getMoteLinks

/**
The getMoteLinks command returns information about links assigned to the mote. 
The response contains a list of links starting with Nth link on the mote, where 
N is supplied as the idx parameter in the request. To retrieve all links on the 
device the user can call this command with idx that increments by number of 
links returned with priorresponse, until the command returns RC_END_OF_LIST 
response code. Note that links assigned to a mote may change between API calls. 
*/
dn_err_t dn_ipmg_getMoteLinks(uint8_t* macAddress, uint16_t idx, dn_ipmg_getMoteLinks_rpt* reply) {
   uint8_t    extraFlags;
   dn_err_t   rc;
   
   // lock the module
   dn_lock();
   
   // verify no ongoing transmissions
   if (dn_ipmg_vars.busyTx) {
      // unlock the module
      dn_unlock();
      
      // return
      return DN_ERR_BUSY;
   }
   
   // store callback information
   dn_ipmg_vars.cmdId          = CMDID_GETMOTELINKS;
   dn_ipmg_vars.replyContents  = (uint8_t*)reply;
   
   // extraFlags
   extraFlags = 0x00;
   
   // build outputBuf
   memcpy(&dn_ipmg_vars.outputBuf[DN_GETMOTELINKS_REQ_OFFS_MACADDRESS],macAddress,8);
   dn_write_uint16_t(&dn_ipmg_vars.outputBuf[DN_GETMOTELINKS_REQ_OFFS_IDX],idx);
   
   // send outputBuf
   rc = dn_serial_mg_sendRequest(
      CMDID_GETMOTELINKS,                                       // cmdId
      extraFlags,                                               // extraFlags
      dn_ipmg_vars.outputBuf,                                   // payload
      DN_GETMOTELINKS_REQ_LEN,                                  // length
      dn_ipmg_getMoteLinks_reply                                // replyCb
   );
   
   if (rc==DN_ERR_NONE) {
      // I'm now busy transmitting
      dn_ipmg_vars.busyTx         = TRUE;
   }
   
   // unlock the module
   dn_unlock();
   
   return rc;
   
}

void dn_ipmg_getMoteLinks_reply(uint8_t cmdId, uint8_t rc, uint8_t* payload, uint8_t len) {
   dn_ipmg_getMoteLinks_rpt* reply;
   
   // verify I'm expecting this answer
   if (dn_ipmg_vars.busyTx==FALSE || dn_ipmg_vars.cmdId!=cmdId) {
      return;
   }
   
   // verify length
   if (rc==DN_SERIAL_RC_OK && len<DN_GETMOTELINKS_REPLY_LEN) {
      return;
   }
   
   // cast the replyContent
   reply = (dn_ipmg_getMoteLinks_rpt*)dn_ipmg_vars.replyContents;
   
   // store RC
   reply->RC = rc;
   
   // parse returned value (iff RC==0)
   if (rc==DN_SERIAL_RC_OK) {
      
      dn_read_uint16_t(&reply->idx,&payload[DN_GETMOTELINKS_REPLY_OFFS_IDX]);
      reply->utilization = payload[DN_GETMOTELINKS_REPLY_OFFS_UTILIZATION];
      reply->numLinks = payload[DN_GETMOTELINKS_REPLY_OFFS_NUMLINKS];
      reply->frameId_1 = payload[DN_GETMOTELINKS_REPLY_OFFS_FRAMEID_1];
      dn_read_uint32_t(&reply->slot_1,&payload[DN_GETMOTELINKS_REPLY_OFFS_SLOT_1]);
      reply->channelOffset_1 = payload[DN_GETMOTELINKS_REPLY_OFFS_CHANNELOFFSET_1];
      dn_read_uint16_t(&reply->moteId_1,&payload[DN_GETMOTELINKS_REPLY_OFFS_MOTEID_1]);
      reply->flags_1 = payload[DN_GETMOTELINKS_REPLY_OFFS_FLAGS_1];
      reply->frameId_2 = payload[DN_GETMOTELINKS_REPLY_OFFS_FRAMEID_2];
      dn_read_uint32_t(&reply->slot_2,&payload[DN_GETMOTELINKS_REPLY_OFFS_SLOT_2]);
      reply->channelOffset_2 = payload[DN_GETMOTELINKS_REPLY_OFFS_CHANNELOFFSET_2];
      dn_read_uint16_t(&reply->moteId_2,&payload[DN_GETMOTELINKS_REPLY_OFFS_MOTEID_2]);
      reply->flags_2 = payload[DN_GETMOTELINKS_REPLY_OFFS_FLAGS_2];
      reply->frameId_3 = payload[DN_GETMOTELINKS_REPLY_OFFS_FRAMEID_3];
      dn_read_uint32_t(&reply->slot_3,&payload[DN_GETMOTELINKS_REPLY_OFFS_SLOT_3]);
      reply->channelOffset_3 = payload[DN_GETMOTELINKS_REPLY_OFFS_CHANNELOFFSET_3];
      dn_read_uint16_t(&reply->moteId_3,&payload[DN_GETMOTELINKS_REPLY_OFFS_MOTEID_3]);
      reply->flags_3 = payload[DN_GETMOTELINKS_REPLY_OFFS_FLAGS_3];
      reply->frameId_4 = payload[DN_GETMOTELINKS_REPLY_OFFS_FRAMEID_4];
      dn_read_uint32_t(&reply->slot_4,&payload[DN_GETMOTELINKS_REPLY_OFFS_SLOT_4]);
      reply->channelOffset_4 = payload[DN_GETMOTELINKS_REPLY_OFFS_CHANNELOFFSET_4];
      dn_read_uint16_t(&reply->moteId_4,&payload[DN_GETMOTELINKS_REPLY_OFFS_MOTEID_4]);
      reply->flags_4 = payload[DN_GETMOTELINKS_REPLY_OFFS_FLAGS_4];
      reply->frameId_5 = payload[DN_GETMOTELINKS_REPLY_OFFS_FRAMEID_5];
      dn_read_uint32_t(&reply->slot_5,&payload[DN_GETMOTELINKS_REPLY_OFFS_SLOT_5]);
      reply->channelOffset_5 = payload[DN_GETMOTELINKS_REPLY_OFFS_CHANNELOFFSET_5];
      dn_read_uint16_t(&reply->moteId_5,&payload[DN_GETMOTELINKS_REPLY_OFFS_MOTEID_5]);
      reply->flags_5 = payload[DN_GETMOTELINKS_REPLY_OFFS_FLAGS_5];
      reply->frameId_6 = payload[DN_GETMOTELINKS_REPLY_OFFS_FRAMEID_6];
      dn_read_uint32_t(&reply->slot_6,&payload[DN_GETMOTELINKS_REPLY_OFFS_SLOT_6]);
      reply->channelOffset_6 = payload[DN_GETMOTELINKS_REPLY_OFFS_CHANNELOFFSET_6];
      dn_read_uint16_t(&reply->moteId_6,&payload[DN_GETMOTELINKS_REPLY_OFFS_MOTEID_6]);
      reply->flags_6 = payload[DN_GETMOTELINKS_REPLY_OFFS_FLAGS_6];
      reply->frameId_7 = payload[DN_GETMOTELINKS_REPLY_OFFS_FRAMEID_7];
      dn_read_uint32_t(&reply->slot_7,&payload[DN_GETMOTELINKS_REPLY_OFFS_SLOT_7]);
      reply->channelOffset_7 = payload[DN_GETMOTELINKS_REPLY_OFFS_CHANNELOFFSET_7];
      dn_read_uint16_t(&reply->moteId_7,&payload[DN_GETMOTELINKS_REPLY_OFFS_MOTEID_7]);
      reply->flags_7 = payload[DN_GETMOTELINKS_REPLY_OFFS_FLAGS_7];
      reply->frameId_8 = payload[DN_GETMOTELINKS_REPLY_OFFS_FRAMEID_8];
      dn_read_uint32_t(&reply->slot_8,&payload[DN_GETMOTELINKS_REPLY_OFFS_SLOT_8]);
      reply->channelOffset_8 = payload[DN_GETMOTELINKS_REPLY_OFFS_CHANNELOFFSET_8];
      dn_read_uint16_t(&reply->moteId_8,&payload[DN_GETMOTELINKS_REPLY_OFFS_MOTEID_8]);
      reply->flags_8 = payload[DN_GETMOTELINKS_REPLY_OFFS_FLAGS_8];
      reply->frameId_9 = payload[DN_GETMOTELINKS_REPLY_OFFS_FRAMEID_9];
      dn_read_uint32_t(&reply->slot_9,&payload[DN_GETMOTELINKS_REPLY_OFFS_SLOT_9]);
      reply->channelOffset_9 = payload[DN_GETMOTELINKS_REPLY_OFFS_CHANNELOFFSET_9];
      dn_read_uint16_t(&reply->moteId_9,&payload[DN_GETMOTELINKS_REPLY_OFFS_MOTEID_9]);
      reply->flags_9 = payload[DN_GETMOTELINKS_REPLY_OFFS_FLAGS_9];
      reply->frameId_10 = payload[DN_GETMOTELINKS_REPLY_OFFS_FRAMEID_10];
      dn_read_uint32_t(&reply->slot_10,&payload[DN_GETMOTELINKS_REPLY_OFFS_SLOT_10]);
      reply->channelOffset_10 = payload[DN_GETMOTELINKS_REPLY_OFFS_CHANNELOFFSET_10];
      dn_read_uint16_t(&reply->moteId_10,&payload[DN_GETMOTELINKS_REPLY_OFFS_MOTEID_10]);
      reply->flags_10 = payload[DN_GETMOTELINKS_REPLY_OFFS_FLAGS_10];
   }
   
   // call the callback
   dn_ipmg_vars.replyCb(cmdId);
   
   // I'm not busy transmitting anymore
   dn_ipmg_vars.busyTx=FALSE;
}

//========== serialRX

void dn_ipmg_rxSerialRequest(uint8_t cmdId, uint8_t flags, uint8_t* payload, uint8_t len) {
   
   dn_ipmg_notifLog_nt* notif_notifLog;
   dn_ipmg_notifData_nt* notif_notifData;
   dn_ipmg_notifIpData_nt* notif_notifIpData;
   dn_ipmg_notifHealthReport_nt* notif_notifHealthReport;
   dn_ipmg_eventMoteReset_nt* notif_eventMoteReset;
   dn_ipmg_eventNetworkReset_nt* notif_eventNetworkReset;
   dn_ipmg_eventCommandFinished_nt* notif_eventCommandFinished;
   dn_ipmg_eventMoteJoin_nt* notif_eventMoteJoin;
   dn_ipmg_eventMoteOperational_nt* notif_eventMoteOperational;
   dn_ipmg_eventMoteLost_nt* notif_eventMoteLost;
   dn_ipmg_eventNetworkTime_nt* notif_eventNetworkTime;
   dn_ipmg_eventPingResponse_nt* notif_eventPingResponse;
   dn_ipmg_eventPathCreate_nt* notif_eventPathCreate;
   dn_ipmg_eventPathDelete_nt* notif_eventPathDelete;
   dn_ipmg_eventPacketSent_nt* notif_eventPacketSent;
   dn_ipmg_eventMoteCreate_nt* notif_eventMoteCreate;
   dn_ipmg_eventMoteDelete_nt* notif_eventMoteDelete;
   
   uint8_t  notifType;
   uint8_t  subNotifType;
   uint32_t eventId;
   
   // abort if not a notification
   if (cmdId!=CMDID_NOTIFICATION) {
      return;
   }
   
   // abort no space for notifType
   if (len<DN_MIN_NOTIF_LEN) {
      return;
   }
   
   notifType  = payload[0];
   payload    = &payload[1];
   len       -= 1;
   
   // parse notification
   switch(notifType) {
      case DN_NOTIFID_NOTIFEVENT:

         // abort no space for subNotifType
         if (len<DN_MIN_EVENT_LEN) {
            return;
         }
         
         // get subNotifType
         dn_read_uint32_t(&eventId,&payload[0]);
         subNotifType   = payload[4];
         payload        = &payload[5];
         len           -= 5;
         
         switch (subNotifType) {
            case DN_EVENTID_EVENTMOTERESET:
               
               // verify len
               if (len<DN_EVENTMOTERESET_NOTIF_LEN) {
                   return;
               }
               
               // cast notifBuf
               notif_eventMoteReset = (dn_ipmg_eventMoteReset_nt*)dn_ipmg_vars.notifBuf;
               
               notif_eventMoteReset->eventId = eventId;
               // parse the notification
               memcpy(&notif_eventMoteReset->macAddress[0],&payload[DN_EVENTMOTERESET_NOTIF_OFFS_MACADDRESS],8);
               
               // call the notif callback
               dn_ipmg_vars.notifCb(DN_NOTIFID_NOTIFEVENT,DN_EVENTID_EVENTMOTERESET);
               
               break;
               
            case DN_EVENTID_EVENTNETWORKRESET:
               
               // cast notifBuf
               notif_eventNetworkReset = (dn_ipmg_eventNetworkReset_nt*)dn_ipmg_vars.notifBuf;
               
               notif_eventNetworkReset->eventId = eventId;
               // parse the notification
               
               // call the notif callback
               dn_ipmg_vars.notifCb(DN_NOTIFID_NOTIFEVENT,DN_EVENTID_EVENTNETWORKRESET);
               
               break;
               
            case DN_EVENTID_EVENTCOMMANDFINISHED:
               
               // verify len
               if (len<DN_EVENTCOMMANDFINISHED_NOTIF_LEN) {
                   return;
               }
               
               // cast notifBuf
               notif_eventCommandFinished = (dn_ipmg_eventCommandFinished_nt*)dn_ipmg_vars.notifBuf;
               
               notif_eventCommandFinished->eventId = eventId;
               // parse the notification
               dn_read_uint32_t(&notif_eventCommandFinished->callbackId,&payload[DN_EVENTCOMMANDFINISHED_NOTIF_OFFS_CALLBACKID]);
               notif_eventCommandFinished->rc = payload[DN_EVENTCOMMANDFINISHED_NOTIF_OFFS_RC];
               
               // call the notif callback
               dn_ipmg_vars.notifCb(DN_NOTIFID_NOTIFEVENT,DN_EVENTID_EVENTCOMMANDFINISHED);
               
               break;
               
            case DN_EVENTID_EVENTMOTEJOIN:
               
               // verify len
               if (len<DN_EVENTMOTEJOIN_NOTIF_LEN) {
                   return;
               }
               
               // cast notifBuf
               notif_eventMoteJoin = (dn_ipmg_eventMoteJoin_nt*)dn_ipmg_vars.notifBuf;
               
               notif_eventMoteJoin->eventId = eventId;
               // parse the notification
               memcpy(&notif_eventMoteJoin->macAddress[0],&payload[DN_EVENTMOTEJOIN_NOTIF_OFFS_MACADDRESS],8);
               
               // call the notif callback
               dn_ipmg_vars.notifCb(DN_NOTIFID_NOTIFEVENT,DN_EVENTID_EVENTMOTEJOIN);
               
               break;
               
            case DN_EVENTID_EVENTMOTEOPERATIONAL:
               
               // verify len
               if (len<DN_EVENTMOTEOPERATIONAL_NOTIF_LEN) {
                   return;
               }
               
               // cast notifBuf
               notif_eventMoteOperational = (dn_ipmg_eventMoteOperational_nt*)dn_ipmg_vars.notifBuf;
               
               notif_eventMoteOperational->eventId = eventId;
               // parse the notification
               memcpy(&notif_eventMoteOperational->macAddress[0],&payload[DN_EVENTMOTEOPERATIONAL_NOTIF_OFFS_MACADDRESS],8);
               
               // call the notif callback
               dn_ipmg_vars.notifCb(DN_NOTIFID_NOTIFEVENT,DN_EVENTID_EVENTMOTEOPERATIONAL);
               
               break;
               
            case DN_EVENTID_EVENTMOTELOST:
               
               // verify len
               if (len<DN_EVENTMOTELOST_NOTIF_LEN) {
                   return;
               }
               
               // cast notifBuf
               notif_eventMoteLost = (dn_ipmg_eventMoteLost_nt*)dn_ipmg_vars.notifBuf;
               
               notif_eventMoteLost->eventId = eventId;
               // parse the notification
               memcpy(&notif_eventMoteLost->macAddress[0],&payload[DN_EVENTMOTELOST_NOTIF_OFFS_MACADDRESS],8);
               
               // call the notif callback
               dn_ipmg_vars.notifCb(DN_NOTIFID_NOTIFEVENT,DN_EVENTID_EVENTMOTELOST);
               
               break;
               
            case DN_EVENTID_EVENTNETWORKTIME:
               
               // verify len
               if (len<DN_EVENTNETWORKTIME_NOTIF_LEN) {
                   return;
               }
               
               // cast notifBuf
               notif_eventNetworkTime = (dn_ipmg_eventNetworkTime_nt*)dn_ipmg_vars.notifBuf;
               
               notif_eventNetworkTime->eventId = eventId;
               // parse the notification
               dn_read_uint32_t(&notif_eventNetworkTime->uptime,&payload[DN_EVENTNETWORKTIME_NOTIF_OFFS_UPTIME]);
               memcpy(&notif_eventNetworkTime->utcSecs[0],&payload[DN_EVENTNETWORKTIME_NOTIF_OFFS_UTCSECS],8);
               dn_read_uint32_t(&notif_eventNetworkTime->utcUsecs,&payload[DN_EVENTNETWORKTIME_NOTIF_OFFS_UTCUSECS]);
               memcpy(&notif_eventNetworkTime->asn[0],&payload[DN_EVENTNETWORKTIME_NOTIF_OFFS_ASN],5);
               dn_read_uint16_t(&notif_eventNetworkTime->asnOffset,&payload[DN_EVENTNETWORKTIME_NOTIF_OFFS_ASNOFFSET]);
               
               // call the notif callback
               dn_ipmg_vars.notifCb(DN_NOTIFID_NOTIFEVENT,DN_EVENTID_EVENTNETWORKTIME);
               
               break;
               
            case DN_EVENTID_EVENTPINGRESPONSE:
               
               // verify len
               if (len<DN_EVENTPINGRESPONSE_NOTIF_LEN) {
                   return;
               }
               
               // cast notifBuf
               notif_eventPingResponse = (dn_ipmg_eventPingResponse_nt*)dn_ipmg_vars.notifBuf;
               
               notif_eventPingResponse->eventId = eventId;
               // parse the notification
               dn_read_uint32_t(&notif_eventPingResponse->callbackId,&payload[DN_EVENTPINGRESPONSE_NOTIF_OFFS_CALLBACKID]);
               memcpy(&notif_eventPingResponse->macAddress[0],&payload[DN_EVENTPINGRESPONSE_NOTIF_OFFS_MACADDRESS],8);
               dn_read_uint32_t(&notif_eventPingResponse->delay,&payload[DN_EVENTPINGRESPONSE_NOTIF_OFFS_DELAY]);
               dn_read_uint16_t(&notif_eventPingResponse->voltage,&payload[DN_EVENTPINGRESPONSE_NOTIF_OFFS_VOLTAGE]);
               notif_eventPingResponse->temperature = (int8_t)payload[DN_EVENTPINGRESPONSE_NOTIF_OFFS_TEMPERATURE];
               
               // call the notif callback
               dn_ipmg_vars.notifCb(DN_NOTIFID_NOTIFEVENT,DN_EVENTID_EVENTPINGRESPONSE);
               
               break;
               
            case DN_EVENTID_EVENTPATHCREATE:
               
               // verify len
               if (len<DN_EVENTPATHCREATE_NOTIF_LEN) {
                   return;
               }
               
               // cast notifBuf
               notif_eventPathCreate = (dn_ipmg_eventPathCreate_nt*)dn_ipmg_vars.notifBuf;
               
               notif_eventPathCreate->eventId = eventId;
               // parse the notification
               memcpy(&notif_eventPathCreate->source[0],&payload[DN_EVENTPATHCREATE_NOTIF_OFFS_SOURCE],8);
               memcpy(&notif_eventPathCreate->dest[0],&payload[DN_EVENTPATHCREATE_NOTIF_OFFS_DEST],8);
               notif_eventPathCreate->direction = payload[DN_EVENTPATHCREATE_NOTIF_OFFS_DIRECTION];
               
               // call the notif callback
               dn_ipmg_vars.notifCb(DN_NOTIFID_NOTIFEVENT,DN_EVENTID_EVENTPATHCREATE);
               
               break;
               
            case DN_EVENTID_EVENTPATHDELETE:
               
               // verify len
               if (len<DN_EVENTPATHDELETE_NOTIF_LEN) {
                   return;
               }
               
               // cast notifBuf
               notif_eventPathDelete = (dn_ipmg_eventPathDelete_nt*)dn_ipmg_vars.notifBuf;
               
               notif_eventPathDelete->eventId = eventId;
               // parse the notification
               memcpy(&notif_eventPathDelete->source[0],&payload[DN_EVENTPATHDELETE_NOTIF_OFFS_SOURCE],8);
               memcpy(&notif_eventPathDelete->dest[0],&payload[DN_EVENTPATHDELETE_NOTIF_OFFS_DEST],8);
               notif_eventPathDelete->direction = payload[DN_EVENTPATHDELETE_NOTIF_OFFS_DIRECTION];
               
               // call the notif callback
               dn_ipmg_vars.notifCb(DN_NOTIFID_NOTIFEVENT,DN_EVENTID_EVENTPATHDELETE);
               
               break;
               
            case DN_EVENTID_EVENTPACKETSENT:
               
               // verify len
               if (len<DN_EVENTPACKETSENT_NOTIF_LEN) {
                   return;
               }
               
               // cast notifBuf
               notif_eventPacketSent = (dn_ipmg_eventPacketSent_nt*)dn_ipmg_vars.notifBuf;
               
               notif_eventPacketSent->eventId = eventId;
               // parse the notification
               dn_read_uint32_t(&notif_eventPacketSent->callbackId,&payload[DN_EVENTPACKETSENT_NOTIF_OFFS_CALLBACKID]);
               notif_eventPacketSent->rc = payload[DN_EVENTPACKETSENT_NOTIF_OFFS_RC];
               
               // call the notif callback
               dn_ipmg_vars.notifCb(DN_NOTIFID_NOTIFEVENT,DN_EVENTID_EVENTPACKETSENT);
               
               break;
               
            case DN_EVENTID_EVENTMOTECREATE:
               
               // verify len
               if (len<DN_EVENTMOTECREATE_NOTIF_LEN) {
                   return;
               }
               
               // cast notifBuf
               notif_eventMoteCreate = (dn_ipmg_eventMoteCreate_nt*)dn_ipmg_vars.notifBuf;
               
               notif_eventMoteCreate->eventId = eventId;
               // parse the notification
               memcpy(&notif_eventMoteCreate->macAddress[0],&payload[DN_EVENTMOTECREATE_NOTIF_OFFS_MACADDRESS],8);
               dn_read_uint16_t(&notif_eventMoteCreate->moteId,&payload[DN_EVENTMOTECREATE_NOTIF_OFFS_MOTEID]);
               
               // call the notif callback
               dn_ipmg_vars.notifCb(DN_NOTIFID_NOTIFEVENT,DN_EVENTID_EVENTMOTECREATE);
               
               break;
               
            case DN_EVENTID_EVENTMOTEDELETE:
               
               // verify len
               if (len<DN_EVENTMOTEDELETE_NOTIF_LEN) {
                   return;
               }
               
               // cast notifBuf
               notif_eventMoteDelete = (dn_ipmg_eventMoteDelete_nt*)dn_ipmg_vars.notifBuf;
               
               notif_eventMoteDelete->eventId = eventId;
               // parse the notification
               memcpy(&notif_eventMoteDelete->macAddress[0],&payload[DN_EVENTMOTEDELETE_NOTIF_OFFS_MACADDRESS],8);
               dn_read_uint16_t(&notif_eventMoteDelete->moteId,&payload[DN_EVENTMOTEDELETE_NOTIF_OFFS_MOTEID]);
               
               // call the notif callback
               dn_ipmg_vars.notifCb(DN_NOTIFID_NOTIFEVENT,DN_EVENTID_EVENTMOTEDELETE);
               
               break;
               
            default:
               // unknown subNotifType
               break;
         }
         
         break;
         
      case DN_NOTIFID_NOTIFLOG:
         
         // verify length payload received
         if (len<DN_NOTIFLOG_NOTIF_LEN) {
            return;
         }
         
         // cast notifBuf
         notif_notifLog = (dn_ipmg_notifLog_nt*)dn_ipmg_vars.notifBuf;
         
         // parse the notification
         memcpy(&notif_notifLog->macAddress[0],&payload[DN_NOTIFLOG_NOTIF_OFFS_MACADDRESS],8);
         notif_notifLog->logMsgLen = len-DN_NOTIFLOG_NOTIF_OFFS_LOGMSG;
         memcpy(&notif_notifLog->logMsg[0],&payload[DN_NOTIFLOG_NOTIF_OFFS_LOGMSG],len-DN_NOTIFLOG_NOTIF_OFFS_LOGMSG);
         
         // call the notif callback
         dn_ipmg_vars.notifCb(DN_NOTIFID_NOTIFLOG,DN_SUBCMDID_NONE);
         
         break;
         
      case DN_NOTIFID_NOTIFDATA:
         
         // verify length payload received
         if (len<DN_NOTIFDATA_NOTIF_LEN) {
            return;
         }
         
         // cast notifBuf
         notif_notifData = (dn_ipmg_notifData_nt*)dn_ipmg_vars.notifBuf;
         
         // parse the notification
         memcpy(&notif_notifData->utcSecs[0],&payload[DN_NOTIFDATA_NOTIF_OFFS_UTCSECS],8);
         dn_read_uint32_t(&notif_notifData->utcUsecs,&payload[DN_NOTIFDATA_NOTIF_OFFS_UTCUSECS]);
         memcpy(&notif_notifData->macAddress[0],&payload[DN_NOTIFDATA_NOTIF_OFFS_MACADDRESS],8);
         dn_read_uint16_t(&notif_notifData->srcPort,&payload[DN_NOTIFDATA_NOTIF_OFFS_SRCPORT]);
         dn_read_uint16_t(&notif_notifData->dstPort,&payload[DN_NOTIFDATA_NOTIF_OFFS_DSTPORT]);
         notif_notifData->dataLen = len-DN_NOTIFDATA_NOTIF_OFFS_DATA;
         memcpy(&notif_notifData->data[0],&payload[DN_NOTIFDATA_NOTIF_OFFS_DATA],len-DN_NOTIFDATA_NOTIF_OFFS_DATA);
         
         // call the notif callback
         dn_ipmg_vars.notifCb(DN_NOTIFID_NOTIFDATA,DN_SUBCMDID_NONE);
         
         break;
         
      case DN_NOTIFID_NOTIFIPDATA:
         
         // verify length payload received
         if (len<DN_NOTIFIPDATA_NOTIF_LEN) {
            return;
         }
         
         // cast notifBuf
         notif_notifIpData = (dn_ipmg_notifIpData_nt*)dn_ipmg_vars.notifBuf;
         
         // parse the notification
         memcpy(&notif_notifIpData->utcSecs[0],&payload[DN_NOTIFIPDATA_NOTIF_OFFS_UTCSECS],8);
         dn_read_uint32_t(&notif_notifIpData->utcUsecs,&payload[DN_NOTIFIPDATA_NOTIF_OFFS_UTCUSECS]);
         memcpy(&notif_notifIpData->macAddress[0],&payload[DN_NOTIFIPDATA_NOTIF_OFFS_MACADDRESS],8);
         notif_notifIpData->dataLen = len-DN_NOTIFIPDATA_NOTIF_OFFS_DATA;
         memcpy(&notif_notifIpData->data[0],&payload[DN_NOTIFIPDATA_NOTIF_OFFS_DATA],len-DN_NOTIFIPDATA_NOTIF_OFFS_DATA);
         
         // call the notif callback
         dn_ipmg_vars.notifCb(DN_NOTIFID_NOTIFIPDATA,DN_SUBCMDID_NONE);
         
         break;
         
      case DN_NOTIFID_NOTIFHEALTHREPORT:
         
         // verify length payload received
         if (len<DN_NOTIFHEALTHREPORT_NOTIF_LEN) {
            return;
         }
         
         // cast notifBuf
         notif_notifHealthReport = (dn_ipmg_notifHealthReport_nt*)dn_ipmg_vars.notifBuf;
         
         // parse the notification
         memcpy(&notif_notifHealthReport->macAddress[0],&payload[DN_NOTIFHEALTHREPORT_NOTIF_OFFS_MACADDRESS],8);
         notif_notifHealthReport->payloadLen = len-DN_NOTIFHEALTHREPORT_NOTIF_OFFS_PAYLOAD;
         memcpy(&notif_notifHealthReport->payload[0],&payload[DN_NOTIFHEALTHREPORT_NOTIF_OFFS_PAYLOAD],len-DN_NOTIFHEALTHREPORT_NOTIF_OFFS_PAYLOAD);
         
         // call the notif callback
         dn_ipmg_vars.notifCb(DN_NOTIFID_NOTIFHEALTHREPORT,DN_SUBCMDID_NONE);
         
         break;
         
      default:
         // unknown notifType
         return;
   }
}

//=========================== helpers =========================================

