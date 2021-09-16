#include <clock_io.h>
#include <platform.h>
#include <serial.h>

int main()
{
    // Initialize.
    platform_init();

    uint32_t tm    = boot_time() + 1000000;
    uint8_t  count = 0;
    while (1) {
        uint32_t curTime = boot_time();
        if (curTime >= tm) {
            tm += 1000000;
            SCON &= 0xFC;
            SBUF = count;
            ++count;
        }
    }
}
