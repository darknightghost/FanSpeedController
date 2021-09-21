#pragma once

#include <chrono>

#include <QtCore/QMetaEnum>
#include <QtCore/QThread>
#include <QtSerialPort/QSerialPort>

#include <command.h>

#include <locale/string_table.h>

/**
 * @brief       Board controller.
 */
class BoardController : public QThread {
    Q_OBJECT;

  public:
    using FirmwareMode = ::FirmwareMode;
    Q_ENUM(FirmwareMode);

    using ReadablePort = ::ReadablePort;
    Q_ENUM(ReadablePort);

    using WritablePort = ::WritablePort;
    Q_ENUM(WritablePort);

  private:
    StringTable *m_stringTable; ///< String table.

    QSerialPort *m_serialPort; ///< Serial port.

  public:
    /**
     * @brief       Constructor.
     *
     * @param[in]   parent              Parent
     * @param[in]   stringTable         String table.
     */
    BoardController(QObject *parent, StringTable *stringTable);

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
     * @param[in]   message     Message.
     */
    void printInfo(QString message);

    /**
     * @brief       Error signal.
     *
     * @param[in]   message     Error message.
     */
    void printError(QString message);

    /**
     * @brief       Firmware mode signal.
     *
     * @param[in]   mode    Firmware mode.
     */
    void firmwareModeUpdated(FirmwareMode mode);

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
