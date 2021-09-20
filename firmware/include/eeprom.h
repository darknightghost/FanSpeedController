#pragma once

#include <stdint.h>

#define RECORD_SIZE 32

/**
 * @brief       Record.
 */
struct config_record {
    uint8_t  pwm_map[20];     ///< PWM map.
    uint32_t sourceFullSpeed; ///< Source full speed.
    uint32_t targetFullSpeed; ///< Target full speed.
};

_Static_assert(sizeof(struct config_record) < RECORD_SIZE,
               "Record size is too big!");

/**
 * @brief       Initialize.
 */
void eeprom_init();

/**
 * @brief       Read record.
 *
 * @param[out]  record      Record.
 */
void eeprom_read_record(struct config_record *record);

/**
 * @brief       Write record.
 *
 * @param[out]  record      Record.
 */
void eeprom_write_record(struct config_record *record);
