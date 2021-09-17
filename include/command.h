#pragma once

#if defined __cplusplus
    #include <cstdint>
#else
    #include <stdint.h>
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

/// Fan test command, manual mode only.
#define CMD_TYPE_READ_SPEED ((uint8_t)0x20)
#define CMD_TYPE_SET_PWM    ((uint8_t)0x21)

/// Setting command.
#define CMD_TYPE_READ_SPEED_MAP  ((uint8_t)0x30)
#define CMD_TYPE_WRITE_SPEED_MAP ((uint8_t)0x31)

#define REPLY_TYPE_FAILED  ((uint8_t)0x00) ///< Command failed.
#define REPLY_TYPE_SUCCESS ((uint8_t)0x01) ///< Success.

#if defined __cplusplus
/**
 * @brief       Command type.
 */
enum class CMDType : uint8_t {
    GetMode       = CMD_TYPE_GET_MODE,       ///< Get firmware mode.
    SetMode       = CMD_TYPE_SET_MODE,       ///< Set firmware mode.
    ReadPort      = CMD_TYPE_READ_PORT,      ///< Read output port.
    WritePort     = CMD_TYPE_WRITE_PORT,     ///< Write input port.
    ReadSpeed     = CMD_TYPE_READ_SPEED,     ///< Read fan speed.
    SetPWM        = CMD_TYPE_SET_PWM,        ///< Set output pwm.
    ReadSpeedMap  = CMD_TYPE_READ_SPEED_MAP, ///< Read current speed map.
    WriteSpeedMap = CMD_TYPE_WRITE_SPEED_MAP ///< Write current speed map.
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
 * @brief       Command ReadSpeed.
 */
struct CMDReadSpeed {
    struct CMDHeader header; ///< Command header.
};

/**
 * @brief       Command SetPWM.
 */
struct CMDSetPWM {
    struct CMDHeader header;    ///< Command header.
    uint8_t          dutyCycle; ///< Duty cycle, 1-100.
};

/**
 * @brief       Command ReadSpeedMap.
 */
struct CMDReadSpeedMap {
    struct CMDHeader header; ///< Command header.
};

/**
 * @brief       Command WriteSpeedMap.
 */
struct CMDWriteSpeedMap {
    struct CMDHeader header;    ///< Command header.
    uint8_t          rates[20]; ///< 5% a stage, 0-100.
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
 * @brief       Reply ReadSpeed.
 */
struct ReplyReadSpeed {
    struct ReplyHeader header;    ///< Header.
    uint8_t            speedLow;  ///< 0-7 bit of the speed(rpm).
    uint8_t            speedHigh; ///< 8-15 bit of the speed(rpm).
};

/**
 * @brief       Reply SetPWM.
 */
struct ReplySetPWM {
    struct ReplyHeader header; ///< Header.
};

/**
 * @brief       Reply ReadSpeedMap.
 */
struct ReplyReadSpeedMap {
    struct ReplyHeader header;    ///< Header.
    uint8_t            rates[20]; ///< 5% a stage, 0-100.
};

/**
 * @brief       Reply WriteSpeedMap.
 */
struct ReplyWriteSpeedMap {
    struct ReplyHeader header; ///< Header.
};

#if ! defined BUILD_FIRMWARE
    #pragma pack(pop)
#endif