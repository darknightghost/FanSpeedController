#include <clock_io.h>
#include <platform.h>
#include <serial.h>

/**
 * @brief       Initialize platform.
 */
void platform_init()
{
    // Initialize I/O ports.
    io_init();

    // Disable interruption.
    IE      = 0;
    IE2     = 0;
    INTCLKO = 0;

    // Set interrupt priority
    // Interrupt 1: 2
    // Serial 1: 0
    // Timer 0: 3
    IP  = 0x02;
    IPH = 0x06;

    IP2  = 0x00;
    IP2H = 0x00;

    IP3  = 0x00;
    IP3H = 0x00;

    // Initialize clock.
    clock_init();

    // Initialize serial.
    serial_init();

    // Initialize pwm.

    // Enable interruption.
    IE |= 0x80;

    // Enable functions.
    enable_clock();
    enable_io();
    // enable_serial();
}
