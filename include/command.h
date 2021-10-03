#pragma once

#if defined BUILD_FIRMWARE
    #include <types.h>
#else
    #if defined __cplusplus
        #include <cstdint>
    #else
        #include <stdint.h>
    #endif
#endif

#if ! defined BUILD_FIRMWARE
    #pragma pack(push, 1)
#endif

/// Begining of the command.
#define CMD_BEGIN ((uint8_t)0xFF)

/// Mode command.
#define CMD_TYPE_GET_MODE ((uint8_t)0x00)
#define CMD_TYPE_SET_MODE ((uint8_t)0x01)

// Mode.
#define FIRMWARE_MODE_NORMAL ((uint8_t)0x00)
#define FIRMWARE_MODE_MANUAL ((uint8_t)0x01)
#define FIRMWARE_MODE_TEST   ((uint8_t)0x02)

/// Board test command, test mode only.
#define CMD_TYPE_READ_PORT  ((uint8_t)0x10)
#define CMD_TYPE_WRITE_PORT ((uint8_t)0x11)

/// Port to read/write.
#define PORT_READ_PWM_INPUT     ((uint8_t)0x00)
#define PORT_READ_SPEED_INPUT   ((uint8_t)0x01)
#define PORT_WRITE_SPEED_OUTPUT ((uint8_t)0x00)
#define PORT_WRITE_PWM_OUTPUT   ((uint8_t)0x01)

// Status monitor command.
#define CMD_TYPE_GET_INPUT_SPEED ((uint8_t)0x20)
#define CMD_TYPE_GET_INPUT_PWM   ((uint8_t)0x21)

/// Fan test command, manual mode only.
#define CMD_TYPE_SET_OUTPUT_SPEED ((uint8_t)0x30)
#define CMD_TYPE_SET_OUTPUT_PWM   ((uint8_t)0x31)

/// Config.
#define CMD_TYPE_READ_CONFIG  ((uint8_t)0x40)
#define CMD_TYPE_WRITE_CONFIG ((uint8_t)0x41)

/// Read clock.
#define CMD_TYPE_READ_CLOCK ((uint8_t)0x50)

#define REPLY_TYPE_FAILED  ((uint8_t)0x00) ///< Command failed.
#define REPLY_TYPE_SUCCESS ((uint8_t)0x01) ///< Success.

#if defined __cplusplus
/**
 * @brief       Command type.
 */
enum class CMDType : uint8_t {
    GetMode        = CMD_TYPE_GET_MODE,         ///< Get firmware mode.
    SetMode        = CMD_TYPE_SET_MODE,         ///< Set firmware mode.
    ReadPort       = CMD_TYPE_READ_PORT,        ///< Read output port.
    WritePort      = CMD_TYPE_WRITE_PORT,       ///< Write input port.
    GetInputSpeed  = CMD_TYPE_GET_INPUT_SPEED,  ///< Get input fan speed.
    GetInputPWM    = CMD_TYPE_GET_INPUT_PWM,    ///< Get input pwm.
    SetOutputSpeed = CMD_TYPE_SET_OUTPUT_SPEED, ///< Set output speed.
    SetOutputPWM   = CMD_TYPE_SET_OUTPUT_PWM,   ///< Set output pwm.
    ReacConfig     = CMD_TYPE_READ_CONFIG,      ///< Read config.
    WriteConfig    = CMD_TYPE_WRITE_CONFIG,     ///< Write config.
    ReadClock      = CMD_TYPE_READ_CLOCK        ///< Read clock.
};

/**
 * @brief       Firmware mode.
 */
enum class FirmwareMode : uint8_t {
    Normal = FIRMWARE_MODE_NORMAL, ///< Normal mode.
    Manual = FIRMWARE_MODE_MANUAL, ///< Manual mode.
    Test   = FIRMWARE_MODE_TEST    ///< Test mode.
};

/**
 * @brief   Readable port.
 */
enum class ReadablePort : uint8_t {
    PWMInput   = PORT_READ_PWM_INPUT,  ///< PWM input.
    SpeedInput = PORT_READ_SPEED_INPUT ///< Speed input.
};

/**
 * @brief   Writable port.
 */
enum class WritablePort : uint8_t {
    SpeedOutput = PORT_WRITE_SPEED_OUTPUT, ///< Speed output.
    PWMOutput   = PORT_WRITE_PWM_OUTPUT    ///< PWM output.
};

/**
 * @brief   Reply type.
 */
enum class ReplyType : uint8_t {
    Failed  = REPLY_TYPE_FAILED, ///< Command failed.
    Success = REPLY_TYPE_SUCCESS ///< Success.
};

#endif

/**
 * @brief   Firmware config.
 */
struct FirmwareConfig {
    uint8_t pwmMap[10]; ///< 10% a stage, 0-100.
    struct {
        uint16_t source; ///< Source speed(HZ).
        uint16_t dest;   ///< Dest speed(HZ).
    } speedMap[10];      ///< 10% a stage, 0-100.
};

/**
 * @brief       Command header.
 */
struct CMDHeader {
    uint8_t cmdBegin; ///< Value = CMD_BEGIN.
#if defined __cplusplus
    CMDType cmdType; ///< Command type.
#else
    uint8_t cmdType;   ///< Command type.
#endif
};

/**
 * @brief       Command GetMode.
 */
struct CMDGetMode {
    struct CMDHeader header; ///< Command header.
};

/**
 * @brief       Command SetMode.
 */
struct CMDSetMode {
    struct CMDHeader header; ///< Command header.
#if defined __cplusplus
    FirmwareMode mode; ///< Mode to set.
#else
    uint8_t mode;      ///< Mode to set.
#endif
};

/**
 * @brief       Command ReadPort.
 */
struct CMDReadPort {
    struct CMDHeader header; ///< Command header.
#if defined __cplusplus
    ReadablePort port; ///< Port to read.
#else
    uint8_t port;      ///< Port to read.
#endif
};

/**
 * @brief       Command WritePort.
 */
struct CMDWritePort {
    struct CMDHeader header; ///< Command header.
#if defined __cplusplus
    WritablePort port; ///< Port to write.
#else
    uint8_t port;      ///< Port to write.
#endif
    uint8_t value; ///< Value(0 or 1).
};

/**
 * @brief       Command GetInputSpeed.
 */
struct CMDGetInputSpeed {
    struct CMDHeader header; ///< Command header.
};

/**
 * @brief       Command GetInputPWM.
 */
struct CMDGetInputPWM {
    struct CMDHeader header; ///< Command header.
};

/**
 * @brief       Command SetOutputSpeed.
 */
struct CMDSetOutputSpeed {
    struct CMDHeader header; ///< Command header.
    uint16_t         speed;  ///< Speed(Hz).
};

/**
 * @brief       Command SetOutputPWM.
 */
struct CMDSetOutputPWM {
    struct CMDHeader header;    ///< Command header.
    uint8_t          dutyCycle; ///< Duty cycle, 1-100.
};

/**
 * @brief       Command ReadConfig.
 */
struct CMDReadConfig {
    struct CMDHeader header; ///< Command header.
};

/**
 * @brief       Command WriteConfig.
 */
struct CMDWriteConfig {
    struct CMDHeader      header; ///< Command header.
    struct FirmwareConfig config; ///< Config.
};

/**
 * @brief       Command ReadClock.
 */
struct CMDReadClock {
    struct CMDHeader header; ///< Command header.
};

/**
 * @brief       Reply Header.
 */
struct ReplyHeader {
#if defined __cplusplus
    ReplyType replyType; ///< Reply type.
#else
    uint8_t replyType; ///< Reply type.
#endif
};

/**
 * @brief       Reply Failed.
 */
struct ReplyFailed {
    struct ReplyHeader header; ///< Header.
};

/**
 * @brief       Reply GetMode.
 */
struct ReplyGetMode {
    struct ReplyHeader header; ///< Header.
#if defined __cplusplus
    FirmwareMode mode; ///< Current mode.
#else
    uint8_t mode;      ///< Current mode.
#endif
};

/**
 * @brief       Reply SetMode.
 */
struct ReplySetMode {
    struct ReplyHeader header; ///< Header.
};

/**
 * @brief       Reply ReadPort.
 */
struct ReplyReadPort {
    struct ReplyHeader header; ///< Header.
    uint8_t            value;  ///< Value(0 or 1).
};

/**
 * @brief       Reply WritePort.
 */
struct ReplyWritePort {
    struct ReplyHeader header; ///< Header.
};

/**
 * @brief       Reply GetInputSpeed.
 */
struct ReplyGetInputSpeed {
    struct ReplyHeader header; ///< Header.
    uint16_t           speed;  ///< Speed(HZ).
};

/**
 * @brief       Reply GetInputPWM.
 */
struct ReplyGetInputPWM {
    struct ReplyHeader header;    ///< Header.
    uint8_t            dutyCycle; ///< Duty cycle, 1-100.
};

/**
 * @brief       Reply SetOutputSpeed.
 */
struct ReplySetOutputSpeed {
    struct ReplyHeader header; ///< Header.
};

/**
 * @brief       Reply SetOutputPWM.
 */
struct ReplySetOutputPWM {
    struct ReplyHeader header; ///< Header.
};

/**
 * @brief       Reply ReadConfig.
 */
struct ReplyReadConfig {
    struct ReplyHeader    header; ///< Header.
    struct FirmwareConfig config; ///< Config.
};

/**
 * @brief       Reply WriteConfig.
 */
struct ReplyWriteConfig {
    struct ReplyHeader header; ///< Header.
};

/**
 * @brief       Reply ReadClock.
 */
struct ReplyReadClock {
    struct ReplyHeader header;   ///< Header.
    uint32_t           bootTime; ///< Boot time.
};

#if ! defined BUILD_FIRMWARE
    #pragma pack(pop)
#endif
