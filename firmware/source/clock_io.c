#include <clock_io.h>

#define SAMPLING_TTICKS       11764
#define SAMPLING_MICROSECONDS (SAMPLING_TTICKS * 17)

static __data uint32_t l_boot_time = 0;                    ///< Boot time.
static __data uint8_t  l_mode      = FIRMWARE_MODE_NORMAL; ///< Firmware mode.

static __data uint16_t l_current_sampling_tick = 0; ///< Curent sampling tick.
static __data uint16_t l_current_input_speed_tick_count
    = 0; ///< Input speed ticks count.
static __data uint16_t l_current_input_pwm_high_level_count
    = 0; ///< Curent input pwm high-level ticks.

static __data uint16_t l_output_speed_interval
    = 0xFF; ///< Inverval to output low-level on speed output pin.
static __data uint16_t l_output_speed_ticks_count
    = 0; ///< Count to output low-level on speed output pin.

static __data uint8_t
    l_pwm_map[20]; ///< Count to output low-level on speed output pin.
static __data uint16_t l_sourceFullSpeed; ///< Source full speed.
static __data uint16_t l_targetFullSpeed; ///< Target full speed.

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
    __idata uint32_t ret = l_boot_time;
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
void int1_isr(void) __interrupt INT_INT1
{
    ++l_current_input_speed_tick_count;
}
