#pragma once

#include <types.h>

// Definations of STC8G1K08-36I-SOP8
// IRC = 33.1776 MHz

#if defined EDITOR_AUTO_COMPLETE
    // Auto-complete.
    #define __data
    #define __idata
    #define __xdata
    #define __at(addr)
    #define __sfr  uint8_t
    #define __sbit uint8_t
    #define __interrupt
    #define INT_TIMER0
    #define INT_INT1
    #define INT_UART1

#else
    #define INT_TIMER0 1
    #define INT_INT1   2
    #define INT_UART1  4

#endif

// I/O ports.
// P3
__sfr  __at(0xB0) P3;
__sbit __at(0xB0) P3_0;
__sbit __at(0xB1) P3_1;
__sbit __at(0xB2) P3_2;
__sbit __at(0xB3) P3_3;
__sbit __at(0xB4) P3_4;
__sbit __at(0xB5) P3_5;
__sbit __at(0xB6) P3_6;
__sbit __at(0xB7) P3_7;

__sfr __at(0xB1) P3M1;
__sfr __at(0xB2) P3M0;

#define P3PU (*((uint8_t *)0xFE12))

// P5
__sfr  __at(0xC8) P5;
__sbit __at(0xCC) P5_4;
__sbit __at(0xCD) P5_5;

__sfr __at(0xC9) P5M1;
__sfr __at(0xCA) P5M0;

#define P5PU (*((uint8_t *)0xFE15))

// Serial
__sfr __at(0x98) SCON;
__sfr __at(0x99) SBUF;
__sfr __at(0x87) PCON;

// Timer
__sfr __at(0x88) TCON;
__sfr __at(0x89) TMOD;
__sfr __at(0x8A) TL0;
__sfr __at(0x8B) TL1;
__sfr __at(0x8C) TH0;
__sfr __at(0x8D) TH1;
__sfr __at(0x8E) AUXR;

// Interrupt
__sfr __at(0xA8) IE;
__sfr __at(0xAF) IE2;
__sfr __at(0x8F) INTCLKO;
__sfr __at(0xB8) IP;
__sfr __at(0xB7) IPH;
__sfr __at(0xB5) IP2;
__sfr __at(0xB6) IP2H;
__sfr __at(0xDF) IP3;
__sfr __at(0xEE) IP3H;

#define EEPROM_SIZE      4096
#define EEPROM_PAGE_SIZE 512

__sfr __at(0xC2) IAP_DATA;
__sfr __at(0xC3) IAP_ADDRH;
__sfr __at(0xC4) IAP_ADDRL;
__sfr __at(0xC5) IAP_CMD;
__sfr __at(0xC6) IAP_TRIG;
__sfr __at(0xC7) IAP_CONTR;
__sfr __at(0xF5) IAP_TPS;

// Board
#define PWM_INPUT    P5_4
#define SPEED_OUTPUT P5_5
#define SPEED_INPUT  P3_3
#define PWM_OUTPUT   P3_2

// Generate bit mask.
#define MASK(type, value) (~((type)(value)))

/**
 * @brief       Initialize platform.
 */
extern void platform_init();

/**
 * @brief       Reboot.
 */
extern void reboot();
