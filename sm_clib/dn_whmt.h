/*
Copyright (c) 2015, Dust Networks. All rights reserved.

C library to connect to a SmartMesh WirelessHART Mote.

\license See attached DN_LICENSE.txt.
*/

#ifndef dn_whmt_h
#define dn_whmt_h

#include "dn_common.h"
#include "dn_endianness.h"
#include "dn_clib_version.h"

//=========================== defines =========================================

#define MAX_FRAME_LENGTH                    128
#define DN_SUBCMDID_NONE                    0xff

//===== target memory
#define DN_MEMORY_NV_ONLY              0x00
#define DN_MEMORY_NV_RAM               0x01

#define DN_TRANTYPE_BESTEFFORT         0x00
#define DN_TRANTYPE_ACKNOWLEDGED       0x01

#define DN_TRANDIR_REQUEST             0x00
#define DN_TRANDIR_RESPONSE            0x01

//===== command IDs (requests)
#define CMDID_SETPARAMETER                  0x1
#define CMDID_GETPARAMETER                  0x2
#define CMDID_SETNVPARAMETER                0x3
#define CMDID_GETNVPARAMETER                0x4
#define CMDID_SEND                          0x5
#define CMDID_JOIN                          0x6
#define CMDID_DISCONNECT                    0x7
#define CMDID_RESET                         0x8
#define CMDID_LOWPOWERSLEEP                 0x9
#define CMDID_HARTPAYLOAD                   0xa
#define CMDID_TESTRADIOTX                   0xb
#define CMDID_TESTRADIORX                   0xc
#define CMDID_CLEARNV                       0x10
#define CMDID_SEARCH                        0x11
#define CMDID_TESTRADIOTXEXT                0x13
#define CMDID_TESTRADIORXEXT                0x14
#define CMDID_ZEROIZE                       0x15
#define CMDID_FILEWRITE                     0x17
#define CMDID_FILEREAD                      0x18
#define CMDID_FILEOPEN                      0x19

//===== command IDs (notifications)
#define CMDID_TIMEINDICATION           0xd
#define CMDID_SERVICEINDICATION        0xe
#define CMDID_EVENTS                   0xf
#define CMDID_DATARECEIVED             0x81
#define CMDID_ADVRECEIVED              0x12
#define CMDID_SUSPENDSTARTED           0x16

//===== parameter IDs
#define PARAMID_MACADDRESS             0x1
#define PARAMID_JOINKEY                0x2
#define PARAMID_NETWORKID              0x3
#define PARAMID_TXPOWER                0x4
#define PARAMID_POWERINFO              0x5
#define PARAMID_JOINDUTYCYCLE          0x6
#define PARAMID_BATTERYLIFE            0x7
#define PARAMID_SERVICE                0x8
#define PARAMID_HARTDEVICESTATUS       0x9
#define PARAMID_HARTDEVICEINFO         0xa
#define PARAMID_EVENTMASK              0xb
#define PARAMID_MOTEINFO               0xc
#define PARAMID_NETWORKINFO            0xd
#define PARAMID_MOTESTATUS             0xe
#define PARAMID_TIME                   0xf
#define PARAMID_CHARGE                 0x10
#define PARAMID_TESTRADIORXSTATS       0x11
#define PARAMID_WRITEPROTECT           0x12
#define PARAMID_TTL                    0x13
#define PARAMID_HARTANTENNAGAIN        0x14
#define PARAMID_OTAPLOCKOUT            0x15
#define PARAMID_HRCOUNTERMODE          0x17
#define PARAMID_AUTOJOIN               0x18
#define PARAMID_COMPLIANTMODE          0x19
#define PARAMID_LOCK                   0x1a

//===== format of requests

// setParameter_txPower
#define DN_SETPARAMETER_TXPOWER_REQ_OFFS_TXPOWER                     1
#define DN_SETPARAMETER_TXPOWER_REQ_LEN                              2

// setParameter_joinDutyCycle
#define DN_SETPARAMETER_JOINDUTYCYCLE_REQ_OFFS_DUTYCYCLE             1
#define DN_SETPARAMETER_JOINDUTYCYCLE_REQ_LEN                        2

// setParameter_batteryLife
#define DN_SETPARAMETER_BATTERYLIFE_REQ_OFFS_BATTERYLIFE             1
#define DN_SETPARAMETER_BATTERYLIFE_REQ_OFFS_POWERSTATUS             3
#define DN_SETPARAMETER_BATTERYLIFE_REQ_LEN                          4

// setParameter_service
#define DN_SETPARAMETER_SERVICE_REQ_OFFS_SERVICEID                   1
#define DN_SETPARAMETER_SERVICE_REQ_OFFS_SERVICEREQFLAGS             2
#define DN_SETPARAMETER_SERVICE_REQ_OFFS_APPDOMAIN                   3
#define DN_SETPARAMETER_SERVICE_REQ_OFFS_DESTADDR                    4
#define DN_SETPARAMETER_SERVICE_REQ_OFFS_TIME                        6
#define DN_SETPARAMETER_SERVICE_REQ_LEN                              10

// setParameter_hartDeviceStatus
#define DN_SETPARAMETER_HARTDEVICESTATUS_REQ_OFFS_HARTDEVSTATUS      1
#define DN_SETPARAMETER_HARTDEVICESTATUS_REQ_LEN                     2

// setParameter_hartDeviceInfo
#define DN_SETPARAMETER_HARTDEVICEINFO_REQ_OFFS_HARTCMD0             1
#define DN_SETPARAMETER_HARTDEVICEINFO_REQ_OFFS_HARTCMD20            23
#define DN_SETPARAMETER_HARTDEVICEINFO_REQ_LEN                       55

// setParameter_eventMask
#define DN_SETPARAMETER_EVENTMASK_REQ_OFFS_EVENTMASK                 1
#define DN_SETPARAMETER_EVENTMASK_REQ_LEN                            5

// setParameter_writeProtect
#define DN_SETPARAMETER_WRITEPROTECT_REQ_OFFS_WRITEPROTECT           1
#define DN_SETPARAMETER_WRITEPROTECT_REQ_LEN                         2

// setParameter_lock
#define DN_SETPARAMETER_LOCK_REQ_OFFS_CODE                           1
#define DN_SETPARAMETER_LOCK_REQ_OFFS_MASTER                         2
#define DN_SETPARAMETER_LOCK_REQ_LEN                                 4

// getParameter_joinDutyCycle
#define DN_GETPARAMETER_JOINDUTYCYCLE_REQ_LEN                        1

// getParameter_service
#define DN_GETPARAMETER_SERVICE_REQ_OFFS_SERVICEID                   1
#define DN_GETPARAMETER_SERVICE_REQ_LEN                              2

// getParameter_moteInfo
#define DN_GETPARAMETER_MOTEINFO_REQ_LEN                             1

// getParameter_networkInfo
#define DN_GETPARAMETER_NETWORKINFO_REQ_LEN                          1

// getParameter_moteStatus
#define DN_GETPARAMETER_MOTESTATUS_REQ_LEN                           1

// getParameter_time
#define DN_GETPARAMETER_TIME_REQ_LEN                                 1

// getParameter_charge
#define DN_GETPARAMETER_CHARGE_REQ_LEN                               1

// getParameter_testRadioRxStats
#define DN_GETPARAMETER_TESTRADIORXSTATS_REQ_LEN                     1

// getParameter_lock
#define DN_GETPARAMETER_LOCK_REQ_LEN                                 1

// setNVParameter_macAddress
#define DN_SETNVPARAMETER_MACADDRESS_REQ_OFFS_MACADDR                5
#define DN_SETNVPARAMETER_MACADDRESS_REQ_LEN                         13

// setNVParameter_joinKey
#define DN_SETNVPARAMETER_JOINKEY_REQ_OFFS_JOINKEY                   5
#define DN_SETNVPARAMETER_JOINKEY_REQ_LEN                            21

// setNVParameter_networkId
#define DN_SETNVPARAMETER_NETWORKID_REQ_OFFS_NETWORKID               5
#define DN_SETNVPARAMETER_NETWORKID_REQ_LEN                          7

// setNVParameter_txPower
#define DN_SETNVPARAMETER_TXPOWER_REQ_OFFS_TXPOWER                   5
#define DN_SETNVPARAMETER_TXPOWER_REQ_LEN                            6

// setNVParameter_powerInfo
#define DN_SETNVPARAMETER_POWERINFO_REQ_OFFS_POWERSOURCE             5
#define DN_SETNVPARAMETER_POWERINFO_REQ_OFFS_DISCHARGECUR            6
#define DN_SETNVPARAMETER_POWERINFO_REQ_OFFS_DISCHARGETIME           8
#define DN_SETNVPARAMETER_POWERINFO_REQ_OFFS_RECOVERTIME             12
#define DN_SETNVPARAMETER_POWERINFO_REQ_LEN                          16

// setNVParameter_ttl
#define DN_SETNVPARAMETER_TTL_REQ_OFFS_TIMETOLIVE                    5
#define DN_SETNVPARAMETER_TTL_REQ_LEN                                6

// setNVParameter_HARTantennaGain
#define DN_SETNVPARAMETER_HARTANTENNAGAIN_REQ_OFFS_ANTENNAGAIN       5
#define DN_SETNVPARAMETER_HARTANTENNAGAIN_REQ_LEN                    6

// setNVParameter_OTAPlockout
#define DN_SETNVPARAMETER_OTAPLOCKOUT_REQ_OFFS_OTAPLOCKOUT           5
#define DN_SETNVPARAMETER_OTAPLOCKOUT_REQ_LEN                        6

// setNVParameter_hrCounterMode
#define DN_SETNVPARAMETER_HRCOUNTERMODE_REQ_OFFS_HRCOUNTERMODE       5
#define DN_SETNVPARAMETER_HRCOUNTERMODE_REQ_LEN                      6

// setNVParameter_autojoin
#define DN_SETNVPARAMETER_AUTOJOIN_REQ_OFFS_RESERVED                 5
#define DN_SETNVPARAMETER_AUTOJOIN_REQ_OFFS_NVPARAMID                9
#define DN_SETNVPARAMETER_AUTOJOIN_REQ_OFFS_AUTOJOIN                 10
#define DN_SETNVPARAMETER_AUTOJOIN_REQ_LEN                           11

// setNVParameter_compliantMode
#define DN_SETNVPARAMETER_COMPLIANTMODE_REQ_OFFS_COMPLIANTMODE       5
#define DN_SETNVPARAMETER_COMPLIANTMODE_REQ_LEN                      6

// setNVParameter_lock
#define DN_SETNVPARAMETER_LOCK_REQ_OFFS_CODE                         5
#define DN_SETNVPARAMETER_LOCK_REQ_OFFS_MASTER                       6
#define DN_SETNVPARAMETER_LOCK_REQ_LEN                               8

// getNVParameter_macAddress
#define DN_GETNVPARAMETER_MACADDRESS_REQ_LEN                         5

// getNVParameter_networkId
#define DN_GETNVPARAMETER_NETWORKID_REQ_LEN                          5

// getNVParameter_txPower
#define DN_GETNVPARAMETER_TXPOWER_REQ_LEN                            5

// getNVParameter_powerInfo
#define DN_GETNVPARAMETER_POWERINFO_REQ_LEN                          5

// getNVParameter_ttl
#define DN_GETNVPARAMETER_TTL_REQ_LEN                                5

// getNVParameter_HARTantennaGain
#define DN_GETNVPARAMETER_HARTANTENNAGAIN_REQ_LEN                    5

// getNVParameter_OTAPlockout
#define DN_GETNVPARAMETER_OTAPLOCKOUT_REQ_LEN                        5

// getNVParameter_hrCounterMode
#define DN_GETNVPARAMETER_HRCOUNTERMODE_REQ_LEN                      5

// getNVParameter_autojoin
#define DN_GETNVPARAMETER_AUTOJOIN_REQ_OFFS_RESERVED                 5
#define DN_GETNVPARAMETER_AUTOJOIN_REQ_OFFS_NVPARAMID                9
#define DN_GETNVPARAMETER_AUTOJOIN_REQ_LEN                           10

// getNVParameter_compliantMode
#define DN_GETNVPARAMETER_COMPLIANTMODE_REQ_LEN                      5

// getNVParameter_lock
#define DN_GETNVPARAMETER_LOCK_REQ_LEN                               5

// send
#define DN_SEND_REQ_OFFS_DESTADDR                                    0
#define DN_SEND_REQ_OFFS_SERVICEID                                   2
#define DN_SEND_REQ_OFFS_APPDOMAIN                                   3
#define DN_SEND_REQ_OFFS_PRIORITY                                    4
#define DN_SEND_REQ_OFFS_RESERVED                                    5
#define DN_SEND_REQ_OFFS_SEQNUM                                      7
#define DN_SEND_REQ_OFFS_PAYLOADLEN                                  8
#define DN_SEND_REQ_OFFS_PAYLOAD                                     9
#define DN_SEND_REQ_LEN                                              9

// join
#define DN_JOIN_REQ_LEN                                              0

// disconnect
#define DN_DISCONNECT_REQ_LEN                                        0

// reset
#define DN_RESET_REQ_LEN                                             0

// lowPowerSleep
#define DN_LOWPOWERSLEEP_REQ_LEN                                     0

// hartPayload
#define DN_HARTPAYLOAD_REQ_OFFS_PAYLOADLEN                           0
#define DN_HARTPAYLOAD_REQ_OFFS_PAYLOAD                              1
#define DN_HARTPAYLOAD_REQ_LEN                                       1

// testRadioTx
#define DN_TESTRADIOTX_REQ_OFFS_CHANNEL                              0
#define DN_TESTRADIOTX_REQ_OFFS_NUMPACKETS                           1
#define DN_TESTRADIOTX_REQ_LEN                                       3

// testRadioRx
#define DN_TESTRADIORX_REQ_OFFS_CHANNEL                              0
#define DN_TESTRADIORX_REQ_OFFS_TIME                                 1
#define DN_TESTRADIORX_REQ_LEN                                       3

// clearNV
#define DN_CLEARNV_REQ_LEN                                           0

// search
#define DN_SEARCH_REQ_LEN                                            0

// testRadioTxExt
#define DN_TESTRADIOTXEXT_REQ_OFFS_TESTTYPE                          0
#define DN_TESTRADIOTXEXT_REQ_OFFS_CHANMASK                          1
#define DN_TESTRADIOTXEXT_REQ_OFFS_REPEATCNT                         3
#define DN_TESTRADIOTXEXT_REQ_OFFS_TXPOWER                           5
#define DN_TESTRADIOTXEXT_REQ_OFFS_SEQSIZE                           6
#define DN_TESTRADIOTXEXT_REQ_OFFS_PKLEN_1                           7
#define DN_TESTRADIOTXEXT_REQ_OFFS_DELAY_1                           8
#define DN_TESTRADIOTXEXT_REQ_OFFS_PKLEN_2                           10
#define DN_TESTRADIOTXEXT_REQ_OFFS_DELAY_2                           11
#define DN_TESTRADIOTXEXT_REQ_OFFS_PKLEN_3                           13
#define DN_TESTRADIOTXEXT_REQ_OFFS_DELAY_3                           14
#define DN_TESTRADIOTXEXT_REQ_OFFS_PKLEN_4                           16
#define DN_TESTRADIOTXEXT_REQ_OFFS_DELAY_4                           17
#define DN_TESTRADIOTXEXT_REQ_OFFS_PKLEN_5                           19
#define DN_TESTRADIOTXEXT_REQ_OFFS_DELAY_5                           20
#define DN_TESTRADIOTXEXT_REQ_OFFS_PKLEN_6                           22
#define DN_TESTRADIOTXEXT_REQ_OFFS_DELAY_6                           23
#define DN_TESTRADIOTXEXT_REQ_OFFS_PKLEN_7                           25
#define DN_TESTRADIOTXEXT_REQ_OFFS_DELAY_7                           26
#define DN_TESTRADIOTXEXT_REQ_OFFS_PKLEN_8                           28
#define DN_TESTRADIOTXEXT_REQ_OFFS_DELAY_8                           29
#define DN_TESTRADIOTXEXT_REQ_OFFS_PKLEN_9                           31
#define DN_TESTRADIOTXEXT_REQ_OFFS_DELAY_9                           32
#define DN_TESTRADIOTXEXT_REQ_OFFS_PKLEN_10                          34
#define DN_TESTRADIOTXEXT_REQ_OFFS_DELAY_10                          35
#define DN_TESTRADIOTXEXT_REQ_OFFS_STATIONID                         37
#define DN_TESTRADIOTXEXT_REQ_LEN                                    38

// testRadioRxExt
#define DN_TESTRADIORXEXT_REQ_OFFS_CHANNELMASK                       0
#define DN_TESTRADIORXEXT_REQ_OFFS_TIME                              2
#define DN_TESTRADIORXEXT_REQ_OFFS_STATIONID                         4
#define DN_TESTRADIORXEXT_REQ_LEN                                    5

// zeroize
#define DN_ZEROIZE_REQ_LEN                                           0

// fileWrite
#define DN_FILEWRITE_REQ_OFFS_DESCRIPTOR                             0
#define DN_FILEWRITE_REQ_OFFS_OFFSET                                 4
#define DN_FILEWRITE_REQ_OFFS_LENGTH                                 6
#define DN_FILEWRITE_REQ_OFFS_DATA                                   7
#define DN_FILEWRITE_REQ_LEN                                         7

// fileRead
#define DN_FILEREAD_REQ_OFFS_DESCRIPTOR                              0
#define DN_FILEREAD_REQ_OFFS_OFFSET                                  4
#define DN_FILEREAD_REQ_OFFS_LENGTH                                  6
#define DN_FILEREAD_REQ_LEN                                          7

// fileOpen
#define DN_FILEOPEN_REQ_OFFS_NAME                                    0
#define DN_FILEOPEN_REQ_OFFS_OPTIONS                                 12
#define DN_FILEOPEN_REQ_OFFS_SIZE                                    13
#define DN_FILEOPEN_REQ_OFFS_MODE                                    15
#define DN_FILEOPEN_REQ_LEN                                          16

//===== format of replies

// setParameter_txPower
#define DN_SETPARAMETER_TXPOWER_REPLY_LEN                            1

// setParameter_joinDutyCycle
#define DN_SETPARAMETER_JOINDUTYCYCLE_REPLY_LEN                      1

// setParameter_batteryLife
#define DN_SETPARAMETER_BATTERYLIFE_REPLY_LEN                        1

// setParameter_service
#define DN_SETPARAMETER_SERVICE_REPLY_OFFS_NUMSERVICES               1
#define DN_SETPARAMETER_SERVICE_REPLY_LEN                            2

// setParameter_hartDeviceStatus
#define DN_SETPARAMETER_HARTDEVICESTATUS_REPLY_LEN                   1

// setParameter_hartDeviceInfo
#define DN_SETPARAMETER_HARTDEVICEINFO_REPLY_LEN                     1

// setParameter_eventMask
#define DN_SETPARAMETER_EVENTMASK_REPLY_LEN                          1

// setParameter_writeProtect
#define DN_SETPARAMETER_WRITEPROTECT_REPLY_LEN                       1

// setParameter_lock
#define DN_SETPARAMETER_LOCK_REPLY_LEN                               1

// getParameter_joinDutyCycle
#define DN_GETPARAMETER_JOINDUTYCYCLE_REPLY_OFFS_JOINDUTYCYCLE       1
#define DN_GETPARAMETER_JOINDUTYCYCLE_REPLY_LEN                      2

// getParameter_service
#define DN_GETPARAMETER_SERVICE_REPLY_OFFS_SERVICEID                 1
#define DN_GETPARAMETER_SERVICE_REPLY_OFFS_SERVICESTATE              2
#define DN_GETPARAMETER_SERVICE_REPLY_OFFS_SERVICEFLAGS              3
#define DN_GETPARAMETER_SERVICE_REPLY_OFFS_APPDOMAIN                 4
#define DN_GETPARAMETER_SERVICE_REPLY_OFFS_DESTADDR                  5
#define DN_GETPARAMETER_SERVICE_REPLY_OFFS_TIME                      7
#define DN_GETPARAMETER_SERVICE_REPLY_LEN                            11

// getParameter_moteInfo
#define DN_GETPARAMETER_MOTEINFO_REPLY_OFFS_APIVERSION               1
#define DN_GETPARAMETER_MOTEINFO_REPLY_OFFS_SERIALNUM                2
#define DN_GETPARAMETER_MOTEINFO_REPLY_OFFS_HWMODEL                  10
#define DN_GETPARAMETER_MOTEINFO_REPLY_OFFS_HWREV                    11
#define DN_GETPARAMETER_MOTEINFO_REPLY_OFFS_SWMAJORREV               12
#define DN_GETPARAMETER_MOTEINFO_REPLY_OFFS_SWMINORREV               13
#define DN_GETPARAMETER_MOTEINFO_REPLY_OFFS_SWPATCH                  14
#define DN_GETPARAMETER_MOTEINFO_REPLY_OFFS_SWBUILD                  15
#define DN_GETPARAMETER_MOTEINFO_REPLY_LEN                           17

// getParameter_networkInfo
#define DN_GETPARAMETER_NETWORKINFO_REPLY_OFFS_MACADDRESS            1
#define DN_GETPARAMETER_NETWORKINFO_REPLY_OFFS_MOTEID                9
#define DN_GETPARAMETER_NETWORKINFO_REPLY_OFFS_NETWORKID             11
#define DN_GETPARAMETER_NETWORKINFO_REPLY_LEN                        13

// getParameter_moteStatus
#define DN_GETPARAMETER_MOTESTATUS_REPLY_OFFS_STATE                  1
#define DN_GETPARAMETER_MOTESTATUS_REPLY_OFFS_MOTESTATEREASON        2
#define DN_GETPARAMETER_MOTESTATUS_REPLY_OFFS_CHANGECOUNTER          3
#define DN_GETPARAMETER_MOTESTATUS_REPLY_OFFS_NUMPARENTS             5
#define DN_GETPARAMETER_MOTESTATUS_REPLY_OFFS_MOTEALARMS             6
#define DN_GETPARAMETER_MOTESTATUS_REPLY_OFFS_STATUSFLAGS            10
#define DN_GETPARAMETER_MOTESTATUS_REPLY_LEN                         11

// getParameter_time
#define DN_GETPARAMETER_TIME_REPLY_OFFS_UTCTIME                      1
#define DN_GETPARAMETER_TIME_REPLY_OFFS_ASN                          9
#define DN_GETPARAMETER_TIME_REPLY_OFFS_ASNOFFSET                    14
#define DN_GETPARAMETER_TIME_REPLY_LEN                               16

// getParameter_charge
#define DN_GETPARAMETER_CHARGE_REPLY_OFFS_CHARGE                     1
#define DN_GETPARAMETER_CHARGE_REPLY_OFFS_UPTIME                     5
#define DN_GETPARAMETER_CHARGE_REPLY_OFFS_TEMPERATURE                9
#define DN_GETPARAMETER_CHARGE_REPLY_OFFS_FRACTIONALTEMP             10
#define DN_GETPARAMETER_CHARGE_REPLY_LEN                             11

// getParameter_testRadioRxStats
#define DN_GETPARAMETER_TESTRADIORXSTATS_REPLY_OFFS_RXOK             1
#define DN_GETPARAMETER_TESTRADIORXSTATS_REPLY_OFFS_RXFAILED         3
#define DN_GETPARAMETER_TESTRADIORXSTATS_REPLY_LEN                   5

// getParameter_lock
#define DN_GETPARAMETER_LOCK_REPLY_OFFS_CODE                         1
#define DN_GETPARAMETER_LOCK_REPLY_OFFS_MASTER                       2
#define DN_GETPARAMETER_LOCK_REPLY_LEN                               4

// setNVParameter_macAddress
#define DN_SETNVPARAMETER_MACADDRESS_REPLY_LEN                       1

// setNVParameter_joinKey
#define DN_SETNVPARAMETER_JOINKEY_REPLY_LEN                          1

// setNVParameter_networkId
#define DN_SETNVPARAMETER_NETWORKID_REPLY_LEN                        1

// setNVParameter_txPower
#define DN_SETNVPARAMETER_TXPOWER_REPLY_LEN                          1

// setNVParameter_powerInfo
#define DN_SETNVPARAMETER_POWERINFO_REPLY_LEN                        1

// setNVParameter_ttl
#define DN_SETNVPARAMETER_TTL_REPLY_LEN                              1

// setNVParameter_HARTantennaGain
#define DN_SETNVPARAMETER_HARTANTENNAGAIN_REPLY_LEN                  1

// setNVParameter_OTAPlockout
#define DN_SETNVPARAMETER_OTAPLOCKOUT_REPLY_LEN                      1

// setNVParameter_hrCounterMode
#define DN_SETNVPARAMETER_HRCOUNTERMODE_REPLY_LEN                    1

// setNVParameter_autojoin
#define DN_SETNVPARAMETER_AUTOJOIN_REPLY_OFFS_NVPARAMID              1
#define DN_SETNVPARAMETER_AUTOJOIN_REPLY_LEN                         2

// setNVParameter_compliantMode
#define DN_SETNVPARAMETER_COMPLIANTMODE_REPLY_LEN                    1

// setNVParameter_lock
#define DN_SETNVPARAMETER_LOCK_REPLY_LEN                             1

// getNVParameter_macAddress
#define DN_GETNVPARAMETER_MACADDRESS_REPLY_OFFS_MACADDR              1
#define DN_GETNVPARAMETER_MACADDRESS_REPLY_LEN                       9

// getNVParameter_networkId
#define DN_GETNVPARAMETER_NETWORKID_REPLY_OFFS_NETWORKID             1
#define DN_GETNVPARAMETER_NETWORKID_REPLY_LEN                        3

// getNVParameter_txPower
#define DN_GETNVPARAMETER_TXPOWER_REPLY_OFFS_TXPOWER                 1
#define DN_GETNVPARAMETER_TXPOWER_REPLY_LEN                          2

// getNVParameter_powerInfo
#define DN_GETNVPARAMETER_POWERINFO_REPLY_OFFS_POWERSOURCE           1
#define DN_GETNVPARAMETER_POWERINFO_REPLY_OFFS_DISCHARGECUR          2
#define DN_GETNVPARAMETER_POWERINFO_REPLY_OFFS_DISCHARGETIME         4
#define DN_GETNVPARAMETER_POWERINFO_REPLY_OFFS_RECOVERTIME           8
#define DN_GETNVPARAMETER_POWERINFO_REPLY_LEN                        12

// getNVParameter_ttl
#define DN_GETNVPARAMETER_TTL_REPLY_OFFS_TIMETOLIVE                  1
#define DN_GETNVPARAMETER_TTL_REPLY_LEN                              2

// getNVParameter_HARTantennaGain
#define DN_GETNVPARAMETER_HARTANTENNAGAIN_REPLY_OFFS_ANTENNAGAIN     1
#define DN_GETNVPARAMETER_HARTANTENNAGAIN_REPLY_LEN                  2

// getNVParameter_OTAPlockout
#define DN_GETNVPARAMETER_OTAPLOCKOUT_REPLY_OFFS_OTAPLOCKOUT         1
#define DN_GETNVPARAMETER_OTAPLOCKOUT_REPLY_LEN                      2

// getNVParameter_hrCounterMode
#define DN_GETNVPARAMETER_HRCOUNTERMODE_REPLY_OFFS_HRCOUNTERMODE     1
#define DN_GETNVPARAMETER_HRCOUNTERMODE_REPLY_LEN                    2

// getNVParameter_autojoin
#define DN_GETNVPARAMETER_AUTOJOIN_REPLY_OFFS_NVPARAMID              1
#define DN_GETNVPARAMETER_AUTOJOIN_REPLY_OFFS_AUTOJOIN               2
#define DN_GETNVPARAMETER_AUTOJOIN_REPLY_LEN                         3

// getNVParameter_compliantMode
#define DN_GETNVPARAMETER_COMPLIANTMODE_REPLY_OFFS_COMPLIANTMODE     1
#define DN_GETNVPARAMETER_COMPLIANTMODE_REPLY_LEN                    2

// getNVParameter_lock
#define DN_GETNVPARAMETER_LOCK_REPLY_OFFS_CODE                       1
#define DN_GETNVPARAMETER_LOCK_REPLY_OFFS_MASTER                     2
#define DN_GETNVPARAMETER_LOCK_REPLY_LEN                             4

// send
#define DN_SEND_REPLY_LEN                                            0

// join
#define DN_JOIN_REPLY_LEN                                            0

// disconnect
#define DN_DISCONNECT_REPLY_LEN                                      0

// reset
#define DN_RESET_REPLY_LEN                                           0

// lowPowerSleep
#define DN_LOWPOWERSLEEP_REPLY_LEN                                   0

// hartPayload
#define DN_HARTPAYLOAD_REPLY_OFFS_PAYLOADLEN                         0
#define DN_HARTPAYLOAD_REPLY_OFFS_PAYLOAD                            1
#define DN_HARTPAYLOAD_REPLY_LEN                                     1

// testRadioTx
#define DN_TESTRADIOTX_REPLY_LEN                                     0

// testRadioRx
#define DN_TESTRADIORX_REPLY_LEN                                     0

// clearNV
#define DN_CLEARNV_REPLY_LEN                                         0

// search
#define DN_SEARCH_REPLY_LEN                                          0

// testRadioTxExt
#define DN_TESTRADIOTXEXT_REPLY_LEN                                  0

// testRadioRxExt
#define DN_TESTRADIORXEXT_REPLY_LEN                                  0

// zeroize
#define DN_ZEROIZE_REPLY_LEN                                         0

// fileWrite
#define DN_FILEWRITE_REPLY_OFFS_LENGTH                               0
#define DN_FILEWRITE_REPLY_LEN                                       4

// fileRead
#define DN_FILEREAD_REPLY_OFFS_DESCRIPTOR                            0
#define DN_FILEREAD_REPLY_OFFS_OFFSET                                4
#define DN_FILEREAD_REPLY_OFFS_LENGTH                                6
#define DN_FILEREAD_REPLY_OFFS_DATA                                  7
#define DN_FILEREAD_REPLY_LEN                                        7

// fileOpen
#define DN_FILEOPEN_REPLY_OFFS_DESCRIPTOR                            0
#define DN_FILEOPEN_REPLY_LEN                                        4

//===== format of notifications

// timeIndication
#define DN_TIMEINDICATION_NOTIF_OFFS_UTCSEC                          0
#define DN_TIMEINDICATION_NOTIF_OFFS_UTCMICROSEC                     4
#define DN_TIMEINDICATION_NOTIF_OFFS_ASN                             8
#define DN_TIMEINDICATION_NOTIF_OFFS_ASNOFFSET                       13
#define DN_TIMEINDICATION_NOTIF_LEN                                  15

// serviceIndication
#define DN_SERVICEINDICATION_NOTIF_OFFS_EVENTCODE                    0
#define DN_SERVICEINDICATION_NOTIF_OFFS_NETMGRCODE                   1
#define DN_SERVICEINDICATION_NOTIF_OFFS_SERVICEID                    2
#define DN_SERVICEINDICATION_NOTIF_OFFS_SERVICESTATE                 3
#define DN_SERVICEINDICATION_NOTIF_OFFS_SERVICEFLAGS                 4
#define DN_SERVICEINDICATION_NOTIF_OFFS_APPDOMAIN                    5
#define DN_SERVICEINDICATION_NOTIF_OFFS_DESTADDR                     6
#define DN_SERVICEINDICATION_NOTIF_OFFS_TIME                         8
#define DN_SERVICEINDICATION_NOTIF_LEN                               12

// events
#define DN_EVENTS_NOTIF_OFFS_EVENTS                                  0
#define DN_EVENTS_NOTIF_OFFS_STATE                                   4
#define DN_EVENTS_NOTIF_OFFS_MOTEALARMS                              5
#define DN_EVENTS_NOTIF_LEN                                          9

// dataReceived
#define DN_DATARECEIVED_NOTIF_OFFS_SRCADDR                           0
#define DN_DATARECEIVED_NOTIF_OFFS_SEQNUM                            2
#define DN_DATARECEIVED_NOTIF_OFFS_PKTLENGTH                         3
#define DN_DATARECEIVED_NOTIF_OFFS_DATA                              4
#define DN_DATARECEIVED_NOTIF_LEN                                    4

// advReceived
#define DN_ADVRECEIVED_NOTIF_OFFS_NETID                              0
#define DN_ADVRECEIVED_NOTIF_OFFS_MOTEID                             2
#define DN_ADVRECEIVED_NOTIF_OFFS_RSSI                               4
#define DN_ADVRECEIVED_NOTIF_OFFS_JOINPRI                            5
#define DN_ADVRECEIVED_NOTIF_LEN                                     6

// suspendStarted
#define DN_SUSPENDSTARTED_NOTIF_OFFS_DURATION                        0
#define DN_SUSPENDSTARTED_NOTIF_LEN                                  4

//=========================== typedef =========================================

//=== reply types

typedef struct {
   uint8_t    RC;
} dn_whmt_setParameter_txPower_rpt;

typedef struct {
   uint8_t    RC;
} dn_whmt_setParameter_joinDutyCycle_rpt;

typedef struct {
   uint8_t    RC;
} dn_whmt_setParameter_batteryLife_rpt;

typedef struct {
   uint8_t    RC;
   uint8_t    numServices;
} dn_whmt_setParameter_service_rpt;

typedef struct {
   uint8_t    RC;
} dn_whmt_setParameter_hartDeviceStatus_rpt;

typedef struct {
   uint8_t    RC;
} dn_whmt_setParameter_hartDeviceInfo_rpt;

typedef struct {
   uint8_t    RC;
} dn_whmt_setParameter_eventMask_rpt;

typedef struct {
   uint8_t    RC;
} dn_whmt_setParameter_writeProtect_rpt;

typedef struct {
   uint8_t    RC;
} dn_whmt_setParameter_lock_rpt;

typedef struct {
   uint8_t    RC;
   uint8_t    joinDutyCycle;
} dn_whmt_getParameter_joinDutyCycle_rpt;

typedef struct {
   uint8_t    RC;
   uint8_t    serviceId;
   uint8_t    serviceState;
   uint8_t    serviceFlags;
   uint8_t    appDomain;
   uint16_t   destAddr;
   uint32_t   time;
} dn_whmt_getParameter_service_rpt;

typedef struct {
   uint8_t    RC;
   uint8_t    apiVersion;
   uint8_t    serialNum[8];
   uint8_t    hwModel;
   uint8_t    hwRev;
   uint8_t    swMajorRev;
   uint8_t    swMinorRev;
   uint8_t    swPatch;
   uint16_t   swBuild;
} dn_whmt_getParameter_moteInfo_rpt;

typedef struct {
   uint8_t    RC;
   uint8_t    macAddress[8];
   uint16_t   moteId;
   uint16_t   networkId;
} dn_whmt_getParameter_networkInfo_rpt;

typedef struct {
   uint8_t    RC;
   uint8_t    state;
   uint8_t    moteStateReason;
   uint16_t   changeCounter;
   uint8_t    numParents;
   uint32_t   moteAlarms;
   uint8_t    statusFlags;
} dn_whmt_getParameter_moteStatus_rpt;

typedef struct {
   uint8_t    RC;
   uint8_t    utcTime[8];
   uint8_t    asn[5];
   uint16_t   asnOffset;
} dn_whmt_getParameter_time_rpt;

typedef struct {
   uint8_t    RC;
   uint32_t   charge;
   uint32_t   uptime;
   int8_t     temperature;
   uint8_t    fractionalTemp;
} dn_whmt_getParameter_charge_rpt;

typedef struct {
   uint8_t    RC;
   uint16_t   rxOk;
   uint16_t   rxFailed;
} dn_whmt_getParameter_testRadioRxStats_rpt;

typedef struct {
   uint8_t    RC;
   uint8_t    code;
   uint16_t   master;
} dn_whmt_getParameter_lock_rpt;

typedef struct {
   uint8_t    RC;
} dn_whmt_setNVParameter_macAddress_rpt;

typedef struct {
   uint8_t    RC;
} dn_whmt_setNVParameter_joinKey_rpt;

typedef struct {
   uint8_t    RC;
} dn_whmt_setNVParameter_networkId_rpt;

typedef struct {
   uint8_t    RC;
} dn_whmt_setNVParameter_txPower_rpt;

typedef struct {
   uint8_t    RC;
} dn_whmt_setNVParameter_powerInfo_rpt;

typedef struct {
   uint8_t    RC;
} dn_whmt_setNVParameter_ttl_rpt;

typedef struct {
   uint8_t    RC;
} dn_whmt_setNVParameter_HARTantennaGain_rpt;

typedef struct {
   uint8_t    RC;
} dn_whmt_setNVParameter_OTAPlockout_rpt;

typedef struct {
   uint8_t    RC;
} dn_whmt_setNVParameter_hrCounterMode_rpt;

typedef struct {
   uint8_t    RC;
   uint8_t    nvParamId;
} dn_whmt_setNVParameter_autojoin_rpt;

typedef struct {
   uint8_t    RC;
} dn_whmt_setNVParameter_compliantMode_rpt;

typedef struct {
   uint8_t    RC;
} dn_whmt_setNVParameter_lock_rpt;

typedef struct {
   uint8_t    RC;
   uint8_t    macAddr[8];
} dn_whmt_getNVParameter_macAddress_rpt;

typedef struct {
   uint8_t    RC;
   uint16_t   networkId;
} dn_whmt_getNVParameter_networkId_rpt;

typedef struct {
   uint8_t    RC;
   int8_t     txPower;
} dn_whmt_getNVParameter_txPower_rpt;

typedef struct {
   uint8_t    RC;
   uint8_t    powerSource;
   uint16_t   dischargeCur;
   uint32_t   dischargeTime;
   uint32_t   recoverTime;
} dn_whmt_getNVParameter_powerInfo_rpt;

typedef struct {
   uint8_t    RC;
   uint8_t    timeToLive;
} dn_whmt_getNVParameter_ttl_rpt;

typedef struct {
   uint8_t    RC;
   int8_t     antennaGain;
} dn_whmt_getNVParameter_HARTantennaGain_rpt;

typedef struct {
   uint8_t    RC;
   uint8_t    otapLockout;
} dn_whmt_getNVParameter_OTAPlockout_rpt;

typedef struct {
   uint8_t    RC;
   uint8_t    hrCounterMode;
} dn_whmt_getNVParameter_hrCounterMode_rpt;

typedef struct {
   uint8_t    RC;
   uint8_t    nvParamId;
   uint8_t    autojoin;
} dn_whmt_getNVParameter_autojoin_rpt;

typedef struct {
   uint8_t    RC;
   uint8_t    compliantMode;
} dn_whmt_getNVParameter_compliantMode_rpt;

typedef struct {
   uint8_t    RC;
   uint8_t    code;
   uint16_t   master;
} dn_whmt_getNVParameter_lock_rpt;

typedef struct {
   uint8_t    RC;
} dn_whmt_send_rpt;

typedef struct {
   uint8_t    RC;
} dn_whmt_join_rpt;

typedef struct {
   uint8_t    RC;
} dn_whmt_disconnect_rpt;

typedef struct {
   uint8_t    RC;
} dn_whmt_reset_rpt;

typedef struct {
   uint8_t    RC;
} dn_whmt_lowPowerSleep_rpt;

typedef struct {
   uint8_t    RC;
   uint8_t    payloadLen;
   uint8_t    payload[MAX_FRAME_LENGTH];
} dn_whmt_hartPayload_rpt;

typedef struct {
   uint8_t    RC;
} dn_whmt_testRadioTx_rpt;

typedef struct {
   uint8_t    RC;
} dn_whmt_testRadioRx_rpt;

typedef struct {
   uint8_t    RC;
} dn_whmt_clearNV_rpt;

typedef struct {
   uint8_t    RC;
} dn_whmt_search_rpt;

typedef struct {
   uint8_t    RC;
} dn_whmt_testRadioTxExt_rpt;

typedef struct {
   uint8_t    RC;
} dn_whmt_testRadioRxExt_rpt;

typedef struct {
   uint8_t    RC;
} dn_whmt_zeroize_rpt;

typedef struct {
   uint8_t    RC;
   int32_t    length;
} dn_whmt_fileWrite_rpt;

typedef struct {
   uint8_t    RC;
   int32_t    descriptor;
   uint16_t   offset;
   uint8_t    length;
   uint8_t    data[MAX_FRAME_LENGTH];
} dn_whmt_fileRead_rpt;

typedef struct {
   uint8_t    RC;
   int32_t    descriptor;
} dn_whmt_fileOpen_rpt;

//=== notification types

typedef struct {
   uint32_t   utcSec;
   uint32_t   utcMicroSec;
   uint8_t    asn[5];
   uint16_t   asnOffset;
} dn_whmt_timeIndication_nt;

typedef struct {
   uint8_t    eventCode;
   uint8_t    netMgrCode;
   uint8_t    serviceId;
   uint8_t    serviceState;
   uint8_t    serviceFlags;
   uint8_t    appDomain;
   uint16_t   destAddr;
   uint32_t   time;
} dn_whmt_serviceIndication_nt;

typedef struct {
   uint32_t   events;
   uint8_t    state;
   uint32_t   moteAlarms;
} dn_whmt_events_nt;

typedef struct {
   uint8_t    tranType;
   uint8_t    tranDir;
   uint16_t   srcAddr;
   uint8_t    seqNum;
   uint8_t    pktLength;
   uint8_t    data[MAX_FRAME_LENGTH];
} dn_whmt_dataReceived_nt;

typedef struct {
   uint16_t   netId;
   uint16_t   moteid;
   int8_t     rssi;
   uint8_t    joinPri;
} dn_whmt_advReceived_nt;

typedef struct {
   uint32_t   duration;
} dn_whmt_suspendStarted_nt;

//=== callback signature
typedef void (*dn_whmt_notif_cbt)(uint8_t cmdId, uint8_t subCmdId);
typedef void (*dn_whmt_reply_cbt)(uint8_t cmdId);
typedef void (*dn_whmt_status_cbt)(uint8_t newStatus); // only used in SmartMesh IP manager

//=========================== variables =======================================

//=========================== prototypes ======================================

#ifdef __cplusplus
 extern "C" {
#endif

//==== admin
void     dn_whmt_init(dn_whmt_notif_cbt notifCb, uint8_t* notifBuf, uint8_t notifBufLen, dn_whmt_reply_cbt replyCb);
void     dn_whmt_cancelTx();


//==== API
dn_err_t dn_whmt_setParameter_txPower(int8_t txPower, dn_whmt_setParameter_txPower_rpt* reply);
dn_err_t dn_whmt_setParameter_joinDutyCycle(uint8_t dutyCycle, dn_whmt_setParameter_joinDutyCycle_rpt* reply);
dn_err_t dn_whmt_setParameter_batteryLife(uint16_t batteryLife, uint8_t powerStatus, dn_whmt_setParameter_batteryLife_rpt* reply);
dn_err_t dn_whmt_setParameter_service(uint8_t serviceId, uint8_t serviceReqFlags, uint8_t appDomain, uint16_t destAddr, uint32_t time, dn_whmt_setParameter_service_rpt* reply);
dn_err_t dn_whmt_setParameter_hartDeviceStatus(uint8_t hartDevStatus, dn_whmt_setParameter_hartDeviceStatus_rpt* reply);
dn_err_t dn_whmt_setParameter_hartDeviceInfo(uint8_t* hartCmd0, uint8_t* hartCmd20, dn_whmt_setParameter_hartDeviceInfo_rpt* reply);
dn_err_t dn_whmt_setParameter_eventMask(uint32_t eventMask, dn_whmt_setParameter_eventMask_rpt* reply);
dn_err_t dn_whmt_setParameter_writeProtect(uint8_t writeProtect, dn_whmt_setParameter_writeProtect_rpt* reply);
dn_err_t dn_whmt_setParameter_lock(uint8_t code, uint16_t master, dn_whmt_setParameter_lock_rpt* reply);
dn_err_t dn_whmt_getParameter_joinDutyCycle(dn_whmt_getParameter_joinDutyCycle_rpt* reply);
dn_err_t dn_whmt_getParameter_service(uint8_t serviceId, dn_whmt_getParameter_service_rpt* reply);
dn_err_t dn_whmt_getParameter_moteInfo(dn_whmt_getParameter_moteInfo_rpt* reply);
dn_err_t dn_whmt_getParameter_networkInfo(dn_whmt_getParameter_networkInfo_rpt* reply);
dn_err_t dn_whmt_getParameter_moteStatus(dn_whmt_getParameter_moteStatus_rpt* reply);
dn_err_t dn_whmt_getParameter_time(dn_whmt_getParameter_time_rpt* reply);
dn_err_t dn_whmt_getParameter_charge(dn_whmt_getParameter_charge_rpt* reply);
dn_err_t dn_whmt_getParameter_testRadioRxStats(dn_whmt_getParameter_testRadioRxStats_rpt* reply);
dn_err_t dn_whmt_getParameter_lock(dn_whmt_getParameter_lock_rpt* reply);
dn_err_t dn_whmt_setNVParameter_macAddress(uint8_t memory, uint8_t* macAddr, dn_whmt_setNVParameter_macAddress_rpt* reply);
dn_err_t dn_whmt_setNVParameter_joinKey(uint8_t memory, uint8_t* joinKey, dn_whmt_setNVParameter_joinKey_rpt* reply);
dn_err_t dn_whmt_setNVParameter_networkId(uint8_t memory, uint16_t networkId, dn_whmt_setNVParameter_networkId_rpt* reply);
dn_err_t dn_whmt_setNVParameter_txPower(uint8_t memory, int8_t txPower, dn_whmt_setNVParameter_txPower_rpt* reply);
dn_err_t dn_whmt_setNVParameter_powerInfo(uint8_t memory, uint8_t powerSource, uint16_t dischargeCur, uint32_t dischargeTime, uint32_t recoverTime, dn_whmt_setNVParameter_powerInfo_rpt* reply);
dn_err_t dn_whmt_setNVParameter_ttl(uint8_t memory, uint8_t timeToLive, dn_whmt_setNVParameter_ttl_rpt* reply);
dn_err_t dn_whmt_setNVParameter_HARTantennaGain(uint8_t memory, int8_t antennaGain, dn_whmt_setNVParameter_HARTantennaGain_rpt* reply);
dn_err_t dn_whmt_setNVParameter_OTAPlockout(uint8_t memory, uint8_t otapLockout, dn_whmt_setNVParameter_OTAPlockout_rpt* reply);
dn_err_t dn_whmt_setNVParameter_hrCounterMode(uint8_t memory, uint8_t hrCounterMode, dn_whmt_setNVParameter_hrCounterMode_rpt* reply);
dn_err_t dn_whmt_setNVParameter_autojoin(uint8_t memory, uint32_t reserved, uint8_t nvParamId, uint8_t autojoin, dn_whmt_setNVParameter_autojoin_rpt* reply);
dn_err_t dn_whmt_setNVParameter_compliantMode(uint8_t memory, uint8_t compliantMode, dn_whmt_setNVParameter_compliantMode_rpt* reply);
dn_err_t dn_whmt_setNVParameter_lock(uint8_t memory, uint8_t code, uint16_t master, dn_whmt_setNVParameter_lock_rpt* reply);
dn_err_t dn_whmt_getNVParameter_macAddress(dn_whmt_getNVParameter_macAddress_rpt* reply);
dn_err_t dn_whmt_getNVParameter_networkId(dn_whmt_getNVParameter_networkId_rpt* reply);
dn_err_t dn_whmt_getNVParameter_txPower(dn_whmt_getNVParameter_txPower_rpt* reply);
dn_err_t dn_whmt_getNVParameter_powerInfo(dn_whmt_getNVParameter_powerInfo_rpt* reply);
dn_err_t dn_whmt_getNVParameter_ttl(dn_whmt_getNVParameter_ttl_rpt* reply);
dn_err_t dn_whmt_getNVParameter_HARTantennaGain(dn_whmt_getNVParameter_HARTantennaGain_rpt* reply);
dn_err_t dn_whmt_getNVParameter_OTAPlockout(dn_whmt_getNVParameter_OTAPlockout_rpt* reply);
dn_err_t dn_whmt_getNVParameter_hrCounterMode(dn_whmt_getNVParameter_hrCounterMode_rpt* reply);
dn_err_t dn_whmt_getNVParameter_autojoin(uint32_t reserved, uint8_t nvParamId, dn_whmt_getNVParameter_autojoin_rpt* reply);
dn_err_t dn_whmt_getNVParameter_compliantMode(dn_whmt_getNVParameter_compliantMode_rpt* reply);
dn_err_t dn_whmt_getNVParameter_lock(dn_whmt_getNVParameter_lock_rpt* reply);
dn_err_t dn_whmt_send(bool tranType, bool tranDir, uint16_t destAddr, uint8_t serviceId, uint8_t appDomain, uint8_t priority, uint16_t reserved, uint8_t seqNum, uint8_t payloadLen, uint8_t* payload, dn_whmt_send_rpt* reply);
dn_err_t dn_whmt_join(dn_whmt_join_rpt* reply);
dn_err_t dn_whmt_disconnect(dn_whmt_disconnect_rpt* reply);
dn_err_t dn_whmt_reset(dn_whmt_reset_rpt* reply);
dn_err_t dn_whmt_lowPowerSleep(dn_whmt_lowPowerSleep_rpt* reply);
dn_err_t dn_whmt_hartPayload(uint8_t payloadLen, uint8_t* payload, dn_whmt_hartPayload_rpt* reply);
dn_err_t dn_whmt_testRadioTx(uint8_t channel, uint16_t numPackets, dn_whmt_testRadioTx_rpt* reply);
dn_err_t dn_whmt_testRadioRx(uint8_t channel, uint16_t time, dn_whmt_testRadioRx_rpt* reply);
dn_err_t dn_whmt_clearNV(dn_whmt_clearNV_rpt* reply);
dn_err_t dn_whmt_search(dn_whmt_search_rpt* reply);
dn_err_t dn_whmt_testRadioTxExt(uint8_t testType, uint16_t chanMask, uint16_t repeatCnt, int8_t txPower, uint8_t seqSize, uint8_t pkLen_1, uint16_t delay_1, uint8_t pkLen_2, uint16_t delay_2, uint8_t pkLen_3, uint16_t delay_3, uint8_t pkLen_4, uint16_t delay_4, uint8_t pkLen_5, uint16_t delay_5, uint8_t pkLen_6, uint16_t delay_6, uint8_t pkLen_7, uint16_t delay_7, uint8_t pkLen_8, uint16_t delay_8, uint8_t pkLen_9, uint16_t delay_9, uint8_t pkLen_10, uint16_t delay_10, uint8_t stationId, dn_whmt_testRadioTxExt_rpt* reply);
dn_err_t dn_whmt_testRadioRxExt(uint16_t channelMask, uint16_t time, uint8_t stationId, dn_whmt_testRadioRxExt_rpt* reply);
dn_err_t dn_whmt_zeroize(dn_whmt_zeroize_rpt* reply);
dn_err_t dn_whmt_fileWrite(int32_t descriptor, uint16_t offset, uint8_t length, uint8_t* data, uint8_t dataLen, dn_whmt_fileWrite_rpt* reply);
dn_err_t dn_whmt_fileRead(int32_t descriptor, uint16_t offset, uint8_t length, dn_whmt_fileRead_rpt* reply);
dn_err_t dn_whmt_fileOpen(uint8_t* name, uint8_t options, uint16_t size, uint8_t mode, dn_whmt_fileOpen_rpt* reply);

#ifdef __cplusplus
}
#endif

#endif
