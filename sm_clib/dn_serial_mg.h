/*
Copyright (c) 2014, Dust Networks. All rights reserved.

Serial connector.

\license See attached DN_LICENSE.txt.
*/

#ifndef DN_SERIAL_H
#define DN_SERIAL_H

#include "dn_common.h"

//=========================== defines =========================================

#define DN_SERIAL_API_MASK_RESPONSE    0x01
#define DN_SERIAL_API_MASK_PACKETID    0x02
#define DN_SERIAL_API_MASK_SYNC        0x08

#define DN_SERIAL_FLAG_DATA            (0<<0)
#define DN_SERIAL_FLAG_ACK             (1<<0)

#define DN_SERIAL_FLAG_UNACKNOWLEDGED  (0<<1)
#define DN_SERIAL_FLAG_ACKNOWLEDGED    (1<<1)

// return code
#define DN_SERIAL_RC_OK                0x00

// connection states
#define DN_SERIAL_ST_DISCONNECTED      0x00
#define DN_SERIAL_ST_HELLO_SENT        0x01
#define DN_SERIAL_ST_CONNECTED         0x02

#define DN_API_VERSION                 4

#define SERIAL_CMID_HELLO              1
#define SERIAL_CMID_HELLO_RESPONSE     2
#define SERIAL_CMID_MGR_HELLO          3

#define HELLO_RESP_OFFS_RC             0
#define HELLO_RESP_OFFS_VERSION        1
#define HELLO_RESP_OFFS_MGRSEQNO       2
#define HELLO_RESP_OFFS_CLISEQNO       3
#define HELLO_RESP_OFFS_MODE           4

//=========================== typedef =========================================

typedef void (*dn_status_change_cbt)(uint8_t newStatus);
typedef void (*dn_serial_request_cbt)(uint8_t cmdId, uint8_t flags, uint8_t* payload, uint8_t len);
typedef void (*dn_serial_reply_cbt)(uint8_t cmdId, uint8_t rc, uint8_t* payload, uint8_t len);

//=========================== variables =======================================

//=========================== prototypes ======================================

void     dn_serial_mg_init(dn_serial_request_cbt requestCb, dn_status_change_cbt statusChangeCb);
dn_err_t dn_serial_mg_initiateConnect();
dn_err_t dn_serial_mg_sendRequest(
   uint8_t              cmdId,
   bool                 isAck,
   uint8_t*             payload,
   uint8_t              length,
   dn_serial_reply_cbt  replyCb
);

#endif
