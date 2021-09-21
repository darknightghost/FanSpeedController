#pragma once

#include <command.h>

#include <QtCore/QMetaEnum>
#include <QtCore/QThread>
#include <QtSerialPort/QSerialPort>

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
    QSerialPort m_serialPort; ///< Serial port.

  public:
    /**
     * @brief       Constructor.
     *
     * @param[in]   parent      Parent.
     */
    BoardController(QObject *parent);

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
    void info(QString message);

    /**
     * @brief       Error signal.
     *
     * @param[in]   message     Error message.
     */
    void error(QString message);

    /**
     * @brief       Firmware mode signal.
     *
     * @param[in]   mode    Firmware mode.
     */
    void firmwareModeUpdated(FirmwareMode mode);

  public slots:
    /**
     * @brief       Update open status.
     */
    void updateOpenStatus();

    /**
     * @brief       Update firmware mode.
     */
    void updatetFirmwareMode();

    /**
     * @brief       Set firmware mode.
     *
     * @param[in]   ode        Mode to set.
     */
    void setFirmwareMode(FirmwareMode mode);
};
