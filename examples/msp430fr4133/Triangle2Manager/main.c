#include <msp430fr4133.h>
#include <stdint.h>
#include "hal_LCD.h"
#include "dn_ipmt.h"

//=========================== defines =========================================

// mote state
#define MOTE_STATE_IDLE           0x01
#define MOTE_STATE_SEARCHING      0x02
#define MOTE_STATE_NEGOCIATING    0x03
#define MOTE_STATE_CONNECTED      0x04
#define MOTE_STATE_OPERATIONAL    0x05

// service types
#define SERVICE_TYPE_BW           0x00

// timings
#define SERIAL_RESPONSE_TIMEOUT   16384          // 16384@32kHz = 500ms
#define CMD_PERIOD                32768          // 32768@32kHz = 1s
#define ONE_SEC                   32768          // 32768@32kHz = 1s
#define DATA_PERIOD_S                10          // number of second between data packets

// app
#define NUM_STEPS                 8
#define DIRECTION_UP              1
#define DIRECTION_DOWN            0

// api
#define SRC_PORT                  0xf0b8
#define DST_PORT                  0xf0b8

//=========================== typedef =========================================

typedef void (*fsm_timer_callback)(void);
typedef void (*fsm_reply_callback)(void);

//=========================== variables =======================================

typedef struct {
   // fsm
   fsm_timer_callback   fsmCb;
   // reply
   fsm_reply_callback   replyCb;
   // app
   uint8_t              counter;
   uint8_t              secUntilTx;
   uint8_t              direction;
   // api
   uint8_t              socketId;                          // ID of the mote's UDP socket
   uint8_t              replyBuf[MAX_FRAME_LENGTH];        // holds notifications from ipmt
   uint8_t              notifBuf[MAX_FRAME_LENGTH];        // notifications buffer internal to ipmt
} app_vars_t;

app_vars_t app_vars;

//=========================== prototypes ======================================

// board
void init_board(void);
// fsm
void fsm_scheduleEvent(uint16_t delay, fsm_timer_callback cb);
void fsm_cancelEvent(void);
void fsm_setCallback(fsm_reply_callback cb);
// ipmt
void dn_ipmt_notif_cb(uint8_t cmdId, uint8_t subCmdId);
void dn_ipmt_reply_cb(uint8_t cmdId);
// app
uint16_t nextValue(void);
// api
void api_response_timeout(void);
void api_getMoteStatus(void);
void api_getMoteStatus_reply(void);
void api_openSocket(void);
void api_openSocket_reply(void);
void api_bindSocket(void);
void api_bindSocket_reply(void);
void api_join(void);
void api_join_reply(void);
void api_sendTo(void);
void api_sendTo_reply(void);

//=========================== main ============================================

int main(void) {
   // hold the watchdog timer
   WDTCTL     =  WDTPW + WDTHOLD;
   
   // board
   init_board();
   
   // reset local variables
   memset(&app_vars,    0, sizeof(app_vars));
   
   // initialize the ipmt module
   dn_ipmt_init(
      dn_ipmt_notif_cb,                // notifCb
      app_vars.notifBuf,               // notifBuf
      sizeof(app_vars.notifBuf),       // notifBufLen
      dn_ipmt_reply_cb                 // replyCb
   );
   
   // schedule the first event
   fsm_scheduleEvent(CMD_PERIOD, &api_getMoteStatus);
   
   __bis_SR_register(LPM0_bits | GIE);
}

//=========================== private =========================================

//===== board

void init_board(void) {
   
   PM5CTL0   &= ~LOCKLPM5;                       // Disable the GPIO power-on
                                                 // default high-impedance mode
                                                 // to activate previously
                                                 // configured port settings
   
   //=== LCD initialization
   
   Init_LCD();
   
   //=== CPU speed
   
   // P4.1 and P4.2 drive the external crystal
   P4SEL0    |=  0x06;
   
   // swicth on the crysyal oscillator
   do {
      CSCTL7 &= ~(XT1OFFG | DCOFFG);             // Clear XT1 and DCO fault flag
      SFRIFG1&= ~OFIFG;
   } while (SFRIFG1 & OFIFG);                    // Test oscillator fault flag
   
   // slave the DCO to the crystal
   __bis_SR_register(SCG0);                      // disable FLL
   CSCTL3    |=  SELREF__XT1CLK;                 // Set XT1CLK as FLL reference source
   CSCTL0     =  0;                              // clear DCO and MOD registers
   CSCTL1    &= ~(DCORSEL_7);                    // Clear DCO frequency select bits first
   CSCTL1    |=  DCORSEL_3;                      // Set DCO = 8MHz
   CSCTL2     =  FLLD_0 + 243;                   // DCODIV = 8MHz
   __delay_cycles(3);  
   __bic_SR_register(SCG0);                      // enable FLL
   while(CSCTL7 & (FLLUNLOCK0 | FLLUNLOCK1));    // Poll until FLL is locked
   
   // choose ACLK, MCLK and SMCLK speeds
   CSCTL4     = SELMS__DCOCLKDIV | SELA__XT1CLK; // Set ACLK = XT1CLK = 32768Hz
                                                 // DCOCLK = MCLK and SMCLK source
   CSCTL5    |= DIVM_0 | DIVS_1;                 // MCLK = DCOCLK = 8MHZ,
                                                 // SMCLK = MCLK/2 = 4MHz
}

//===== fsm

void fsm_scheduleEvent(uint16_t delay, fsm_timer_callback cb) {
   
   // remember what function to call
   app_vars.fsmCb       = cb;
   
   // configure/start the RTC
   RTCCTL               =  0;
   RTCMOD               =  delay;                // delay (in 32kHz clock cycles)
   RTCCTL              |=  RTCIE;                // enable interrupts
   RTCCTL              |=  RTCPS_0;              // divider /1
   RTCCTL              |=  RTCSS_2;              // clock source is XT1CLK
   RTCCTL              |=  RTCSR;                // reset the RTC
}

void fsm_cancelEvent(void) {
   // stop the RTC
   RTCCTL               =  0;
   
   // clear function to call
   app_vars.fsmCb       = NULL;
}

void fsm_setCallback(fsm_reply_callback cb) {
   app_vars.replyCb     = cb;
}

//===== ipmt

void dn_ipmt_notif_cb(uint8_t cmdId, uint8_t subCmdId) {
   dn_ipmt_events_nt* dn_ipmt_events_notif;
   
   switch (cmdId) {
      case CMDID_EVENTS:
         
         // parse notification
         dn_ipmt_events_notif = (dn_ipmt_events_nt*)app_vars.notifBuf;
         
         switch (dn_ipmt_events_notif->state) {
            case MOTE_STATE_IDLE:
               fsm_scheduleEvent(CMD_PERIOD,api_getMoteStatus);
               break;
            case MOTE_STATE_OPERATIONAL:
               fsm_scheduleEvent(CMD_PERIOD,api_sendTo);
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

void dn_ipmt_reply_cb(uint8_t cmdId) {
   app_vars.replyCb();
}

//===== app

uint16_t nextValue(void) {
   uint16_t newValue;
   
   // decide whether to go up or down
   if (app_vars.counter==0) {
      app_vars.direction = DIRECTION_UP;
   }
   if (app_vars.counter==NUM_STEPS-1) {
      app_vars.direction = DIRECTION_DOWN;
   }
   
   // calculate new value
   if (app_vars.direction==DIRECTION_UP) {
      app_vars.counter++;
   } else {
      app_vars.counter--;
   }
   
   newValue  = 0x10000/NUM_STEPS;
   newValue *= app_vars.counter;
   
   return newValue;
}

//===== api

void api_response_timeout(void) {
  // issue cancel command
   dn_ipmt_cancelTx();
   
   // schedule first event
   fsm_scheduleEvent(CMD_PERIOD, &api_getMoteStatus);
}

//=== getMoteStatus

void api_getMoteStatus(void) {
   
   // display status on LCD
   clearLCD();
   showChar('G',pos1);
  
   // arm callback
   fsm_setCallback(api_getMoteStatus_reply);
   
   // issue function
   dn_ipmt_getParameter_moteStatus(
      (dn_ipmt_getParameter_moteStatus_rpt*)(app_vars.replyBuf)
   );
   
   // schedule timeout event
   fsm_scheduleEvent(SERIAL_RESPONSE_TIMEOUT, api_response_timeout);
}

void api_getMoteStatus_reply(void) {
   dn_ipmt_getParameter_moteStatus_rpt* reply;
   
   // cancel timeout
   fsm_cancelEvent();
   
   // parse reply
   reply = (dn_ipmt_getParameter_moteStatus_rpt*)app_vars.replyBuf;
   
   // display on LCD
   showChar('0'+reply->state,pos2);
   
   // choose next step
   switch (reply->state) {
      case MOTE_STATE_IDLE:
         fsm_scheduleEvent(CMD_PERIOD, &api_openSocket);
         break;
      case MOTE_STATE_OPERATIONAL:
         fsm_scheduleEvent(CMD_PERIOD,api_sendTo);
         break;
      default:
         fsm_scheduleEvent(CMD_PERIOD, api_getMoteStatus);
         break;
   }
}

//=== openSocket

void api_openSocket(void) {
   
   // display status on LCD
   showChar('O',pos3);
  
   // arm callback
   fsm_setCallback(api_openSocket_reply);
   
   // issue function
   dn_ipmt_openSocket(
      0,                                              // protocol
      (dn_ipmt_openSocket_rpt*)(app_vars.replyBuf)    // reply
   );
   
   // schedule timeout event
   fsm_scheduleEvent(SERIAL_RESPONSE_TIMEOUT, api_response_timeout);
}

void api_openSocket_reply(void) {
   dn_ipmt_openSocket_rpt* reply;
   
   // cancel timeout
   fsm_cancelEvent();
   
   // parse reply
   reply = (dn_ipmt_openSocket_rpt*)app_vars.replyBuf;
   
   // store the socketID
   app_vars.socketId = reply->socketId;
   
   // choose next step
   fsm_scheduleEvent(CMD_PERIOD, api_bindSocket);
}

//===== bindSocket

void api_bindSocket(void) {
   
   // display status on LCD
   showChar('B',pos4);
   
   // arm callback
   fsm_setCallback(api_bindSocket_reply);
   
   // issue function
   dn_ipmt_bindSocket(
      app_vars.socketId,                              // socketId
      SRC_PORT,                                       // port
      (dn_ipmt_bindSocket_rpt*)(app_vars.replyBuf)    // reply
   );
   
   // schedule timeout event
   fsm_scheduleEvent(SERIAL_RESPONSE_TIMEOUT, api_response_timeout);
}

void api_bindSocket_reply(void) {
   // cancel timeout
   fsm_cancelEvent();
   
   // choose next step
   fsm_scheduleEvent(CMD_PERIOD, api_join);
}

//===== join

void api_join(void) {
   
   // display status on LCD
   showChar('J',pos5);
   
   // arm callback
   fsm_setCallback(api_join_reply);
   
   // issue function
   dn_ipmt_join(
      (dn_ipmt_join_rpt*)(app_vars.replyBuf)     // reply
   );
   
   // schedule timeout event
   fsm_scheduleEvent(SERIAL_RESPONSE_TIMEOUT, api_response_timeout);
}

void api_join_reply(void) {
   // cancel timeout
   fsm_cancelEvent();
   
   // choose next step
   // no next step at this point. FSM will advance when we received a "joined"
   // notification
}

//===== sendTo

void api_sendTo(void) {
   uint8_t  payload[2];
   uint8_t  dest_addr[16];
   
   // send only every DATA_PERIOD_S seconds
   if (app_vars.secUntilTx>0) {
      // decrement number of second to still wait
      app_vars.secUntilTx--;
      
      // cancel timeout
      fsm_cancelEvent();
      
      // choose next step
      fsm_scheduleEvent(ONE_SEC, api_sendTo);
      
      return;
   } else {
      app_vars.secUntilTx = DATA_PERIOD_S;
   }
   
   // arm callback
   fsm_setCallback(api_sendTo_reply);
   
   // create payload
   dn_write_uint16_t(payload, nextValue());
   memcpy(dest_addr,ipv6Addr_manager,16);
   
   // display status on LCD
   showChar('0'+app_vars.counter,pos6);
   
   // issue function
   dn_ipmt_sendTo(
      app_vars.socketId,                                   // socketId
      dest_addr,                                           // destIP
      DST_PORT,                                            // destPort
      SERVICE_TYPE_BW,                                     // serviceType
      0,                                                   // priority
      0xffff,                                              // packetId
      payload,                                             // payload
      sizeof(payload),                                     // payloadLen
      (dn_ipmt_sendTo_rpt*)(app_vars.replyBuf)             // reply
   );
   
   // schedule timeout event
   fsm_scheduleEvent(SERIAL_RESPONSE_TIMEOUT, api_response_timeout);
}

void api_sendTo_reply(void) {
   
   // cancel timeout
   fsm_cancelEvent();
   
   // choose next step
   fsm_scheduleEvent(ONE_SEC, api_sendTo);
}

//=========================== interrupts ======================================

#pragma vector = RTC_VECTOR
__interrupt void RTC_ISR(void) {
   volatile uint16_t iv;
   iv = RTCIV;
   
   app_vars.fsmCb();
}
