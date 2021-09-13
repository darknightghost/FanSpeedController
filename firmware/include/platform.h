#pragma once

#include <stdint.h>

/**
 * @brief       Register data type.
 */
union register_t {
    uint8_t value; ///< Value of the port.
    struct {
        uint8_t bit0 : 1; ///< Bit 0.
        uint8_t bit1 : 1; ///< Bit 1.
        uint8_t bit2 : 1; ///< Bit 2.
        uint8_t bit3 : 1; ///< Bit 3.
        uint8_t bit4 : 1; ///< Bit 4.
        uint8_t bit5 : 1; ///< Bit 5.
        uint8_t bit6 : 1; ///< Bit 6.
        uint8_t bit7 : 1; ///< Bit 7.
    } __attribute__((packed)) bits;
} __attribute__((packed));

_Static_assert(sizeof(register_t) == 1);

// Definations of STC8G1K08-36I-SOP8
// I/O ports.
// P3
#define P3_REG (*((union register_t *)(0xB0)))
#define P3     (P3_REG.value)
#define P3_0   (P3_REG.bits.bit0)
#define P3_1   (P3_REG.bits.bit1)
#define P3_2   (P3_REG.bits.bit2)
#define P3_3   (P3_REG.bits.bit3)

// P5
#define P5_REG (*((union register_t *)(0xC8)))
#define P5     (P5_REG.value)
#define P5_4   (P5_REG.bits.bit4)
#define P5_5   (P5_REG.bits.bit5)
