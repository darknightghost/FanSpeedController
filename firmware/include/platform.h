#pragma once

#include <stdint.h>

// Definations of STC8G1K08-36I-SOP8

#if defined EDITOR_AUTO_COMPLETE
    // Auto-complete.
    #define __data
    #define __idata
    #define __xdata
    #define __at(addr)
    #define __sfr  uint8_t
    #define __sbit uint8_t

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

/**
 * @brief       Initialize platform.
 */
extern void platform_init();
