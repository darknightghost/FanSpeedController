#include <eeprom.h>
#include <platform.h>

/**
 * @brief       header.
 */
struct record_allocation_table {
    uint8_t records0_0; ///< 0x0000-0x007F.
    uint8_t records0_1; ///< 0x0080-0x01FF.
    uint8_t records1_0; ///< 0x0200-0x027F.
    uint8_t records1_1; ///< 0x0280-0x03FF
    uint8_t records2_0; ///< 0x0400-0x047F.
    uint8_t records2_1; ///< 0x0480-0x05FF.
    uint8_t records3_0; ///< 0x0600-0x067F.
    uint8_t records3_1; ///< 0x0680-0x07FF.
    uint8_t records4_0; ///< 0x0800-0x087F.
    uint8_t records4_1; ///< 0x0880-0x09FF.
    uint8_t records5_0; ///< 0x0A00-0x0A7F.
    uint8_t records5_1; ///< 0x0A80-0x0BFF.
    uint8_t records6_0; ///< 0x0C00-0x0C7F.
    uint8_t records6_1; ///< 0x0C80-0x0DFF.
    uint8_t records7_0; ///< 0x0E00-0x0E7F.
    uint8_t records7_1; ///< 0x0E80-0x0FFF.
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
static __xdata struct config_record default_record; ///< Default record.

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
    rat.records0_0 = 0xFC;
    rat.records0_1 = 0xFF;
    rat.records1_0 = 0xFF;
    rat.records1_1 = 0xFF;
    rat.records2_0 = 0xFF;
    rat.records2_1 = 0xFF;
    rat.records3_0 = 0xFF;
    rat.records3_1 = 0xFF;
    rat.records4_0 = 0xFF;
    rat.records4_1 = 0xFF;
    rat.records5_0 = 0xFF;
    rat.records5_1 = 0xFF;
    rat.records6_0 = 0xFF;
    rat.records6_1 = 0xFF;
    rat.records7_0 = 0xFF;
    rat.records7_1 = 0xFF;

    // Write RAT.
    if (eeprom_write_bytes(PAGE_0, (uint8_t *)(&rat), sizeof(rat)) < 0) {
        return -1;
    }

    // Make default record.
    for (uint8_t i = 0; i < 20; ++i) {
        default_record.pwm_map[i] = 100;
    }
    default_record.sourceFullSpeed = 1000;
    default_record.targetFullSpeed = 1000;

    // Write record.
    if (eeprom_write_bytes(PAGE_0 + RECORD_SIZE, (uint8_t *)(&default_record),
                           sizeof(default_record))
        < 0) {
        return -1;
    }

    return 0;
}

/**
 * @brief       Initialize.
 */
void eeprom_init()
{
    IAP_CONTR |= 0x80;
    IAP_TPS = 33;
}

/**
 * @brief       Read record.
 */
void eeprom_read_record(struct config_record *record);

/**
 * @brief       Write record.
 */
void eeprom_write_record(struct config_record *record);
