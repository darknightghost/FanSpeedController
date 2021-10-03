#include <chrono>

#include <QtCore/QDebug>
#include <QtCore/QMetaType>

#include <controller/board_controller.h>
#include <utils/utils.h>

/**
 * @brief       Constructor.
 */
BoardController::BoardController(StringTable *stringTable) :
    QThread(nullptr), m_stringTable(stringTable)
{
    qRegisterMetaType<FirmwareMode>("FirmwareMode");
    qRegisterMetaType<ReadablePort>("ReadablePort");
    qRegisterMetaType<WritablePort>("WritablePort");
    this->moveToThread(this);
}

/**
 * @brief       Destructor.
 */
BoardController::~BoardController() {}

/**
 * @brief       Open serial.
 */
void BoardController::open(QString name)
{
    if (m_serialPort.isOpened()) {
        m_serialPort.close();
    }

    // Open port.
    if (m_serialPort.open(name)) {
        qDebug() << "Port" << name << "opened.";
        emit this->printInfo(
            QDateTime::currentDateTime(),
            m_stringTable->getString("STR_MESSAGE_PORT_OPENED").arg(name));
        this->updateOpenStatus();
    } else {
        qDebug() << "Failed to open port" << name << ".";
        emit this->printError(
            QDateTime::currentDateTime(),
            m_stringTable->getString("STR_MESSAGE_PORT_OPEN_FAILED").arg(name));
        m_serialPort.close();
        this->updateOpenStatus();
    }
}

/**
 * @brief       Close serial.
 */
void BoardController::close()
{
    if (m_serialPort.isOpened()) {
        QString name = m_serialPort.name();
        m_serialPort.clearRead();
        m_serialPort.close();
        emit this->printInfo(
            QDateTime::currentDateTime(),
            m_stringTable->getString("STR_MESSAGE_PORT_CLOSED").arg(name));
        this->updateOpenStatus();
    }
}

/**
 * @brief       Update open status.
 */
void BoardController::updateOpenStatus()
{
    if (m_serialPort.isOpened()) {
        emit this->opened();
    } else {
        emit this->closed();
    }
}

/**
 * @brief       Update firmware mode.
 */
void BoardController::updateFirmwareMode()
{
    if (! m_serialPort.isOpened()) {
        emit this->printError(
            QDateTime::currentDateTime(),
            m_stringTable->getString("STR_MESSAGE_OPERATION_FAILED"));
        return;
    }
    m_serialPort.clearRead();

    // Send command.
    CMDGetMode command;
    command.header.cmdBegin = CMD_BEGIN;
    command.header.cmdType  = CMDType::GetMode;

    if (this->sendCommand(reinterpret_cast<const uint8_t *>(&command),
                          sizeof(command))
        < 0) {
        emit this->printError(
            QDateTime::currentDateTime(),
            m_stringTable->getString("STR_MESSAGE_COMMAND_SEND_FAILED"));
        emit this->printError(
            QDateTime::currentDateTime(),
            m_stringTable->getString("STR_MESSAGE_OPERATION_FAILED"));
        return;
    }

    emit this->printInfo(
        QDateTime::currentDateTime(),
        m_stringTable->getString("STR_MESSAGE_COMMAND_SEND")
            .arg(QString::fromUtf8(
                QByteArray(reinterpret_cast<char *>(&command), sizeof(command))
                    .toHex(' ')
                    .toUpper())));

    // Get reply.
    ReplyGetMode reply;
    uint8_t *    pReply = reinterpret_cast<uint8_t *>(&reply);

    // Receive first byte.
    if (this->receiveReply(pReply, sizeof(uint8_t)) < 0) {
        emit this->printError(
            QDateTime::currentDateTime(),
            m_stringTable->getString("STR_MESSAGE_REPLY_RECV_FAILED"));
        emit this->printError(
            QDateTime::currentDateTime(),
            m_stringTable->getString("STR_MESSAGE_OPERATION_FAILED"));
        return;
    }
    switch (reply.header.replyType) {
        case ReplyType::Success:
            break;

        case ReplyType::Failed:
            emit this->printInfo(
                QDateTime::currentDateTime(),
                m_stringTable->getString("STR_MESSAGE_REPLY")
                    .arg(QString::fromUtf8(
                        QByteArray(reinterpret_cast<char *>(pReply),
                                   sizeof(uint8_t))
                            .toHex(' ')
                            .toUpper())));
            emit this->printError(
                QDateTime::currentDateTime(),
                m_stringTable->getString("STR_MESSAGE_OPERATION_FAILED"));
            return;

        default:
            emit this->printError(
                QDateTime::currentDateTime(),
                m_stringTable->getString("STR_MESSAGE_REPLY_PARSE_ERROR"));
            emit this->printError(
                QDateTime::currentDateTime(),
                m_stringTable->getString("STR_MESSAGE_OPERATION_FAILED"));
            return;
    }

    // Receive remaining data,
    if (this->receiveReply(pReply + 1, sizeof(reply) - sizeof(uint8_t)) < 0) {
        emit this->printError(
            QDateTime::currentDateTime(),
            m_stringTable->getString("STR_MESSAGE_REPLY_RECV_FAILED"));
        emit this->printError(
            QDateTime::currentDateTime(),
            m_stringTable->getString("STR_MESSAGE_OPERATION_FAILED"));
        return;
    }

    emit this->printInfo(
        QDateTime::currentDateTime(),
        m_stringTable->getString("STR_MESSAGE_REPLY")
            .arg(QString::fromUtf8(
                QByteArray(reinterpret_cast<char *>(&reply), sizeof(reply))
                    .toHex(' ')
                    .toUpper())));
    // Parse type.
    switch (reply.mode) {
        case FirmwareMode::Normal:
        case FirmwareMode::Manual:
        case FirmwareMode::Test:
            emit this->printInfo(
                QDateTime::currentDateTime(),
                m_stringTable->getString("STR_MESSAGE_OPERATION_SUCCEED"));
            emit this->firmwareModeUpdated(reply.mode);
            return;

        default:
            emit this->printError(
                QDateTime::currentDateTime(),
                m_stringTable->getString("STR_MESSAGE_REPLY_PARSE_ERROR"));
            emit this->printError(
                QDateTime::currentDateTime(),
                m_stringTable->getString("STR_MESSAGE_OPERATION_FAILED"));
            return;
    }
}

/**
 * @brief       Set firmware mode.
 */
void BoardController::setFirmwareMode(FirmwareMode mode)
{
    if (! m_serialPort.isOpened()) {
        emit this->printError(
            QDateTime::currentDateTime(),
            m_stringTable->getString("STR_MESSAGE_OPERATION_FAILED"));
        return;
    }
    m_serialPort.clearRead();

    // Send command.
    CMDSetMode command;
    command.header.cmdBegin = CMD_BEGIN;
    command.header.cmdType  = CMDType::SetMode;
    command.mode            = mode;

    if (this->sendCommand(reinterpret_cast<const uint8_t *>(&command),
                          sizeof(command))
        < 0) {
        emit this->printError(
            QDateTime::currentDateTime(),
            m_stringTable->getString("STR_MESSAGE_COMMAND_SEND_FAILED"));
        emit this->printError(
            QDateTime::currentDateTime(),
            m_stringTable->getString("STR_MESSAGE_OPERATION_FAILED"));
        return;
    }

    emit this->printInfo(
        QDateTime::currentDateTime(),
        m_stringTable->getString("STR_MESSAGE_COMMAND_SEND")
            .arg(QString::fromUtf8(
                QByteArray(reinterpret_cast<char *>(&command), sizeof(command))
                    .toHex(' ')
                    .toUpper())));

    // Get reply.
    ReplySetMode reply;
    uint8_t *    pReply = reinterpret_cast<uint8_t *>(&reply);

    // Receive first byte.
    if (this->receiveReply(pReply, sizeof(reply)) < 0) {
        emit this->printError(
            QDateTime::currentDateTime(),
            m_stringTable->getString("STR_MESSAGE_REPLY_RECV_FAILED"));
        emit this->printError(
            QDateTime::currentDateTime(),
            m_stringTable->getString("STR_MESSAGE_OPERATION_FAILED"));
        return;
    }

    emit this->printInfo(
        QDateTime::currentDateTime(),
        m_stringTable->getString("STR_MESSAGE_REPLY")
            .arg(QString::fromUtf8(
                QByteArray(reinterpret_cast<char *>(&reply), sizeof(reply))
                    .toHex(' ')
                    .toUpper())));

    switch (reply.header.replyType) {
        case ReplyType::Success:
            emit this->printInfo(
                QDateTime::currentDateTime(),
                m_stringTable->getString("STR_MESSAGE_OPERATION_SUCCEED"));
            break;

        case ReplyType::Failed:
            emit this->printError(
                QDateTime::currentDateTime(),
                m_stringTable->getString("STR_MESSAGE_OPERATION_FAILED"));
            return;

        default:
            emit this->printError(
                QDateTime::currentDateTime(),
                m_stringTable->getString("STR_MESSAGE_REPLY_PARSE_ERROR"));
            emit this->printError(
                QDateTime::currentDateTime(),
                m_stringTable->getString("STR_MESSAGE_OPERATION_FAILED"));
            return;
    }
}

/**
 * @brief       Update fan speed.
 */
void BoardController::updateSpeed()
{
    if (! m_serialPort.isOpened()) {
        emit this->printError(
            QDateTime::currentDateTime(),
            m_stringTable->getString("STR_MESSAGE_OPERATION_FAILED"));
        return;
    }
    m_serialPort.clearRead();

    // Send command.
    CMDGetSpeed command;
    command.header.cmdBegin = CMD_BEGIN;
    command.header.cmdType  = CMDType::GetSpeed;

    if (this->sendCommand(reinterpret_cast<const uint8_t *>(&command),
                          sizeof(command))
        < 0) {
        emit this->printError(
            QDateTime::currentDateTime(),
            m_stringTable->getString("STR_MESSAGE_COMMAND_SEND_FAILED"));
        emit this->printError(
            QDateTime::currentDateTime(),
            m_stringTable->getString("STR_MESSAGE_OPERATION_FAILED"));
        return;
    }

    emit this->printInfo(
        QDateTime::currentDateTime(),
        m_stringTable->getString("STR_MESSAGE_COMMAND_SEND")
            .arg(QString::fromUtf8(
                QByteArray(reinterpret_cast<char *>(&command), sizeof(command))
                    .toHex(' ')
                    .toUpper())));

    // Get reply.
    ReplyGetSpeed reply;
    uint8_t *     pReply = reinterpret_cast<uint8_t *>(&reply);

    // Receive first byte.
    if (this->receiveReply(pReply, sizeof(uint8_t)) < 0) {
        emit this->printError(
            QDateTime::currentDateTime(),
            m_stringTable->getString("STR_MESSAGE_REPLY_RECV_FAILED"));
        emit this->printError(
            QDateTime::currentDateTime(),
            m_stringTable->getString("STR_MESSAGE_OPERATION_FAILED"));
        return;
    }
    switch (reply.header.replyType) {
        case ReplyType::Success:
            break;

        case ReplyType::Failed:
            emit this->printInfo(
                QDateTime::currentDateTime(),
                m_stringTable->getString("STR_MESSAGE_REPLY")
                    .arg(QString::fromUtf8(
                        QByteArray(reinterpret_cast<char *>(pReply),
                                   sizeof(uint8_t))
                            .toHex(' ')
                            .toUpper())));
            emit this->printError(
                QDateTime::currentDateTime(),
                m_stringTable->getString("STR_MESSAGE_OPERATION_FAILED"));
            return;

        default:
            emit this->printError(
                QDateTime::currentDateTime(),
                m_stringTable->getString("STR_MESSAGE_REPLY_PARSE_ERROR"));
            emit this->printError(
                QDateTime::currentDateTime(),
                m_stringTable->getString("STR_MESSAGE_OPERATION_FAILED"));
            return;
    }

    // Receive remaining data,
    if (this->receiveReply(pReply + 1, sizeof(reply) - sizeof(uint8_t)) < 0) {
        emit this->printError(
            QDateTime::currentDateTime(),
            m_stringTable->getString("STR_MESSAGE_REPLY_RECV_FAILED"));
        emit this->printError(
            QDateTime::currentDateTime(),
            m_stringTable->getString("STR_MESSAGE_OPERATION_FAILED"));
        return;
    }

    emit this->printInfo(
        QDateTime::currentDateTime(),
        m_stringTable->getString("STR_MESSAGE_REPLY")
            .arg(QString::fromUtf8(
                QByteArray(reinterpret_cast<char *>(&reply), sizeof(reply))
                    .toHex(' ')
                    .toUpper())));
    // Parse type.
    emit this->printInfo(
        QDateTime::currentDateTime(),
        m_stringTable->getString("STR_MESSAGE_OPERATION_SUCCEED"));
    emit this->speedUpdated(reply.speed);
    return;
}

/**
 * @brief       Update clock.
 */
void BoardController::updateClock()
{
    if (! m_serialPort.isOpened()) {
        emit this->printError(
            QDateTime::currentDateTime(),
            m_stringTable->getString("STR_MESSAGE_OPERATION_FAILED"));
        return;
    }
    m_serialPort.clearRead();

    // Send command.
    CMDReadClock command;
    command.header.cmdBegin = CMD_BEGIN;
    command.header.cmdType  = CMDType::ReadClock;

    if (this->sendCommand(reinterpret_cast<const uint8_t *>(&command),
                          sizeof(command))
        < 0) {
        emit this->printError(
            QDateTime::currentDateTime(),
            m_stringTable->getString("STR_MESSAGE_COMMAND_SEND_FAILED"));
        emit this->printError(
            QDateTime::currentDateTime(),
            m_stringTable->getString("STR_MESSAGE_OPERATION_FAILED"));
        return;
    }

    emit this->printInfo(
        QDateTime::currentDateTime(),
        m_stringTable->getString("STR_MESSAGE_COMMAND_SEND")
            .arg(QString::fromUtf8(
                QByteArray(reinterpret_cast<char *>(&command), sizeof(command))
                    .toHex(' ')
                    .toUpper())));

    // Get reply.
    ReplyReadClock reply;
    uint8_t *      pReply = reinterpret_cast<uint8_t *>(&reply);

    // Receive first byte.
    if (this->receiveReply(pReply, sizeof(uint8_t)) < 0) {
        emit this->printError(
            QDateTime::currentDateTime(),
            m_stringTable->getString("STR_MESSAGE_REPLY_RECV_FAILED"));
        emit this->printError(
            QDateTime::currentDateTime(),
            m_stringTable->getString("STR_MESSAGE_OPERATION_FAILED"));
        return;
    }
    switch (reply.header.replyType) {
        case ReplyType::Success:
            break;

        case ReplyType::Failed:
            emit this->printInfo(
                QDateTime::currentDateTime(),
                m_stringTable->getString("STR_MESSAGE_REPLY")
                    .arg(QString::fromUtf8(
                        QByteArray(reinterpret_cast<char *>(pReply),
                                   sizeof(uint8_t))
                            .toHex(' ')
                            .toUpper())));
            emit this->printError(
                QDateTime::currentDateTime(),
                m_stringTable->getString("STR_MESSAGE_OPERATION_FAILED"));
            return;

        default:
            emit this->printError(
                QDateTime::currentDateTime(),
                m_stringTable->getString("STR_MESSAGE_REPLY_PARSE_ERROR"));
            emit this->printError(
                QDateTime::currentDateTime(),
                m_stringTable->getString("STR_MESSAGE_OPERATION_FAILED"));
            return;
    }

    // Receive remaining data,
    if (this->receiveReply(pReply + 1, sizeof(reply) - sizeof(uint8_t)) < 0) {
        emit this->printError(
            QDateTime::currentDateTime(),
            m_stringTable->getString("STR_MESSAGE_REPLY_RECV_FAILED"));
        emit this->printError(
            QDateTime::currentDateTime(),
            m_stringTable->getString("STR_MESSAGE_OPERATION_FAILED"));
        return;
    }

    emit this->printInfo(
        QDateTime::currentDateTime(),
        m_stringTable->getString("STR_MESSAGE_REPLY")
            .arg(QString::fromUtf8(
                QByteArray(reinterpret_cast<char *>(&reply), sizeof(reply))
                    .toHex(' ')
                    .toUpper())));
    // Parse type.
    emit this->printInfo(
        QDateTime::currentDateTime(),
        m_stringTable->getString("STR_MESSAGE_OPERATION_SUCCEED"));
    emit this->clockUpdated(reply.bootTime);
    return;
}

/**
 * @brief       Send command.
 */
qint64 BoardController::sendCommand(const uint8_t *data, size_t size)
{
    // Send data.
    ssize_t ret = m_serialPort.write(data, size);
    if (ret < 0) {
        return -1;
    }

    return ret;
}

/**
 * @brief       Receive reply.
 */
qint64 BoardController::receiveReply(uint8_t *data, size_t size)
{
    // Receive data.
    ssize_t received
        = m_serialPort.read(data, size, ::std::chrono::milliseconds(1000));

    if (received < 0) {
        emit this->printInfo(
            QDateTime::currentDateTime(),
            m_stringTable->getString("STR_MESSAGE_REPLY_RECV_FAILED"));
        return -1;
    } else if (static_cast<size_t>(received) < size) {
        emit this->printInfo(
            QDateTime::currentDateTime(),
            m_stringTable->getString("STR_MESSAGE_REPLY_OUT_OF_TIME"));
        return -1;
    }

    return received;
}
