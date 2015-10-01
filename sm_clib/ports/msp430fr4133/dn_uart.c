/*
Copyright (c) 2015, Dust Networks. All rights reserved.

Port of the uart module to the MSP430FR4133.

On the MSP430FR4133 LaunchPad, we connect the SmartMesh IP device's serial
API to UCA0 using:
- P1.0 (UCA0TXD)
- P1.1 (UCA0RXD)

\license See attached DN_LICENSE.txt.
*/

#include <msp430fr4133.h>
#include "dn_uart.h"

//=========================== variables =======================================

typedef struct {
   dn_uart_rxByte_cbt   ipmt_uart_rxByte_cb;
} dn_uart_vars_t;

dn_uart_vars_t dn_uart_vars;

//=========================== prototypes ======================================

//=========================== public ==========================================

void dn_uart_init(dn_uart_rxByte_cbt rxByte_cb){
   
   // call back function
   dn_uart_vars.ipmt_uart_rxByte_cb = rxByte_cb;
   
   P1SEL0    |=  0x03;                      // set 2-UART pin as second function
   
   // configure UART
   UCA0CTLW0 |=  UCSWRST;                   // Put eUSCI in reset
   UCA0CTLW0 |=  UCSSEL__SMCLK;             // clock from SMCLK
   UCA0BR1    =  0;
   UCA0BR0    =  2;                         // 1000000/115200 = 8.68
   UCA0MCTLW  =  0xbb00 | UCBRF_2 | UCOS16; // 1000000/115200 - INT(1000000/115200)=0.68
                                            // UCBRSx value = 0xD6 (See UG)
   UCA0CTLW0 &= ~UCSWRST;                   // initialize eUSCI
   UCA0IE    |=  UCRXIE;                    // enable USCI_A0 RX interrupt
}

void dn_uart_txByte(uint8_t byte){
   while(!(UCA0IFG & UCTXIFG));             // wait until TX buffer ready
   UCA0TXBUF = byte;                        // send byte
}

void dn_uart_txFlush(){
   // nothing to do since MSP430 driver is byte-oriented
}

//=========================== private =========================================

//=========================== helpers =========================================

//=========================== interrupt handlers ==============================

#pragma vector=USCI_A0_VECTOR
__interrupt void usci_a0_ISR(void) {
   // hand over byte to ipmt module
   dn_uart_vars.ipmt_uart_rxByte_cb(UCA0RXBUF);
   P1OUT ^= 0x20;
}
