#include <platform.h>
#include <serial.h>

int main()
{
    // Initialize.
    platform_init();

    // SBUF = 'c';
    while (1) {
        /*
        if (SCON & 0x02) {
            // Sent.
            SCON &= 0xFD;
            SBUF = 'C';
        }
        */
    }
}
