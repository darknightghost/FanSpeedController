#pragma once

#include <platform.h>

/**
 * @brief       Initialize serial.
 * Baud rate = 9600.
 */
extern void serial_init();

/**
 * @brief       Serial ISR.
 */
extern void serial_isr(void) __interrupt 4;
