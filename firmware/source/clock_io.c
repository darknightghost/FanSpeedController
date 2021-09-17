#include <clock_io.h>

static uint32_t l_boot_time = 0;
static uint8_t  l_mode      = FIRMWARE_MODE_NORMAL;

/**
 * @brief       Initialize clock.
 */
void clock_init()
{
    TCON &= 0xEF;
    AUXR |= 0x80;
    TMOD &= 0xF0;
    TL0 = 0xCC;
    TH0 = 0xFD;
}

/**
 * @brief       Enable the clock.
 */
void enable_clock()
{
    IE |= 0x02;
    TCON |= 0x10;
}

/**
 * @brief       Get boot time(μs).
 */
uint32_t boot_time()
{
    IE &= 0xFD;
    uint32_t ret = l_boot_time;
    IE |= 0x02;
    return ret;
}

/**
 * @brief       Timer0 ISR.
 */
void timer0_isr(void) __interrupt INT_TIMER0
{
    TCON &= 0xDF;
    l_boot_time += 17;
}

/**
 * @brief       Initialize I/O.
 */
void io_init()
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

    // Interrupt 1 : falling edge trigge.
    TCON |= 0x04;
    TCON &= 0xF7;
}

/**
 * @brief       Enable I/O.
 */
void enable_io()
{
    IE |= 0x04;
}

/**
 * @brief       Get current mode.
 */
uint8_t current_mode()
{
    return l_mode;
}

/**
 * @brief       Set current mode.
 */
void set_current_mode(uint8_t mode)
{
    l_mode = mode;
}

/**
 * @brief       INT1 ISR.
 */
void int1_isr(void) __interrupt INT_INT1 {}
