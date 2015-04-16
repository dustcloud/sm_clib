/*
Copyright (c) 2015, Dust Networks. All rights reserved.

C library to connect to a SmartMesh IP Manager.

\license See attached DN_LICENSE.txt.
*/

#ifndef dn_ipmg_h
#define dn_ipmg_h

#include "dn_common.h"
#include "dn_endianness.h"
#include "dn_clib_version.h"

//=========================== defines =========================================

#define MAX_FRAME_LENGTH                    128
#define DN_SUBCMDID_NONE                    0xff



//===== command IDs (requests)
#define CMDID_RESET                         0x15
#define CMDID_SUBSCRIBE                     0x16
#define CMDID_GETTIME                       0x17
#define CMDID_SETNETWORKCONFIG              0x1a
#define CMDID_CLEARSTATISTICS               0x1f
#define CMDID_EXCHANGEMOTEJOINKEY           0x21
#define CMDID_EXCHANGENETWORKID             0x22
#define CMDID_RADIOTESTTX                   0x23
#define CMDID_RADIOTESTRX                   0x25
#define CMDID_GETRADIOTESTSTATISTICS        0x26
#define CMDID_SETACLENTRY                   0x27
#define CMDID_GETNEXTACLENTRY               0x28
#define CMDID_DELETEACLENTRY                0x29
#define CMDID_PINGMOTE                      0x2a
#define CMDID_GETLOG                        0x2b
#define CMDID_SENDDATA                      0x2c
#define CMDID_STARTNETWORK                  0x2d
#define CMDID_GETSYSTEMINFO                 0x2e
#define CMDID_GETMOTECONFIG                 0x2f
#define CMDID_GETPATHINFO                   0x30
#define CMDID_GETNEXTPATHINFO               0x31
#define CMDID_SETADVERTISING                0x32
#define CMDID_SETDOWNSTREAMFRAMEMODE        0x33
#define CMDID_GETMANAGERSTATISTICS          0x35
#define CMDID_SETTIME                       0x36
#define CMDID_GETLICENSE                    0x37
#define CMDID_SETLICENSE                    0x38
#define CMDID_SETCLIUSER                    0x3a
#define CMDID_SENDIP                        0x3b
#define CMDID_RESTOREFACTORYDEFAULTS        0x3d
#define CMDID_GETMOTEINFO                   0x3e
#define CMDID_GETNETWORKCONFIG              0x3f
#define CMDID_GETNETWORKINFO                0x40
#define CMDID_GETMOTECONFIGBYID             0x41
#define CMDID_SETCOMMONJOINKEY              0x42
#define CMDID_GETIPCONFIG                   0x43
#define CMDID_SETIPCONFIG                   0x44
#define CMDID_DELETEMOTE                    0x45
#define CMDID_GETMOTELINKS                  0x46

//===== command IDs (notifications)
// DN_CMDID_* from notifications
#define CMDID_NOTIFICATION                  0x14
// DN_NOTIFID_*
#define DN_NOTIFID_NOTIFEVENT               0x1
#define DN_NOTIFID_NOTIFLOG                 0x2
#define DN_NOTIFID_NOTIFDATA                0x4
#define DN_NOTIFID_NOTIFIPDATA              0x5
#define DN_NOTIFID_NOTIFHEALTHREPORT        0x6
// DN_EVENTID_*
#define DN_EVENTID_EVENTMOTERESET           0x0
#define DN_EVENTID_EVENTNETWORKRESET        0x1
#define DN_EVENTID_EVENTCOMMANDFINISHED     0x2
#define DN_EVENTID_EVENTMOTEJOIN            0x3
#define DN_EVENTID_EVENTMOTEOPERATIONAL     0x4
#define DN_EVENTID_EVENTMOTELOST            0x5
#define DN_EVENTID_EVENTNETWORKTIME         0x6
#define DN_EVENTID_EVENTPINGRESPONSE        0x7
#define DN_EVENTID_EVENTPATHCREATE          0xa
#define DN_EVENTID_EVENTPATHDELETE          0xb
#define DN_EVENTID_EVENTPACKETSENT          0xc
#define DN_EVENTID_EVENTMOTECREATE          0xd
#define DN_EVENTID_EVENTMOTEDELETE          0xe

//===== parameter IDs
// The SmartMesh IP Manager API has no param IDs

//===== format of requests

// reset
#define DN_RESET_REQ_OFFS_TYPE                                       0
#define DN_RESET_REQ_OFFS_MACADDRESS                                 1
#define DN_RESET_REQ_LEN                                             9

// subscribe
#define DN_SUBSCRIBE_REQ_OFFS_FILTER                                 0
#define DN_SUBSCRIBE_REQ_OFFS_UNACKFILTER                            4
#define DN_SUBSCRIBE_REQ_LEN                                         8

// getTime
#define DN_GETTIME_REQ_LEN                                           0

// setNetworkConfig
#define DN_SETNETWORKCONFIG_REQ_OFFS_NETWORKID                       0
#define DN_SETNETWORKCONFIG_REQ_OFFS_APTXPOWER                       2
#define DN_SETNETWORKCONFIG_REQ_OFFS_FRAMEPROFILE                    3
#define DN_SETNETWORKCONFIG_REQ_OFFS_MAXMOTES                        4
#define DN_SETNETWORKCONFIG_REQ_OFFS_BASEBANDWIDTH                   6
#define DN_SETNETWORKCONFIG_REQ_OFFS_DOWNFRAMEMULTVAL                8
#define DN_SETNETWORKCONFIG_REQ_OFFS_NUMPARENTS                      9
#define DN_SETNETWORKCONFIG_REQ_OFFS_CCAMODE                         10
#define DN_SETNETWORKCONFIG_REQ_OFFS_CHANNELLIST                     11
#define DN_SETNETWORKCONFIG_REQ_OFFS_AUTOSTARTNETWORK                13
#define DN_SETNETWORKCONFIG_REQ_OFFS_LOCMODE                         14
#define DN_SETNETWORKCONFIG_REQ_OFFS_BBMODE                          15
#define DN_SETNETWORKCONFIG_REQ_OFFS_BBSIZE                          16
#define DN_SETNETWORKCONFIG_REQ_OFFS_ISRADIOTEST                     17
#define DN_SETNETWORKCONFIG_REQ_OFFS_BWMULT                          18
#define DN_SETNETWORKCONFIG_REQ_OFFS_ONECHANNEL                      20
#define DN_SETNETWORKCONFIG_REQ_LEN                                  21

// clearStatistics
#define DN_CLEARSTATISTICS_REQ_LEN                                   0

// exchangeMoteJoinKey
#define DN_EXCHANGEMOTEJOINKEY_REQ_OFFS_MACADDRESS                   0
#define DN_EXCHANGEMOTEJOINKEY_REQ_OFFS_KEY                          8
#define DN_EXCHANGEMOTEJOINKEY_REQ_LEN                               24

// exchangeNetworkId
#define DN_EXCHANGENETWORKID_REQ_OFFS_ID                             0
#define DN_EXCHANGENETWORKID_REQ_LEN                                 2

// radiotestTx
#define DN_RADIOTESTTX_REQ_OFFS_TESTTYPE                             0
#define DN_RADIOTESTTX_REQ_OFFS_CHANMASK                             1
#define DN_RADIOTESTTX_REQ_OFFS_REPEATCNT                            3
#define DN_RADIOTESTTX_REQ_OFFS_TXPOWER                              5
#define DN_RADIOTESTTX_REQ_OFFS_SEQSIZE                              6
#define DN_RADIOTESTTX_REQ_OFFS_PKLEN_1                              7
#define DN_RADIOTESTTX_REQ_OFFS_DELAY_1                              8
#define DN_RADIOTESTTX_REQ_OFFS_PKLEN_2                              10
#define DN_RADIOTESTTX_REQ_OFFS_DELAY_2                              11
#define DN_RADIOTESTTX_REQ_OFFS_PKLEN_3                              13
#define DN_RADIOTESTTX_REQ_OFFS_DELAY_3                              14
#define DN_RADIOTESTTX_REQ_OFFS_PKLEN_4                              16
#define DN_RADIOTESTTX_REQ_OFFS_DELAY_4                              17
#define DN_RADIOTESTTX_REQ_OFFS_PKLEN_5                              19
#define DN_RADIOTESTTX_REQ_OFFS_DELAY_5                              20
#define DN_RADIOTESTTX_REQ_OFFS_PKLEN_6                              22
#define DN_RADIOTESTTX_REQ_OFFS_DELAY_6                              23
#define DN_RADIOTESTTX_REQ_OFFS_PKLEN_7                              25
#define DN_RADIOTESTTX_REQ_OFFS_DELAY_7                              26
#define DN_RADIOTESTTX_REQ_OFFS_PKLEN_8                              28
#define DN_RADIOTESTTX_REQ_OFFS_DELAY_8                              29
#define DN_RADIOTESTTX_REQ_OFFS_PKLEN_9                              31
#define DN_RADIOTESTTX_REQ_OFFS_DELAY_9                              32
#define DN_RADIOTESTTX_REQ_OFFS_PKLEN_10                             34
#define DN_RADIOTESTTX_REQ_OFFS_DELAY_10                             35
#define DN_RADIOTESTTX_REQ_OFFS_STATIONID                            37
#define DN_RADIOTESTTX_REQ_LEN                                       38

// radiotestRx
#define DN_RADIOTESTRX_REQ_OFFS_MASK                                 0
#define DN_RADIOTESTRX_REQ_OFFS_DURATION                             2
#define DN_RADIOTESTRX_REQ_OFFS_STATIONID                            4
#define DN_RADIOTESTRX_REQ_LEN                                       5

// getRadiotestStatistics
#define DN_GETRADIOTESTSTATISTICS_REQ_LEN                            0

// setACLEntry
#define DN_SETACLENTRY_REQ_OFFS_MACADDRESS                           0
#define DN_SETACLENTRY_REQ_OFFS_JOINKEY                              8
#define DN_SETACLENTRY_REQ_LEN                                       24

// getNextACLEntry
#define DN_GETNEXTACLENTRY_REQ_OFFS_MACADDRESS                       0
#define DN_GETNEXTACLENTRY_REQ_LEN                                   8

// deleteACLEntry
#define DN_DELETEACLENTRY_REQ_OFFS_MACADDRESS                        0
#define DN_DELETEACLENTRY_REQ_LEN                                    8

// pingMote
#define DN_PINGMOTE_REQ_OFFS_MACADDRESS                              0
#define DN_PINGMOTE_REQ_LEN                                          8

// getLog
#define DN_GETLOG_REQ_OFFS_MACADDRESS                                0
#define DN_GETLOG_REQ_LEN                                            8

// sendData
#define DN_SENDDATA_REQ_OFFS_MACADDRESS                              0
#define DN_SENDDATA_REQ_OFFS_PRIORITY                                8
#define DN_SENDDATA_REQ_OFFS_SRCPORT                                 9
#define DN_SENDDATA_REQ_OFFS_DSTPORT                                 11
#define DN_SENDDATA_REQ_OFFS_OPTIONS                                 13
#define DN_SENDDATA_REQ_OFFS_DATA                                    14
#define DN_SENDDATA_REQ_LEN                                          14

// startNetwork
#define DN_STARTNETWORK_REQ_LEN                                      0

// getSystemInfo
#define DN_GETSYSTEMINFO_REQ_LEN                                     0

// getMoteConfig
#define DN_GETMOTECONFIG_REQ_OFFS_MACADDRESS                         0
#define DN_GETMOTECONFIG_REQ_OFFS_NEXT                               8
#define DN_GETMOTECONFIG_REQ_LEN                                     9

// getPathInfo
#define DN_GETPATHINFO_REQ_OFFS_SOURCE                               0
#define DN_GETPATHINFO_REQ_OFFS_DEST                                 8
#define DN_GETPATHINFO_REQ_LEN                                       16

// getNextPathInfo
#define DN_GETNEXTPATHINFO_REQ_OFFS_MACADDRESS                       0
#define DN_GETNEXTPATHINFO_REQ_OFFS_FILTER                           8
#define DN_GETNEXTPATHINFO_REQ_OFFS_PATHID                           9
#define DN_GETNEXTPATHINFO_REQ_LEN                                   11

// setAdvertising
#define DN_SETADVERTISING_REQ_OFFS_ACTIVATE                          0
#define DN_SETADVERTISING_REQ_LEN                                    1

// setDownstreamFrameMode
#define DN_SETDOWNSTREAMFRAMEMODE_REQ_OFFS_FRAMEMODE                 0
#define DN_SETDOWNSTREAMFRAMEMODE_REQ_LEN                            1

// getManagerStatistics
#define DN_GETMANAGERSTATISTICS_REQ_LEN                              0

// setTime
#define DN_SETTIME_REQ_OFFS_TRIGGER                                  0
#define DN_SETTIME_REQ_OFFS_UTCSECS                                  1
#define DN_SETTIME_REQ_OFFS_UTCUSECS                                 9
#define DN_SETTIME_REQ_LEN                                           13

// getLicense
#define DN_GETLICENSE_REQ_LEN                                        0

// setLicense
#define DN_SETLICENSE_REQ_OFFS_LICENSE                               0
#define DN_SETLICENSE_REQ_LEN                                        13

// setCLIUser
#define DN_SETCLIUSER_REQ_OFFS_ROLE                                  0
#define DN_SETCLIUSER_REQ_OFFS_PASSWORD                              1
#define DN_SETCLIUSER_REQ_LEN                                        17

// sendIP
#define DN_SENDIP_REQ_OFFS_MACADDRESS                                0
#define DN_SENDIP_REQ_OFFS_PRIORITY                                  8
#define DN_SENDIP_REQ_OFFS_OPTIONS                                   9
#define DN_SENDIP_REQ_OFFS_ENCRYPTEDOFFSET                           10
#define DN_SENDIP_REQ_OFFS_DATA                                      11
#define DN_SENDIP_REQ_LEN                                            11

// restoreFactoryDefaults
#define DN_RESTOREFACTORYDEFAULTS_REQ_LEN                            0

// getMoteInfo
#define DN_GETMOTEINFO_REQ_OFFS_MACADDRESS                           0
#define DN_GETMOTEINFO_REQ_LEN                                       8

// getNetworkConfig
#define DN_GETNETWORKCONFIG_REQ_LEN                                  0

// getNetworkInfo
#define DN_GETNETWORKINFO_REQ_LEN                                    0

// getMoteConfigById
#define DN_GETMOTECONFIGBYID_REQ_OFFS_MOTEID                         0
#define DN_GETMOTECONFIGBYID_REQ_LEN                                 2

// setCommonJoinKey
#define DN_SETCOMMONJOINKEY_REQ_OFFS_KEY                             0
#define DN_SETCOMMONJOINKEY_REQ_LEN                                  16

// getIPConfig
#define DN_GETIPCONFIG_REQ_LEN                                       0

// setIPConfig
#define DN_SETIPCONFIG_REQ_OFFS_IPV6ADDRESS                          0
#define DN_SETIPCONFIG_REQ_OFFS_MASK                                 16
#define DN_SETIPCONFIG_REQ_LEN                                       32

// deleteMote
#define DN_DELETEMOTE_REQ_OFFS_MACADDRESS                            0
#define DN_DELETEMOTE_REQ_LEN                                        8

// getMoteLinks
#define DN_GETMOTELINKS_REQ_OFFS_MACADDRESS                          0
#define DN_GETMOTELINKS_REQ_OFFS_IDX                                 8
#define DN_GETMOTELINKS_REQ_LEN                                      10

//===== format of replies

// reset
#define DN_RESET_REPLY_OFFS_MACADDRESS                               0
#define DN_RESET_REPLY_LEN                                           8

// subscribe
#define DN_SUBSCRIBE_REPLY_LEN                                       0

// getTime
#define DN_GETTIME_REPLY_OFFS_UPTIME                                 0
#define DN_GETTIME_REPLY_OFFS_UTCSECS                                4
#define DN_GETTIME_REPLY_OFFS_UTCUSECS                               12
#define DN_GETTIME_REPLY_OFFS_ASN                                    16
#define DN_GETTIME_REPLY_OFFS_ASNOFFSET                              21
#define DN_GETTIME_REPLY_LEN                                         23

// setNetworkConfig
#define DN_SETNETWORKCONFIG_REPLY_LEN                                0

// clearStatistics
#define DN_CLEARSTATISTICS_REPLY_LEN                                 0

// exchangeMoteJoinKey
#define DN_EXCHANGEMOTEJOINKEY_REPLY_OFFS_CALLBACKID                 0
#define DN_EXCHANGEMOTEJOINKEY_REPLY_LEN                             4

// exchangeNetworkId
#define DN_EXCHANGENETWORKID_REPLY_OFFS_CALLBACKID                   0
#define DN_EXCHANGENETWORKID_REPLY_LEN                               4

// radiotestTx
#define DN_RADIOTESTTX_REPLY_LEN                                     0

// radiotestRx
#define DN_RADIOTESTRX_REPLY_LEN                                     0

// getRadiotestStatistics
#define DN_GETRADIOTESTSTATISTICS_REPLY_OFFS_RXOK                    0
#define DN_GETRADIOTESTSTATISTICS_REPLY_OFFS_RXFAIL                  2
#define DN_GETRADIOTESTSTATISTICS_REPLY_LEN                          4

// setACLEntry
#define DN_SETACLENTRY_REPLY_LEN                                     0

// getNextACLEntry
#define DN_GETNEXTACLENTRY_REPLY_OFFS_MACADDRESS                     0
#define DN_GETNEXTACLENTRY_REPLY_OFFS_JOINKEY                        8
#define DN_GETNEXTACLENTRY_REPLY_LEN                                 24

// deleteACLEntry
#define DN_DELETEACLENTRY_REPLY_LEN                                  0

// pingMote
#define DN_PINGMOTE_REPLY_OFFS_CALLBACKID                            0
#define DN_PINGMOTE_REPLY_LEN                                        4

// getLog
#define DN_GETLOG_REPLY_LEN                                          0

// sendData
#define DN_SENDDATA_REPLY_OFFS_CALLBACKID                            0
#define DN_SENDDATA_REPLY_LEN                                        4

// startNetwork
#define DN_STARTNETWORK_REPLY_LEN                                    0

// getSystemInfo
#define DN_GETSYSTEMINFO_REPLY_OFFS_MACADDRESS                       0
#define DN_GETSYSTEMINFO_REPLY_OFFS_HWMODEL                          8
#define DN_GETSYSTEMINFO_REPLY_OFFS_HWREV                            9
#define DN_GETSYSTEMINFO_REPLY_OFFS_SWMAJOR                          10
#define DN_GETSYSTEMINFO_REPLY_OFFS_SWMINOR                          11
#define DN_GETSYSTEMINFO_REPLY_OFFS_SWPATCH                          12
#define DN_GETSYSTEMINFO_REPLY_OFFS_SWBUILD                          13
#define DN_GETSYSTEMINFO_REPLY_LEN                                   15

// getMoteConfig
#define DN_GETMOTECONFIG_REPLY_OFFS_MACADDRESS                       0
#define DN_GETMOTECONFIG_REPLY_OFFS_MOTEID                           8
#define DN_GETMOTECONFIG_REPLY_OFFS_ISAP                             10
#define DN_GETMOTECONFIG_REPLY_OFFS_STATE                            11
#define DN_GETMOTECONFIG_REPLY_OFFS_RESERVED                         12
#define DN_GETMOTECONFIG_REPLY_OFFS_ISROUTING                        13
#define DN_GETMOTECONFIG_REPLY_LEN                                   14

// getPathInfo
#define DN_GETPATHINFO_REPLY_OFFS_SOURCE                             0
#define DN_GETPATHINFO_REPLY_OFFS_DEST                               8
#define DN_GETPATHINFO_REPLY_OFFS_DIRECTION                          16
#define DN_GETPATHINFO_REPLY_OFFS_NUMLINKS                           17
#define DN_GETPATHINFO_REPLY_OFFS_QUALITY                            18
#define DN_GETPATHINFO_REPLY_OFFS_RSSISRCDEST                        19
#define DN_GETPATHINFO_REPLY_OFFS_RSSIDESTSRC                        20
#define DN_GETPATHINFO_REPLY_LEN                                     21

// getNextPathInfo
#define DN_GETNEXTPATHINFO_REPLY_OFFS_PATHID                         0
#define DN_GETNEXTPATHINFO_REPLY_OFFS_SOURCE                         2
#define DN_GETNEXTPATHINFO_REPLY_OFFS_DEST                           10
#define DN_GETNEXTPATHINFO_REPLY_OFFS_DIRECTION                      18
#define DN_GETNEXTPATHINFO_REPLY_OFFS_NUMLINKS                       19
#define DN_GETNEXTPATHINFO_REPLY_OFFS_QUALITY                        20
#define DN_GETNEXTPATHINFO_REPLY_OFFS_RSSISRCDEST                    21
#define DN_GETNEXTPATHINFO_REPLY_OFFS_RSSIDESTSRC                    22
#define DN_GETNEXTPATHINFO_REPLY_LEN                                 23

// setAdvertising
#define DN_SETADVERTISING_REPLY_OFFS_CALLBACKID                      0
#define DN_SETADVERTISING_REPLY_LEN                                  4

// setDownstreamFrameMode
#define DN_SETDOWNSTREAMFRAMEMODE_REPLY_OFFS_CALLBACKID              0
#define DN_SETDOWNSTREAMFRAMEMODE_REPLY_LEN                          4

// getManagerStatistics
#define DN_GETMANAGERSTATISTICS_REPLY_OFFS_SERTXCNT                  0
#define DN_GETMANAGERSTATISTICS_REPLY_OFFS_SERRXCNT                  2
#define DN_GETMANAGERSTATISTICS_REPLY_OFFS_SERRXCRCERR               4
#define DN_GETMANAGERSTATISTICS_REPLY_OFFS_SERRXOVERRUNS             6
#define DN_GETMANAGERSTATISTICS_REPLY_OFFS_APIESTABCONN              8
#define DN_GETMANAGERSTATISTICS_REPLY_OFFS_APIDROPPEDCONN            10
#define DN_GETMANAGERSTATISTICS_REPLY_OFFS_APITXOK                   12
#define DN_GETMANAGERSTATISTICS_REPLY_OFFS_APITXERR                  14
#define DN_GETMANAGERSTATISTICS_REPLY_OFFS_APITXFAIL                 16
#define DN_GETMANAGERSTATISTICS_REPLY_OFFS_APIRXOK                   18
#define DN_GETMANAGERSTATISTICS_REPLY_OFFS_APIRXPROTERR              20
#define DN_GETMANAGERSTATISTICS_REPLY_LEN                            22

// setTime
#define DN_SETTIME_REPLY_LEN                                         0

// getLicense
#define DN_GETLICENSE_REPLY_OFFS_LICENSE                             0
#define DN_GETLICENSE_REPLY_LEN                                      13

// setLicense
#define DN_SETLICENSE_REPLY_LEN                                      0

// setCLIUser
#define DN_SETCLIUSER_REPLY_LEN                                      0

// sendIP
#define DN_SENDIP_REPLY_OFFS_CALLBACKID                              0
#define DN_SENDIP_REPLY_LEN                                          4

// restoreFactoryDefaults
#define DN_RESTOREFACTORYDEFAULTS_REPLY_LEN                          0

// getMoteInfo
#define DN_GETMOTEINFO_REPLY_OFFS_MACADDRESS                         0
#define DN_GETMOTEINFO_REPLY_OFFS_STATE                              8
#define DN_GETMOTEINFO_REPLY_OFFS_NUMNBRS                            9
#define DN_GETMOTEINFO_REPLY_OFFS_NUMGOODNBRS                        10
#define DN_GETMOTEINFO_REPLY_OFFS_REQUESTEDBW                        11
#define DN_GETMOTEINFO_REPLY_OFFS_TOTALNEEDEDBW                      15
#define DN_GETMOTEINFO_REPLY_OFFS_ASSIGNEDBW                         19
#define DN_GETMOTEINFO_REPLY_OFFS_PACKETSRECEIVED                    23
#define DN_GETMOTEINFO_REPLY_OFFS_PACKETSLOST                        27
#define DN_GETMOTEINFO_REPLY_OFFS_AVGLATENCY                         31
#define DN_GETMOTEINFO_REPLY_LEN                                     35

// getNetworkConfig
#define DN_GETNETWORKCONFIG_REPLY_OFFS_NETWORKID                     0
#define DN_GETNETWORKCONFIG_REPLY_OFFS_APTXPOWER                     2
#define DN_GETNETWORKCONFIG_REPLY_OFFS_FRAMEPROFILE                  3
#define DN_GETNETWORKCONFIG_REPLY_OFFS_MAXMOTES                      4
#define DN_GETNETWORKCONFIG_REPLY_OFFS_BASEBANDWIDTH                 6
#define DN_GETNETWORKCONFIG_REPLY_OFFS_DOWNFRAMEMULTVAL              8
#define DN_GETNETWORKCONFIG_REPLY_OFFS_NUMPARENTS                    9
#define DN_GETNETWORKCONFIG_REPLY_OFFS_CCAMODE                       10
#define DN_GETNETWORKCONFIG_REPLY_OFFS_CHANNELLIST                   11
#define DN_GETNETWORKCONFIG_REPLY_OFFS_AUTOSTARTNETWORK              13
#define DN_GETNETWORKCONFIG_REPLY_OFFS_LOCMODE                       14
#define DN_GETNETWORKCONFIG_REPLY_OFFS_BBMODE                        15
#define DN_GETNETWORKCONFIG_REPLY_OFFS_BBSIZE                        16
#define DN_GETNETWORKCONFIG_REPLY_OFFS_ISRADIOTEST                   17
#define DN_GETNETWORKCONFIG_REPLY_OFFS_BWMULT                        18
#define DN_GETNETWORKCONFIG_REPLY_OFFS_ONECHANNEL                    20
#define DN_GETNETWORKCONFIG_REPLY_LEN                                21

// getNetworkInfo
#define DN_GETNETWORKINFO_REPLY_OFFS_NUMMOTES                        0
#define DN_GETNETWORKINFO_REPLY_OFFS_ASNSIZE                         2
#define DN_GETNETWORKINFO_REPLY_OFFS_ADVERTISEMENTSTATE              4
#define DN_GETNETWORKINFO_REPLY_OFFS_DOWNFRAMESTATE                  5
#define DN_GETNETWORKINFO_REPLY_OFFS_NETRELIABILITY                  6
#define DN_GETNETWORKINFO_REPLY_OFFS_NETPATHSTABILITY                7
#define DN_GETNETWORKINFO_REPLY_OFFS_NETLATENCY                      8
#define DN_GETNETWORKINFO_REPLY_OFFS_NETSTATE                        12
#define DN_GETNETWORKINFO_REPLY_OFFS_IPV6ADDRESS                     13
#define DN_GETNETWORKINFO_REPLY_LEN                                  29

// getMoteConfigById
#define DN_GETMOTECONFIGBYID_REPLY_OFFS_MACADDRESS                   0
#define DN_GETMOTECONFIGBYID_REPLY_OFFS_MOTEID                       8
#define DN_GETMOTECONFIGBYID_REPLY_OFFS_ISAP                         10
#define DN_GETMOTECONFIGBYID_REPLY_OFFS_STATE                        11
#define DN_GETMOTECONFIGBYID_REPLY_OFFS_RESERVED                     12
#define DN_GETMOTECONFIGBYID_REPLY_OFFS_ISROUTING                    13
#define DN_GETMOTECONFIGBYID_REPLY_LEN                               14

// setCommonJoinKey
#define DN_SETCOMMONJOINKEY_REPLY_LEN                                0

// getIPConfig
#define DN_GETIPCONFIG_REPLY_OFFS_IPV6ADDRESS                        0
#define DN_GETIPCONFIG_REPLY_OFFS_MASK                               16
#define DN_GETIPCONFIG_REPLY_LEN                                     32

// setIPConfig
#define DN_SETIPCONFIG_REPLY_LEN                                     0

// deleteMote
#define DN_DELETEMOTE_REPLY_LEN                                      0

// getMoteLinks
#define DN_GETMOTELINKS_REPLY_OFFS_IDX                               0
#define DN_GETMOTELINKS_REPLY_OFFS_UTILIZATION                       2
#define DN_GETMOTELINKS_REPLY_OFFS_NUMLINKS                          3
#define DN_GETMOTELINKS_REPLY_OFFS_FRAMEID_1                         4
#define DN_GETMOTELINKS_REPLY_OFFS_SLOT_1                            5
#define DN_GETMOTELINKS_REPLY_OFFS_CHANNELOFFSET_1                   9
#define DN_GETMOTELINKS_REPLY_OFFS_MOTEID_1                          10
#define DN_GETMOTELINKS_REPLY_OFFS_FLAGS_1                           12
#define DN_GETMOTELINKS_REPLY_OFFS_FRAMEID_2                         13
#define DN_GETMOTELINKS_REPLY_OFFS_SLOT_2                            14
#define DN_GETMOTELINKS_REPLY_OFFS_CHANNELOFFSET_2                   18
#define DN_GETMOTELINKS_REPLY_OFFS_MOTEID_2                          19
#define DN_GETMOTELINKS_REPLY_OFFS_FLAGS_2                           21
#define DN_GETMOTELINKS_REPLY_OFFS_FRAMEID_3                         22
#define DN_GETMOTELINKS_REPLY_OFFS_SLOT_3                            23
#define DN_GETMOTELINKS_REPLY_OFFS_CHANNELOFFSET_3                   27
#define DN_GETMOTELINKS_REPLY_OFFS_MOTEID_3                          28
#define DN_GETMOTELINKS_REPLY_OFFS_FLAGS_3                           30
#define DN_GETMOTELINKS_REPLY_OFFS_FRAMEID_4                         31
#define DN_GETMOTELINKS_REPLY_OFFS_SLOT_4                            32
#define DN_GETMOTELINKS_REPLY_OFFS_CHANNELOFFSET_4                   36
#define DN_GETMOTELINKS_REPLY_OFFS_MOTEID_4                          37
#define DN_GETMOTELINKS_REPLY_OFFS_FLAGS_4                           39
#define DN_GETMOTELINKS_REPLY_OFFS_FRAMEID_5                         40
#define DN_GETMOTELINKS_REPLY_OFFS_SLOT_5                            41
#define DN_GETMOTELINKS_REPLY_OFFS_CHANNELOFFSET_5                   45
#define DN_GETMOTELINKS_REPLY_OFFS_MOTEID_5                          46
#define DN_GETMOTELINKS_REPLY_OFFS_FLAGS_5                           48
#define DN_GETMOTELINKS_REPLY_OFFS_FRAMEID_6                         49
#define DN_GETMOTELINKS_REPLY_OFFS_SLOT_6                            50
#define DN_GETMOTELINKS_REPLY_OFFS_CHANNELOFFSET_6                   54
#define DN_GETMOTELINKS_REPLY_OFFS_MOTEID_6                          55
#define DN_GETMOTELINKS_REPLY_OFFS_FLAGS_6                           57
#define DN_GETMOTELINKS_REPLY_OFFS_FRAMEID_7                         58
#define DN_GETMOTELINKS_REPLY_OFFS_SLOT_7                            59
#define DN_GETMOTELINKS_REPLY_OFFS_CHANNELOFFSET_7                   63
#define DN_GETMOTELINKS_REPLY_OFFS_MOTEID_7                          64
#define DN_GETMOTELINKS_REPLY_OFFS_FLAGS_7                           66
#define DN_GETMOTELINKS_REPLY_OFFS_FRAMEID_8                         67
#define DN_GETMOTELINKS_REPLY_OFFS_SLOT_8                            68
#define DN_GETMOTELINKS_REPLY_OFFS_CHANNELOFFSET_8                   72
#define DN_GETMOTELINKS_REPLY_OFFS_MOTEID_8                          73
#define DN_GETMOTELINKS_REPLY_OFFS_FLAGS_8                           75
#define DN_GETMOTELINKS_REPLY_OFFS_FRAMEID_9                         76
#define DN_GETMOTELINKS_REPLY_OFFS_SLOT_9                            77
#define DN_GETMOTELINKS_REPLY_OFFS_CHANNELOFFSET_9                   81
#define DN_GETMOTELINKS_REPLY_OFFS_MOTEID_9                          82
#define DN_GETMOTELINKS_REPLY_OFFS_FLAGS_9                           84
#define DN_GETMOTELINKS_REPLY_OFFS_FRAMEID_10                        85
#define DN_GETMOTELINKS_REPLY_OFFS_SLOT_10                           86
#define DN_GETMOTELINKS_REPLY_OFFS_CHANNELOFFSET_10                  90
#define DN_GETMOTELINKS_REPLY_OFFS_MOTEID_10                         91
#define DN_GETMOTELINKS_REPLY_OFFS_FLAGS_10                          93
#define DN_GETMOTELINKS_REPLY_LEN                                    94

//===== format of notifications
//DN_NOTIFID_*
#define DN_MIN_NOTIF_LEN                                             1
#define DN_MIN_EVENT_LEN                                             5

// notifLog
#define DN_NOTIFLOG_NOTIF_OFFS_MACADDRESS                            0
#define DN_NOTIFLOG_NOTIF_OFFS_LOGMSG                                8
#define DN_NOTIFLOG_NOTIF_LEN                                        8

// notifData
#define DN_NOTIFDATA_NOTIF_OFFS_UTCSECS                              0
#define DN_NOTIFDATA_NOTIF_OFFS_UTCUSECS                             8
#define DN_NOTIFDATA_NOTIF_OFFS_MACADDRESS                           12
#define DN_NOTIFDATA_NOTIF_OFFS_SRCPORT                              20
#define DN_NOTIFDATA_NOTIF_OFFS_DSTPORT                              22
#define DN_NOTIFDATA_NOTIF_OFFS_DATA                                 24
#define DN_NOTIFDATA_NOTIF_LEN                                       24

// notifIpData
#define DN_NOTIFIPDATA_NOTIF_OFFS_UTCSECS                            0
#define DN_NOTIFIPDATA_NOTIF_OFFS_UTCUSECS                           8
#define DN_NOTIFIPDATA_NOTIF_OFFS_MACADDRESS                         12
#define DN_NOTIFIPDATA_NOTIF_OFFS_DATA                               20
#define DN_NOTIFIPDATA_NOTIF_LEN                                     20

// notifHealthReport
#define DN_NOTIFHEALTHREPORT_NOTIF_OFFS_MACADDRESS                   0
#define DN_NOTIFHEALTHREPORT_NOTIF_OFFS_PAYLOAD                      8
#define DN_NOTIFHEALTHREPORT_NOTIF_LEN                               8

// eventMoteReset
#define DN_EVENTMOTERESET_NOTIF_OFFS_MACADDRESS                      0
#define DN_EVENTMOTERESET_NOTIF_LEN                                  8

// eventNetworkReset
#define DN_EVENTNETWORKRESET_NOTIF_LEN                               0

// eventCommandFinished
#define DN_EVENTCOMMANDFINISHED_NOTIF_OFFS_CALLBACKID                0
#define DN_EVENTCOMMANDFINISHED_NOTIF_OFFS_RC                        4
#define DN_EVENTCOMMANDFINISHED_NOTIF_LEN                            5

// eventMoteJoin
#define DN_EVENTMOTEJOIN_NOTIF_OFFS_MACADDRESS                       0
#define DN_EVENTMOTEJOIN_NOTIF_LEN                                   8

// eventMoteOperational
#define DN_EVENTMOTEOPERATIONAL_NOTIF_OFFS_MACADDRESS                0
#define DN_EVENTMOTEOPERATIONAL_NOTIF_LEN                            8

// eventMoteLost
#define DN_EVENTMOTELOST_NOTIF_OFFS_MACADDRESS                       0
#define DN_EVENTMOTELOST_NOTIF_LEN                                   8

// eventNetworkTime
#define DN_EVENTNETWORKTIME_NOTIF_OFFS_UPTIME                        0
#define DN_EVENTNETWORKTIME_NOTIF_OFFS_UTCSECS                       4
#define DN_EVENTNETWORKTIME_NOTIF_OFFS_UTCUSECS                      12
#define DN_EVENTNETWORKTIME_NOTIF_OFFS_ASN                           16
#define DN_EVENTNETWORKTIME_NOTIF_OFFS_ASNOFFSET                     21
#define DN_EVENTNETWORKTIME_NOTIF_LEN                                23

// eventPingResponse
#define DN_EVENTPINGRESPONSE_NOTIF_OFFS_CALLBACKID                   0
#define DN_EVENTPINGRESPONSE_NOTIF_OFFS_MACADDRESS                   4
#define DN_EVENTPINGRESPONSE_NOTIF_OFFS_DELAY                        12
#define DN_EVENTPINGRESPONSE_NOTIF_OFFS_VOLTAGE                      16
#define DN_EVENTPINGRESPONSE_NOTIF_OFFS_TEMPERATURE                  18
#define DN_EVENTPINGRESPONSE_NOTIF_LEN                               19

// eventPathCreate
#define DN_EVENTPATHCREATE_NOTIF_OFFS_SOURCE                         0
#define DN_EVENTPATHCREATE_NOTIF_OFFS_DEST                           8
#define DN_EVENTPATHCREATE_NOTIF_OFFS_DIRECTION                      16
#define DN_EVENTPATHCREATE_NOTIF_LEN                                 17

// eventPathDelete
#define DN_EVENTPATHDELETE_NOTIF_OFFS_SOURCE                         0
#define DN_EVENTPATHDELETE_NOTIF_OFFS_DEST                           8
#define DN_EVENTPATHDELETE_NOTIF_OFFS_DIRECTION                      16
#define DN_EVENTPATHDELETE_NOTIF_LEN                                 17

// eventPacketSent
#define DN_EVENTPACKETSENT_NOTIF_OFFS_CALLBACKID                     0
#define DN_EVENTPACKETSENT_NOTIF_OFFS_RC                             4
#define DN_EVENTPACKETSENT_NOTIF_LEN                                 5

// eventMoteCreate
#define DN_EVENTMOTECREATE_NOTIF_OFFS_MACADDRESS                     0
#define DN_EVENTMOTECREATE_NOTIF_OFFS_MOTEID                         8
#define DN_EVENTMOTECREATE_NOTIF_LEN                                 10

// eventMoteDelete
#define DN_EVENTMOTEDELETE_NOTIF_OFFS_MACADDRESS                     0
#define DN_EVENTMOTEDELETE_NOTIF_OFFS_MOTEID                         8
#define DN_EVENTMOTEDELETE_NOTIF_LEN                                 10

//=========================== typedef =========================================

//=== reply types

typedef struct {
   uint8_t    RC;
   uint8_t    macAddress[8];
} dn_ipmg_reset_rpt;

typedef struct {
   uint8_t    RC;
} dn_ipmg_subscribe_rpt;

typedef struct {
   uint8_t    RC;
   uint32_t   uptime;
   uint8_t    utcSecs[8];
   uint32_t   utcUsecs;
   uint8_t    asn[5];
   uint16_t   asnOffset;
} dn_ipmg_getTime_rpt;

typedef struct {
   uint8_t    RC;
} dn_ipmg_setNetworkConfig_rpt;

typedef struct {
   uint8_t    RC;
} dn_ipmg_clearStatistics_rpt;

typedef struct {
   uint8_t    RC;
   uint32_t   callbackId;
} dn_ipmg_exchangeMoteJoinKey_rpt;

typedef struct {
   uint8_t    RC;
   uint32_t   callbackId;
} dn_ipmg_exchangeNetworkId_rpt;

typedef struct {
   uint8_t    RC;
} dn_ipmg_radiotestTx_rpt;

typedef struct {
   uint8_t    RC;
} dn_ipmg_radiotestRx_rpt;

typedef struct {
   uint8_t    RC;
   uint16_t   rxOk;
   uint16_t   rxFail;
} dn_ipmg_getRadiotestStatistics_rpt;

typedef struct {
   uint8_t    RC;
} dn_ipmg_setACLEntry_rpt;

typedef struct {
   uint8_t    RC;
   uint8_t    macAddress[8];
   uint8_t    joinKey[16];
} dn_ipmg_getNextACLEntry_rpt;

typedef struct {
   uint8_t    RC;
} dn_ipmg_deleteACLEntry_rpt;

typedef struct {
   uint8_t    RC;
   uint32_t   callbackId;
} dn_ipmg_pingMote_rpt;

typedef struct {
   uint8_t    RC;
} dn_ipmg_getLog_rpt;

typedef struct {
   uint8_t    RC;
   uint32_t   callbackId;
} dn_ipmg_sendData_rpt;

typedef struct {
   uint8_t    RC;
} dn_ipmg_startNetwork_rpt;

typedef struct {
   uint8_t    RC;
   uint8_t    macAddress[8];
   uint8_t    hwModel;
   uint8_t    hwRev;
   uint8_t    swMajor;
   uint8_t    swMinor;
   uint8_t    swPatch;
   uint16_t   swBuild;
} dn_ipmg_getSystemInfo_rpt;

typedef struct {
   uint8_t    RC;
   uint8_t    macAddress[8];
   uint16_t   moteId;
   bool       isAP;
   uint8_t    state;
   uint8_t    reserved;
   bool       isRouting;
} dn_ipmg_getMoteConfig_rpt;

typedef struct {
   uint8_t    RC;
   uint8_t    source[8];
   uint8_t    dest[8];
   uint8_t    direction;
   uint8_t    numLinks;
   uint8_t    quality;
   int8_t     rssiSrcDest;
   int8_t     rssiDestSrc;
} dn_ipmg_getPathInfo_rpt;

typedef struct {
   uint8_t    RC;
   uint16_t   pathId;
   uint8_t    source[8];
   uint8_t    dest[8];
   uint8_t    direction;
   uint8_t    numLinks;
   uint8_t    quality;
   int8_t     rssiSrcDest;
   int8_t     rssiDestSrc;
} dn_ipmg_getNextPathInfo_rpt;

typedef struct {
   uint8_t    RC;
   uint32_t   callbackId;
} dn_ipmg_setAdvertising_rpt;

typedef struct {
   uint8_t    RC;
   uint32_t   callbackId;
} dn_ipmg_setDownstreamFrameMode_rpt;

typedef struct {
   uint8_t    RC;
   uint16_t   serTxCnt;
   uint16_t   serRxCnt;
   uint16_t   serRxCRCErr;
   uint16_t   serRxOverruns;
   uint16_t   apiEstabConn;
   uint16_t   apiDroppedConn;
   uint16_t   apiTxOk;
   uint16_t   apiTxErr;
   uint16_t   apiTxFail;
   uint16_t   apiRxOk;
   uint16_t   apiRxProtErr;
} dn_ipmg_getManagerStatistics_rpt;

typedef struct {
   uint8_t    RC;
} dn_ipmg_setTime_rpt;

typedef struct {
   uint8_t    RC;
   uint8_t    license[13];
} dn_ipmg_getLicense_rpt;

typedef struct {
   uint8_t    RC;
} dn_ipmg_setLicense_rpt;

typedef struct {
   uint8_t    RC;
} dn_ipmg_setCLIUser_rpt;

typedef struct {
   uint8_t    RC;
   uint32_t   callbackId;
} dn_ipmg_sendIP_rpt;

typedef struct {
   uint8_t    RC;
} dn_ipmg_restoreFactoryDefaults_rpt;

typedef struct {
   uint8_t    RC;
   uint8_t    macAddress[8];
   uint8_t    state;
   uint8_t    numNbrs;
   uint8_t    numGoodNbrs;
   uint32_t   requestedBw;
   uint32_t   totalNeededBw;
   uint32_t   assignedBw;
   uint32_t   packetsReceived;
   uint32_t   packetsLost;
   uint32_t   avgLatency;
} dn_ipmg_getMoteInfo_rpt;

typedef struct {
   uint8_t    RC;
   uint16_t   networkId;
   int8_t     apTxPower;
   uint8_t    frameProfile;
   uint16_t   maxMotes;
   uint16_t   baseBandwidth;
   uint8_t    downFrameMultVal;
   uint8_t    numParents;
   uint8_t    ccaMode;
   uint16_t   channelList;
   bool       autoStartNetwork;
   uint8_t    locMode;
   uint8_t    bbMode;
   uint8_t    bbSize;
   uint8_t    isRadioTest;
   uint16_t   bwMult;
   uint8_t    oneChannel;
} dn_ipmg_getNetworkConfig_rpt;

typedef struct {
   uint8_t    RC;
   uint16_t   numMotes;
   uint16_t   asnSize;
   uint8_t    advertisementState;
   uint8_t    downFrameState;
   uint8_t    netReliability;
   uint8_t    netPathStability;
   uint32_t   netLatency;
   uint8_t    netState;
   uint8_t    ipv6Address[16];
} dn_ipmg_getNetworkInfo_rpt;

typedef struct {
   uint8_t    RC;
   uint8_t    macAddress[8];
   uint16_t   moteId;
   bool       isAP;
   uint8_t    state;
   uint8_t    reserved;
   bool       isRouting;
} dn_ipmg_getMoteConfigById_rpt;

typedef struct {
   uint8_t    RC;
} dn_ipmg_setCommonJoinKey_rpt;

typedef struct {
   uint8_t    RC;
   uint8_t    ipv6Address[16];
   uint8_t    mask[16];
} dn_ipmg_getIPConfig_rpt;

typedef struct {
   uint8_t    RC;
} dn_ipmg_setIPConfig_rpt;

typedef struct {
   uint8_t    RC;
} dn_ipmg_deleteMote_rpt;

typedef struct {
   uint8_t    RC;
   uint16_t   idx;
   uint8_t    utilization;
   uint8_t    numLinks;
   uint8_t    frameId_1;
   uint32_t   slot_1;
   uint8_t    channelOffset_1;
   uint16_t   moteId_1;
   uint8_t    flags_1;
   uint8_t    frameId_2;
   uint32_t   slot_2;
   uint8_t    channelOffset_2;
   uint16_t   moteId_2;
   uint8_t    flags_2;
   uint8_t    frameId_3;
   uint32_t   slot_3;
   uint8_t    channelOffset_3;
   uint16_t   moteId_3;
   uint8_t    flags_3;
   uint8_t    frameId_4;
   uint32_t   slot_4;
   uint8_t    channelOffset_4;
   uint16_t   moteId_4;
   uint8_t    flags_4;
   uint8_t    frameId_5;
   uint32_t   slot_5;
   uint8_t    channelOffset_5;
   uint16_t   moteId_5;
   uint8_t    flags_5;
   uint8_t    frameId_6;
   uint32_t   slot_6;
   uint8_t    channelOffset_6;
   uint16_t   moteId_6;
   uint8_t    flags_6;
   uint8_t    frameId_7;
   uint32_t   slot_7;
   uint8_t    channelOffset_7;
   uint16_t   moteId_7;
   uint8_t    flags_7;
   uint8_t    frameId_8;
   uint32_t   slot_8;
   uint8_t    channelOffset_8;
   uint16_t   moteId_8;
   uint8_t    flags_8;
   uint8_t    frameId_9;
   uint32_t   slot_9;
   uint8_t    channelOffset_9;
   uint16_t   moteId_9;
   uint8_t    flags_9;
   uint8_t    frameId_10;
   uint32_t   slot_10;
   uint8_t    channelOffset_10;
   uint16_t   moteId_10;
   uint8_t    flags_10;
} dn_ipmg_getMoteLinks_rpt;

//=== notification types

typedef struct {
   uint8_t    macAddress[8];
   uint8_t    logMsgLen;
   uint8_t    logMsg[MAX_FRAME_LENGTH];
} dn_ipmg_notifLog_nt;

typedef struct {
   uint8_t    utcSecs[8];
   uint32_t   utcUsecs;
   uint8_t    macAddress[8];
   uint16_t   srcPort;
   uint16_t   dstPort;
   uint8_t    dataLen;
   uint8_t    data[MAX_FRAME_LENGTH];
} dn_ipmg_notifData_nt;

typedef struct {
   uint8_t    utcSecs[8];
   uint32_t   utcUsecs;
   uint8_t    macAddress[8];
   uint8_t    dataLen;
   uint8_t    data[MAX_FRAME_LENGTH];
} dn_ipmg_notifIpData_nt;

typedef struct {
   uint8_t    macAddress[8];
   uint8_t    payloadLen;
   uint8_t    payload[MAX_FRAME_LENGTH];
} dn_ipmg_notifHealthReport_nt;

typedef struct {
   uint32_t   eventId;
   uint8_t    macAddress[8];
} dn_ipmg_eventMoteReset_nt;

typedef struct {
   uint32_t   eventId;
} dn_ipmg_eventNetworkReset_nt;

typedef struct {
   uint32_t   eventId;
   uint32_t   callbackId;
   uint8_t    rc;
} dn_ipmg_eventCommandFinished_nt;

typedef struct {
   uint32_t   eventId;
   uint8_t    macAddress[8];
} dn_ipmg_eventMoteJoin_nt;

typedef struct {
   uint32_t   eventId;
   uint8_t    macAddress[8];
} dn_ipmg_eventMoteOperational_nt;

typedef struct {
   uint32_t   eventId;
   uint8_t    macAddress[8];
} dn_ipmg_eventMoteLost_nt;

typedef struct {
   uint32_t   eventId;
   uint32_t   uptime;
   uint8_t    utcSecs[8];
   uint32_t   utcUsecs;
   uint8_t    asn[5];
   uint16_t   asnOffset;
} dn_ipmg_eventNetworkTime_nt;

typedef struct {
   uint32_t   eventId;
   uint32_t   callbackId;
   uint8_t    macAddress[8];
   uint32_t   delay;
   uint16_t   voltage;
   int8_t     temperature;
} dn_ipmg_eventPingResponse_nt;

typedef struct {
   uint32_t   eventId;
   uint8_t    source[8];
   uint8_t    dest[8];
   uint8_t    direction;
} dn_ipmg_eventPathCreate_nt;

typedef struct {
   uint32_t   eventId;
   uint8_t    source[8];
   uint8_t    dest[8];
   uint8_t    direction;
} dn_ipmg_eventPathDelete_nt;

typedef struct {
   uint32_t   eventId;
   uint32_t   callbackId;
   uint8_t    rc;
} dn_ipmg_eventPacketSent_nt;

typedef struct {
   uint32_t   eventId;
   uint8_t    macAddress[8];
   uint16_t   moteId;
} dn_ipmg_eventMoteCreate_nt;

typedef struct {
   uint32_t   eventId;
   uint8_t    macAddress[8];
   uint16_t   moteId;
} dn_ipmg_eventMoteDelete_nt;

//=== callback signature
typedef void (*dn_ipmg_notif_cbt)(uint8_t cmdId, uint8_t subCmdId);
typedef void (*dn_ipmg_reply_cbt)(uint8_t cmdId);
typedef void (*dn_ipmg_status_cbt)(uint8_t newStatus); // only used in SmartMesh IP manager

//=========================== variables =======================================

//=========================== prototypes ======================================

#ifdef __cplusplus
 extern "C" {
#endif

//==== admin
void     dn_ipmg_init(dn_ipmg_notif_cbt notifCb, uint8_t* notifBuf, uint8_t notifBufLen, dn_ipmg_reply_cbt replyCb, dn_ipmg_status_cbt statusCb);
void     dn_ipmg_cancelTx();
dn_err_t dn_ipmg_initiateConnect();

//==== API
dn_err_t dn_ipmg_reset(uint8_t type, uint8_t* macAddress, dn_ipmg_reset_rpt* reply);
dn_err_t dn_ipmg_subscribe(uint32_t filter, uint32_t unackFilter, dn_ipmg_subscribe_rpt* reply);
dn_err_t dn_ipmg_getTime(dn_ipmg_getTime_rpt* reply);
dn_err_t dn_ipmg_setNetworkConfig(uint16_t networkId, int8_t apTxPower, uint8_t frameProfile, uint16_t maxMotes, uint16_t baseBandwidth, uint8_t downFrameMultVal, uint8_t numParents, uint8_t ccaMode, uint16_t channelList, bool autoStartNetwork, uint8_t locMode, uint8_t bbMode, uint8_t bbSize, uint8_t isRadioTest, uint16_t bwMult, uint8_t oneChannel, dn_ipmg_setNetworkConfig_rpt* reply);
dn_err_t dn_ipmg_clearStatistics(dn_ipmg_clearStatistics_rpt* reply);
dn_err_t dn_ipmg_exchangeMoteJoinKey(uint8_t* macAddress, uint8_t* key, dn_ipmg_exchangeMoteJoinKey_rpt* reply);
dn_err_t dn_ipmg_exchangeNetworkId(uint16_t id, dn_ipmg_exchangeNetworkId_rpt* reply);
dn_err_t dn_ipmg_radiotestTx(uint8_t testType, uint16_t chanMask, uint16_t repeatCnt, int8_t txPower, uint8_t seqSize, uint8_t pkLen_1, uint16_t delay_1, uint8_t pkLen_2, uint16_t delay_2, uint8_t pkLen_3, uint16_t delay_3, uint8_t pkLen_4, uint16_t delay_4, uint8_t pkLen_5, uint16_t delay_5, uint8_t pkLen_6, uint16_t delay_6, uint8_t pkLen_7, uint16_t delay_7, uint8_t pkLen_8, uint16_t delay_8, uint8_t pkLen_9, uint16_t delay_9, uint8_t pkLen_10, uint16_t delay_10, uint8_t stationId, dn_ipmg_radiotestTx_rpt* reply);
dn_err_t dn_ipmg_radiotestRx(uint16_t mask, uint16_t duration, uint8_t stationId, dn_ipmg_radiotestRx_rpt* reply);
dn_err_t dn_ipmg_getRadiotestStatistics(dn_ipmg_getRadiotestStatistics_rpt* reply);
dn_err_t dn_ipmg_setACLEntry(uint8_t* macAddress, uint8_t* joinKey, dn_ipmg_setACLEntry_rpt* reply);
dn_err_t dn_ipmg_getNextACLEntry(uint8_t* macAddress, dn_ipmg_getNextACLEntry_rpt* reply);
dn_err_t dn_ipmg_deleteACLEntry(uint8_t* macAddress, dn_ipmg_deleteACLEntry_rpt* reply);
dn_err_t dn_ipmg_pingMote(uint8_t* macAddress, dn_ipmg_pingMote_rpt* reply);
dn_err_t dn_ipmg_getLog(uint8_t* macAddress, dn_ipmg_getLog_rpt* reply);
dn_err_t dn_ipmg_sendData(uint8_t* macAddress, uint8_t priority, uint16_t srcPort, uint16_t dstPort, uint8_t options, uint8_t* data, uint8_t dataLen, dn_ipmg_sendData_rpt* reply);
dn_err_t dn_ipmg_startNetwork(dn_ipmg_startNetwork_rpt* reply);
dn_err_t dn_ipmg_getSystemInfo(dn_ipmg_getSystemInfo_rpt* reply);
dn_err_t dn_ipmg_getMoteConfig(uint8_t* macAddress, bool next, dn_ipmg_getMoteConfig_rpt* reply);
dn_err_t dn_ipmg_getPathInfo(uint8_t* source, uint8_t* dest, dn_ipmg_getPathInfo_rpt* reply);
dn_err_t dn_ipmg_getNextPathInfo(uint8_t* macAddress, uint8_t filter, uint16_t pathId, dn_ipmg_getNextPathInfo_rpt* reply);
dn_err_t dn_ipmg_setAdvertising(uint8_t activate, dn_ipmg_setAdvertising_rpt* reply);
dn_err_t dn_ipmg_setDownstreamFrameMode(uint8_t frameMode, dn_ipmg_setDownstreamFrameMode_rpt* reply);
dn_err_t dn_ipmg_getManagerStatistics(dn_ipmg_getManagerStatistics_rpt* reply);
dn_err_t dn_ipmg_setTime(uint8_t trigger, uint8_t* utcSecs, uint32_t utcUsecs, dn_ipmg_setTime_rpt* reply);
dn_err_t dn_ipmg_getLicense(dn_ipmg_getLicense_rpt* reply);
dn_err_t dn_ipmg_setLicense(uint8_t* license, dn_ipmg_setLicense_rpt* reply);
dn_err_t dn_ipmg_setCLIUser(uint8_t role, uint8_t* password, dn_ipmg_setCLIUser_rpt* reply);
dn_err_t dn_ipmg_sendIP(uint8_t* macAddress, uint8_t priority, uint8_t options, uint8_t encryptedOffset, uint8_t* data, uint8_t dataLen, dn_ipmg_sendIP_rpt* reply);
dn_err_t dn_ipmg_restoreFactoryDefaults(dn_ipmg_restoreFactoryDefaults_rpt* reply);
dn_err_t dn_ipmg_getMoteInfo(uint8_t* macAddress, dn_ipmg_getMoteInfo_rpt* reply);
dn_err_t dn_ipmg_getNetworkConfig(dn_ipmg_getNetworkConfig_rpt* reply);
dn_err_t dn_ipmg_getNetworkInfo(dn_ipmg_getNetworkInfo_rpt* reply);
dn_err_t dn_ipmg_getMoteConfigById(uint16_t moteId, dn_ipmg_getMoteConfigById_rpt* reply);
dn_err_t dn_ipmg_setCommonJoinKey(uint8_t* key, dn_ipmg_setCommonJoinKey_rpt* reply);
dn_err_t dn_ipmg_getIPConfig(dn_ipmg_getIPConfig_rpt* reply);
dn_err_t dn_ipmg_setIPConfig(uint8_t* ipv6Address, uint8_t* mask, dn_ipmg_setIPConfig_rpt* reply);
dn_err_t dn_ipmg_deleteMote(uint8_t* macAddress, dn_ipmg_deleteMote_rpt* reply);
dn_err_t dn_ipmg_getMoteLinks(uint8_t* macAddress, uint16_t idx, dn_ipmg_getMoteLinks_rpt* reply);

#ifdef __cplusplus
}
#endif

#endif
