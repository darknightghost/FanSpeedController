#include <command.h>

#include <platform.h>
#include <serial.h>

/**
 * @brief       Initialize serial.
 */
void serial_init()
{
    // Enable serial.
    PCON &= 0x7F;
    SCON = 0x50;
    AUXR &= 0xBF;
    AUXR &= 0xFE;
    TMOD &= 0x0F;
    TMOD |= 0x20;
    TL1 = 0xF7;
    TH1 = 0xF7;
    TCON |= 0x40;
}

/**
 * @brief       Enable serial.
 */
void enable_serial()
{
    IE |= 0x10;
}

/**
 * @brief       Serial ISR.
 */
void serial_isr(void) __interrupt INT_UART1
{
    if (SCON & 0x01) {
        // Received.
        uint8_t val = SBUF;
        SBUF        = val;
        while (! (SCON & 0x02)) {
            // Sent.
        }
    }

    SCON &= 0xFC;
}
