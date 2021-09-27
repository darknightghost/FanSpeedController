#include <command.h>

#include <clock_io.h>
#include <platform.h>
#include <serial.h>

#define READ_TIMEOUT 100000 ///< 100ms

/**
 * @brief       Initialize serial.
 */
void serial_init()
{
    // Enable serial.
    PCON &= 0x7F;
    SCON = 0x50;
    AUXR &= 0xBF;
    AUXR &= 0xFE;
    TMOD &= 0x0F;
    TMOD |= 0x20;
    TL1 = 0xF7;
    TH1 = 0xF7;
    TCON |= 0x40;
}

/**
 * @brief       Enable serial.
 */
void enable_serial()
{
    IE |= 0x10;
}

/**
 * @brief       Read byte.
 *
 * @param[out]  byte        Byte read. Do not change when failed.
 *
 * @return      On success, the method  returns 0, otherwise returns -1.
 */
static inline int serial_read_byte(uint8_t *byte)
{
    __idata uint32_t begin_time = boot_time();

    // Wait for data.
    while (! (SCON & 0x01)) {
        if (boot_time() - begin_time > READ_TIMEOUT) {
            // Timeout.
            return -1;
        }
    }

    // Read.
    *byte = SBUF;
    SCON &= 0xFE;

    return 0;
}

/**
 * @brief       Write byte.
 *
 * @param[in]  byte        Byte to write.
 */
static inline void serial_write_byte(uint8_t byte)
{
    // Clear send status.
    SCON &= 0xFD;

    // Send.
    SBUF = byte;

    // Wait.
    while (! (SCON & 0x02)) {
    }
    // Clear send status.
    SCON &= 0xFD;

    return;
}

/**
 * @brief       Write bytes.
 *
 * @param[in]  bytes        Bytes to write.
 * @param[in]  size         Size of bytes.
 */
static inline void serial_write_bytes(uint8_t *bytes, uint8_t size)
{
    for (uint8_t i = 0; i < size; ++i) {
        serial_write_byte(*bytes);
        ++bytes;
    }

    return;
}

/**
 * @brief       Get firmware mode.
 */
static inline void cmd_get_mode()
{
    __xdata struct ReplyGetMode reply;
    reply.header.replyType = REPLY_TYPE_SUCCESS;
    reply.mode             = current_mode();
    serial_write_bytes((uint8_t *)(&reply), (uint8_t)sizeof(reply));
}

/**
 * @brief       Set firmware mode.
 *
 * @param[in]   mode    Mode.
 */
static inline void cmd_set_mode(uint8_t mode)
{
    set_current_mode(mode);
    __xdata struct ReplySetMode reply;
    reply.header.replyType = REPLY_TYPE_SUCCESS;
    serial_write_bytes((uint8_t *)(&reply), (uint8_t)sizeof(reply));
}

/**
 * @brief       Read output port.
 *
 * @param[in]   port        Port to read.
 */
static inline void cmd_read_port(uint8_t port)
{
    // Check.
    if (current_mode() != FIRMWARE_MODE_TEST) {
        serial_write_byte(REPLY_TYPE_FAILED);
        return;
    }

    // Reply.
    __xdata struct ReplyReadPort reply;
    reply.header.replyType = REPLY_TYPE_SUCCESS;
    switch (port) {
        case PORT_READ_PWM_INPUT:
            reply.value = PWM_INPUT ? 1 : 0;
            break;

        case PORT_READ_SPEED_INPUT:
            reply.value = SPEED_INPUT ? 1 : 0;
            break;
    }

    serial_write_bytes((uint8_t *)(&reply), (uint8_t)sizeof(reply));
}

/**
 * @brief       Write input port.
 *
 * @param[in]   port        Port to read.
 * @param[in]   value       Value to write.
 */
static inline void cmd_write_port(uint8_t port, uint8_t value)
{
    // Check.
    if (current_mode() != FIRMWARE_MODE_TEST) {
        serial_write_byte(REPLY_TYPE_FAILED);
        return;
    }

    // Reply.
    __xdata struct ReplyWritePort reply;
    reply.header.replyType = REPLY_TYPE_SUCCESS;
    switch (port) {
        case PORT_WRITE_SPEED_OUTPUT:
            SPEED_OUTPUT = value ? 1 : 0;
            break;

        case PORT_WRITE_PWM_OUTPUT:
            PWM_OUTPUT = value ? 1 : 0;
            break;
    }

    serial_write_bytes((uint8_t *)(&reply), (uint8_t)sizeof(reply));
}

/**
 * @brief       Read fan speed.
 */
static inline void cmd_read_speed() {}

/**
 * @brief       Set output pwm.
 */
static inline void cmd_set_pwm() {}

/**
 * @brief       Read config.
 */
static inline void cmd_read_config() {}

/**
 * @brief       Write config.
 */
static inline void cmd_write_config() {}

/**
 * @brief       Read clock.
 */
static inline void cmd_read_clock()
{
    // Reply.
    __xdata struct ReplyReadClock reply;
    reply.header.replyType = REPLY_TYPE_SUCCESS;
    reply.bootTime         = boot_time();

    serial_write_bytes((uint8_t *)(&reply), (uint8_t)sizeof(reply));
}

/**
 * @brief       Handle command.
 */
static inline void serial_on_command()
{
    __xdata uint8_t byte;

    // Read first byte.
    if (serial_read_byte(&byte) < 0) {
        serial_write_byte(REPLY_TYPE_FAILED);
        return;
    }
    // Parse beginning of the command.
    if (byte != CMD_BEGIN) {
        serial_write_byte(REPLY_TYPE_FAILED);
        return;
    }

_PARSE_CMD:
    // Read second byte.
    if (serial_read_byte(&byte) < 0) {
        serial_write_byte(REPLY_TYPE_FAILED);
        return;
    }

    // Parse command.
    switch (byte) {
        case CMD_BEGIN: {
            serial_write_byte(REPLY_TYPE_FAILED);
            goto _PARSE_CMD;
        }

        case CMD_TYPE_GET_MODE: {
            goto _PARSE_CMD_GET_MODE;
        }

        case CMD_TYPE_SET_MODE: {
            goto _PARSE_CMD_SET_MODE;
        }

        case CMD_TYPE_READ_PORT: {
            goto _PARSE_CMD_READ_PORT;
        }

        case CMD_TYPE_WRITE_PORT: {
            goto _PARSE_CMD_WRITE_PORT;
        }

        case CMD_TYPE_READ_SPEED: {
            goto _PARSE_CMD_READ_SPEED;
        }

        case CMD_TYPE_SET_PWM: {
            goto _PARSE_CMD_SET_PWM;
        }

        case CMD_TYPE_READ_CONFIG: {
            goto _PARSE_CMD_READ_CONFIG;
        }

        case CMD_TYPE_WRITE_CONFIG: {
            goto _PARSE_CMD_WRITE_CONFIG;
        }

        case CMD_TYPE_READ_CLOCK: {
            goto _PARSE_CMD_READ_CLOCK;
        }

        default: {
            serial_write_byte(REPLY_TYPE_FAILED);
            return;
        }
    }

    // Parse commands.
_PARSE_CMD_GET_MODE : {
    cmd_get_mode();
    return;
}

_PARSE_CMD_SET_MODE : {
    // Read first argument.
    if (serial_read_byte(&byte) < 0) {
        serial_write_byte(REPLY_TYPE_FAILED);
        return;
    }

    // Parse first argument.
    switch (byte) {
        case CMD_BEGIN: {
            serial_write_byte(REPLY_TYPE_FAILED);
            goto _PARSE_CMD;
        }

        case FIRMWARE_MODE_NORMAL:
        case FIRMWARE_MODE_MANUAL:
        case FIRMWARE_MODE_TEST: {
            cmd_set_mode(byte);
            return;
        }

        default: {
            serial_write_byte(REPLY_TYPE_FAILED);
            return;
        }
    }
}

_PARSE_CMD_READ_PORT : {
    // Read first argument.
    if (serial_read_byte(&byte) < 0) {
        serial_write_byte(REPLY_TYPE_FAILED);
        return;
    }

    // Parse first argument.
    switch (byte) {
        case CMD_BEGIN: {
            serial_write_byte(REPLY_TYPE_FAILED);
            goto _PARSE_CMD;
        }

        case PORT_READ_PWM_INPUT:
        case PORT_READ_SPEED_INPUT: {
            cmd_read_port(byte);
            return;
        }

        default: {
            serial_write_byte(REPLY_TYPE_FAILED);
            return;
        }
    }
}

_PARSE_CMD_WRITE_PORT : {
    // Read first argument.
    __xdata uint8_t port;
    if (serial_read_byte(&port) < 0) {
        serial_write_byte(REPLY_TYPE_FAILED);
        return;
    }

    // Parse first argument.
    switch (port) {
        case CMD_BEGIN: {
            serial_write_byte(REPLY_TYPE_FAILED);
            goto _PARSE_CMD;
        }

        case PORT_WRITE_SPEED_OUTPUT:
        case PORT_WRITE_PWM_OUTPUT: {
            break;
        }

        default: {
            serial_write_byte(REPLY_TYPE_FAILED);
            return;
        }
    }

    // Read second argument.
    __xdata uint8_t value;
    if (serial_read_byte(&value) < 0) {
        serial_write_byte(REPLY_TYPE_FAILED);
        return;
    }

    // Parse second argument.
    switch (value) {
        case CMD_BEGIN: {
            serial_write_byte(REPLY_TYPE_FAILED);
            goto _PARSE_CMD;
        }

        case 0:
        case 1: {
            cmd_write_port(port, value);
            break;
        }

        default: {
            serial_write_byte(REPLY_TYPE_FAILED);
            return;
        }
    }
    return;
}

_PARSE_CMD_READ_SPEED : {
    serial_write_byte(REPLY_TYPE_FAILED);
    return;
}

_PARSE_CMD_SET_PWM : {
    serial_write_byte(REPLY_TYPE_FAILED);
    return;
}

_PARSE_CMD_READ_CONFIG : {
    serial_write_byte(REPLY_TYPE_FAILED);
    return;
}

_PARSE_CMD_WRITE_CONFIG : {
    serial_write_byte(REPLY_TYPE_FAILED);
    return;
}

_PARSE_CMD_READ_CLOCK : {
    cmd_read_clock();
    return;
}
}

/**
 * @brief       Serial ISR.
 */
void serial_isr(void) __interrupt INT_UART1
{
    if (SCON & 0x01) {
        // Received.
        // Disable interruption.
        IE &= 0xEF;

        // Handle command.
        serial_on_command();

        // Enable interruption.
        IE |= 0x10;
    }

    if (SCON & 0x02) {
        // Clear send status.
        SCON &= 0xFD;
    }
}
