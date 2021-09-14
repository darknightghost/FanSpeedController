#include <platform.h>

/**
 * @brief       Initialize platform.
 */
void platform_init()
{
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
}
