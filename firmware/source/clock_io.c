#include <types.h>

#include <clock_io.h>

#define TICK_US               17
#define SAMPLING_TTICKS       29411
#define SAMPLING_MICROSECONDS ((uint32_t)SAMPLING_TTICKS * TICK_US)

#define FLAG_SPEED_COUNT_UPDATED 0x01
#define FLAG_PWM_COUNT_UPDATED   0x02

static __data uint32_t l_boot_time = 0;                    ///< Boot time.
static __data uint8_t  l_mode      = FIRMWARE_MODE_NORMAL; ///< Firmware mode.

static __data uint8_t time0_flags = 0; ///< Timer 0 flags.

// Input Speed.
static __data uint16_t l_speed_current_sampling_tick
    = 0; ///< Curent sampling tick.
static __data uint16_t l_speed_current_sampling_count
    = 0;                                        ///< Sampling input speed count.
static __data uint16_t l_speed_input_count = 0; ///< Input speed count.
static __data uint16_t l_speed_input_hz    = 0; ///< Input speed hz.

// Input PWM.
#define PWM_SAMPLING_COUNT_MAX ((uint8_t)100)
static __data uint8_t l_input_sampling_pwm_count
    = 0; ///< Sampling input pwm count.
static __data uint8_t l_input_sampling_pwm_high_level_count
    = 0; ///< Sampling input pwm high level count.
static __data uint8_t l_input_pwm_high_level_count = 0; ///< Input pwm count.

/*
static __data uint16_t l_output_change_tick = 0; ///< Input speed count.
static __data uint16_t l_current_input_pwm_high_level_count
    = 0; ///< Curent input pwm high-level ticks.

static __data uint16_t l_output_speed_interval
    = 0xFF; ///< Inverval to output low-level on speed output pin.
static __data uint16_t l_output_speed_ticks_count
    = 0; ///< Count to output low-level on speed output pin.
*/
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
void timer0_isr() __interrupt INT_TIMER0
{
    TCON &= 0xDF;
    l_boot_time += 17;
    ++l_speed_current_sampling_tick;
    if (l_speed_current_sampling_tick >= SAMPLING_TTICKS) {
        // Input speed.
        l_speed_input_count            = l_speed_current_sampling_count;
        l_speed_current_sampling_count = 0;
        l_speed_current_sampling_tick  = 0;
        time0_flags |= FLAG_SPEED_COUNT_UPDATED;
    }
}

/**
 * @brief       Timer0 ISR second stage.
 */
void timer0_isr_second_stage()
{
    // Update speed.
    if (time0_flags & FLAG_SPEED_COUNT_UPDATED) {
        time0_flags &= MASK(uint8_t, FLAG_SPEED_COUNT_UPDATED);
        __idata uint32_t tmp = l_speed_input_count;
        tmp                  = tmp * 1000000 / SAMPLING_MICROSECONDS;
        l_speed_input_hz     = (uint16_t)tmp;
    }

    // Update pwm.
    if (time0_flags & FLAG_PWM_COUNT_UPDATED) {
        time0_flags &= MASK(uint8_t, FLAG_PWM_COUNT_UPDATED);
    }
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
 * @brief       Get input speed.
 */
uint16_t input_speed()
{
    return l_speed_input_hz;
}

/**
 * @brief       INT1 ISR.
 */
void int1_isr(void) __interrupt INT_INT3
{
    ++l_speed_current_sampling_count;
}

/**
 * @brief       PCA ISR.
 */
// 30 instruction cycle.
void pca_isr(void) __interrupt INT_PCA
{
    ++l_input_sampling_pwm_count;
    if (PWM_INPUT > 0) {
        ++l_input_sampling_pwm_high_level_count;
    }
    if (l_input_sampling_pwm_count >= PWM_SAMPLING_COUNT_MAX) {
        l_input_pwm_high_level_count = l_input_sampling_pwm_high_level_count;
        l_input_sampling_pwm_count   = 0;
        l_input_sampling_pwm_high_level_count = 0;
    }
}
