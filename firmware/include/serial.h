#pragma once

#include <platform.h>

/**
 * @brief       Initialize serial.
 * Baud rate = 9600.
 */
extern void serial_init();

/**
 * @brief       Enable serial.
 */
extern void enable_serial();

/**
 * @brief       Serial ISR.
 */
extern void serial_isr(void) __interrupt INT_UART1;
