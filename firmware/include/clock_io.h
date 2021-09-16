#pragma once

#include <stdint.h>

#include <platform.h>

/// 17μs a tick.
/**
 * @brief       Initialize clock.
 */
extern void clock_init();

/**
 * @brief       Enable the clock.
 */
extern void enable_clock();

/**
 * @brief       Get boot time(μs).
 */
extern uint32_t boot_time();

/**
 * @brief       Timer0 ISR.
 */
extern void timer0_isr(void) __interrupt INT_TIMER0;

/**
 * @brief       Initialize I/O.
 */
extern void io_init();

/**
 * @brief       Enable I/O.
 */
extern void enable_io();

/**
 * @brief       INT1 ISR.
 */
extern void int1_isr(void) __interrupt INT_INT1;
