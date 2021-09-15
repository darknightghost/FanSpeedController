#include <platform.h>
#include <serial.h>

/**
 * @brief       Initialize platform.
 */
void platform_init()
{
    // Initialize I/O ports.
    // P3
    P3PU = 0x03;
    P5PU = 0;

    P3M1 = 0xFC;
    P3M0 = 0x0C;
    P3   = 0xFF;

    // P5
    P5M1 = 0xFF;
    P5M0 = 0x30;
    P5   = 0xFF;

    // Initialize interruption.
    // EA=0, ES=1, ET0=1, EX1=1
    IE      = 0x16;
    IE2     = 0;
    INTCLKO = 0;
    // Interrupt 1 : falling edge trigge.
    TCON = 0x04;

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

    // Initialize serial.
    serial_init();

    // Initialize clock.

    // Initialize pwm.

    // Enable interruption.
    IE |= 0x80;

    // Start the clock.
}
