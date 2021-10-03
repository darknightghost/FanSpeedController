#pragma once

#include <chrono>

#include <QtCore/QDateTime>
#include <QtCore/QMetaEnum>
#include <QtCore/QThread>
#include <QtSerialPort/QSerialPort>

#include <command.h>

#include <locale/string_table.h>
#include <serial/serial.h>

/**
 * @brief       Board controller.
 */
class BoardController : public QThread {
    Q_OBJECT;

  public:
    Q_ENUM(FirmwareMode);
    Q_ENUM(ReadablePort);
    Q_ENUM(WritablePort);

  private:
    StringTable *m_stringTable; ///< String table.

    Serial m_serialPort; ///< Serial port.

  public:
    /**
     * @brief       Constructor.
     *
     * @param[in]   stringTable         String table.
     */
    BoardController(StringTable *stringTable);

    /**
     * @brief       Destructor.
     */
    virtual ~BoardController();

  signals:
    /**
     * @brief       Opened signal.
     */
    void opened();

    /**
     * @brief       Closed signal.
     */
    void closed();

    /**
     * @brief       Infomation signal.
     *
     * @param[in]   time        Current time.
     * @param[in]   message     Message.
     */
    void printInfo(QDateTime time, QString message);

    /**
     * @brief       Error signal.
     *
     * @param[in]   time        Current time.
     * @param[in]   message     Error message.
     */
    void printError(QDateTime time, QString message);

    /**
     * @brief       Firmware mode signal.
     *
     * @param[in]   success Success flag.
     * @param[in]   mode    Firmware mode.
     */
    void firmwareModeUpdated(bool success, FirmwareMode mode);

    /**
     * @brief       Firmware mode signal.
     *
     * @param[in]   speed   Speed(HZ).
     */
    void speedUpdated(quint16 speed);

    /**
     * @brief       Firmware mode signal.
     *
     * @param[in]   time    Boot time(microseconds).
     */
    void clockUpdated(quint32 time);

  public slots:
    /**
     * @brief       Open serial.
     *
     * @param[in]   name        Name of the port.
     */
    void open(QString name);

    /**
     * @brief       Close serial.
     */
    void close();

    /**
     * @brief       Update open status.
     */
    void updateOpenStatus();

    /**
     * @brief       Update firmware mode.
     */
    void updateFirmwareMode();

    /**
     * @brief       Set firmware mode.
     *
     * @param[in]   ode        Mode to set.
     */
    void setFirmwareMode(FirmwareMode mode);

    /**
     * @brief       Update fan speed.
     */
    void updateSpeed();

    /**
     * @brief       Update clock.
     */
    void updateClock();

  private:
    /**
     * @brief       Send command.
     *
     * @param[in]   data    Data to send.
     * @param[in]   size    Size of data.
     *
     * @return      Bytes sent, or -1 when failed.
     */
    qint64 sendCommand(const uint8_t *data, size_t size);

    /**
     * @brief       Receive reply.
     *
     * @param[out]  data    Data received.
     * @param[in]   size    Size to receive.
     *
     * @return      Bytes received, or -1 when failed.
     */
    qint64 receiveReply(uint8_t *data, size_t size);
};
