/* --COPYRIGHT--,BSD
 * Copyright (c) 2014, Texas Instruments Incorporated
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * --/COPYRIGHT--*/
/********************************************************************
*
* Standard register and bit definitions for the Texas Instruments
* MSP430 microcontroller.
*
* This file supports assembler and C development for
* MSP430FR2XX_FR4XXGENERIC device.
*
* Texas Instruments, Version 1.0
*
* Rev. 1.0, Setup
*
*
********************************************************************/

#ifndef __msp430FR2XX_FR4XXGENERIC
#define __msp430FR2XX_FR4XXGENERIC

//#define __MSP430_HEADER_VERSION__ 1158      /* Beta-Build-Tag: #0008 */

#ifdef  __IAR_SYSTEMS_ICC__
#ifndef _SYSTEM_BUILD
#pragma system_include
#endif
#endif

#if (((__TID__ >> 8) & 0x7F) != 0x2b)     /* 0x2b = 43 dec */
#error msp430fr2xx_4xxgeneric.h file for use with ICC430/A430 only
#endif


#ifdef __IAR_SYSTEMS_ICC__
#include "in430.h"
#pragma language=save
#pragma language=extended

#define DEFC(name, address) __no_init volatile unsigned char name @ address;
#define DEFW(name, address) __no_init volatile unsigned short name @ address;

#define DEFCW(name, address) __no_init union \
{ \
  struct \
  { \
    volatile unsigned char  name##_L; \
    volatile unsigned char  name##_H; \
  }; \
  volatile unsigned short   name; \
} @ address;

#define READ_ONLY_DEFCW(name, address) __no_init union \
{ \
  struct \
  { \
    volatile READ_ONLY unsigned char  name##_L; \
    volatile READ_ONLY unsigned char  name##_H; \
  }; \
  volatile READ_ONLY unsigned short   name; \
} @ address;


#if __REGISTER_MODEL__ == __REGISTER_MODEL_REG20__
#define __ACCESS_20BIT_REG__  void __data20 * volatile
#else
#define __ACCESS_20BIT_REG__  volatile unsigned short  /* only short access from C is allowed in small memory model */
#endif

#define DEFA(name, address) __no_init union \
{ \
  struct \
  { \
    volatile unsigned char  name##_L; \
    volatile unsigned char  name##_H; \
  }; \
  struct \
  { \
    volatile unsigned short name##L; \
    volatile unsigned short name##H; \
  }; \
  __ACCESS_20BIT_REG__ name; \
} @ address;

#endif  /* __IAR_SYSTEMS_ICC__  */


#ifdef __IAR_SYSTEMS_ASM__
#define DEFC(name, address) sfrb name = address;
#define DEFW(name, address) sfrw name = address;

#define DEFCW(name, address) sfrbw name, name##_L, name##_H, address;
sfrbw macro name, name_L, name_H, address;
sfrb name_L = address;
sfrb name_H = address+1;
sfrw name   = address;
      endm

#define READ_ONLY_DEFCW(name, address) const_sfrbw name, name##_L, name##_H, address;
const_sfrbw macro name, name_L, name_H, address;
const sfrb name_L = address;
const sfrb name_H = address+1;
const sfrw name   = address;
      endm

#define DEFA(name, address) sfrba name, name##L, name##H, name##_L, name##_H, address;
sfrba macro name, nameL, nameH, name_L, name_H, address;
sfrb name_L = address;
sfrb name_H = address+1;
sfrw nameL  = address;
sfrw nameH  = address+2;
sfrl name   = address;
      endm

#endif /* __IAR_SYSTEMS_ASM__*/

#ifdef __cplusplus
#define READ_ONLY
#else
#define READ_ONLY const
#endif

/************************************************************
* STANDARD BITS
************************************************************/

#define BIT0                (0x0001u)
#define BIT1                (0x0002u)
#define BIT2                (0x0004u)
#define BIT3                (0x0008u)
#define BIT4                (0x0010u)
#define BIT5                (0x0020u)
#define BIT6                (0x0040u)
#define BIT7                (0x0080u)
#define BIT8                (0x0100u)
#define BIT9                (0x0200u)
#define BITA                (0x0400u)
#define BITB                (0x0800u)
#define BITC                (0x1000u)
#define BITD                (0x2000u)
#define BITE                (0x4000u)
#define BITF                (0x8000u)

/************************************************************
* STATUS REGISTER BITS
************************************************************/

#define C                   (0x0001u)
#define Z                   (0x0002u)
#define N                   (0x0004u)
#define V                   (0x0100u)
#define GIE                 (0x0008u)
#define CPUOFF              (0x0010u)
#define OSCOFF              (0x0020u)
#define SCG0                (0x0040u)
#define SCG1                (0x0080u)

/* Low Power Modes coded with Bits 4-7 in SR */

#ifndef __IAR_SYSTEMS_ICC__ /* Begin #defines for assembler */
#define LPM0                (CPUOFF)
#define LPM1                (SCG0+CPUOFF)
#define LPM2                (SCG1+CPUOFF)
#define LPM3                (SCG1+SCG0+CPUOFF)
#define LPM4                (SCG1+SCG0+OSCOFF+CPUOFF)
/* End #defines for assembler */

#else /* Begin #defines for C */
#define LPM0_bits           (CPUOFF)
#define LPM1_bits           (SCG0+CPUOFF)
#define LPM2_bits           (SCG1+CPUOFF)
#define LPM3_bits           (SCG1+SCG0+CPUOFF)
#define LPM4_bits           (SCG1+SCG0+OSCOFF+CPUOFF)

#include "in430.h"

#define LPM0      __bis_SR_register(LPM0_bits)         /* Enter Low Power Mode 0 */
#define LPM0_EXIT __bic_SR_register_on_exit(LPM0_bits) /* Exit Low Power Mode 0 */
#define LPM1      __bis_SR_register(LPM1_bits)         /* Enter Low Power Mode 1 */
#define LPM1_EXIT __bic_SR_register_on_exit(LPM1_bits) /* Exit Low Power Mode 1 */
#define LPM2      __bis_SR_register(LPM2_bits)         /* Enter Low Power Mode 2 */
#define LPM2_EXIT __bic_SR_register_on_exit(LPM2_bits) /* Exit Low Power Mode 2 */
#define LPM3      __bis_SR_register(LPM3_bits)         /* Enter Low Power Mode 3 */
#define LPM3_EXIT __bic_SR_register_on_exit(LPM3_bits) /* Exit Low Power Mode 3 */
#define LPM4      __bis_SR_register(LPM4_bits)         /* Enter Low Power Mode 4 */
#define LPM4_EXIT __bic_SR_register_on_exit(LPM4_bits) /* Exit Low Power Mode 4 */
#endif /* End #defines for C */

/************************************************************
* CPU
************************************************************/
#define __MSP430_HAS_MSP430XV2_CPU__  /* Definition to show that it has MSP430XV2 CPU */

#if defined(__MSP430_HAS_T0A2__) || defined(__MSP430_HAS_T1A2__) || defined(__MSP430_HAS_T2A2__) || defined(__MSP430_HAS_T3A2__) \
 || defined(__MSP430_HAS_T0A3__) || defined(__MSP430_HAS_T1A3__) || defined(__MSP430_HAS_T2A3__) || defined(__MSP430_HAS_T3A3__) \
 || defined(__MSP430_HAS_T0A5__) || defined(__MSP430_HAS_T1A5__) || defined(__MSP430_HAS_T2A5__) || defined(__MSP430_HAS_T3A5__) \
 || defined(__MSP430_HAS_T0A7__) || defined(__MSP430_HAS_T1A7__) || defined(__MSP430_HAS_T2A7__) || defined(__MSP430_HAS_T3A7__)
  #define __MSP430_HAS_TxA7__
#endif
#if defined(__MSP430_HAS_T0B3__) || defined(__MSP430_HAS_T0B5__) || defined(__MSP430_HAS_T0B7__) \
 || defined(__MSP430_HAS_T1B3__) || defined(__MSP430_HAS_T1B5__) || defined(__MSP430_HAS_T1B7__)
  #define __MSP430_HAS_TxB7__
#endif
#if defined(__MSP430_HAS_T0D3__) || defined(__MSP430_HAS_T0D5__) || defined(__MSP430_HAS_T0D7__) \
 || defined(__MSP430_HAS_T1D3__) || defined(__MSP430_HAS_T1D5__) || defined(__MSP430_HAS_T1D7__)
  #define __MSP430_HAS_TxD7__
#endif
#if defined(__MSP430_HAS_USCI_A0__) || defined(__MSP430_HAS_USCI_A1__) || defined(__MSP430_HAS_USCI_A2__) || defined(__MSP430_HAS_USCI_A3__)
  #define __MSP430_HAS_USCI_Ax__
#endif
#if defined(__MSP430_HAS_USCI_B0__) || defined(__MSP430_HAS_USCI_B1__) || defined(__MSP430_HAS_USCI_B2__) || defined(__MSP430_HAS_USCI_B3__)
  #define __MSP430_HAS_USCI_Bx__
#endif
#if defined(__MSP430_HAS_EUSCI_A0__) || defined(__MSP430_HAS_EUSCI_A1__) || defined(__MSP430_HAS_EUSCI_A2__) || defined(__MSP430_HAS_EUSCI_A3__)
  #define __MSP430_HAS_EUSCI_Ax__
#endif
#if defined(__MSP430_HAS_EUSCI_B0__) || defined(__MSP430_HAS_EUSCI_B1__) || defined(__MSP430_HAS_EUSCI_B2__) || defined(__MSP430_HAS_EUSCI_B3__)
  #define __MSP430_HAS_EUSCI_Bx__
#endif
#ifdef __MSP430_HAS_EUSCI_B0__
  #define __MSP430_HAS_EUSCI_Bx__
#endif

/************************************************************
* ADC
************************************************************/
#ifdef  __MSP430_HAS_ADC__             /* Definition to show that Module is available */

#define OFS_ADCCTL0           (0x0000u)   /* ADC Control 0 */
#define OFS_ADCCTL0_L          OFS_ADCCTL0
#define OFS_ADCCTL0_H          OFS_ADCCTL0+1
#define OFS_ADCCTL1           (0x0002u)   /* ADC Control 1 */
#define OFS_ADCCTL1_L          OFS_ADCCTL1
#define OFS_ADCCTL1_H          OFS_ADCCTL1+1
#define OFS_ADCCTL2           (0x0004u)   /* ADC Control 2 */
#define OFS_ADCCTL2_L          OFS_ADCCTL2
#define OFS_ADCCTL2_H          OFS_ADCCTL2+1
#define OFS_ADCLO             (0x0006u)   /* ADC Window Comparator High Threshold */
#define OFS_ADCLO_L            OFS_ADCLO
#define OFS_ADCLO_H            OFS_ADCLO+1
#define OFS_ADCHI             (0x0008u)   /* ADC Window Comparator High Threshold */
#define OFS_ADCHI_L            OFS_ADCHI
#define OFS_ADCHI_H            OFS_ADCHI+1
#define OFS_ADCMCTL0          (0x000Au)   /* ADC Memory Control 0 */
#define OFS_ADCMCTL0_L         OFS_ADCMCTL0
#define OFS_ADCMCTL0_H         OFS_ADCMCTL0+1
#define OFS_ADCMEM0           (0x0012u)   /* ADC Conversion Memory 0 */
#define OFS_ADCMEM0_L          OFS_ADCMEM0
#define OFS_ADCMEM0_H          OFS_ADCMEM0+1
#define OFS_ADCIE             (0x001Au)   /* ADC Interrupt Enable */
#define OFS_ADCIE_L            OFS_ADCIE
#define OFS_ADCIE_H            OFS_ADCIE+1
#define OFS_ADCIFG            (0x001Cu)   /* ADC Interrupt Flag */
#define OFS_ADCIFG_L           OFS_ADCIFG
#define OFS_ADCIFG_H           OFS_ADCIFG+1
#define OFS_ADCIV             (0x001Eu)   /* ADC Interrupt Vector Word */
#define OFS_ADCIV_L            OFS_ADCIV
#define OFS_ADCIV_H            OFS_ADCIV+1

/* ADCCTL0 Control Bits */
#define ADCSC               (0x0001u)   /* ADC Start Conversion */
#define ADCENC              (0x0002u)   /* ADC Enable Conversion */
#define ADCON               (0x0010u)   /* ADC On/enable */
#define ADCMSC              (0x0080u)   /* ADC Multiple SampleConversion */
#define ADCSHT0             (0x0100u)   /* ADC Sample Hold Select Bit: 0 */
#define ADCSHT1             (0x0200u)   /* ADC Sample Hold Select Bit: 1 */
#define ADCSHT2             (0x0400u)   /* ADC Sample Hold Select Bit: 2 */
#define ADCSHT3             (0x0800u)   /* ADC Sample Hold Select Bit: 3 */

/* ADCCTL0 Control Bits */
#define ADCSC_L             (0x0001u)   /* ADC Start Conversion */
#define ADCENC_L            (0x0002u)   /* ADC Enable Conversion */
#define ADCON_L             (0x0010u)   /* ADC On/enable */
#define ADCMSC_L            (0x0080u)   /* ADC Multiple SampleConversion */

/* ADCCTL0 Control Bits */
#define ADCSHT0_H           (0x0001u)   /* ADC Sample Hold Select Bit: 0 */
#define ADCSHT1_H           (0x0002u)   /* ADC Sample Hold Select Bit: 1 */
#define ADCSHT2_H           (0x0004u)   /* ADC Sample Hold Select Bit: 2 */
#define ADCSHT3_H           (0x0008u)   /* ADC Sample Hold Select Bit: 3 */

#define ADCSHT_0            (0*0x100u)  /* ADC Sample Hold Select 0 */
#define ADCSHT_1            (1*0x100u)  /* ADC Sample Hold Select 1 */
#define ADCSHT_2            (2*0x100u)  /* ADC Sample Hold Select 2 */
#define ADCSHT_3            (3*0x100u)  /* ADC Sample Hold Select 3 */
#define ADCSHT_4            (4*0x100u)  /* ADC Sample Hold Select 4 */
#define ADCSHT_5            (5*0x100u)  /* ADC Sample Hold Select 5 */
#define ADCSHT_6            (6*0x100u)  /* ADC Sample Hold Select 6 */
#define ADCSHT_7            (7*0x100u)  /* ADC Sample Hold Select 7 */
#define ADCSHT_8            (8*0x100u)  /* ADC Sample Hold Select 8 */
#define ADCSHT_9            (9*0x100u)  /* ADC Sample Hold Select 9 */
#define ADCSHT_10           (10*0x100u) /* ADC Sample Hold Select 10 */
#define ADCSHT_11           (11*0x100u) /* ADC Sample Hold Select 11 */
#define ADCSHT_12           (12*0x100u) /* ADC Sample Hold Select 12 */
#define ADCSHT_13           (13*0x100u) /* ADC Sample Hold Select 13 */
#define ADCSHT_14           (14*0x100u) /* ADC Sample Hold Select 14 */
#define ADCSHT_15           (15*0x100u) /* ADC Sample Hold Select 15 */

/* ADCCTL1 Control Bits */
#define ADCBUSY             (0x0001u)    /* ADC Busy */
#define ADCCONSEQ0          (0x0002u)    /* ADC Conversion Sequence Select 0 */
#define ADCCONSEQ1          (0x0004u)    /* ADC Conversion Sequence Select 1 */
#define ADCSSEL0            (0x0008u)    /* ADC Clock Source Select 0 */
#define ADCSSEL1            (0x0010u)    /* ADC Clock Source Select 1 */
#define ADCDIV0             (0x0020u)    /* ADC Clock Divider Select 0 */
#define ADCDIV1             (0x0040u)    /* ADC Clock Divider Select 1 */
#define ADCDIV2             (0x0080u)    /* ADC Clock Divider Select 2 */
#define ADCISSH             (0x0100u)    /* ADC Invert Sample Hold Signal */
#define ADCSHP              (0x0200u)    /* ADC Sample/Hold Pulse Mode */
#define ADCSHS0             (0x0400u)    /* ADC Sample/Hold Source 0 */
#define ADCSHS1             (0x0800u)    /* ADC Sample/Hold Source 1 */

/* ADCCTL1 Control Bits */
#define ADCBUSY_L           (0x0001u)    /* ADC Busy */
#define ADCCONSEQ0_L        (0x0002u)    /* ADC Conversion Sequence Select 0 */
#define ADCCONSEQ1_L        (0x0004u)    /* ADC Conversion Sequence Select 1 */
#define ADCSSEL0_L          (0x0008u)    /* ADC Clock Source Select 0 */
#define ADCSSEL1_L          (0x0010u)    /* ADC Clock Source Select 1 */
#define ADCDIV0_L           (0x0020u)    /* ADC Clock Divider Select 0 */
#define ADCDIV1_L           (0x0040u)    /* ADC Clock Divider Select 1 */
#define ADCDIV2_L           (0x0080u)    /* ADC Clock Divider Select 2 */

/* ADCCTL1 Control Bits */
#define ADCISSH_H           (0x0001u)    /* ADC Invert Sample Hold Signal */
#define ADCSHP_H            (0x0002u)    /* ADC Sample/Hold Pulse Mode */
#define ADCSHS0_H           (0x0004u)    /* ADC Sample/Hold Source 0 */
#define ADCSHS1_H           (0x0008u)    /* ADC Sample/Hold Source 1 */

#define ADCCONSEQ_0         (0*2u)       /* ADC Conversion Sequence Select: 0 */
#define ADCCONSEQ_1         (1*2u)       /* ADC Conversion Sequence Select: 1 */
#define ADCCONSEQ_2         (2*2u)       /* ADC Conversion Sequence Select: 2 */
#define ADCCONSEQ_3         (3*2u)       /* ADC Conversion Sequence Select: 3 */

#define ADCSSEL_0           (0*8u)       /* ADC Clock Source Select: 0 */
#define ADCSSEL_1           (1*8u)       /* ADC Clock Source Select: 1 */
#define ADCSSEL_2           (2*8u)       /* ADC Clock Source Select: 2 */
#define ADCSSEL_3           (3*8u)       /* ADC Clock Source Select: 3 */

#define ADCDIV_0            (0*0x20u)    /* ADC Clock Divider Select: 0 */
#define ADCDIV_1            (1*0x20u)    /* ADC Clock Divider Select: 1 */
#define ADCDIV_2            (2*0x20u)    /* ADC Clock Divider Select: 2 */
#define ADCDIV_3            (3*0x20u)    /* ADC Clock Divider Select: 3 */
#define ADCDIV_4            (4*0x20u)    /* ADC Clock Divider Select: 4 */
#define ADCDIV_5            (5*0x20u)    /* ADC Clock Divider Select: 5 */
#define ADCDIV_6            (6*0x20u)    /* ADC Clock Divider Select: 6 */
#define ADCDIV_7            (7*0x20u)    /* ADC Clock Divider Select: 7 */

#define ADCSHS_0            (0*0x400u)   /* ADC Sample/Hold Source: 0 */
#define ADCSHS_1            (1*0x400u)   /* ADC Sample/Hold Source: 1 */
#define ADCSHS_2            (2*0x400u)   /* ADC Sample/Hold Source: 2 */
#define ADCSHS_3            (3*0x400u)   /* ADC Sample/Hold Source: 3 */

/* ADCCTL2 Control Bits */
#define ADCSR               (0x0004u)    /* ADC Sampling Rate */
#define ADCDF               (0x0008u)    /* ADC Data Format */
#define ADCRES              (0x0010u)    /* ADC Resolution Bit */
#define ADCPDIV0            (0x0100u)    /* ADC predivider Bit: 0 */
#define ADCPDIV1            (0x0200u)    /* ADC predivider Bit: 1 */

/* ADCCTL2 Control Bits */
#define ADCSR_L             (0x0004u)    /* ADC Sampling Rate */
#define ADCDF_L             (0x0008u)    /* ADC Data Format */
#define ADCRES_L            (0x0010u)    /* ADC Resolution Bit */

/* ADCCTL2 Control Bits */
#define ADCPDIV0_H          (0x0001u)    /* ADC predivider Bit: 0 */
#define ADCPDIV1_H          (0x0002u)    /* ADC predivider Bit: 1 */

#define ADCPDIV_0           (0x0000u)    /* ADC predivider /1 */
#define ADCPDIV_1           (0x0100u)    /* ADC predivider /2 */
#define ADCPDIV_2           (0x0200u)    /* ADC predivider /64 */
#define ADCPDIV_3           (0x0300u)    /* ADC predivider reserved */

#define ADCPDIV__1          (0x0000u)    /* ADC predivider /1 */
#define ADCPDIV__4          (0x0100u)    /* ADC predivider /2 */
#define ADCPDIV__64         (0x0200u)    /* ADC predivider /64 */

/* ADCMCTL0 Control Bits */
#define ADCINCH0            (0x0001u)    /* ADC Input Channel Select Bit 0 */
#define ADCINCH1            (0x0002u)    /* ADC Input Channel Select Bit 1 */
#define ADCINCH2            (0x0004u)    /* ADC Input Channel Select Bit 2 */
#define ADCINCH3            (0x0008u)    /* ADC Input Channel Select Bit 3 */
#define ADCSREF0            (0x0010u)    /* ADC Select Reference Bit 0 */
#define ADCSREF1            (0x0020u)    /* ADC Select Reference Bit 1 */
#define ADCSREF2            (0x0040u)    /* ADC Select Reference Bit 2 */

/* ADCMCTL0 Control Bits */
#define ADCINCH0_L          (0x0001u)    /* ADC Input Channel Select Bit 0 */
#define ADCINCH1_L          (0x0002u)    /* ADC Input Channel Select Bit 1 */
#define ADCINCH2_L          (0x0004u)    /* ADC Input Channel Select Bit 2 */
#define ADCINCH3_L          (0x0008u)    /* ADC Input Channel Select Bit 3 */
#define ADCSREF0_L          (0x0010u)    /* ADC Select Reference Bit 0 */
#define ADCSREF1_L          (0x0020u)    /* ADC Select Reference Bit 1 */
#define ADCSREF2_L          (0x0040u)    /* ADC Select Reference Bit 2 */

#define ADCINCH_0           (0)         /* ADC Input Channel 0 */
#define ADCINCH_1           (1)         /* ADC Input Channel 1 */
#define ADCINCH_2           (2)         /* ADC Input Channel 2 */
#define ADCINCH_3           (3)         /* ADC Input Channel 3 */
#define ADCINCH_4           (4)         /* ADC Input Channel 4 */
#define ADCINCH_5           (5)         /* ADC Input Channel 5 */
#define ADCINCH_6           (6)         /* ADC Input Channel 6 */
#define ADCINCH_7           (7)         /* ADC Input Channel 7 */
#define ADCINCH_8           (8)         /* ADC Input Channel 8 */
#define ADCINCH_9           (9)         /* ADC Input Channel 9 */
#define ADCINCH_10          (10)        /* ADC Input Channel 10 */
#define ADCINCH_11          (11)        /* ADC Input Channel 11 */
#define ADCINCH_12          (12)        /* ADC Input Channel 12 */
#define ADCINCH_13          (13)        /* ADC Input Channel 13 */
#define ADCINCH_14          (14)        /* ADC Input Channel 14 */
#define ADCINCH_15          (15)        /* ADC Input Channel 15 */

#define ADCSREF_0           (0*0x10u)    /* ADC Select Reference 0 */
#define ADCSREF_1           (1*0x10u)    /* ADC Select Reference 1 */
#define ADCSREF_2           (2*0x10u)    /* ADC Select Reference 2 */
#define ADCSREF_3           (3*0x10u)    /* ADC Select Reference 3 */
#define ADCSREF_4           (4*0x10u)    /* ADC Select Reference 4 */
#define ADCSREF_5           (5*0x10u)    /* ADC Select Reference 5 */
#define ADCSREF_6           (6*0x10u)    /* ADC Select Reference 6 */
#define ADCSREF_7           (7*0x10u)    /* ADC Select Reference 7 */

/* ADCIE Interrupt Enable Bits */
#define ADCIE0              (0x0001u)    /* ADC Interrupt enable */
#define ADCINIE             (0x0002u)    /* ADC Interrupt enable for the inside of window of the Window comparator */
#define ADCLOIE             (0x0004u)    /* ADC Interrupt enable for lower threshold of the Window comparator */
#define ADCHIIE             (0x0008u)    /* ADC Interrupt enable for upper threshold of the Window comparator */
#define ADCOVIE             (0x0010u)    /* ADC ADCMEM overflow Interrupt enable */
#define ADCTOVIE            (0x0020u)    /* ADC conversion-time-overflow Interrupt enable */

/* ADCIE Interrupt Enable Bits */
#define ADCIE0_L            (0x0001u)    /* ADC Interrupt enable */
#define ADCINIE_L           (0x0002u)    /* ADC Interrupt enable for the inside of window of the Window comparator */
#define ADCLOIE_L           (0x0004u)    /* ADC Interrupt enable for lower threshold of the Window comparator */
#define ADCHIIE_L           (0x0008u)    /* ADC Interrupt enable for upper threshold of the Window comparator */
#define ADCOVIE_L           (0x0010u)    /* ADC ADCMEM overflow Interrupt enable */
#define ADCTOVIE_L          (0x0020u)    /* ADC conversion-time-overflow Interrupt enable */

/* ADCIFG Interrupt Flag Bits */
#define ADCIFG0             (0x0001u)    /* ADC Interrupt Flag */
#define ADCINIFG            (0x0002u)    /* ADC Interrupt Flag for the inside of window of the Window comparator */
#define ADCLOIFG            (0x0004u)    /* ADC Interrupt Flag for lower threshold of the Window comparator */
#define ADCHIIFG            (0x0008u)    /* ADC Interrupt Flag for upper threshold of the Window comparator */
#define ADCOVIFG            (0x0010u)    /* ADC ADCMEM overflow Interrupt Flag */
#define ADCTOVIFG           (0x0020u)    /* ADC conversion-time-overflow Interrupt Flag */

/* ADCIFG Interrupt Flag Bits */
#define ADCIFG0_L           (0x0001u)    /* ADC Interrupt Flag */
#define ADCINIFG_L          (0x0002u)    /* ADC Interrupt Flag for the inside of window of the Window comparator */
#define ADCLOIFG_L          (0x0004u)    /* ADC Interrupt Flag for lower threshold of the Window comparator */
#define ADCHIIFG_L          (0x0008u)    /* ADC Interrupt Flag for upper threshold of the Window comparator */
#define ADCOVIFG_L          (0x0010u)    /* ADC ADCMEM overflow Interrupt Flag */
#define ADCTOVIFG_L         (0x0020u)    /* ADC conversion-time-overflow Interrupt Flag */

/* ADCIV Definitions */
#define ADCIV_NONE          (0x0000u)    /* No Interrupt pending */
#define ADCIV_ADCOVIFG      (0x0002u)    /* ADCOVIFG */
#define ADCIV_ADCTOVIFG     (0x0004u)    /* ADCTOVIFG */
#define ADCIV_ADCHIIFG      (0x0006u)    /* ADCHIIFG */
#define ADCIV_ADCLOIFG      (0x0008u)    /* ADCLOIFG */
#define ADCIV_ADCINIFG      (0x000Au)    /* ADCINIFG */
#define ADCIV_ADCIFG        (0x000Cu)    /* ADCIFG */

#endif
/************************************************************
* Captivate
************************************************************/
#ifdef  __MSP430_HAS_CAPTIVATE__       /* Definition to show that Module is available */
#define CAPTIVATE_BASE __MSP430_HAS_CAPTIVATE__

#define OFS_CAPCTRL0          (0x0000u)   /* Captivate Control Register 0 */
#define OFS_CAPCTRL0_L         OFS_CAPCTRL0
#define OFS_CAPCTRL0_H         OFS_CAPCTRL0+1
#define OFS_CAPCTRL1          (0x0002u)   /* Captivate Control Register 1 */
#define OFS_CAPCTRL1_L         OFS_CAPCTRL1
#define OFS_CAPCTRL1_H         OFS_CAPCTRL1+1
#define OFS_CAPCFG0           (0x0004u)   /* Captivate Block Configuration Register 0 */
#define OFS_CAPCFG0_L          OFS_CAPCFG0
#define OFS_CAPCFG0_H          OFS_CAPCFG0+1
#define OFS_CAPCLK0           (0x0006u)   /* Captivate Clock Control Register 0 */
#define OFS_CAPCLK0_L          OFS_CAPCLK0
#define OFS_CAPCLK0_H          OFS_CAPCLK0+1
#define OFS_CAPSGN0           (0x0008u)   /* Captivate Charge and Transfer Control 0 */
#define OFS_CAPSGN0_L          OFS_CAPSGN0
#define OFS_CAPSGN0_H          OFS_CAPSGN0+1
#define OFS_CAPTCCTRL0        (0x000Au)   /* Captivate Timer/Counter Control Register 0 */
#define OFS_CAPTCCTRL0_L       OFS_CAPTCCTRL0
#define OFS_CAPTCCTRL0_H       OFS_CAPTCCTRL0+1
#define OFS_CAPTMR0           (0x000Cu)   /* Captivate Timer Register 0 */
#define OFS_CAPTMR0_L          OFS_CAPTMR0
#define OFS_CAPTMR0_H          OFS_CAPTMR0+1
#define OFS_CAPTCR0           (0x000Eu)   /* Captivate Timer Compare Register 0 */
#define OFS_CAPTCR0_L          OFS_CAPTCR0
#define OFS_CAPTCR0_H          OFS_CAPTCR0+1
#define OFS_CAPCNTMAX         (0x0010u)   /* Captivate Maximum Count Register */
#define OFS_CAPCNTMAX_L        OFS_CAPCNTMAX
#define OFS_CAPCNTMAX_H        OFS_CAPCNTMAX+1
#define OFS_CAPFSMCTRL0       (0x0012u)   /* Captivate State Machine Control 0 */
#define OFS_CAPFSMCTRL0_L      OFS_CAPFSMCTRL0
#define OFS_CAPFSMCTRL0_H      OFS_CAPFSMCTRL0+1
#define OFS_CAPTHRHI          (0x0014u)   /* Captivate Count Threshold High Value Register */
#define OFS_CAPTHRHI_L         OFS_CAPTHRHI
#define OFS_CAPTHRHI_H         OFS_CAPTHRHI+1
#define OFS_CAPTHRLO          (0x0016u)   /* Captivate Count Threshold Low Value Register */
#define OFS_CAPTHRLO_L         OFS_CAPTHRLO
#define OFS_CAPTHRLO_H         OFS_CAPTHRLO+1
#define OFS_CAPDETECT0        (0x0018u)   /* Captivate Detection Register 0 */
#define OFS_CAPDETECT0_L       OFS_CAPDETECT0
#define OFS_CAPDETECT0_H       OFS_CAPDETECT0+1
#define OFS_CAPREFC           (0x001Au)   /* Captivate Reference Capacitor Register */
#define OFS_CAPREFC_L          OFS_CAPREFC
#define OFS_CAPREFC_H          OFS_CAPREFC+1
#define OFS_CAPCHEN0          (0x0020u)   /* Captivate Channel Enable Register 0 */
#define OFS_CAPCHEN0_L         OFS_CAPCHEN0
#define OFS_CAPCHEN0_H         OFS_CAPCHEN0+1
#define OFS_CAPCHEN1          (0x0022u)   /* Captivate Channel Enable Register 1 */
#define OFS_CAPCHEN1_L         OFS_CAPCHEN1
#define OFS_CAPCHEN1_H         OFS_CAPCHEN1+1
//#define   CAPCHEN2_O            (0x0024u)   /* Captivate Channel Enable Register 2 */
//#define   CAPCHEN3_O            (0x0026u)   /* Captivate Channel Enable Register 3 */
//#define   CAPCHEN4_O            (0x0028u)   /* Captivate Channel Enable Register 4 */
//#define   CAPCHEN5_O            (0x002Au)   /* Captivate Channel Enable Register 5 */
#define OFS_CAPCHACT0         (0x002Cu)   /* Captivate Channel Active 0 */
#define OFS_CAPCHACT0_L        OFS_CAPCHACT0
#define OFS_CAPCHACT0_H        OFS_CAPCHACT0+1
#define OFS_CAPCHACT1         (0x002Eu)   /* Captivate Channel Active 1 */
#define OFS_CAPCHACT1_L        OFS_CAPCHACT1
#define OFS_CAPCHACT1_H        OFS_CAPCHACT1+1
//#define   CAPCHACT2_O           (0x0030u)   /* Captivate Channel Active 2 */
//#define   CAPCHACT3_O           (0x0032u)   /* Captivate Channel Active 3 */
//#define   CAPCHACT4_O           (0x0034u)   /* Captivate Channel Active 4 */
//#define   CAPCHACT5_O           (0x0036u)   /* Captivate Channel Active 5 */
#define OFS_CAPCHCTRL0        (0x0038u)   /* Captivate Channel Control 0 */
#define OFS_CAPCHCTRL0_L       OFS_CAPCHCTRL0
#define OFS_CAPCHCTRL0_H       OFS_CAPCHCTRL0+1
#define OFS_CAPCHCTRL1        (0x003Au)   /* Captivate Channel Control 1 */
#define OFS_CAPCHCTRL1_L       OFS_CAPCHCTRL1
#define OFS_CAPCHCTRL1_H       OFS_CAPCHCTRL1+1
//#define   CAPCHCTRL2_O          (0x003Cu)   /* Captivate Channel Control 2 */
//#define   CAPCHCTRL3_O          (0x003Eu)   /* Captivate Channel Control 3 */
//#define   CAPCHCTRL4_O          (0x0040u)   /* Captivate Channel Control 4 */
//#define   CAPCHCTRL5_O          (0x0042u)   /* Captivate Channel Control 5 */
#define OFS_CAPCHOFF0         (0x0044u)   /* Captivate Channel Off 0 */
#define OFS_CAPCHOFF0_L        OFS_CAPCHOFF0
#define OFS_CAPCHOFF0_H        OFS_CAPCHOFF0+1
#define OFS_CAPCHOFF1         (0x0046u)   /* Captivate Channel Off 1 */
#define OFS_CAPCHOFF1_L        OFS_CAPCHOFF1
#define OFS_CAPCHOFF1_H        OFS_CAPCHOFF1+1
//#define   CAPCHOFF2_O           (0x0048u)   /* Captivate Channel Off 2 */
//#define   CAPCHOFF3_O           (0x004Au)   /* Captivate Channel Off 3 */
//#define   CAPCHOFF4_O           (0x004Cu)   /* Captivate Channel Off 4 */
//#define   CAPCHOFF5_O           (0x004Eu)   /* Captivate Channel Off 5 */
#define OFS_CAPRATIO0         (0x0050u)   /* Captivate Ratio Block 0 */
#define OFS_CAPRATIO0_L        OFS_CAPRATIO0
#define OFS_CAPRATIO0_H        OFS_CAPRATIO0+1
#define OFS_CAPRATIO1         (0x0052u)   /* Captivate Ratio Block 1 */
#define OFS_CAPRATIO1_L        OFS_CAPRATIO1
#define OFS_CAPRATIO1_H        OFS_CAPRATIO1+1
#define OFS_CAPRATIO2         (0x0054u)   /* Captivate Ratio Block 2 */
#define OFS_CAPRATIO2_L        OFS_CAPRATIO2
#define OFS_CAPRATIO2_H        OFS_CAPRATIO2+1
#define OFS_CAPRATIO3         (0x0056u)   /* Captivate Ratio Block 3 */
#define OFS_CAPRATIO3_L        OFS_CAPRATIO3
#define OFS_CAPRATIO3_H        OFS_CAPRATIO3+1
#define OFS_CAPRATIO4         (0x0058u)   /* Captivate Ratio Block 4 */
#define OFS_CAPRATIO4_L        OFS_CAPRATIO4
#define OFS_CAPRATIO4_H        OFS_CAPRATIO4+1
#define OFS_CAPRATIO5         (0x005Au)   /* Captivate Ratio Block 5 */
#define OFS_CAPRATIO5_L        OFS_CAPRATIO5
#define OFS_CAPRATIO5_H        OFS_CAPRATIO5+1
#define OFS_CAPRATIO6         (0x005Cu)   /* Captivate Ratio Block 6 */
#define OFS_CAPRATIO6_L        OFS_CAPRATIO6
#define OFS_CAPRATIO6_H        OFS_CAPRATIO6+1
#define OFS_CAPRATIO7         (0x005Eu)   /* Captivate Ratio Block 7 */
#define OFS_CAPRATIO7_L        OFS_CAPRATIO7
#define OFS_CAPRATIO7_H        OFS_CAPRATIO7+1
#define OFS_CAPRATIO8         (0x0060u)   /* Captivate Ratio Block 8 */
#define OFS_CAPRATIO8_L        OFS_CAPRATIO8
#define OFS_CAPRATIO8_H        OFS_CAPRATIO8+1
#define OFS_CAPRATIO9         (0x0062u)   /* Captivate Ratio Block 9 */
#define OFS_CAPRATIO9_L        OFS_CAPRATIO9
#define OFS_CAPRATIO9_H        OFS_CAPRATIO9+1
#define OFS_CAPRATIO10        (0x0064u)   /* Captivate Ratio Block 10 */
#define OFS_CAPRATIO10_L       OFS_CAPRATIO10
#define OFS_CAPRATIO10_H       OFS_CAPRATIO10+1
#define OFS_CAPRATIO11        (0x0066u)   /* Captivate Ratio Block 11 */
#define OFS_CAPRATIO11_L       OFS_CAPRATIO11
#define OFS_CAPRATIO11_H       OFS_CAPRATIO11+1
#define OFS_CAPPCC0           (0x0070u)   /* Captivate PCC Register Block 0 */
#define OFS_CAPPCC0_L          OFS_CAPPCC0
#define OFS_CAPPCC0_H          OFS_CAPPCC0+1
#define OFS_CAPPCC1           (0x0072u)   /* Captivate PCC Register Block 1 */
#define OFS_CAPPCC1_L          OFS_CAPPCC1
#define OFS_CAPPCC1_H          OFS_CAPPCC1+1
#define OFS_CAPPCC2           (0x0074u)   /* Captivate PCC Register Block 2 */
#define OFS_CAPPCC2_L          OFS_CAPPCC2
#define OFS_CAPPCC2_H          OFS_CAPPCC2+1
#define OFS_CAPPCC3           (0x0076u)   /* Captivate PCC Register Block 3 */
#define OFS_CAPPCC3_L          OFS_CAPPCC3
#define OFS_CAPPCC3_H          OFS_CAPPCC3+1
#define OFS_CAPPCC4           (0x0078u)   /* Captivate PCC Register Block 4 */
#define OFS_CAPPCC4_L          OFS_CAPPCC4
#define OFS_CAPPCC4_H          OFS_CAPPCC4+1
#define OFS_CAPPCC5           (0x007Au)   /* Captivate PCC Register Block 5 */
#define OFS_CAPPCC5_L          OFS_CAPPCC5
#define OFS_CAPPCC5_H          OFS_CAPPCC5+1
#define OFS_CAPPCC6           (0x007Cu)   /* Captivate PCC Register Block 6 */
#define OFS_CAPPCC6_L          OFS_CAPPCC6
#define OFS_CAPPCC6_H          OFS_CAPPCC6+1
#define OFS_CAPPCC7           (0x007Eu)   /* Captivate PCC Register Block 7 */
#define OFS_CAPPCC7_L          OFS_CAPPCC7
#define OFS_CAPPCC7_H          OFS_CAPPCC7+1
#define OFS_CAPPCC8           (0x0080u)   /* Captivate PCC Register Block 8 */
#define OFS_CAPPCC8_L          OFS_CAPPCC8
#define OFS_CAPPCC8_H          OFS_CAPPCC8+1
#define OFS_CAPPCC9           (0x0082u)   /* Captivate PCC Register Block 9 */
#define OFS_CAPPCC9_L          OFS_CAPPCC9
#define OFS_CAPPCC9_H          OFS_CAPPCC9+1
#define OFS_CAPPCC10          (0x0084u)   /* Captivate PCC Register Block 10 */
#define OFS_CAPPCC10_L         OFS_CAPPCC10
#define OFS_CAPPCC10_H         OFS_CAPPCC10+1
#define OFS_CAPPCC11          (0x0086u)   /* Captivate PCC Register Block 11 */
#define OFS_CAPPCC11_L         OFS_CAPPCC11
#define OFS_CAPPCC11_H         OFS_CAPPCC11+1
#define OFS_CAPCVD0           (0x0090u)   /* Captivate Conversion Data Register 0 */
#define OFS_CAPCVD0_L          OFS_CAPCVD0
#define OFS_CAPCVD0_H          OFS_CAPCVD0+1
#define OFS_CAPCVD1           (0x0092u)   /* Captivate Conversion Data Register 1 */
#define OFS_CAPCVD1_L          OFS_CAPCVD1
#define OFS_CAPCVD1_H          OFS_CAPCVD1+1
#define OFS_CAPCVD2           (0x0094u)   /* Captivate Conversion Data Register 2 */
#define OFS_CAPCVD2_L          OFS_CAPCVD2
#define OFS_CAPCVD2_H          OFS_CAPCVD2+1
#define OFS_CAPCVD3           (0x0096u)   /* Captivate Conversion Data Register 3 */
#define OFS_CAPCVD3_L          OFS_CAPCVD3
#define OFS_CAPCVD3_H          OFS_CAPCVD3+1
#define OFS_CAPCVD4           (0x0098u)   /* Captivate Conversion Data Register 4 */
#define OFS_CAPCVD4_L          OFS_CAPCVD4
#define OFS_CAPCVD4_H          OFS_CAPCVD4+1
#define OFS_CAPCVD5           (0x009Au)   /* Captivate Conversion Data Register 5 */
#define OFS_CAPCVD5_L          OFS_CAPCVD5
#define OFS_CAPCVD5_H          OFS_CAPCVD5+1
#define OFS_CAPCVD6           (0x009Cu)   /* Captivate Conversion Data Register 6 */
#define OFS_CAPCVD6_L          OFS_CAPCVD6
#define OFS_CAPCVD6_H          OFS_CAPCVD6+1
#define OFS_CAPCVD7           (0x009Eu)   /* Captivate Conversion Data Register 7 */
#define OFS_CAPCVD7_L          OFS_CAPCVD7
#define OFS_CAPCVD7_H          OFS_CAPCVD7+1
#define OFS_CAPCVD8           (0x00A0u)   /* Captivate Conversion Data Register 8 */
#define OFS_CAPCVD8_L          OFS_CAPCVD8
#define OFS_CAPCVD8_H          OFS_CAPCVD8+1
#define OFS_CAPCVD9           (0x00A2u)   /* Captivate Conversion Data Register 9 */
#define OFS_CAPCVD9_L          OFS_CAPCVD9
#define OFS_CAPCVD9_H          OFS_CAPCVD9+1
#define OFS_CAPCVD10          (0x00A4u)   /* Captivate Conversion Data Register 10 */
#define OFS_CAPCVD10_L         OFS_CAPCVD10
#define OFS_CAPCVD10_H         OFS_CAPCVD10+1
#define OFS_CAPCVD11          (0x00A6u)   /* Captivate Conversion Data Register 11 */
#define OFS_CAPCVD11_L         OFS_CAPCVD11
#define OFS_CAPCVD11_H         OFS_CAPCVD11+1
#define OFS_CAPFCINT0         (0x00B0u)   /* Captivate Filtered Count Integer Value Register 0 */
#define OFS_CAPFCINT0_L        OFS_CAPFCINT0
#define OFS_CAPFCINT0_H        OFS_CAPFCINT0+1
#define OFS_CAPFCDEC0         (0x00B2u)   /* Captivate Filtered Count Decimal Value Register 0 */
#define OFS_CAPFCDEC0_L        OFS_CAPFCDEC0
#define OFS_CAPFCDEC0_H        OFS_CAPFCDEC0+1
#define OFS_CAPFCINT1         (0x00B4u)   /* Captivate Filtered Count Integer Value Register 1 */
#define OFS_CAPFCINT1_L        OFS_CAPFCINT1
#define OFS_CAPFCINT1_H        OFS_CAPFCINT1+1
#define OFS_CAPFCDEC1         (0x00B6u)   /* Captivate Filtered Count Decimal Value Register 1 */
#define OFS_CAPFCDEC1_L        OFS_CAPFCDEC1
#define OFS_CAPFCDEC1_H        OFS_CAPFCDEC1+1
#define OFS_CAPFCINT2         (0x00B8u)   /* Captivate Filtered Count Integer Value Register 2 */
#define OFS_CAPFCINT2_L        OFS_CAPFCINT2
#define OFS_CAPFCINT2_H        OFS_CAPFCINT2+1
#define OFS_CAPFCDEC2         (0x00BAu)   /* Captivate Filtered Count Decimal Value Register 2 */
#define OFS_CAPFCDEC2_L        OFS_CAPFCDEC2
#define OFS_CAPFCDEC2_H        OFS_CAPFCDEC2+1
#define OFS_CAPFCINT3         (0x00BCu)   /* Captivate Filtered Count Integer Value Register 3 */
#define OFS_CAPFCINT3_L        OFS_CAPFCINT3
#define OFS_CAPFCINT3_H        OFS_CAPFCINT3+1
#define OFS_CAPFCDEC3         (0x00BEu)   /* Captivate Filtered Count Decimal Value Register 3 */
#define OFS_CAPFCDEC3_L        OFS_CAPFCDEC3
#define OFS_CAPFCDEC3_H        OFS_CAPFCDEC3+1
#define OFS_CAPFCINT4         (0x00C0u)   /* Captivate Filtered Count Integer Value Register 4 */
#define OFS_CAPFCINT4_L        OFS_CAPFCINT4
#define OFS_CAPFCINT4_H        OFS_CAPFCINT4+1
#define OFS_CAPFCDEC4         (0x00C2u)   /* Captivate Filtered Count Decimal Value Register 4 */
#define OFS_CAPFCDEC4_L        OFS_CAPFCDEC4
#define OFS_CAPFCDEC4_H        OFS_CAPFCDEC4+1
#define OFS_CAPFCINT5         (0x00C4u)   /* Captivate Filtered Count Integer Value Register 5 */
#define OFS_CAPFCINT5_L        OFS_CAPFCINT5
#define OFS_CAPFCINT5_H        OFS_CAPFCINT5+1
#define OFS_CAPFCDEC5         (0x00C6u)   /* Captivate Filtered Count Decimal Value Register 5 */
#define OFS_CAPFCDEC5_L        OFS_CAPFCDEC5
#define OFS_CAPFCDEC5_H        OFS_CAPFCDEC5+1
#define OFS_CAPFCINT6         (0x00C8u)   /* Captivate Filtered Count Integer Value Register 6 */
#define OFS_CAPFCINT6_L        OFS_CAPFCINT6
#define OFS_CAPFCINT6_H        OFS_CAPFCINT6+1
#define OFS_CAPFCDEC6         (0x00CAu)   /* Captivate Filtered Count Decimal Value Register 6 */
#define OFS_CAPFCDEC6_L        OFS_CAPFCDEC6
#define OFS_CAPFCDEC6_H        OFS_CAPFCDEC6+1
#define OFS_CAPFCINT7         (0x00CCu)   /* Captivate Filtered Count Integer Value Register 7 */
#define OFS_CAPFCINT7_L        OFS_CAPFCINT7
#define OFS_CAPFCINT7_H        OFS_CAPFCINT7+1
#define OFS_CAPFCDEC7         (0x00CEu)   /* Captivate Filtered Count Decimal Value Register 7 */
#define OFS_CAPFCDEC7_L        OFS_CAPFCDEC7
#define OFS_CAPFCDEC7_H        OFS_CAPFCDEC7+1
#define OFS_CAPFCINT8         (0x00D0u)   /* Captivate Filtered Count Integer Value Register 8 */
#define OFS_CAPFCINT8_L        OFS_CAPFCINT8
#define OFS_CAPFCINT8_H        OFS_CAPFCINT8+1
#define OFS_CAPFCDEC8         (0x00D2u)   /* Captivate Filtered Count Decimal Value Register 8 */
#define OFS_CAPFCDEC8_L        OFS_CAPFCDEC8
#define OFS_CAPFCDEC8_H        OFS_CAPFCDEC8+1
#define OFS_CAPFCINT9         (0x00D4u)   /* Captivate Filtered Count Integer Value Register 9 */
#define OFS_CAPFCINT9_L        OFS_CAPFCINT9
#define OFS_CAPFCINT9_H        OFS_CAPFCINT9+1
#define OFS_CAPFCDEC9         (0x00D6u)   /* Captivate Filtered Count Decimal Value Register 9 */
#define OFS_CAPFCDEC9_L        OFS_CAPFCDEC9
#define OFS_CAPFCDEC9_H        OFS_CAPFCDEC9+1
#define OFS_CAPFCINT10        (0x00D8u)   /* Captivate Filtered Count Integer Value Register 10 */
#define OFS_CAPFCINT10_L       OFS_CAPFCINT10
#define OFS_CAPFCINT10_H       OFS_CAPFCINT10+1
#define OFS_CAPFCDEC10        (0x00DAu)   /* Captivate Filtered Count Decimal Value Register 10 */
#define OFS_CAPFCDEC10_L       OFS_CAPFCDEC10
#define OFS_CAPFCDEC10_H       OFS_CAPFCDEC10+1
#define OFS_CAPFCINT11        (0x00DCu)   /* Captivate Filtered Count Integer Value Register 11 */
#define OFS_CAPFCINT11_L       OFS_CAPFCINT11
#define OFS_CAPFCINT11_H       OFS_CAPFCINT11+1
#define OFS_CAPFCDEC11        (0x00DEu)   /* Captivate Filtered Count Decimal Value Register 11 */
#define OFS_CAPFCDEC11_L       OFS_CAPFCDEC11
#define OFS_CAPFCDEC11_H       OFS_CAPFCDEC11+1
#define OFS_CAPLTAINT0        (0x00F0u)   /* Captivate LTA Integer Value Register 0 */
#define OFS_CAPLTAINT0_L       OFS_CAPLTAINT0
#define OFS_CAPLTAINT0_H       OFS_CAPLTAINT0+1
#define OFS_CAPLTADEC0        (0x00F2u)   /* Captivate LTA Decimal Value Register 0 */
#define OFS_CAPLTADEC0_L       OFS_CAPLTADEC0
#define OFS_CAPLTADEC0_H       OFS_CAPLTADEC0+1
#define OFS_CAPLTAINT1        (0x00F4u)   /* Captivate LTA Integer Value Register 1 */
#define OFS_CAPLTAINT1_L       OFS_CAPLTAINT1
#define OFS_CAPLTAINT1_H       OFS_CAPLTAINT1+1
#define OFS_CAPLTADEC1        (0x00F6u)   /* Captivate LTA Decimal Value Register 1 */
#define OFS_CAPLTADEC1_L       OFS_CAPLTADEC1
#define OFS_CAPLTADEC1_H       OFS_CAPLTADEC1+1
#define OFS_CAPLTAINT2        (0x00F8u)   /* Captivate LTA Integer Value Register 2 */
#define OFS_CAPLTAINT2_L       OFS_CAPLTAINT2
#define OFS_CAPLTAINT2_H       OFS_CAPLTAINT2+1
#define OFS_CAPLTADEC2        (0x00FAu)   /* Captivate LTA Decimal Value Register 2 */
#define OFS_CAPLTADEC2_L       OFS_CAPLTADEC2
#define OFS_CAPLTADEC2_H       OFS_CAPLTADEC2+1
#define OFS_CAPLTAINT3        (0x00FCu)   /* Captivate LTA Integer Value Register 3 */
#define OFS_CAPLTAINT3_L       OFS_CAPLTAINT3
#define OFS_CAPLTAINT3_H       OFS_CAPLTAINT3+1
#define OFS_CAPLTADEC3        (0x00FEu)   /* Captivate LTA Decimal Value Register 3 */
#define OFS_CAPLTADEC3_L       OFS_CAPLTADEC3
#define OFS_CAPLTADEC3_H       OFS_CAPLTADEC3+1
#define OFS_CAPLTAINT4        (0x0100u)   /* Captivate LTA Integer Value Register 4 */
#define OFS_CAPLTAINT4_L       OFS_CAPLTAINT4
#define OFS_CAPLTAINT4_H       OFS_CAPLTAINT4+1
#define OFS_CAPLTADEC4        (0x0102u)   /* Captivate LTA Decimal Value Register 4 */
#define OFS_CAPLTADEC4_L       OFS_CAPLTADEC4
#define OFS_CAPLTADEC4_H       OFS_CAPLTADEC4+1
#define OFS_CAPLTAINT5        (0x0104u)   /* Captivate LTA Integer Value Register 5 */
#define OFS_CAPLTAINT5_L       OFS_CAPLTAINT5
#define OFS_CAPLTAINT5_H       OFS_CAPLTAINT5+1
#define OFS_CAPLTADEC5        (0x0106u)   /* Captivate LTA Decimal Value Register 5 */
#define OFS_CAPLTADEC5_L       OFS_CAPLTADEC5
#define OFS_CAPLTADEC5_H       OFS_CAPLTADEC5+1
#define OFS_CAPLTAINT6        (0x0108u)   /* Captivate LTA Integer Value Register 6 */
#define OFS_CAPLTAINT6_L       OFS_CAPLTAINT6
#define OFS_CAPLTAINT6_H       OFS_CAPLTAINT6+1
#define OFS_CAPLTADEC6        (0x010Au)   /* Captivate LTA Decimal Value Register 6 */
#define OFS_CAPLTADEC6_L       OFS_CAPLTADEC6
#define OFS_CAPLTADEC6_H       OFS_CAPLTADEC6+1
#define OFS_CAPLTAINT7        (0x010Cu)   /* Captivate LTA Integer Value Register 7 */
#define OFS_CAPLTAINT7_L       OFS_CAPLTAINT7
#define OFS_CAPLTAINT7_H       OFS_CAPLTAINT7+1
#define OFS_CAPLTADEC7        (0x010Eu)   /* Captivate LTA Decimal Value Register 7 */
#define OFS_CAPLTADEC7_L       OFS_CAPLTADEC7
#define OFS_CAPLTADEC7_H       OFS_CAPLTADEC7+1
#define OFS_CAPLTAINT8        (0x0110u)   /* Captivate LTA Integer Value Register 8 */
#define OFS_CAPLTAINT8_L       OFS_CAPLTAINT8
#define OFS_CAPLTAINT8_H       OFS_CAPLTAINT8+1
#define OFS_CAPLTADEC8        (0x0112u)   /* Captivate LTA Decimal Value Register 8 */
#define OFS_CAPLTADEC8_L       OFS_CAPLTADEC8
#define OFS_CAPLTADEC8_H       OFS_CAPLTADEC8+1
#define OFS_CAPLTAINT9        (0x0114u)   /* Captivate LTA Integer Value Register 9 */
#define OFS_CAPLTAINT9_L       OFS_CAPLTAINT9
#define OFS_CAPLTAINT9_H       OFS_CAPLTAINT9+1
#define OFS_CAPLTADEC9        (0x0116u)   /* Captivate LTA Decimal Value Register 9 */
#define OFS_CAPLTADEC9_L       OFS_CAPLTADEC9
#define OFS_CAPLTADEC9_H       OFS_CAPLTADEC9+1
#define OFS_CAPLTAINT10       (0x0118u)   /* Captivate LTA Integer Value Register 10 */
#define OFS_CAPLTAINT10_L      OFS_CAPLTAINT10
#define OFS_CAPLTAINT10_H      OFS_CAPLTAINT10+1
#define OFS_CAPLTADEC10       (0x011Au)   /* Captivate LTA Decimal Value Register 10 */
#define OFS_CAPLTADEC10_L      OFS_CAPLTADEC10
#define OFS_CAPLTADEC10_H      OFS_CAPLTADEC10+1
#define OFS_CAPLTAINT11       (0x011Cu)   /* Captivate LTA Integer Value Register 11 */
#define OFS_CAPLTAINT11_L      OFS_CAPLTAINT11
#define OFS_CAPLTAINT11_H      OFS_CAPLTAINT11+1
#define OFS_CAPLTADEC11       (0x011Eu)   /* Captivate LTA Decimal Value Register 11 */
#define OFS_CAPLTADEC11_L      OFS_CAPLTADEC11
#define OFS_CAPLTADEC11_H      OFS_CAPLTADEC11+1
#define OFS_CAPIE             (0x0120u)   /* Captivate Interrupt Enable Register */
#define OFS_CAPIE_L            OFS_CAPIE
#define OFS_CAPIE_H            OFS_CAPIE+1
#define OFS_CAPIFG            (0x0122u)   /* Captivate Interrupt Flag Register */
#define OFS_CAPIFG_L           OFS_CAPIFG
#define OFS_CAPIFG_H           OFS_CAPIFG+1
#define OFS_CAPIV             (0x0124u)   /* Captivate Interrupt Vector Register */
#define OFS_CAPIV_L            OFS_CAPIV
#define OFS_CAPIV_H            OFS_CAPIV+1

/* CAPCTRL0 */
#define CAPPWR              (0x0001u)  /* Controls Captivate LDO */
#define CAPSTART            (0x0002u)  /* Start conversion */
#define CAPRESET            (0x0020u)  /* Reset bit */
#define FWSTBL              (0x0040u)  /* Controls the stabilization by firmware */
#define CAPSTATUS           (0x0100u)  /* Indicates status of the Captivate module */
#define CIPF                (0x0200u)  /* Conversion in progress flag */

#define CAPPWR_L            (0x0001u)  /* Controls Captivate LDO */
#define CAPSTART_L          (0x0002u)  /* Start conversion */
#define CAPRESET_L          (0x0020u)  /* Reset bit */
#define FWSTBL_L            (0x0040u)  /* Controls the stabilization by firmware */

#define CAPSTATUS_H         (0x0001u)  /* Indicates status of the Captivate module */
#define CIPF_H              (0x0002u)  /* Conversion in progress flag */

/* CAPCTRL1 */
#define SYNCEN              (0x0001u)  /* Enable synchronization feature */
#define SYNCEDGE            (0x0002u)  /* SYNC edge selection */
#define SYNCPF              (0x0004u)  /* Synchronization (SYNC) pending flag */
#define SYNCOVRF            (0x0008u)  /* Synchronization (SYNC) overflow flag */
#define COUPLING            (0x0080u)  /* Reduces coupling between Cs and Cx receiver lines */
#define CAPLPMCFG           (0x0400u)  /* Low-power mode */
#define CSDISCHRG           (0x0800u)  /* Configures to what level the Pool Capacitor Cs is discharged */
#define OPAMPBIAS0          (0x1000u)  /* Sample and Hold strength selection Bit 0 */
#define OPAMPBIAS1          (0x2000u)  /* Sample and Hold strength selection Bit 1 */
#define STBL                (0x4000u)  /* Selection for stabilization time after Captivate power-on */

#define SYNCEN_L            (0x0001u)  /* Enable synchronization feature */
#define SYNCEDGE_L          (0x0002u)  /* SYNC edge selection */
#define SYNCPF_L            (0x0004u)  /* Synchronization (SYNC) pending flag */
#define SYNCOVRF_L          (0x0008u)  /* Synchronization (SYNC) overflow flag */
#define COUPLING_L          (0x0080u)  /* Reduces coupling between Cs and Cx receiver lines */

#define CAPLPMCFG_H         (0x0004u)  /* Low-power mode */
#define CSDISCHRG_H         (0x0008u)  /* Configures to what level the Pool Capacitor Cs is discharged */
#define OPAMPBIAS0_H        (0x0010u)  /* Sample and Hold strength selection Bit 0 */
#define OPAMPBIAS1_H        (0x0020u)  /* Sample and Hold strength selection Bit 1 */
#define STBL_H              (0x0040u)  /* Selection for stabilization time after Captivate power-on */

/* CAPCFG0 */
#define CFGBLCK0           (0x0001u)  /* Configure Captivate block 0 */
#define CFGBLCK1           (0x0002u)  /* Configure Captivate block 1 */
#define CFGBLCK2           (0x0004u)  /* Configure Captivate block 2 */
#define CFGBLCK3           (0x0008u)  /* Configure Captivate block 3 */
#define CFGBLCK4           (0x0010u)  /* Configure Captivate block 4 */
#define CFGBLCK5           (0x0020u)  /* Configure Captivate block 5 */
#define CFGBLCK6           (0x0040u)  /* Configure Captivate block 6 */
#define CFGBLCK7           (0x0080u)  /* Configure Captivate block 7 */
#define CFGBLCK8           (0x0100u)  /* Configure Captivate block 8 */
#define CFGBLCK9           (0x0200u)  /* Configure Captivate block 9 */
#define CFGBLCK10           (0x0400u)  /* Configure Captivate block 10 */
#define CFGBLCK11           (0x0800u)  /* Configure Captivate block 11 */

#define CFGBLCK0_L          (0x0001u)  /* Configure Captivate block 0 */
#define CFGBLCK1_L          (0x0002u)  /* Configure Captivate block 1 */
#define CFGBLCK2_L          (0x0004u)  /* Configure Captivate block 2 */
#define CFGBLCK3_L          (0x0008u)  /* Configure Captivate block 3 */
#define CFGBLCK4_L          (0x0010u)  /* Configure Captivate block 4 */
#define CFGBLCK5_L          (0x0020u)  /* Configure Captivate block 5 */
#define CFGBLCK6_L          (0x0040u)  /* Configure Captivate block 6 */
#define CFGBLCK7_L          (0x0080u)  /* Configure Captivate block 7 */

#define CFGBLCK8_H          (0x0001u)  /* Configure Captivate block 8 */
#define CFGBLCK9_H          (0x0002u)  /* Configure Captivate block 9 */
#define CFGBLCK10_H         (0x0004u)  /* Configure Captivate block 10 */
#define CFGBLCK11_H         (0x0008u)  /* Configure Captivate block 11 */

/* CAPCLK0 */
#define MODEN               (0x0001u)  /* Enable frequency modulation */
#define MODINTVL0           (0x0002u)  /* conversion frequency sweeps in the range Bit 0 */
#define MODINTVL1           (0x0004u)  /* conversion frequency sweeps in the range Bit 1 */
#define MODINTVL2           (0x0008u)  /* conversion frequency sweeps in the range Bit 2 */
#define FREQSHFT0           (0x0010u)  /* Selects the oscillator frequency Bit 0 */
#define FREQSHFT1           (0x0020u)  /* Selects the oscillator frequency Bit 1 */
#define MODRNG0             (0x0040u)  /* Selects the size of each modulation steps Bit 0 */
#define MODRNG1             (0x0080u)  /* Selects the size of each modulation steps Bit 1 */
#define ANADEAD             (0x0200u)  /* Ensure dead time (non-overlap interval) between the Cph and Tph */
#define DLYSEL0             (0x0400u)  /* Analog dead time delay selection Bit 0 */
#define DLYSEL1             (0x0800u)  /* Analog dead time delay selection Bit 1 */
#define FREQDIV0            (0x1000u)  /* Frequency divider for conversion clock Bit 0 */
#define FREQDIV1            (0x2000u)  /* Frequency divider for conversion clock Bit 1 */
#define FREQDIV2            (0x4000u)  /* Frequency divider for conversion clock Bit 2 */
#define CAPCLKSEL           (0x8000u)  /* Captivate Clock select */

#define MODEN_L             (0x0001u)  /* Enable frequency modulation */
#define MODINTVL0_L         (0x0002u)  /* conversion frequency sweeps in the range Bit 0 */
#define MODINTVL1_L         (0x0004u)  /* conversion frequency sweeps in the range Bit 1 */
#define MODINTVL2_L         (0x0008u)  /* conversion frequency sweeps in the range Bit 2 */
#define FREQSHFT0_L         (0x0010u)  /* Selects the oscillator frequency Bit 0 */
#define FREQSHFT1_L         (0x0020u)  /* Selects the oscillator frequency Bit 1 */
#define MODRNG0_L           (0x0040u)  /* Selects the size of each modulation steps Bit 0 */
#define MODRNG1_L           (0x0080u)  /* Selects the size of each modulation steps Bit 1 */

#define ANADEAD_H           (0x0002u)  /* Ensure dead time (non-overlap interval) between the Cph and Tph */
#define DLYSEL0_H           (0x0004u)  /* Analog dead time delay selection Bit 0 */
#define DLYSEL1_H           (0x0008u)  /* Analog dead time delay selection Bit 1 */
#define FREQDIV0_H          (0x0010u)  /* Frequency divider for conversion clock Bit 0 */
#define FREQDIV1_H          (0x0020u)  /* Frequency divider for conversion clock Bit 1 */
#define FREQDIV2_H          (0x0040u)  /* Frequency divider for conversion clock Bit 2 */
#define CAPCLKSEL_H         (0x0080u)  /* Captivate Clock select */

#define MODINTVL_0          (0x0000u)  /* conversion frequency sweeps in the range: 1 */
#define MODINTVL_1          (0x0002u)  /* conversion frequency sweeps in the range: 2 */
#define MODINTVL_2          (0x0004u)  /* conversion frequency sweeps in the range: 4 */
#define MODINTVL_3          (0x0006u)  /* conversion frequency sweeps in the range: 8 */
#define MODINTVL_4          (0x0008u)  /* conversion frequency sweeps in the range: 16 */
#define MODINTVL_5          (0x000Au)  /* conversion frequency sweeps in the range: 32 */
#define MODINTVL_6          (0x000Cu)  /* conversion frequency sweeps in the range: 64 */
#define MODINTVL_7          (0x000Eu)  /* conversion frequency sweeps in the range: 128 */

#define MODINTVL__1         (0x0000u)  /* conversion frequency sweeps in the range: 1 */
#define MODINTVL__2         (0x0002u)  /* conversion frequency sweeps in the range: 2 */
#define MODINTVL__4         (0x0004u)  /* conversion frequency sweeps in the range: 4 */
#define MODINTVL__8         (0x0006u)  /* conversion frequency sweeps in the range: 8 */
#define MODINTVL__16        (0x0008u)  /* conversion frequency sweeps in the range: 16 */
#define MODINTVL__32        (0x000Au)  /* conversion frequency sweeps in the range: 32 */
#define MODINTVL__64        (0x000Cu)  /* conversion frequency sweeps in the range: 64 */
#define MODINTVL__128       (0x000Eu)  /* conversion frequency sweeps in the range: 128 */

#define FREQSHFT_0          (0x0000u)  /* Selects the oscillator frequency: 16.0MHz */
#define FREQSHFT_1          (0x0010u)  /* Selects the oscillator frequency: 14.7MHz */
#define FREQSHFT_2          (0x0020u)  /* Selects the oscillator frequency: 13.1MHz */
#define FREQSHFT_3          (0x0030u)  /* Selects the oscillator frequency: 11.2MHz */

#define MODRNG_0            (0x0000u)  /* Selects the size of each modulation steps: 5% */
#define MODRNG_1            (0x0040u)  /* Selects the size of each modulation steps: 8% */
#define MODRNG_2            (0x0080u)  /* Selects the size of each modulation steps: 10% */
#define MODRNG_3            (0x00C0u)  /* Selects the size of each modulation steps: 16% */

#define DLYSEL_0            (0x0000u)  /* Analog dead time delay selection: 5ns */
#define DLYSEL_1            (0x0400u)  /* Analog dead time delay selection: 10ns */
#define DLYSEL_2            (0x0800u)  /* Analog dead time delay selection: 2.5ns */
#define DLYSEL_3            (0x0C00u)  /* Analog dead time delay selection: 3.3ns */

#define FREQDIV_0           (0x0000u)  /* Frequency divider for conversion clock: 1 */
#define FREQDIV_1           (0x1000u)  /* Frequency divider for conversion clock: 2 */
#define FREQDIV_2           (0x2000u)  /* Frequency divider for conversion clock: 4 */
#define FREQDIV_3           (0x3000u)  /* Frequency divider for conversion clock: 8 */
#define FREQDIV_4           (0x4000u)  /* Frequency divider for conversion clock: 16 */
#define FREQDIV_5           (0x5000u)  /* Frequency divider for conversion clock: 32 */
#define FREQDIV_6           (0x6000u)  /* Frequency divider for conversion clock: 64 */
#define FREQDIV_7           (0x7000u)  /* Frequency divider for conversion clock: 128 */

#define FREQDIV__1          (0x0000u)  /* Frequency divider for conversion clock: 1 */
#define FREQDIV__2          (0x1000u)  /* Frequency divider for conversion clock: 2 */
#define FREQDIV__4          (0x2000u)  /* Frequency divider for conversion clock: 4 */
#define FREQDIV__8          (0x3000u)  /* Frequency divider for conversion clock: 8 */
#define FREQDIV__16         (0x4000u)  /* Frequency divider for conversion clock: 16 */
#define FREQDIV__32         (0x5000u)  /* Frequency divider for conversion clock: 32 */
#define FREQDIV__64         (0x6000u)  /* Frequency divider for conversion clock: 64 */
#define FREQDIV__128        (0x7000u)  /* Frequency divider for conversion clock: 128 */

/* CAPSGN0 */
#define TPHLEN0             (0x0001u)  /* Configure Captivate block Bit 0 */
#define TPHLEN1             (0x0002u)  /* Configure Captivate block Bit 1 */
#define TPHLEN2             (0x0004u)  /* Configure Captivate block Bit 2 */
#define TPHLEN3             (0x0008u)  /* Configure Captivate block Bit 3 */
#define CPHLEN0             (0x0100u)  /* Configure Captivate block Bit 0 */
#define CPHLEN1             (0x0200u)  /* Configure Captivate block Bit 1 */
#define CPHLEN2             (0x0400u)  /* Configure Captivate block Bit 2 */
#define CPHLEN3             (0x0800u)  /* Configure Captivate block Bit 3 */

#define TPHLEN0_L           (0x0001u)  /* Configure Captivate block Bit 0 */
#define TPHLEN1_L           (0x0002u)  /* Configure Captivate block Bit 1 */
#define TPHLEN2_L           (0x0004u)  /* Configure Captivate block Bit 2 */
#define TPHLEN3_L           (0x0008u)  /* Configure Captivate block Bit 3 */

#define CPHLEN0_H           (0x0001u)  /* Configure Captivate block Bit 0 */
#define CPHLEN1_H           (0x0002u)  /* Configure Captivate block Bit 1 */
#define CPHLEN2_H           (0x0004u)  /* Configure Captivate block Bit 2 */
#define CPHLEN3_H           (0x0008u)  /* Configure Captivate block Bit 3 */

#define TPHLEN_0            (0x0000u)  /* Configure Captivate block: 0 */
#define TPHLEN_1            (0x0001u)  /* Configure Captivate block: 1 */
#define TPHLEN_2            (0x0002u)  /* Configure Captivate block: 2 */
#define TPHLEN_3            (0x0003u)  /* Configure Captivate block: 3 */
#define TPHLEN_4            (0x0004u)  /* Configure Captivate block: 4 */
#define TPHLEN_5            (0x0005u)  /* Configure Captivate block: 5 */
#define TPHLEN_6            (0x0006u)  /* Configure Captivate block: 6 */
#define TPHLEN_7            (0x0007u)  /* Configure Captivate block: 7 */
#define TPHLEN_8            (0x0008u)  /* Configure Captivate block: 8 */
#define TPHLEN_9            (0x0009u)  /* Configure Captivate block: 9 */
#define TPHLEN_10            (0x000Au)  /* Configure Captivate block: 10 */
#define TPHLEN_11            (0x000Bu)  /* Configure Captivate block: 11 */
#define TPHLEN_12            (0x000Cu)  /* Configure Captivate block: 12 */
#define TPHLEN_13            (0x000Du)  /* Configure Captivate block: 13 */
#define TPHLEN_14            (0x000Eu)  /* Configure Captivate block: 14 */
#define TPHLEN_15            (0x000Fu)  /* Configure Captivate block: 15 */
#define CPHLEN_0            (0x0000u)  /* Configure Captivate block: 0 */
#define CPHLEN_1            (0x0100u)  /* Configure Captivate block: 1 */
#define CPHLEN_2            (0x0200u)  /* Configure Captivate block: 2 */
#define CPHLEN_3            (0x0300u)  /* Configure Captivate block: 3 */
#define CPHLEN_4            (0x0400u)  /* Configure Captivate block: 4 */
#define CPHLEN_5            (0x0500u)  /* Configure Captivate block: 5 */
#define CPHLEN_6            (0x0600u)  /* Configure Captivate block: 6 */
#define CPHLEN_7            (0x0700u)  /* Configure Captivate block: 7 */
#define CPHLEN_8            (0x0800u)  /* Configure Captivate block: 8 */
#define CPHLEN_9            (0x0900u)  /* Configure Captivate block: 9 */
#define CPHLEN_10            (0x0A00u)  /* Configure Captivate block: 10 */
#define CPHLEN_11            (0x0B00u)  /* Configure Captivate block: 11 */
#define CPHLEN_12            (0x0C00u)  /* Configure Captivate block: 12 */
#define CPHLEN_13            (0x0D00u)  /* Configure Captivate block: 13 */
#define CPHLEN_14            (0x0E00u)  /* Configure Captivate block: 14 */
#define CPHLEN_15            (0x0F00u)  /* Configure Captivate block: 15 */

/* CAPTCCTRL0 */
#define CAPTIMEN            (0x0001u)  /* Captivate timer enable */
#define CAPTCLR             (0x0002u)  /* Captivate timer clear */
#define CAPTSSEL0           (0x0004u)  /* Captivate timer source select Bit 0 */
#define CAPTSSEL1           (0x0008u)  /* Captivate timer source select Bit 1 */
#define CAPTDIV0            (0x0010u)  /* Captivate timer divider Bit 0 */
#define CAPTDIV1            (0x0020u)  /* Captivate timer divider Bit 1 */
#define CAPTDIV2            (0x0040u)  /* Captivate timer divider Bit 2 */
#define CAPTCONV            (0x0080u)  /* Captivate timer conversion start select */
#define CAPCNTREN           (0x0100u)  /* Enables Captivate conversion counter */
#define CAPCNTRCLR          (0x0200u)  /* Captivate conversion counter clear */
#define CAPTCNTRSEL0        (0x0400u)  /* Captivate counter select Bit 0 */
#define CAPTCNTRSEL1        (0x0800u)  /* Captivate counter select Bit 1 */
#define CAPTCNTRSEL2        (0x1000u)  /* Captivate counter select Bit 2 */

#define CAPTIMEN_L          (0x0001u)  /* Captivate timer enable */
#define CAPTCLR_L           (0x0002u)  /* Captivate timer clear */
#define CAPTSSEL0_L         (0x0004u)  /* Captivate timer source select Bit 0 */
#define CAPTSSEL1_L         (0x0008u)  /* Captivate timer source select Bit 1 */
#define CAPTDIV0_L          (0x0010u)  /* Captivate timer divider Bit 0 */
#define CAPTDIV1_L          (0x0020u)  /* Captivate timer divider Bit 1 */
#define CAPTDIV2_L          (0x0040u)  /* Captivate timer divider Bit 2 */
#define CAPTCONV_L          (0x0080u)  /* Captivate timer conversion start select */

#define CAPCNTREN_H         (0x0001u)  /* Enables Captivate conversion counter */
#define CAPCNTRCLR_H        (0x0002u)  /* Captivate conversion counter clear */
#define CAPTCNTRSEL0_H      (0x0004u)  /* Captivate counter select Bit 0 */
#define CAPTCNTRSEL1_H      (0x0008u)  /* Captivate counter select Bit 1 */
#define CAPTCNTRSEL2_H      (0x0010u)  /* Captivate counter select Bit 2 */

#define CAPTSSEL_0          (0x0000u)  /* Captivate timer source select: XT1CLK */
#define CAPTSSEL_1          (0x0004u)  /* Captivate timer source select: VLOCLK */
#define CAPTSSEL_2          (0x0008u)  /* Captivate timer source select: REFO */
#define CAPTSSEL_3          (0x000Cu)  /* Captivate timer source select: Reserved */

#define CAPTSSEL__XT1CLK    (0x0000u)  /* Captivate timer source select: XT1CLK */
#define CAPTSSEL__VLOCLK    (0x0004u)  /* Captivate timer source select: VLOCLK */
#define CAPTSSEL__REFO      (0x0008u)  /* Captivate timer source select: REFO */

#define CAPTDIV_0           (0x0000u)  /* Captivate timer divider: 1 */
#define CAPTDIV_1           (0x0010u)  /* Captivate timer divider: 2 */
#define CAPTDIV_2           (0x0020u)  /* Captivate timer divider: 4 */
#define CAPTDIV_3           (0x0030u)  /* Captivate timer divider: 8 */
#define CAPTDIV_4           (0x0040u)  /* Captivate timer divider: 16 */
#define CAPTDIV_5           (0x0050u)  /* Captivate timer divider: 32 */
#define CAPTDIV_6           (0x0060u)  /* Captivate timer divider: 64 */
#define CAPTDIV_7           (0x0070u)  /* Captivate timer divider: 128 */

#define CAPTDIV__1          (0x0000u)  /* Captivate timer divider: 1 */
#define CAPTDIV__2          (0x0010u)  /* Captivate timer divider: 2 */
#define CAPTDIV__4          (0x0020u)  /* Captivate timer divider: 4 */
#define CAPTDIV__8          (0x0030u)  /* Captivate timer divider: 8 */
#define CAPTDIV__16         (0x0040u)  /* Captivate timer divider: 16 */
#define CAPTDIV__32         (0x0050u)  /* Captivate timer divider: 32 */
#define CAPTDIV__64         (0x0060u)  /* Captivate timer divider: 64 */
#define CAPTDIV__128        (0x0070u)  /* Captivate timer divider: 128 */

#define CAPTCNTRSEL_0       (0x0000u)  /* Captivate counter select: 16 */
#define CAPTCNTRSEL_1       (0x0400u)  /* Captivate counter select: 32 */
#define CAPTCNTRSEL_2       (0x0800u)  /* Captivate counter select: 64 */
#define CAPTCNTRSEL_3       (0x0C00u)  /* Captivate counter select: 128 */
#define CAPTCNTRSEL_4       (0x1000u)  /* Captivate counter select: 256 */
#define CAPTCNTRSEL_5       (0x1400u)  /* Captivate counter select: 512 */
#define CAPTCNTRSEL_6       (0x1800u)  /* Captivate counter select: 1024 */
#define CAPTCNTRSEL_7       (0x1C00u)  /* Captivate counter select: 2048 */

#define CAPTCNTRSEL__16     (0x0000u)  /* Captivate counter select: 16 */
#define CAPTCNTRSEL__32     (0x0400u)  /* Captivate counter select: 32 */
#define CAPTCNTRSEL__64     (0x0800u)  /* Captivate counter select: 64 */
#define CAPTCNTRSEL__128    (0x0C00u)  /* Captivate counter select: 128 */
#define CAPTCNTRSEL__256    (0x1000u)  /* Captivate counter select: 256 */
#define CAPTCNTRSEL__512    (0x1400u)  /* Captivate counter select: 512 */
#define CAPTCNTRSEL__1024   (0x1800u)  /* Captivate counter select: 1024 */
#define CAPTCNTRSEL__2048   (0x1C00u)  /* Captivate counter select: 2048 */

/* CAPTMR0 */
#define xCAPTMR0            (0x0001u)  /* Count of the Captivate timer Bit 0 */
#define xCAPTMR1            (0x0002u)  /* Count of the Captivate timer Bit 1 */
#define xCAPTMR2            (0x0004u)  /* Count of the Captivate timer Bit 2 */
#define xCAPTMR3            (0x0008u)  /* Count of the Captivate timer Bit 3 */
#define xCAPTMR4            (0x0010u)  /* Count of the Captivate timer Bit 4 */
#define xCAPTMR5            (0x0020u)  /* Count of the Captivate timer Bit 5 */
#define xCAPTMR6            (0x0040u)  /* Count of the Captivate timer Bit 6 */
#define xCAPTMR7            (0x0080u)  /* Count of the Captivate timer Bit 7 */
#define xCAPTMR8            (0x0100u)  /* Count of the Captivate timer Bit 8 */
#define xCAPTMR9            (0x0200u)  /* Count of the Captivate timer Bit 9 */
#define xCAPTMR10            (0x0400u)  /* Count of the Captivate timer Bit 10 */
#define xCAPTMR11            (0x0800u)  /* Count of the Captivate timer Bit 11 */
#define xCAPTMR12            (0x1000u)  /* Count of the Captivate timer Bit 12 */
#define xCAPTMR13            (0x2000u)  /* Count of the Captivate timer Bit 13 */
#define xCAPTMR14            (0x4000u)  /* Count of the Captivate timer Bit 14 */
#define xCAPTMR15            (0x8000u)  /* Count of the Captivate timer Bit 15 */

#define xCAPTMR0_L          (0x0001u)  /* Count of the Captivate timer Bit 0 */
#define xCAPTMR1_L          (0x0002u)  /* Count of the Captivate timer Bit 1 */
#define xCAPTMR2_L          (0x0004u)  /* Count of the Captivate timer Bit 2 */
#define xCAPTMR3_L          (0x0008u)  /* Count of the Captivate timer Bit 3 */
#define xCAPTMR4_L          (0x0010u)  /* Count of the Captivate timer Bit 4 */
#define xCAPTMR5_L          (0x0020u)  /* Count of the Captivate timer Bit 5 */
#define xCAPTMR6_L          (0x0040u)  /* Count of the Captivate timer Bit 6 */
#define xCAPTMR7_L          (0x0080u)  /* Count of the Captivate timer Bit 7 */

#define xCAPTMR8_H          (0x0001u)  /* Count of the Captivate timer Bit 8 */
#define xCAPTMR9_H          (0x0002u)  /* Count of the Captivate timer Bit 9 */
#define xCAPTMR10_H         (0x0004u)  /* Count of the Captivate timer Bit 10 */
#define xCAPTMR11_H         (0x0008u)  /* Count of the Captivate timer Bit 11 */
#define xCAPTMR12_H         (0x0010u)  /* Count of the Captivate timer Bit 12 */
#define xCAPTMR13_H         (0x0020u)  /* Count of the Captivate timer Bit 13 */
#define xCAPTMR14_H         (0x0040u)  /* Count of the Captivate timer Bit 14 */
#define xCAPTMR15_H         (0x0080u)  /* Count of the Captivate timer Bit 15 */

/* CAPTCR0 */
#define CAPTCMP0           (0x0001u)  /* Data for the comparison to the timer value Bit 0 */
#define CAPTCMP1           (0x0002u)  /* Data for the comparison to the timer value Bit 1 */
#define CAPTCMP2           (0x0004u)  /* Data for the comparison to the timer value Bit 2 */
#define CAPTCMP3           (0x0008u)  /* Data for the comparison to the timer value Bit 3 */
#define CAPTCMP4           (0x0010u)  /* Data for the comparison to the timer value Bit 4 */
#define CAPTCMP5           (0x0020u)  /* Data for the comparison to the timer value Bit 5 */
#define CAPTCMP6           (0x0040u)  /* Data for the comparison to the timer value Bit 6 */
#define CAPTCMP7           (0x0080u)  /* Data for the comparison to the timer value Bit 7 */
#define CAPTCMP8           (0x0100u)  /* Data for the comparison to the timer value Bit 8 */
#define CAPTCMP9           (0x0200u)  /* Data for the comparison to the timer value Bit 9 */
#define CAPTCMP10           (0x0400u)  /* Data for the comparison to the timer value Bit 10 */
#define CAPTCMP11           (0x0800u)  /* Data for the comparison to the timer value Bit 11 */
#define CAPTCMP12           (0x1000u)  /* Data for the comparison to the timer value Bit 12 */
#define CAPTCMP13           (0x2000u)  /* Data for the comparison to the timer value Bit 13 */
#define CAPTCMP14           (0x4000u)  /* Data for the comparison to the timer value Bit 14 */
#define CAPTCMP15           (0x8000u)  /* Data for the comparison to the timer value Bit 15 */

#define CAPTCMP0_L          (0x0001u)  /* Data for the comparison to the timer value Bit 0 */
#define CAPTCMP1_L          (0x0002u)  /* Data for the comparison to the timer value Bit 1 */
#define CAPTCMP2_L          (0x0004u)  /* Data for the comparison to the timer value Bit 2 */
#define CAPTCMP3_L          (0x0008u)  /* Data for the comparison to the timer value Bit 3 */
#define CAPTCMP4_L          (0x0010u)  /* Data for the comparison to the timer value Bit 4 */
#define CAPTCMP5_L          (0x0020u)  /* Data for the comparison to the timer value Bit 5 */
#define CAPTCMP6_L          (0x0040u)  /* Data for the comparison to the timer value Bit 6 */
#define CAPTCMP7_L          (0x0080u)  /* Data for the comparison to the timer value Bit 7 */

#define CAPTCMP8_H          (0x0001u)  /* Data for the comparison to the timer value Bit 8 */
#define CAPTCMP9_H          (0x0002u)  /* Data for the comparison to the timer value Bit 9 */
#define CAPTCMP10_H         (0x0004u)  /* Data for the comparison to the timer value Bit 10 */
#define CAPTCMP11_H         (0x0008u)  /* Data for the comparison to the timer value Bit 11 */
#define CAPTCMP12_H         (0x0010u)  /* Data for the comparison to the timer value Bit 12 */
#define CAPTCMP13_H         (0x0020u)  /* Data for the comparison to the timer value Bit 13 */
#define CAPTCMP14_H         (0x0040u)  /* Data for the comparison to the timer value Bit 14 */
#define CAPTCMP15_H         (0x0080u)  /* Data for the comparison to the timer value Bit 15 */

/* CAPCNTMAX */
#define CNTMAX0            (0x0001u)  /* Maximum Count Value Register Bit 0 */
#define CNTMAX1            (0x0002u)  /* Maximum Count Value Register Bit 1 */
#define CNTMAX2            (0x0004u)  /* Maximum Count Value Register Bit 2 */
#define CNTMAX3            (0x0008u)  /* Maximum Count Value Register Bit 3 */
#define CNTMAX4            (0x0010u)  /* Maximum Count Value Register Bit 4 */
#define CNTMAX5            (0x0020u)  /* Maximum Count Value Register Bit 5 */
#define CNTMAX6            (0x0040u)  /* Maximum Count Value Register Bit 6 */
#define CNTMAX7            (0x0080u)  /* Maximum Count Value Register Bit 7 */
#define CNTMAX8            (0x0100u)  /* Maximum Count Value Register Bit 8 */
#define CNTMAX9            (0x0200u)  /* Maximum Count Value Register Bit 9 */
#define CNTMAX10            (0x0400u)  /* Maximum Count Value Register Bit 10 */
#define CNTMAX11            (0x0800u)  /* Maximum Count Value Register Bit 11 */
#define CNTMAX12            (0x1000u)  /* Maximum Count Value Register Bit 12 */

#define CNTMAX0_L           (0x0001u)  /* Maximum Count Value Register Bit 0 */
#define CNTMAX1_L           (0x0002u)  /* Maximum Count Value Register Bit 1 */
#define CNTMAX2_L           (0x0004u)  /* Maximum Count Value Register Bit 2 */
#define CNTMAX3_L           (0x0008u)  /* Maximum Count Value Register Bit 3 */
#define CNTMAX4_L           (0x0010u)  /* Maximum Count Value Register Bit 4 */
#define CNTMAX5_L           (0x0020u)  /* Maximum Count Value Register Bit 5 */
#define CNTMAX6_L           (0x0040u)  /* Maximum Count Value Register Bit 6 */
#define CNTMAX7_L           (0x0080u)  /* Maximum Count Value Register Bit 7 */

#define CNTMAX8_H           (0x0001u)  /* Maximum Count Value Register Bit 8 */
#define CNTMAX9_H           (0x0002u)  /* Maximum Count Value Register Bit 9 */
#define CNTMAX10_H          (0x0004u)  /* Maximum Count Value Register Bit 10 */
#define CNTMAX11_H          (0x0008u)  /* Maximum Count Value Register Bit 11 */
#define CNTMAX12_H          (0x0010u)  /* Maximum Count Value Register Bit 12 */

/* CAPFSMCTRL0 */
#define LTABETA0            (0x0001u)  /* These bits weight the value for the LTA data measurement. Bit 0 */
#define LTABETA1            (0x0002u)  /* These bits weight the value for the LTA data measurement. Bit 1 */
#define LTABETA2            (0x0004u)  /* These bits weight the value for the LTA data measurement. Bit 2 */
#define LTACTRL0            (0x0008u)  /* LTA control Bit 0 */
#define LTACTRL1            (0x0010u)  /* LTA control Bit 1 */
#define CNTBETA0            (0x0100u)  /* These bits weight the value for the filtered data measurement. Bit 0 */
#define CNTBETA1            (0x0200u)  /* These bits weight the value for the filtered data measurement. Bit 1 */
#define CNTBETA2            (0x0400u)  /* These bits weight the value for the filtered data measurement. Bit 2 */
#define FSMBYPS             (0x0800u)  /* Bypass the processing state machine */

#define LTABETA0_L          (0x0001u)  /* These bits weight the value for the LTA data measurement. Bit 0 */
#define LTABETA1_L          (0x0002u)  /* These bits weight the value for the LTA data measurement. Bit 1 */
#define LTABETA2_L          (0x0004u)  /* These bits weight the value for the LTA data measurement. Bit 2 */
#define LTACTRL0_L          (0x0008u)  /* LTA control Bit 0 */
#define LTACTRL1_L          (0x0010u)  /* LTA control Bit 1 */

#define CNTBETA0_H          (0x0001u)  /* These bits weight the value for the filtered data measurement. Bit 0 */
#define CNTBETA1_H          (0x0002u)  /* These bits weight the value for the filtered data measurement. Bit 1 */
#define CNTBETA2_H          (0x0004u)  /* These bits weight the value for the filtered data measurement. Bit 2 */
#define FSMBYPS_H           (0x0008u)  /* Bypass the processing state machine */

#define LTABETA_0           (0x0000u)  /* Filtered count beta value: 0 */
#define LTABETA_1           (0x0001u)  /* Filtered count beta value: 1 */
#define LTABETA_2           (0x0002u)  /* Filtered count beta value: 2 */
#define LTABETA_3           (0x0003u)  /* Filtered count beta value: 3 */
#define LTABETA_4           (0x0004u)  /* Filtered count beta value: 4 */
#define LTABETA_5           (0x0005u)  /* Filtered count beta value: 5 */
#define LTABETA_6           (0x0006u)  /* Filtered count beta value: 6 */
#define LTABETA_7           (0x0007u)  /* Filtered count beta value: 7 */

#define LTACTRL_0           (0x0000u)  /* LTA control: 0 */
#define LTACTRL_1           (0x0008u)  /* LTA control: 1 */
#define LTACTRL_2           (0x0010u)  /* LTA control: 2 */
#define LTACTRL_3           (0x0018u)  /* LTA control: 3 */

#define CNTBETA_0           (0x0000u)  /* Filtered count beta value: 0 */
#define CNTBETA_1           (0x0100u)  /* Filtered count beta value: 1 */
#define CNTBETA_2           (0x0200u)  /* Filtered count beta value: 2 */
#define CNTBETA_3           (0x0300u)  /* Filtered count beta value: 3 */
#define CNTBETA_4           (0x0400u)  /* Filtered count beta value: 4 */
#define CNTBETA_5           (0x0500u)  /* Filtered count beta value: 5 */
#define CNTBETA_6           (0x0600u)  /* Filtered count beta value: 6 */
#define CNTBETA_7           (0x0700u)  /* Filtered count beta value: 7 */

/* CAPTHRHI */
#define HITHRES0           (0x0001u)  /* Upper threshold level Bit 0 */
#define HITHRES1           (0x0002u)  /* Upper threshold level Bit 1 */
#define HITHRES2           (0x0004u)  /* Upper threshold level Bit 2 */
#define HITHRES3           (0x0008u)  /* Upper threshold level Bit 3 */
#define HITHRES4           (0x0010u)  /* Upper threshold level Bit 4 */
#define HITHRES5           (0x0020u)  /* Upper threshold level Bit 5 */
#define HITHRES6           (0x0040u)  /* Upper threshold level Bit 6 */
#define HITHRES7           (0x0080u)  /* Upper threshold level Bit 7 */
#define HITHRES8           (0x0100u)  /* Upper threshold level Bit 8 */
#define HITHRES9           (0x0200u)  /* Upper threshold level Bit 9 */
#define HITHRES10           (0x0400u)  /* Upper threshold level Bit 10 */
#define HITHRES11           (0x0800u)  /* Upper threshold level Bit 11 */
#define HITHRES12           (0x1000u)  /* Upper threshold level Bit 12 */

#define HITHRES0_L          (0x0001u)  /* Upper threshold level Bit 0 */
#define HITHRES1_L          (0x0002u)  /* Upper threshold level Bit 1 */
#define HITHRES2_L          (0x0004u)  /* Upper threshold level Bit 2 */
#define HITHRES3_L          (0x0008u)  /* Upper threshold level Bit 3 */
#define HITHRES4_L          (0x0010u)  /* Upper threshold level Bit 4 */
#define HITHRES5_L          (0x0020u)  /* Upper threshold level Bit 5 */
#define HITHRES6_L          (0x0040u)  /* Upper threshold level Bit 6 */
#define HITHRES7_L          (0x0080u)  /* Upper threshold level Bit 7 */

#define HITHRES8_H          (0x0001u)  /* Upper threshold level Bit 8 */
#define HITHRES9_H          (0x0002u)  /* Upper threshold level Bit 9 */
#define HITHRES10_H         (0x0004u)  /* Upper threshold level Bit 10 */
#define HITHRES11_H         (0x0008u)  /* Upper threshold level Bit 11 */
#define HITHRES12_H         (0x0010u)  /* Upper threshold level Bit 12 */

/* CAPTHRLO */
#define LOTHRES0           (0x0001u)  /* Lower threshold level Bit 0 */
#define LOTHRES1           (0x0002u)  /* Lower threshold level Bit 1 */
#define LOTHRES2           (0x0004u)  /* Lower threshold level Bit 2 */
#define LOTHRES3           (0x0008u)  /* Lower threshold level Bit 3 */
#define LOTHRES4           (0x0010u)  /* Lower threshold level Bit 4 */
#define LOTHRES5           (0x0020u)  /* Lower threshold level Bit 5 */
#define LOTHRES6           (0x0040u)  /* Lower threshold level Bit 6 */
#define LOTHRES7           (0x0080u)  /* Lower threshold level Bit 7 */
#define LOTHRES8           (0x0100u)  /* Lower threshold level Bit 8 */
#define LOTHRES9           (0x0200u)  /* Lower threshold level Bit 9 */
#define LOTHRES10           (0x0400u)  /* Lower threshold level Bit 10 */
#define LOTHRES11           (0x0800u)  /* Lower threshold level Bit 11 */
#define LOTHRES12           (0x1000u)  /* Lower threshold level Bit 12 */

#define LOTHRES0_L          (0x0001u)  /* Lower threshold level Bit 0 */
#define LOTHRES1_L          (0x0002u)  /* Lower threshold level Bit 1 */
#define LOTHRES2_L          (0x0004u)  /* Lower threshold level Bit 2 */
#define LOTHRES3_L          (0x0008u)  /* Lower threshold level Bit 3 */
#define LOTHRES4_L          (0x0010u)  /* Lower threshold level Bit 4 */
#define LOTHRES5_L          (0x0020u)  /* Lower threshold level Bit 5 */
#define LOTHRES6_L          (0x0040u)  /* Lower threshold level Bit 6 */
#define LOTHRES7_L          (0x0080u)  /* Lower threshold level Bit 7 */

#define LOTHRES8_H          (0x0001u)  /* Lower threshold level Bit 8 */
#define LOTHRES9_H          (0x0002u)  /* Lower threshold level Bit 9 */
#define LOTHRES10_H         (0x0004u)  /* Lower threshold level Bit 10 */
#define LOTHRES11_H         (0x0008u)  /* Lower threshold level Bit 11 */
#define LOTHRES12_H         (0x0010u)  /* Lower threshold level Bit 12 */

/* CAPDETECT0 */
#define CAPDTCT0           (0x0001u)  /* Indicates a detected threshold on Captivate block 0 */
#define CAPDTCT1           (0x0002u)  /* Indicates a detected threshold on Captivate block 1 */
#define CAPDTCT2           (0x0004u)  /* Indicates a detected threshold on Captivate block 2 */
#define CAPDTCT3           (0x0008u)  /* Indicates a detected threshold on Captivate block 3 */
#define CAPDTCT4           (0x0010u)  /* Indicates a detected threshold on Captivate block 4 */
#define CAPDTCT5           (0x0020u)  /* Indicates a detected threshold on Captivate block 5 */
#define CAPDTCT6           (0x0040u)  /* Indicates a detected threshold on Captivate block 6 */
#define CAPDTCT7           (0x0080u)  /* Indicates a detected threshold on Captivate block 7 */
#define CAPDTCT8           (0x0100u)  /* Indicates a detected threshold on Captivate block 8 */
#define CAPDTCT9           (0x0200u)  /* Indicates a detected threshold on Captivate block 9 */
#define CAPDTCT10           (0x0400u)  /* Indicates a detected threshold on Captivate block 10 */
#define CAPDTCT11           (0x0800u)  /* Indicates a detected threshold on Captivate block 11 */

#define CAPDTCT0_L          (0x0001u)  /* Indicates a detected threshold on Captivate block 0 */
#define CAPDTCT1_L          (0x0002u)  /* Indicates a detected threshold on Captivate block 1 */
#define CAPDTCT2_L          (0x0004u)  /* Indicates a detected threshold on Captivate block 2 */
#define CAPDTCT3_L          (0x0008u)  /* Indicates a detected threshold on Captivate block 3 */
#define CAPDTCT4_L          (0x0010u)  /* Indicates a detected threshold on Captivate block 4 */
#define CAPDTCT5_L          (0x0020u)  /* Indicates a detected threshold on Captivate block 5 */
#define CAPDTCT6_L          (0x0040u)  /* Indicates a detected threshold on Captivate block 6 */
#define CAPDTCT7_L          (0x0080u)  /* Indicates a detected threshold on Captivate block 7 */

#define CAPDTCT8_H          (0x0001u)  /* Indicates a detected threshold on Captivate block 8 */
#define CAPDTCT9_H          (0x0002u)  /* Indicates a detected threshold on Captivate block 9 */
#define CAPDTCT10_H         (0x0004u)  /* Indicates a detected threshold on Captivate block 10 */
#define CAPDTCT11_H         (0x0008u)  /* Indicates a detected threshold on Captivate block 11 */

/* CAPREFC */
#define REFCAPEN0           (0x0001u)  /* Enable reference circuit for a specific Captivate block Bit 0 */
#define REFCAPEN1           (0x0002u)  /* Enable reference circuit for a specific Captivate block Bit 1 */
#define REFCAPEN2           (0x0004u)  /* Enable reference circuit for a specific Captivate block Bit 2 */
#define REFCAPEN3           (0x0008u)  /* Enable reference circuit for a specific Captivate block Bit 3 */
#define REFCAPSZ0           (0x0100u)  /* Select calibration capacitances Bit 0 */
#define REFCAPSZ1           (0x0200u)  /* Select calibration capacitances Bit 1 */
#define REFCAPSZ2           (0x0400u)  /* Select calibration capacitances Bit 2 */

#define REFCAPEN0_L         (0x0001u)  /* Enable reference circuit for a specific Captivate block Bit 0 */
#define REFCAPEN1_L         (0x0002u)  /* Enable reference circuit for a specific Captivate block Bit 1 */
#define REFCAPEN2_L         (0x0004u)  /* Enable reference circuit for a specific Captivate block Bit 2 */
#define REFCAPEN3_L         (0x0008u)  /* Enable reference circuit for a specific Captivate block Bit 3 */

#define REFCAPSZ0_H         (0x0001u)  /* Select calibration capacitances Bit 0 */
#define REFCAPSZ1_H         (0x0002u)  /* Select calibration capacitances Bit 1 */
#define REFCAPSZ2_H         (0x0004u)  /* Select calibration capacitances Bit 2 */

#define REFCAPEN_0          (0x0000u)  /* Enable reference circuit for a specific Captivate block: 0 */
#define REFCAPEN_1          (0x0001u)  /* Enable reference circuit for a specific Captivate block: 1 */
#define REFCAPEN_2          (0x0002u)  /* Enable reference circuit for a specific Captivate block: 2 */
#define REFCAPEN_3          (0x0003u)  /* Enable reference circuit for a specific Captivate block: 3 */
#define REFCAPEN_4          (0x0004u)  /* Enable reference circuit for a specific Captivate block: 4 */
#define REFCAPEN_5          (0x0005u)  /* Enable reference circuit for a specific Captivate block: 5 */
#define REFCAPEN_6          (0x0006u)  /* Enable reference circuit for a specific Captivate block: 6 */
#define REFCAPEN_7          (0x0007u)  /* Enable reference circuit for a specific Captivate block: 7 */
#define REFCAPEN_8          (0x0008u)  /* Enable reference circuit for a specific Captivate block: 8 */
#define REFCAPEN_9          (0x0009u)  /* Enable reference circuit for a specific Captivate block: 9 */
#define REFCAPEN_10          (0x000Au)  /* Enable reference circuit for a specific Captivate block: 10 */
#define REFCAPEN_11          (0x000Bu)  /* Enable reference circuit for a specific Captivate block: 11 */
#define REFCAPEN_12          (0x000Cu)  /* Enable reference circuit for a specific Captivate block: 12 */
#define REFCAPEN_13          (0x000Du)  /* Enable reference circuit for a specific Captivate block: 13 */
#define REFCAPEN_14          (0x000Eu)  /* Enable reference circuit for a specific Captivate block: 14 */
#define REFCAPEN_15          (0x000Fu)  /* Enable reference circuit for a specific Captivate block: 15 */
#define REFCAPSZ_0          (0x0000u)  /* Select calibration capacitances: 0 */
#define REFCAPSZ_1          (0x0100u)  /* Select calibration capacitances: 1 */
#define REFCAPSZ_2          (0x0200u)  /* Select calibration capacitances: 2 */
#define REFCAPSZ_3          (0x0300u)  /* Select calibration capacitances: 3 */
#define REFCAPSZ_4          (0x0400u)  /* Select calibration capacitances: 4 */
#define REFCAPSZ_5          (0x0500u)  /* Select calibration capacitances: 5 */
#define REFCAPSZ_6          (0x0600u)  /* Select calibration capacitances: 6 */
#define REFCAPSZ_7          (0x0700u)  /* Select calibration capacitances: 7 */

/* CAPCHEN0 */
#define CH0EN0              (0x0001u)  /* Enable channel CAP0.0 */
#define CH0EN1              (0x0002u)  /* Enable channel CAP0.1 */
#define CH0EN2              (0x0004u)  /* Enable channel CAP0.2 */
#define CH0EN3              (0x0008u)  /* Enable channel CAP0.3 */
#define CH0EN4              (0x0010u)  /* Enable channel CAP0.4 */
#define CH0EN5              (0x0020u)  /* Enable channel CAP0.5 */
#define CH0EN6              (0x0040u)  /* Enable channel CAP0.6 */
#define CH0EN7              (0x0080u)  /* Enable channel CAP0.7 */
#define CH1EN0              (0x0100u)  /* Enable channel CAP1.0 */
#define CH1EN1              (0x0200u)  /* Enable channel CAP1.1 */
#define CH1EN2              (0x0400u)  /* Enable channel CAP1.2 */
#define CH1EN3              (0x0800u)  /* Enable channel CAP1.3 */
#define CH1EN4              (0x1000u)  /* Enable channel CAP1.4 */
#define CH1EN5              (0x2000u)  /* Enable channel CAP1.5 */
#define CH1EN6              (0x4000u)  /* Enable channel CAP1.6 */
#define CH1EN7              (0x8000u)  /* Enable channel CAP1.7 */

#define CH0EN0_L            (0x0001u)  /* Enable channel CAP0.0 */
#define CH0EN1_L            (0x0002u)  /* Enable channel CAP0.1 */
#define CH0EN2_L            (0x0004u)  /* Enable channel CAP0.2 */
#define CH0EN3_L            (0x0008u)  /* Enable channel CAP0.3 */
#define CH0EN4_L            (0x0010u)  /* Enable channel CAP0.4 */
#define CH0EN5_L            (0x0020u)  /* Enable channel CAP0.5 */
#define CH0EN6_L            (0x0040u)  /* Enable channel CAP0.6 */
#define CH0EN7_L            (0x0080u)  /* Enable channel CAP0.7 */

#define CH1EN0_H            (0x0001u)  /* Enable channel CAP1.0 */
#define CH1EN1_H            (0x0002u)  /* Enable channel CAP1.1 */
#define CH1EN2_H            (0x0004u)  /* Enable channel CAP1.2 */
#define CH1EN3_H            (0x0008u)  /* Enable channel CAP1.3 */
#define CH1EN4_H            (0x0010u)  /* Enable channel CAP1.4 */
#define CH1EN5_H            (0x0020u)  /* Enable channel CAP1.5 */
#define CH1EN6_H            (0x0040u)  /* Enable channel CAP1.6 */
#define CH1EN7_H            (0x0080u)  /* Enable channel CAP1.7 */

/* CAPCHEN1 */
#define CH2EN0              (0x0001u)  /* Enable channel CAP2.0 */
#define CH2EN1              (0x0002u)  /* Enable channel CAP2.1 */
#define CH2EN2              (0x0004u)  /* Enable channel CAP2.2 */
#define CH2EN3              (0x0008u)  /* Enable channel CAP2.3 */
#define CH2EN4              (0x0010u)  /* Enable channel CAP2.4 */
#define CH2EN5              (0x0020u)  /* Enable channel CAP2.5 */
#define CH2EN6              (0x0040u)  /* Enable channel CAP2.6 */
#define CH2EN7              (0x0080u)  /* Enable channel CAP2.7 */
#define CH3EN0              (0x0100u)  /* Enable channel CAP3.0 */
#define CH3EN1              (0x0200u)  /* Enable channel CAP3.1 */
#define CH3EN2              (0x0400u)  /* Enable channel CAP3.2 */
#define CH3EN3              (0x0800u)  /* Enable channel CAP3.3 */
#define CH3EN4              (0x1000u)  /* Enable channel CAP3.4 */
#define CH3EN5              (0x2000u)  /* Enable channel CAP3.5 */
#define CH3EN6              (0x4000u)  /* Enable channel CAP3.6 */
#define CH3EN7              (0x8000u)  /* Enable channel CAP3.7 */

#define CH2EN0_L            (0x0001u)  /* Enable channel CAP2.0 */
#define CH2EN1_L            (0x0002u)  /* Enable channel CAP2.1 */
#define CH2EN2_L            (0x0004u)  /* Enable channel CAP2.2 */
#define CH2EN3_L            (0x0008u)  /* Enable channel CAP2.3 */
#define CH2EN4_L            (0x0010u)  /* Enable channel CAP2.4 */
#define CH2EN5_L            (0x0020u)  /* Enable channel CAP2.5 */
#define CH2EN6_L            (0x0040u)  /* Enable channel CAP2.6 */
#define CH2EN7_L            (0x0080u)  /* Enable channel CAP2.7 */

#define CH3EN0_H            (0x0001u)  /* Enable channel CAP3.0 */
#define CH3EN1_H            (0x0002u)  /* Enable channel CAP3.1 */
#define CH3EN2_H            (0x0004u)  /* Enable channel CAP3.2 */
#define CH3EN3_H            (0x0008u)  /* Enable channel CAP3.3 */
#define CH3EN4_H            (0x0010u)  /* Enable channel CAP3.4 */
#define CH3EN5_H            (0x0020u)  /* Enable channel CAP3.5 */
#define CH3EN6_H            (0x0040u)  /* Enable channel CAP3.6 */
#define CH3EN7_H            (0x0080u)  /* Enable channel CAP3.7 */

/* CAPCHACT0 */
#define CH0ACT0             (0x0001u)  /* Control function of activated Captivate channel CAP0.0 */
#define CH0ACT1             (0x0002u)  /* Control function of activated Captivate channel CAP0.1 */
#define CH0ACT2             (0x0004u)  /* Control function of activated Captivate channel CAP0.2 */
#define CH0ACT3             (0x0008u)  /* Control function of activated Captivate channel CAP0.3 */
#define CH0ACT4             (0x0010u)  /* Control function of activated Captivate channel CAP0.4 */
#define CH0ACT5             (0x0020u)  /* Control function of activated Captivate channel CAP0.5 */
#define CH0ACT6             (0x0040u)  /* Control function of activated Captivate channel CAP0.6 */
#define CH0ACT7             (0x0080u)  /* Control function of activated Captivate channel CAP0.7 */
#define CH1ACT0             (0x0100u)  /* Control function of activated Captivate channel CAP1.0 */
#define CH1ACT1             (0x0200u)  /* Control function of activated Captivate channel CAP1.1 */
#define CH1ACT2             (0x0400u)  /* Control function of activated Captivate channel CAP1.2 */
#define CH1ACT3             (0x0800u)  /* Control function of activated Captivate channel CAP1.3 */
#define CH1ACT4             (0x1000u)  /* Control function of activated Captivate channel CAP1.4 */
#define CH1ACT5             (0x2000u)  /* Control function of activated Captivate channel CAP1.5 */
#define CH1ACT6             (0x4000u)  /* Control function of activated Captivate channel CAP1.6 */
#define CH1ACT7             (0x8000u)  /* Control function of activated Captivate channel CAP1.7 */

#define CH0ACT0_L           (0x0001u)  /* Control function of activated Captivate channel CAP0.0 */
#define CH0ACT1_L           (0x0002u)  /* Control function of activated Captivate channel CAP0.1 */
#define CH0ACT2_L           (0x0004u)  /* Control function of activated Captivate channel CAP0.2 */
#define CH0ACT3_L           (0x0008u)  /* Control function of activated Captivate channel CAP0.3 */
#define CH0ACT4_L           (0x0010u)  /* Control function of activated Captivate channel CAP0.4 */
#define CH0ACT5_L           (0x0020u)  /* Control function of activated Captivate channel CAP0.5 */
#define CH0ACT6_L           (0x0040u)  /* Control function of activated Captivate channel CAP0.6 */
#define CH0ACT7_L           (0x0080u)  /* Control function of activated Captivate channel CAP0.7 */

#define CH1ACT0_H           (0x0001u)  /* Control function of activated Captivate channel CAP1.0 */
#define CH1ACT1_H           (0x0002u)  /* Control function of activated Captivate channel CAP1.1 */
#define CH1ACT2_H           (0x0004u)  /* Control function of activated Captivate channel CAP1.2 */
#define CH1ACT3_H           (0x0008u)  /* Control function of activated Captivate channel CAP1.3 */
#define CH1ACT4_H           (0x0010u)  /* Control function of activated Captivate channel CAP1.4 */
#define CH1ACT5_H           (0x0020u)  /* Control function of activated Captivate channel CAP1.5 */
#define CH1ACT6_H           (0x0040u)  /* Control function of activated Captivate channel CAP1.6 */
#define CH1ACT7_H           (0x0080u)  /* Control function of activated Captivate channel CAP1.7 */

/* CAPCHACT1 */
#define CH2ACT0             (0x0001u)  /* Control function of activated Captivate channel CAP2.0 */
#define CH2ACT1             (0x0002u)  /* Control function of activated Captivate channel CAP2.1 */
#define CH2ACT2             (0x0004u)  /* Control function of activated Captivate channel CAP2.2 */
#define CH2ACT3             (0x0008u)  /* Control function of activated Captivate channel CAP2.3 */
#define CH2ACT4             (0x0010u)  /* Control function of activated Captivate channel CAP2.4 */
#define CH2ACT5             (0x0020u)  /* Control function of activated Captivate channel CAP2.5 */
#define CH2ACT6             (0x0040u)  /* Control function of activated Captivate channel CAP2.6 */
#define CH2ACT7             (0x0080u)  /* Control function of activated Captivate channel CAP2.7 */
#define CH3ACT0             (0x0100u)  /* Control function of activated Captivate channel CAP3.0 */
#define CH3ACT1             (0x0200u)  /* Control function of activated Captivate channel CAP3.1 */
#define CH3ACT2             (0x0400u)  /* Control function of activated Captivate channel CAP3.2 */
#define CH3ACT3             (0x0800u)  /* Control function of activated Captivate channel CAP3.3 */
#define CH3ACT4             (0x1000u)  /* Control function of activated Captivate channel CAP3.4 */
#define CH3ACT5             (0x2000u)  /* Control function of activated Captivate channel CAP3.5 */
#define CH3ACT6             (0x4000u)  /* Control function of activated Captivate channel CAP3.6 */
#define CH3ACT7             (0x8000u)  /* Control function of activated Captivate channel CAP3.7 */

#define CH2ACT0_L           (0x0001u)  /* Control function of activated Captivate channel CAP2.0 */
#define CH2ACT1_L           (0x0002u)  /* Control function of activated Captivate channel CAP2.1 */
#define CH2ACT2_L           (0x0004u)  /* Control function of activated Captivate channel CAP2.2 */
#define CH2ACT3_L           (0x0008u)  /* Control function of activated Captivate channel CAP2.3 */
#define CH2ACT4_L           (0x0010u)  /* Control function of activated Captivate channel CAP2.4 */
#define CH2ACT5_L           (0x0020u)  /* Control function of activated Captivate channel CAP2.5 */
#define CH2ACT6_L           (0x0040u)  /* Control function of activated Captivate channel CAP2.6 */
#define CH2ACT7_L           (0x0080u)  /* Control function of activated Captivate channel CAP2.7 */

#define CH3ACT0_H           (0x0001u)  /* Control function of activated Captivate channel CAP3.0 */
#define CH3ACT1_H           (0x0002u)  /* Control function of activated Captivate channel CAP3.1 */
#define CH3ACT2_H           (0x0004u)  /* Control function of activated Captivate channel CAP3.2 */
#define CH3ACT3_H           (0x0008u)  /* Control function of activated Captivate channel CAP3.3 */
#define CH3ACT4_H           (0x0010u)  /* Control function of activated Captivate channel CAP3.4 */
#define CH3ACT5_H           (0x0020u)  /* Control function of activated Captivate channel CAP3.5 */
#define CH3ACT6_H           (0x0040u)  /* Control function of activated Captivate channel CAP3.6 */
#define CH3ACT7_H           (0x0080u)  /* Control function of activated Captivate channel CAP3.7 */

/* CAPCHCTRL0 */
#define CH0CTRL0            (0x0001u)  /* Channel control (TX/RX) CAP0.0 */
#define CH0CTRL1            (0x0002u)  /* Channel control (TX/RX) CAP0.1 */
#define CH0CTRL2            (0x0004u)  /* Channel control (TX/RX) CAP0.2 */
#define CH0CTRL3            (0x0008u)  /* Channel control (TX/RX) CAP0.3 */
#define CH0CTRL4            (0x0010u)  /* Channel control (TX/RX) CAP0.4 */
#define CH0CTRL5            (0x0020u)  /* Channel control (TX/RX) CAP0.5 */
#define CH0CTRL6            (0x0040u)  /* Channel control (TX/RX) CAP0.6 */
#define CH0CTRL7            (0x0080u)  /* Channel control (TX/RX) CAP0.7 */
#define CH1CTRL0            (0x0100u)  /* Channel control (TX/RX) CAP1.0 */
#define CH1CTRL1            (0x0200u)  /* Channel control (TX/RX) CAP1.1 */
#define CH1CTRL2            (0x0400u)  /* Channel control (TX/RX) CAP1.2 */
#define CH1CTRL3            (0x0800u)  /* Channel control (TX/RX) CAP1.3 */
#define CH1CTRL4            (0x1000u)  /* Channel control (TX/RX) CAP1.4 */
#define CH1CTRL5            (0x2000u)  /* Channel control (TX/RX) CAP1.5 */
#define CH1CTRL6            (0x4000u)  /* Channel control (TX/RX) CAP1.6 */
#define CH1CTRL7            (0x8000u)  /* Channel control (TX/RX) CAP1.7 */

#define CH0CTRL0_L          (0x0001u)  /* Channel control (TX/RX) CAP0.0 */
#define CH0CTRL1_L          (0x0002u)  /* Channel control (TX/RX) CAP0.1 */
#define CH0CTRL2_L          (0x0004u)  /* Channel control (TX/RX) CAP0.2 */
#define CH0CTRL3_L          (0x0008u)  /* Channel control (TX/RX) CAP0.3 */
#define CH0CTRL4_L          (0x0010u)  /* Channel control (TX/RX) CAP0.4 */
#define CH0CTRL5_L          (0x0020u)  /* Channel control (TX/RX) CAP0.5 */
#define CH0CTRL6_L          (0x0040u)  /* Channel control (TX/RX) CAP0.6 */
#define CH0CTRL7_L          (0x0080u)  /* Channel control (TX/RX) CAP0.7 */

#define CH1CTRL0_H          (0x0001u)  /* Channel control (TX/RX) CAP1.0 */
#define CH1CTRL1_H          (0x0002u)  /* Channel control (TX/RX) CAP1.1 */
#define CH1CTRL2_H          (0x0004u)  /* Channel control (TX/RX) CAP1.2 */
#define CH1CTRL3_H          (0x0008u)  /* Channel control (TX/RX) CAP1.3 */
#define CH1CTRL4_H          (0x0010u)  /* Channel control (TX/RX) CAP1.4 */
#define CH1CTRL5_H          (0x0020u)  /* Channel control (TX/RX) CAP1.5 */
#define CH1CTRL6_H          (0x0040u)  /* Channel control (TX/RX) CAP1.6 */
#define CH1CTRL7_H          (0x0080u)  /* Channel control (TX/RX) CAP1.7 */

/* CAPCHCTRL1 */
#define CH2CTRL0            (0x0001u)  /* Channel control (TX/RX) CAP2.0 */
#define CH2CTRL1            (0x0002u)  /* Channel control (TX/RX) CAP2.1 */
#define CH2CTRL2            (0x0004u)  /* Channel control (TX/RX) CAP2.2 */
#define CH2CTRL3            (0x0008u)  /* Channel control (TX/RX) CAP2.3 */
#define CH2CTRL4            (0x0010u)  /* Channel control (TX/RX) CAP2.4 */
#define CH2CTRL5            (0x0020u)  /* Channel control (TX/RX) CAP2.5 */
#define CH2CTRL6            (0x0040u)  /* Channel control (TX/RX) CAP2.6 */
#define CH2CTRL7            (0x0080u)  /* Channel control (TX/RX) CAP2.7 */
#define CH3CTRL0            (0x0100u)  /* Channel control (TX/RX) CAP3.0 */
#define CH3CTRL1            (0x0200u)  /* Channel control (TX/RX) CAP3.1 */
#define CH3CTRL2            (0x0400u)  /* Channel control (TX/RX) CAP3.2 */
#define CH3CTRL3            (0x0800u)  /* Channel control (TX/RX) CAP3.3 */
#define CH3CTRL4            (0x1000u)  /* Channel control (TX/RX) CAP3.4 */
#define CH3CTRL5            (0x2000u)  /* Channel control (TX/RX) CAP3.5 */
#define CH3CTRL6            (0x4000u)  /* Channel control (TX/RX) CAP3.6 */
#define CH3CTRL7            (0x8000u)  /* Channel control (TX/RX) CAP3.7 */

#define CH2CTRL0_L          (0x0001u)  /* Channel control (TX/RX) CAP2.0 */
#define CH2CTRL1_L          (0x0002u)  /* Channel control (TX/RX) CAP2.1 */
#define CH2CTRL2_L          (0x0004u)  /* Channel control (TX/RX) CAP2.2 */
#define CH2CTRL3_L          (0x0008u)  /* Channel control (TX/RX) CAP2.3 */
#define CH2CTRL4_L          (0x0010u)  /* Channel control (TX/RX) CAP2.4 */
#define CH2CTRL5_L          (0x0020u)  /* Channel control (TX/RX) CAP2.5 */
#define CH2CTRL6_L          (0x0040u)  /* Channel control (TX/RX) CAP2.6 */
#define CH2CTRL7_L          (0x0080u)  /* Channel control (TX/RX) CAP2.7 */

#define CH3CTRL0_H          (0x0001u)  /* Channel control (TX/RX) CAP3.0 */
#define CH3CTRL1_H          (0x0002u)  /* Channel control (TX/RX) CAP3.1 */
#define CH3CTRL2_H          (0x0004u)  /* Channel control (TX/RX) CAP3.2 */
#define CH3CTRL3_H          (0x0008u)  /* Channel control (TX/RX) CAP3.3 */
#define CH3CTRL4_H          (0x0010u)  /* Channel control (TX/RX) CAP3.4 */
#define CH3CTRL5_H          (0x0020u)  /* Channel control (TX/RX) CAP3.5 */
#define CH3CTRL6_H          (0x0040u)  /* Channel control (TX/RX) CAP3.6 */
#define CH3CTRL7_H          (0x0080u)  /* Channel control (TX/RX) CAP3.7 */

/* CAPCHOFF0 */
#define CH0OFF0             (0x0001u)  /* Channel control (High-Z/GND) CAP0.0 */
#define CH0OFF1             (0x0002u)  /* Channel control (High-Z/GND) CAP0.1 */
#define CH0OFF2             (0x0004u)  /* Channel control (High-Z/GND) CAP0.2 */
#define CH0OFF3             (0x0008u)  /* Channel control (High-Z/GND) CAP0.3 */
#define CH0OFF4             (0x0010u)  /* Channel control (High-Z/GND) CAP0.4 */
#define CH0OFF5             (0x0020u)  /* Channel control (High-Z/GND) CAP0.5 */
#define CH0OFF6             (0x0040u)  /* Channel control (High-Z/GND) CAP0.6 */
#define CH0OFF7             (0x0080u)  /* Channel control (High-Z/GND) CAP0.7 */
#define CH1OFF0             (0x0100u)  /* Channel control (High-Z/GND) CAP1.0 */
#define CH1OFF1             (0x0200u)  /* Channel control (High-Z/GND) CAP1.1 */
#define CH1OFF2             (0x0400u)  /* Channel control (High-Z/GND) CAP1.2 */
#define CH1OFF3             (0x0800u)  /* Channel control (High-Z/GND) CAP1.3 */
#define CH1OFF4             (0x1000u)  /* Channel control (High-Z/GND) CAP1.4 */
#define CH1OFF5             (0x2000u)  /* Channel control (High-Z/GND) CAP1.5 */
#define CH1OFF6             (0x4000u)  /* Channel control (High-Z/GND) CAP1.6 */
#define CH1OFF7             (0x8000u)  /* Channel control (High-Z/GND) CAP1.7 */

#define CH0OFF0_L           (0x0001u)  /* Channel control (High-Z/GND) CAP0.0 */
#define CH0OFF1_L           (0x0002u)  /* Channel control (High-Z/GND) CAP0.1 */
#define CH0OFF2_L           (0x0004u)  /* Channel control (High-Z/GND) CAP0.2 */
#define CH0OFF3_L           (0x0008u)  /* Channel control (High-Z/GND) CAP0.3 */
#define CH0OFF4_L           (0x0010u)  /* Channel control (High-Z/GND) CAP0.4 */
#define CH0OFF5_L           (0x0020u)  /* Channel control (High-Z/GND) CAP0.5 */
#define CH0OFF6_L           (0x0040u)  /* Channel control (High-Z/GND) CAP0.6 */
#define CH0OFF7_L           (0x0080u)  /* Channel control (High-Z/GND) CAP0.7 */

#define CH1OFF0_H           (0x0001u)  /* Channel control (High-Z/GND) CAP1.0 */
#define CH1OFF1_H           (0x0002u)  /* Channel control (High-Z/GND) CAP1.1 */
#define CH1OFF2_H           (0x0004u)  /* Channel control (High-Z/GND) CAP1.2 */
#define CH1OFF3_H           (0x0008u)  /* Channel control (High-Z/GND) CAP1.3 */
#define CH1OFF4_H           (0x0010u)  /* Channel control (High-Z/GND) CAP1.4 */
#define CH1OFF5_H           (0x0020u)  /* Channel control (High-Z/GND) CAP1.5 */
#define CH1OFF6_H           (0x0040u)  /* Channel control (High-Z/GND) CAP1.6 */
#define CH1OFF7_H           (0x0080u)  /* Channel control (High-Z/GND) CAP1.7 */

/* CAPCHOFF1 */
#define CH2OFF0             (0x0001u)  /* Channel control (High-Z/GND) CAP2.0 */
#define CH2OFF1             (0x0002u)  /* Channel control (High-Z/GND) CAP2.1 */
#define CH2OFF2             (0x0004u)  /* Channel control (High-Z/GND) CAP2.2 */
#define CH2OFF3             (0x0008u)  /* Channel control (High-Z/GND) CAP2.3 */
#define CH2OFF4             (0x0010u)  /* Channel control (High-Z/GND) CAP2.4 */
#define CH2OFF5             (0x0020u)  /* Channel control (High-Z/GND) CAP2.5 */
#define CH2OFF6             (0x0040u)  /* Channel control (High-Z/GND) CAP2.6 */
#define CH2OFF7             (0x0080u)  /* Channel control (High-Z/GND) CAP2.7 */
#define CH3OFF0             (0x0100u)  /* Channel control (High-Z/GND) CAP3.0 */
#define CH3OFF1             (0x0200u)  /* Channel control (High-Z/GND) CAP3.1 */
#define CH3OFF2             (0x0400u)  /* Channel control (High-Z/GND) CAP3.2 */
#define CH3OFF3             (0x0800u)  /* Channel control (High-Z/GND) CAP3.3 */
#define CH3OFF4             (0x1000u)  /* Channel control (High-Z/GND) CAP3.4 */
#define CH3OFF5             (0x2000u)  /* Channel control (High-Z/GND) CAP3.5 */
#define CH3OFF6             (0x4000u)  /* Channel control (High-Z/GND) CAP3.6 */
#define CH3OFF7             (0x8000u)  /* Channel control (High-Z/GND) CAP3.7 */

#define CH2OFF0_L           (0x0001u)  /* Channel control (High-Z/GND) CAP2.0 */
#define CH2OFF1_L           (0x0002u)  /* Channel control (High-Z/GND) CAP2.1 */
#define CH2OFF2_L           (0x0004u)  /* Channel control (High-Z/GND) CAP2.2 */
#define CH2OFF3_L           (0x0008u)  /* Channel control (High-Z/GND) CAP2.3 */
#define CH2OFF4_L           (0x0010u)  /* Channel control (High-Z/GND) CAP2.4 */
#define CH2OFF5_L           (0x0020u)  /* Channel control (High-Z/GND) CAP2.5 */
#define CH2OFF6_L           (0x0040u)  /* Channel control (High-Z/GND) CAP2.6 */
#define CH2OFF7_L           (0x0080u)  /* Channel control (High-Z/GND) CAP2.7 */

#define CH3OFF0_H           (0x0001u)  /* Channel control (High-Z/GND) CAP3.0 */
#define CH3OFF1_H           (0x0002u)  /* Channel control (High-Z/GND) CAP3.1 */
#define CH3OFF2_H           (0x0004u)  /* Channel control (High-Z/GND) CAP3.2 */
#define CH3OFF3_H           (0x0008u)  /* Channel control (High-Z/GND) CAP3.3 */
#define CH3OFF4_H           (0x0010u)  /* Channel control (High-Z/GND) CAP3.4 */
#define CH3OFF5_H           (0x0020u)  /* Channel control (High-Z/GND) CAP3.5 */
#define CH3OFF6_H           (0x0040u)  /* Channel control (High-Z/GND) CAP3.6 */
#define CH3OFF7_H           (0x0080u)  /* Channel control (High-Z/GND) CAP3.7 */

/* CAPRATIOx */
#define RATIOP0            (0x0001u)   /* Ratio selection P Bit: 0 */
#define RATIOP1            (0x0002u)   /* Ratio selection P Bit: 1 */
#define RATIOP2            (0x0004u)   /* Ratio selection P Bit: 2 */
#define RATIOP3            (0x0008u)   /* Ratio selection P Bit: 3 */
#define RATIOP4            (0x0010u)   /* Ratio selection P Bit: 4 */
#define RATIOP5            (0x0020u)   /* Ratio selection P Bit: 5 */
#define RATION0            (0x0100u)  /* Ratio selection N Bit: 0 */
#define RATION1            (0x0200u)  /* Ratio selection N Bit: 1 */
#define RATION2            (0x0400u)  /* Ratio selection N Bit: 2 */

#define RATIOP0_L           (0x0001u)   /* Ratio selection P Bit: 0 */
#define RATIOP1_L           (0x0002u)   /* Ratio selection P Bit: 1 */
#define RATIOP2_L           (0x0004u)   /* Ratio selection P Bit: 2 */
#define RATIOP3_L           (0x0008u)   /* Ratio selection P Bit: 3 */
#define RATIOP4_L           (0x0010u)   /* Ratio selection P Bit: 4 */
#define RATIOP5_L           (0x0020u)   /* Ratio selection P Bit: 5 */

#define RATION0_H           (0x0001u)  /* Ratio selection N Bit: 0 */
#define RATION1_H           (0x0002u)  /* Ratio selection N Bit: 1 */
#define RATION2_H           (0x0004u)  /* Ratio selection N Bit: 2 */

#define RATIOP_0           (0x0000u)  /* Ratio selection P: 0 */
#define RATIOP_1           (0x0001u)  /* Ratio selection P: 1 */
#define RATIOP_2           (0x0002u)  /* Ratio selection P: 2 */
#define RATIOP_3           (0x0003u)  /* Ratio selection P: 3 */
#define RATIOP_4           (0x0004u)  /* Ratio selection P: 4 */
#define RATIOP_5           (0x0005u)  /* Ratio selection P: 5 */
#define RATIOP_6           (0x0006u)  /* Ratio selection P: 6 */
#define RATIOP_7           (0x0007u)  /* Ratio selection P: 7 */
#define RATIOP_8           (0x0008u)  /* Ratio selection P: 8 */
#define RATIOP_9           (0x0009u)  /* Ratio selection P: 9 */
#define RATIOP_10           (0x000Au)  /* Ratio selection P: 10 */
#define RATIOP_11           (0x000Bu)  /* Ratio selection P: 11 */
#define RATIOP_12           (0x000Cu)  /* Ratio selection P: 12 */
#define RATIOP_13           (0x000Du)  /* Ratio selection P: 13 */
#define RATIOP_14           (0x000Eu)  /* Ratio selection P: 14 */
#define RATIOP_15           (0x000Fu)  /* Ratio selection P: 15 */
#define RATIOP_16           (0x0010u)  /* Ratio selection P: 16 */
#define RATIOP_17           (0x0011u)  /* Ratio selection P: 17 */
#define RATIOP_18           (0x0012u)  /* Ratio selection P: 18 */
#define RATIOP_19           (0x0013u)  /* Ratio selection P: 19 */
#define RATIOP_20           (0x0014u)  /* Ratio selection P: 20 */
#define RATIOP_21           (0x0015u)  /* Ratio selection P: 21 */
#define RATIOP_22           (0x0016u)  /* Ratio selection P: 22 */
#define RATIOP_23           (0x0017u)  /* Ratio selection P: 23 */
#define RATIOP_24           (0x0018u)  /* Ratio selection P: 24 */
#define RATIOP_25           (0x0019u)  /* Ratio selection P: 25 */
#define RATIOP_26           (0x001Au)  /* Ratio selection P: 26 */
#define RATIOP_27           (0x001Bu)  /* Ratio selection P: 27 */
#define RATIOP_28           (0x001Cu)  /* Ratio selection P: 28 */
#define RATIOP_29           (0x001Du)  /* Ratio selection P: 29 */
#define RATIOP_30           (0x001Eu)  /* Ratio selection P: 30 */
#define RATIOP_31           (0x001Fu)  /* Ratio selection P: 31 */
#define RATIOP_32           (0x0020u)  /* Ratio selection P: 32 */
#define RATION_0           (0x0000u)  /* Ratio selection N: 0 */
#define RATION_1           (0x0100u)  /* Ratio selection N: 1 */
#define RATION_2           (0x0200u)  /* Ratio selection N: 2 */
#define RATION_3           (0x0300u)  /* Ratio selection N: 3 */
#define RATION_4           (0x0400u)  /* Ratio selection N: 4 */
#define RATION_5           (0x0500u)  /* Ratio selection N: 5 */
#define RATION_6           (0x0600u)  /* Ratio selection N: 6 */
#define RATION_7           (0x0700u)  /* Ratio selection N: 7 */

/* CAPPCCx */
#define PCC0                (0x0001u)  /* PCC selection for block Bit 0 */
#define PCC1                (0x0002u)  /* PCC selection for block Bit 1 */
#define PCC2                (0x0004u)  /* PCC selection for block Bit 2 */
#define PCC3                (0x0008u)  /* PCC selection for block Bit 3 */
#define PCC4                (0x0010u)  /* PCC selection for block Bit 4 */
#define PCC5                (0x0020u)  /* PCC selection for block Bit 5 */
#define PCC6                (0x0040u)  /* PCC selection for block Bit 6 */
#define PCC7                (0x0080u)  /* PCC selection for block Bit 7 */
#define PCCSCL0             (0x0100u)  /* PCC scale setting Bit 0 */
#define PCCSCL1             (0x0200u)  /* PCC scale setting Bit 1 */
#define PCCDEC              (0x0400u)  /* Decoder for PCC selection */

#define PCC0_L              (0x0001u)  /* PCC selection for block Bit 0 */
#define PCC1_L              (0x0002u)  /* PCC selection for block Bit 1 */
#define PCC2_L              (0x0004u)  /* PCC selection for block Bit 2 */
#define PCC3_L              (0x0008u)  /* PCC selection for block Bit 3 */
#define PCC4_L              (0x0010u)  /* PCC selection for block Bit 4 */
#define PCC5_L              (0x0020u)  /* PCC selection for block Bit 5 */
#define PCC6_L              (0x0040u)  /* PCC selection for block Bit 6 */
#define PCC7_L              (0x0080u)  /* PCC selection for block Bit 7 */

#define PCCSCL0_H           (0x0001u)  /* PCC scale setting Bit 0 */
#define PCCSCL1_H           (0x0002u)  /* PCC scale setting Bit 1 */
#define PCCDEC_H            (0x0004u)  /* Decoder for PCC selection */

#define PCCSCL_0            (0x0000u)  /* PCC scale setting: 0 */
#define PCCSCL_1            (0x0100u)  /* PCC scale setting: 1 */
#define PCCSCL_2            (0x0200u)  /* PCC scale setting: 2 */
#define PCCSCL_3            (0x0300u)  /* PCC scale setting: 3 */

/* CAPCVDx */
#define CVD0               (0x0001u)  /* 12-bit conversion value Bit: 0 */
#define CVD1               (0x0002u)  /* 12-bit conversion value Bit: 1 */
#define CVD2               (0x0004u)  /* 12-bit conversion value Bit: 2 */
#define CVD3               (0x0008u)  /* 12-bit conversion value Bit: 3 */
#define CVD4               (0x0010u)  /* 12-bit conversion value Bit: 4 */
#define CVD5               (0x0020u)  /* 12-bit conversion value Bit: 5 */
#define CVD6               (0x0040u)  /* 12-bit conversion value Bit: 6 */
#define CVD7               (0x0080u)  /* 12-bit conversion value Bit: 7 */
#define CVD8               (0x0100u)  /* 12-bit conversion value Bit: 8 */
#define CVD9               (0x0200u)  /* 12-bit conversion value Bit: 9 */
#define CVD10               (0x0400u)  /* 12-bit conversion value Bit: 10 */
#define CVD11               (0x0800u)  /* 12-bit conversion value Bit: 11 */
#define CVD12               (0x1000u)  /* 12-bit conversion value Bit: 12 */
#define CVDOVR              (0x2000u)  /* Conversion data has been overwritten */

#define CVD0_L              (0x0001u)  /* 12-bit conversion value Bit: 0 */
#define CVD1_L              (0x0002u)  /* 12-bit conversion value Bit: 1 */
#define CVD2_L              (0x0004u)  /* 12-bit conversion value Bit: 2 */
#define CVD3_L              (0x0008u)  /* 12-bit conversion value Bit: 3 */
#define CVD4_L              (0x0010u)  /* 12-bit conversion value Bit: 4 */
#define CVD5_L              (0x0020u)  /* 12-bit conversion value Bit: 5 */
#define CVD6_L              (0x0040u)  /* 12-bit conversion value Bit: 6 */
#define CVD7_L              (0x0080u)  /* 12-bit conversion value Bit: 7 */

#define CVD8_H              (0x0001u)  /* 12-bit conversion value Bit: 8 */
#define CVD9_H              (0x0002u)  /* 12-bit conversion value Bit: 9 */
#define CVD10_H             (0x0004u)  /* 12-bit conversion value Bit: 10 */
#define CVD11_H             (0x0008u)  /* 12-bit conversion value Bit: 11 */
#define CVD12_H             (0x0010u)  /* 12-bit conversion value Bit: 12 */
#define CVDOVR_H            (0x0020u)  /* Conversion data has been overwritten */

/* CAPFCINTx */
#define FCINT0             (0x0001u)  /* Filtered count integer value Bit: 0 */
#define FCINT1             (0x0002u)  /* Filtered count integer value Bit: 1 */
#define FCINT2             (0x0004u)  /* Filtered count integer value Bit: 2 */
#define FCINT3             (0x0008u)  /* Filtered count integer value Bit: 3 */
#define FCINT4             (0x0010u)  /* Filtered count integer value Bit: 4 */
#define FCINT5             (0x0020u)  /* Filtered count integer value Bit: 5 */
#define FCINT6             (0x0040u)  /* Filtered count integer value Bit: 6 */
#define FCINT7             (0x0080u)  /* Filtered count integer value Bit: 7 */
#define FCINT8             (0x0100u)  /* Filtered count integer value Bit: 8 */
#define FCINT9             (0x0200u)  /* Filtered count integer value Bit: 9 */
#define FCINT10             (0x0400u)  /* Filtered count integer value Bit: 10 */
#define FCINT11             (0x0800u)  /* Filtered count integer value Bit: 11 */
#define FCINT12             (0x1000u)  /* Filtered count integer value Bit: 12 */

#define FCINT0_L            (0x0001u)  /* Filtered count integer value Bit: 0 */
#define FCINT1_L            (0x0002u)  /* Filtered count integer value Bit: 1 */
#define FCINT2_L            (0x0004u)  /* Filtered count integer value Bit: 2 */
#define FCINT3_L            (0x0008u)  /* Filtered count integer value Bit: 3 */
#define FCINT4_L            (0x0010u)  /* Filtered count integer value Bit: 4 */
#define FCINT5_L            (0x0020u)  /* Filtered count integer value Bit: 5 */
#define FCINT6_L            (0x0040u)  /* Filtered count integer value Bit: 6 */
#define FCINT7_L            (0x0080u)  /* Filtered count integer value Bit: 7 */

#define FCINT8_H            (0x0001u)  /* Filtered count integer value Bit: 8 */
#define FCINT9_H            (0x0002u)  /* Filtered count integer value Bit: 9 */
#define FCINT10_H           (0x0004u)  /* Filtered count integer value Bit: 10 */
#define FCINT11_H           (0x0008u)  /* Filtered count integer value Bit: 11 */
#define FCINT12_H           (0x0010u)  /* Filtered count integer value Bit: 12 */

/* CAPFCDECx */
#define FCDEC0              (0x0100u)  /* Filtered count decimal value Bit: 0 */
#define FCDEC1              (0x0200u)  /* Filtered count decimal value Bit: 1 */
#define FCDEC2              (0x0400u)  /* Filtered count decimal value Bit: 2 */
#define FCDEC3              (0x0800u)  /* Filtered count decimal value Bit: 3 */
#define FCDEC4              (0x1000u)  /* Filtered count decimal value Bit: 4 */
#define FCDEC5              (0x2000u)  /* Filtered count decimal value Bit: 5 */
#define FCDEC6              (0x4000u)  /* Filtered count decimal value Bit: 6 */
#define FCDEC7              (0x8000u)  /* Filtered count decimal value Bit: 7 */

#define FCDEC0_H            (0x0001u)  /* Filtered count decimal value Bit: 0 */
#define FCDEC1_H            (0x0002u)  /* Filtered count decimal value Bit: 1 */
#define FCDEC2_H            (0x0004u)  /* Filtered count decimal value Bit: 2 */
#define FCDEC3_H            (0x0008u)  /* Filtered count decimal value Bit: 3 */
#define FCDEC4_H            (0x0010u)  /* Filtered count decimal value Bit: 4 */
#define FCDEC5_H            (0x0020u)  /* Filtered count decimal value Bit: 5 */
#define FCDEC6_H            (0x0040u)  /* Filtered count decimal value Bit: 6 */
#define FCDEC7_H            (0x0080u)  /* Filtered count decimal value Bit: 7 */

/* CAPLTAINTx */
#define LTAINT0            (0x0001u)  /* LTA integer value Bit: 0 */
#define LTAINT1            (0x0002u)  /* LTA integer value Bit: 1 */
#define LTAINT2            (0x0004u)  /* LTA integer value Bit: 2 */
#define LTAINT3            (0x0008u)  /* LTA integer value Bit: 3 */
#define LTAINT4            (0x0010u)  /* LTA integer value Bit: 4 */
#define LTAINT5            (0x0020u)  /* LTA integer value Bit: 5 */
#define LTAINT6            (0x0040u)  /* LTA integer value Bit: 6 */
#define LTAINT7            (0x0080u)  /* LTA integer value Bit: 7 */
#define LTAINT8            (0x0100u)  /* LTA integer value Bit: 8 */
#define LTAINT9            (0x0200u)  /* LTA integer value Bit: 9 */
#define LTAINT10            (0x0400u)  /* LTA integer value Bit: 10 */
#define LTAINT11            (0x0800u)  /* LTA integer value Bit: 11 */
#define LTAINT12            (0x1000u)  /* LTA integer value Bit: 12 */

#define LTAINT0_L           (0x0001u)  /* LTA integer value Bit: 0 */
#define LTAINT1_L           (0x0002u)  /* LTA integer value Bit: 1 */
#define LTAINT2_L           (0x0004u)  /* LTA integer value Bit: 2 */
#define LTAINT3_L           (0x0008u)  /* LTA integer value Bit: 3 */
#define LTAINT4_L           (0x0010u)  /* LTA integer value Bit: 4 */
#define LTAINT5_L           (0x0020u)  /* LTA integer value Bit: 5 */
#define LTAINT6_L           (0x0040u)  /* LTA integer value Bit: 6 */
#define LTAINT7_L           (0x0080u)  /* LTA integer value Bit: 7 */

#define LTAINT8_H           (0x0001u)  /* LTA integer value Bit: 8 */
#define LTAINT9_H           (0x0002u)  /* LTA integer value Bit: 9 */
#define LTAINT10_H          (0x0004u)  /* LTA integer value Bit: 10 */
#define LTAINT11_H          (0x0008u)  /* LTA integer value Bit: 11 */
#define LTAINT12_H          (0x0010u)  /* LTA integer value Bit: 12 */

/* CAPLTADECx */
#define LTADEC0             (0x0100u)  /* LTA decimal value Bit: 0 */
#define LTADEC1             (0x0200u)  /* LTA decimal value Bit: 1 */
#define LTADEC2             (0x0400u)  /* LTA decimal value Bit: 2 */
#define LTADEC3             (0x0800u)  /* LTA decimal value Bit: 3 */
#define LTADEC4             (0x1000u)  /* LTA decimal value Bit: 4 */
#define LTADEC5             (0x2000u)  /* LTA decimal value Bit: 5 */
#define LTADEC6             (0x4000u)  /* LTA decimal value Bit: 6 */
#define LTADEC7             (0x8000u)  /* LTA decimal value Bit: 7 */

#define LTADEC0_H           (0x0001u)  /* LTA decimal value Bit: 0 */
#define LTADEC1_H           (0x0002u)  /* LTA decimal value Bit: 1 */
#define LTADEC2_H           (0x0004u)  /* LTA decimal value Bit: 2 */
#define LTADEC3_H           (0x0008u)  /* LTA decimal value Bit: 3 */
#define LTADEC4_H           (0x0010u)  /* LTA decimal value Bit: 4 */
#define LTADEC5_H           (0x0020u)  /* LTA decimal value Bit: 5 */
#define LTADEC6_H           (0x0040u)  /* LTA decimal value Bit: 6 */
#define LTADEC7_H           (0x0080u)  /* LTA decimal value Bit: 7 */

/* CAPIE */
#define EOCIEN              (0x0001u)  /* End of conversion interrupt enable */
#define CAPDTCTIEN          (0x0002u)  /* Captivate detection interrupt enable */
#define CAPTIEN             (0x0004u)  /* Captivate Timer interrupt enable */
#define CAPCNTRIEN          (0x0008u)  /* Captivate Conversion Counter interrupt enable */
#define CAPMAXIEN           (0x0100u)  /* Captivate maximum count interrupt enable */

#define EOCIEN_L            (0x0001u)  /* End of conversion interrupt enable */
#define CAPDTCTIEN_L        (0x0002u)  /* Captivate detection interrupt enable */
#define CAPTIEN_L           (0x0004u)  /* Captivate Timer interrupt enable */
#define CAPCNTRIEN_L        (0x0008u)  /* Captivate Conversion Counter interrupt enable */

#define CAPMAXIEN_H         (0x0001u)  /* Captivate maximum count interrupt enable */

/* CAPIFG */
#define EOCIFG              (0x0001u)  /* End of conversion interrupt flag */
#define CAPDTCTIFG          (0x0002u)  /* Captivate detection interrupt flag */
#define CAPTIFG             (0x0004u)  /* Captivate Timer interrupt flag */
#define CAPCNTRIFG          (0x0008u)  /* Captivate Conversion Counter interrupt flag */
#define CAPMAXIFG           (0x0100u)  /* Captivate maximum count interrupt flag */

#define EOCIFG_L            (0x0001u)  /* End of conversion interrupt flag */
#define CAPDTCTIFG_L        (0x0002u)  /* Captivate detection interrupt flag */
#define CAPTIFG_L           (0x0004u)  /* Captivate Timer interrupt flag */
#define CAPCNTRIFG_L        (0x0008u)  /* Captivate Conversion Counter interrupt flag */

#define CAPMAXIFG_H         (0x0001u)  /* Captivate maximum count interrupt flag */

/* CAPIV Definitions */
#define CAPIV_NONE          (0x0000u)    /* No Interrupt pending */
#define CAPIV_EOCIFG        (0x0002u)    /* End of conversion interrupt, Flag = EOCIFG */
#define CAPIV_CAPDTCTIFG    (0x0004u)    /* Detection interrupt, Flag = CAPDTCTIFG */
#define CAPIV_CAPTIFG       (0x0006u)    /* Captivate Timer interrupt, Flag = CAPTIFG */
#define CAPIV_CAPCNTRIFG    (0x0008u)    /* Captivate Counter interrupt, Flag = CAPCNTRIFG */
#define CAPIV_CAPMAXIFG     (0x000Au)    /* max count value reached, Flag = CAPMAXIFG */


#endif
/************************************************************
* Capacitive_Touch_IO 0
************************************************************/
#ifdef  __MSP430_HAS_CAP_TOUCH_IO_0__ /* Definition to show that Module is available */

#define OFS_CAPTIO0CTL        (0x000Eu)  /* Capacitive_Touch_IO 0 control register */
#define OFS_CAPTIO0CTL_L       OFS_CAPTIO0CTL
#define OFS_CAPTIO0CTL_H       OFS_CAPTIO0CTL+1

#define  CAPSIO0CTL         CAPTIO0CTL  /* legacy define */

/* CAPTIOxCTL Control Bits */
#define CAPTIOPISEL0         (0x0002u)    /* CapTouchIO Pin Select Bit: 0 */
#define CAPTIOPISEL1         (0x0004u)    /* CapTouchIO Pin Select Bit: 1 */
#define CAPTIOPISEL2         (0x0008u)    /* CapTouchIO Pin Select Bit: 2 */
#define CAPTIOPOSEL0         (0x0010u)    /* CapTouchIO Port Select Bit: 0 */
#define CAPTIOPOSEL1         (0x0020u)    /* CapTouchIO Port Select Bit: 1 */
#define CAPTIOPOSEL2         (0x0040u)    /* CapTouchIO Port Select Bit: 2 */
#define CAPTIOPOSEL3         (0x0080u)    /* CapTouchIO Port Select Bit: 3 */
#define CAPTIOEN             (0x0100u)    /* CapTouchIO Enable */
#define CAPTIO               (0x0200u)    /* CapTouchIO state */

/* CAPTIOxCTL Control Bits */
#define CAPTIOPISEL0_L      (0x0002u)    /* CapTouchIO Pin Select Bit: 0 */
#define CAPTIOPISEL1_L      (0x0004u)    /* CapTouchIO Pin Select Bit: 1 */
#define CAPTIOPISEL2_L      (0x0008u)    /* CapTouchIO Pin Select Bit: 2 */
#define CAPTIOPOSEL0_L      (0x0010u)    /* CapTouchIO Port Select Bit: 0 */
#define CAPTIOPOSEL1_L      (0x0020u)    /* CapTouchIO Port Select Bit: 1 */
#define CAPTIOPOSEL2_L      (0x0040u)    /* CapTouchIO Port Select Bit: 2 */
#define CAPTIOPOSEL3_L      (0x0080u)    /* CapTouchIO Port Select Bit: 3 */

/* CAPTIOxCTL Control Bits */
#define CAPTIOEN_H          (0x0001u)    /* CapTouchIO Enable */
#define CAPTIO_H            (0x0002u)    /* CapTouchIO state */

/* Legacy defines */
#define CAPSIOPISEL0         (0x0002u)    /* CapTouchIO Pin Select Bit: 0 */
#define CAPSIOPISEL1         (0x0004u)    /* CapTouchIO Pin Select Bit: 1 */
#define CAPSIOPISEL2         (0x0008u)    /* CapTouchIO Pin Select Bit: 2 */
#define CAPSIOPOSEL0         (0x0010u)    /* CapTouchIO Port Select Bit: 0 */
#define CAPSIOPOSEL1         (0x0020u)    /* CapTouchIO Port Select Bit: 1 */
#define CAPSIOPOSEL2         (0x0040u)    /* CapTouchIO Port Select Bit: 2 */
#define CAPSIOPOSEL3         (0x0080u)    /* CapTouchIO Port Select Bit: 3 */
#define CAPSIOEN             (0x0100u)    /* CapTouchIO Enable */
#define CAPSIO               (0x0200u)    /* CapTouchIO state */

#endif
/************************************************************
* Capacitive_Touch_IO 1
************************************************************/
#ifdef  __MSP430_HAS_CAP_TOUCH_IO_1__ /* Definition to show that Module is available */

#define OFS_CAPTIO1CTL        (0x000Eu)  /* Capacitive_Touch_IO 1 control register */
#define OFS_CAPTIO1CTL_L       OFS_CAPTIO1CTL
#define OFS_CAPTIO1CTL_H       OFS_CAPTIO1CTL+1

#define  CAPSIO1CTL         CAPTIO1CTL  /* legacy define */

#endif
/************************************************************
* Comparator E
************************************************************/
#ifdef  __MSP430_HAS_COMP_E__          /* Definition to show that Module is available */

#define OFS_CECTL0            (0x0000u)  /* Comparator E Control Register 0 */
#define OFS_CECTL0_L           OFS_CECTL0
#define OFS_CECTL0_H           OFS_CECTL0+1
#define OFS_CECTL1            (0x0002u)  /* Comparator E Control Register 1 */
#define OFS_CECTL1_L           OFS_CECTL1
#define OFS_CECTL1_H           OFS_CECTL1+1
#define OFS_CECTL2            (0x0004u)  /* Comparator E Control Register 2 */
#define OFS_CECTL2_L           OFS_CECTL2
#define OFS_CECTL2_H           OFS_CECTL2+1
#define OFS_CECTL3            (0x0006u)  /* Comparator E Control Register 3 */
#define OFS_CECTL3_L           OFS_CECTL3
#define OFS_CECTL3_H           OFS_CECTL3+1
#define OFS_CEINT             (0x000Cu)  /* Comparator E Interrupt Register */
#define OFS_CEINT_L            OFS_CEINT
#define OFS_CEINT_H            OFS_CEINT+1
#define OFS_CEIV              (0x000Eu)  /* Comparator E Interrupt Vector Word */
#define OFS_CEIV_L             OFS_CEIV
#define OFS_CEIV_H             OFS_CEIV+1

/* CECTL0 Control Bits */
#define CEIPSEL0            (0x0001u)  /* Comp. E Pos. Channel Input Select 0 */
#define CEIPSEL1            (0x0002u)  /* Comp. E Pos. Channel Input Select 1 */
#define CEIPSEL2            (0x0004u)  /* Comp. E Pos. Channel Input Select 2 */
#define CEIPSEL3            (0x0008u)  /* Comp. E Pos. Channel Input Select 3 */
//#define RESERVED            (0x0010u)  /* Comp. E */
//#define RESERVED            (0x0020u)  /* Comp. E */
//#define RESERVED            (0x0040u)  /* Comp. E */
#define CEIPEN              (0x0080u)  /* Comp. E Pos. Channel Input Enable */
#define CEIMSEL0            (0x0100u)  /* Comp. E Neg. Channel Input Select 0 */
#define CEIMSEL1            (0x0200u)  /* Comp. E Neg. Channel Input Select 1 */
#define CEIMSEL2            (0x0400u)  /* Comp. E Neg. Channel Input Select 2 */
#define CEIMSEL3            (0x0800u)  /* Comp. E Neg. Channel Input Select 3 */
//#define RESERVED            (0x1000u)  /* Comp. E */
//#define RESERVED            (0x2000u)  /* Comp. E */
//#define RESERVED            (0x4000u)  /* Comp. E */
#define CEIMEN              (0x8000u)  /* Comp. E Neg. Channel Input Enable */

/* CECTL0 Control Bits */
#define CEIPSEL0_L          (0x0001u)  /* Comp. E Pos. Channel Input Select 0 */
#define CEIPSEL1_L          (0x0002u)  /* Comp. E Pos. Channel Input Select 1 */
#define CEIPSEL2_L          (0x0004u)  /* Comp. E Pos. Channel Input Select 2 */
#define CEIPSEL3_L          (0x0008u)  /* Comp. E Pos. Channel Input Select 3 */
//#define RESERVED            (0x0010u)  /* Comp. E */
//#define RESERVED            (0x0020u)  /* Comp. E */
//#define RESERVED            (0x0040u)  /* Comp. E */
#define CEIPEN_L            (0x0080u)  /* Comp. E Pos. Channel Input Enable */
//#define RESERVED            (0x1000u)  /* Comp. E */
//#define RESERVED            (0x2000u)  /* Comp. E */
//#define RESERVED            (0x4000u)  /* Comp. E */

/* CECTL0 Control Bits */
//#define RESERVED            (0x0010u)  /* Comp. E */
//#define RESERVED            (0x0020u)  /* Comp. E */
//#define RESERVED            (0x0040u)  /* Comp. E */
#define CEIMSEL0_H          (0x0001u)  /* Comp. E Neg. Channel Input Select 0 */
#define CEIMSEL1_H          (0x0002u)  /* Comp. E Neg. Channel Input Select 1 */
#define CEIMSEL2_H          (0x0004u)  /* Comp. E Neg. Channel Input Select 2 */
#define CEIMSEL3_H          (0x0008u)  /* Comp. E Neg. Channel Input Select 3 */
//#define RESERVED            (0x1000u)  /* Comp. E */
//#define RESERVED            (0x2000u)  /* Comp. E */
//#define RESERVED            (0x4000u)  /* Comp. E */
#define CEIMEN_H            (0x0080u)  /* Comp. E Neg. Channel Input Enable */

#define CEIPSEL_0           (0x0000u)  /* Comp. E V+ terminal Input Select: Channel 0 */
#define CEIPSEL_1           (0x0001u)  /* Comp. E V+ terminal Input Select: Channel 1 */
#define CEIPSEL_2           (0x0002u)  /* Comp. E V+ terminal Input Select: Channel 2 */
#define CEIPSEL_3           (0x0003u)  /* Comp. E V+ terminal Input Select: Channel 3 */
#define CEIPSEL_4           (0x0004u)  /* Comp. E V+ terminal Input Select: Channel 4 */
#define CEIPSEL_5           (0x0005u)  /* Comp. E V+ terminal Input Select: Channel 5 */
#define CEIPSEL_6           (0x0006u)  /* Comp. E V+ terminal Input Select: Channel 6 */
#define CEIPSEL_7           (0x0007u)  /* Comp. E V+ terminal Input Select: Channel 7 */
#define CEIPSEL_8           (0x0008u)  /* Comp. E V+ terminal Input Select: Channel 8 */
#define CEIPSEL_9           (0x0009u)  /* Comp. E V+ terminal Input Select: Channel 9 */
#define CEIPSEL_10          (0x000Au)  /* Comp. E V+ terminal Input Select: Channel 10 */
#define CEIPSEL_11          (0x000Bu)  /* Comp. E V+ terminal Input Select: Channel 11 */
#define CEIPSEL_12          (0x000Cu)  /* Comp. E V+ terminal Input Select: Channel 12 */
#define CEIPSEL_13          (0x000Du)  /* Comp. E V+ terminal Input Select: Channel 13 */
#define CEIPSEL_14          (0x000Eu)  /* Comp. E V+ terminal Input Select: Channel 14 */
#define CEIPSEL_15          (0x000Fu)  /* Comp. E V+ terminal Input Select: Channel 15 */

#define CEIMSEL_0           (0x0000u)  /* Comp. E V- Terminal Input Select: Channel 0 */
#define CEIMSEL_1           (0x0100u)  /* Comp. E V- Terminal Input Select: Channel 1 */
#define CEIMSEL_2           (0x0200u)  /* Comp. E V- Terminal Input Select: Channel 2 */
#define CEIMSEL_3           (0x0300u)  /* Comp. E V- Terminal Input Select: Channel 3 */
#define CEIMSEL_4           (0x0400u)  /* Comp. E V- Terminal Input Select: Channel 4 */
#define CEIMSEL_5           (0x0500u)  /* Comp. E V- Terminal Input Select: Channel 5 */
#define CEIMSEL_6           (0x0600u)  /* Comp. E V- Terminal Input Select: Channel 6 */
#define CEIMSEL_7           (0x0700u)  /* Comp. E V- Terminal Input Select: Channel 7 */
#define CEIMSEL_8           (0x0800u)  /* Comp. E V- terminal Input Select: Channel 8 */
#define CEIMSEL_9           (0x0900u)  /* Comp. E V- terminal Input Select: Channel 9 */
#define CEIMSEL_10          (0x0A00u)  /* Comp. E V- terminal Input Select: Channel 10 */
#define CEIMSEL_11          (0x0B00u)  /* Comp. E V- terminal Input Select: Channel 11 */
#define CEIMSEL_12          (0x0C00u)  /* Comp. E V- terminal Input Select: Channel 12 */
#define CEIMSEL_13          (0x0D00u)  /* Comp. E V- terminal Input Select: Channel 13 */
#define CEIMSEL_14          (0x0E00u)  /* Comp. E V- terminal Input Select: Channel 14 */
#define CEIMSEL_15          (0x0F00u)  /* Comp. E V- terminal Input Select: Channel 15 */

/* CECTL1 Control Bits */
#define CEOUT               (0x0001u)  /* Comp. E Output */
#define CEOUTPOL            (0x0002u)  /* Comp. E Output Polarity */
#define CEF                 (0x0004u)  /* Comp. E Enable Output Filter */
#define CEIES               (0x0008u)  /* Comp. E Interrupt Edge Select */
#define CESHORT             (0x0010u)  /* Comp. E Input Short */
#define CEEX                (0x0020u)  /* Comp. E Exchange Inputs */
#define CEFDLY0             (0x0040u)  /* Comp. E Filter delay Bit 0 */
#define CEFDLY1             (0x0080u)  /* Comp. E Filter delay Bit 1 */
#define CEPWRMD0            (0x0100u)  /* Comp. E Power mode Bit 0 */
#define CEPWRMD1            (0x0200u)  /* Comp. E Power mode Bit 1 */
#define CEON                (0x0400u)  /* Comp. E enable */
#define CEMRVL              (0x0800u)  /* Comp. E CEMRV Level */
#define CEMRVS              (0x1000u)  /* Comp. E Output selects between VREF0 or VREF1*/
//#define RESERVED            (0x2000u)  /* Comp. E */
//#define RESERVED            (0x4000u)  /* Comp. E */
//#define RESERVED            (0x8000u)  /* Comp. E */

/* CECTL1 Control Bits */
#define CEOUT_L             (0x0001u)  /* Comp. E Output */
#define CEOUTPOL_L          (0x0002u)  /* Comp. E Output Polarity */
#define CEF_L               (0x0004u)  /* Comp. E Enable Output Filter */
#define CEIES_L             (0x0008u)  /* Comp. E Interrupt Edge Select */
#define CESHORT_L           (0x0010u)  /* Comp. E Input Short */
#define CEEX_L              (0x0020u)  /* Comp. E Exchange Inputs */
#define CEFDLY0_L           (0x0040u)  /* Comp. E Filter delay Bit 0 */
#define CEFDLY1_L           (0x0080u)  /* Comp. E Filter delay Bit 1 */
//#define RESERVED            (0x2000u)  /* Comp. E */
//#define RESERVED            (0x4000u)  /* Comp. E */
//#define RESERVED            (0x8000u)  /* Comp. E */

/* CECTL1 Control Bits */
#define CEPWRMD0_H          (0x0001u)  /* Comp. E Power mode Bit 0 */
#define CEPWRMD1_H          (0x0002u)  /* Comp. E Power mode Bit 1 */
#define CEON_H              (0x0004u)  /* Comp. E enable */
#define CEMRVL_H            (0x0008u)  /* Comp. E CEMRV Level */
#define CEMRVS_H            (0x0010u)  /* Comp. E Output selects between VREF0 or VREF1*/
//#define RESERVED            (0x2000u)  /* Comp. E */
//#define RESERVED            (0x4000u)  /* Comp. E */
//#define RESERVED            (0x8000u)  /* Comp. E */

#define CEPWRMD_0          (0x0000u)  /* Comp. E Power mode 0 */
#define CEPWRMD_1          (0x0100u)  /* Comp. E Power mode 1 */
#define CEPWRMD_2          (0x0200u)  /* Comp. E Power mode 2 */
#define CEPWRMD_3          (0x0300u)  /* Comp. E Power mode 3*/

#define CEFDLY_0           (0x0000u)  /* Comp. E Filter delay 0 : 450ns */
#define CEFDLY_1           (0x0040u)  /* Comp. E Filter delay 1 : 900ns */
#define CEFDLY_2           (0x0080u)  /* Comp. E Filter delay 2 : 1800ns */
#define CEFDLY_3           (0x00C0u)  /* Comp. E Filter delay 3 : 3600ns */

/* CECTL2 Control Bits */
#define CEREF00             (0x0001u)  /* Comp. E Reference 0 Resistor Select Bit : 0 */
#define CEREF01             (0x0002u)  /* Comp. E Reference 0 Resistor Select Bit : 1 */
#define CEREF02             (0x0004u)  /* Comp. E Reference 0 Resistor Select Bit : 2 */
#define CEREF03             (0x0008u)  /* Comp. E Reference 0 Resistor Select Bit : 3 */
#define CEREF04             (0x0010u)  /* Comp. E Reference 0 Resistor Select Bit : 4 */
#define CERSEL              (0x0020u)  /* Comp. E Reference select */
#define CERS0               (0x0040u)  /* Comp. E Reference Source Bit : 0 */
#define CERS1               (0x0080u)  /* Comp. E Reference Source Bit : 1 */
#define CEREF10             (0x0100u)  /* Comp. E Reference 1 Resistor Select Bit : 0 */
#define CEREF11             (0x0200u)  /* Comp. E Reference 1 Resistor Select Bit : 1 */
#define CEREF12             (0x0400u)  /* Comp. E Reference 1 Resistor Select Bit : 2 */
#define CEREF13             (0x0800u)  /* Comp. E Reference 1 Resistor Select Bit : 3 */
#define CEREF14             (0x1000u)  /* Comp. E Reference 1 Resistor Select Bit : 4 */
#define CEREFL0             (0x2000u)  /* Comp. E Reference voltage level Bit : 0 */
#define CEREFL1             (0x4000u)  /* Comp. E Reference voltage level Bit : 1 */
#define CEREFACC            (0x8000u)  /* Comp. E Reference Accuracy */

/* CECTL2 Control Bits */
#define CEREF00_L           (0x0001u)  /* Comp. E Reference 0 Resistor Select Bit : 0 */
#define CEREF01_L           (0x0002u)  /* Comp. E Reference 0 Resistor Select Bit : 1 */
#define CEREF02_L           (0x0004u)  /* Comp. E Reference 0 Resistor Select Bit : 2 */
#define CEREF03_L           (0x0008u)  /* Comp. E Reference 0 Resistor Select Bit : 3 */
#define CEREF04_L           (0x0010u)  /* Comp. E Reference 0 Resistor Select Bit : 4 */
#define CERSEL_L            (0x0020u)  /* Comp. E Reference select */
#define CERS0_L             (0x0040u)  /* Comp. E Reference Source Bit : 0 */
#define CERS1_L             (0x0080u)  /* Comp. E Reference Source Bit : 1 */

/* CECTL2 Control Bits */
#define CEREF10_H           (0x0001u)  /* Comp. E Reference 1 Resistor Select Bit : 0 */
#define CEREF11_H           (0x0002u)  /* Comp. E Reference 1 Resistor Select Bit : 1 */
#define CEREF12_H           (0x0004u)  /* Comp. E Reference 1 Resistor Select Bit : 2 */
#define CEREF13_H           (0x0008u)  /* Comp. E Reference 1 Resistor Select Bit : 3 */
#define CEREF14_H           (0x0010u)  /* Comp. E Reference 1 Resistor Select Bit : 4 */
#define CEREFL0_H           (0x0020u)  /* Comp. E Reference voltage level Bit : 0 */
#define CEREFL1_H           (0x0040u)  /* Comp. E Reference voltage level Bit : 1 */
#define CEREFACC_H          (0x0080u)  /* Comp. E Reference Accuracy */

#define CEREF0_0            (0x0000u)  /* Comp. E Int. Ref.0 Select 0 : 1/32 */
#define CEREF0_1            (0x0001u)  /* Comp. E Int. Ref.0 Select 1 : 2/32 */
#define CEREF0_2            (0x0002u)  /* Comp. E Int. Ref.0 Select 2 : 3/32 */
#define CEREF0_3            (0x0003u)  /* Comp. E Int. Ref.0 Select 3 : 4/32 */
#define CEREF0_4            (0x0004u)  /* Comp. E Int. Ref.0 Select 4 : 5/32 */
#define CEREF0_5            (0x0005u)  /* Comp. E Int. Ref.0 Select 5 : 6/32 */
#define CEREF0_6            (0x0006u)  /* Comp. E Int. Ref.0 Select 6 : 7/32 */
#define CEREF0_7            (0x0007u)  /* Comp. E Int. Ref.0 Select 7 : 8/32 */
#define CEREF0_8            (0x0008u)  /* Comp. E Int. Ref.0 Select 0 : 9/32 */
#define CEREF0_9            (0x0009u)  /* Comp. E Int. Ref.0 Select 1 : 10/32 */
#define CEREF0_10           (0x000Au)  /* Comp. E Int. Ref.0 Select 2 : 11/32 */
#define CEREF0_11           (0x000Bu)  /* Comp. E Int. Ref.0 Select 3 : 12/32 */
#define CEREF0_12           (0x000Cu)  /* Comp. E Int. Ref.0 Select 4 : 13/32 */
#define CEREF0_13           (0x000Du)  /* Comp. E Int. Ref.0 Select 5 : 14/32 */
#define CEREF0_14           (0x000Eu)  /* Comp. E Int. Ref.0 Select 6 : 15/32 */
#define CEREF0_15           (0x000Fu)  /* Comp. E Int. Ref.0 Select 7 : 16/32 */
#define CEREF0_16           (0x0010u)  /* Comp. E Int. Ref.0 Select 0 : 17/32 */
#define CEREF0_17           (0x0011u)  /* Comp. E Int. Ref.0 Select 1 : 18/32 */
#define CEREF0_18           (0x0012u)  /* Comp. E Int. Ref.0 Select 2 : 19/32 */
#define CEREF0_19           (0x0013u)  /* Comp. E Int. Ref.0 Select 3 : 20/32 */
#define CEREF0_20           (0x0014u)  /* Comp. E Int. Ref.0 Select 4 : 21/32 */
#define CEREF0_21           (0x0015u)  /* Comp. E Int. Ref.0 Select 5 : 22/32 */
#define CEREF0_22           (0x0016u)  /* Comp. E Int. Ref.0 Select 6 : 23/32 */
#define CEREF0_23           (0x0017u)  /* Comp. E Int. Ref.0 Select 7 : 24/32 */
#define CEREF0_24           (0x0018u)  /* Comp. E Int. Ref.0 Select 0 : 25/32 */
#define CEREF0_25           (0x0019u)  /* Comp. E Int. Ref.0 Select 1 : 26/32 */
#define CEREF0_26           (0x001Au)  /* Comp. E Int. Ref.0 Select 2 : 27/32 */
#define CEREF0_27           (0x001Bu)  /* Comp. E Int. Ref.0 Select 3 : 28/32 */
#define CEREF0_28           (0x001Cu)  /* Comp. E Int. Ref.0 Select 4 : 29/32 */
#define CEREF0_29           (0x001Du)  /* Comp. E Int. Ref.0 Select 5 : 30/32 */
#define CEREF0_30           (0x001Eu)  /* Comp. E Int. Ref.0 Select 6 : 31/32 */
#define CEREF0_31           (0x001Fu)  /* Comp. E Int. Ref.0 Select 7 : 32/32 */

#define CERS_0              (0x0000u)  /* Comp. E Reference Source 0 : Off */
#define CERS_1              (0x0040u)  /* Comp. E Reference Source 1 : Vcc */
#define CERS_2              (0x0080u)  /* Comp. E Reference Source 2 : Shared Ref. */
#define CERS_3              (0x00C0u)  /* Comp. E Reference Source 3 : Shared Ref. / Off */

#define CEREF1_0            (0x0000u)  /* Comp. E Int. Ref.1 Select 0 : 1/32 */
#define CEREF1_1            (0x0100u)  /* Comp. E Int. Ref.1 Select 1 : 2/32 */
#define CEREF1_2            (0x0200u)  /* Comp. E Int. Ref.1 Select 2 : 3/32 */
#define CEREF1_3            (0x0300u)  /* Comp. E Int. Ref.1 Select 3 : 4/32 */
#define CEREF1_4            (0x0400u)  /* Comp. E Int. Ref.1 Select 4 : 5/32 */
#define CEREF1_5            (0x0500u)  /* Comp. E Int. Ref.1 Select 5 : 6/32 */
#define CEREF1_6            (0x0600u)  /* Comp. E Int. Ref.1 Select 6 : 7/32 */
#define CEREF1_7            (0x0700u)  /* Comp. E Int. Ref.1 Select 7 : 8/32 */
#define CEREF1_8            (0x0800u)  /* Comp. E Int. Ref.1 Select 0 : 9/32 */
#define CEREF1_9            (0x0900u)  /* Comp. E Int. Ref.1 Select 1 : 10/32 */
#define CEREF1_10           (0x0A00u)  /* Comp. E Int. Ref.1 Select 2 : 11/32 */
#define CEREF1_11           (0x0B00u)  /* Comp. E Int. Ref.1 Select 3 : 12/32 */
#define CEREF1_12           (0x0C00u)  /* Comp. E Int. Ref.1 Select 4 : 13/32 */
#define CEREF1_13           (0x0D00u)  /* Comp. E Int. Ref.1 Select 5 : 14/32 */
#define CEREF1_14           (0x0E00u)  /* Comp. E Int. Ref.1 Select 6 : 15/32 */
#define CEREF1_15           (0x0F00u)  /* Comp. E Int. Ref.1 Select 7 : 16/32 */
#define CEREF1_16           (0x1000u)  /* Comp. E Int. Ref.1 Select 0 : 17/32 */
#define CEREF1_17           (0x1100u)  /* Comp. E Int. Ref.1 Select 1 : 18/32 */
#define CEREF1_18           (0x1200u)  /* Comp. E Int. Ref.1 Select 2 : 19/32 */
#define CEREF1_19           (0x1300u)  /* Comp. E Int. Ref.1 Select 3 : 20/32 */
#define CEREF1_20           (0x1400u)  /* Comp. E Int. Ref.1 Select 4 : 21/32 */
#define CEREF1_21           (0x1500u)  /* Comp. E Int. Ref.1 Select 5 : 22/32 */
#define CEREF1_22           (0x1600u)  /* Comp. E Int. Ref.1 Select 6 : 23/32 */
#define CEREF1_23           (0x1700u)  /* Comp. E Int. Ref.1 Select 7 : 24/32 */
#define CEREF1_24           (0x1800u)  /* Comp. E Int. Ref.1 Select 0 : 25/32 */
#define CEREF1_25           (0x1900u)  /* Comp. E Int. Ref.1 Select 1 : 26/32 */
#define CEREF1_26           (0x1A00u)  /* Comp. E Int. Ref.1 Select 2 : 27/32 */
#define CEREF1_27           (0x1B00u)  /* Comp. E Int. Ref.1 Select 3 : 28/32 */
#define CEREF1_28           (0x1C00u)  /* Comp. E Int. Ref.1 Select 4 : 29/32 */
#define CEREF1_29           (0x1D00u)  /* Comp. E Int. Ref.1 Select 5 : 30/32 */
#define CEREF1_30           (0x1E00u)  /* Comp. E Int. Ref.1 Select 6 : 31/32 */
#define CEREF1_31           (0x1F00u)  /* Comp. E Int. Ref.1 Select 7 : 32/32 */

#define CEREFL_0            (0x0000u)  /* Comp. E Reference voltage level 0 : None */
#define CEREFL_1            (0x2000u)  /* Comp. E Reference voltage level 1 : 1.2V */
#define CEREFL_2            (0x4000u)  /* Comp. E Reference voltage level 2 : 2.0V  */
#define CEREFL_3            (0x6000u)  /* Comp. E Reference voltage level 3 : 2.5V  */

#define CEPD0               (0x0001u)  /* Comp. E Disable Input Buffer of Port Register .0 */
#define CEPD1               (0x0002u)  /* Comp. E Disable Input Buffer of Port Register .1 */
#define CEPD2               (0x0004u)  /* Comp. E Disable Input Buffer of Port Register .2 */
#define CEPD3               (0x0008u)  /* Comp. E Disable Input Buffer of Port Register .3 */
#define CEPD4               (0x0010u)  /* Comp. E Disable Input Buffer of Port Register .4 */
#define CEPD5               (0x0020u)  /* Comp. E Disable Input Buffer of Port Register .5 */
#define CEPD6               (0x0040u)  /* Comp. E Disable Input Buffer of Port Register .6 */
#define CEPD7               (0x0080u)  /* Comp. E Disable Input Buffer of Port Register .7 */
#define CEPD8               (0x0100u)  /* Comp. E Disable Input Buffer of Port Register .8 */
#define CEPD9               (0x0200u)  /* Comp. E Disable Input Buffer of Port Register .9 */
#define CEPD10              (0x0400u)  /* Comp. E Disable Input Buffer of Port Register .10 */
#define CEPD11              (0x0800u)  /* Comp. E Disable Input Buffer of Port Register .11 */
#define CEPD12              (0x1000u)  /* Comp. E Disable Input Buffer of Port Register .12 */
#define CEPD13              (0x2000u)  /* Comp. E Disable Input Buffer of Port Register .13 */
#define CEPD14              (0x4000u)  /* Comp. E Disable Input Buffer of Port Register .14 */
#define CEPD15              (0x8000u)  /* Comp. E Disable Input Buffer of Port Register .15 */

#define CEPD0_L             (0x0001u)  /* Comp. E Disable Input Buffer of Port Register .0 */
#define CEPD1_L             (0x0002u)  /* Comp. E Disable Input Buffer of Port Register .1 */
#define CEPD2_L             (0x0004u)  /* Comp. E Disable Input Buffer of Port Register .2 */
#define CEPD3_L             (0x0008u)  /* Comp. E Disable Input Buffer of Port Register .3 */
#define CEPD4_L             (0x0010u)  /* Comp. E Disable Input Buffer of Port Register .4 */
#define CEPD5_L             (0x0020u)  /* Comp. E Disable Input Buffer of Port Register .5 */
#define CEPD6_L             (0x0040u)  /* Comp. E Disable Input Buffer of Port Register .6 */
#define CEPD7_L             (0x0080u)  /* Comp. E Disable Input Buffer of Port Register .7 */

#define CEPD8_H             (0x0001u)  /* Comp. E Disable Input Buffer of Port Register .8 */
#define CEPD9_H             (0x0002u)  /* Comp. E Disable Input Buffer of Port Register .9 */
#define CEPD10_H            (0x0004u)  /* Comp. E Disable Input Buffer of Port Register .10 */
#define CEPD11_H            (0x0008u)  /* Comp. E Disable Input Buffer of Port Register .11 */
#define CEPD12_H            (0x0010u)  /* Comp. E Disable Input Buffer of Port Register .12 */
#define CEPD13_H            (0x0020u)  /* Comp. E Disable Input Buffer of Port Register .13 */
#define CEPD14_H            (0x0040u)  /* Comp. E Disable Input Buffer of Port Register .14 */
#define CEPD15_H            (0x0080u)  /* Comp. E Disable Input Buffer of Port Register .15 */

/* CEINT Control Bits */
#define CEIFG                (0x0001u)  /* Comp. E Interrupt Flag */
#define CEIIFG               (0x0002u)  /* Comp. E Interrupt Flag Inverted Polarity */
//#define RESERVED             (0x0004u)  /* Comp. E */
//#define RESERVED             (0x0008u)  /* Comp. E */
#define CERDYIFG             (0x0010u)  /* Comp. E Comparator_E ready interrupt flag */
//#define RESERVED             (0x0020u)  /* Comp. E */
//#define RESERVED             (0x0040u)  /* Comp. E */
//#define RESERVED             (0x0080u)  /* Comp. E */
#define CEIE                 (0x0100u)  /* Comp. E Interrupt Enable */
#define CEIIE                (0x0200u)  /* Comp. E Interrupt Enable Inverted Polarity */
//#define RESERVED             (0x0400u)  /* Comp. E */
//#define RESERVED             (0x0800u)  /* Comp. E */
#define CERDYIE              (0x1000u)  /* Comp. E Comparator_E ready interrupt enable */
//#define RESERVED             (0x2000u)  /* Comp. E */
//#define RESERVED             (0x4000u)  /* Comp. E */
//#define RESERVED             (0x8000u)  /* Comp. E */

/* CEINT Control Bits */
#define CEIFG_L             (0x0001u)  /* Comp. E Interrupt Flag */
#define CEIIFG_L            (0x0002u)  /* Comp. E Interrupt Flag Inverted Polarity */
//#define RESERVED             (0x0004u)  /* Comp. E */
//#define RESERVED             (0x0008u)  /* Comp. E */
#define CERDYIFG_L          (0x0010u)  /* Comp. E Comparator_E ready interrupt flag */
//#define RESERVED             (0x0020u)  /* Comp. E */
//#define RESERVED             (0x0040u)  /* Comp. E */
//#define RESERVED             (0x0080u)  /* Comp. E */
//#define RESERVED             (0x0400u)  /* Comp. E */
//#define RESERVED             (0x0800u)  /* Comp. E */
//#define RESERVED             (0x2000u)  /* Comp. E */
//#define RESERVED             (0x4000u)  /* Comp. E */
//#define RESERVED             (0x8000u)  /* Comp. E */

/* CEINT Control Bits */
//#define RESERVED             (0x0004u)  /* Comp. E */
//#define RESERVED             (0x0008u)  /* Comp. E */
//#define RESERVED             (0x0020u)  /* Comp. E */
//#define RESERVED             (0x0040u)  /* Comp. E */
//#define RESERVED             (0x0080u)  /* Comp. E */
#define CEIE_H              (0x0001u)  /* Comp. E Interrupt Enable */
#define CEIIE_H             (0x0002u)  /* Comp. E Interrupt Enable Inverted Polarity */
//#define RESERVED             (0x0400u)  /* Comp. E */
//#define RESERVED             (0x0800u)  /* Comp. E */
#define CERDYIE_H           (0x0010u)  /* Comp. E Comparator_E ready interrupt enable */
//#define RESERVED             (0x2000u)  /* Comp. E */
//#define RESERVED             (0x4000u)  /* Comp. E */
//#define RESERVED             (0x8000u)  /* Comp. E */

/* CEIV Definitions */
#define CEIV_NONE           (0x0000u)    /* No Interrupt pending */
#define CEIV_CEIFG          (0x0002u)    /* CEIFG */
#define CEIV_CEIIFG         (0x0004u)    /* CEIIFG */
#define CEIV_CERDYIFG       (0x000Au)    /* CERDYIFG */

#endif
/*************************************************************
* CRC Module
*************************************************************/
#ifdef  __MSP430_HAS_CRC__            /* Definition to show that Module is available */

#define OFS_CRCDI             (0x0000u)  /* CRC Data In Register */
#define OFS_CRCDI_L            OFS_CRCDI
#define OFS_CRCDI_H            OFS_CRCDI+1
#define OFS_CRCDIRB           (0x0002u)  /* CRC data in reverse byte Register */
#define OFS_CRCDIRB_L          OFS_CRCDIRB
#define OFS_CRCDIRB_H          OFS_CRCDIRB+1
#define OFS_CRCINIRES         (0x0004u)  /* CRC Initialisation Register and Result Register */
#define OFS_CRCINIRES_L        OFS_CRCINIRES
#define OFS_CRCINIRES_H        OFS_CRCINIRES+1
#define OFS_CRCRESR           (0x0006u)  /* CRC reverse result Register */
#define OFS_CRCRESR_L          OFS_CRCRESR
#define OFS_CRCRESR_H          OFS_CRCRESR+1

#endif
/************************************************************
* CLOCK SYSTEM CONTROL
************************************************************/
#ifdef  __MSP430_HAS_CS__             /* Definition to show that Module is available */

#define OFS_CSCTL0            (0x0000u)   /* CS Control Register 0 */
#define OFS_CSCTL0_L           OFS_CSCTL0
#define OFS_CSCTL0_H           OFS_CSCTL0+1
#define OFS_CSCTL1            (0x0002u)   /* CS Control Register 1 */
#define OFS_CSCTL1_L           OFS_CSCTL1
#define OFS_CSCTL1_H           OFS_CSCTL1+1
#define OFS_CSCTL2            (0x0004u)   /* CS Control Register 2 */
#define OFS_CSCTL2_L           OFS_CSCTL2
#define OFS_CSCTL2_H           OFS_CSCTL2+1
#define OFS_CSCTL3            (0x0006u)   /* CS Control Register 3 */
#define OFS_CSCTL3_L           OFS_CSCTL3
#define OFS_CSCTL3_H           OFS_CSCTL3+1
#define OFS_CSCTL4            (0x0008u)   /* CS Control Register 4 */
#define OFS_CSCTL4_L           OFS_CSCTL4
#define OFS_CSCTL4_H           OFS_CSCTL4+1
#define OFS_CSCTL5            (0x000Au)   /* CS Control Register 5 */
#define OFS_CSCTL5_L           OFS_CSCTL5
#define OFS_CSCTL5_H           OFS_CSCTL5+1
#define OFS_CSCTL6            (0x000Cu)   /* CS Control Register 6 */
#define OFS_CSCTL6_L           OFS_CSCTL6
#define OFS_CSCTL6_H           OFS_CSCTL6+1
#define OFS_CSCTL7            (0x000Eu)   /* CS Control Register 7 */
#define OFS_CSCTL7_L           OFS_CSCTL7
#define OFS_CSCTL7_H           OFS_CSCTL7+1
#define OFS_CSCTL8            (0x0010u)   /* CS Control Register 8 */
#define OFS_CSCTL8_L           OFS_CSCTL8
#define OFS_CSCTL8_H           OFS_CSCTL8+1

/* CSCTL0 Control Bits */
#define DCO0                (0x0001u)    /* DCO TAP Bit : 0 */
#define DCO1                (0x0002u)    /* DCO TAP Bit : 1 */
#define DCO2                (0x0004u)    /* DCO TAP Bit : 2 */
#define DCO3                (0x0008u)    /* DCO TAP Bit : 3 */
#define DCO4                (0x0010u)    /* DCO TAP Bit : 4 */
#define DCO5                (0x0020u)    /* DCO TAP Bit : 5 */
#define DCO6                (0x0040u)    /* DCO TAP Bit : 6 */
#define DCO7                (0x0080u)    /* DCO TAP Bit : 7 */
#define DCO8                (0x0100u)    /* DCO TAP Bit : 8 */
#define MOD0                (0x0200u)    /* Modulation Bit Counter Bit : 0 */
#define MOD1                (0x0400u)    /* Modulation Bit Counter Bit : 1 */
#define MOD2                (0x0800u)    /* Modulation Bit Counter Bit : 2 */
#define MOD3                (0x1000u)    /* Modulation Bit Counter Bit : 3 */
#define MOD4                (0x2000u)    /* Modulation Bit Counter Bit : 4 */

/* CSCTL0 Control Bits */
#define DCO0_L              (0x0001u)    /* DCO TAP Bit : 0 */
#define DCO1_L              (0x0002u)    /* DCO TAP Bit : 1 */
#define DCO2_L              (0x0004u)    /* DCO TAP Bit : 2 */
#define DCO3_L              (0x0008u)    /* DCO TAP Bit : 3 */
#define DCO4_L              (0x0010u)    /* DCO TAP Bit : 4 */
#define DCO5_L              (0x0020u)    /* DCO TAP Bit : 5 */
#define DCO6_L              (0x0040u)    /* DCO TAP Bit : 6 */
#define DCO7_L              (0x0080u)    /* DCO TAP Bit : 7 */

/* CSCTL0 Control Bits */
#define DCO8_H              (0x0001u)    /* DCO TAP Bit : 8 */
#define MOD0_H              (0x0002u)    /* Modulation Bit Counter Bit : 0 */
#define MOD1_H              (0x0004u)    /* Modulation Bit Counter Bit : 1 */
#define MOD2_H              (0x0008u)    /* Modulation Bit Counter Bit : 2 */
#define MOD3_H              (0x0010u)    /* Modulation Bit Counter Bit : 3 */
#define MOD4_H              (0x0020u)    /* Modulation Bit Counter Bit : 4 */

/* CSCTL1 Control Bits */
#define DISMOD              (0x0001u)    /* Disable Modulation */
#define DCORSEL0            (0x0002u)    /* DCO frequency range select Bit: 0 */
#define DCORSEL1            (0x0004u)    /* DCO frequency range select Bit: 1 */
#define DCORSEL2            (0x0008u)    /* DCO frequency range select Bit: 2 */
#define DCOFTRIM0           (0x0010u)    /* DCO frequency trim. Bit: 0 */
#define DCOFTRIM1           (0x0020u)    /* DCO frequency trim. Bit: 1 */
#define DCOFTRIM2           (0x0040u)    /* DCO frequency trim. Bit: 2 */
#define DCOFTRIMEN          (0x0080u)    /* DCO frequency trim enable */

/* CSCTL1 Control Bits */
#define DISMOD_L            (0x0001u)    /* Disable Modulation */
#define DCORSEL0_L          (0x0002u)    /* DCO frequency range select Bit: 0 */
#define DCORSEL1_L          (0x0004u)    /* DCO frequency range select Bit: 1 */
#define DCORSEL2_L          (0x0008u)    /* DCO frequency range select Bit: 2 */
#define DCOFTRIM0_L         (0x0010u)    /* DCO frequency trim. Bit: 0 */
#define DCOFTRIM1_L         (0x0020u)    /* DCO frequency trim. Bit: 1 */
#define DCOFTRIM2_L         (0x0040u)    /* DCO frequency trim. Bit: 2 */
#define DCOFTRIMEN_L        (0x0080u)    /* DCO frequency trim enable */

#define DCORSEL_0           (0x0000u)    /* DCO frequency range select: 0 */
#define DCORSEL_1           (0x0002u)    /* DCO frequency range select: 1 */
#define DCORSEL_2           (0x0004u)    /* DCO frequency range select: 2 */
#define DCORSEL_3           (0x0006u)    /* DCO frequency range select: 3 */
#define DCORSEL_4           (0x0008u)    /* DCO frequency range select: 4 */
#define DCORSEL_5           (0x000Au)    /* DCO frequency range select: 5 */
#define DCORSEL_6           (0x000Cu)    /* DCO frequency range select: 6 */
#define DCORSEL_7           (0x000Eu)    /* DCO frequency range select: 7 */

#define DCOFTRIM_0          (0x0000u)    /* DCO frequency trim: 0 */
#define DCOFTRIM_1          (0x0010u)    /* DCO frequency trim: 1 */
#define DCOFTRIM_2          (0x0020u)    /* DCO frequency trim: 2 */
#define DCOFTRIM_3          (0x0030u)    /* DCO frequency trim: 3 */
#define DCOFTRIM_4          (0x0040u)    /* DCO frequency trim: 4 */
#define DCOFTRIM_5          (0x0050u)    /* DCO frequency trim: 5 */
#define DCOFTRIM_6          (0x0060u)    /* DCO frequency trim: 6 */
#define DCOFTRIM_7          (0x0070u)    /* DCO frequency trim: 7 */

/* CSCTL2 Control Bits */
#define FLLN0               (0x0001u)    /* FLL Multipier Bit : 0 */
#define FLLN1               (0x0002u)    /* FLL Multipier Bit : 1 */
#define FLLN2               (0x0004u)    /* FLL Multipier Bit : 2 */
#define FLLN3               (0x0008u)    /* FLL Multipier Bit : 3 */
#define FLLN4               (0x0010u)    /* FLL Multipier Bit : 4 */
#define FLLN5               (0x0020u)    /* FLL Multipier Bit : 5 */
#define FLLN6               (0x0040u)    /* FLL Multipier Bit : 6 */
#define FLLN7               (0x0080u)    /* FLL Multipier Bit : 7 */
#define FLLN8               (0x0100u)    /* FLL Multipier Bit : 8 */
#define FLLN9               (0x0200u)    /* FLL Multipier Bit : 9 */
#define FLLD0               (0x1000u)    /* Loop Divider Bit : 0 */
#define FLLD1               (0x2000u)    /* Loop Divider Bit : 1 */
#define FLLD2               (0x4000u)    /* Loop Divider Bit : 1 */

/* CSCTL2 Control Bits */
#define FLLN0_L             (0x0001u)    /* FLL Multipier Bit : 0 */
#define FLLN1_L             (0x0002u)    /* FLL Multipier Bit : 1 */
#define FLLN2_L             (0x0004u)    /* FLL Multipier Bit : 2 */
#define FLLN3_L             (0x0008u)    /* FLL Multipier Bit : 3 */
#define FLLN4_L             (0x0010u)    /* FLL Multipier Bit : 4 */
#define FLLN5_L             (0x0020u)    /* FLL Multipier Bit : 5 */
#define FLLN6_L             (0x0040u)    /* FLL Multipier Bit : 6 */
#define FLLN7_L             (0x0080u)    /* FLL Multipier Bit : 7 */

/* CSCTL2 Control Bits */
#define FLLN8_H             (0x0001u)    /* FLL Multipier Bit : 8 */
#define FLLN9_H             (0x0002u)    /* FLL Multipier Bit : 9 */
#define FLLD0_H             (0x0010u)    /* Loop Divider Bit : 0 */
#define FLLD1_H             (0x0020u)    /* Loop Divider Bit : 1 */
#define FLLD2_H             (0x0040u)    /* Loop Divider Bit : 1 */

#define FLLD_0             (0x0000u)    /* Multiply Selected Loop Freq. By 1 */
#define FLLD_1             (0x1000u)    /* Multiply Selected Loop Freq. By 2 */
#define FLLD_2             (0x2000u)    /* Multiply Selected Loop Freq. By 4 */
#define FLLD_3             (0x3000u)    /* Multiply Selected Loop Freq. By 8 */
#define FLLD_4             (0x4000u)    /* Multiply Selected Loop Freq. By 16 */
#define FLLD_5             (0x5000u)    /* Multiply Selected Loop Freq. By 32 */
#define FLLD_6             (0x6000u)    /* Reserved */
#define FLLD_7             (0x7000u)    /* Reserved */
#define FLLD__1            (0x0000u)    /* Multiply Selected Loop Freq. By 1 */
#define FLLD__2            (0x1000u)    /* Multiply Selected Loop Freq. By 2 */
#define FLLD__4            (0x2000u)    /* Multiply Selected Loop Freq. By 4 */
#define FLLD__8            (0x3000u)    /* Multiply Selected Loop Freq. By 8 */
#define FLLD__16           (0x4000u)    /* Multiply Selected Loop Freq. By 16 */
#define FLLD__32           (0x5000u)    /* Multiply Selected Loop Freq. By 32 */

/* CSCTL3 Control Bits */
#define FLLREFDIV0          (0x0001u)    /* Reference Divider Bit : 0 */
#define FLLREFDIV1          (0x0002u)    /* Reference Divider Bit : 1 */
#define FLLREFDIV2          (0x0004u)    /* Reference Divider Bit : 2 */
#define SELREF0             (0x0010u)    /* FLL Reference Clock Select Bit : 0 */
#define SELREF1             (0x0020u)    /* FLL Reference Clock Select Bit : 1 */

/* CSCTL3 Control Bits */
#define FLLREFDIV0_L        (0x0001u)    /* Reference Divider Bit : 0 */
#define FLLREFDIV1_L        (0x0002u)    /* Reference Divider Bit : 1 */
#define FLLREFDIV2_L        (0x0004u)    /* Reference Divider Bit : 2 */
#define SELREF0_L           (0x0010u)    /* FLL Reference Clock Select Bit : 0 */
#define SELREF1_L           (0x0020u)    /* FLL Reference Clock Select Bit : 1 */

#define FLLREFDIV_0         (0x0000u)    /* Reference Divider: f(LFCLK)/1 */
#define FLLREFDIV_1         (0x0001u)    /* Reference Divider: f(LFCLK)/2 */
#define FLLREFDIV_2         (0x0002u)    /* Reference Divider: f(LFCLK)/4 */
#define FLLREFDIV_3         (0x0003u)    /* Reference Divider: f(LFCLK)/8 */
#define FLLREFDIV_4         (0x0004u)    /* Reference Divider: f(LFCLK)/12 */
#define FLLREFDIV_5         (0x0005u)    /* Reference Divider: f(LFCLK)/16 */
#define FLLREFDIV_6         (0x0006u)    /* Reference Divider: f(LFCLK)/16 */
#define FLLREFDIV_7         (0x0007u)    /* Reference Divider: f(LFCLK)/16 */
#define FLLREFDIV__1        (0x0000u)    /* Reference Divider: f(LFCLK)/1   */
#define FLLREFDIV__32       (0x0001u)    /* Reference Divider: f(LFCLK)/32  */
#define FLLREFDIV__64       (0x0002u)    /* Reference Divider: f(LFCLK)/64  */
#define FLLREFDIV__128      (0x0003u)    /* Reference Divider: f(LFCLK)/128 */
#define FLLREFDIV__256      (0x0004u)    /* Reference Divider: f(LFCLK)/256 */
#define FLLREFDIV__512      (0x0005u)    /* Reference Divider: f(LFCLK)/512 */
#define SELREF_0            (0x0000u)    /* FLL Reference Clock Select 0 */
#define SELREF_1            (0x0010u)    /* FLL Reference Clock Select 1 */
#define SELREF_2            (0x0020u)    /* FLL Reference Clock Select 2 */
#define SELREF_3            (0x0030u)    /* FLL Reference Clock Select 3 */
#define SELREF__XT1CLK      (0x0000u)    /* Multiply Selected Loop Freq. By XT1CLK */
#define SELREF__REFOCLK     (0x0010u)    /* Multiply Selected Loop Freq. By REFOCLK */

/* CSCTL4 Control Bits */
#define SELMS0              (0x0001u)   /* MCLK and SMCLK Source Select Bit: 0 */
#define SELMS1              (0x0002u)   /* MCLK and SMCLK Source Select Bit: 1 */
#define SELMS2              (0x0004u)   /* MCLK and SMCLK Source Select Bit: 2 */
#define SELA                (0x0100u)   /* ACLK Source Select Bit: 0 */

/* CSCTL4 Control Bits */
#define SELMS0_L            (0x0001u)   /* MCLK and SMCLK Source Select Bit: 0 */
#define SELMS1_L            (0x0002u)   /* MCLK and SMCLK Source Select Bit: 1 */
#define SELMS2_L            (0x0004u)   /* MCLK and SMCLK Source Select Bit: 2 */

/* CSCTL4 Control Bits */
#define SELA_H              (0x0001u)   /* ACLK Source Select Bit: 0 */

#define SELMS_0             (0x0000u)   /* MCLK and SMCLK Source Select 0 */
#define SELMS_1             (0x0001u)   /* MCLK and SMCLK Source Select 1 */
#define SELMS_2             (0x0002u)   /* MCLK and SMCLK Source Select 2 */
#define SELMS_3             (0x0003u)   /* MCLK and SMCLK Source Select 3 */
#define SELMS_4             (0x0004u)   /* MCLK and SMCLK Source Select 4 */
#define SELMS_5             (0x0005u)   /* MCLK and SMCLK Source Select 5 */
#define SELMS_6             (0x0006u)   /* MCLK and SMCLK Source Select 6 */
#define SELMS_7             (0x0007u)   /* MCLK and SMCLK Source Select 7 */
#define SELMS__DCOCLKDIV    (0x0000u)   /* MCLK and SMCLK Source Select DCOCLKDIV */
#define SELMS__REFOCLK      (0x0001u)   /* MCLK and SMCLK Source Select REFOCLK */
#define SELMS__XT1CLK       (0x0002u)   /* MCLK and SMCLK Source Select XT1CLK */
#define SELMS__VLOCLK       (0x0003u)   /* MCLK and SMCLK Source Select VLOCLK */

#define SELA__XT1CLK        (0x0000u)   /* ACLK Source Select XT1CLK */
#define SELA__REFOCLK       (0x0100u)   /* ACLK Source Select REFOCLK */

/* CSCTL5 Control Bits */
#define DIVM0               (0x0001u)   /* MCLK Divider Bit: 0 */
#define DIVM1               (0x0002u)   /* MCLK Divider Bit: 1 */
#define DIVM2               (0x0004u)   /* MCLK Divider Bit: 2 */
#define DIVS0               (0x0010u)   /* SMCLK Divider Bit: 0 */
#define DIVS1               (0x0020u)   /* SMCLK Divider Bit: 1 */
#define SMCLKOFF            (0x0100u)   /* SMCLK off */
#define VLOAUTOOFF          (0x1000u)   /* VLO automatic off enable */

/* CSCTL5 Control Bits */
#define DIVM0_L             (0x0001u)   /* MCLK Divider Bit: 0 */
#define DIVM1_L             (0x0002u)   /* MCLK Divider Bit: 1 */
#define DIVM2_L             (0x0004u)   /* MCLK Divider Bit: 2 */
#define DIVS0_L             (0x0010u)   /* SMCLK Divider Bit: 0 */
#define DIVS1_L             (0x0020u)   /* SMCLK Divider Bit: 1 */

/* CSCTL5 Control Bits */
#define SMCLKOFF_H          (0x0001u)   /* SMCLK off */
#define VLOAUTOOFF_H        (0x0010u)   /* VLO automatic off enable */

#define DIVM_0              (0x0000u)    /* MCLK Source Divider 0 */
#define DIVM_1              (0x0001u)    /* MCLK Source Divider 1 */
#define DIVM_2              (0x0002u)    /* MCLK Source Divider 2 */
#define DIVM_3              (0x0003u)    /* MCLK Source Divider 3 */
#define DIVM_4              (0x0004u)    /* MCLK Source Divider 4 */
#define DIVM_5              (0x0005u)    /* MCLK Source Divider 5 */
#define DIVM_6              (0x0006u)    /* MCLK Source Divider 6 */
#define DIVM_7              (0x0007u)    /* MCLK Source Divider 7 */
#define DIVM__1             (0x0000u)    /* MCLK Source Divider f(MCLK)/1 */
#define DIVM__2             (0x0001u)    /* MCLK Source Divider f(MCLK)/2 */
#define DIVM__4             (0x0002u)    /* MCLK Source Divider f(MCLK)/4 */
#define DIVM__8             (0x0003u)    /* MCLK Source Divider f(MCLK)/8 */
#define DIVM__16            (0x0004u)    /* MCLK Source Divider f(MCLK)/16 */
#define DIVM__32            (0x0005u)    /* MCLK Source Divider f(MCLK)/32 */
#define DIVM__64            (0x0006u)    /* MCLK Source Divider f(MCLK)/64 */
#define DIVM__128           (0x0007u)    /* MCLK Source Divider f(MCLK)/128 */

#define DIVS_0              (0x0000u)    /* SMCLK Source Divider 0 */
#define DIVS_1              (0x0010u)    /* SMCLK Source Divider 1 */
#define DIVS_2              (0x0020u)    /* SMCLK Source Divider 2 */
#define DIVS_3              (0x0030u)    /* SMCLK Source Divider 3 */
#define DIVS__1             (0x0000u)    /* SMCLK Source Divider f(SMCLK)/1 */
#define DIVS__2             (0x0010u)    /* SMCLK Source Divider f(SMCLK)/2 */
#define DIVS__4             (0x0020u)    /* SMCLK Source Divider f(SMCLK)/4 */
#define DIVS__8             (0x0030u)    /* SMCLK Source Divider f(SMCLK)/8 */

/* CSCTL6 Control Bits */
#define XT1AUTOOFF          (0x0001u)    /* XT1 automatic off enable */
#define XT1AGCOFF           (0x0002u)    /* XT1 Automatic Gain Control (AGC) disable */
#define XT1BYPASS           (0x0010u)    /* XT1 bypass mode : 0: internal 1:sourced from external pin */
#define XTS                 (0x0020u)    /* 1: Selects high-freq. oscillator */
#define XT1DRIVE0           (0x0040u)    /* XT1 Drive Level mode Bit 0 */
#define XT1DRIVE1           (0x0080u)    /* XT1 Drive Level mode Bit 1 */

/* CSCTL6 Control Bits */
#define XT1AUTOOFF_L        (0x0001u)    /* XT1 automatic off enable */
#define XT1AGCOFF_L         (0x0002u)    /* XT1 Automatic Gain Control (AGC) disable */
#define XT1BYPASS_L         (0x0010u)    /* XT1 bypass mode : 0: internal 1:sourced from external pin */
#define XTS_L               (0x0020u)    /* 1: Selects high-freq. oscillator */
#define XT1DRIVE0_L         (0x0040u)    /* XT1 Drive Level mode Bit 0 */
#define XT1DRIVE1_L         (0x0080u)    /* XT1 Drive Level mode Bit 1 */

#define XT1DRIVE_0          (0x0000u)    /* XT1 Drive Level mode: 0 */
#define XT1DRIVE_1          (0x0040u)    /* XT1 Drive Level mode: 1 */
#define XT1DRIVE_2          (0x0080u)    /* XT1 Drive Level mode: 2 */
#define XT1DRIVE_3          (0x00C0u)    /* XT1 Drive Level mode: 3 */

/* CSCTL7 Control Bits */
#define DCOFFG              (0x0001u)    /* DCO fault flag */
#define XT1OFFG             (0x0002u)    /* XT1 Low Frequency Oscillator Fault Flag */
#define FLLULIFG            (0x0010u)    /* FLL unlock interrupt flag */
#define ENSTFCNT1           (0x0040u)    /* Enable start counter for XT1 */
#define FLLUNLOCK0          (0x0100u)    /* FLL unlock condition Bit: 0 */
#define FLLUNLOCK1          (0x0200u)    /* FLL unlock condition Bit: 1 */
#define FLLUNLOCKHIS0       (0x0400u)    /* Unlock history Bit: 0 */
#define FLLUNLOCKHIS1       (0x0800u)    /* Unlock history Bit: 1 */
#define FLLULPUC            (0x1000u)    /* FLL unlock PUC enable */
#define FLLWARNEN           (0x2000u)    /* Warning enable */

/* CSCTL7 Control Bits */
#define DCOFFG_L            (0x0001u)    /* DCO fault flag */
#define XT1OFFG_L           (0x0002u)    /* XT1 Low Frequency Oscillator Fault Flag */
#define FLLULIFG_L          (0x0010u)    /* FLL unlock interrupt flag */
#define ENSTFCNT1_L         (0x0040u)    /* Enable start counter for XT1 */

/* CSCTL7 Control Bits */
#define FLLUNLOCK0_H        (0x0001u)    /* FLL unlock condition Bit: 0 */
#define FLLUNLOCK1_H        (0x0002u)    /* FLL unlock condition Bit: 1 */
#define FLLUNLOCKHIS0_H     (0x0004u)    /* Unlock history Bit: 0 */
#define FLLUNLOCKHIS1_H     (0x0008u)    /* Unlock history Bit: 1 */
#define FLLULPUC_H          (0x0010u)    /* FLL unlock PUC enable */
#define FLLWARNEN_H         (0x0020u)    /* Warning enable */

#define FLLUNLOCK_0         (0x0000u)    /* FLL unlock condition: 0 */
#define FLLUNLOCK_1         (0x0100u)    /* FLL unlock condition: 1 */
#define FLLUNLOCK_2         (0x0200u)    /* FLL unlock condition: 2 */
#define FLLUNLOCK_3         (0x0300u)    /* FLL unlock condition: 3 */
#define FLLUNLOCKHIS_0      (0x0000u)    /* Unlock history: 0 */
#define FLLUNLOCKHIS_1      (0x0400u)    /* Unlock history: 1 */
#define FLLUNLOCKHIS_2      (0x0800u)    /* Unlock history: 2 */
#define FLLUNLOCKHIS_3      (0x0C00u)    /* Unlock history: 3 */

/* CSCTL8 Control Bits */
#define ACLKREQEN           (0x0001u)    /* ACLK Clock Request Enable */
#define MCLKREQEN           (0x0002u)    /* MCLK Clock Request Enable */
#define SMCLKREQEN          (0x0004u)    /* SMCLK Clock Request Enable */
#define MODCLKREQEN         (0x0008u)    /* MODOSC Clock Request Enable */

/* CSCTL8 Control Bits */
#define ACLKREQEN_L         (0x0001u)    /* ACLK Clock Request Enable */
#define MCLKREQEN_L         (0x0002u)    /* MCLK Clock Request Enable */
#define SMCLKREQEN_L        (0x0004u)    /* SMCLK Clock Request Enable */
#define MODCLKREQEN_L       (0x0008u)    /* MODOSC Clock Request Enable */

#endif
/*************************************************************
* FRAM Memory
*************************************************************/
#ifdef  __MSP430_HAS_FRAM__           /* Definition to show that Module is available */

#define OFS_FRCTL0            (0x0000u)  /* FRAM Controller Control 0 */
#define OFS_FRCTL0_L           OFS_FRCTL0
#define OFS_FRCTL0_H           OFS_FRCTL0+1
#define OFS_GCCTL0            (0x0004u)  /* General Control 0 */
#define OFS_GCCTL0_L           OFS_GCCTL0
#define OFS_GCCTL0_H           OFS_GCCTL0+1
#define OFS_GCCTL1            (0x0006u)  /* General Control 1 */
#define OFS_GCCTL1_L           OFS_GCCTL1
#define OFS_GCCTL1_H           OFS_GCCTL1+1

#define FRCTLPW             (0xA500u)  /* FRAM password for write */
#define FRPW                (0x9600u)  /* FRAM password returned by read */
#define FWPW                (0xA500u)  /* FRAM password for write */
#define FXPW                (0x3300u)  /* for use with XOR instruction */

/* FRCTL0 Control Bits */
//#define RESERVED            (0x0001u)  /* RESERVED */
//#define RESERVED            (0x0002u)  /* RESERVED */
//#define RESERVED            (0x0004u)  /* RESERVED */
#define NWAITS0             (0x0010u)  /* FRAM Wait state control Bit: 0 */
#define NWAITS1             (0x0020u)  /* FRAM Wait state control Bit: 1 */
#define NWAITS2             (0x0040u)  /* FRAM Wait state control Bit: 2 */
//#define RESERVED            (0x0080u)  /* RESERVED */

/* FRCTL0 Control Bits */
//#define RESERVED            (0x0001u)  /* RESERVED */
//#define RESERVED            (0x0002u)  /* RESERVED */
//#define RESERVED            (0x0004u)  /* RESERVED */
#define NWAITS0_L           (0x0010u)  /* FRAM Wait state control Bit: 0 */
#define NWAITS1_L           (0x0020u)  /* FRAM Wait state control Bit: 1 */
#define NWAITS2_L           (0x0040u)  /* FRAM Wait state control Bit: 2 */
//#define RESERVED            (0x0080u)  /* RESERVED */

#define NWAITS_0            (0x0000u)  /* FRAM Wait state control: 0 */
#define NWAITS_1            (0x0010u)  /* FRAM Wait state control: 1 */
#define NWAITS_2            (0x0020u)  /* FRAM Wait state control: 2 */
#define NWAITS_3            (0x0030u)  /* FRAM Wait state control: 3 */
#define NWAITS_4            (0x0040u)  /* FRAM Wait state control: 4 */
#define NWAITS_5            (0x0050u)  /* FRAM Wait state control: 5 */
#define NWAITS_6            (0x0060u)  /* FRAM Wait state control: 6 */
#define NWAITS_7            (0x0070u)  /* FRAM Wait state control: 7 */

/* GCCTL0 Control Bits */
//#define RESERVED            (0x0001u)  /* RESERVED */
#define FRLPMPWR            (0x0002u)  /* FRAM Enable FRAM auto power up after LPM */
#define FRPWR               (0x0004u)  /* FRAM Power Control */
#define ACCTEIE             (0x0008u)  /* Enable NMI event if Access time error occurs */
//#define RESERVED            (0x0010u)  /* RESERVED */
#define CBDIE               (0x0020u)  /* Enable NMI event if correctable bit error detected */
#define UBDIE               (0x0040u)  /* Enable NMI event if uncorrectable bit error detected */
#define UBDRSTEN            (0x0080u)  /* Enable Power Up Clear (PUC) reset if FRAM uncorrectable bit error detected */

/* GCCTL0 Control Bits */
//#define RESERVED            (0x0001u)  /* RESERVED */
#define FRLPMPWR_L          (0x0002u)  /* FRAM Enable FRAM auto power up after LPM */
#define FRPWR_L             (0x0004u)  /* FRAM Power Control */
#define ACCTEIE_L           (0x0008u)  /* Enable NMI event if Access time error occurs */
//#define RESERVED            (0x0010u)  /* RESERVED */
#define CBDIE_L             (0x0020u)  /* Enable NMI event if correctable bit error detected */
#define UBDIE_L             (0x0040u)  /* Enable NMI event if uncorrectable bit error detected */
#define UBDRSTEN_L          (0x0080u)  /* Enable Power Up Clear (PUC) reset if FRAM uncorrectable bit error detected */

/* GCCTL1 Control Bits */
//#define RESERVED            (0x0001u)  /* RESERVED */
#define CBDIFG              (0x0002u)  /* FRAM correctable bit error flag */
#define UBDIFG              (0x0004u)  /* FRAM uncorrectable bit error flag */
#define ACCTEIFG            (0x0008u)  /* Access time error flag */

/* GCCTL1 Control Bits */
//#define RESERVED            (0x0001u)  /* RESERVED */
#define CBDIFG_L            (0x0002u)  /* FRAM correctable bit error flag */
#define UBDIFG_L            (0x0004u)  /* FRAM uncorrectable bit error flag */
#define ACCTEIFG_L          (0x0008u)  /* Access time error flag */

#endif
/************************************************************
* LCD_E
************************************************************/
#ifdef  __MSP430_HAS_LCD_E__          /* Definition to show that Module is available */

#define OFS_LCDCTL0           (0x0000u)  /* LCD_E Control Register 0 */
#define OFS_LCDCTL0_L          OFS_LCDCTL0
#define OFS_LCDCTL0_H          OFS_LCDCTL0+1
#define OFS_LCDCTL1           (0x0002u)  /* LCD_E Control Register 1 */
#define OFS_LCDCTL1_L          OFS_LCDCTL1
#define OFS_LCDCTL1_H          OFS_LCDCTL1+1
#define OFS_LCDBLKCTL         (0x0004u)  /* LCD_E blinking control register */
#define OFS_LCDBLKCTL_L        OFS_LCDBLKCTL
#define OFS_LCDBLKCTL_H        OFS_LCDBLKCTL+1
#define OFS_LCDMEMCTL         (0x0006u)  /* LCD_E memory control register */
#define OFS_LCDMEMCTL_L        OFS_LCDMEMCTL
#define OFS_LCDMEMCTL_H        OFS_LCDMEMCTL+1
#define OFS_LCDVCTL           (0x0008u)  /* LCD_E Voltage Control Register */
#define OFS_LCDVCTL_L          OFS_LCDVCTL
#define OFS_LCDVCTL_H          OFS_LCDVCTL+1
#define OFS_LCDPCTL0          (0x000Au)  /* LCD_E Port Control Register 0 */
#define OFS_LCDPCTL0_L         OFS_LCDPCTL0
#define OFS_LCDPCTL0_H         OFS_LCDPCTL0+1
#define OFS_LCDPCTL1          (0x000Cu)  /* LCD_E Port Control Register 1 */
#define OFS_LCDPCTL1_L         OFS_LCDPCTL1
#define OFS_LCDPCTL1_H         OFS_LCDPCTL1+1
#define OFS_LCDPCTL2          (0x000Eu)  /* LCD_E Port Control Register 2 */
#define OFS_LCDPCTL2_L         OFS_LCDPCTL2
#define OFS_LCDPCTL2_H         OFS_LCDPCTL2+1
#define OFS_LCDCSSEL0         (0x0014u)  /* LCD_E COM/SEG select register 0 */
#define OFS_LCDCSSEL0_L        OFS_LCDCSSEL0
#define OFS_LCDCSSEL0_H        OFS_LCDCSSEL0+1
#define OFS_LCDCSSEL1         (0x0016u)  /* LCD_E COM/SEG select register 1 */
#define OFS_LCDCSSEL1_L        OFS_LCDCSSEL1
#define OFS_LCDCSSEL1_H        OFS_LCDCSSEL1+1
#define OFS_LCDCSSEL2         (0x0018u)  /* LCD_E COM/SEG select register 2 */
#define OFS_LCDCSSEL2_L        OFS_LCDCSSEL2
#define OFS_LCDCSSEL2_H        OFS_LCDCSSEL2+1
#define OFS_LCDIV             (0x001Eu)  /* LCD_E Interrupt Vector Register */

// LCDCTL0
#define LCDON               (0x0001u)  /* LCD_E LCD On */
#define LCDLP               (0x0002u)  /* LCD_E Low Power Waveform */
#define LCDSON              (0x0004u)  /* LCD_E LCD Segments On */
#define LCDMX0              (0x0008u)  /* LCD_E Mux Rate Bit: 0 */
#define LCDMX1              (0x0010u)  /* LCD_E Mux Rate Bit: 1 */
#define LCDMX2              (0x0020u)  /* LCD_E Mux Rate Bit: 2 */
#define LCDSSEL0            (0x0040u)  /* LCD_E Clock Select Bit: 0 */
#define LCDSSEL1            (0x0080u)  /* LCD_E Clock Select Bit: 1 */
#define LCDDIV0             (0x0800u)  /* LCD_E LCD frequency divider Bit: 0 */
#define LCDDIV1             (0x1000u)  /* LCD_E LCD frequency divider Bit: 1 */
#define LCDDIV2             (0x2000u)  /* LCD_E LCD frequency divider Bit: 2 */
#define LCDDIV3             (0x4000u)  /* LCD_E LCD frequency divider Bit: 3 */
#define LCDDIV4             (0x8000u)  /* LCD_E LCD frequency divider Bit: 4 */

// LCDCTL0
#define LCDON_L             (0x0001u)  /* LCD_E LCD On */
#define LCDLP_L             (0x0002u)  /* LCD_E Low Power Waveform */
#define LCDSON_L            (0x0004u)  /* LCD_E LCD Segments On */
#define LCDMX0_L            (0x0008u)  /* LCD_E Mux Rate Bit: 0 */
#define LCDMX1_L            (0x0010u)  /* LCD_E Mux Rate Bit: 1 */
#define LCDMX2_L            (0x0020u)  /* LCD_E Mux Rate Bit: 2 */
#define LCDSSEL0_L          (0x0040u)  /* LCD_E Clock Select Bit: 0 */
#define LCDSSEL1_L          (0x0080u)  /* LCD_E Clock Select Bit: 1 */

// LCDCTL0
#define LCDDIV0_H           (0x0008u)  /* LCD_E LCD frequency divider Bit: 0 */
#define LCDDIV1_H           (0x0010u)  /* LCD_E LCD frequency divider Bit: 1 */
#define LCDDIV2_H           (0x0020u)  /* LCD_E LCD frequency divider Bit: 2 */
#define LCDDIV3_H           (0x0040u)  /* LCD_E LCD frequency divider Bit: 3 */
#define LCDDIV4_H           (0x0080u)  /* LCD_E LCD frequency divider Bit: 4 */

#define LCDSSEL_0           (0x0000u)  /* LCD_E Clock Select: 0 */
#define LCDSSEL_1           (0x0040u)  /* LCD_E Clock Select: 1 */
#define LCDSSEL_2           (0x0080u)  /* LCD_E Clock Select: 2 */
#define LCDSSEL_3           (0x00C0u)  /* LCD_E Clock Select: 3 */
#define LCDSSEL__XTCLK      (0x0000u)  /* LCD_E Clock Select: XTCLK */
#define LCDSSEL__ACLK       (0x0040u)  /* LCD_E Clock Select: ACLK */
#define LCDSSEL__VLOCLK     (0x0080u)  /* LCD_E Clock Select: VLOCLK */

#define LCDDIV_0            (0x0000u)  /* LCD_E LCD frequency divider: /1 */
#define LCDDIV_1            (0x0800u)  /* LCD_E LCD frequency divider: /2 */
#define LCDDIV_2            (0x1000u)  /* LCD_E LCD frequency divider: /3 */
#define LCDDIV_3            (0x1800u)  /* LCD_E LCD frequency divider: /4 */
#define LCDDIV_4            (0x2000u)  /* LCD_E LCD frequency divider: /5 */
#define LCDDIV_5            (0x2800u)  /* LCD_E LCD frequency divider: /6 */
#define LCDDIV_6            (0x3000u)  /* LCD_E LCD frequency divider: /7 */
#define LCDDIV_7            (0x3800u)  /* LCD_E LCD frequency divider: /8 */
#define LCDDIV_8            (0x4000u)  /* LCD_E LCD frequency divider: /9 */
#define LCDDIV_9            (0x4800u)  /* LCD_E LCD frequency divider: /10 */
#define LCDDIV_10           (0x5000u)  /* LCD_E LCD frequency divider: /11 */
#define LCDDIV_11           (0x5800u)  /* LCD_E LCD frequency divider: /12 */
#define LCDDIV_12           (0x6000u)  /* LCD_E LCD frequency divider: /13 */
#define LCDDIV_13           (0x6800u)  /* LCD_E LCD frequency divider: /14 */
#define LCDDIV_14           (0x7000u)  /* LCD_E LCD frequency divider: /15 */
#define LCDDIV_15           (0x7800u)  /* LCD_E LCD frequency divider: /16 */
#define LCDDIV_16           (0x8000u)  /* LCD_E LCD frequency divider: /17 */
#define LCDDIV_17           (0x8800u)  /* LCD_E LCD frequency divider: /18 */
#define LCDDIV_18           (0x9000u)  /* LCD_E LCD frequency divider: /19 */
#define LCDDIV_19           (0x9800u)  /* LCD_E LCD frequency divider: /20 */
#define LCDDIV_20           (0xA000u)  /* LCD_E LCD frequency divider: /21 */
#define LCDDIV_21           (0xA800u)  /* LCD_E LCD frequency divider: /22 */
#define LCDDIV_22           (0xB000u)  /* LCD_E LCD frequency divider: /23 */
#define LCDDIV_23           (0xB800u)  /* LCD_E LCD frequency divider: /24 */
#define LCDDIV_24           (0xC000u)  /* LCD_E LCD frequency divider: /25 */
#define LCDDIV_25           (0xC800u)  /* LCD_E LCD frequency divider: /26 */
#define LCDDIV_26           (0xD000u)  /* LCD_E LCD frequency divider: /27 */
#define LCDDIV_27           (0xD800u)  /* LCD_E LCD frequency divider: /28 */
#define LCDDIV_28           (0xE000u)  /* LCD_E LCD frequency divider: /29 */
#define LCDDIV_29           (0xE800u)  /* LCD_E LCD frequency divider: /30 */
#define LCDDIV_30           (0xF000u)  /* LCD_E LCD frequency divider: /31 */
#define LCDDIV_31           (0xF800u)  /* LCD_E LCD frequency divider: /32 */
#define LCDDIV__1           (0x0000u)  /* LCD_E LCD frequency divider: /1 */
#define LCDDIV__2           (0x0800u)  /* LCD_E LCD frequency divider: /2 */
#define LCDDIV__3           (0x1000u)  /* LCD_E LCD frequency divider: /3 */
#define LCDDIV__4           (0x1800u)  /* LCD_E LCD frequency divider: /4 */
#define LCDDIV__5           (0x2000u)  /* LCD_E LCD frequency divider: /5 */
#define LCDDIV__6           (0x2800u)  /* LCD_E LCD frequency divider: /6 */
#define LCDDIV__7           (0x3000u)  /* LCD_E LCD frequency divider: /7 */
#define LCDDIV__8           (0x3800u)  /* LCD_E LCD frequency divider: /8 */
#define LCDDIV__9           (0x4000u)  /* LCD_E LCD frequency divider: /9 */
#define LCDDIV__10          (0x4800u)  /* LCD_E LCD frequency divider: /10 */
#define LCDDIV__11          (0x5000u)  /* LCD_E LCD frequency divider: /11 */
#define LCDDIV__12          (0x5800u)  /* LCD_E LCD frequency divider: /12 */
#define LCDDIV__13          (0x6000u)  /* LCD_E LCD frequency divider: /13 */
#define LCDDIV__14          (0x6800u)  /* LCD_E LCD frequency divider: /14 */
#define LCDDIV__15          (0x7000u)  /* LCD_E LCD frequency divider: /15 */
#define LCDDIV__16          (0x7800u)  /* LCD_E LCD frequency divider: /16 */
#define LCDDIV__17          (0x8000u)  /* LCD_E LCD frequency divider: /17 */
#define LCDDIV__18          (0x8800u)  /* LCD_E LCD frequency divider: /18 */
#define LCDDIV__19          (0x9000u)  /* LCD_E LCD frequency divider: /19 */
#define LCDDIV__20          (0x9800u)  /* LCD_E LCD frequency divider: /20 */
#define LCDDIV__21          (0xA000u)  /* LCD_E LCD frequency divider: /21 */
#define LCDDIV__22          (0xA800u)  /* LCD_E LCD frequency divider: /22 */
#define LCDDIV__23          (0xB000u)  /* LCD_E LCD frequency divider: /23 */
#define LCDDIV__24          (0xB800u)  /* LCD_E LCD frequency divider: /24 */
#define LCDDIV__25          (0xC000u)  /* LCD_E LCD frequency divider: /25 */
#define LCDDIV__26          (0xC800u)  /* LCD_E LCD frequency divider: /26 */
#define LCDDIV__27          (0xD000u)  /* LCD_E LCD frequency divider: /27 */
#define LCDDIV__28          (0xD800u)  /* LCD_E LCD frequency divider: /28 */
#define LCDDIV__29          (0xE000u)  /* LCD_E LCD frequency divider: /29 */
#define LCDDIV__30          (0xE800u)  /* LCD_E LCD frequency divider: /30 */
#define LCDDIV__31          (0xF000u)  /* LCD_E LCD frequency divider: /31 */
#define LCDDIV__32          (0xF800u)  /* LCD_E LCD frequency divider: /32 */

/* Display modes coded with Bits 3-5 */
#define LCDSTATIC           (LCDSON)
#define LCD2MUX             (LCDMX0+LCDSON)
#define LCD3MUX             (LCDMX1+LCDSON)
#define LCD4MUX             (LCDMX1+LCDMX0+LCDSON)
#define LCD5MUX             (LCDMX2+LCDSON)
#define LCD6MUX             (LCDMX2+LCDMX0+LCDSON)
#define LCD7MUX             (LCDMX2+LCDMX1+LCDSON)
#define LCD8MUX             (LCDMX2+LCDMX1+LCDMX0+LCDSON)

// LCDCTL1
#define LCDFRMIFG           (0x0001u)  /* LCD_E LCD frame interrupt flag */
#define LCDBLKOFFIFG        (0x0002u)  /* LCD_E LCD blinking off interrupt flag, */
#define LCDBLKONIFG         (0x0004u)  /* LCD_E LCD blinking on interrupt flag, */
#define LCDFRMIE            (0x0100u)  /* LCD_E LCD frame interrupt enable */
#define LCDBLKOFFIE         (0x0200u)  /* LCD_E LCD blinking off interrupt flag, */
#define LCDBLKONIE          (0x0400u)  /* LCD_E LCD blinking on interrupt flag, */

// LCDCTL1
#define LCDFRMIFG_L         (0x0001u)  /* LCD_E LCD frame interrupt flag */
#define LCDBLKOFFIFG_L      (0x0002u)  /* LCD_E LCD blinking off interrupt flag, */
#define LCDBLKONIFG_L       (0x0004u)  /* LCD_E LCD blinking on interrupt flag, */

// LCDCTL1
#define LCDFRMIE_H          (0x0001u)  /* LCD_E LCD frame interrupt enable */
#define LCDBLKOFFIE_H       (0x0002u)  /* LCD_E LCD blinking off interrupt flag, */
#define LCDBLKONIE_H        (0x0004u)  /* LCD_E LCD blinking on interrupt flag, */

// LCDBLKCTL
#define LCDBLKMOD0          (0x0001u)  /* LCD_E Blinking mode Bit: 0 */
#define LCDBLKMOD1          (0x0002u)  /* LCD_E Blinking mode Bit: 1 */
#define LCDBLKPRE0          (0x0004u)  /* LCD_E Clock pre-scaler for blinking frequency Bit: 0 */
#define LCDBLKPRE1          (0x0008u)  /* LCD_E Clock pre-scaler for blinking frequency Bit: 1 */
#define LCDBLKPRE2          (0x0010u)  /* LCD_E Clock pre-scaler for blinking frequency Bit: 2 */

// LCDBLKCTL
#define LCDBLKMOD0_L        (0x0001u)  /* LCD_E Blinking mode Bit: 0 */
#define LCDBLKMOD1_L        (0x0002u)  /* LCD_E Blinking mode Bit: 1 */
#define LCDBLKPRE0_L        (0x0004u)  /* LCD_E Clock pre-scaler for blinking frequency Bit: 0 */
#define LCDBLKPRE1_L        (0x0008u)  /* LCD_E Clock pre-scaler for blinking frequency Bit: 1 */
#define LCDBLKPRE2_L        (0x0010u)  /* LCD_E Clock pre-scaler for blinking frequency Bit: 2 */

#define LCDBLKMOD_0         (0x0000u)  /* LCD_E Blinking mode: Off */
#define LCDBLKMOD_1         (0x0001u)  /* LCD_E Blinking mode: Individual */
#define LCDBLKMOD_2         (0x0002u)  /* LCD_E Blinking mode: All */
#define LCDBLKMOD_3         (0x0003u)  /* LCD_E Blinking mode: Switching */

#define LCDBLKPRE_0         (0x0000u)  /* LCD_E Clock pre-scaler for blinking frequency: 0 */
#define LCDBLKPRE_1         (0x0004u)  /* LCD_E Clock pre-scaler for blinking frequency: 1 */
#define LCDBLKPRE_2         (0x0008u)  /* LCD_E Clock pre-scaler for blinking frequency: 2 */
#define LCDBLKPRE_3         (0x000Cu)  /* LCD_E Clock pre-scaler for blinking frequency: 3 */
#define LCDBLKPRE_4         (0x0010u)  /* LCD_E Clock pre-scaler for blinking frequency: 4 */
#define LCDBLKPRE_5         (0x0014u)  /* LCD_E Clock pre-scaler for blinking frequency: 5 */
#define LCDBLKPRE_6         (0x0018u)  /* LCD_E Clock pre-scaler for blinking frequency: 6 */
#define LCDBLKPRE_7         (0x001Cu)  /* LCD_E Clock pre-scaler for blinking frequency: 7 */

#define LCDBLKPRE__4        (0x0000u)  /* LCD_E Clock pre-scaler for blinking frequency: 4   */
#define LCDBLKPRE__8        (0x0004u)  /* LCD_E Clock pre-scaler for blinking frequency: 8   */
#define LCDBLKPRE__16       (0x0008u)  /* LCD_E Clock pre-scaler for blinking frequency: 16  */
#define LCDBLKPRE__32       (0x000Cu)  /* LCD_E Clock pre-scaler for blinking frequency: 32  */
#define LCDBLKPRE__64       (0x0010u)  /* LCD_E Clock pre-scaler for blinking frequency: 64  */
#define LCDBLKPRE__128      (0x0014u)  /* LCD_E Clock pre-scaler for blinking frequency: 128 */
#define LCDBLKPRE__256      (0x0018u)  /* LCD_E Clock pre-scaler for blinking frequency: 256 */
#define LCDBLKPRE__512      (0x001Cu)  /* LCD_E Clock pre-scaler for blinking frequency: 512 */

// LCDMEMCTL
#define LCDDISP             (0x0001u)  /* LCD_E LCD memory registers for display */
#define LCDCLRM             (0x0002u)  /* LCD_E Clear LCD memory */
#define LCDCLRBM            (0x0004u)  /* LCD_E Clear LCD blinking memory */

// LCDMEMCTL
#define LCDDISP_L           (0x0001u)  /* LCD_E LCD memory registers for display */
#define LCDCLRM_L           (0x0002u)  /* LCD_E Clear LCD memory */
#define LCDCLRBM_L          (0x0004u)  /* LCD_E Clear LCD blinking memory */

// LCDVCTL
#define LCDREFMODE          (0x0001u)  /* Selects wether R13 voltage is switched or in static mode */
#define LCDSELVDD           (0x0020u)  /* selects if R33 is supplied either from Vcc internally or from charge pump */
#define LCDREFEN            (0x0040u)  /* Internal reference voltage enable on R13 */
#define LCDCPEN             (0x0080u)  /* Charge pump enable */
#define VLCD0               (0x0100u)  /* VLCD select: 0 */
#define VLCD1               (0x0200u)  /* VLCD select: 1 */
#define VLCD2               (0x0400u)  /* VLCD select: 2 */
#define VLCD3               (0x0800u)  /* VLCD select: 3 */
#define LCDCPFSEL0          (0x1000u)  /* Charge pump frequency selection Bit: 0 */
#define LCDCPFSEL1          (0x2000u)  /* Charge pump frequency selection Bit: 1 */
#define LCDCPFSEL2          (0x4000u)  /* Charge pump frequency selection Bit: 2 */
#define LCDCPFSEL3          (0x8000u)  /* Charge pump frequency selection Bit: 3 */

// LCDVCTL
#define LCDREFMODE_L        (0x0001u)  /* Selects wether R13 voltage is switched or in static mode */
#define LCDSELVDD_L         (0x0020u)  /* selects if R33 is supplied either from Vcc internally or from charge pump */
#define LCDREFEN_L          (0x0040u)  /* Internal reference voltage enable on R13 */
#define LCDCPEN_L           (0x0080u)  /* Charge pump enable */

// LCDVCTL
#define VLCD0_H             (0x0001u)  /* VLCD select: 0 */
#define VLCD1_H             (0x0002u)  /* VLCD select: 1 */
#define VLCD2_H             (0x0004u)  /* VLCD select: 2 */
#define VLCD3_H             (0x0008u)  /* VLCD select: 3 */
#define LCDCPFSEL0_H        (0x0010u)  /* Charge pump frequency selection Bit: 0 */
#define LCDCPFSEL1_H        (0x0020u)  /* Charge pump frequency selection Bit: 1 */
#define LCDCPFSEL2_H        (0x0040u)  /* Charge pump frequency selection Bit: 2 */
#define LCDCPFSEL3_H        (0x0080u)  /* Charge pump frequency selection Bit: 3 */

/* Charge pump voltage selections */
#define VLCD_0              (0x0000u)  /* VLCD = 2.60V */
#define VLCD_1              (0x0100u)  /* VLCD = 2.66V */
#define VLCD_2              (0x0200u)  /* VLCD = 2.72V */
#define VLCD_3              (0x0300u)  /* VLCD = 2.78V */
#define VLCD_4              (0x0400u)  /* VLCD = 2.84V */
#define VLCD_5              (0x0500u)  /* VLCD = 2.90V */
#define VLCD_6              (0x0600u)  /* VLCD = 2.96V */
#define VLCD_7              (0x0700u)  /* VLCD = 3.02V */
#define VLCD_8              (0x0800u)  /* VLCD = 3.08V */
#define VLCD_9              (0x0900u)  /* VLCD = 3.14V */
#define VLCD_10             (0x0A00u)  /* VLCD = 3.20V */
#define VLCD_11             (0x0B00u)  /* VLCD = 3.26V */
#define VLCD_12             (0x0C00u)  /* VLCD = 3.32V */
#define VLCD_13             (0x0D00u)  /* VLCD = 3.38V */
#define VLCD_14             (0x0E00u)  /* VLCD = 3.44V */
#define VLCD_15             (0x0F00u)  /* VLCD = 3.50V */

// LCDPCTL0
#define LCDS0               (0x0001u)  /* LCD Segment  0 enable. */
#define LCDS1               (0x0002u)  /* LCD Segment  1 enable. */
#define LCDS2               (0x0004u)  /* LCD Segment  2 enable. */
#define LCDS3               (0x0008u)  /* LCD Segment  3 enable. */
#define LCDS4               (0x0010u)  /* LCD Segment  4 enable. */
#define LCDS5               (0x0020u)  /* LCD Segment  5 enable. */
#define LCDS6               (0x0040u)  /* LCD Segment  6 enable. */
#define LCDS7               (0x0080u)  /* LCD Segment  7 enable. */
#define LCDS8               (0x0100u)  /* LCD Segment  8 enable. */
#define LCDS9               (0x0200u)  /* LCD Segment  9 enable. */
#define LCDS10              (0x0400u)  /* LCD Segment 10 enable. */
#define LCDS11              (0x0800u)  /* LCD Segment 11 enable. */
#define LCDS12              (0x1000u)  /* LCD Segment 12 enable. */
#define LCDS13              (0x2000u)  /* LCD Segment 13 enable. */
#define LCDS14              (0x4000u)  /* LCD Segment 14 enable. */
#define LCDS15              (0x8000u)  /* LCD Segment 15 enable. */

// LCDPCTL0
#define LCDS0_L             (0x0001u)  /* LCD Segment  0 enable. */
#define LCDS1_L             (0x0002u)  /* LCD Segment  1 enable. */
#define LCDS2_L             (0x0004u)  /* LCD Segment  2 enable. */
#define LCDS3_L             (0x0008u)  /* LCD Segment  3 enable. */
#define LCDS4_L             (0x0010u)  /* LCD Segment  4 enable. */
#define LCDS5_L             (0x0020u)  /* LCD Segment  5 enable. */
#define LCDS6_L             (0x0040u)  /* LCD Segment  6 enable. */
#define LCDS7_L             (0x0080u)  /* LCD Segment  7 enable. */

// LCDPCTL0
#define LCDS8_H             (0x0001u)  /* LCD Segment  8 enable. */
#define LCDS9_H             (0x0002u)  /* LCD Segment  9 enable. */
#define LCDS10_H            (0x0004u)  /* LCD Segment 10 enable. */
#define LCDS11_H            (0x0008u)  /* LCD Segment 11 enable. */
#define LCDS12_H            (0x0010u)  /* LCD Segment 12 enable. */
#define LCDS13_H            (0x0020u)  /* LCD Segment 13 enable. */
#define LCDS14_H            (0x0040u)  /* LCD Segment 14 enable. */
#define LCDS15_H            (0x0080u)  /* LCD Segment 15 enable. */

// LCDPCTL1
#define LCDS16              (0x0001u)  /* LCD Segment 16 enable. */
#define LCDS17              (0x0002u)  /* LCD Segment 17 enable. */
#define LCDS18              (0x0004u)  /* LCD Segment 18 enable. */
#define LCDS19              (0x0008u)  /* LCD Segment 19 enable. */
#define LCDS20              (0x0010u)  /* LCD Segment 20 enable. */
#define LCDS21              (0x0020u)  /* LCD Segment 21 enable. */
#define LCDS22              (0x0040u)  /* LCD Segment 22 enable. */
#define LCDS23              (0x0080u)  /* LCD Segment 23 enable. */
#define LCDS24              (0x0100u)  /* LCD Segment 24 enable. */
#define LCDS25              (0x0200u)  /* LCD Segment 25 enable. */
#define LCDS26              (0x0400u)  /* LCD Segment 26 enable. */
#define LCDS27              (0x0800u)  /* LCD Segment 27 enable. */
#define LCDS28              (0x1000u)  /* LCD Segment 28 enable. */
#define LCDS29              (0x2000u)  /* LCD Segment 29 enable. */
#define LCDS30              (0x4000u)  /* LCD Segment 30 enable. */
#define LCDS31              (0x8000u)  /* LCD Segment 31 enable. */

// LCDPCTL1
#define LCDS16_L            (0x0001u)  /* LCD Segment 16 enable. */
#define LCDS17_L            (0x0002u)  /* LCD Segment 17 enable. */
#define LCDS18_L            (0x0004u)  /* LCD Segment 18 enable. */
#define LCDS19_L            (0x0008u)  /* LCD Segment 19 enable. */
#define LCDS20_L            (0x0010u)  /* LCD Segment 20 enable. */
#define LCDS21_L            (0x0020u)  /* LCD Segment 21 enable. */
#define LCDS22_L            (0x0040u)  /* LCD Segment 22 enable. */
#define LCDS23_L            (0x0080u)  /* LCD Segment 23 enable. */

// LCDPCTL1
#define LCDS24_H            (0x0001u)  /* LCD Segment 24 enable. */
#define LCDS25_H            (0x0002u)  /* LCD Segment 25 enable. */
#define LCDS26_H            (0x0004u)  /* LCD Segment 26 enable. */
#define LCDS27_H            (0x0008u)  /* LCD Segment 27 enable. */
#define LCDS28_H            (0x0010u)  /* LCD Segment 28 enable. */
#define LCDS29_H            (0x0020u)  /* LCD Segment 29 enable. */
#define LCDS30_H            (0x0040u)  /* LCD Segment 30 enable. */
#define LCDS31_H            (0x0080u)  /* LCD Segment 31 enable. */

// LCDPCTL2
#define LCDS32              (0x0001u)  /* LCD Segment 32 enable. */
#define LCDS33              (0x0002u)  /* LCD Segment 33 enable. */
#define LCDS34              (0x0004u)  /* LCD Segment 34 enable. */
#define LCDS35              (0x0008u)  /* LCD Segment 35 enable. */
#define LCDS36              (0x0010u)  /* LCD Segment 36 enable. */
#define LCDS37              (0x0020u)  /* LCD Segment 37 enable. */
#define LCDS38              (0x0040u)  /* LCD Segment 38 enable. */
#define LCDS39              (0x0080u)  /* LCD Segment 39 enable. */
#define LCDS40              (0x0100u)  /* LCD Segment 40 enable. */
#define LCDS41              (0x0200u)  /* LCD Segment 41 enable. */
#define LCDS42              (0x0400u)  /* LCD Segment 42 enable. */
#define LCDS43              (0x0800u)  /* LCD Segment 43 enable. */
#define LCDS44              (0x1000u)  /* LCD Segment 44 enable. */
#define LCDS45              (0x2000u)  /* LCD Segment 45 enable. */
#define LCDS46              (0x4000u)  /* LCD Segment 46 enable. */
#define LCDS47              (0x8000u)  /* LCD Segment 47 enable. */

// LCDPCTL2
#define LCDS32_L            (0x0001u)  /* LCD Segment 32 enable. */
#define LCDS33_L            (0x0002u)  /* LCD Segment 33 enable. */
#define LCDS34_L            (0x0004u)  /* LCD Segment 34 enable. */
#define LCDS35_L            (0x0008u)  /* LCD Segment 35 enable. */
#define LCDS36_L            (0x0010u)  /* LCD Segment 36 enable. */
#define LCDS37_L            (0x0020u)  /* LCD Segment 37 enable. */
#define LCDS38_L            (0x0040u)  /* LCD Segment 38 enable. */
#define LCDS39_L            (0x0080u)  /* LCD Segment 39 enable. */

// LCDPCTL2
#define LCDS40_H            (0x0001u)  /* LCD Segment 40 enable. */
#define LCDS41_H            (0x0002u)  /* LCD Segment 41 enable. */
#define LCDS42_H            (0x0004u)  /* LCD Segment 42 enable. */
#define LCDS43_H            (0x0008u)  /* LCD Segment 43 enable. */
#define LCDS44_H            (0x0010u)  /* LCD Segment 44 enable. */
#define LCDS45_H            (0x0020u)  /* LCD Segment 45 enable. */
#define LCDS46_H            (0x0040u)  /* LCD Segment 46 enable. */
#define LCDS47_H            (0x0080u)  /* LCD Segment 47 enable. */

// LCDCSSEL0
#define LCDCSS0             (0x0001u)  /* Selects pin L0  as either common or segment line */
#define LCDCSS1             (0x0002u)  /* Selects pin L1  as either common or segment line */
#define LCDCSS2             (0x0004u)  /* Selects pin L2  as either common or segment line */
#define LCDCSS3             (0x0008u)  /* Selects pin L3  as either common or segment line */
#define LCDCSS4             (0x0010u)  /* Selects pin L4  as either common or segment line */
#define LCDCSS5             (0x0020u)  /* Selects pin L5  as either common or segment line */
#define LCDCSS6             (0x0040u)  /* Selects pin L6  as either common or segment line */
#define LCDCSS7             (0x0080u)  /* Selects pin L7  as either common or segment line */
#define LCDCSS8             (0x0100u)  /* Selects pin L8  as either common or segment line */
#define LCDCSS9             (0x0200u)  /* Selects pin L9  as either common or segment line */
#define LCDCSS10            (0x0400u)  /* Selects pin L10 as either common or segment line */
#define LCDCSS11            (0x0800u)  /* Selects pin L11 as either common or segment line */
#define LCDCSS12            (0x1000u)  /* Selects pin L12 as either common or segment line */
#define LCDCSS13            (0x2000u)  /* Selects pin L13 as either common or segment line */
#define LCDCSS14            (0x4000u)  /* Selects pin L14 as either common or segment line */
#define LCDCSS15            (0x8000u)  /* Selects pin L15 as either common or segment line */

// LCDCSSEL0
#define LCDCSS0_L           (0x0001u)  /* Selects pin L0  as either common or segment line */
#define LCDCSS1_L           (0x0002u)  /* Selects pin L1  as either common or segment line */
#define LCDCSS2_L           (0x0004u)  /* Selects pin L2  as either common or segment line */
#define LCDCSS3_L           (0x0008u)  /* Selects pin L3  as either common or segment line */
#define LCDCSS4_L           (0x0010u)  /* Selects pin L4  as either common or segment line */
#define LCDCSS5_L           (0x0020u)  /* Selects pin L5  as either common or segment line */
#define LCDCSS6_L           (0x0040u)  /* Selects pin L6  as either common or segment line */
#define LCDCSS7_L           (0x0080u)  /* Selects pin L7  as either common or segment line */

// LCDCSSEL0
#define LCDCSS8_H           (0x0001u)  /* Selects pin L8  as either common or segment line */
#define LCDCSS9_H           (0x0002u)  /* Selects pin L9  as either common or segment line */
#define LCDCSS10_H          (0x0004u)  /* Selects pin L10 as either common or segment line */
#define LCDCSS11_H          (0x0008u)  /* Selects pin L11 as either common or segment line */
#define LCDCSS12_H          (0x0010u)  /* Selects pin L12 as either common or segment line */
#define LCDCSS13_H          (0x0020u)  /* Selects pin L13 as either common or segment line */
#define LCDCSS14_H          (0x0040u)  /* Selects pin L14 as either common or segment line */
#define LCDCSS15_H          (0x0080u)  /* Selects pin L15 as either common or segment line */

// LCDCSSEL1
#define LCDCSS16            (0x0001u)  /* Selects pin L16 as either common or segment line */
#define LCDCSS17            (0x0002u)  /* Selects pin L17 as either common or segment line */
#define LCDCSS18            (0x0004u)  /* Selects pin L18 as either common or segment line */
#define LCDCSS19            (0x0008u)  /* Selects pin L19 as either common or segment line */
#define LCDCSS20            (0x0010u)  /* Selects pin L20 as either common or segment line */
#define LCDCSS21            (0x0020u)  /* Selects pin L21 as either common or segment line */
#define LCDCSS22            (0x0040u)  /* Selects pin L22 as either common or segment line */
#define LCDCSS23            (0x0080u)  /* Selects pin L23 as either common or segment line */
#define LCDCSS24            (0x0100u)  /* Selects pin L24 as either common or segment line */
#define LCDCSS25            (0x0200u)  /* Selects pin L25 as either common or segment line */
#define LCDCSS26            (0x0400u)  /* Selects pin L26 as either common or segment line */
#define LCDCSS27            (0x0800u)  /* Selects pin L27 as either common or segment line */
#define LCDCSS28            (0x1000u)  /* Selects pin L28 as either common or segment line */
#define LCDCSS29            (0x2000u)  /* Selects pin L29 as either common or segment line */
#define LCDCSS30            (0x4000u)  /* Selects pin L30 as either common or segment line */
#define LCDCSS31            (0x8000u)  /* Selects pin L31 as either common or segment line */

// LCDCSSEL1
#define LCDCSS16_L          (0x0001u)  /* Selects pin L16 as either common or segment line */
#define LCDCSS17_L          (0x0002u)  /* Selects pin L17 as either common or segment line */
#define LCDCSS18_L          (0x0004u)  /* Selects pin L18 as either common or segment line */
#define LCDCSS19_L          (0x0008u)  /* Selects pin L19 as either common or segment line */
#define LCDCSS20_L          (0x0010u)  /* Selects pin L20 as either common or segment line */
#define LCDCSS21_L          (0x0020u)  /* Selects pin L21 as either common or segment line */
#define LCDCSS22_L          (0x0040u)  /* Selects pin L22 as either common or segment line */
#define LCDCSS23_L          (0x0080u)  /* Selects pin L23 as either common or segment line */

// LCDCSSEL1
#define LCDCSS24_H          (0x0001u)  /* Selects pin L24 as either common or segment line */
#define LCDCSS25_H          (0x0002u)  /* Selects pin L25 as either common or segment line */
#define LCDCSS26_H          (0x0004u)  /* Selects pin L26 as either common or segment line */
#define LCDCSS27_H          (0x0008u)  /* Selects pin L27 as either common or segment line */
#define LCDCSS28_H          (0x0010u)  /* Selects pin L28 as either common or segment line */
#define LCDCSS29_H          (0x0020u)  /* Selects pin L29 as either common or segment line */
#define LCDCSS30_H          (0x0040u)  /* Selects pin L30 as either common or segment line */
#define LCDCSS31_H          (0x0080u)  /* Selects pin L31 as either common or segment line */

// LCDCSSEL2
#define LCDCSS32            (0x0001u)  /* Selects pin L32 as either common or segment line */
#define LCDCSS33            (0x0002u)  /* Selects pin L33 as either common or segment line */
#define LCDCSS34            (0x0004u)  /* Selects pin L34 as either common or segment line */
#define LCDCSS35            (0x0008u)  /* Selects pin L35 as either common or segment line */
#define LCDCSS36            (0x0010u)  /* Selects pin L36 as either common or segment line */
#define LCDCSS37            (0x0020u)  /* Selects pin L37 as either common or segment line */
#define LCDCSS38            (0x0040u)  /* Selects pin L38 as either common or segment line */
#define LCDCSS39            (0x0080u)  /* Selects pin L39 as either common or segment line */
#define LCDCSS40            (0x0100u)  /* Selects pin L40 as either common or segment line */
#define LCDCSS41            (0x0200u)  /* Selects pin L41 as either common or segment line */
#define LCDCSS42            (0x0400u)  /* Selects pin L42 as either common or segment line */
#define LCDCSS43            (0x0800u)  /* Selects pin L43 as either common or segment line */
#define LCDCSS44            (0x1000u)  /* Selects pin L44 as either common or segment line */
#define LCDCSS45            (0x2000u)  /* Selects pin L45 as either common or segment line */
#define LCDCSS46            (0x4000u)  /* Selects pin L46 as either common or segment line */
#define LCDCSS47            (0x8000u)  /* Selects pin L47 as either common or segment line */

// LCDCSSEL2
#define LCDCSS32_L          (0x0001u)  /* Selects pin L32 as either common or segment line */
#define LCDCSS33_L          (0x0002u)  /* Selects pin L33 as either common or segment line */
#define LCDCSS34_L          (0x0004u)  /* Selects pin L34 as either common or segment line */
#define LCDCSS35_L          (0x0008u)  /* Selects pin L35 as either common or segment line */
#define LCDCSS36_L          (0x0010u)  /* Selects pin L36 as either common or segment line */
#define LCDCSS37_L          (0x0020u)  /* Selects pin L37 as either common or segment line */
#define LCDCSS38_L          (0x0040u)  /* Selects pin L38 as either common or segment line */
#define LCDCSS39_L          (0x0080u)  /* Selects pin L39 as either common or segment line */

// LCDCSSEL2
#define LCDCSS40_H          (0x0001u)  /* Selects pin L40 as either common or segment line */
#define LCDCSS41_H          (0x0002u)  /* Selects pin L41 as either common or segment line */
#define LCDCSS42_H          (0x0004u)  /* Selects pin L42 as either common or segment line */
#define LCDCSS43_H          (0x0008u)  /* Selects pin L43 as either common or segment line */
#define LCDCSS44_H          (0x0010u)  /* Selects pin L44 as either common or segment line */
#define LCDCSS45_H          (0x0020u)  /* Selects pin L45 as either common or segment line */
#define LCDCSS46_H          (0x0040u)  /* Selects pin L46 as either common or segment line */
#define LCDCSS47_H          (0x0080u)  /* Selects pin L47 as either common or segment line */

#define OFS_LCDM0W            (0x0020u)  /* LCD Memory 0/1 */
#define OFS_LCDM0W_L           OFS_LCDM0W
#define OFS_LCDM0W_H           OFS_LCDM0W+1
#define LCDM0               LCDM0W_L  /* LCD Memory 0 */
#define LCDM1               LCDM0W_H  /* LCD Memory 1 */
#define LCDMEM_             LCDM0     /* LCD Memory */
#ifndef __IAR_SYSTEMS_ICC__
#define LCDMEM              LCDM0     /* LCD Memory (for assembler) */
#else
#define LCDMEM              ((char*) &LCDM0) /* LCD Memory (for C) */
#endif
#define OFS_LCDM2W            (0x0022u)  /* LCD Memory 2/3 */
#define OFS_LCDM2W_L           OFS_LCDM2W
#define OFS_LCDM2W_H           OFS_LCDM2W+1
#define LCDM2               LCDM2W_L  /* LCD Memory 2 */
#define LCDM3               LCDM2W_H  /* LCD Memory 3 */
#define OFS_LCDM4W            (0x0024u)  /* LCD Memory 4/5 */
#define OFS_LCDM4W_L           OFS_LCDM4W
#define OFS_LCDM4W_H           OFS_LCDM4W+1
#define LCDM4               LCDM4W_L  /* LCD Memory 4 */
#define LCDM5               LCDM4W_H  /* LCD Memory 5 */
#define OFS_LCDM6W            (0x0026u)  /* LCD Memory 6/7 */
#define OFS_LCDM6W_L           OFS_LCDM6W
#define OFS_LCDM6W_H           OFS_LCDM6W+1
#define LCDM6               LCDM6W_L  /* LCD Memory 6 */
#define LCDM7               LCDM6W_H  /* LCD Memory 7 */
#define OFS_LCDM8W            (0x0028u)  /* LCD Memory 8/9 */
#define OFS_LCDM8W_L           OFS_LCDM8W
#define OFS_LCDM8W_H           OFS_LCDM8W+1
#define LCDM8               LCDM8W_L  /* LCD Memory 8 */
#define LCDM9               LCDM8W_H  /* LCD Memory 9 */
#define OFS_LCDM10W           (0x002Au)  /* LCD Memory 10/11 */
#define OFS_LCDM10W_L          OFS_LCDM10W
#define OFS_LCDM10W_H          OFS_LCDM10W+1
#define LCDM10              LCDM10W_L /* LCD Memory 10 */
#define LCDM11              LCDM10W_H /* LCD Memory 11 */
#define OFS_LCDM12W           (0x002Cu)  /* LCD Memory 12/13 */
#define OFS_LCDM12W_L          OFS_LCDM12W
#define OFS_LCDM12W_H          OFS_LCDM12W+1
#define LCDM12              LCDM12W_L /* LCD Memory 12 */
#define LCDM13              LCDM12W_H /* LCD Memory 13 */
#define OFS_LCDM14W           (0x002Eu)  /* LCD Memory 14/15 */
#define OFS_LCDM14W_L          OFS_LCDM14W
#define OFS_LCDM14W_H          OFS_LCDM14W+1
#define LCDM14              LCDM14W_L /* LCD Memory 14 */
#define LCDM15              LCDM14W_H /* LCD Memory 15 */
#define OFS_LCDM16W           (0x0030u)  /* LCD Memory 16/17 */
#define OFS_LCDM16W_L          OFS_LCDM16W
#define OFS_LCDM16W_H          OFS_LCDM16W+1
#define LCDM16              LCDM16W_L /* LCD Memory 16 */
#define LCDM17              LCDM16W_H /* LCD Memory 17 */
#define OFS_LCDM18W           (0x0032u)  /* LCD Memory 18/19 */
#define OFS_LCDM18W_L          OFS_LCDM18W
#define OFS_LCDM18W_H          OFS_LCDM18W+1
#define LCDM18              LCDM18W_L /* LCD Memory 18 */
#define LCDM19              LCDM18W_H /* LCD Memory 19 */
#define OFS_LCDM20W           (0x0034u)  /* LCD Memory 20/21 */
#define OFS_LCDM20W_L          OFS_LCDM20W
#define OFS_LCDM20W_H          OFS_LCDM20W+1
#define LCDM20              LCDM20W_L /* LCD Memory 20 */
#define LCDM21              LCDM20W_H /* LCD Memory 21 */
#define OFS_LCDM22W           (0x0036u)  /* LCD Memory 22/23 */
#define OFS_LCDM22W_L          OFS_LCDM22W
#define OFS_LCDM22W_H          OFS_LCDM22W+1
#define LCDM22              LCDM22W_L /* LCD Memory 22 */
#define LCDM23              LCDM22W_H /* LCD Memory 23 */
#define OFS_LCDM24W           (0x0038u)  /* LCD Memory 24/25 */
#define OFS_LCDM24W_L          OFS_LCDM24W
#define OFS_LCDM24W_H          OFS_LCDM24W+1
#define LCDM24              LCDM24W_L /* LCD Memory 24 */
#define LCDM25              LCDM24W_H /* LCD Memory 25 */
#define OFS_LCDM26W           (0x003Au)  /* LCD Memory 26/27 */
#define OFS_LCDM26W_L          OFS_LCDM26W
#define OFS_LCDM26W_H          OFS_LCDM26W+1
#define LCDM26              LCDM26W_L /* LCD Memory 26 */
#define LCDM27              LCDM26W_H /* LCD Memory 27 */
#define OFS_LCDM28W           (0x003Cu)  /* LCD Memory 28/29 */
#define OFS_LCDM28W_L          OFS_LCDM28W
#define OFS_LCDM28W_H          OFS_LCDM28W+1
#define LCDM28              LCDM28W_L /* LCD Memory 28 */
#define LCDM29              LCDM28W_H /* LCD Memory 29 */
#define OFS_LCDM30W           (0x003Eu)  /* LCD Memory 30/31 */
#define OFS_LCDM30W_L          OFS_LCDM30W
#define OFS_LCDM30W_H          OFS_LCDM30W+1
#define LCDM30              LCDM30W_L /* LCD Memory 30 */
#define LCDM31              LCDM30W_H /* LCD Memory 31 */
#define OFS_LCDM32W           (0x0040u)  /* LCD Memory 32/33 */
#define OFS_LCDM32W_L          OFS_LCDM32W
#define OFS_LCDM32W_H          OFS_LCDM32W+1
#define LCDM32              LCDM32W_L /* LCD Memory 32 */
#define LCDM33              LCDM32W_H /* LCD Memory 33 */
#define OFS_LCDM34W           (0x0042u)  /* LCD Memory 34/35 */
#define OFS_LCDM34W_L          OFS_LCDM34W
#define OFS_LCDM34W_H          OFS_LCDM34W+1
#define LCDM34              LCDM34W_L /* LCD Memory 34 */
#define LCDM35              LCDM34W_H /* LCD Memory 35 */
#define OFS_LCDM36W           (0x0044u)  /* LCD Memory 36/37 */
#define OFS_LCDM36W_L          OFS_LCDM36W
#define OFS_LCDM36W_H          OFS_LCDM36W+1
#define LCDM36              LCDM36W_L /* LCD Memory 36 */
#define LCDM37              LCDM36W_H /* LCD Memory 37 */
#define OFS_LCDM38W           (0x0046u)  /* LCD Memory 38/39 */
#define OFS_LCDM38W_L          OFS_LCDM38W
#define OFS_LCDM38W_H          OFS_LCDM38W+1
#define LCDM38              LCDM38W_L /* LCD Memory 38 */
#define LCDM39              LCDM38W_H /* LCD Memory 39 */

#define OFS_LCDBM0W           (0x0040u)  /* LCD Blinking Memory 0/1 */
#define OFS_LCDBM0W_L          OFS_LCDBM0W
#define OFS_LCDBM0W_H          OFS_LCDBM0W+1
#define LCDBM0              LCDBM0W_L /* LCD Blinking Memory 0 */
#define LCDBM1              LCDBM0W_H /* LCD Blinking Memory 1 */
#define LCDBMEM_            LCDBM0    /* LCD Blinking Memory */
#ifndef __IAR_SYSTEMS_ICC__
#define LCDBMEM             LCDBM0    /* LCD Blinking Memory (for assembler) */
#else
#define LCDBMEM             ((char*) &LCDBM0) /* LCD Blinking Memory (for C) */
#endif
#define OFS_LCDBM2W           (0x0042u)  /* LCD Blinking Memory 2/3 */
#define OFS_LCDBM2W_L          OFS_LCDBM2W
#define OFS_LCDBM2W_H          OFS_LCDBM2W+1
#define LCDBM2              LCDBM2W_L  /* LCD Blinking Memory 2 */
#define LCDBM3              LCDBM2W_H  /* LCD Blinking Memory 3 */
#define OFS_LCDBM4W           (0x0044u)  /* LCD Blinking Memory 4/5 */
#define OFS_LCDBM4W_L          OFS_LCDBM4W
#define OFS_LCDBM4W_H          OFS_LCDBM4W+1
#define LCDBM4              LCDBM4W_L  /* LCD Blinking Memory 4 */
#define LCDBM5              LCDBM4W_H  /* LCD Blinking Memory 5 */
#define OFS_LCDBM6W           (0x0046u)  /* LCD Blinking Memory 6/7 */
#define OFS_LCDBM6W_L          OFS_LCDBM6W
#define OFS_LCDBM6W_H          OFS_LCDBM6W+1
#define LCDBM6              LCDBM6W_L  /* LCD Blinking Memory 6 */
#define LCDBM7              LCDBM6W_H  /* LCD Blinking Memory 7 */
#define OFS_LCDBM8W           (0x0048u)  /* LCD Blinking Memory 8/9 */
#define OFS_LCDBM8W_L          OFS_LCDBM8W
#define OFS_LCDBM8W_H          OFS_LCDBM8W+1
#define LCDBM8              LCDBM8W_L  /* LCD Blinking Memory 8 */
#define LCDBM9              LCDBM8W_H  /* LCD Blinking Memory 9 */
#define OFS_LCDBM10W          (0x004Au)  /* LCD Blinking Memory 10/11 */
#define OFS_LCDBM10W_L         OFS_LCDBM10W
#define OFS_LCDBM10W_H         OFS_LCDBM10W+1
#define LCDBM10             LCDBM10W_L /* LCD Blinking Memory 10 */
#define LCDBM11             LCDBM10W_H /* LCD Blinking Memory 11 */
#define OFS_LCDBM12W          (0x004Cu)  /* LCD Blinking Memory 12/13 */
#define OFS_LCDBM12W_L         OFS_LCDBM12W
#define OFS_LCDBM12W_H         OFS_LCDBM12W+1
#define LCDBM12             LCDBM12W_L /* LCD Blinking Memory 12 */
#define LCDBM13             LCDBM12W_H /* LCD Blinking Memory 13 */
#define OFS_LCDBM14W          (0x004Eu)  /* LCD Blinking Memory 14/15 */
#define OFS_LCDBM14W_L         OFS_LCDBM14W
#define OFS_LCDBM14W_H         OFS_LCDBM14W+1
#define LCDBM14             LCDBM14W_L /* LCD Blinking Memory 14 */
#define LCDBM15             LCDBM14W_H /* LCD Blinking Memory 15 */
#define OFS_LCDBM16W          (0x0050u)  /* LCD Blinking Memory 16/17 */
#define OFS_LCDBM16W_L         OFS_LCDBM16W
#define OFS_LCDBM16W_H         OFS_LCDBM16W+1
#define LCDBM16             LCDBM16W_L /* LCD Blinking Memory 16 */
#define LCDBM17             LCDBM16W_H /* LCD Blinking Memory 17 */
#define OFS_LCDBM18W          (0x0052u)  /* LCD Blinking Memory 18/19 */
#define OFS_LCDBM18W_L         OFS_LCDBM18W
#define OFS_LCDBM18W_H         OFS_LCDBM18W+1
#define LCDBM18             LCDBM18W_L /* LCD Blinking Memory 18 */
#define LCDBM19             LCDBM18W_H /* LCD Blinking Memory 19 */

/* LCDIV Definitions */
#define LCDIV_NONE         (0x0000u)    /* No Interrupt pending */
#define LCDIV_LCDBLKOFFIFG (0x0004u)    /* Blink, segments off */
#define LCDIV_LCDBLKONIFG  (0x0006u)    /* Blink, segments on */
#define LCDIV_LCDFRMIFG    (0x0008u)    /* Frame interrupt */

#endif
/************************************************************
* Memory Protection Unit
************************************************************/
#ifdef  __MSP430_HAS_MPU__             /* Definition to show that Module is available */

#define OFS_MPUCTL0           (0x0000u)  /* MPU Control Register 0 */
#define OFS_MPUCTL0_L          OFS_MPUCTL0
#define OFS_MPUCTL0_H          OFS_MPUCTL0+1
#define OFS_MPUCTL1           (0x0002u)  /* MPU Control Register 1 */
#define OFS_MPUCTL1_L          OFS_MPUCTL1
#define OFS_MPUCTL1_H          OFS_MPUCTL1+1
#define OFS_MPUSEGB2          (0x0004u)  /* MPU Segmentation Border 2 Register */
#define OFS_MPUSEGB2_L         OFS_MPUSEGB2
#define OFS_MPUSEGB2_H         OFS_MPUSEGB2+1
#define OFS_MPUSEGB1          (0x0006u)  /* MPU Segmentation Border 1 Register */
#define OFS_MPUSEGB1_L         OFS_MPUSEGB1
#define OFS_MPUSEGB1_H         OFS_MPUSEGB1+1
#define OFS_MPUSAM            (0x0008u)  /* MPU Access Management Register */
#define OFS_MPUSAM_L           OFS_MPUSAM
#define OFS_MPUSAM_H           OFS_MPUSAM+1
#define OFS_MPUIPC0           (0x000Au)  /* MPU IP Control 0 Register */
#define OFS_MPUIPC0_L          OFS_MPUIPC0
#define OFS_MPUIPC0_H          OFS_MPUIPC0+1
#define OFS_MPUIPSEGB2        (0x000Cu)  /* MPU IP Segment Border 2 Register */
#define OFS_MPUIPSEGB2_L       OFS_MPUIPSEGB2
#define OFS_MPUIPSEGB2_H       OFS_MPUIPSEGB2+1
#define OFS_MPUIPSEGB1        (0x000Eu)  /* MPU IP Segment Border 1 Register */
#define OFS_MPUIPSEGB1_L       OFS_MPUIPSEGB1
#define OFS_MPUIPSEGB1_H       OFS_MPUIPSEGB1+1

/* MPUCTL0 Control Bits */
#define MPUENA               (0x0001u)  /* MPU Enable */
#define MPULOCK              (0x0002u)  /* MPU Lock */
#define MPUSEGIE             (0x0010u)  /* MPU Enable NMI on Segment violation */

/* MPUCTL0 Control Bits */
#define MPUENA_L            (0x0001u)  /* MPU Enable */
#define MPULOCK_L           (0x0002u)  /* MPU Lock */
#define MPUSEGIE_L          (0x0010u)  /* MPU Enable NMI on Segment violation */

#define MPUPW                (0xA500u)  /* MPU Access Password */
#define MPUPW_H              (0xA5)    /* MPU Access Password */

/* MPUCTL1 Control Bits */
#define MPUSEG1IFG           (0x0001u)  /* MPU Main Memory Segment 1 violation interupt flag */
#define MPUSEG2IFG           (0x0002u)  /* MPU Main Memory Segment 2 violation interupt flag */
#define MPUSEG3IFG           (0x0004u)  /* MPU Main Memory Segment 3 violation interupt flag */
#define MPUSEGIIFG           (0x0008u)  /* MPU Info Memory Segment violation interupt flag */
#define MPUSEGIPIFG          (0x0010u)  /* MPU IP Memory Segment violation interupt flag */

/* MPUCTL1 Control Bits */
#define MPUSEG1IFG_L        (0x0001u)  /* MPU Main Memory Segment 1 violation interupt flag */
#define MPUSEG2IFG_L        (0x0002u)  /* MPU Main Memory Segment 2 violation interupt flag */
#define MPUSEG3IFG_L        (0x0004u)  /* MPU Main Memory Segment 3 violation interupt flag */
#define MPUSEGIIFG_L        (0x0008u)  /* MPU Info Memory Segment violation interupt flag */
#define MPUSEGIPIFG_L       (0x0010u)  /* MPU IP Memory Segment violation interupt flag */

/* MPUSEGB2 Control Bits */

/* MPUSEGB2 Control Bits */

/* MPUSEGB2 Control Bits */

/* MPUSEGB1 Control Bits */

/* MPUSEGB1 Control Bits */

/* MPUSEGB1 Control Bits */

/* MPUSAM Control Bits */
#define MPUSEG1RE            (0x0001u)  /* MPU Main memory Segment 1 Read enable */
#define MPUSEG1WE            (0x0002u)  /* MPU Main memory Segment 1 Write enable */
#define MPUSEG1XE            (0x0004u)  /* MPU Main memory Segment 1 Execute enable */
#define MPUSEG1VS            (0x0008u)  /* MPU Main memory Segment 1 Violation select */
#define MPUSEG2RE            (0x0010u)  /* MPU Main memory Segment 2 Read enable */
#define MPUSEG2WE            (0x0020u)  /* MPU Main memory Segment 2 Write enable */
#define MPUSEG2XE            (0x0040u)  /* MPU Main memory Segment 2 Execute enable */
#define MPUSEG2VS            (0x0080u)  /* MPU Main memory Segment 2 Violation select */
#define MPUSEG3RE            (0x0100u)  /* MPU Main memory Segment 3 Read enable */
#define MPUSEG3WE            (0x0200u)  /* MPU Main memory Segment 3 Write enable */
#define MPUSEG3XE            (0x0400u)  /* MPU Main memory Segment 3 Execute enable */
#define MPUSEG3VS            (0x0800u)  /* MPU Main memory Segment 3 Violation select */
#define MPUSEGIRE            (0x1000u)  /* MPU Info memory Segment Read enable */
#define MPUSEGIWE            (0x2000u)  /* MPU Info memory Segment Write enable */
#define MPUSEGIXE            (0x4000u)  /* MPU Info memory Segment Execute enable */
#define MPUSEGIVS            (0x8000u)  /* MPU Info memory Segment Violation select */

/* MPUSAM Control Bits */
#define MPUSEG1RE_L         (0x0001u)  /* MPU Main memory Segment 1 Read enable */
#define MPUSEG1WE_L         (0x0002u)  /* MPU Main memory Segment 1 Write enable */
#define MPUSEG1XE_L         (0x0004u)  /* MPU Main memory Segment 1 Execute enable */
#define MPUSEG1VS_L         (0x0008u)  /* MPU Main memory Segment 1 Violation select */
#define MPUSEG2RE_L         (0x0010u)  /* MPU Main memory Segment 2 Read enable */
#define MPUSEG2WE_L         (0x0020u)  /* MPU Main memory Segment 2 Write enable */
#define MPUSEG2XE_L         (0x0040u)  /* MPU Main memory Segment 2 Execute enable */
#define MPUSEG2VS_L         (0x0080u)  /* MPU Main memory Segment 2 Violation select */

/* MPUSAM Control Bits */
#define MPUSEG3RE_H         (0x0001u)  /* MPU Main memory Segment 3 Read enable */
#define MPUSEG3WE_H         (0x0002u)  /* MPU Main memory Segment 3 Write enable */
#define MPUSEG3XE_H         (0x0004u)  /* MPU Main memory Segment 3 Execute enable */
#define MPUSEG3VS_H         (0x0008u)  /* MPU Main memory Segment 3 Violation select */
#define MPUSEGIRE_H         (0x0010u)  /* MPU Info memory Segment Read enable */
#define MPUSEGIWE_H         (0x0020u)  /* MPU Info memory Segment Write enable */
#define MPUSEGIXE_H         (0x0040u)  /* MPU Info memory Segment Execute enable */
#define MPUSEGIVS_H         (0x0080u)  /* MPU Info memory Segment Violation select */

/* MPUIPC0 Control Bits */
#define MPUIPVS              (0x0020u)  /* MPU MPU IP protection segment Violation Select */
#define MPUIPENA             (0x0040u)  /* MPU MPU IP Protection Enable */
#define MPUIPLOCK            (0x0080u)  /* MPU IP Protection Lock */

/* MPUIPC0 Control Bits */
#define MPUIPVS_L           (0x0020u)  /* MPU MPU IP protection segment Violation Select */
#define MPUIPENA_L          (0x0040u)  /* MPU MPU IP Protection Enable */
#define MPUIPLOCK_L         (0x0080u)  /* MPU IP Protection Lock */

/* MPUIPSEGB2 Control Bits */

/* MPUIPSEGB2 Control Bits */

/* MPUIPSEGB2 Control Bits */

/* MPUIPSEGB1 Control Bits */

/* MPUIPSEGB1 Control Bits */

/* MPUIPSEGB1 Control Bits */

#endif
/************************************************************
* HARDWARE MULTIPLIER 32Bit
************************************************************/
#ifdef  __MSP430_HAS_MPY32__          /* Definition to show that Module is available */

#define OFS_MPY               (0x0000u)  /* Multiply Unsigned/Operand 1 */
#define OFS_MPY_L              OFS_MPY
#define OFS_MPY_H              OFS_MPY+1
#define OFS_MPYS              (0x0002u)  /* Multiply Signed/Operand 1 */
#define OFS_MPYS_L             OFS_MPYS
#define OFS_MPYS_H             OFS_MPYS+1
#define OFS_MAC               (0x0004u)  /* Multiply Unsigned and Accumulate/Operand 1 */
#define OFS_MAC_L              OFS_MAC
#define OFS_MAC_H              OFS_MAC+1
#define OFS_MACS              (0x0006u)  /* Multiply Signed and Accumulate/Operand 1 */
#define OFS_MACS_L             OFS_MACS
#define OFS_MACS_H             OFS_MACS+1
#define OFS_OP2               (0x0008u)  /* Operand 2 */
#define OFS_OP2_L              OFS_OP2
#define OFS_OP2_H              OFS_OP2+1
#define OFS_RESLO             (0x000Au)  /* Result Low Word */
#define OFS_RESLO_L            OFS_RESLO
#define OFS_RESLO_H            OFS_RESLO+1
#define OFS_RESHI             (0x000Cu)  /* Result High Word */
#define OFS_RESHI_L            OFS_RESHI
#define OFS_RESHI_H            OFS_RESHI+1
#define OFS_SUMEXT            (0x000Eu)  /* Sum Extend */
#define OFS_SUMEXT_L           OFS_SUMEXT
#define OFS_SUMEXT_H           OFS_SUMEXT+1
#define OFS_MPY32CTL0         (0x002Cu)
#define OFS_MPY32CTL0_L        OFS_MPY32CTL0
#define OFS_MPY32CTL0_H        OFS_MPY32CTL0+1

#define OFS_MPY32L            (0x0010u)  /* 32-bit operand 1 - multiply - low word */
#define OFS_MPY32L_L           OFS_MPY32L
#define OFS_MPY32L_H           OFS_MPY32L+1
#define OFS_MPY32H            (0x0012u)  /* 32-bit operand 1 - multiply - high word */
#define OFS_MPY32H_L           OFS_MPY32H
#define OFS_MPY32H_H           OFS_MPY32H+1
#define OFS_MPYS32L           (0x0014u)  /* 32-bit operand 1 - signed multiply - low word */
#define OFS_MPYS32L_L          OFS_MPYS32L
#define OFS_MPYS32L_H          OFS_MPYS32L+1
#define OFS_MPYS32H           (0x0016u)  /* 32-bit operand 1 - signed multiply - high word */
#define OFS_MPYS32H_L          OFS_MPYS32H
#define OFS_MPYS32H_H          OFS_MPYS32H+1
#define OFS_MAC32L            (0x0018u)  /* 32-bit operand 1 - multiply accumulate - low word */
#define OFS_MAC32L_L           OFS_MAC32L
#define OFS_MAC32L_H           OFS_MAC32L+1
#define OFS_MAC32H            (0x001Au)  /* 32-bit operand 1 - multiply accumulate - high word */
#define OFS_MAC32H_L           OFS_MAC32H
#define OFS_MAC32H_H           OFS_MAC32H+1
#define OFS_MACS32L           (0x001Cu)  /* 32-bit operand 1 - signed multiply accumulate - low word */
#define OFS_MACS32L_L          OFS_MACS32L
#define OFS_MACS32L_H          OFS_MACS32L+1
#define OFS_MACS32H           (0x001Eu)  /* 32-bit operand 1 - signed multiply accumulate - high word */
#define OFS_MACS32H_L          OFS_MACS32H
#define OFS_MACS32H_H          OFS_MACS32H+1
#define OFS_OP2L              (0x0020u)  /* 32-bit operand 2 - low word */
#define OFS_OP2L_L             OFS_OP2L
#define OFS_OP2L_H             OFS_OP2L+1
#define OFS_OP2H              (0x0022u)  /* 32-bit operand 2 - high word */
#define OFS_OP2H_L             OFS_OP2H
#define OFS_OP2H_H             OFS_OP2H+1
#define OFS_RES0              (0x0024u)  /* 32x32-bit result 0 - least significant word */
#define OFS_RES0_L             OFS_RES0
#define OFS_RES0_H             OFS_RES0+1
#define OFS_RES1              (0x0026u)  /* 32x32-bit result 1 */
#define OFS_RES1_L             OFS_RES1
#define OFS_RES1_H             OFS_RES1+1
#define OFS_RES2              (0x0028u)  /* 32x32-bit result 2 */
#define OFS_RES2_L             OFS_RES2
#define OFS_RES2_H             OFS_RES2+1
#define OFS_RES3              (0x002Au)  /* 32x32-bit result 3 - most significant word */
#define OFS_RES3_L             OFS_RES3
#define OFS_RES3_H             OFS_RES3+1
#define OFS_SUMEXT            (0x000Eu)
#define OFS_SUMEXT_L           OFS_SUMEXT
#define OFS_SUMEXT_H           OFS_SUMEXT+1
#define OFS_MPY32CTL0         (0x002Cu)  /* MPY32 Control Register 0 */
#define OFS_MPY32CTL0_L        OFS_MPY32CTL0
#define OFS_MPY32CTL0_H        OFS_MPY32CTL0+1

#define MPY_B               MPY_L      /* Multiply Unsigned/Operand 1 (Byte Access) */
#define MPYS_B              MPYS_L     /* Multiply Signed/Operand 1 (Byte Access) */
#define MAC_B               MAC_L      /* Multiply Unsigned and Accumulate/Operand 1 (Byte Access) */
#define MACS_B              MACS_L     /* Multiply Signed and Accumulate/Operand 1 (Byte Access) */
#define OP2_B               OP2_L      /* Operand 2 (Byte Access) */
#define MPY32L_B            MPY32L_L   /* 32-bit operand 1 - multiply - low word (Byte Access) */
#define MPY32H_B            MPY32H_L   /* 32-bit operand 1 - multiply - high word (Byte Access) */
#define MPYS32L_B           MPYS32L_L  /* 32-bit operand 1 - signed multiply - low word (Byte Access) */
#define MPYS32H_B           MPYS32H_L  /* 32-bit operand 1 - signed multiply - high word (Byte Access) */
#define MAC32L_B            MAC32L_L   /* 32-bit operand 1 - multiply accumulate - low word (Byte Access) */
#define MAC32H_B            MAC32H_L   /* 32-bit operand 1 - multiply accumulate - high word (Byte Access) */
#define MACS32L_B           MACS32L_L  /* 32-bit operand 1 - signed multiply accumulate - low word (Byte Access) */
#define MACS32H_B           MACS32H_L  /* 32-bit operand 1 - signed multiply accumulate - high word (Byte Access) */
#define OP2L_B              OP2L_L     /* 32-bit operand 2 - low word (Byte Access) */
#define OP2H_B              OP2H_L     /* 32-bit operand 2 - high word (Byte Access) */

/* MPY32CTL0 Control Bits */
#define MPYC                (0x0001u)  /* Carry of the multiplier */
//#define RESERVED            (0x0002u)  /* Reserved */
#define MPYFRAC             (0x0004u)  /* Fractional mode */
#define MPYSAT              (0x0008u)  /* Saturation mode */
#define MPYM0               (0x0010u)  /* Multiplier mode Bit:0 */
#define MPYM1               (0x0020u)  /* Multiplier mode Bit:1 */
#define OP1_32              (0x0040u)  /* Bit-width of operand 1 0:16Bit / 1:32Bit */
#define OP2_32              (0x0080u)  /* Bit-width of operand 2 0:16Bit / 1:32Bit */
#define MPYDLYWRTEN         (0x0100u)  /* Delayed write enable */
#define MPYDLY32            (0x0200u)  /* Delayed write mode */

/* MPY32CTL0 Control Bits */
#define MPYC_L              (0x0001u)  /* Carry of the multiplier */
//#define RESERVED            (0x0002u)  /* Reserved */
#define MPYFRAC_L           (0x0004u)  /* Fractional mode */
#define MPYSAT_L            (0x0008u)  /* Saturation mode */
#define MPYM0_L             (0x0010u)  /* Multiplier mode Bit:0 */
#define MPYM1_L             (0x0020u)  /* Multiplier mode Bit:1 */
#define OP1_32_L            (0x0040u)  /* Bit-width of operand 1 0:16Bit / 1:32Bit */
#define OP2_32_L            (0x0080u)  /* Bit-width of operand 2 0:16Bit / 1:32Bit */

/* MPY32CTL0 Control Bits */
//#define RESERVED            (0x0002u)  /* Reserved */
#define MPYDLYWRTEN_H       (0x0001u)  /* Delayed write enable */
#define MPYDLY32_H          (0x0002u)  /* Delayed write mode */

#define MPYM_0              (0x0000u)  /* Multiplier mode: MPY */
#define MPYM_1              (0x0010u)  /* Multiplier mode: MPYS */
#define MPYM_2              (0x0020u)  /* Multiplier mode: MAC */
#define MPYM_3              (0x0030u)  /* Multiplier mode: MACS */
#define MPYM__MPY           (0x0000u)  /* Multiplier mode: MPY */
#define MPYM__MPYS          (0x0010u)  /* Multiplier mode: MPYS */
#define MPYM__MAC           (0x0020u)  /* Multiplier mode: MAC */
#define MPYM__MACS          (0x0030u)  /* Multiplier mode: MACS */

#endif
/************************************************************
* PMM - Power Management System for FR2xx/FR4xx
************************************************************/
#ifdef  __MSP430_HAS_PMM_FRAM__       /* Definition to show that Module is available */

#define OFS_PMMCTL0           (0x0000u)  /* PMM Control 0 */
#define OFS_PMMCTL0_L          OFS_PMMCTL0
#define OFS_PMMCTL0_H          OFS_PMMCTL0+1
#define OFS_PMMCTL1           (0x0002u)  /* PMM Control 1 */
#define OFS_PMMCTL1_L          OFS_PMMCTL1
#define OFS_PMMCTL1_H          OFS_PMMCTL1+1
#define OFS_PMMCTL2           (0x0004u)  /* PMM Control 2 */
#define OFS_PMMCTL2_L          OFS_PMMCTL2
#define OFS_PMMCTL2_H          OFS_PMMCTL2+1
#define OFS_PMMIFG            (0x000Au)  /* PMM Interrupt Flag */
#define OFS_PMMIFG_L           OFS_PMMIFG
#define OFS_PMMIFG_H           OFS_PMMIFG+1
#define OFS_PMMIE             (0x000Eu)  /* PMM Interrupt Enable */
#define OFS_PMMIE_L            OFS_PMMIE
#define OFS_PMMIE_H            OFS_PMMIE+1
#define OFS_PM5CTL0           (0x0010u)  /* PMM Power Mode 5 Control Register 0 */
#define OFS_PM5CTL0_L          OFS_PM5CTL0
#define OFS_PM5CTL0_H          OFS_PM5CTL0+1

#define PMMPW               (0xA500u)  /* PMM Register Write Password */
#define PMMPW_H             (0xA5)    /* PMM Register Write Password for high word access */

/* PMMCTL0 Control Bits */
#define PMMSWBOR            (0x0004u)  /* PMM Software BOR */
#define PMMSWPOR            (0x0008u)  /* PMM Software POR */
#define PMMREGOFF           (0x0010u)  /* PMM Turn Regulator off */
#define SVSHE               (0x0040u)  /* SVS high side enable */
#define PMMLPRST            (0x0080u)  /* PMM Low-Power Reset Enable */

/* PMMCTL0 Control Bits */
#define PMMSWBOR_L          (0x0004u)  /* PMM Software BOR */
#define PMMSWPOR_L          (0x0008u)  /* PMM Software POR */
#define PMMREGOFF_L         (0x0010u)  /* PMM Turn Regulator off */
#define SVSHE_L             (0x0040u)  /* SVS high side enable */
#define PMMLPRST_L          (0x0080u)  /* PMM Low-Power Reset Enable */

/* PMMCTL1 Control Bits */

#define PMMCTL1KEY          (0xCC00u)  /* PMM PMMCTL1 Register Write Password */

/* PMMCTL2 Control Bits */
#define INTREFEN            (0x0001u)  /* Internal Reference Enable */
#define EXTREFEN            (0x0002u)  /* External Reference output Enable */
#define TSENSOREN           (0x0008u)  /* Temperature Sensor Enable */
#define REFGENACT           (0x0100u)  /* REF Reference generator active */
#define REFBGACT            (0x0200u)  /* REF Reference bandgap active */
#define BGMODE              (0x0800u)  /* REF Bandgap mode */
#define REFGENRDY           (0x1000u)  /* REF Reference generator ready */
#define REFBGRDY            (0x2000u)  /* REF Reference bandgap ready */

/* PMMCTL2 Control Bits */
#define INTREFEN_L          (0x0001u)  /* Internal Reference Enable */
#define EXTREFEN_L          (0x0002u)  /* External Reference output Enable */
#define TSENSOREN_L         (0x0008u)  /* Temperature Sensor Enable */

/* PMMCTL2 Control Bits */
#define REFGENACT_H         (0x0001u)  /* REF Reference generator active */
#define REFBGACT_H          (0x0002u)  /* REF Reference bandgap active */
#define BGMODE_H            (0x0008u)  /* REF Bandgap mode */
#define REFGENRDY_H         (0x0010u)  /* REF Reference generator ready */
#define REFBGRDY_H          (0x0020u)  /* REF Reference bandgap ready */

/* PMMIFG Control Bits */
#define PMMBORIFG           (0x0100u)  /* PMM Software BOR interrupt flag */
#define PMMRSTIFG           (0x0200u)  /* PMM RESET pin interrupt flag */
#define PMMPORIFG           (0x0400u)  /* PMM Software POR interrupt flag */
#define SVSHIFG             (0x2000u)  /* SVS low side interrupt flag */
#define PMMLPM5IFG          (0x8000u)  /* LPM5 indication Flag */

/* PMMIFG Control Bits */
#define PMMBORIFG_H         (0x0001u)  /* PMM Software BOR interrupt flag */
#define PMMRSTIFG_H         (0x0002u)  /* PMM RESET pin interrupt flag */
#define PMMPORIFG_H         (0x0004u)  /* PMM Software POR interrupt flag */
#define SVSHIFG_H           (0x0020u)  /* SVS low side interrupt flag */
#define PMMLPM5IFG_H        (0x0080u)  /* LPM5 indication Flag */

/* PMMIE Control Bits */

/* PM5CTL0 Power Mode 5 Control Bits */
#define LOCKLPM5            (0x0001u)  /* Lock I/O pin configuration upon entry/exit to/from LPM5 */
#define LPM5SW              (0x0010u)  /* LPMx.5 switch dis/connected */
#define LPM5SM              (0x0020u)  /* Manual mode for LPM3.5 switch */

/* PM5CTL0 Power Mode 5 Control Bits */
#define LOCKLPM5_L          (0x0001u)  /* Lock I/O pin configuration upon entry/exit to/from LPM5 */
#define LPM5SW_L            (0x0010u)  /* LPMx.5 switch dis/connected */
#define LPM5SM_L            (0x0020u)  /* Manual mode for LPM3.5 switch */


#endif
/************************************************************
* DIGITAL I/O Port1/2 Pull up / Pull down Resistors
************************************************************/
#ifdef  __MSP430_HAS_PORT1_R__        /* Definition to show that Module is available */
#ifdef  __MSP430_HAS_PORT2_R__        /* Definition to show that Module is available */
#ifdef  __MSP430_HAS_PORTA_R__        /* Definition to show that Module is available */

#define OFS_PAIN              (0x0000u)  /* Port A Input */
#define OFS_PAIN_L             OFS_PAIN
#define OFS_PAIN_H             OFS_PAIN+1
#define OFS_PAOUT             (0x0002u)  /* Port A Output */
#define OFS_PAOUT_L            OFS_PAOUT
#define OFS_PAOUT_H            OFS_PAOUT+1
#define OFS_PADIR             (0x0004u)  /* Port A Direction */
#define OFS_PADIR_L            OFS_PADIR
#define OFS_PADIR_H            OFS_PADIR+1
#define OFS_PAREN             (0x0006u)  /* Port A Resistor Enable */
#define OFS_PAREN_L            OFS_PAREN
#define OFS_PAREN_H            OFS_PAREN+1
#define OFS_PASEL0            (0x000Au)  /* Port A Selection 0 */
#define OFS_PASEL0_L           OFS_PASEL0
#define OFS_PASEL0_H           OFS_PASEL0+1
#define OFS_PASEL1            (0x000Cu)  /* Port A Selection 1 */
#define OFS_PASEL1_L           OFS_PASEL1
#define OFS_PASEL1_H           OFS_PASEL1+1
#define OFS_PASELC            (0x0016u)  /* Port A Complement Selection */
#define OFS_PASELC_L           OFS_PASELC
#define OFS_PASELC_H           OFS_PASELC+1
#define OFS_PAIES             (0x0018u)  /* Port A Interrupt Edge Select */
#define OFS_PAIES_L            OFS_PAIES
#define OFS_PAIES_H            OFS_PAIES+1
#define OFS_PAIE              (0x001Au)  /* Port A Interrupt Enable */
#define OFS_PAIE_L             OFS_PAIE
#define OFS_PAIE_H             OFS_PAIE+1
#define OFS_PAIFG             (0x001Cu)  /* Port A Interrupt Flag */
#define OFS_PAIFG_L            OFS_PAIFG
#define OFS_PAIFG_H            OFS_PAIFG+1


#define OFS_P1IN              (0x0000u)
#define OFS_P1OUT             (0x0002u)
#define OFS_P1DIR             (0x0004u)
#define OFS_P1REN             (0x0006u)
#define OFS_P1SEL0            (0x000Au)
#define OFS_P1SEL1            (0x000Cu)
#define OFS_P1SELC            (0x0016u)
#define OFS_P1IV              (0x000Eu)  /* Port 1 Interrupt Vector Word */
#define OFS_P1IES             (0x0018u)
#define OFS_P1IE              (0x001Au)
#define OFS_P1IFG             (0x001Cu)
#define OFS_P2IN              (0x0001u)
#define OFS_P2OUT             (0x0003u)
#define OFS_P2DIR             (0x0005u)
#define OFS_P2REN             (0x0007u)
#define OFS_P2SEL0            (0x000Bu)
#define OFS_P2SEL1            (0x000Du)
#define OFS_P2SELC            (0x0017u)
#define OFS_P2IV              (0x001Eu)  /* Port 2 Interrupt Vector Word */
#define OFS_P2IES             (0x0019u)
#define OFS_P2IE              (0x001Bu)
#define OFS_P2IFG             (0x001du)
#define P1IN                (PAIN_L)  /* Port 1 Input */
#define P1OUT               (PAOUT_L) /* Port 1 Output */
#define P1DIR               (PADIR_L) /* Port 1 Direction */
#define P1REN               (PAREN_L) /* Port 1 Resistor Enable */
#define P1SEL0              (PASEL0_L) /* Port 1 Selection 0 */
#define P1SEL1              (PASEL1_L) /* Port 1 Selection 1 */
#define P1SELC              (PASELC_L) /* Port 1 Complement Selection */
#define P1IES               (PAIES_L) /* Port 1 Interrupt Edge Select */
#define P1IE                (PAIE_L)  /* Port 1 Interrupt Enable */
#define P1IFG               (PAIFG_L) /* Port 1 Interrupt Flag */

//Definitions for P1IV
#define P1IV_NONE            (0x0000u)    /* No Interrupt pending */
#define P1IV_P1IFG0          (0x0002u)    /* P1IV P1IFG.0 */
#define P1IV_P1IFG1          (0x0004u)    /* P1IV P1IFG.1 */
#define P1IV_P1IFG2          (0x0006u)    /* P1IV P1IFG.2 */
#define P1IV_P1IFG3          (0x0008u)    /* P1IV P1IFG.3 */
#define P1IV_P1IFG4          (0x000Au)    /* P1IV P1IFG.4 */
#define P1IV_P1IFG5          (0x000Cu)    /* P1IV P1IFG.5 */
#define P1IV_P1IFG6          (0x000Eu)    /* P1IV P1IFG.6 */
#define P1IV_P1IFG7          (0x0010u)    /* P1IV P1IFG.7 */

#define P2IN                (PAIN_H)  /* Port 2 Input */
#define P2OUT               (PAOUT_H) /* Port 2 Output */
#define P2DIR               (PADIR_H) /* Port 2 Direction */
#define P2REN               (PAREN_H) /* Port 2 Resistor Enable */
#define P2SEL0              (PASEL0_H) /* Port 2 Selection 0 */
#define P2SEL1              (PASEL1_H) /* Port 2 Selection 1 */
#define P2SELC              (PASELC_H) /* Port 2 Complement Selection */
#define P2IES               (PAIES_H) /* Port 2 Interrupt Edge Select */
#define P2IE                (PAIE_H)  /* Port 2 Interrupt Enable */
#define P2IFG               (PAIFG_H) /* Port 2 Interrupt Flag */

//Definitions for P2IV
#define P2IV_NONE            (0x0000u)    /* No Interrupt pending */
#define P2IV_P2IFG0          (0x0002u)    /* P2IV P2IFG.0 */
#define P2IV_P2IFG1          (0x0004u)    /* P2IV P2IFG.1 */
#define P2IV_P2IFG2          (0x0006u)    /* P2IV P2IFG.2 */
#define P2IV_P2IFG3          (0x0008u)    /* P2IV P2IFG.3 */
#define P2IV_P2IFG4          (0x000Au)    /* P2IV P2IFG.4 */
#define P2IV_P2IFG5          (0x000Cu)    /* P2IV P2IFG.5 */
#define P2IV_P2IFG6          (0x000Eu)    /* P2IV P2IFG.6 */
#define P2IV_P2IFG7          (0x0010u)    /* P2IV P2IFG.7 */


#endif
#endif
#endif
/************************************************************
* DIGITAL I/O Port3/4 Pull up / Pull down Resistors
************************************************************/
#ifdef  __MSP430_HAS_PORT3_R__        /* Definition to show that Module is available */
#ifdef  __MSP430_HAS_PORT4_R__        /* Definition to show that Module is available */
#ifdef  __MSP430_HAS_PORTB_R__        /* Definition to show that Module is available */

#define OFS_PBIN              (0x0000u)  /* Port B Input */
#define OFS_PBIN_L             OFS_PBIN
#define OFS_PBIN_H             OFS_PBIN+1
#define OFS_PBOUT             (0x0002u)  /* Port B Output */
#define OFS_PBOUT_L            OFS_PBOUT
#define OFS_PBOUT_H            OFS_PBOUT+1
#define OFS_PBDIR             (0x0004u)  /* Port B Direction */
#define OFS_PBDIR_L            OFS_PBDIR
#define OFS_PBDIR_H            OFS_PBDIR+1
#define OFS_PBREN             (0x0006u)  /* Port B Resistor Enable */
#define OFS_PBREN_L            OFS_PBREN
#define OFS_PBREN_H            OFS_PBREN+1
#define OFS_PBSEL0            (0x000Au)  /* Port B Selection 0 */
#define OFS_PBSEL0_L           OFS_PBSEL0
#define OFS_PBSEL0_H           OFS_PBSEL0+1
#define OFS_PBSEL1            (0x000Cu)  /* Port B Selection 1 */
#define OFS_PBSEL1_L           OFS_PBSEL1
#define OFS_PBSEL1_H           OFS_PBSEL1+1
#define OFS_PBSELC            (0x0016u)  /* Port B Complement Selection */
#define OFS_PBSELC_L           OFS_PBSELC
#define OFS_PBSELC_H           OFS_PBSELC+1
#define OFS_PBIES             (0x0018u)  /* Port B Interrupt Edge Select */
#define OFS_PBIES_L            OFS_PBIES
#define OFS_PBIES_H            OFS_PBIES+1
#define OFS_PBIE              (0x001Au)  /* Port B Interrupt Enable */
#define OFS_PBIE_L             OFS_PBIE
#define OFS_PBIE_H             OFS_PBIE+1
#define OFS_PBIFG             (0x001Cu)  /* Port B Interrupt Flag */
#define OFS_PBIFG_L            OFS_PBIFG
#define OFS_PBIFG_H            OFS_PBIFG+1


#define OFS_P3IN              (0x0000u)
#define OFS_P3OUT             (0x0002u)
#define OFS_P3DIR             (0x0004u)
#define OFS_P3REN             (0x0006u)
#define OFS_P3SEL0            (0x000Au)
#define OFS_P3SEL1            (0x000Cu)
#define OFS_P3SELC            (0x0016u)
#define OFS_P3IV              (0x000Eu)  /* Port 3 Interrupt Vector Word */
#define OFS_P3IES             (0x0018u)
#define OFS_P3IE              (0x001Au)
#define OFS_P3IFG             (0x001Cu)
#define OFS_P4IN              (0x0001u)
#define OFS_P4OUT             (0x0003u)
#define OFS_P4DIR             (0x0005u)
#define OFS_P4REN             (0x0007u)
#define OFS_P4SEL0            (0x000Bu)
#define OFS_P4SEL1            (0x000Du)
#define OFS_P4SELC            (0x0017u)
#define OFS_P4IV              (0x001Eu)  /* Port 4 Interrupt Vector Word */
#define OFS_P4IES             (0x0019u)
#define OFS_P4IE              (0x001Bu)
#define OFS_P4IFG             (0x001du)
#define P3IN                (PBIN_L)  /* Port 3 Input */
#define P3OUT               (PBOUT_L) /* Port 3 Output */
#define P3DIR               (PBDIR_L) /* Port 3 Direction */
#define P3REN               (PBREN_L) /* Port 3 Resistor Enable */
#define P3SEL0              (PBSEL0_L) /* Port 3 Selection 0 */
#define P3SEL1              (PBSEL1_L) /* Port 3 Selection 1 */
#define P3SELC              (PBSELC_L) /* Port 3 Complement Selection */
#define P3IES               (PBIES_L) /* Port 3 Interrupt Edge Select */
#define P3IE                (PBIE_L)  /* Port 3 Interrupt Enable */
#define P3IFG               (PBIFG_L) /* Port 3 Interrupt Flag */

//Definitions for P3IV
#define P3IV_NONE            (0x0000u)    /* No Interrupt pending */
#define P3IV_P3IFG0          (0x0002u)    /* P3IV P3IFG.0 */
#define P3IV_P3IFG1          (0x0004u)    /* P3IV P3IFG.1 */
#define P3IV_P3IFG2          (0x0006u)    /* P3IV P3IFG.2 */
#define P3IV_P3IFG3          (0x0008u)    /* P3IV P3IFG.3 */
#define P3IV_P3IFG4          (0x000Au)    /* P3IV P3IFG.4 */
#define P3IV_P3IFG5          (0x000Cu)    /* P3IV P3IFG.5 */
#define P3IV_P3IFG6          (0x000Eu)    /* P3IV P3IFG.6 */
#define P3IV_P3IFG7          (0x0010u)    /* P3IV P3IFG.7 */

#define P4IN                (PBIN_H)  /* Port 4 Input */
#define P4OUT               (PBOUT_H) /* Port 4 Output */
#define P4DIR               (PBDIR_H) /* Port 4 Direction */
#define P4REN               (PBREN_H) /* Port 4 Resistor Enable */
#define P4SEL0              (PBSEL0_H) /* Port 4 Selection 0 */
#define P4SEL1              (PBSEL1_H) /* Port 4 Selection 1 */
#define P4SELC              (PBSELC_H) /* Port 4 Complement Selection */
#define P4IES               (PBIES_H) /* Port 4 Interrupt Edge Select */
#define P4IE                (PBIE_H)  /* Port 4 Interrupt Enable */
#define P4IFG               (PBIFG_H) /* Port 4 Interrupt Flag */

//Definitions for P4IV
#define P4IV_NONE            (0x0000u)    /* No Interrupt pending */
#define P4IV_P4IFG0          (0x0002u)    /* P4IV P4IFG.0 */
#define P4IV_P4IFG1          (0x0004u)    /* P4IV P4IFG.1 */
#define P4IV_P4IFG2          (0x0006u)    /* P4IV P4IFG.2 */
#define P4IV_P4IFG3          (0x0008u)    /* P4IV P4IFG.3 */
#define P4IV_P4IFG4          (0x000Au)    /* P4IV P4IFG.4 */
#define P4IV_P4IFG5          (0x000Cu)    /* P4IV P4IFG.5 */
#define P4IV_P4IFG6          (0x000Eu)    /* P4IV P4IFG.6 */
#define P4IV_P4IFG7          (0x0010u)    /* P4IV P4IFG.7 */


#endif
#endif
#endif
/************************************************************
* DIGITAL I/O Port5/6 Pull up / Pull down Resistors
************************************************************/
#ifdef  __MSP430_HAS_PORT5_R__        /* Definition to show that Module is available */
#ifdef  __MSP430_HAS_PORT6_R__        /* Definition to show that Module is available */
#ifdef  __MSP430_HAS_PORTC_R__        /* Definition to show that Module is available */

#define OFS_PCIN              (0x0000u)  /* Port C Input */
#define OFS_PCIN_L             OFS_PCIN
#define OFS_PCIN_H             OFS_PCIN+1
#define OFS_PCOUT             (0x0002u)  /* Port C Output */
#define OFS_PCOUT_L            OFS_PCOUT
#define OFS_PCOUT_H            OFS_PCOUT+1
#define OFS_PCDIR             (0x0004u)  /* Port C Direction */
#define OFS_PCDIR_L            OFS_PCDIR
#define OFS_PCDIR_H            OFS_PCDIR+1
#define OFS_PCREN             (0x0006u)  /* Port C Resistor Enable */
#define OFS_PCREN_L            OFS_PCREN
#define OFS_PCREN_H            OFS_PCREN+1
#define OFS_PCSEL0            (0x000Au)  /* Port C Selection 0 */
#define OFS_PCSEL0_L           OFS_PCSEL0
#define OFS_PCSEL0_H           OFS_PCSEL0+1
#define OFS_PCSEL1            (0x000Cu)  /* Port C Selection 1 */
#define OFS_PCSEL1_L           OFS_PCSEL1
#define OFS_PCSEL1_H           OFS_PCSEL1+1
#define OFS_PCSELC            (0x0016u)  /* Port C Complement Selection */
#define OFS_PCSELC_L           OFS_PCSELC
#define OFS_PCSELC_H           OFS_PCSELC+1
#define OFS_PCIES             (0x0018u)  /* Port C Interrupt Edge Select */
#define OFS_PCIES_L            OFS_PCIES
#define OFS_PCIES_H            OFS_PCIES+1
#define OFS_PCIE              (0x001Au)  /* Port C Interrupt Enable */
#define OFS_PCIE_L             OFS_PCIE
#define OFS_PCIE_H             OFS_PCIE+1
#define OFS_PCIFG             (0x001Cu)  /* Port C Interrupt Flag */
#define OFS_PCIFG_L            OFS_PCIFG
#define OFS_PCIFG_H            OFS_PCIFG+1


#define OFS_P5IN              (0x0000u)
#define OFS_P5OUT             (0x0002u)
#define OFS_P5DIR             (0x0004u)
#define OFS_P5REN             (0x0006u)
#define OFS_P5SEL0            (0x000Au)
#define OFS_P5SEL1            (0x000Cu)
#define OFS_P5SELC            (0x0016u)
#define OFS_P5IV              (0x000Eu)  /* Port 5 Interrupt Vector Word */
#define OFS_P5IES             (0x0018u)
#define OFS_P5IE              (0x001Au)
#define OFS_P5IFG             (0x001Cu)
#define OFS_P6IN              (0x0001u)
#define OFS_P6OUT             (0x0003u)
#define OFS_P6DIR             (0x0005u)
#define OFS_P6REN             (0x0007u)
#define OFS_P6SEL0            (0x000Bu)
#define OFS_P6SEL1            (0x000Du)
#define OFS_P6SELC            (0x0017u)
#define OFS_P6IV              (0x001Eu)  /* Port 6 Interrupt Vector Word */
#define OFS_P6IES             (0x0019u)
#define OFS_P6IE              (0x001Bu)
#define OFS_P6IFG             (0x001du)
#define P5IN                (PCIN_L)  /* Port 5 Input */
#define P5OUT               (PCOUT_L) /* Port 5 Output */
#define P5DIR               (PCDIR_L) /* Port 5 Direction */
#define P5REN               (PCREN_L) /* Port 5 Resistor Enable */
#define P5SEL0              (PCSEL0_L) /* Port 5 Selection 0 */
#define P5SEL1              (PCSEL1_L) /* Port 5 Selection 1 */
#define P5SELC              (PCSELC_L) /* Port 5 Complement Selection */
#define P5IES               (PCIES_L) /* Port 5 Interrupt Edge Select */
#define P5IE                (PCIE_L)  /* Port 5 Interrupt Enable */
#define P5IFG               (PCIFG_L) /* Port 5 Interrupt Flag */

//Definitions for P5IV
#define P5IV_NONE            (0x0000u)    /* No Interrupt pending */
#define P5IV_P5IFG0          (0x0002u)    /* P5IV P5IFG.0 */
#define P5IV_P5IFG1          (0x0004u)    /* P5IV P5IFG.1 */
#define P5IV_P5IFG2          (0x0006u)    /* P5IV P5IFG.2 */
#define P5IV_P5IFG3          (0x0008u)    /* P5IV P5IFG.3 */
#define P5IV_P5IFG4          (0x000Au)    /* P5IV P5IFG.4 */
#define P5IV_P5IFG5          (0x000Cu)    /* P5IV P5IFG.5 */
#define P5IV_P5IFG6          (0x000Eu)    /* P5IV P5IFG.6 */
#define P5IV_P5IFG7          (0x0010u)    /* P5IV P5IFG.7 */

#define P6IN                (PCIN_H)  /* Port 6 Input */
#define P6OUT               (PCOUT_H) /* Port 6 Output */
#define P6DIR               (PCDIR_H) /* Port 6 Direction */
#define P6REN               (PCREN_H) /* Port 6 Resistor Enable */
#define P6SEL0              (PCSEL0_H) /* Port 6 Selection 0 */
#define P6SEL1              (PCSEL1_H) /* Port 6 Selection 1 */
#define P6SELC              (PCSELC_H) /* Port 6 Complement Selection */
#define P6IES               (PCIES_H) /* Port 6 Interrupt Edge Select */
#define P6IE                (PCIE_H)  /* Port 6 Interrupt Enable */
#define P6IFG               (PCIFG_H) /* Port 6 Interrupt Flag */

//Definitions for P6IV
#define P6IV_NONE            (0x0000u)    /* No Interrupt pending */
#define P6IV_P6IFG0          (0x0002u)    /* P6IV P6IFG.0 */
#define P6IV_P6IFG1          (0x0004u)    /* P6IV P6IFG.1 */
#define P6IV_P6IFG2          (0x0006u)    /* P6IV P6IFG.2 */
#define P6IV_P6IFG3          (0x0008u)    /* P6IV P6IFG.3 */
#define P6IV_P6IFG4          (0x000Au)    /* P6IV P6IFG.4 */
#define P6IV_P6IFG5          (0x000Cu)    /* P6IV P6IFG.5 */
#define P6IV_P6IFG6          (0x000Eu)    /* P6IV P6IFG.6 */
#define P6IV_P6IFG7          (0x0010u)    /* P6IV P6IFG.7 */


#endif
#endif
#endif
/************************************************************
* DIGITAL I/O Port7/8 Pull up / Pull down Resistors
************************************************************/
#ifdef  __MSP430_HAS_PORT7_R__        /* Definition to show that Module is available */
#ifdef  __MSP430_HAS_PORT8_R__        /* Definition to show that Module is available */
#ifdef  __MSP430_HAS_PORTD_R__        /* Definition to show that Module is available */

#define OFS_PDIN              (0x0000u)  /* Port D Input */
#define OFS_PDIN_L             OFS_PDIN
#define OFS_PDIN_H             OFS_PDIN+1
#define OFS_PDOUT             (0x0002u)  /* Port D Output */
#define OFS_PDOUT_L            OFS_PDOUT
#define OFS_PDOUT_H            OFS_PDOUT+1
#define OFS_PDDIR             (0x0004u)  /* Port D Direction */
#define OFS_PDDIR_L            OFS_PDDIR
#define OFS_PDDIR_H            OFS_PDDIR+1
#define OFS_PDREN             (0x0006u)  /* Port D Resistor Enable */
#define OFS_PDREN_L            OFS_PDREN
#define OFS_PDREN_H            OFS_PDREN+1
#define OFS_PDSEL0            (0x000Au)  /* Port D Selection 0 */
#define OFS_PDSEL0_L           OFS_PDSEL0
#define OFS_PDSEL0_H           OFS_PDSEL0+1
#define OFS_PDSEL1            (0x000Cu)  /* Port D Selection 1 */
#define OFS_PDSEL1_L           OFS_PDSEL1
#define OFS_PDSEL1_H           OFS_PDSEL1+1
#define OFS_PDSELC            (0x0016u)  /* Port D Complement Selection */
#define OFS_PDSELC_L           OFS_PDSELC
#define OFS_PDSELC_H           OFS_PDSELC+1
#define OFS_PDIES             (0x0018u)  /* Port D Interrupt Edge Select */
#define OFS_PDIES_L            OFS_PDIES
#define OFS_PDIES_H            OFS_PDIES+1
#define OFS_PDIE              (0x001Au)  /* Port D Interrupt Enable */
#define OFS_PDIE_L             OFS_PDIE
#define OFS_PDIE_H             OFS_PDIE+1
#define OFS_PDIFG             (0x001Cu)  /* Port D Interrupt Flag */
#define OFS_PDIFG_L            OFS_PDIFG
#define OFS_PDIFG_H            OFS_PDIFG+1


#define OFS_P7IN              (0x0000u)
#define OFS_P7OUT             (0x0002u)
#define OFS_P7DIR             (0x0004u)
#define OFS_P7REN             (0x0006u)
#define OFS_P7SEL0            (0x000Au)
#define OFS_P7SEL1            (0x000Cu)
#define OFS_P7SELC            (0x0016u)
#define OFS_P7IV              (0x000Eu)  /* Port 7 Interrupt Vector Word */
#define OFS_P7IES             (0x0018u)
#define OFS_P7IE              (0x001Au)
#define OFS_P7IFG             (0x001Cu)
#define OFS_P8IN              (0x0001u)
#define OFS_P8OUT             (0x0003u)
#define OFS_P8DIR             (0x0005u)
#define OFS_P8REN             (0x0007u)
#define OFS_P8SEL0            (0x000Bu)
#define OFS_P8SEL1            (0x000Du)
#define OFS_P8SELC            (0x0017u)
#define OFS_P8IV              (0x001Eu)  /* Port 8 Interrupt Vector Word */
#define OFS_P8IES             (0x0019u)
#define OFS_P8IE              (0x001Bu)
#define OFS_P8IFG             (0x001du)
#define P7IN                (PDIN_L)  /* Port 7 Input */
#define P7OUT               (PDOUT_L) /* Port 7 Output */
#define P7DIR               (PDDIR_L) /* Port 7 Direction */
#define P7REN               (PDREN_L) /* Port 7 Resistor Enable */
#define P7SEL0              (PDSEL0_L) /* Port 7 Selection 0 */
#define P7SEL1              (PDSEL1_L) /* Port 7 Selection 1 */
#define P7SELC              (PDSELC_L) /* Port 7 Complement Selection */
#define P7IES               (PDIES_L) /* Port 7 Interrupt Edge Select */
#define P7IE                (PDIE_L)  /* Port 7 Interrupt Enable */
#define P7IFG               (PDIFG_L) /* Port 7 Interrupt Flag */

//Definitions for P7IV
#define P7IV_NONE            (0x0000u)    /* No Interrupt pending */
#define P7IV_P7IFG0          (0x0002u)    /* P7IV P7IFG.0 */
#define P7IV_P7IFG1          (0x0004u)    /* P7IV P7IFG.1 */
#define P7IV_P7IFG2          (0x0006u)    /* P7IV P7IFG.2 */
#define P7IV_P7IFG3          (0x0008u)    /* P7IV P7IFG.3 */
#define P7IV_P7IFG4          (0x000Au)    /* P7IV P7IFG.4 */
#define P7IV_P7IFG5          (0x000Cu)    /* P7IV P7IFG.5 */
#define P7IV_P7IFG6          (0x000Eu)    /* P7IV P7IFG.6 */
#define P7IV_P7IFG7          (0x0010u)    /* P7IV P7IFG.7 */

#define P8IN                (PDIN_H)  /* Port 8 Input */
#define P8OUT               (PDOUT_H) /* Port 8 Output */
#define P8DIR               (PDDIR_H) /* Port 8 Direction */
#define P8REN               (PDREN_H) /* Port 8 Resistor Enable */
#define P8SEL0              (PDSEL0_H) /* Port 8 Selection 0 */
#define P8SEL1              (PDSEL1_H) /* Port 8 Selection 1 */
#define P8SELC              (PDSELC_H) /* Port 8 Complement Selection */
#define P8IES               (PDIES_H) /* Port 8 Interrupt Edge Select */
#define P8IE                (PDIE_H)  /* Port 8 Interrupt Enable */
#define P8IFG               (PDIFG_H) /* Port 8 Interrupt Flag */

//Definitions for P8IV
#define P8IV_NONE            (0x0000u)    /* No Interrupt pending */
#define P8IV_P8IFG0          (0x0002u)    /* P8IV P8IFG.0 */
#define P8IV_P8IFG1          (0x0004u)    /* P8IV P8IFG.1 */
#define P8IV_P8IFG2          (0x0006u)    /* P8IV P8IFG.2 */
#define P8IV_P8IFG3          (0x0008u)    /* P8IV P8IFG.3 */
#define P8IV_P8IFG4          (0x000Au)    /* P8IV P8IFG.4 */
#define P8IV_P8IFG5          (0x000Cu)    /* P8IV P8IFG.5 */
#define P8IV_P8IFG6          (0x000Eu)    /* P8IV P8IFG.6 */
#define P8IV_P8IFG7          (0x0010u)    /* P8IV P8IFG.7 */


#endif
#endif
#endif
/************************************************************
* DIGITAL I/O Port9/10 Pull up / Pull down Resistors
************************************************************/
#ifdef  __MSP430_HAS_PORT9_R__        /* Definition to show that Module is available */
#ifdef  __MSP430_HAS_PORT10_R__        /* Definition to show that Module is available */
#ifdef  __MSP430_HAS_PORTE_R__        /* Definition to show that Module is available */

#define OFS_PEIN              (0x0000u)  /* Port E Input */
#define OFS_PEIN_L             OFS_PEIN
#define OFS_PEIN_H             OFS_PEIN+1
#define OFS_PEOUT             (0x0002u)  /* Port E Output */
#define OFS_PEOUT_L            OFS_PEOUT
#define OFS_PEOUT_H            OFS_PEOUT+1
#define OFS_PEDIR             (0x0004u)  /* Port E Direction */
#define OFS_PEDIR_L            OFS_PEDIR
#define OFS_PEDIR_H            OFS_PEDIR+1
#define OFS_PEREN             (0x0006u)  /* Port E Resistor Enable */
#define OFS_PEREN_L            OFS_PEREN
#define OFS_PEREN_H            OFS_PEREN+1
#define OFS_PESEL0            (0x000Au)  /* Port E Selection 0 */
#define OFS_PESEL0_L           OFS_PESEL0
#define OFS_PESEL0_H           OFS_PESEL0+1
#define OFS_PESEL1            (0x000Cu)  /* Port E Selection 1 */
#define OFS_PESEL1_L           OFS_PESEL1
#define OFS_PESEL1_H           OFS_PESEL1+1
#define OFS_PESELC            (0x0016u)  /* Port E Complement Selection */
#define OFS_PESELC_L           OFS_PESELC
#define OFS_PESELC_H           OFS_PESELC+1
#define OFS_PEIES             (0x0018u)  /* Port E Interrupt Edge Select */
#define OFS_PEIES_L            OFS_PEIES
#define OFS_PEIES_H            OFS_PEIES+1
#define OFS_PEIE              (0x001Au)  /* Port E Interrupt Enable */
#define OFS_PEIE_L             OFS_PEIE
#define OFS_PEIE_H             OFS_PEIE+1
#define OFS_PEIFG             (0x001Cu)  /* Port E Interrupt Flag */
#define OFS_PEIFG_L            OFS_PEIFG
#define OFS_PEIFG_H            OFS_PEIFG+1


#define OFS_P9IN              (0x0000u)
#define OFS_P9OUT             (0x0002u)
#define OFS_P9DIR             (0x0004u)
#define OFS_P9REN             (0x0006u)
#define OFS_P9SEL0            (0x000Au)
#define OFS_P9SEL1            (0x000Cu)
#define OFS_P9SELC            (0x0016u)
#define OFS_P9IV              (0x000Eu)  /* Port 9 Interrupt Vector Word */
#define OFS_P9IES             (0x0018u)
#define OFS_P9IE              (0x001Au)
#define OFS_P9IFG             (0x001Cu)
#define OFS_P10IN             (0x0001u)
#define OFS_P10OUT            (0x0003u)
#define OFS_P10DIR            (0x0005u)
#define OFS_P10REN            (0x0007u)
#define OFS_P10SEL0           (0x000Bu)
#define OFS_P10SEL1           (0x000Du)
#define OFS_P10SELC           (0x0017u)
#define OFS_P10IV             (0x001Eu)  /* Port 10 Interrupt Vector Word */
#define OFS_P10IES            (0x0019u)
#define OFS_P10IE             (0x001Bu)
#define OFS_P10IFG            (0x001du)
#define P9IN                (PEIN_L)  /* Port 9 Input */
#define P9OUT               (PEOUT_L) /* Port 9 Output */
#define P9DIR               (PEDIR_L) /* Port 9 Direction */
#define P9REN               (PEREN_L) /* Port 9 Resistor Enable */
#define P9SEL0              (PESEL0_L) /* Port 9 Selection 0 */
#define P9SEL1              (PESEL1_L) /* Port 9 Selection 1 */
#define P9SELC              (PESELC_L) /* Port 9 Complement Selection */
#define P9IES               (PEIES_L) /* Port 9 Interrupt Edge Select */
#define P9IE                (PEIE_L)  /* Port 9 Interrupt Enable */
#define P9IFG               (PEIFG_L) /* Port 9 Interrupt Flag */

//Definitions for P9IV
#define P9IV_NONE            (0x0000u)    /* No Interrupt pending */
#define P9IV_P9IFG0          (0x0002u)    /* P9IV P9IFG.0 */
#define P9IV_P9IFG1          (0x0004u)    /* P9IV P9IFG.1 */
#define P9IV_P9IFG2          (0x0006u)    /* P9IV P9IFG.2 */
#define P9IV_P9IFG3          (0x0008u)    /* P9IV P9IFG.3 */
#define P9IV_P9IFG4          (0x000Au)    /* P9IV P9IFG.4 */
#define P9IV_P9IFG5          (0x000Cu)    /* P9IV P9IFG.5 */
#define P9IV_P9IFG6          (0x000Eu)    /* P9IV P9IFG.6 */
#define P9IV_P9IFG7          (0x0010u)    /* P9IV P9IFG.7 */

#define P10IN                (PEIN_H)  /* Port 10 Input */
#define P10OUT               (PEOUT_H) /* Port 10 Output */
#define P10DIR               (PEDIR_H) /* Port 10 Direction */
#define P10REN               (PEREN_H) /* Port 10 Resistor Enable */
#define P10SEL0              (PESEL0_H) /* Port 10 Selection 0 */
#define P10SEL1              (PESEL1_H) /* Port 10 Selection 1 */
#define P10SELC              (PESELC_H) /* Port 10 Complement Selection */
#define P10IES               (PEIES_H) /* Port 10 Interrupt Edge Select */
#define P10IE                (PEIE_H)  /* Port 10 Interrupt Enable */
#define P10IFG               (PEIFG_H) /* Port 10 Interrupt Flag */

//Definitions for P10IV
#define P10IV_NONE            (0x0000u)    /* No Interrupt pending */
#define P10IV_P10IFG0          (0x0002u)    /* P10IV P10IFG.0 */
#define P10IV_P10IFG1          (0x0004u)    /* P10IV P10IFG.1 */
#define P10IV_P10IFG2          (0x0006u)    /* P10IV P10IFG.2 */
#define P10IV_P10IFG3          (0x0008u)    /* P10IV P10IFG.3 */
#define P10IV_P10IFG4          (0x000Au)    /* P10IV P10IFG.4 */
#define P10IV_P10IFG5          (0x000Cu)    /* P10IV P10IFG.5 */
#define P10IV_P10IFG6          (0x000Eu)    /* P10IV P10IFG.6 */
#define P10IV_P10IFG7          (0x0010u)    /* P10IV P10IFG.7 */


#endif
#endif
#endif
/************************************************************
* DIGITAL I/O Port11 Pull up / Pull down Resistors
************************************************************/
#ifdef  __MSP430_HAS_PORT11_R__        /* Definition to show that Module is available */
#ifdef  __MSP430_HAS_PORTF_R__        /* Definition to show that Module is available */

#define OFS_PFIN              (0x0000u)  /* Port F Input */
#define OFS_PFIN_L             OFS_PFIN
#define OFS_PFIN_H             OFS_PFIN+1
#define OFS_PFOUT             (0x0002u)  /* Port F Output */
#define OFS_PFOUT_L            OFS_PFOUT
#define OFS_PFOUT_H            OFS_PFOUT+1
#define OFS_PFDIR             (0x0004u)  /* Port F Direction */
#define OFS_PFDIR_L            OFS_PFDIR
#define OFS_PFDIR_H            OFS_PFDIR+1
#define OFS_PFREN             (0x0006u)  /* Port F Resistor Enable */
#define OFS_PFREN_L            OFS_PFREN
#define OFS_PFREN_H            OFS_PFREN+1
#define OFS_PFSEL0            (0x000Au)  /* Port F Selection 0 */
#define OFS_PFSEL0_L           OFS_PFSEL0
#define OFS_PFSEL0_H           OFS_PFSEL0+1
#define OFS_PFSEL1            (0x000Cu)  /* Port F Selection 1 */
#define OFS_PFSEL1_L           OFS_PFSEL1
#define OFS_PFSEL1_H           OFS_PFSEL1+1
#define OFS_PFSELC            (0x0016u)  /* Port F Complement Selection */
#define OFS_PFSELC_L           OFS_PFSELC
#define OFS_PFSELC_H           OFS_PFSELC+1
#define OFS_PFIES             (0x0018u)  /* Port F Interrupt Edge Select */
#define OFS_PFIES_L            OFS_PFIES
#define OFS_PFIES_H            OFS_PFIES+1
#define OFS_PFIE              (0x001Au)  /* Port F Interrupt Enable */
#define OFS_PFIE_L             OFS_PFIE
#define OFS_PFIE_H             OFS_PFIE+1
#define OFS_PFIFG             (0x001Cu)  /* Port F Interrupt Flag */
#define OFS_PFIFG_L            OFS_PFIFG
#define OFS_PFIFG_H            OFS_PFIFG+1


#define OFS_P11IN             (0x0000u)
#define OFS_P11OUT            (0x0002u)
#define OFS_P11DIR            (0x0004u)
#define OFS_P11REN            (0x0006u)
#define OFS_P11SEL0           (0x000Au)
#define OFS_P11SEL1           (0x000Cu)
#define OFS_P11SELC           (0x0016u)
#define OFS_P11IV             (0x000Eu)  /* Port 11 Interrupt Vector Word */
#define OFS_P11IES            (0x0018u)
#define OFS_P11IE             (0x001Au)
#define OFS_P11IFG            (0x001Cu)
#define P11IN                (PFIN_L)  /* Port 11 Input */
#define P11OUT               (PFOUT_L) /* Port 11 Output */
#define P11DIR               (PFDIR_L) /* Port 11 Direction */
#define P11REN               (PFREN_L) /* Port 11 Resistor Enable */
#define P11SEL0              (PFSEL0_L) /* Port 11 Selection0 */
#define P11SEL1              (PFSEL1_L) /* Port 11 Selection1 */
#define OFS_P11SELC           (0x0017u)

#define P11IES               (PFIES_L) /* Port 11 Interrupt Edge Select */
#define P11IE                (PFIE_L)  /* Port 11 Interrupt Enable */
#define P11IFG               (PFIFG_L) /* Port 11 Interrupt Flag */

//Definitions for P11IV
#define P11IV_NONE            (0x0000u)    /* No Interrupt pending */
#define P11IV_P11IFG0          (0x0002u)    /* P11IV P11IFG.0 */
#define P11IV_P11IFG1          (0x0004u)    /* P11IV P11IFG.1 */
#define P11IV_P11IFG2          (0x0006u)    /* P11IV P11IFG.2 */
#define P11IV_P11IFG3          (0x0008u)    /* P11IV P11IFG.3 */
#define P11IV_P11IFG4          (0x000Au)    /* P11IV P11IFG.4 */
#define P11IV_P11IFG5          (0x000Cu)    /* P11IV P11IFG.5 */
#define P11IV_P11IFG6          (0x000Eu)    /* P11IV P11IFG.6 */
#define P11IV_P11IFG7          (0x0010u)    /* P11IV P11IFG.7 */


#endif
#endif
/************************************************************
* DIGITAL I/O PortJ Pull up / Pull down Resistors
************************************************************/
#ifdef  __MSP430_HAS_PORTJ_R__        /* Definition to show that Module is available */

#define OFS_PJIN              (0x0000u)  /* Port J Input */
#define OFS_PJIN_L             OFS_PJIN
#define OFS_PJIN_H             OFS_PJIN+1
#define OFS_PJOUT             (0x0002u)  /* Port J Output */
#define OFS_PJOUT_L            OFS_PJOUT
#define OFS_PJOUT_H            OFS_PJOUT+1
#define OFS_PJDIR             (0x0004u)  /* Port J Direction */
#define OFS_PJDIR_L            OFS_PJDIR
#define OFS_PJDIR_H            OFS_PJDIR+1
#define OFS_PJREN             (0x0006u)  /* Port J Resistor Enable */
#define OFS_PJREN_L            OFS_PJREN
#define OFS_PJREN_H            OFS_PJREN+1
#define OFS_PJSEL0            (0x000Au)  /* Port J Selection 0 */
#define OFS_PJSEL0_L           OFS_PJSEL0
#define OFS_PJSEL0_H           OFS_PJSEL0+1
#define OFS_PJSEL1            (0x000Cu)  /* Port J Selection 1 */
#define OFS_PJSEL1_L           OFS_PJSEL1
#define OFS_PJSEL1_H           OFS_PJSEL1+1
#define OFS_PJSELC            (0x0016u)  /* Port J Complement Selection */
#define OFS_PJSELC_L           OFS_PJSELC
#define OFS_PJSELC_H           OFS_PJSELC+1

#endif
/*************************************************************
* RAM Control Module for FRAM
*************************************************************/
#ifdef  __MSP430_HAS_RC_FRAM__        /* Definition to show that Module is available */

#define OFS_RCCTL0            (0x0000u)  /* Ram Controller Control Register */
#define OFS_RCCTL0_L           OFS_RCCTL0
#define OFS_RCCTL0_H           OFS_RCCTL0+1

/* RCCTL0 Control Bits */
#define RCRS0OFF0           (0x0001u)  /* RAM Controller RAM Sector 0 Off Bit: 0 */
#define RCRS0OFF1           (0x0002u)  /* RAM Controller RAM Sector 0 Off Bit: 1 */
#define RCRS4OFF0           (0x0100u)  /* RAM Controller RAM Sector 4 Off Bit: 0 */
#define RCRS4OFF1           (0x0200u)  /* RAM Controller RAM Sector 4 Off Bit: 1 */
#define RCRS5OFF0           (0x0400u)  /* RAM Controller RAM Sector 5 Off Bit: 0 */
#define RCRS5OFF1           (0x0800u)  /* RAM Controller RAM Sector 5 Off Bit: 1 */
#define RCRS6OFF0           (0x1000u)  /* RAM Controller RAM Sector 6 Off Bit: 0 */
#define RCRS6OFF1           (0x2000u)  /* RAM Controller RAM Sector 6 Off Bit: 1 */
#define RCRS7OFF0           (0x4000u)  /* RAM Controller RAM Sector 7 (USB) Off Bit: 0 */
#define RCRS7OFF1           (0x8000u)  /* RAM Controller RAM Sector 7 (USB) Off Bit: 1 */

/* RCCTL0 Control Bits */
#define RCRS0OFF0_L         (0x0001u)  /* RAM Controller RAM Sector 0 Off Bit: 0 */
#define RCRS0OFF1_L         (0x0002u)  /* RAM Controller RAM Sector 0 Off Bit: 1 */

/* RCCTL0 Control Bits */
#define RCRS4OFF0_H         (0x0001u)  /* RAM Controller RAM Sector 4 Off Bit: 0 */
#define RCRS4OFF1_H         (0x0002u)  /* RAM Controller RAM Sector 4 Off Bit: 1 */
#define RCRS5OFF0_H         (0x0004u)  /* RAM Controller RAM Sector 5 Off Bit: 0 */
#define RCRS5OFF1_H         (0x0008u)  /* RAM Controller RAM Sector 5 Off Bit: 1 */
#define RCRS6OFF0_H         (0x0010u)  /* RAM Controller RAM Sector 6 Off Bit: 0 */
#define RCRS6OFF1_H         (0x0020u)  /* RAM Controller RAM Sector 6 Off Bit: 1 */
#define RCRS7OFF0_H         (0x0040u)  /* RAM Controller RAM Sector 7 (USB) Off Bit: 0 */
#define RCRS7OFF1_H         (0x0080u)  /* RAM Controller RAM Sector 7 (USB) Off Bit: 1 */

#define RCKEY               (0x5A00u)

#define RCRS0OFF_0          (0x0000u)  /* RAM Controller RAM Sector 0 Off : 0 */
#define RCRS0OFF_1          (0x0001u)  /* RAM Controller RAM Sector 0 Off : 1 */
#define RCRS0OFF_2          (0x0002u)  /* RAM Controller RAM Sector 0 Off : 2 */
#define RCRS0OFF_3          (0x0003u)  /* RAM Controller RAM Sector 0 Off : 3 */
#define RCRS4OFF_0          (0x0000u)  /* RAM Controller RAM Sector 4 Off : 0 */
#define RCRS4OFF_2          (0x0100u)  /* RAM Controller RAM Sector 4 Off : 1 */
#define RCRS4OFF_3          (0x0200u)  /* RAM Controller RAM Sector 4 Off : 2 */
#define RCRS4OFF_4          (0x0300u)  /* RAM Controller RAM Sector 4 Off : 3 */
#define RCRS5OFF_0          (0x0000u)  /* RAM Controller RAM Sector 5 Off : 0 */
#define RCRS5OFF_1          (0x0400u)  /* RAM Controller RAM Sector 5 Off : 1 */
#define RCRS5OFF_2          (0x0800u)  /* RAM Controller RAM Sector 5 Off : 2 */
#define RCRS5OFF_3          (0x0C00u)  /* RAM Controller RAM Sector 5 Off : 3 */
#define RCRS6OFF_0          (0x0000u)  /* RAM Controller RAM Sector 6 Off : 0 */
#define RCRS6OFF_1          (0x0100u)  /* RAM Controller RAM Sector 6 Off : 1 */
#define RCRS6OFF_2          (0x0200u)  /* RAM Controller RAM Sector 6 Off : 2 */
#define RCRS6OFF_3          (0x0300u)  /* RAM Controller RAM Sector 6 Off : 3 */
#define RCRS7OFF_0          (0x0000u)  /* RAM Controller RAM Sector 7 Off : 0 */
#define RCRS7OFF_1          (0x4000u)  /* RAM Controller RAM Sector 7 Off : 1 */
#define RCRS7OFF_2          (0x8000u)  /* RAM Controller RAM Sector 7 Off : 2*/
#define RCRS7OFF_3          (0xC000u)  /* RAM Controller RAM Sector 7 Off : 3*/

#endif
/************************************************************
* Shared Reference
************************************************************/
#ifdef  __MSP430_HAS_REF_A__           /* Definition to show that Module is available */

#define OFS_REFCTL0           (0x0000u)  /* REF Shared Reference control register 0 */
#define OFS_REFCTL0_L          OFS_REFCTL0
#define OFS_REFCTL0_H          OFS_REFCTL0+1

/* REFCTL0 Control Bits */
#define REFON               (0x0001u)  /* REF Reference On */
#define REFOUT              (0x0002u)  /* REF Reference output Buffer On */
//#define RESERVED            (0x0004u)  /* Reserved */
#define REFTCOFF            (0x0008u)  /* REF Temp.Sensor off */
#define REFVSEL0            (0x0010u)  /* REF Reference Voltage Level Select Bit:0 */
#define REFVSEL1            (0x0020u)  /* REF Reference Voltage Level Select Bit:1 */
#define REFGENOT            (0x0040u)  /* REF Reference generator one-time trigger */
#define REFBGOT             (0x0080u)  /* REF Bandgap and bandgap buffer one-time trigger */
#define REFGENACT           (0x0100u)  /* REF Reference generator active */
#define REFBGACT            (0x0200u)  /* REF Reference bandgap active */
#define REFGENBUSY          (0x0400u)  /* REF Reference generator busy */
#define BGMODE              (0x0800u)  /* REF Bandgap mode */
#define REFGENRDY           (0x1000u)  /* REF Reference generator ready */
#define REFBGRDY            (0x2000u)  /* REF Reference bandgap ready */
//#define RESERVED            (0x4000u)  /* Reserved */
//#define RESERVED            (0x8000u)  /* Reserved */

/* REFCTL0 Control Bits */
#define REFON_L             (0x0001u)  /* REF Reference On */
#define REFOUT_L            (0x0002u)  /* REF Reference output Buffer On */
//#define RESERVED            (0x0004u)  /* Reserved */
#define REFTCOFF_L          (0x0008u)  /* REF Temp.Sensor off */
#define REFVSEL0_L          (0x0010u)  /* REF Reference Voltage Level Select Bit:0 */
#define REFVSEL1_L          (0x0020u)  /* REF Reference Voltage Level Select Bit:1 */
#define REFGENOT_L          (0x0040u)  /* REF Reference generator one-time trigger */
#define REFBGOT_L           (0x0080u)  /* REF Bandgap and bandgap buffer one-time trigger */
//#define RESERVED            (0x4000u)  /* Reserved */
//#define RESERVED            (0x8000u)  /* Reserved */

/* REFCTL0 Control Bits */
//#define RESERVED            (0x0004u)  /* Reserved */
#define REFGENACT_H         (0x0001u)  /* REF Reference generator active */
#define REFBGACT_H          (0x0002u)  /* REF Reference bandgap active */
#define REFGENBUSY_H        (0x0004u)  /* REF Reference generator busy */
#define BGMODE_H            (0x0008u)  /* REF Bandgap mode */
#define REFGENRDY_H         (0x0010u)  /* REF Reference generator ready */
#define REFBGRDY_H          (0x0020u)  /* REF Reference bandgap ready */
//#define RESERVED            (0x4000u)  /* Reserved */
//#define RESERVED            (0x8000u)  /* Reserved */

#define REFVSEL_0           (0x0000u)  /* REF Reference Voltage Level Select 1.2V */
#define REFVSEL_1           (0x0010u)  /* REF Reference Voltage Level Select 2.0V */
#define REFVSEL_2           (0x0020u)  /* REF Reference Voltage Level Select 2.5V */
#define REFVSEL_3           (0x0030u)  /* REF Reference Voltage Level Select 2.5V */

#endif
/************************************************************
* Real Time Clock
************************************************************/
#ifdef  __MSP430_HAS_RTC_B__          /* Definition to show that Module is available */

#define OFS_RTCCTL01          (0x0000u)  /* Real Timer Control 0/1 */
#define OFS_RTCCTL01_L         OFS_RTCCTL01
#define OFS_RTCCTL01_H         OFS_RTCCTL01+1
#define OFS_RTCCTL23          (0x0002u)  /* Real Timer Control 2/3 */
#define OFS_RTCCTL23_L         OFS_RTCCTL23
#define OFS_RTCCTL23_H         OFS_RTCCTL23+1
#define OFS_RTCPS0CTL         (0x0008u)  /* Real Timer Prescale Timer 0 Control */
#define OFS_RTCPS0CTL_L        OFS_RTCPS0CTL
#define OFS_RTCPS0CTL_H        OFS_RTCPS0CTL+1
#define OFS_RTCPS1CTL         (0x000Au)  /* Real Timer Prescale Timer 1 Control */
#define OFS_RTCPS1CTL_L        OFS_RTCPS1CTL
#define OFS_RTCPS1CTL_H        OFS_RTCPS1CTL+1
#define OFS_RTCPS             (0x000Cu)  /* Real Timer Prescale Timer Control */
#define OFS_RTCPS_L            OFS_RTCPS
#define OFS_RTCPS_H            OFS_RTCPS+1
#define OFS_RTCIV             (0x000Eu)  /* Real Time Clock Interrupt Vector */
#define OFS_RTCTIM0           (0x0010u)  /* Real Time Clock Time 0 */
#define OFS_RTCTIM0_L          OFS_RTCTIM0
#define OFS_RTCTIM0_H          OFS_RTCTIM0+1
#define OFS_RTCTIM1           (0x0012u)  /* Real Time Clock Time 1 */
#define OFS_RTCTIM1_L          OFS_RTCTIM1
#define OFS_RTCTIM1_H          OFS_RTCTIM1+1
#define OFS_RTCDATE           (0x0014u)  /* Real Time Clock Date */
#define OFS_RTCDATE_L          OFS_RTCDATE
#define OFS_RTCDATE_H          OFS_RTCDATE+1
#define OFS_RTCYEAR           (0x0016u)  /* Real Time Clock Year */
#define OFS_RTCYEAR_L          OFS_RTCYEAR
#define OFS_RTCYEAR_H          OFS_RTCYEAR+1
#define OFS_RTCAMINHR         (0x0018u)  /* Real Time Clock Alarm Min/Hour */
#define OFS_RTCAMINHR_L        OFS_RTCAMINHR
#define OFS_RTCAMINHR_H        OFS_RTCAMINHR+1
#define OFS_RTCADOWDAY        (0x001Au)  /* Real Time Clock Alarm day of week/day */
#define OFS_RTCADOWDAY_L       OFS_RTCADOWDAY
#define OFS_RTCADOWDAY_H       OFS_RTCADOWDAY+1
#define OFS_BIN2BCD           (0x001Cu)  /* Real Time Binary-to-BCD conversion register */
#define OFS_BCD2BIN           (0x001Eu)  /* Real Time BCD-to-binary conversion register */
#define OFS_RTCSEC            (0x0010u)
#define OFS_RTCMIN            (0x0011u)
#define OFS_RTCHOUR           (0x0012u)
#define OFS_RTCDOW            (0x0013u)
#define OFS_RTCDAY            (0x0014u)
#define OFS_RTCMON            (0x0015u)
#define OFS_RTCAMIN           (0x0018u)
#define OFS_RTCAHOUR          (0x0019u)
#define OFS_RTCADOW           (0x001Au)
#define OFS_RTCADAY           (0x001Bu)

#define RTCCTL0             RTCCTL01_L  /* Real Time Clock Control 0 */
#define RTCCTL1             RTCCTL01_H  /* Real Time Clock Control 1 */
#define RTCCTL2             RTCCTL23_L  /* Real Time Clock Control 2 */
#define RTCCTL3             RTCCTL23_H  /* Real Time Clock Control 3 */
#define RTCNT12             RTCTIM0
#define RTCNT34             RTCTIM1
#define RTCNT1              RTCTIM0_L
#define RTCNT2              RTCTIM0_H
#define RTCNT3              RTCTIM1_L
#define RTCNT4              RTCTIM1_H
#define RTCSEC              RTCTIM0_L
#define RTCMIN              RTCTIM0_H
#define RTCHOUR             RTCTIM1_L
#define RTCDOW              RTCTIM1_H
#define RTCDAY              RTCDATE_L
#define RTCMON              RTCDATE_H
#define RTCYEARL            RTCYEAR_L
#define RTCYEARH            RTCYEAR_H
#define RT0PS               RTCPS_L
#define RT1PS               RTCPS_H
#define RTCAMIN             RTCAMINHR_L  /* Real Time Clock Alarm Min */
#define RTCAHOUR            RTCAMINHR_H  /* Real Time Clock Alarm Hour */
#define RTCADOW             RTCADOWDAY_L /* Real Time Clock Alarm day of week */
#define RTCADAY             RTCADOWDAY_H /* Real Time Clock Alarm day */

/* RTCCTL01 Control Bits */
#define RTCBCD              (0x8000u)     /* RTC BCD  0:Binary / 1:BCD */
#define RTCHOLD             (0x4000u)     /* RTC Hold */
//#define RESERVED            (0x2000u)     /* RESERVED */
#define RTCRDY              (0x1000u)     /* RTC Ready */
//#define RESERVED            (0x0800u)     /* RESERVED */
//#define RESERVED            (0x0400u)     /* RESERVED */
#define RTCTEV1             (0x0200u)     /* RTC Time Event 1 */
#define RTCTEV0             (0x0100u)     /* RTC Time Event 0 */
#define RTCOFIE             (0x0080u)     /* RTC 32kHz cyrstal oscillator fault interrupt enable */
#define RTCTEVIE            (0x0040u)     /* RTC Time Event Interrupt Enable Flag */
#define RTCAIE              (0x0020u)     /* RTC Alarm Interrupt Enable Flag */
#define RTCRDYIE            (0x0010u)     /* RTC Ready Interrupt Enable Flag */
#define RTCOFIFG            (0x0008u)     /* RTC 32kHz cyrstal oscillator fault interrupt flag */
#define RTCTEVIFG           (0x0004u)     /* RTC Time Event Interrupt Flag */
#define RTCAIFG             (0x0002u)     /* RTC Alarm Interrupt Flag */
#define RTCRDYIFG           (0x0001u)     /* RTC Ready Interrupt Flag */

/* RTCCTL01 Control Bits */
//#define RESERVED            (0x2000u)     /* RESERVED */
//#define RESERVED            (0x0800u)     /* RESERVED */
//#define RESERVED            (0x0400u)     /* RESERVED */
#define RTCOFIE_L           (0x0080u)     /* RTC 32kHz cyrstal oscillator fault interrupt enable */
#define RTCTEVIE_L          (0x0040u)     /* RTC Time Event Interrupt Enable Flag */
#define RTCAIE_L            (0x0020u)     /* RTC Alarm Interrupt Enable Flag */
#define RTCRDYIE_L          (0x0010u)     /* RTC Ready Interrupt Enable Flag */
#define RTCOFIFG_L          (0x0008u)     /* RTC 32kHz cyrstal oscillator fault interrupt flag */
#define RTCTEVIFG_L         (0x0004u)     /* RTC Time Event Interrupt Flag */
#define RTCAIFG_L           (0x0002u)     /* RTC Alarm Interrupt Flag */
#define RTCRDYIFG_L         (0x0001u)     /* RTC Ready Interrupt Flag */

/* RTCCTL01 Control Bits */
#define RTCBCD_H            (0x0080u)     /* RTC BCD  0:Binary / 1:BCD */
#define RTCHOLD_H           (0x0040u)     /* RTC Hold */
//#define RESERVED            (0x2000u)     /* RESERVED */
#define RTCRDY_H            (0x0010u)     /* RTC Ready */
//#define RESERVED            (0x0800u)     /* RESERVED */
//#define RESERVED            (0x0400u)     /* RESERVED */
#define RTCTEV1_H           (0x0002u)     /* RTC Time Event 1 */
#define RTCTEV0_H           (0x0001u)     /* RTC Time Event 0 */

#define RTCTEV_0            (0x0000u)     /* RTC Time Event: 0 (Min. changed) */
#define RTCTEV_1            (0x0100u)     /* RTC Time Event: 1 (Hour changed) */
#define RTCTEV_2            (0x0200u)     /* RTC Time Event: 2 (12:00 changed) */
#define RTCTEV_3            (0x0300u)     /* RTC Time Event: 3 (00:00 changed) */
#define RTCTEV__MIN         (0x0000u)     /* RTC Time Event: 0 (Min. changed) */
#define RTCTEV__HOUR        (0x0100u)     /* RTC Time Event: 1 (Hour changed) */
#define RTCTEV__0000        (0x0200u)     /* RTC Time Event: 2 (00:00 changed) */
#define RTCTEV__1200        (0x0300u)     /* RTC Time Event: 3 (12:00 changed) */

/* RTCCTL23 Control Bits */
#define RTCCALF1            (0x0200u)     /* RTC Calibration Frequency Bit 1 */
#define RTCCALF0            (0x0100u)     /* RTC Calibration Frequency Bit 0 */
#define RTCCALS             (0x0080u)     /* RTC Calibration Sign */
//#define Reserved          (0x0040u)
#define RTCCAL5             (0x0020u)     /* RTC Calibration Bit 5 */
#define RTCCAL4             (0x0010u)     /* RTC Calibration Bit 4 */
#define RTCCAL3             (0x0008u)     /* RTC Calibration Bit 3 */
#define RTCCAL2             (0x0004u)     /* RTC Calibration Bit 2 */
#define RTCCAL1             (0x0002u)     /* RTC Calibration Bit 1 */
#define RTCCAL0             (0x0001u)     /* RTC Calibration Bit 0 */

/* RTCCTL23 Control Bits */
#define RTCCALS_L           (0x0080u)     /* RTC Calibration Sign */
//#define Reserved          (0x0040u)
#define RTCCAL5_L           (0x0020u)     /* RTC Calibration Bit 5 */
#define RTCCAL4_L           (0x0010u)     /* RTC Calibration Bit 4 */
#define RTCCAL3_L           (0x0008u)     /* RTC Calibration Bit 3 */
#define RTCCAL2_L           (0x0004u)     /* RTC Calibration Bit 2 */
#define RTCCAL1_L           (0x0002u)     /* RTC Calibration Bit 1 */
#define RTCCAL0_L           (0x0001u)     /* RTC Calibration Bit 0 */

/* RTCCTL23 Control Bits */
#define RTCCALF1_H          (0x0002u)     /* RTC Calibration Frequency Bit 1 */
#define RTCCALF0_H          (0x0001u)     /* RTC Calibration Frequency Bit 0 */
//#define Reserved          (0x0040u)

#define RTCCALF_0           (0x0000u)     /* RTC Calibration Frequency: No Output */
#define RTCCALF_1           (0x0100u)     /* RTC Calibration Frequency: 512 Hz */
#define RTCCALF_2           (0x0200u)     /* RTC Calibration Frequency: 256 Hz */
#define RTCCALF_3           (0x0300u)     /* RTC Calibration Frequency: 1 Hz */

#define RTCAE               (0x80)     /* Real Time Clock Alarm enable */

/* RTCPS0CTL Control Bits */
//#define Reserved          (0x0080u)
//#define Reserved          (0x0040u)
//#define Reserved          (0x0020u)
#define RT0IP2              (0x0010u)     /* RTC Prescale Timer 0 Interrupt Interval Bit: 2 */
#define RT0IP1              (0x0008u)     /* RTC Prescale Timer 0 Interrupt Interval Bit: 1 */
#define RT0IP0              (0x0004u)     /* RTC Prescale Timer 0 Interrupt Interval Bit: 0 */
#define RT0PSIE             (0x0002u)     /* RTC Prescale Timer 0 Interrupt Enable Flag */
#define RT0PSIFG            (0x0001u)     /* RTC Prescale Timer 0 Interrupt Flag */

/* RTCPS0CTL Control Bits */
//#define Reserved          (0x0080u)
//#define Reserved          (0x0040u)
//#define Reserved          (0x0020u)
#define RT0IP2_L            (0x0010u)     /* RTC Prescale Timer 0 Interrupt Interval Bit: 2 */
#define RT0IP1_L            (0x0008u)     /* RTC Prescale Timer 0 Interrupt Interval Bit: 1 */
#define RT0IP0_L            (0x0004u)     /* RTC Prescale Timer 0 Interrupt Interval Bit: 0 */
#define RT0PSIE_L           (0x0002u)     /* RTC Prescale Timer 0 Interrupt Enable Flag */
#define RT0PSIFG_L          (0x0001u)     /* RTC Prescale Timer 0 Interrupt Flag */

#define RT0IP_0             (0x0000u)     /* RTC Prescale Timer 0 Interrupt Interval /2 */
#define RT0IP_1             (0x0004u)     /* RTC Prescale Timer 0 Interrupt Interval /4 */
#define RT0IP_2             (0x0008u)     /* RTC Prescale Timer 0 Interrupt Interval /8 */
#define RT0IP_3             (0x000Cu)     /* RTC Prescale Timer 0 Interrupt Interval /16 */
#define RT0IP_4             (0x0010u)     /* RTC Prescale Timer 0 Interrupt Interval /32 */
#define RT0IP_5             (0x0014u)     /* RTC Prescale Timer 0 Interrupt Interval /64 */
#define RT0IP_6             (0x0018u)     /* RTC Prescale Timer 0 Interrupt Interval /128 */
#define RT0IP_7             (0x001Cu)     /* RTC Prescale Timer 0 Interrupt Interval /256 */

#define RT0IP__2            (0x0000u)     /* RTC Prescale Timer 0 Interrupt Interval /2 */
#define RT0IP__4            (0x0004u)     /* RTC Prescale Timer 0 Interrupt Interval /4 */
#define RT0IP__8            (0x0008u)     /* RTC Prescale Timer 0 Interrupt Interval /8 */
#define RT0IP__16           (0x000Cu)     /* RTC Prescale Timer 0 Interrupt Interval /16 */
#define RT0IP__32           (0x0010u)     /* RTC Prescale Timer 0 Interrupt Interval /32 */
#define RT0IP__64           (0x0014u)     /* RTC Prescale Timer 0 Interrupt Interval /64 */
#define RT0IP__128          (0x0018u)     /* RTC Prescale Timer 0 Interrupt Interval /128 */
#define RT0IP__256          (0x001Cu)     /* RTC Prescale Timer 0 Interrupt Interval /256 */

/* RTCPS1CTL Control Bits */
//#define Reserved          (0x0080u)
//#define Reserved          (0x0040u)
//#define Reserved          (0x0020u)
#define RT1IP2              (0x0010u)     /* RTC Prescale Timer 1 Interrupt Interval Bit: 2 */
#define RT1IP1              (0x0008u)     /* RTC Prescale Timer 1 Interrupt Interval Bit: 1 */
#define RT1IP0              (0x0004u)     /* RTC Prescale Timer 1 Interrupt Interval Bit: 0 */
#define RT1PSIE             (0x0002u)     /* RTC Prescale Timer 1 Interrupt Enable Flag */
#define RT1PSIFG            (0x0001u)     /* RTC Prescale Timer 1 Interrupt Flag */

/* RTCPS1CTL Control Bits */
//#define Reserved          (0x0080u)
//#define Reserved          (0x0040u)
//#define Reserved          (0x0020u)
#define RT1IP2_L            (0x0010u)     /* RTC Prescale Timer 1 Interrupt Interval Bit: 2 */
#define RT1IP1_L            (0x0008u)     /* RTC Prescale Timer 1 Interrupt Interval Bit: 1 */
#define RT1IP0_L            (0x0004u)     /* RTC Prescale Timer 1 Interrupt Interval Bit: 0 */
#define RT1PSIE_L           (0x0002u)     /* RTC Prescale Timer 1 Interrupt Enable Flag */
#define RT1PSIFG_L          (0x0001u)     /* RTC Prescale Timer 1 Interrupt Flag */

#define RT1IP_0             (0x0000u)     /* RTC Prescale Timer 1 Interrupt Interval /2 */
#define RT1IP_1             (0x0004u)     /* RTC Prescale Timer 1 Interrupt Interval /4 */
#define RT1IP_2             (0x0008u)     /* RTC Prescale Timer 1 Interrupt Interval /8 */
#define RT1IP_3             (0x000Cu)     /* RTC Prescale Timer 1 Interrupt Interval /16 */
#define RT1IP_4             (0x0010u)     /* RTC Prescale Timer 1 Interrupt Interval /32 */
#define RT1IP_5             (0x0014u)     /* RTC Prescale Timer 1 Interrupt Interval /64 */
#define RT1IP_6             (0x0018u)     /* RTC Prescale Timer 1 Interrupt Interval /128 */
#define RT1IP_7             (0x001Cu)     /* RTC Prescale Timer 1 Interrupt Interval /256 */

#define RT1IP__2            (0x0000u)     /* RTC Prescale Timer 1 Interrupt Interval /2 */
#define RT1IP__4            (0x0004u)     /* RTC Prescale Timer 1 Interrupt Interval /4 */
#define RT1IP__8            (0x0008u)     /* RTC Prescale Timer 1 Interrupt Interval /8 */
#define RT1IP__16           (0x000Cu)     /* RTC Prescale Timer 1 Interrupt Interval /16 */
#define RT1IP__32           (0x0010u)     /* RTC Prescale Timer 1 Interrupt Interval /32 */
#define RT1IP__64           (0x0014u)     /* RTC Prescale Timer 1 Interrupt Interval /64 */
#define RT1IP__128          (0x0018u)     /* RTC Prescale Timer 1 Interrupt Interval /128 */
#define RT1IP__256          (0x001Cu)     /* RTC Prescale Timer 1 Interrupt Interval /256 */

/* RTC Definitions */
#define RTCIV_NONE           (0x0000u)    /* No Interrupt pending */
#define RTCIV_RTCRDYIFG      (0x0002u)    /* RTC ready: RTCRDYIFG */
#define RTCIV_RTCTEVIFG      (0x0004u)    /* RTC interval timer: RTCTEVIFG */
#define RTCIV_RTCAIFG        (0x0006u)    /* RTC user alarm: RTCAIFG */
#define RTCIV_RT0PSIFG       (0x0008u)    /* RTC prescaler 0: RT0PSIFG */
#define RTCIV_RT1PSIFG       (0x000Au)    /* RTC prescaler 1: RT1PSIFG */
#define RTCIV_RTCOFIFG       (0x000Cu)    /* RTC Oscillator fault */

/* Legacy Definitions */
#define RTC_NONE           (0x0000u)      /* No Interrupt pending */
#define RTC_RTCRDYIFG      (0x0002u)      /* RTC ready: RTCRDYIFG */
#define RTC_RTCTEVIFG      (0x0004u)      /* RTC interval timer: RTCTEVIFG */
#define RTC_RTCAIFG        (0x0006u)      /* RTC user alarm: RTCAIFG */
#define RTC_RT0PSIFG       (0x0008u)      /* RTC prescaler 0: RT0PSIFG */
#define RTC_RT1PSIFG       (0x000Au)      /* RTC prescaler 1: RT1PSIFG */
#define RTC_RTCOFIFG       (0x000Cu)      /* RTC Oscillator fault */

#endif
/************************************************************
* Real-Time Clock (RTC) Counter
************************************************************/
#ifdef  __MSP430_HAS_RTC__            /* Definition to show that Module is available */

#define OFS_RTCCTL            (0x0000u)  /* RTC control Register */
#define OFS_RTCCTL_L           OFS_RTCCTL
#define OFS_RTCCTL_H           OFS_RTCCTL+1
#define OFS_RTCIV             (0x0004u)  /* RTC interrupt vector */
#define OFS_RTCIV_L            OFS_RTCIV
#define OFS_RTCIV_H            OFS_RTCIV+1
#define OFS_RTCMOD            (0x0008u)  /* RTC moduloRegister */
#define OFS_RTCMOD_L           OFS_RTCMOD
#define OFS_RTCMOD_H           OFS_RTCMOD+1
#define OFS_RTCCNT            (0x000Cu)  /* RTC counter Register */
#define OFS_RTCCNT_L           OFS_RTCCNT
#define OFS_RTCCNT_H           OFS_RTCCNT+1

/* RTC control Register */
#define RTCIF               (0x0001u)  /* Low-Power-Counter Interrupt Flag */
#define RTCIE               (0x0002u)  /* Low-Power-Counter Interrupt Enable */
#define RTCSR               (0x0040u)  /* Low-Power-Counter Software Reset */
#define RTCPS0              (0x0100u)  /* Low-Power-Counter Clock Pre-divider Select Bit: 0 */
#define RTCPS1              (0x0200u)  /* Low-Power-Counter Clock Pre-divider Select Bit: 1 */
#define RTCPS2              (0x0400u)  /* Low-Power-Counter Clock Pre-divider Select Bit: 2 */
#define RTCSS0              (0x1000u)  /* Low-Power-Counter Clock Source Select Bit: 0 */
#define RTCSS1              (0x2000u)  /* Low-Power-Counter Clock Source Select Bit: 1 */

/* RTC control Register */
#define RTCIF_L             (0x0001u)  /* Low-Power-Counter Interrupt Flag */
#define RTCIE_L             (0x0002u)  /* Low-Power-Counter Interrupt Enable */
#define RTCSR_L             (0x0040u)  /* Low-Power-Counter Software Reset */

/* RTC control Register */
#define RTCPS0_H            (0x0001u)  /* Low-Power-Counter Clock Pre-divider Select Bit: 0 */
#define RTCPS1_H            (0x0002u)  /* Low-Power-Counter Clock Pre-divider Select Bit: 1 */
#define RTCPS2_H            (0x0004u)  /* Low-Power-Counter Clock Pre-divider Select Bit: 2 */
#define RTCSS0_H            (0x0010u)  /* Low-Power-Counter Clock Source Select Bit: 0 */
#define RTCSS1_H            (0x0020u)  /* Low-Power-Counter Clock Source Select Bit: 1 */

#define RTCPS_0             (0x0000u)  /* Low-Power-Counter Clock Pre-divider Select: 0 */
#define RTCPS_1             (0x0100u)  /* Low-Power-Counter Clock Pre-divider Select: 1 */
#define RTCPS_2             (0x0200u)  /* Low-Power-Counter Clock Pre-divider Select: 2 */
#define RTCPS_3             (0x0300u)  /* Low-Power-Counter Clock Pre-divider Select: 3 */
#define RTCPS_4             (0x0400u)  /* Low-Power-Counter Clock Pre-divider Select: 4 */
#define RTCPS_5             (0x0500u)  /* Low-Power-Counter Clock Pre-divider Select: 5 */
#define RTCPS_6             (0x0600u)  /* Low-Power-Counter Clock Pre-divider Select: 6 */
#define RTCPS_7             (0x0700u)  /* Low-Power-Counter Clock Pre-divider Select: 7 */
#define RTCPS__1            (0x0000u)  /* Low-Power-Counter Clock Pre-divider Select: /1 */
#define RTCPS__10           (0x0100u)  /* Low-Power-Counter Clock Pre-divider Select: /10 */
#define RTCPS__100          (0x0200u)  /* Low-Power-Counter Clock Pre-divider Select: /100 */
#define RTCPS__1000         (0x0300u)  /* Low-Power-Counter Clock Pre-divider Select: /1000 */
#define RTCPS__16           (0x0400u)  /* Low-Power-Counter Clock Pre-divider Select: /16 */
#define RTCPS__64           (0x0500u)  /* Low-Power-Counter Clock Pre-divider Select: /64 */
#define RTCPS__256          (0x0600u)  /* Low-Power-Counter Clock Pre-divider Select: /256 */
#define RTCPS__1024         (0x0700u)  /* Low-Power-Counter Clock Pre-divider Select: /1024 */

#define RTCSS_0             (0x0000u)  /* Low-Power-Counter Clock Source Select: 0 */
#define RTCSS_1             (0x1000u)  /* Low-Power-Counter Clock Source Select: 1 */
#define RTCSS_2             (0x2000u)  /* Low-Power-Counter Clock Source Select: 2 */
#define RTCSS_3             (0x3000u)  /* Low-Power-Counter Clock Source Select: 3 */

#define RTCSS__DISABLED     (0x0000u)  /* Low-Power-Counter Clock Source Select: DISABLED  */
#define RTCSS__SMCLK        (0x1000u)  /* Low-Power-Counter Clock Source Select: SMCLK */
#define RTCSS__XT1CLK       (0x2000u)  /* Low-Power-Counter Clock Source Select: XT1CLK */
#define RTCSS__VLOCLK       (0x3000u)  /* Low-Power-Counter Clock Source Select: VLOCLK */

/* RTCIV Definitions */
#define RTCIV_NONE          (0x0000u)  /* No Interrupt pending */
#define RTCIV_RTCIF         (0x0002u)  /* RTC Overflow */


#endif
/************************************************************
* SFR - Special Function Register Module
************************************************************/
#ifdef  __MSP430_HAS_SFR__            /* Definition to show that Module is available */

#define OFS_SFRIE1            (0x0000u)  /* Interrupt Enable 1 */
#define OFS_SFRIE1_L           OFS_SFRIE1
#define OFS_SFRIE1_H           OFS_SFRIE1+1

/* SFRIE1 Control Bits */
#define WDTIE               (0x0001u)  /* WDT Interrupt Enable */
#define OFIE                (0x0002u)  /* Osc Fault Enable */
//#define Reserved          (0x0004u)
#define VMAIE               (0x0008u)  /* Vacant Memory Interrupt Enable */
#define NMIIE               (0x0010u)  /* NMI Interrupt Enable */
#define JMBINIE             (0x0040u)  /* JTAG Mail Box input Interrupt Enable */
#define JMBOUTIE            (0x0080u)  /* JTAG Mail Box output Interrupt Enable */

#define WDTIE_L             (0x0001u)  /* WDT Interrupt Enable */
#define OFIE_L              (0x0002u)  /* Osc Fault Enable */
//#define Reserved          (0x0004u)
#define VMAIE_L             (0x0008u)  /* Vacant Memory Interrupt Enable */
#define NMIIE_L             (0x0010u)  /* NMI Interrupt Enable */
#define JMBINIE_L           (0x0040u)  /* JTAG Mail Box input Interrupt Enable */
#define JMBOUTIE_L          (0x0080u)  /* JTAG Mail Box output Interrupt Enable */

#define OFS_SFRIFG1           (0x0002u)  /* Interrupt Flag 1 */
#define OFS_SFRIFG1_L          OFS_SFRIFG1
#define OFS_SFRIFG1_H          OFS_SFRIFG1+1
/* SFRIFG1 Control Bits */
#define WDTIFG              (0x0001u)  /* WDT Interrupt Flag */
#define OFIFG               (0x0002u)  /* Osc Fault Flag */
//#define Reserved          (0x0004u)
#define VMAIFG              (0x0008u)  /* Vacant Memory Interrupt Flag */
#define NMIIFG              (0x0010u)  /* NMI Interrupt Flag */
//#define Reserved          (0x0020u)
#define JMBINIFG            (0x0040u)  /* JTAG Mail Box input Interrupt Flag */
#define JMBOUTIFG           (0x0080u)  /* JTAG Mail Box output Interrupt Flag */

#define WDTIFG_L            (0x0001u)  /* WDT Interrupt Flag */
#define OFIFG_L             (0x0002u)  /* Osc Fault Flag */
//#define Reserved          (0x0004u)
#define VMAIFG_L            (0x0008u)  /* Vacant Memory Interrupt Flag */
#define NMIIFG_L            (0x0010u)  /* NMI Interrupt Flag */
//#define Reserved          (0x0020u)
#define JMBINIFG_L          (0x0040u)  /* JTAG Mail Box input Interrupt Flag */
#define JMBOUTIFG_L         (0x0080u)  /* JTAG Mail Box output Interrupt Flag */

#define OFS_SFRRPCR           (0x0004u)  /* RESET Pin Control Register */
#define OFS_SFRRPCR_L          OFS_SFRRPCR
#define OFS_SFRRPCR_H          OFS_SFRRPCR+1
/* SFRRPCR Control Bits */
#define SYSNMI              (0x0001u)  /* NMI select */
#define SYSNMIIES           (0x0002u)  /* NMI edge select */
#define SYSRSTUP            (0x0004u)  /* RESET Pin pull down/up select */
#define SYSRSTRE            (0x0008u)  /* RESET Pin Resistor enable */

#define SYSNMI_L            (0x0001u)  /* NMI select */
#define SYSNMIIES_L         (0x0002u)  /* NMI edge select */
#define SYSRSTUP_L          (0x0004u)  /* RESET Pin pull down/up select */
#define SYSRSTRE_L          (0x0008u)  /* RESET Pin Resistor enable */

#endif
/************************************************************
* SYS - System Module
************************************************************/
#ifdef  __MSP430_HAS_SYS__            /* Definition to show that Module is available */

#define OFS_SYSCTL            (0x0000u)  /* System control */
#define OFS_SYSCTL_L           OFS_SYSCTL
#define OFS_SYSCTL_H           OFS_SYSCTL+1
#define OFS_SYSBSLC           (0x0002u)  /* Boot strap configuration area */
#define OFS_SYSBSLC_L          OFS_SYSBSLC
#define OFS_SYSBSLC_H          OFS_SYSBSLC+1
#define OFS_SYSJMBC           (0x0006u)  /* JTAG mailbox control */
#define OFS_SYSJMBC_L          OFS_SYSJMBC
#define OFS_SYSJMBC_H          OFS_SYSJMBC+1
#define OFS_SYSJMBI0          (0x0008u)  /* JTAG mailbox input 0 */
#define OFS_SYSJMBI0_L         OFS_SYSJMBI0
#define OFS_SYSJMBI0_H         OFS_SYSJMBI0+1
#define OFS_SYSJMBI1          (0x000Au)  /* JTAG mailbox input 1 */
#define OFS_SYSJMBI1_L         OFS_SYSJMBI1
#define OFS_SYSJMBI1_H         OFS_SYSJMBI1+1
#define OFS_SYSJMBO0          (0x000Cu)  /* JTAG mailbox output 0 */
#define OFS_SYSJMBO0_L         OFS_SYSJMBO0
#define OFS_SYSJMBO0_H         OFS_SYSJMBO0+1
#define OFS_SYSJMBO1          (0x000Eu)  /* JTAG mailbox output 1 */
#define OFS_SYSJMBO1_L         OFS_SYSJMBO1
#define OFS_SYSJMBO1_H         OFS_SYSJMBO1+1

#define OFS_SYSBERRIV         (0x0018u)  /* Bus Error vector generator */
#define OFS_SYSBERRIV_L        OFS_SYSBERRIV
#define OFS_SYSBERRIV_H        OFS_SYSBERRIV+1
#define OFS_SYSUNIV           (0x001Au)  /* User NMI vector generator */
#define OFS_SYSUNIV_L          OFS_SYSUNIV
#define OFS_SYSUNIV_H          OFS_SYSUNIV+1
#define OFS_SYSSNIV           (0x001Cu)  /* System NMI vector generator */
#define OFS_SYSSNIV_L          OFS_SYSSNIV
#define OFS_SYSSNIV_H          OFS_SYSSNIV+1
#define OFS_SYSRSTIV          (0x001Eu)  /* Reset vector generator */
#define OFS_SYSRSTIV_L         OFS_SYSRSTIV
#define OFS_SYSRSTIV_H         OFS_SYSRSTIV+1
#define OFS_SYSCFG0           (0x0020u)  /* System Configuration 0 */
#define OFS_SYSCFG0_L          OFS_SYSCFG0
#define OFS_SYSCFG0_H          OFS_SYSCFG0+1
#define OFS_SYSCFG1           (0x0022u)  /* System Configuration 1 */
#define OFS_SYSCFG1_L          OFS_SYSCFG1
#define OFS_SYSCFG1_H          OFS_SYSCFG1+1
#define OFS_SYSCFG2           (0x0024u)  /* System Configuration 2 */
#define OFS_SYSCFG2_L          OFS_SYSCFG2
#define OFS_SYSCFG2_H          OFS_SYSCFG2+1

/* SYSCTL Control Bits */
#define SYSRIVECT           (0x0001u)  /* SYS - RAM based interrupt vectors */
//#define RESERVED            (0x0002u)  /* SYS - Reserved */
#define SYSPMMPE            (0x0004u)  /* SYS - PMM access protect */
//#define RESERVED            (0x0008u)  /* SYS - Reserved */
#define SYSBSLIND           (0x0010u)  /* SYS - TCK/RST indication detected */
#define SYSJTAGPIN          (0x0020u)  /* SYS - Dedicated JTAG pins enabled */
//#define RESERVED            (0x0040u)  /* SYS - Reserved */
//#define RESERVED            (0x0080u)  /* SYS - Reserved */
//#define RESERVED            (0x0100u)  /* SYS - Reserved */
//#define RESERVED            (0x0200u)  /* SYS - Reserved */
//#define RESERVED            (0x0400u)  /* SYS - Reserved */
//#define RESERVED            (0x0800u)  /* SYS - Reserved */
//#define RESERVED            (0x1000u)  /* SYS - Reserved */
//#define RESERVED            (0x2000u)  /* SYS - Reserved */
//#define RESERVED            (0x4000u)  /* SYS - Reserved */
//#define RESERVED            (0x8000u)  /* SYS - Reserved */

/* SYSCTL Control Bits */
#define SYSRIVECT_L         (0x0001u)  /* SYS - RAM based interrupt vectors */
//#define RESERVED            (0x0002u)  /* SYS - Reserved */
#define SYSPMMPE_L          (0x0004u)  /* SYS - PMM access protect */
//#define RESERVED            (0x0008u)  /* SYS - Reserved */
#define SYSBSLIND_L         (0x0010u)  /* SYS - TCK/RST indication detected */
#define SYSJTAGPIN_L        (0x0020u)  /* SYS - Dedicated JTAG pins enabled */
//#define RESERVED            (0x0040u)  /* SYS - Reserved */
//#define RESERVED            (0x0080u)  /* SYS - Reserved */
//#define RESERVED            (0x0100u)  /* SYS - Reserved */
//#define RESERVED            (0x0200u)  /* SYS - Reserved */
//#define RESERVED            (0x0400u)  /* SYS - Reserved */
//#define RESERVED            (0x0800u)  /* SYS - Reserved */
//#define RESERVED            (0x1000u)  /* SYS - Reserved */
//#define RESERVED            (0x2000u)  /* SYS - Reserved */
//#define RESERVED            (0x4000u)  /* SYS - Reserved */
//#define RESERVED            (0x8000u)  /* SYS - Reserved */

/* SYSBSLC Control Bits */
#define SYSBSLR             (0x0004u)  /* SYS - RAM assigned to BSL */
//#define RESERVED            (0x0008u)  /* SYS - Reserved */
//#define RESERVED            (0x0010u)  /* SYS - Reserved */
//#define RESERVED            (0x0020u)  /* SYS - Reserved */
//#define RESERVED            (0x0040u)  /* SYS - Reserved */
//#define RESERVED            (0x0080u)  /* SYS - Reserved */
//#define RESERVED            (0x0100u)  /* SYS - Reserved */
//#define RESERVED            (0x0200u)  /* SYS - Reserved */
//#define RESERVED            (0x0400u)  /* SYS - Reserved */
//#define RESERVED            (0x0800u)  /* SYS - Reserved */
//#define RESERVED            (0x1000u)  /* SYS - Reserved */
//#define RESERVED            (0x2000u)  /* SYS - Reserved */
#define SYSBSLOFF           (0x4000u)  /* SYS - BSL Memory disabled */
#define SYSBSLPE            (0x8000u)  /* SYS - BSL Memory protection enabled */

/* SYSBSLC Control Bits */
#define SYSBSLR_L           (0x0004u)  /* SYS - RAM assigned to BSL */
//#define RESERVED            (0x0008u)  /* SYS - Reserved */
//#define RESERVED            (0x0010u)  /* SYS - Reserved */
//#define RESERVED            (0x0020u)  /* SYS - Reserved */
//#define RESERVED            (0x0040u)  /* SYS - Reserved */
//#define RESERVED            (0x0080u)  /* SYS - Reserved */
//#define RESERVED            (0x0100u)  /* SYS - Reserved */
//#define RESERVED            (0x0200u)  /* SYS - Reserved */
//#define RESERVED            (0x0400u)  /* SYS - Reserved */
//#define RESERVED            (0x0800u)  /* SYS - Reserved */
//#define RESERVED            (0x1000u)  /* SYS - Reserved */
//#define RESERVED            (0x2000u)  /* SYS - Reserved */

/* SYSBSLC Control Bits */
//#define RESERVED            (0x0008u)  /* SYS - Reserved */
//#define RESERVED            (0x0010u)  /* SYS - Reserved */
//#define RESERVED            (0x0020u)  /* SYS - Reserved */
//#define RESERVED            (0x0040u)  /* SYS - Reserved */
//#define RESERVED            (0x0080u)  /* SYS - Reserved */
//#define RESERVED            (0x0100u)  /* SYS - Reserved */
//#define RESERVED            (0x0200u)  /* SYS - Reserved */
//#define RESERVED            (0x0400u)  /* SYS - Reserved */
//#define RESERVED            (0x0800u)  /* SYS - Reserved */
//#define RESERVED            (0x1000u)  /* SYS - Reserved */
//#define RESERVED            (0x2000u)  /* SYS - Reserved */
#define SYSBSLOFF_H         (0x0040u)  /* SYS - BSL Memory disabled */
#define SYSBSLPE_H          (0x0080u)  /* SYS - BSL Memory protection enabled */

/* SYSJMBC Control Bits */
#define JMBIN0FG            (0x0001u)  /* SYS - Incoming JTAG Mailbox 0 Flag */
#define JMBIN1FG            (0x0002u)  /* SYS - Incoming JTAG Mailbox 1 Flag */
#define JMBOUT0FG           (0x0004u)  /* SYS - Outgoing JTAG Mailbox 0 Flag */
#define JMBOUT1FG           (0x0008u)  /* SYS - Outgoing JTAG Mailbox 1 Flag */
#define JMBMODE             (0x0010u)  /* SYS - JMB 16/32 Bit Mode */
//#define RESERVED            (0x0020u)  /* SYS - Reserved */
#define JMBCLR0OFF          (0x0040u)  /* SYS - Incoming JTAG Mailbox 0 Flag auto-clear disalbe */
#define JMBCLR1OFF          (0x0080u)  /* SYS - Incoming JTAG Mailbox 1 Flag auto-clear disalbe */
//#define RESERVED            (0x0100u)  /* SYS - Reserved */
//#define RESERVED            (0x0200u)  /* SYS - Reserved */
//#define RESERVED            (0x0400u)  /* SYS - Reserved */
//#define RESERVED            (0x0800u)  /* SYS - Reserved */
//#define RESERVED            (0x1000u)  /* SYS - Reserved */
//#define RESERVED            (0x2000u)  /* SYS - Reserved */
//#define RESERVED            (0x4000u)  /* SYS - Reserved */
//#define RESERVED            (0x8000u)  /* SYS - Reserved */

/* SYSJMBC Control Bits */
#define JMBIN0FG_L          (0x0001u)  /* SYS - Incoming JTAG Mailbox 0 Flag */
#define JMBIN1FG_L          (0x0002u)  /* SYS - Incoming JTAG Mailbox 1 Flag */
#define JMBOUT0FG_L         (0x0004u)  /* SYS - Outgoing JTAG Mailbox 0 Flag */
#define JMBOUT1FG_L         (0x0008u)  /* SYS - Outgoing JTAG Mailbox 1 Flag */
#define JMBMODE_L           (0x0010u)  /* SYS - JMB 16/32 Bit Mode */
//#define RESERVED            (0x0020u)  /* SYS - Reserved */
#define JMBCLR0OFF_L        (0x0040u)  /* SYS - Incoming JTAG Mailbox 0 Flag auto-clear disalbe */
#define JMBCLR1OFF_L        (0x0080u)  /* SYS - Incoming JTAG Mailbox 1 Flag auto-clear disalbe */
//#define RESERVED            (0x0100u)  /* SYS - Reserved */
//#define RESERVED            (0x0200u)  /* SYS - Reserved */
//#define RESERVED            (0x0400u)  /* SYS - Reserved */
//#define RESERVED            (0x0800u)  /* SYS - Reserved */
//#define RESERVED            (0x1000u)  /* SYS - Reserved */
//#define RESERVED            (0x2000u)  /* SYS - Reserved */
//#define RESERVED            (0x4000u)  /* SYS - Reserved */
//#define RESERVED            (0x8000u)  /* SYS - Reserved */

/* SYSCFG0 Control Bits */
#define PFWP                (0x0001u)  /* Program FRAM Write Protection */
#define DFWP                (0x0002u)  /* Data FRAM Write Protection */

/* SYSCFG0 Control Bits */
#define PFWP_L              (0x0001u)  /* Program FRAM Write Protection */
#define DFWP_L              (0x0002u)  /* Data FRAM Write Protection */

/* SYSCFG1 Control Bits */
#define IREN                (0x0001u)  /* Infrared enable */
#define IRPSEL              (0x0002u)  /* Infrared polarity select */
#define IRMSEL              (0x0004u)  /* Infrared mode select */
#define IRDSSEL             (0x0008u)  /* Infrared data source select */
#define IRDATA              (0x0010u)  /* Infrared enable */

/* SYSCFG1 Control Bits */
#define IREN_L              (0x0001u)  /* Infrared enable */
#define IRPSEL_L            (0x0002u)  /* Infrared polarity select */
#define IRMSEL_L            (0x0004u)  /* Infrared mode select */
#define IRDSSEL_L           (0x0008u)  /* Infrared data source select */
#define IRDATA_L            (0x0010u)  /* Infrared enable */

/* SYSCFG2 Control Bits */
#define ADCPCTL0            (0x0001u)  /* ADC input A0 pin select */
#define ADCPCTL1            (0x0002u)  /* ADC input A1 pin select */
#define ADCPCTL2            (0x0004u)  /* ADC input A2 pin select */
#define ADCPCTL3            (0x0008u)  /* ADC input A3 pin select */
#define ADCPCTL4            (0x0010u)  /* ADC input A4 pin select */
#define ADCPCTL5            (0x0020u)  /* ADC input A5 pin select */
#define ADCPCTL6            (0x0040u)  /* ADC input A6 pin select */
#define ADCPCTL7            (0x0080u)  /* ADC input A7 pin select */
#define ADCPCTL8            (0x0100u)  /* ADC input A8 pin select */
#define ADCPCTL9            (0x0200u)  /* ADC input A9 pin select */
#define LCDPCTL             (0x1000u)  /* LCD Power Pin */

/* SYSCFG2 Control Bits */
#define ADCPCTL0_L          (0x0001u)  /* ADC input A0 pin select */
#define ADCPCTL1_L          (0x0002u)  /* ADC input A1 pin select */
#define ADCPCTL2_L          (0x0004u)  /* ADC input A2 pin select */
#define ADCPCTL3_L          (0x0008u)  /* ADC input A3 pin select */
#define ADCPCTL4_L          (0x0010u)  /* ADC input A4 pin select */
#define ADCPCTL5_L          (0x0020u)  /* ADC input A5 pin select */
#define ADCPCTL6_L          (0x0040u)  /* ADC input A6 pin select */
#define ADCPCTL7_L          (0x0080u)  /* ADC input A7 pin select */

/* SYSCFG2 Control Bits */
#define ADCPCTL8_H          (0x0001u)  /* ADC input A8 pin select */
#define ADCPCTL9_H          (0x0002u)  /* ADC input A9 pin select */
#define LCDPCTL_H           (0x0010u)  /* LCD Power Pin */


#endif
/************************************************************
* Timerx_A7
************************************************************/
#ifdef  __MSP430_HAS_TxA7__            /* Definition to show that Module is available */

#define OFS_TAxCTL            (0x0000u)  /* Timerx_A7 Control */
#define OFS_TAxCCTL0          (0x0002u)  /* Timerx_A7 Capture/Compare Control 0 */
#define OFS_TAxCCTL1          (0x0004u)  /* Timerx_A7 Capture/Compare Control 1 */
#define OFS_TAxCCTL2          (0x0006u)  /* Timerx_A7 Capture/Compare Control 2 */
#define OFS_TAxCCTL3          (0x0008u)  /* Timerx_A7 Capture/Compare Control 3 */
#define OFS_TAxCCTL4          (0x000Au)  /* Timerx_A7 Capture/Compare Control 4 */
#define OFS_TAxCCTL5          (0x000Cu)  /* Timerx_A7 Capture/Compare Control 5 */
#define OFS_TAxCCTL6          (0x000Eu)  /* Timerx_A7 Capture/Compare Control 6 */
#define OFS_TAxR              (0x0010u)  /* Timerx_A7 */
#define OFS_TAxCCR0           (0x0012u)  /* Timerx_A7 Capture/Compare 0 */
#define OFS_TAxCCR1           (0x0014u)  /* Timerx_A7 Capture/Compare 1 */
#define OFS_TAxCCR2           (0x0016u)  /* Timerx_A7 Capture/Compare 2 */
#define OFS_TAxCCR3           (0x0018u)  /* Timerx_A7 Capture/Compare 3 */
#define OFS_TAxCCR4           (0x001Au)  /* Timerx_A7 Capture/Compare 4 */
#define OFS_TAxCCR5           (0x001Cu)  /* Timerx_A7 Capture/Compare 5 */
#define OFS_TAxCCR6           (0x001Eu)  /* Timerx_A7 Capture/Compare 6 */
#define OFS_TAxIV             (0x002Eu)  /* Timerx_A7 Interrupt Vector Word */
#define OFS_TAxEX0            (0x0020u)  /* Timerx_A7 Expansion Register 0 */

/* Bits are already defined within the Timer0_Ax */

/* TAxIV Definitions */
#define TAxIV_NONE          (0x0000u)    /* No Interrupt pending */
#define TAxIV_TACCR1        (0x0002u)    /* TAxCCR1_CCIFG */
#define TAxIV_TACCR2        (0x0004u)    /* TAxCCR2_CCIFG */
#define TAxIV_TACCR3        (0x0006u)    /* TAxCCR3_CCIFG */
#define TAxIV_TACCR4        (0x0008u)    /* TAxCCR4_CCIFG */
#define TAxIV_TACCR5        (0x000Au)    /* TAxCCR5_CCIFG */
#define TAxIV_TACCR6        (0x000Cu)    /* TAxCCR6_CCIFG */
#define TAxIV_TAIFG         (0x000Eu)    /* TAxIFG */

/* Legacy Defines */
#define TAxIV_TAxCCR1      (0x0002u)    /* TAxCCR1_CCIFG */
#define TAxIV_TAxCCR2      (0x0004u)    /* TAxCCR2_CCIFG */
#define TAxIV_TAxCCR3      (0x0006u)    /* TAxCCR3_CCIFG */
#define TAxIV_TAxCCR4      (0x0008u)    /* TAxCCR4_CCIFG */
#define TAxIV_TAxCCR5      (0x000Au)    /* TAxCCR5_CCIFG */
#define TAxIV_TAxCCR6      (0x000Cu)    /* TAxCCR6_CCIFG */
#define TAxIV_TAxIFG       (0x000Eu)    /* TAxIFG */

/* TAxCTL Control Bits */
#define TASSEL1             (0x0200u)  /* Timer A clock source select 1 */
#define TASSEL0             (0x0100u)  /* Timer A clock source select 0 */
#define ID1                 (0x0080u)  /* Timer A clock input divider 1 */
#define ID0                 (0x0040u)  /* Timer A clock input divider 0 */
#define MC1                 (0x0020u)  /* Timer A mode control 1 */
#define MC0                 (0x0010u)  /* Timer A mode control 0 */
#define TACLR               (0x0004u)  /* Timer A counter clear */
#define TAIE                (0x0002u)  /* Timer A counter interrupt enable */
#define TAIFG               (0x0001u)  /* Timer A counter interrupt flag */

#define MC_0                (0*0x10u)  /* Timer A mode control: 0 - Stop */
#define MC_1                (1*0x10u)  /* Timer A mode control: 1 - Up to CCR0 */
#define MC_2                (2*0x10u)  /* Timer A mode control: 2 - Continuous up */
#define MC_3                (3*0x10u)  /* Timer A mode control: 3 - Up/Down */
#define ID_0                (0*0x40u)  /* Timer A input divider: 0 - /1 */
#define ID_1                (1*0x40u)  /* Timer A input divider: 1 - /2 */
#define ID_2                (2*0x40u)  /* Timer A input divider: 2 - /4 */
#define ID_3                (3*0x40u)  /* Timer A input divider: 3 - /8 */
#define TASSEL_0            (0*0x100u) /* Timer A clock source select: 0 - TACLK */
#define TASSEL_1            (1*0x100u) /* Timer A clock source select: 1 - ACLK  */
#define TASSEL_2            (2*0x100u) /* Timer A clock source select: 2 - SMCLK */
#define TASSEL_3            (3*0x100u) /* Timer A clock source select: 3 - INCLK */
#define MC__STOP            (0*0x10u)  /* Timer A mode control: 0 - Stop */
#define MC__UP              (1*0x10u)  /* Timer A mode control: 1 - Up to CCR0 */
#define MC__CONTINUOUS      (2*0x10u)  /* Timer A mode control: 2 - Continuous up */
#define MC__CONTINOUS       (2*0x10u)  /* Legacy define */
#define MC__UPDOWN          (3*0x10u)  /* Timer A mode control: 3 - Up/Down */
#define ID__1               (0*0x40u)  /* Timer A input divider: 0 - /1 */
#define ID__2               (1*0x40u)  /* Timer A input divider: 1 - /2 */
#define ID__4               (2*0x40u)  /* Timer A input divider: 2 - /4 */
#define ID__8               (3*0x40u)  /* Timer A input divider: 3 - /8 */
#define TASSEL__TACLK       (0*0x100u) /* Timer A clock source select: 0 - TACLK */
#define TASSEL__ACLK        (1*0x100u) /* Timer A clock source select: 1 - ACLK  */
#define TASSEL__SMCLK       (2*0x100u) /* Timer A clock source select: 2 - SMCLK */
#define TASSEL__INCLK       (3*0x100u) /* Timer A clock source select: 3 - INCLK */

/* TAxCCTLx Control Bits */
#define CM1                 (0x8000u)  /* Capture mode 1 */
#define CM0                 (0x4000u)  /* Capture mode 0 */
#define CCIS1               (0x2000u)  /* Capture input select 1 */
#define CCIS0               (0x1000u)  /* Capture input select 0 */
#define SCS                 (0x0800u)  /* Capture sychronize */
#define SCCI                (0x0400u)  /* Latched capture signal (read) */
#define CAP                 (0x0100u)  /* Capture mode: 1 /Compare mode : 0 */
#define OUTMOD2             (0x0080u)  /* Output mode 2 */
#define OUTMOD1             (0x0040u)  /* Output mode 1 */
#define OUTMOD0             (0x0020u)  /* Output mode 0 */
#define CCIE                (0x0010u)  /* Capture/compare interrupt enable */
#define CCI                 (0x0008u)  /* Capture input signal (read) */
#define OUT                 (0x0004u)  /* PWM Output signal if output mode 0 */
#define COV                 (0x0002u)  /* Capture/compare overflow flag */
#define CCIFG               (0x0001u)  /* Capture/compare interrupt flag */

#define OUTMOD_0            (0*0x20u)  /* PWM output mode: 0 - output only */
#define OUTMOD_1            (1*0x20u)  /* PWM output mode: 1 - set */
#define OUTMOD_2            (2*0x20u)  /* PWM output mode: 2 - PWM toggle/reset */
#define OUTMOD_3            (3*0x20u)  /* PWM output mode: 3 - PWM set/reset */
#define OUTMOD_4            (4*0x20u)  /* PWM output mode: 4 - toggle */
#define OUTMOD_5            (5*0x20u)  /* PWM output mode: 5 - Reset */
#define OUTMOD_6            (6*0x20u)  /* PWM output mode: 6 - PWM toggle/set */
#define OUTMOD_7            (7*0x20u)  /* PWM output mode: 7 - PWM reset/set */
#define CCIS_0              (0*0x1000u) /* Capture input select: 0 - CCIxA */
#define CCIS_1              (1*0x1000u) /* Capture input select: 1 - CCIxB */
#define CCIS_2              (2*0x1000u) /* Capture input select: 2 - GND */
#define CCIS_3              (3*0x1000u) /* Capture input select: 3 - Vcc */
#define CM_0                (0*0x4000u) /* Capture mode: 0 - disabled */
#define CM_1                (1*0x4000u) /* Capture mode: 1 - pos. edge */
#define CM_2                (2*0x4000u) /* Capture mode: 1 - neg. edge */
#define CM_3                (3*0x4000u) /* Capture mode: 1 - both edges */

/* TAxEX0 Control Bits */
#define TAIDEX0             (0x0001u)  /* Timer A Input divider expansion Bit: 0 */
#define TAIDEX1             (0x0002u)  /* Timer A Input divider expansion Bit: 1 */
#define TAIDEX2             (0x0004u)  /* Timer A Input divider expansion Bit: 2 */

#define TAIDEX_0            (0*0x0001u) /* Timer A Input divider expansion : /1 */
#define TAIDEX_1            (1*0x0001u) /* Timer A Input divider expansion : /2 */
#define TAIDEX_2            (2*0x0001u) /* Timer A Input divider expansion : /3 */
#define TAIDEX_3            (3*0x0001u) /* Timer A Input divider expansion : /4 */
#define TAIDEX_4            (4*0x0001u) /* Timer A Input divider expansion : /5 */
#define TAIDEX_5            (5*0x0001u) /* Timer A Input divider expansion : /6 */
#define TAIDEX_6            (6*0x0001u) /* Timer A Input divider expansion : /7 */
#define TAIDEX_7            (7*0x0001u) /* Timer A Input divider expansion : /8 */

#endif
/************************************************************
* Timerx_B7
************************************************************/
#ifdef  __MSP430_HAS_TxB7__            /* Definition to show that Module is available */

#define OFS_TBxCTL            (0x0000u)  /* Timerx_B7 Control */
#define OFS_TBxCCTL0          (0x0002u)  /* Timerx_B7 Capture/Compare Control 0 */
#define OFS_TBxCCTL1          (0x0004u)  /* Timerx_B7 Capture/Compare Control 1 */
#define OFS_TBxCCTL2          (0x0006u)  /* Timerx_B7 Capture/Compare Control 2 */
#define OFS_TBxCCTL3          (0x0008u)  /* Timerx_B7 Capture/Compare Control 3 */
#define OFS_TBxCCTL4          (0x000Au)  /* Timerx_B7 Capture/Compare Control 4 */
#define OFS_TBxCCTL5          (0x000Cu)  /* Timerx_B7 Capture/Compare Control 5 */
#define OFS_TBxCCTL6          (0x000Eu)  /* Timerx_B7 Capture/Compare Control 6 */
#define OFS_TBxR              (0x0010u)  /* Timerx_B7 */
#define OFS_TBxCCR0           (0x0012u)  /* Timerx_B7 Capture/Compare 0 */
#define OFS_TBxCCR1           (0x0014u)  /* Timerx_B7 Capture/Compare 1 */
#define OFS_TBxCCR2           (0x0016u)  /* Timerx_B7 Capture/Compare 2 */
#define OFS_TBxCCR3           (0x0018u)  /* Timerx_B7 Capture/Compare 3 */
#define OFS_TBxCCR4           (0x001Au)  /* Timerx_B7 Capture/Compare 4 */
#define OFS_TBxCCR5           (0x001Cu)  /* Timerx_B7 Capture/Compare 5 */
#define OFS_TBxCCR6           (0x001Eu)  /* Timerx_B7 Capture/Compare 6 */
#define OFS_TBxIV             (0x002Eu)  /* Timerx_B7 Interrupt Vector Word */
#define OFS_TBxEX0            (0x0020u)  /* Timerx_B7 Expansion Register 0 */

/* Bits are already defined within the Timer0_Ax */

/* TBxIV Definitions */
#define TBxIV_NONE           (0x0000u)    /* No Interrupt pending */
#define TBxIV_TBCCR1         (0x0002u)    /* TBxCCR1_CCIFG */
#define TBxIV_TBCCR2         (0x0004u)    /* TBxCCR2_CCIFG */
#define TBxIV_TBCCR3         (0x0006u)    /* TBxCCR3_CCIFG */
#define TBxIV_TBCCR4         (0x0008u)    /* TBxCCR4_CCIFG */
#define TBxIV_TBCCR5         (0x000Au)    /* TBxCCR5_CCIFG */
#define TBxIV_TBCCR6         (0x000Cu)    /* TBxCCR6_CCIFG */
#define TBxIV_TBIFG          (0x000Eu)    /* TBxIFG */

/* Legacy Defines */
#define TBxIV_TBxCCR1       (0x0002u)    /* TBxCCR1_CCIFG */
#define TBxIV_TBxCCR2       (0x0004u)    /* TBxCCR2_CCIFG */
#define TBxIV_TBxCCR3       (0x0006u)    /* TBxCCR3_CCIFG */
#define TBxIV_TBxCCR4       (0x0008u)    /* TBxCCR4_CCIFG */
#define TBxIV_TBxCCR5       (0x000Au)    /* TBxCCR5_CCIFG */
#define TBxIV_TBxCCR6       (0x000Cu)    /* TBxCCR6_CCIFG */
#define TBxIV_TBxIFG        (0x000Eu)    /* TBxIFG */

/* TBxCTL Control Bits */
#define TBCLGRP1            (0x4000u)    /* Timer_B7 Compare latch load group 1 */
#define TBCLGRP0            (0x2000u)    /* Timer_B7 Compare latch load group 0 */
#define CNTL1               (0x1000u)    /* Counter lenght 1 */
#define CNTL0               (0x0800u)    /* Counter lenght 0 */
#define TBSSEL1             (0x0200u)    /* Clock source 1 */
#define TBSSEL0             (0x0100u)    /* Clock source 0 */
#define TBCLR               (0x0004u)    /* Timer_B7 counter clear */
#define TBIE                (0x0002u)    /* Timer_B7 interrupt enable */
#define TBIFG               (0x0001u)    /* Timer_B7 interrupt flag */

#define SHR1                (0x4000u)    /* Timer_B7 Compare latch load group 1 */
#define SHR0                (0x2000u)    /* Timer_B7 Compare latch load group 0 */

#define TBSSEL_0            (0*0x0100u)  /* Clock Source: TBCLK */
#define TBSSEL_1            (1*0x0100u)  /* Clock Source: ACLK  */
#define TBSSEL_2            (2*0x0100u)  /* Clock Source: SMCLK */
#define TBSSEL_3            (3*0x0100u)  /* Clock Source: INCLK */
#define CNTL_0              (0*0x0800u)  /* Counter lenght: 16 bit */
#define CNTL_1              (1*0x0800u)  /* Counter lenght: 12 bit */
#define CNTL_2              (2*0x0800u)  /* Counter lenght: 10 bit */
#define CNTL_3              (3*0x0800u)  /* Counter lenght:  8 bit */
#define SHR_0               (0*0x2000u)  /* Timer_B7 Group: 0 - individually */
#define SHR_1               (1*0x2000u)  /* Timer_B7 Group: 1 - 3 groups (1-2, 3-4, 5-6) */
#define SHR_2               (2*0x2000u)  /* Timer_B7 Group: 2 - 2 groups (1-3, 4-6)*/
#define SHR_3               (3*0x2000u)  /* Timer_B7 Group: 3 - 1 group (all) */
#define TBCLGRP_0           (0*0x2000u)  /* Timer_B7 Group: 0 - individually */
#define TBCLGRP_1           (1*0x2000u)  /* Timer_B7 Group: 1 - 3 groups (1-2, 3-4, 5-6) */
#define TBCLGRP_2           (2*0x2000u)  /* Timer_B7 Group: 2 - 2 groups (1-3, 4-6)*/
#define TBCLGRP_3           (3*0x2000u)  /* Timer_B7 Group: 3 - 1 group (all) */
#define TBSSEL__TBCLK       (0*0x100u) /* Timer0_B7 clock source select: 0 - TBCLK */
#define TBSSEL__TACLK       (0*0x100u) /* Timer0_B7 clock source select: 0 - TBCLK (legacy) */
#define TBSSEL__ACLK        (1*0x100u)  /* Timer_B7 clock source select: 1 - ACLK  */
#define TBSSEL__SMCLK       (2*0x100u)  /* Timer_B7 clock source select: 2 - SMCLK */
#define TBSSEL__INCLK       (3*0x100u)  /* Timer_B7 clock source select: 3 - INCLK */
#define CNTL__16            (0*0x0800u)  /* Counter lenght: 16 bit */
#define CNTL__12            (1*0x0800u)  /* Counter lenght: 12 bit */
#define CNTL__10            (2*0x0800u)  /* Counter lenght: 10 bit */
#define CNTL__8             (3*0x0800u)  /* Counter lenght:  8 bit */

/* Additional Timer B Control Register bits are defined in Timer A */
/* TBxCCTLx Control Bits */
#define CLLD1               (0x0400u)  /* Compare latch load source 1 */
#define CLLD0               (0x0200u)  /* Compare latch load source 0 */

#define SLSHR1              (0x0400u)  /* Compare latch load source 1 */
#define SLSHR0              (0x0200u)  /* Compare latch load source 0 */

#define SLSHR_0             (0*0x0200u) /* Compare latch load sourec : 0 - immediate */
#define SLSHR_1             (1*0x0200u) /* Compare latch load sourec : 1 - TBR counts to 0 */
#define SLSHR_2             (2*0x0200u) /* Compare latch load sourec : 2 - up/down */
#define SLSHR_3             (3*0x0200u) /* Compare latch load sourec : 3 - TBR counts to TBCTL0 */

#define CLLD_0              (0*0x0200u) /* Compare latch load sourec : 0 - immediate */
#define CLLD_1              (1*0x0200u) /* Compare latch load sourec : 1 - TBR counts to 0 */
#define CLLD_2              (2*0x0200u) /* Compare latch load sourec : 2 - up/down */
#define CLLD_3              (3*0x0200u) /* Compare latch load sourec : 3 - TBR counts to TBCTL0 */

/* TBxEX0 Control Bits */
#define TBIDEX0             (0x0001u)   /* Timer_B7 Input divider expansion Bit: 0 */
#define TBIDEX1             (0x0002u)   /* Timer_B7 Input divider expansion Bit: 1 */
#define TBIDEX2             (0x0004u)   /* Timer_B7 Input divider expansion Bit: 2 */

#define TBIDEX_0            (0*0x0001u) /* Timer_B7 Input divider expansion : /1 */
#define TBIDEX_1            (1*0x0001u) /* Timer_B7 Input divider expansion : /2 */
#define TBIDEX_2            (2*0x0001u) /* Timer_B7 Input divider expansion : /3 */
#define TBIDEX_3            (3*0x0001u) /* Timer_B7 Input divider expansion : /4 */
#define TBIDEX_4            (4*0x0001u) /* Timer_B7 Input divider expansion : /5 */
#define TBIDEX_5            (5*0x0001u) /* Timer_B7 Input divider expansion : /6 */
#define TBIDEX_6            (6*0x0001u) /* Timer_B7 Input divider expansion : /7 */
#define TBIDEX_7            (7*0x0001u) /* Timer_B7 Input divider expansion : /8 */
#define TBIDEX__1           (0*0x0001u) /* Timer_B7 Input divider expansion : /1 */
#define TBIDEX__2           (1*0x0001u) /* Timer_B7 Input divider expansion : /2 */
#define TBIDEX__3           (2*0x0001u) /* Timer_B7 Input divider expansion : /3 */
#define TBIDEX__4           (3*0x0001u) /* Timer_B7 Input divider expansion : /4 */
#define TBIDEX__5           (4*0x0001u) /* Timer_B7 Input divider expansion : /5 */
#define TBIDEX__6           (5*0x0001u) /* Timer_B7 Input divider expansion : /6 */
#define TBIDEX__7           (6*0x0001u) /* Timer_B7 Input divider expansion : /7 */
#define TBIDEX__8           (7*0x0001u) /* Timer_B7 Input divider expansion : /8 */


#define ID1                    (0x0080u)       /* Timer B clock input divider 1 */
#define ID0                    (0x0040u)       /* Timer B clock input divider 0 */
#define MC1                    (0x0020u)       /* Timer B mode control 1 */
#define MC0                    (0x0010u)       /* Timer B mode control 0 */
#define MC__STOP               (0*0x10u)      /* Timer B mode control: 0 - Stop */
#define MC__UP                 (1*0x10u)      /* Timer B mode control: 1 - Up to CCR0 */
#define MC__CONTINUOUS         (2*0x10u)      /* Timer B mode control: 2 - Continuous up */
#define MC__CONTINOUS          (2*0x10u)      /* Legacy define */
#define MC__UPDOWN             (3*0x10u)      /* Timer B mode control: 3 - Up/Down */
#define CM1                    (0x8000u)       /* Capture mode 1 */
#define CM0                    (0x4000u)       /* Capture mode 0 */
#define MC_0                   (0*0x10u)      /* Timer B mode control: 0 - Stop */
#define MC_1                   (1*0x10u)      /* Timer B mode control: 1 - Up to CCR0 */
#define MC_2                   (2*0x10u)      /* Timer B mode control: 2 - Continuous up */
#define MC_3                   (3*0x10u)      /* Timer B mode control: 3 - Up/Down */
#define CAP                    (0x0100u)       /* Capture mode: 1 /Compare mode : 0 */
#define CCIE                   (0x0010u)       /* Capture/compare interrupt enable */
#define CCIFG                  (0x0001u)       /* Capture/compare interrupt flag */
#define CCIS_0                 (0*0x1000u)
#define CCIS_1                 (1*0x1000u)
#define CCIS_2                 (2*0x1000u)
#define CCIS_3                 (3*0x1000u)
#define CM_0                   (0*0x4000u)    /* Capture mode: 0 - disabled */
#define CM_1                   (1*0x4000u)    /* Capture mode: 1 - pos. edge */
#define CM_2                   (2*0x4000u)    /* Capture mode: 1 - neg. edge */
#define CM_3                   (3*0x4000u)    /* Capture mode: 1 - both edges */
#define OUT                    (0x0004u)       /* PWM Output signal if output mode 0 */
#define OUTMOD_0               (0*0x20u)      /* PWM output mode: 0 - output only */
#define OUTMOD_1               (1*0x20u)      /* PWM output mode: 1 - set */
#define OUTMOD_2               (2*0x20u)      /* PWM output mode: 2 - PWM toggle/reset */
#define OUTMOD_3               (3*0x20u)      /* PWM output mode: 3 - PWM set/reset */
#define OUTMOD_4               (4*0x20u)      /* PWM output mode: 4 - toggle */
#define OUTMOD_5               (5*0x20u)      /* PWM output mode: 5 - Reset */
#define OUTMOD_6               (6*0x20u)      /* PWM output mode: 6 - PWM toggle/set */
#define OUTMOD_7               (7*0x20u)      /* PWM output mode: 7 - PWM reset/set */
#define SCCI                   (0x0400u)       /* Latched capture signal (read) */
#define SCS                    (0x0800u)       /* Capture sychronize */
#define CCI                    (0x0008u)       /* Capture input signal (read) */
#define ID__1                  (0*0x40u)      /* Timer B input divider: 0 - /1 */
#define ID__2                  (1*0x40u)      /* Timer B input divider: 1 - /2 */
#define ID__4                  (2*0x40u)      /* Timer B input divider: 2 - /4 */
#define ID__8                  (3*0x40u)      /* Timer B input divider: 3 - /8 */
#define ID_0                   (0*0x40u)      /* Timer B input divider: 0 - /1 */
#define ID_1                   (1*0x40u)      /* Timer B input divider: 1 - /2 */
#define ID_2                   (2*0x40u)      /* Timer B input divider: 2 - /4 */
#define ID_3                   (3*0x40u)      /* Timer B input divider: 3 - /8 */

#endif
/************************************************************
* USCI Ax
************************************************************/
#ifdef  __MSP430_HAS_EUSCI_Ax__      /* Definition to show that Module is available */

#define OFS_UCAxCTLW0         (0x0000u)  /* USCI Ax Control Word Register 0 */
#define OFS_UCAxCTLW0_L        OFS_UCAxCTLW0
#define OFS_UCAxCTLW0_H        OFS_UCAxCTLW0+1
#define OFS_UCAxCTL0          (0x0001u)
#define OFS_UCAxCTL1          (0x0000u)
#define UCAxCTL1            UCAxCTLW0_L  /* USCI Ax Control Register 1 */
#define UCAxCTL0            UCAxCTLW0_H  /* USCI Ax Control Register 0 */
#define OFS_UCAxCTLW1         (0x0002u)  /* USCI Ax Control Word Register 1 */
#define OFS_UCAxCTLW1_L        OFS_UCAxCTLW1
#define OFS_UCAxCTLW1_H        OFS_UCAxCTLW1+1
#define OFS_UCAxBRW           (0x0006u)  /* USCI Ax Baud Word Rate 0 */
#define OFS_UCAxBRW_L          OFS_UCAxBRW
#define OFS_UCAxBRW_H          OFS_UCAxBRW+1
#define OFS_UCAxBR0           (0x0006u)
#define OFS_UCAxBR1           (0x0007u)
#define UCAxBR0             UCAxBRW_L /* USCI Ax Baud Rate 0 */
#define UCAxBR1             UCAxBRW_H /* USCI Ax Baud Rate 1 */
#define OFS_UCAxMCTLW         (0x0008u)  /* USCI Ax Modulation Control */
#define OFS_UCAxMCTLW_L        OFS_UCAxMCTLW
#define OFS_UCAxMCTLW_H        OFS_UCAxMCTLW+1
#define OFS_UCAxSTATW         (0x000Au)  /* USCI Ax Status Register */
#define OFS_UCAxRXBUF         (0x000Cu)  /* USCI Ax Receive Buffer */
#define OFS_UCAxRXBUF_L        OFS_UCAxRXBUF
#define OFS_UCAxRXBUF_H        OFS_UCAxRXBUF+1
#define OFS_UCAxTXBUF         (0x000Eu)  /* USCI Ax Transmit Buffer */
#define OFS_UCAxTXBUF_L        OFS_UCAxTXBUF
#define OFS_UCAxTXBUF_H        OFS_UCAxTXBUF+1
#define OFS_UCAxABCTL         (0x0010u)  /* USCI Ax LIN Control */
#define OFS_UCAxIRCTL         (0x0012u)  /* USCI Ax IrDA Transmit Control */
#define OFS_UCAxIRCTL_L        OFS_UCAxIRCTL
#define OFS_UCAxIRCTL_H        OFS_UCAxIRCTL+1
#define OFS_UCAxIRTCTL        (0x0012u)
#define OFS_UCAxIRRCTL        (0x0013u)
#define UCAxIRTCTL          UCAxIRCTL_L  /* USCI Ax IrDA Transmit Control */
#define UCAxIRRCTL          UCAxIRCTL_H  /* USCI Ax IrDA Receive Control */
#define OFS_UCAxIE            (0x001Au)  /* USCI Ax Interrupt Enable Register */
#define OFS_UCAxIE_L           OFS_UCAxIE
#define OFS_UCAxIE_H           OFS_UCAxIE+1
#define OFS_UCAxIFG           (0x001Cu)  /* USCI Ax Interrupt Flags Register */
#define OFS_UCAxIFG_L          OFS_UCAxIFG
#define OFS_UCAxIFG_H          OFS_UCAxIFG+1
#define OFS_UCAxIE__UART      (0x001Au)
#define OFS_UCAxIE__UART_L     OFS_UCAxIE__UART
#define OFS_UCAxIE__UART_H     OFS_UCAxIE__UART+1
#define OFS_UCAxIFG__UART     (0x001Cu)
#define OFS_UCAxIFG__UART_L    OFS_UCAxIFG__UART
#define OFS_UCAxIFG__UART_H    OFS_UCAxIFG__UART+1
#define OFS_UCAxIV            (0x001Eu)  /* USCI Ax Interrupt Vector Register */

#define OFS_UCAxCTLW0__SPI    (0x0000u)
#define OFS_UCAxCTLW0__SPI_L   OFS_UCAxCTLW0__SPI
#define OFS_UCAxCTLW0__SPI_H   OFS_UCAxCTLW0__SPI+1
#define OFS_UCAxCTL0__SPI     (0x0001u)
#define OFS_UCAxCTL1__SPI     (0x0000u)
#define OFS_UCAxBRW__SPI      (0x0006u)
#define OFS_UCAxBRW__SPI_L     OFS_UCAxBRW__SPI
#define OFS_UCAxBRW__SPI_H     OFS_UCAxBRW__SPI+1
#define OFS_UCAxBR0__SPI      (0x0006u)
#define OFS_UCAxBR1__SPI      (0x0007u)
#define OFS_UCAxSTATW__SPI    (0x000Au)
#define OFS_UCAxRXBUF__SPI    (0x000Cu)
#define OFS_UCAxRXBUF__SPI_L   OFS_UCAxRXBUF__SPI
#define OFS_UCAxRXBUF__SPI_H   OFS_UCAxRXBUF__SPI+1
#define OFS_UCAxTXBUF__SPI    (0x000Eu)
#define OFS_UCAxTXBUF__SPI_L   OFS_UCAxTXBUF__SPI
#define OFS_UCAxTXBUF__SPI_H   OFS_UCAxTXBUF__SPI+1
#define OFS_UCAxIE__SPI       (0x001Au)
#define OFS_UCAxIFG__SPI      (0x001Cu)
#define OFS_UCAxIV__SPI       (0x001Eu)

#endif
/************************************************************
* USCI Bx
************************************************************/
#ifdef  __MSP430_HAS_EUSCI_Bx__       /* Definition to show that Module is available */

#define OFS_UCBxCTLW0__SPI    (0x0000u)
#define OFS_UCBxCTLW0__SPI_L   OFS_UCBxCTLW0__SPI
#define OFS_UCBxCTLW0__SPI_H   OFS_UCBxCTLW0__SPI+1
#define OFS_UCBxCTL0__SPI     (0x0001u)
#define OFS_UCBxCTL1__SPI     (0x0000u)
#define OFS_UCBxBRW__SPI      (0x0006u)
#define OFS_UCBxBRW__SPI_L     OFS_UCBxBRW__SPI
#define OFS_UCBxBRW__SPI_H     OFS_UCBxBRW__SPI+1
#define OFS_UCBxBR0__SPI      (0x0006u)
#define OFS_UCBxBR1__SPI      (0x0007u)
#define OFS_UCBxSTATW__SPI    (0x0008u)
#define OFS_UCBxSTATW__SPI_L   OFS_UCBxSTATW__SPI
#define OFS_UCBxSTATW__SPI_H   OFS_UCBxSTATW__SPI+1
#define OFS_UCBxRXBUF__SPI    (0x000Cu)
#define OFS_UCBxRXBUF__SPI_L   OFS_UCBxRXBUF__SPI
#define OFS_UCBxRXBUF__SPI_H   OFS_UCBxRXBUF__SPI+1
#define OFS_UCBxTXBUF__SPI    (0x000Eu)
#define OFS_UCBxTXBUF__SPI_L   OFS_UCBxTXBUF__SPI
#define OFS_UCBxTXBUF__SPI_H   OFS_UCBxTXBUF__SPI+1
#define OFS_UCBxIE__SPI       (0x002Au)
#define OFS_UCBxIE__SPI_L      OFS_UCBxIE__SPI
#define OFS_UCBxIE__SPI_H      OFS_UCBxIE__SPI+1
#define OFS_UCBxIFG__SPI      (0x002Cu)
#define OFS_UCBxIFG__SPI_L     OFS_UCBxIFG__SPI
#define OFS_UCBxIFG__SPI_H     OFS_UCBxIFG__SPI+1
#define OFS_UCBxIV__SPI       (0x002Eu)

#define OFS_UCBxCTLW0         (0x0000u)  /* USCI Bx Control Word Register 0 */
#define OFS_UCBxCTLW0_L        OFS_UCBxCTLW0
#define OFS_UCBxCTLW0_H        OFS_UCBxCTLW0+1
#define OFS_UCBxCTL0          (0x0001u)
#define OFS_UCBxCTL1          (0x0000u)
#define UCBxCTL1            UCBxCTLW0_L  /* USCI Bx Control Register 1 */
#define UCBxCTL0            UCBxCTLW0_H  /* USCI Bx Control Register 0 */
#define OFS_UCBxCTLW1         (0x0002u)  /* USCI Bx Control Word Register 1 */
#define OFS_UCBxCTLW1_L        OFS_UCBxCTLW1
#define OFS_UCBxCTLW1_H        OFS_UCBxCTLW1+1
#define OFS_UCBxBRW           (0x0006u)  /* USCI Bx Baud Word Rate 0 */
#define OFS_UCBxBRW_L          OFS_UCBxBRW
#define OFS_UCBxBRW_H          OFS_UCBxBRW+1
#define OFS_UCBxBR0           (0x0006u)
#define OFS_UCBxBR1           (0x0007u)
#define UCBxBR0             UCBxBRW_L /* USCI Bx Baud Rate 0 */
#define UCBxBR1             UCBxBRW_H /* USCI Bx Baud Rate 1 */
#define OFS_UCBxSTATW         (0x0008u)  /* USCI Bx Status Word Register */
#define OFS_UCBxSTATW_L        OFS_UCBxSTATW
#define OFS_UCBxSTATW_H        OFS_UCBxSTATW+1
#define OFS_UCBxSTATW__I2C    (0x0008u)
#define OFS_UCBxSTAT__I2C     (0x0008u)
#define OFS_UCBxBCNT__I2C     (0x0009u)
#define UCBxSTAT            UCBxSTATW_L /* USCI Bx Status Register */
#define UCBxBCNT            UCBxSTATW_H /* USCI Bx Byte Counter Register */
#define OFS_UCBxTBCNT         (0x000Au)  /* USCI Bx Byte Counter Threshold Register */
#define OFS_UCBxTBCNT_L        OFS_UCBxTBCNT
#define OFS_UCBxTBCNT_H        OFS_UCBxTBCNT+1
#define OFS_UCBxRXBUF         (0x000Cu)  /* USCI Bx Receive Buffer */
#define OFS_UCBxRXBUF_L        OFS_UCBxRXBUF
#define OFS_UCBxRXBUF_H        OFS_UCBxRXBUF+1
#define OFS_UCBxTXBUF         (0x000Eu)  /* USCI Bx Transmit Buffer */
#define OFS_UCBxTXBUF_L        OFS_UCBxTXBUF
#define OFS_UCBxTXBUF_H        OFS_UCBxTXBUF+1
#define OFS_UCBxI2COA0        (0x0014u)  /* USCI Bx I2C Own Address 0 */
#define OFS_UCBxI2COA0_L       OFS_UCBxI2COA0
#define OFS_UCBxI2COA0_H       OFS_UCBxI2COA0+1
#define OFS_UCBxI2COA1        (0x0016u)  /* USCI Bx I2C Own Address 1 */
#define OFS_UCBxI2COA1_L       OFS_UCBxI2COA1
#define OFS_UCBxI2COA1_H       OFS_UCBxI2COA1+1
#define OFS_UCBxI2COA2        (0x0018u)  /* USCI Bx I2C Own Address 2 */
#define OFS_UCBxI2COA2_L       OFS_UCBxI2COA2
#define OFS_UCBxI2COA2_H       OFS_UCBxI2COA2+1
#define OFS_UCBxI2COA3        (0x001Au)  /* USCI Bx I2C Own Address 3 */
#define OFS_UCBxI2COA3_L       OFS_UCBxI2COA3
#define OFS_UCBxI2COA3_H       OFS_UCBxI2COA3+1
#define OFS_UCBxADDRX         (0x001Cu)  /* USCI Bx Received Address Register */
#define OFS_UCBxADDRX_L        OFS_UCBxADDRX
#define OFS_UCBxADDRX_H        OFS_UCBxADDRX+1
#define OFS_UCBxADDMASK       (0x001Eu)  /* USCI Bx Address Mask Register */
#define OFS_UCBxADDMASK_L      OFS_UCBxADDMASK
#define OFS_UCBxADDMASK_H      OFS_UCBxADDMASK+1
#define OFS_UCBxI2CSA         (0x0020u)  /* USCI Bx I2C Slave Address */
#define OFS_UCBxI2CSA_L        OFS_UCBxI2CSA
#define OFS_UCBxI2CSA_H        OFS_UCBxI2CSA+1
#define OFS_UCBxIE            (0x002Au)  /* USCI Bx Interrupt Enable Register */
#define OFS_UCBxIE_L           OFS_UCBxIE
#define OFS_UCBxIE_H           OFS_UCBxIE+1
#define OFS_UCBxIFG           (0x002Cu)  /* USCI Bx Interrupt Flags Register */
#define OFS_UCBxIFG_L          OFS_UCBxIFG
#define OFS_UCBxIFG_H          OFS_UCBxIFG+1
#define OFS_UCBxIE__I2C       (0x002Au)
#define OFS_UCBxIE__I2C_L      OFS_UCBxIE__I2C
#define OFS_UCBxIE__I2C_H      OFS_UCBxIE__I2C+1
#define OFS_UCBxIFG__I2C      (0x002Cu)
#define OFS_UCBxIFG__I2C_L     OFS_UCBxIFG__I2C
#define OFS_UCBxIFG__I2C_H     OFS_UCBxIFG__I2C+1
#define OFS_UCBxIV            (0x002Eu)  /* USCI Bx Interrupt Vector Register */

#endif
#if (defined(__MSP430_HAS_EUSCI_Ax__) || defined(__MSP430_HAS_EUSCI_Bx__))

// UCAxCTLW0 UART-Mode Control Bits
#define UCPEN               (0x8000u)    /* Async. Mode: Parity enable */
#define UCPAR               (0x4000u)    /* Async. Mode: Parity     0:odd / 1:even */
#define UCMSB               (0x2000u)    /* Async. Mode: MSB first  0:LSB / 1:MSB */
#define UC7BIT              (0x1000u)    /* Async. Mode: Data Bits  0:8-bits / 1:7-bits */
#define UCSPB               (0x0800u)    /* Async. Mode: Stop Bits  0:one / 1: two */
#define UCMODE1             (0x0400u)    /* Async. Mode: USCI Mode 1 */
#define UCMODE0             (0x0200u)    /* Async. Mode: USCI Mode 0 */
#define UCSYNC              (0x0100u)    /* Sync-Mode  0:UART-Mode / 1:SPI-Mode */
#define UCSSEL1             (0x0080u)    /* USCI 0 Clock Source Select 1 */
#define UCSSEL0             (0x0040u)    /* USCI 0 Clock Source Select 0 */
#define UCRXEIE             (0x0020u)    /* RX Error interrupt enable */
#define UCBRKIE             (0x0010u)    /* Break interrupt enable */
#define UCDORM              (0x0008u)    /* Dormant (Sleep) Mode */
#define UCTXADDR            (0x0004u)    /* Send next Data as Address */
#define UCTXBRK             (0x0002u)    /* Send next Data as Break */
#define UCSWRST             (0x0001u)    /* USCI Software Reset */

// UCAxCTLW0 UART-Mode Control Bits
#define UCSSEL1_L           (0x0080u)    /* USCI 0 Clock Source Select 1 */
#define UCSSEL0_L           (0x0040u)    /* USCI 0 Clock Source Select 0 */
#define UCRXEIE_L           (0x0020u)    /* RX Error interrupt enable */
#define UCBRKIE_L           (0x0010u)    /* Break interrupt enable */
#define UCDORM_L            (0x0008u)    /* Dormant (Sleep) Mode */
#define UCTXADDR_L          (0x0004u)    /* Send next Data as Address */
#define UCTXBRK_L           (0x0002u)    /* Send next Data as Break */
#define UCSWRST_L           (0x0001u)    /* USCI Software Reset */

// UCAxCTLW0 UART-Mode Control Bits
#define UCPEN_H             (0x0080u)    /* Async. Mode: Parity enable */
#define UCPAR_H             (0x0040u)    /* Async. Mode: Parity     0:odd / 1:even */
#define UCMSB_H             (0x0020u)    /* Async. Mode: MSB first  0:LSB / 1:MSB */
#define UC7BIT_H            (0x0010u)    /* Async. Mode: Data Bits  0:8-bits / 1:7-bits */
#define UCSPB_H             (0x0008u)    /* Async. Mode: Stop Bits  0:one / 1: two */
#define UCMODE1_H           (0x0004u)    /* Async. Mode: USCI Mode 1 */
#define UCMODE0_H           (0x0002u)    /* Async. Mode: USCI Mode 0 */
#define UCSYNC_H            (0x0001u)    /* Sync-Mode  0:UART-Mode / 1:SPI-Mode */

// UCxxCTLW0 SPI-Mode Control Bits
#define UCCKPH              (0x8000u)    /* Sync. Mode: Clock Phase */
#define UCCKPL              (0x4000u)    /* Sync. Mode: Clock Polarity */
#define UCMST               (0x0800u)    /* Sync. Mode: Master Select */
//#define res               (0x0020u)    /* reserved */
//#define res               (0x0010u)    /* reserved */
//#define res               (0x0008u)    /* reserved */
//#define res               (0x0004u)    /* reserved */
#define UCSTEM             (0x0002u)    /* USCI STE Mode */

// UCBxCTLW0 I2C-Mode Control Bits
#define UCA10               (0x8000u)    /* 10-bit Address Mode */
#define UCSLA10             (0x4000u)    /* 10-bit Slave Address Mode */
#define UCMM                (0x2000u)    /* Multi-Master Environment */
//#define res               (0x1000u)    /* reserved */
//#define res               (0x0100u)    /* reserved */
#define UCTXACK             (0x0020u)    /* Transmit ACK */
#define UCTR                (0x0010u)    /* Transmit/Receive Select/Flag */
#define UCTXNACK            (0x0008u)    /* Transmit NACK */
#define UCTXSTP             (0x0004u)    /* Transmit STOP */
#define UCTXSTT             (0x0002u)    /* Transmit START */

// UCBxCTLW0 I2C-Mode Control Bits
//#define res               (0x1000u)    /* reserved */
//#define res               (0x0100u)    /* reserved */
#define UCTXACK_L           (0x0020u)    /* Transmit ACK */
#define UCTR_L              (0x0010u)    /* Transmit/Receive Select/Flag */
#define UCTXNACK_L          (0x0008u)    /* Transmit NACK */
#define UCTXSTP_L           (0x0004u)    /* Transmit STOP */
#define UCTXSTT_L           (0x0002u)    /* Transmit START */

// UCBxCTLW0 I2C-Mode Control Bits
#define UCA10_H             (0x0080u)    /* 10-bit Address Mode */
#define UCSLA10_H           (0x0040u)    /* 10-bit Slave Address Mode */
#define UCMM_H              (0x0020u)    /* Multi-Master Environment */
//#define res               (0x1000u)    /* reserved */
//#define res               (0x0100u)    /* reserved */

#define UCMODE_0            (0x0000u)    /* Sync. Mode: USCI Mode: 0 */
#define UCMODE_1            (0x0200u)    /* Sync. Mode: USCI Mode: 1 */
#define UCMODE_2            (0x0400u)    /* Sync. Mode: USCI Mode: 2 */
#define UCMODE_3            (0x0600u)    /* Sync. Mode: USCI Mode: 3 */

#define UCSSEL_0            (0x0000u)    /* USCI 0 Clock Source: 0 */
#define UCSSEL_1            (0x0040u)    /* USCI 0 Clock Source: 1 */
#define UCSSEL_2            (0x0080u)    /* USCI 0 Clock Source: 2 */
#define UCSSEL_3            (0x00C0u)    /* USCI 0 Clock Source: 3 */
#define UCSSEL__UCLK        (0x0000u)    /* USCI 0 Clock Source: UCLK */
#define UCSSEL__MODCLK      (0x0040u)    /* USCI 0 Clock Source: MODCLK */
#define UCSSEL__SMCLK       (0x0080u)    /* USCI 0 Clock Source: SMCLK */

// UCAxCTLW1 UART-Mode Control Bits
#define UCGLIT1             (0x0002u)    /* USCI Deglitch Time Bit 1 */
#define UCGLIT0             (0x0001u)    /* USCI Deglitch Time Bit 0 */

// UCAxCTLW1 UART-Mode Control Bits
#define UCGLIT1_L           (0x0002u)    /* USCI Deglitch Time Bit 1 */
#define UCGLIT0_L           (0x0001u)    /* USCI Deglitch Time Bit 0 */

// UCBxCTLW1 I2C-Mode Control Bits
#define UCETXINT            (0x0100u)    /* USCI Early UCTXIFG0 */
#define UCCLTO1             (0x0080u)    /* USCI Clock low timeout Bit: 1 */
#define UCCLTO0             (0x0040u)    /* USCI Clock low timeout Bit: 0 */
#define UCSTPNACK           (0x0020u)    /* USCI Acknowledge Stop last byte */
#define UCSWACK             (0x0010u)    /* USCI Software controlled ACK */
#define UCASTP1             (0x0008u)    /* USCI Automatic Stop condition generation Bit: 1 */
#define UCASTP0             (0x0004u)    /* USCI Automatic Stop condition generation Bit: 0 */
#define UCGLIT1             (0x0002u)    /* USCI Deglitch time Bit: 1 */
#define UCGLIT0             (0x0001u)    /* USCI Deglitch time Bit: 0 */

// UCBxCTLW1 I2C-Mode Control Bits
#define UCCLTO1_L           (0x0080u)    /* USCI Clock low timeout Bit: 1 */
#define UCCLTO0_L           (0x0040u)    /* USCI Clock low timeout Bit: 0 */
#define UCSTPNACK_L         (0x0020u)    /* USCI Acknowledge Stop last byte */
#define UCSWACK_L           (0x0010u)    /* USCI Software controlled ACK */
#define UCASTP1_L           (0x0008u)    /* USCI Automatic Stop condition generation Bit: 1 */
#define UCASTP0_L           (0x0004u)    /* USCI Automatic Stop condition generation Bit: 0 */
#define UCGLIT1_L           (0x0002u)    /* USCI Deglitch time Bit: 1 */
#define UCGLIT0_L           (0x0001u)    /* USCI Deglitch time Bit: 0 */

// UCBxCTLW1 I2C-Mode Control Bits
#define UCETXINT_H          (0x0001u)    /* USCI Early UCTXIFG0 */

#define UCGLIT_0            (0x0000u)    /* USCI Deglitch time: 0 */
#define UCGLIT_1            (0x0001u)    /* USCI Deglitch time: 1 */
#define UCGLIT_2            (0x0002u)    /* USCI Deglitch time: 2 */
#define UCGLIT_3            (0x0003u)    /* USCI Deglitch time: 3 */

#define UCASTP_0            (0x0000u)    /* USCI Automatic Stop condition generation: 0 */
#define UCASTP_1            (0x0004u)    /* USCI Automatic Stop condition generation: 1 */
#define UCASTP_2            (0x0008u)    /* USCI Automatic Stop condition generation: 2 */
#define UCASTP_3            (0x000Cu)    /* USCI Automatic Stop condition generation: 3 */

#define UCCLTO_0            (0x0000u)    /* USCI Clock low timeout: 0 */
#define UCCLTO_1            (0x0040u)    /* USCI Clock low timeout: 1 */
#define UCCLTO_2            (0x0080u)    /* USCI Clock low timeout: 2 */
#define UCCLTO_3            (0x00C0u)    /* USCI Clock low timeout: 3 */

/* UCAxMCTLW Control Bits */
#define UCBRS7              (0x8000u)    /* USCI Second Stage Modulation Select 7 */
#define UCBRS6              (0x4000u)    /* USCI Second Stage Modulation Select 6 */
#define UCBRS5              (0x2000u)    /* USCI Second Stage Modulation Select 5 */
#define UCBRS4              (0x1000u)    /* USCI Second Stage Modulation Select 4 */
#define UCBRS3              (0x0800u)    /* USCI Second Stage Modulation Select 3 */
#define UCBRS2              (0x0400u)    /* USCI Second Stage Modulation Select 2 */
#define UCBRS1              (0x0200u)    /* USCI Second Stage Modulation Select 1 */
#define UCBRS0              (0x0100u)    /* USCI Second Stage Modulation Select 0 */
#define UCBRF3              (0x0080u)    /* USCI First Stage Modulation Select 3 */
#define UCBRF2              (0x0040u)    /* USCI First Stage Modulation Select 2 */
#define UCBRF1              (0x0020u)    /* USCI First Stage Modulation Select 1 */
#define UCBRF0              (0x0010u)    /* USCI First Stage Modulation Select 0 */
#define UCOS16              (0x0001u)    /* USCI 16-times Oversampling enable */

/* UCAxMCTLW Control Bits */
#define UCBRF3_L            (0x0080u)    /* USCI First Stage Modulation Select 3 */
#define UCBRF2_L            (0x0040u)    /* USCI First Stage Modulation Select 2 */
#define UCBRF1_L            (0x0020u)    /* USCI First Stage Modulation Select 1 */
#define UCBRF0_L            (0x0010u)    /* USCI First Stage Modulation Select 0 */
#define UCOS16_L            (0x0001u)    /* USCI 16-times Oversampling enable */

/* UCAxMCTLW Control Bits */
#define UCBRS7_H            (0x0080u)    /* USCI Second Stage Modulation Select 7 */
#define UCBRS6_H            (0x0040u)    /* USCI Second Stage Modulation Select 6 */
#define UCBRS5_H            (0x0020u)    /* USCI Second Stage Modulation Select 5 */
#define UCBRS4_H            (0x0010u)    /* USCI Second Stage Modulation Select 4 */
#define UCBRS3_H            (0x0008u)    /* USCI Second Stage Modulation Select 3 */
#define UCBRS2_H            (0x0004u)    /* USCI Second Stage Modulation Select 2 */
#define UCBRS1_H            (0x0002u)    /* USCI Second Stage Modulation Select 1 */
#define UCBRS0_H            (0x0001u)    /* USCI Second Stage Modulation Select 0 */

#define UCBRF_0             (0x00)    /* USCI First Stage Modulation: 0 */
#define UCBRF_1             (0x10)    /* USCI First Stage Modulation: 1 */
#define UCBRF_2             (0x20)    /* USCI First Stage Modulation: 2 */
#define UCBRF_3             (0x30)    /* USCI First Stage Modulation: 3 */
#define UCBRF_4             (0x40)    /* USCI First Stage Modulation: 4 */
#define UCBRF_5             (0x50)    /* USCI First Stage Modulation: 5 */
#define UCBRF_6             (0x60)    /* USCI First Stage Modulation: 6 */
#define UCBRF_7             (0x70)    /* USCI First Stage Modulation: 7 */
#define UCBRF_8             (0x80)    /* USCI First Stage Modulation: 8 */
#define UCBRF_9             (0x90)    /* USCI First Stage Modulation: 9 */
#define UCBRF_10            (0xA0)    /* USCI First Stage Modulation: A */
#define UCBRF_11            (0xB0)    /* USCI First Stage Modulation: B */
#define UCBRF_12            (0xC0)    /* USCI First Stage Modulation: C */
#define UCBRF_13            (0xD0)    /* USCI First Stage Modulation: D */
#define UCBRF_14            (0xE0)    /* USCI First Stage Modulation: E */
#define UCBRF_15            (0xF0)    /* USCI First Stage Modulation: F */

/* UCAxSTATW Control Bits */
#define UCLISTEN            (0x0080u)  /* USCI Listen mode */
#define UCFE                (0x0040u)  /* USCI Frame Error Flag */
#define UCOE                (0x0020u)  /* USCI Overrun Error Flag */
#define UCPE                (0x0010u)  /* USCI Parity Error Flag */
#define UCBRK               (0x0008u)  /* USCI Break received */
#define UCRXERR             (0x0004u)  /* USCI RX Error Flag */
#define UCADDR              (0x0002u)  /* USCI Address received Flag */
#define UCBUSY              (0x0001u)  /* USCI Busy Flag */
#define UCIDLE              (0x0002u)  /* USCI Idle line detected Flag */

/* UCBxSTATW I2C Control Bits */
#define UCBCNT7             (0x8000u)  /* USCI Byte Counter Bit 7 */
#define UCBCNT6             (0x4000u)  /* USCI Byte Counter Bit 6 */
#define UCBCNT5             (0x2000u)  /* USCI Byte Counter Bit 5 */
#define UCBCNT4             (0x1000u)  /* USCI Byte Counter Bit 4 */
#define UCBCNT3             (0x0800u)  /* USCI Byte Counter Bit 3 */
#define UCBCNT2             (0x0400u)  /* USCI Byte Counter Bit 2 */
#define UCBCNT1             (0x0200u)  /* USCI Byte Counter Bit 1 */
#define UCBCNT0             (0x0100u)  /* USCI Byte Counter Bit 0 */
#define UCSCLLOW            (0x0040u)  /* SCL low */
#define UCGC                (0x0020u)  /* General Call address received Flag */
#define UCBBUSY             (0x0010u)  /* Bus Busy Flag */

/* UCBxTBCNT I2C Control Bits */
#define UCTBCNT7            (0x0080u)  /* USCI Byte Counter Bit 7 */
#define UCTBCNT6            (0x0040u)  /* USCI Byte Counter Bit 6 */
#define UCTBCNT5            (0x0020u)  /* USCI Byte Counter Bit 5 */
#define UCTBCNT4            (0x0010u)  /* USCI Byte Counter Bit 4 */
#define UCTBCNT3            (0x0008u)  /* USCI Byte Counter Bit 3 */
#define UCTBCNT2            (0x0004u)  /* USCI Byte Counter Bit 2 */
#define UCTBCNT1            (0x0002u)  /* USCI Byte Counter Bit 1 */
#define UCTBCNT0            (0x0001u)  /* USCI Byte Counter Bit 0 */

/* UCAxIRCTL Control Bits */
#define UCIRRXFL5           (0x8000u)  /* IRDA Receive Filter Length 5 */
#define UCIRRXFL4           (0x4000u)  /* IRDA Receive Filter Length 4 */
#define UCIRRXFL3           (0x2000u)  /* IRDA Receive Filter Length 3 */
#define UCIRRXFL2           (0x1000u)  /* IRDA Receive Filter Length 2 */
#define UCIRRXFL1           (0x0800u)  /* IRDA Receive Filter Length 1 */
#define UCIRRXFL0           (0x0400u)  /* IRDA Receive Filter Length 0 */
#define UCIRRXPL            (0x0200u)  /* IRDA Receive Input Polarity */
#define UCIRRXFE            (0x0100u)  /* IRDA Receive Filter enable */
#define UCIRTXPL5           (0x0080u)  /* IRDA Transmit Pulse Length 5 */
#define UCIRTXPL4           (0x0040u)  /* IRDA Transmit Pulse Length 4 */
#define UCIRTXPL3           (0x0020u)  /* IRDA Transmit Pulse Length 3 */
#define UCIRTXPL2           (0x0010u)  /* IRDA Transmit Pulse Length 2 */
#define UCIRTXPL1           (0x0008u)  /* IRDA Transmit Pulse Length 1 */
#define UCIRTXPL0           (0x0004u)  /* IRDA Transmit Pulse Length 0 */
#define UCIRTXCLK           (0x0002u)  /* IRDA Transmit Pulse Clock Select */
#define UCIREN              (0x0001u)  /* IRDA Encoder/Decoder enable */

/* UCAxIRCTL Control Bits */
#define UCIRTXPL5_L         (0x0080u)  /* IRDA Transmit Pulse Length 5 */
#define UCIRTXPL4_L         (0x0040u)  /* IRDA Transmit Pulse Length 4 */
#define UCIRTXPL3_L         (0x0020u)  /* IRDA Transmit Pulse Length 3 */
#define UCIRTXPL2_L         (0x0010u)  /* IRDA Transmit Pulse Length 2 */
#define UCIRTXPL1_L         (0x0008u)  /* IRDA Transmit Pulse Length 1 */
#define UCIRTXPL0_L         (0x0004u)  /* IRDA Transmit Pulse Length 0 */
#define UCIRTXCLK_L         (0x0002u)  /* IRDA Transmit Pulse Clock Select */
#define UCIREN_L            (0x0001u)  /* IRDA Encoder/Decoder enable */

/* UCAxIRCTL Control Bits */
#define UCIRRXFL5_H         (0x0080u)  /* IRDA Receive Filter Length 5 */
#define UCIRRXFL4_H         (0x0040u)  /* IRDA Receive Filter Length 4 */
#define UCIRRXFL3_H         (0x0020u)  /* IRDA Receive Filter Length 3 */
#define UCIRRXFL2_H         (0x0010u)  /* IRDA Receive Filter Length 2 */
#define UCIRRXFL1_H         (0x0008u)  /* IRDA Receive Filter Length 1 */
#define UCIRRXFL0_H         (0x0004u)  /* IRDA Receive Filter Length 0 */
#define UCIRRXPL_H          (0x0002u)  /* IRDA Receive Input Polarity */
#define UCIRRXFE_H          (0x0001u)  /* IRDA Receive Filter enable */

/* UCAxABCTL Control Bits */
//#define res               (0x80)    /* reserved */
//#define res               (0x40)    /* reserved */
#define UCDELIM1            (0x20)    /* Break Sync Delimiter 1 */
#define UCDELIM0            (0x10)    /* Break Sync Delimiter 0 */
#define UCSTOE              (0x08)    /* Sync-Field Timeout error */
#define UCBTOE              (0x04)    /* Break Timeout error */
//#define res               (0x02)    /* reserved */
#define UCABDEN             (0x01)    /* Auto Baud Rate detect enable */

/* UCBxI2COA0 Control Bits */
#define UCGCEN              (0x8000u)  /* I2C General Call enable */
#define UCOAEN              (0x0400u)  /* I2C Own Address enable */
#define UCOA9               (0x0200u)  /* I2C Own Address Bit 9 */
#define UCOA8               (0x0100u)  /* I2C Own Address Bit 8 */
#define UCOA7               (0x0080u)  /* I2C Own Address Bit 7 */
#define UCOA6               (0x0040u)  /* I2C Own Address Bit 6 */
#define UCOA5               (0x0020u)  /* I2C Own Address Bit 5 */
#define UCOA4               (0x0010u)  /* I2C Own Address Bit 4 */
#define UCOA3               (0x0008u)  /* I2C Own Address Bit 3 */
#define UCOA2               (0x0004u)  /* I2C Own Address Bit 2 */
#define UCOA1               (0x0002u)  /* I2C Own Address Bit 1 */
#define UCOA0               (0x0001u)  /* I2C Own Address Bit 0 */

/* UCBxI2COA0 Control Bits */
#define UCOA7_L             (0x0080u)  /* I2C Own Address Bit 7 */
#define UCOA6_L             (0x0040u)  /* I2C Own Address Bit 6 */
#define UCOA5_L             (0x0020u)  /* I2C Own Address Bit 5 */
#define UCOA4_L             (0x0010u)  /* I2C Own Address Bit 4 */
#define UCOA3_L             (0x0008u)  /* I2C Own Address Bit 3 */
#define UCOA2_L             (0x0004u)  /* I2C Own Address Bit 2 */
#define UCOA1_L             (0x0002u)  /* I2C Own Address Bit 1 */
#define UCOA0_L             (0x0001u)  /* I2C Own Address Bit 0 */

/* UCBxI2COA0 Control Bits */
#define UCGCEN_H            (0x0080u)  /* I2C General Call enable */
#define UCOAEN_H            (0x0004u)  /* I2C Own Address enable */
#define UCOA9_H             (0x0002u)  /* I2C Own Address Bit 9 */
#define UCOA8_H             (0x0001u)  /* I2C Own Address Bit 8 */

/* UCBxI2COAx Control Bits */
#define UCOAEN              (0x0400u)  /* I2C Own Address enable */
#define UCOA9               (0x0200u)  /* I2C Own Address Bit 9 */
#define UCOA8               (0x0100u)  /* I2C Own Address Bit 8 */
#define UCOA7               (0x0080u)  /* I2C Own Address Bit 7 */
#define UCOA6               (0x0040u)  /* I2C Own Address Bit 6 */
#define UCOA5               (0x0020u)  /* I2C Own Address Bit 5 */
#define UCOA4               (0x0010u)  /* I2C Own Address Bit 4 */
#define UCOA3               (0x0008u)  /* I2C Own Address Bit 3 */
#define UCOA2               (0x0004u)  /* I2C Own Address Bit 2 */
#define UCOA1               (0x0002u)  /* I2C Own Address Bit 1 */
#define UCOA0               (0x0001u)  /* I2C Own Address Bit 0 */

/* UCBxI2COAx Control Bits */
#define UCOA7_L             (0x0080u)  /* I2C Own Address Bit 7 */
#define UCOA6_L             (0x0040u)  /* I2C Own Address Bit 6 */
#define UCOA5_L             (0x0020u)  /* I2C Own Address Bit 5 */
#define UCOA4_L             (0x0010u)  /* I2C Own Address Bit 4 */
#define UCOA3_L             (0x0008u)  /* I2C Own Address Bit 3 */
#define UCOA2_L             (0x0004u)  /* I2C Own Address Bit 2 */
#define UCOA1_L             (0x0002u)  /* I2C Own Address Bit 1 */
#define UCOA0_L             (0x0001u)  /* I2C Own Address Bit 0 */

/* UCBxI2COAx Control Bits */
#define UCOAEN_H            (0x0004u)  /* I2C Own Address enable */
#define UCOA9_H             (0x0002u)  /* I2C Own Address Bit 9 */
#define UCOA8_H             (0x0001u)  /* I2C Own Address Bit 8 */

/* UCBxADDRX Control Bits */
#define UCADDRX9            (0x0200u)  /* I2C Receive Address Bit 9 */
#define UCADDRX8            (0x0100u)  /* I2C Receive Address Bit 8 */
#define UCADDRX7            (0x0080u)  /* I2C Receive Address Bit 7 */
#define UCADDRX6            (0x0040u)  /* I2C Receive Address Bit 6 */
#define UCADDRX5            (0x0020u)  /* I2C Receive Address Bit 5 */
#define UCADDRX4            (0x0010u)  /* I2C Receive Address Bit 4 */
#define UCADDRX3            (0x0008u)  /* I2C Receive Address Bit 3 */
#define UCADDRX2            (0x0004u)  /* I2C Receive Address Bit 2 */
#define UCADDRX1            (0x0002u)  /* I2C Receive Address Bit 1 */
#define UCADDRX0            (0x0001u)  /* I2C Receive Address Bit 0 */

/* UCBxADDRX Control Bits */
#define UCADDRX7_L          (0x0080u)  /* I2C Receive Address Bit 7 */
#define UCADDRX6_L          (0x0040u)  /* I2C Receive Address Bit 6 */
#define UCADDRX5_L          (0x0020u)  /* I2C Receive Address Bit 5 */
#define UCADDRX4_L          (0x0010u)  /* I2C Receive Address Bit 4 */
#define UCADDRX3_L          (0x0008u)  /* I2C Receive Address Bit 3 */
#define UCADDRX2_L          (0x0004u)  /* I2C Receive Address Bit 2 */
#define UCADDRX1_L          (0x0002u)  /* I2C Receive Address Bit 1 */
#define UCADDRX0_L          (0x0001u)  /* I2C Receive Address Bit 0 */

/* UCBxADDRX Control Bits */
#define UCADDRX9_H          (0x0002u)  /* I2C Receive Address Bit 9 */
#define UCADDRX8_H          (0x0001u)  /* I2C Receive Address Bit 8 */

/* UCBxADDMASK Control Bits */
#define UCADDMASK9            (0x0200u)  /* I2C Address Mask Bit 9 */
#define UCADDMASK8            (0x0100u)  /* I2C Address Mask Bit 8 */
#define UCADDMASK7            (0x0080u)  /* I2C Address Mask Bit 7 */
#define UCADDMASK6            (0x0040u)  /* I2C Address Mask Bit 6 */
#define UCADDMASK5            (0x0020u)  /* I2C Address Mask Bit 5 */
#define UCADDMASK4            (0x0010u)  /* I2C Address Mask Bit 4 */
#define UCADDMASK3            (0x0008u)  /* I2C Address Mask Bit 3 */
#define UCADDMASK2            (0x0004u)  /* I2C Address Mask Bit 2 */
#define UCADDMASK1            (0x0002u)  /* I2C Address Mask Bit 1 */
#define UCADDMASK0            (0x0001u)  /* I2C Address Mask Bit 0 */

/* UCBxADDMASK Control Bits */
#define UCADDMASK7_L        (0x0080u)  /* I2C Address Mask Bit 7 */
#define UCADDMASK6_L        (0x0040u)  /* I2C Address Mask Bit 6 */
#define UCADDMASK5_L        (0x0020u)  /* I2C Address Mask Bit 5 */
#define UCADDMASK4_L        (0x0010u)  /* I2C Address Mask Bit 4 */
#define UCADDMASK3_L        (0x0008u)  /* I2C Address Mask Bit 3 */
#define UCADDMASK2_L        (0x0004u)  /* I2C Address Mask Bit 2 */
#define UCADDMASK1_L        (0x0002u)  /* I2C Address Mask Bit 1 */
#define UCADDMASK0_L        (0x0001u)  /* I2C Address Mask Bit 0 */

/* UCBxADDMASK Control Bits */
#define UCADDMASK9_H        (0x0002u)  /* I2C Address Mask Bit 9 */
#define UCADDMASK8_H        (0x0001u)  /* I2C Address Mask Bit 8 */

/* UCBxI2CSA Control Bits */
#define UCSA9               (0x0200u)  /* I2C Slave Address Bit 9 */
#define UCSA8               (0x0100u)  /* I2C Slave Address Bit 8 */
#define UCSA7               (0x0080u)  /* I2C Slave Address Bit 7 */
#define UCSA6               (0x0040u)  /* I2C Slave Address Bit 6 */
#define UCSA5               (0x0020u)  /* I2C Slave Address Bit 5 */
#define UCSA4               (0x0010u)  /* I2C Slave Address Bit 4 */
#define UCSA3               (0x0008u)  /* I2C Slave Address Bit 3 */
#define UCSA2               (0x0004u)  /* I2C Slave Address Bit 2 */
#define UCSA1               (0x0002u)  /* I2C Slave Address Bit 1 */
#define UCSA0               (0x0001u)  /* I2C Slave Address Bit 0 */

/* UCBxI2CSA Control Bits */
#define UCSA7_L             (0x0080u)  /* I2C Slave Address Bit 7 */
#define UCSA6_L             (0x0040u)  /* I2C Slave Address Bit 6 */
#define UCSA5_L             (0x0020u)  /* I2C Slave Address Bit 5 */
#define UCSA4_L             (0x0010u)  /* I2C Slave Address Bit 4 */
#define UCSA3_L             (0x0008u)  /* I2C Slave Address Bit 3 */
#define UCSA2_L             (0x0004u)  /* I2C Slave Address Bit 2 */
#define UCSA1_L             (0x0002u)  /* I2C Slave Address Bit 1 */
#define UCSA0_L             (0x0001u)  /* I2C Slave Address Bit 0 */

/* UCBxI2CSA Control Bits */
#define UCSA9_H             (0x0002u)  /* I2C Slave Address Bit 9 */
#define UCSA8_H             (0x0001u)  /* I2C Slave Address Bit 8 */

/* UCAxIE UART Control Bits */
#define UCTXCPTIE           (0x0008u)  /* UART Transmit Complete Interrupt Enable */
#define UCSTTIE             (0x0004u)  /* UART Start Bit Interrupt Enalble */
#define UCTXIE              (0x0002u)  /* UART Transmit Interrupt Enable */
#define UCRXIE              (0x0001u)  /* UART Receive Interrupt Enable */

/* UCAxIE/UCBxIE SPI Control Bits */

/* UCBxIE I2C Control Bits */
#define UCBIT9IE            (0x4000u)  /* I2C Bit 9 Position Interrupt Enable 3 */
#define UCTXIE3             (0x2000u)  /* I2C Transmit Interrupt Enable 3 */
#define UCRXIE3             (0x1000u)  /* I2C Receive Interrupt Enable 3 */
#define UCTXIE2             (0x0800u)  /* I2C Transmit Interrupt Enable 2 */
#define UCRXIE2             (0x0400u)  /* I2C Receive Interrupt Enable 2 */
#define UCTXIE1             (0x0200u)  /* I2C Transmit Interrupt Enable 1 */
#define UCRXIE1             (0x0100u)  /* I2C Receive Interrupt Enable 1 */
#define UCCLTOIE            (0x0080u)  /* I2C Clock Low Timeout interrupt enable */
#define UCBCNTIE            (0x0040u)  /* I2C Automatic stop assertion interrupt enable */
#define UCNACKIE            (0x0020u)  /* I2C NACK Condition interrupt enable */
#define UCALIE              (0x0010u)  /* I2C Arbitration Lost interrupt enable */
#define UCSTPIE             (0x0008u)  /* I2C STOP Condition interrupt enable */
#define UCSTTIE             (0x0004u)  /* I2C START Condition interrupt enable */
#define UCTXIE0             (0x0002u)  /* I2C Transmit Interrupt Enable 0 */
#define UCRXIE0             (0x0001u)  /* I2C Receive Interrupt Enable 0 */

/* UCAxIFG UART Control Bits */
#define UCTXCPTIFG          (0x0008u)  /* UART Transmit Complete Interrupt Flag */
#define UCSTTIFG            (0x0004u)  /* UART Start Bit Interrupt Flag */
#define UCTXIFG             (0x0002u)  /* UART Transmit Interrupt Flag */
#define UCRXIFG             (0x0001u)  /* UART Receive Interrupt Flag */

/* UCAxIFG/UCBxIFG SPI Control Bits */
#define UCTXIFG             (0x0002u)  /* SPI Transmit Interrupt Flag */
#define UCRXIFG             (0x0001u)  /* SPI Receive Interrupt Flag */

/* UCBxIFG Control Bits */
#define UCBIT9IFG           (0x4000u)  /* I2C Bit 9 Possition Interrupt Flag 3 */
#define UCTXIFG3            (0x2000u)  /* I2C Transmit Interrupt Flag 3 */
#define UCRXIFG3            (0x1000u)  /* I2C Receive Interrupt Flag 3 */
#define UCTXIFG2            (0x0800u)  /* I2C Transmit Interrupt Flag 2 */
#define UCRXIFG2            (0x0400u)  /* I2C Receive Interrupt Flag 2 */
#define UCTXIFG1            (0x0200u)  /* I2C Transmit Interrupt Flag 1 */
#define UCRXIFG1            (0x0100u)  /* I2C Receive Interrupt Flag 1 */
#define UCCLTOIFG           (0x0080u)  /* I2C Clock low Timeout interrupt Flag */
#define UCBCNTIFG           (0x0040u)  /* I2C Byte counter interrupt flag */
#define UCNACKIFG           (0x0020u)  /* I2C NACK Condition interrupt Flag */
#define UCALIFG             (0x0010u)  /* I2C Arbitration Lost interrupt Flag */
#define UCSTPIFG            (0x0008u)  /* I2C STOP Condition interrupt Flag */
#define UCSTTIFG            (0x0004u)  /* I2C START Condition interrupt Flag */
#define UCTXIFG0            (0x0002u)  /* I2C Transmit Interrupt Flag 0 */
#define UCRXIFG0            (0x0001u)  /* I2C Receive Interrupt Flag 0 */

/* USCI UART Definitions */
#define USCI_NONE            (0x0000u)   /* No Interrupt pending */
#define USCI_UART_UCRXIFG    (0x0002u)   /* USCI UCRXIFG */
#define USCI_UART_UCTXIFG    (0x0004u)   /* USCI UCTXIFG */
#define USCI_UART_UCSTTIFG   (0x0006u)   /* USCI UCSTTIFG */
#define USCI_UART_UCTXCPTIFG (0x0008u)   /* USCI UCTXCPTIFG */

/* USCI SPI Definitions */
#define USCI_SPI_UCRXIFG    (0x0002u)    /* USCI UCRXIFG */
#define USCI_SPI_UCTXIFG    (0x0004u)    /* USCI UCTXIFG */

/* USCI I2C Definitions */
#define USCI_I2C_UCALIFG    (0x0002u)    /* USCI I2C Mode: UCALIFG */
#define USCI_I2C_UCNACKIFG  (0x0004u)    /* USCI I2C Mode: UCNACKIFG */
#define USCI_I2C_UCSTTIFG   (0x0006u)    /* USCI I2C Mode: UCSTTIFG*/
#define USCI_I2C_UCSTPIFG   (0x0008u)    /* USCI I2C Mode: UCSTPIFG*/
#define USCI_I2C_UCRXIFG3   (0x000Au)    /* USCI I2C Mode: UCRXIFG3 */
#define USCI_I2C_UCTXIFG3   (0x000Cu)    /* USCI I2C Mode: UCTXIFG3 */
#define USCI_I2C_UCRXIFG2   (0x000Eu)    /* USCI I2C Mode: UCRXIFG2 */
#define USCI_I2C_UCTXIFG2   (0x0010u)    /* USCI I2C Mode: UCTXIFG2 */
#define USCI_I2C_UCRXIFG1   (0x0012u)    /* USCI I2C Mode: UCRXIFG1 */
#define USCI_I2C_UCTXIFG1   (0x0014u)    /* USCI I2C Mode: UCTXIFG1 */
#define USCI_I2C_UCRXIFG0   (0x0016u)    /* USCI I2C Mode: UCRXIFG0 */
#define USCI_I2C_UCTXIFG0   (0x0018u)    /* USCI I2C Mode: UCTXIFG0 */
#define USCI_I2C_UCBCNTIFG  (0x001Au)    /* USCI I2C Mode: UCBCNTIFG */
#define USCI_I2C_UCCLTOIFG  (0x001Cu)    /* USCI I2C Mode: UCCLTOIFG */
#define USCI_I2C_UCBIT9IFG  (0x001Eu)    /* USCI I2C Mode: UCBIT9IFG */

#endif
/************************************************************
* WATCHDOG TIMER A
************************************************************/
#ifdef  __MSP430_HAS_WDT_A__          /* Definition to show that Module is available */

#define OFS_WDTCTL            (0x000Cu)  /* Watchdog Timer Control */
#define OFS_WDTCTL_L           OFS_WDTCTL
#define OFS_WDTCTL_H           OFS_WDTCTL+1
/* The bit names have been prefixed with "WDT" */
/* WDTCTL Control Bits */
#define WDTIS0              (0x0001u)  /* WDT - Timer Interval Select 0 */
#define WDTIS1              (0x0002u)  /* WDT - Timer Interval Select 1 */
#define WDTIS2              (0x0004u)  /* WDT - Timer Interval Select 2 */
#define WDTCNTCL            (0x0008u)  /* WDT - Timer Clear */
#define WDTTMSEL            (0x0010u)  /* WDT - Timer Mode Select */
#define WDTSSEL0            (0x0020u)  /* WDT - Timer Clock Source Select 0 */
#define WDTSSEL1            (0x0040u)  /* WDT - Timer Clock Source Select 1 */
#define WDTHOLD             (0x0080u)  /* WDT - Timer hold */

/* WDTCTL Control Bits */
#define WDTIS0_L            (0x0001u)  /* WDT - Timer Interval Select 0 */
#define WDTIS1_L            (0x0002u)  /* WDT - Timer Interval Select 1 */
#define WDTIS2_L            (0x0004u)  /* WDT - Timer Interval Select 2 */
#define WDTCNTCL_L          (0x0008u)  /* WDT - Timer Clear */
#define WDTTMSEL_L          (0x0010u)  /* WDT - Timer Mode Select */
#define WDTSSEL0_L          (0x0020u)  /* WDT - Timer Clock Source Select 0 */
#define WDTSSEL1_L          (0x0040u)  /* WDT - Timer Clock Source Select 1 */
#define WDTHOLD_L           (0x0080u)  /* WDT - Timer hold */

#define WDTPW               (0x5A00u)

#define WDTIS_0           (0*0x0001u)  /* WDT - Timer Interval Select: /2G */
#define WDTIS_1           (1*0x0001u)  /* WDT - Timer Interval Select: /128M */
#define WDTIS_2           (2*0x0001u)  /* WDT - Timer Interval Select: /8192k */
#define WDTIS_3           (3*0x0001u)  /* WDT - Timer Interval Select: /512k */
#define WDTIS_4           (4*0x0001u)  /* WDT - Timer Interval Select: /32k */
#define WDTIS_5           (5*0x0001u)  /* WDT - Timer Interval Select: /8192 */
#define WDTIS_6           (6*0x0001u)  /* WDT - Timer Interval Select: /512 */
#define WDTIS_7           (7*0x0001u)  /* WDT - Timer Interval Select: /64 */
#define WDTIS__2G         (0*0x0001u)  /* WDT - Timer Interval Select: /2G */
#define WDTIS__128M       (1*0x0001u)  /* WDT - Timer Interval Select: /128M */
#define WDTIS__8192K      (2*0x0001u)  /* WDT - Timer Interval Select: /8192k */
#define WDTIS__512K       (3*0x0001u)  /* WDT - Timer Interval Select: /512k */
#define WDTIS__32K        (4*0x0001u)  /* WDT - Timer Interval Select: /32k */
#define WDTIS__8192       (5*0x0001u)  /* WDT - Timer Interval Select: /8192 */
#define WDTIS__512        (6*0x0001u)  /* WDT - Timer Interval Select: /512 */
#define WDTIS__64         (7*0x0001u)  /* WDT - Timer Interval Select: /64 */

#define WDTSSEL_0         (0*0x0020u)  /* WDT - Timer Clock Source Select: SMCLK */
#define WDTSSEL_1         (1*0x0020u)  /* WDT - Timer Clock Source Select: ACLK */
#define WDTSSEL_2         (2*0x0020u)  /* WDT - Timer Clock Source Select: VLO_CLK */
#define WDTSSEL_3         (3*0x0020u)  /* WDT - Timer Clock Source Select: reserved */
#define WDTSSEL__SMCLK    (0*0x0020u)  /* WDT - Timer Clock Source Select: SMCLK */
#define WDTSSEL__ACLK     (1*0x0020u)  /* WDT - Timer Clock Source Select: ACLK */
#define WDTSSEL__VLO      (2*0x0020u)  /* WDT - Timer Clock Source Select: VLO_CLK */

/* WDT-interval times [1ms] coded with Bits 0-2 */
/* WDT is clocked by fSMCLK (assumed 1MHz) */
#define WDT_MDLY_32         (WDTPW+WDTTMSEL+WDTCNTCL+WDTIS2)                         /* 32ms interval (default) */
#define WDT_MDLY_8          (WDTPW+WDTTMSEL+WDTCNTCL+WDTIS2+WDTIS0)                  /* 8ms     " */
#define WDT_MDLY_0_5        (WDTPW+WDTTMSEL+WDTCNTCL+WDTIS2+WDTIS1)                  /* 0.5ms   " */
#define WDT_MDLY_0_064      (WDTPW+WDTTMSEL+WDTCNTCL+WDTIS2+WDTIS1+WDTIS0)           /* 0.064ms " */
/* WDT is clocked by fACLK (assumed 32KHz) */
#define WDT_ADLY_1000       (WDTPW+WDTTMSEL+WDTCNTCL+WDTIS2+WDTSSEL0)                /* 1000ms  " */
#define WDT_ADLY_250        (WDTPW+WDTTMSEL+WDTCNTCL+WDTIS2+WDTSSEL0+WDTIS0)         /* 250ms   " */
#define WDT_ADLY_16         (WDTPW+WDTTMSEL+WDTCNTCL+WDTIS2+WDTSSEL0+WDTIS1)         /* 16ms    " */
#define WDT_ADLY_1_9        (WDTPW+WDTTMSEL+WDTCNTCL+WDTIS2+WDTSSEL0+WDTIS1+WDTIS0)  /* 1.9ms   " */
/* Watchdog mode -> reset after expired time */
/* WDT is clocked by fSMCLK (assumed 1MHz) */
#define WDT_MRST_32         (WDTPW+WDTCNTCL+WDTIS2)                                  /* 32ms interval (default) */
#define WDT_MRST_8          (WDTPW+WDTCNTCL+WDTIS2+WDTIS0)                           /* 8ms     " */
#define WDT_MRST_0_5        (WDTPW+WDTCNTCL+WDTIS2+WDTIS1)                           /* 0.5ms   " */
#define WDT_MRST_0_064      (WDTPW+WDTCNTCL+WDTIS2+WDTIS1+WDTIS0)                    /* 0.064ms " */
/* WDT is clocked by fACLK (assumed 32KHz) */
#define WDT_ARST_1000       (WDTPW+WDTCNTCL+WDTSSEL0+WDTIS2)                         /* 1000ms  " */
#define WDT_ARST_250        (WDTPW+WDTCNTCL+WDTSSEL0+WDTIS2+WDTIS0)                  /* 250ms   " */
#define WDT_ARST_16         (WDTPW+WDTCNTCL+WDTSSEL0+WDTIS2+WDTIS1)                  /* 16ms    " */
#define WDT_ARST_1_9        (WDTPW+WDTCNTCL+WDTSSEL0+WDTIS2+WDTIS1+WDTIS0)           /* 1.9ms   " */

#endif

/************************************************************
* TLV Descriptors
************************************************************/
#define __MSP430_HAS_TLV__                /* Definition to show that Module is available */
#define TLV_BASE __MSP430_BASEADDRESS_TLV__

#define TLV_START             (0x1A08u)    /* Start Address of the TLV structure */
#define TLV_END               (0x1AFFu)    /* End Address of the TLV structure */

#define TLV_LDTAG             (0x01)      /*  Legacy descriptor (1xx, 2xx, 4xx families) */
#define TLV_PDTAG             (0x02)      /*  Peripheral discovery descriptor */
#define TLV_Reserved3         (0x03)      /*  Future usage */
#define TLV_Reserved4         (0x04)      /*  Future usage */
#define TLV_BLANK             (0x05)      /*  Blank descriptor */
#define TLV_Reserved6         (0x06)      /*  Future usage */
#define TLV_Reserved7         (0x07)      /*  Serial Number */
#define TLV_DIERECORD         (0x08)      /*  Die Record  */
#define TLV_ADCCAL            (0x11)      /*  ADC12 calibration */
#define TLV_ADC12CAL          (0x11)      /*  ADC12 calibration */
#define TLV_REFCAL            (0x12)      /*  REF calibration */
#define TLV_ADC10CAL          (0x13)      /*  ADC10 calibration */
#define TLV_TIMERDCAL         (0x15)      /*  TIMER_D calibration */
#define TLV_TAGEXT            (0xFE)      /*  Tag extender */
#define TLV_TAGEND            (0xFF)      /*  Tag End of Table */

/************************************************************
* Interrupt Vectors (offset from 0xFF80)
************************************************************/


/************************************************************
* End of Modules
************************************************************/
#pragma language=restore

#endif /* #ifndef __msp430FR2XX_FR4XXGENERIC */

