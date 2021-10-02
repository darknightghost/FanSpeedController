#include <clock_io.h>
#include <platform.h>
#include <serial.h>

int main()
{
    // Initialize.
    platform_init();

    while (1) {
        timer0_isr_second_stage();
        // PCON |= 0x01;
    }
}
