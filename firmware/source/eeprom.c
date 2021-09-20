#include <eeprom.h>
#include <platform.h>

/**
 * @brief       header.
 */
struct record_allocation_table {
    uint8_t bitmap[2 * 8];
    uint8_t err;
};

#define PAGE_0 0x0000
#define PAGE_1 0x0200
#define PAGE_2 0x0400
#define PAGE_3 0x0600
#define PAGE_4 0x0800
#define PAGE_5 0x0A00
#define PAGE_6 0x0C00
#define PAGE_7 0x0E00

_Static_assert(sizeof(struct record_allocation_table) < RECORD_SIZE,
               "Record allocation table is too big!");

static __xdata struct record_allocation_table rat; ///< Record allocation table.

/**
 * @brief       Read byte.
 *
 * @param[in]   addr        Address of the byte.
 * @param[out]  byte        Byte read.
 *
 * @return      On success, the method returns 0, otherwise returns -1.
 */
static inline int8_t eeprom_read_byte(uint16_t addr, uint8_t *byte)
{
    IAP_CONTR &= 0xEF;
    IAP_ADDRL = addr & 0xFF;
    IAP_ADDRH = (addr & 0xFF00) >> 8;
    IAP_CMD &= 0xFC;
    IAP_CMD |= 0x01;
    IAP_TRIG = 0x5A;
    IAP_TRIG = 0xA5;

    if (IAP_CONTR & 0x10) {
        return -1;
    } else {
        *byte = IAP_DATA;
        return 0;
    }
}

/**
 * @brief       Write byte.
 *
 * @param[in]   addr        Address of the byte.
 * @param[out]  byte        Byte to write.
 *
 * @return      On success, the method returns 0, otherwise returns -1.
 */
static inline int8_t eeprom_write_byte(uint16_t addr, uint8_t byte)
{
    IAP_CONTR &= 0xEF;
    IAP_ADDRL = addr & 0xFF;
    IAP_ADDRH = (addr & 0xFF00) >> 8;
    IAP_DATA  = byte;
    IAP_CMD &= 0xFC;
    IAP_CMD |= 0x10;
    IAP_TRIG = 0x5A;
    IAP_TRIG = 0xA5;

    if (IAP_CONTR & 0x10) {
        return -1;
    } else {
        return 0;
    }
}

/**
 * @brief       Read bytes.
 *
 * @param[in]   addr        Address of the byte.
 * @param[out]  bytes       Bytes read.
 * @param[in]   size        Size.
 *
 * @return      On success, the method returns 0, otherwise returns bytes read.
 */
static inline int8_t
    eeprom_read_bytes(uint16_t addr, uint8_t *byte, uint8_t size)
{
    for (uint8_t i = 0; i < size; ++i) {
        if (eeprom_read_byte(addr + i, byte + i) < 0) {
            return -1;
        }
    }

    return (int8_t)size;
}

/**
 * @brief       Write bytes.
 *
 * @param[in]   addr        Address of the byte.
 * @param[in]   bytes       Bytes to write.
 * @param[in]   size        Size.
 *
 * @return      On success, the method returns 0, otherwise returns bytes
 *              written.
 */
static inline int8_t
    eeprom_write_bytes(uint16_t addr, const uint8_t *byte, uint8_t size)
{
    for (uint8_t i = 0; i < size; ++i) {
        if (eeprom_write_byte(addr + i, *(byte + i)) < 0) {
            return -1;
        }
    }

    return (int8_t)size;
}

/**
 * @brief       Erase.
 *
 * @param[in]   addr        Address of the byte.
 *
 * @return      On success, the method returns 0, otherwise returns -1.
 */
static inline int8_t eeprom_erase(uint16_t addr)
{
    IAP_CONTR &= 0xEF;
    IAP_ADDRL = addr & 0xFF;
    IAP_ADDRH = (addr & 0xFF00) >> 8;
    IAP_CMD &= 0xFC;
    IAP_CMD |= 0x11;
    IAP_TRIG = 0x5A;
    IAP_TRIG = 0xA5;

    if (IAP_CONTR & 0x10) {
        return -1;
    } else {
        return 0;
    }
}

/**
 * @brief       Format.
 *
 * @return      On success, the method returns 0, otherwise returns -1.
 */
static inline int8_t eeprom_format()
{
    // Erase all.
    if (eeprom_erase(PAGE_0) < 0) {
        return -1;
    }

    if (eeprom_erase(PAGE_1) < 0) {
        return -1;
    }

    if (eeprom_erase(PAGE_2) < 0) {
        return -1;
    }

    if (eeprom_erase(PAGE_3) < 0) {
        return -1;
    }

    if (eeprom_erase(PAGE_4) < 0) {
        return -1;
    }

    if (eeprom_erase(PAGE_5) < 0) {
        return -1;
    }

    if (eeprom_erase(PAGE_6) < 0) {
        return -1;
    }

    if (eeprom_erase(PAGE_7) < 0) {
        return -1;
    }

    // Make RAT.
    rat.bitmap[0] = 0xFE;
    for (uint8_t i = 1; i < 16; ++i) {
        rat.bitmap[i] = 0xFF;
    }
    rat.err = 0;

    // Write RAT.
    if (eeprom_write_bytes(PAGE_0, (uint8_t *)(&rat), sizeof(rat)) < 0) {
        return -1;
    }

    return 0;
}

/**
 * @brief       Get address to read.
 */
uint16_t get_read_addr()
{
    uint8_t group_count = 0;
    for (; group_count < 16; ++group_count) {
        if (rat.bitmap[group_count] != 0x00) {
            break;
        }
    }

    if (group_count == 16) {
        return (8 * 16 - 1) * RECORD_SIZE;
    }

    switch (rat.bitmap[group_count]) {
        case 0xFF:
            return (8 * group_count - 1 + 0) * RECORD_SIZE;

        case 0xFE:
            return (8 * group_count - 1 + 1) * RECORD_SIZE;

        case 0xFC:
            return (8 * group_count - 1 + 2) * RECORD_SIZE;

        case 0xF8:
            return (8 * group_count - 1 + 3) * RECORD_SIZE;

        case 0xF0:
            return (8 * group_count - 1 + 4) * RECORD_SIZE;

        case 0xE0:
            return (8 * group_count - 1 + 5) * RECORD_SIZE;

        case 0xC0:
            return (8 * group_count - 1 + 6) * RECORD_SIZE;

        case 0x80:
            return (8 * group_count - 1 + 7) * RECORD_SIZE;

        default:
            rat.err = 1;
            eeprom_write_bytes(PAGE_0, (uint8_t *)(&rat), sizeof(rat));
            reboot();
            return 0;
    }
}

/**
 * @brief       Allocate address to write.
 */
uint16_t allocate_write_addr()
{
    uint8_t group_count = 0;
    for (; group_count < 16; ++group_count) {
        if (rat.bitmap[group_count] != 0x00) {
            break;
        }
    }

    if (group_count == 16) {
        eeprom_format();
        // Write RAT.
        rat.bitmap[0] = 0xFC;
        if (eeprom_write_bytes(PAGE_0, (uint8_t *)(&rat), sizeof(rat)) < 0) {
            return -1;
        }
        return RECORD_SIZE;
    }

    switch (rat.bitmap[group_count]) {
        case 0xFF:
            // Write RAT.
            rat.bitmap[group_count] = 0xFE;
            if (eeprom_write_bytes(PAGE_0, (uint8_t *)(&rat), sizeof(rat))
                < 0) {
                return -1;
            }
            return (8 * group_count + 0) * RECORD_SIZE;

        case 0xFE:
            // Write RAT.
            rat.bitmap[group_count] = 0xFC;
            if (eeprom_write_bytes(PAGE_0, (uint8_t *)(&rat), sizeof(rat))
                < 0) {
                return -1;
            }
            return (8 * group_count + 1) * RECORD_SIZE;

        case 0xFC:
            // Write RAT.
            rat.bitmap[group_count] = 0xF8;
            if (eeprom_write_bytes(PAGE_0, (uint8_t *)(&rat), sizeof(rat))
                < 0) {
                return -1;
            }
            return (8 * group_count + 2) * RECORD_SIZE;

        case 0xF8:
            // Write RAT.
            rat.bitmap[group_count] = 0xF0;
            if (eeprom_write_bytes(PAGE_0, (uint8_t *)(&rat), sizeof(rat))
                < 0) {
                return -1;
            }
            return (8 * group_count + 3) * RECORD_SIZE;

        case 0xF0:
            // Write RAT.
            rat.bitmap[group_count] = 0xE0;
            if (eeprom_write_bytes(PAGE_0, (uint8_t *)(&rat), sizeof(rat))
                < 0) {
                return -1;
            }
            return (8 * group_count + 4) * RECORD_SIZE;

        case 0xE0:
            // Write RAT.
            rat.bitmap[group_count] = 0xC0;
            if (eeprom_write_bytes(PAGE_0, (uint8_t *)(&rat), sizeof(rat))
                < 0) {
                return -1;
            }
            return (8 * group_count + 5) * RECORD_SIZE;

        case 0xC0:
            // Write RAT.
            rat.bitmap[group_count] = 0x80;
            if (eeprom_write_bytes(PAGE_0, (uint8_t *)(&rat), sizeof(rat))
                < 0) {
                return -1;
            }
            return (8 * group_count + 6) * RECORD_SIZE;

        case 0x80:
            // Write RAT.
            rat.bitmap[group_count] = 0x00;
            if (eeprom_write_bytes(PAGE_0, (uint8_t *)(&rat), sizeof(rat))
                < 0) {
                return -1;
            }
            return (8 * group_count + 7) * RECORD_SIZE;

        default:
            rat.err = 1;
            eeprom_write_bytes(PAGE_0, (uint8_t *)(&rat), sizeof(rat));
            reboot();
            return 0;
    }
}

/**
 * @brief       Initialize.
 */
void eeprom_init()
{
    IAP_CONTR |= 0x80;
    IAP_TPS = 33;

    // Read RAT;
    if (eeprom_read_bytes(PAGE_0, (uint8_t *)(&rat), sizeof(rat)) < 0) {
        reboot();
    }

    if ((rat.bitmap[0] & 0x01) || rat.err) {
        eeprom_format();

        // Make default record.
        __xdata struct config_record default_record;
        for (uint8_t i = 0; i < 20; ++i) {
            default_record.pwm_map[i] = 100;
        }
        default_record.sourceFullSpeed = 1000;
        default_record.targetFullSpeed = 1000;

        // Write record.
        eeprom_write_record(&default_record);
    }
}

/**
 * @brief       Read record.
 */
void eeprom_read_record(struct config_record *record)
{
    uint16_t addr = get_read_addr();
    eeprom_read_bytes(addr, (uint8_t *)record, sizeof(struct config_record));
}

/**
 * @brief       Write record.
 */
void eeprom_write_record(struct config_record *record)
{
    uint16_t addr = allocate_write_addr();
    eeprom_write_bytes(addr, (uint8_t *)record, sizeof(struct config_record));
}
