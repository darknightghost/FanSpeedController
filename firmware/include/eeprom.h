#pragma once

#include <stdint.h>

#include <command.h>

#define RECORD_SIZE 64

/**
 * @brief       Record.
 */
struct config_record {
    struct FirmwareConfig config;
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
