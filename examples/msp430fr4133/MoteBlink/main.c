#include <msp430fr4133.h>
#include <stdint.h>
#include "dn_ipmg.h"
#include "dn_serial_mg.h"

//=========================== defines =========================================

// timings
#define CMD_PERIOD                32768          // 32768@32kHz = 1s
#define BACKOFF_AFTER_TIMEOUT     32768          // 32768@32kHz = 1s
#define SERIAL_RESPONSE_TIMEOUT   16384          // 16384@32kHz = 500ms
#define OAP_RESPONSE_TIMEOUT      0xffff         // 0xffff@32kHz = ~2s
#define INTER_FRAME_PERIOD        32             // min number of 32kHz ticks between two TX frames over serial port

// subscription
#define SUBSC_FILTER_DATA         0x10

// mote state
#define MOTE_STATE_OPERATIONAL    0x04

#define DN_UDP_PORT_OAP           0xf0b9

//=========================== typedef =========================================

typedef void (*fsm_timer_callback)(void);
typedef void (*fsm_reply_callback)(void);

//=========================== variables =======================================

typedef struct {
   // fsm
   fsm_timer_callback   fsmCb;
   // reply
   fsm_reply_callback   replyCb;
   // api
   uint8_t              currentMac[8];
   uint8_t              replyBuf[MAX_FRAME_LENGTH];        // holds notifications from ipmg
   uint8_t              notifBuf[MAX_FRAME_LENGTH];        // notifications buffer internal to ipmg
   uint8_t              ledState;
} app_vars_t;

app_vars_t              app_vars;

//=========================== prototypes ======================================

void dn_ipmg_notif_cb(uint8_t cmdId, uint8_t subCmdId);
void dn_ipmg_reply_cb(uint8_t cmdId);
void dn_ipmg_status_cb(uint8_t newStatus);
// board
void init_board(void);
// fsm
void fsm_scheduleEvent(uint16_t delay, fsm_timer_callback cb);
void fsm_cancelEvent(void);
void fsm_setCallback(fsm_reply_callback cb);
// api
void api_response_timeout(void);
void oap_response_timeout(void);
void api_initiateConnect(void);
void api_subscribe(void);
void api_subscribe_reply(void);
void api_getNextMoteConfig(void);
void api_getNextMoteConfig_reply(void);
void api_toggleLed(void);
void api_toggleLed_reply(void);
// ipmg

//=========================== main ============================================

int main(void) {
   // hold the watchdog timer
   WDTCTL     =  WDTPW + WDTHOLD;
   
   // board
   init_board();
   
   // reset local variables
   memset(&app_vars,    0, sizeof(app_vars));
   
   // initialize the ipmt module
   dn_ipmg_init(
      dn_ipmg_notif_cb,                // notifCb
      app_vars.notifBuf,               // notifBuf
      sizeof(app_vars.notifBuf),       // notifBufLen
      dn_ipmg_reply_cb,                // replyCb
      dn_ipmg_status_cb                // statusCb
   );
   
   // schedule the first event
   fsm_scheduleEvent(CMD_PERIOD, &api_initiateConnect);
   
   __bis_SR_register(LPM0_bits | GIE);
}

//=========================== private =========================================

//===== board

void init_board(void) {
   
   PM5CTL0   &= ~LOCKLPM5;                       // Disable the GPIO power-on
                                                 // default high-impedance mode
                                                 // to activate previously
                                                 // configured port settings
   
   //=== CPU speed
   
   // P4.1 and P4.2 drive the external crystal
   P4SEL0    |=  0x06;
   
   // switch on the crysyal oscillator
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

//===== api

void api_response_timeout(void) {
  // issue cancel command
   dn_ipmg_cancelTx();
   
   // schedule first event
   fsm_scheduleEvent(BACKOFF_AFTER_TIMEOUT,api_initiateConnect);
}

void oap_response_timeout(void) {
      
   fsm_scheduleEvent(BACKOFF_AFTER_TIMEOUT,api_getNextMoteConfig);
}

//=== api_initiateConnect

void api_initiateConnect(void) {
   
   // issue command
   dn_ipmg_initiateConnect();
   
   // schedule timeout event
   fsm_scheduleEvent(
      SERIAL_RESPONSE_TIMEOUT,
      api_response_timeout
   );
}

//=== api_subscribe

void api_subscribe(void) {
   
   // arm callback
   fsm_setCallback(api_subscribe_reply);
   
   // issue command
   dn_ipmg_subscribe(
      SUBSC_FILTER_DATA,                              // filter
      0x00000000,                                     // unackFilter
      (dn_ipmg_subscribe_rpt*)(app_vars.replyBuf)     // reply
   );
   
   // schedule timeout event
   fsm_scheduleEvent(
      SERIAL_RESPONSE_TIMEOUT,
      api_response_timeout
   );
}

void api_subscribe_reply(void) {
   
   // schedule next event
   fsm_scheduleEvent(
      CMD_PERIOD,
      api_getNextMoteConfig
   );
}

//===== api_getNextMoteConfig

void api_getNextMoteConfig(void) {
   
   // arm callback
   fsm_setCallback(api_getNextMoteConfig_reply);
   
   // issue function
   dn_ipmg_getMoteConfig(
      app_vars.currentMac,                            // macAddress
      TRUE,                                           // next
      (dn_ipmg_getMoteConfig_rpt*)(app_vars.replyBuf) // reply
   );
   
   // schedule timeout event
   fsm_scheduleEvent(
      SERIAL_RESPONSE_TIMEOUT,
      api_response_timeout
   );
}

void api_getNextMoteConfig_reply(void) {
   dn_ipmg_getMoteConfig_rpt* reply;
   
   // parse reply
   reply = (dn_ipmg_getMoteConfig_rpt*)app_vars.replyBuf;
   
   if (reply->RC==0) {
      // remember current MAC
      memcpy(app_vars.currentMac,reply->macAddress,sizeof(reply->macAddress));
      
      if (reply->state==MOTE_STATE_OPERATIONAL && reply->isAP==FALSE) {
         
         // switch LED on
         app_vars.ledState=1;
         
         fsm_scheduleEvent(
            CMD_PERIOD,
            api_toggleLed
         );
      } else {
         // schedule next event
         fsm_scheduleEvent(
            CMD_PERIOD,
            api_getNextMoteConfig
         );
      }
   } else {
      // end of list or problem reading
      
      // reset next mote to ask
      memset(app_vars.currentMac,0,sizeof(app_vars.currentMac));
      
      // schedule next event
      fsm_scheduleEvent(
         CMD_PERIOD,
         api_getNextMoteConfig
      );
   }
}

//===== api_toggleLed

void api_toggleLed(void) {
   uint8_t    payload[10];
   
   // arm callback
   fsm_setCallback(api_toggleLed_reply);
   
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
   dn_ipmg_sendData(
      app_vars.currentMac,                       // macAddress
      0x00,                                      // priority
      DN_UDP_PORT_OAP,                           // srcPort
      DN_UDP_PORT_OAP,                           // dstPort
      0x00,                                      // options
      payload,                                   // data
      sizeof(payload),                           // dataLen
      (dn_ipmg_sendData_rpt*)(app_vars.replyBuf) // reply
   );
   
   // schedule timeout event
   fsm_scheduleEvent(
      SERIAL_RESPONSE_TIMEOUT,
      api_response_timeout
   );
}

void api_toggleLed_reply(void) {
   
   // schedule timeout event
   fsm_scheduleEvent(
      OAP_RESPONSE_TIMEOUT,
      oap_response_timeout
   );
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
            
            fsm_scheduleEvent(
               INTER_FRAME_PERIOD,
               api_toggleLed
            );
         } else {
            // move to next mote
            
            fsm_scheduleEvent(
               INTER_FRAME_PERIOD,
               api_getNextMoteConfig
            );
         }
      }
   }
}

void dn_ipmg_reply_cb(uint8_t cmdId) {
   app_vars.replyCb();
}

void dn_ipmg_status_cb(uint8_t newStatus) {
   switch (newStatus) {
      case DN_SERIAL_ST_CONNECTED:
         
         // schedule next event
         fsm_scheduleEvent(
            INTER_FRAME_PERIOD,
            api_subscribe
         );
         break;
      case DN_SERIAL_ST_DISCONNECTED:
         
         // schedule first event
         fsm_scheduleEvent(
            INTER_FRAME_PERIOD,
            api_initiateConnect
         );
         
         break;
      default:
         // nothing to do
         break;
   }
}

//=========================== interrupts ======================================

#pragma vector = RTC_VECTOR
__interrupt void RTC_ISR(void) {
   volatile uint16_t iv;
   iv = RTCIV;
   
   app_vars.fsmCb();
}
