#include <platform.h>

int main()
{
    // Initialize.
    platform_init();

    while (1) {
        P3_2 = P3_3;
        P5_5 = P5_4;
    }
}
