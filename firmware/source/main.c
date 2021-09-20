#include <clock_io.h>
#include <platform.h>
#include <serial.h>

int main()
{
    // Initialize.
    platform_init();

    while (1) {
        PCON |= 0x01;
    }
}
