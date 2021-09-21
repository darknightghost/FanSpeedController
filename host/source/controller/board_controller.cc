#include <QtCore/QDebug>

#include <controller/board_controller.h>

/**
 * @brief       Constructor.
 */
BoardController::BoardController(QObject *parent, StringTable *stringTable) :
    QThread(parent), m_stringTable(stringTable),
    m_serialPort(new QSerialPort(this))
{}

/**
 * @brief       Destructor.
 */
BoardController::~BoardController() {}

/**
 * @brief       Open serial.
 */
void BoardController::open(QString name)
{
    if (m_serialPort->isOpen()) {
        m_serialPort->close();
    }

    // Open port.
    m_serialPort->setPortName(name);
    if (m_serialPort->open(QIODevice::ReadWrite)) {
        m_serialPort->setBaudRate(QSerialPort::Baud9600,
                                  QSerialPort::AllDirections);
        m_serialPort->setDataBits(QSerialPort::Data8);
        m_serialPort->setFlowControl(QSerialPort::NoFlowControl);
        m_serialPort->setParity(QSerialPort::NoParity);
        m_serialPort->setStopBits(QSerialPort::OneStop);

        qDebug() << "Port" << name << "opened.";
        emit this->printInfo(
            m_stringTable->getString("STR_MESSAGE_PORT_OPENED").arg(name));
        this->updateOpenStatus();
    } else {
        qDebug() << "Failed to open port" << name << ".";
        emit this->printError(
            m_stringTable->getString("STR_MESSAGE_PORT_OPEN_FAILED").arg(name));
        m_serialPort->clear();
        m_serialPort->close();
        this->updateOpenStatus();
    }
}

/**
 * @brief       Close serial.
 */
void BoardController::close()
{
    if (m_serialPort->isOpen()) {
        QString name = m_serialPort->portName();
        m_serialPort->clear();
        m_serialPort->close();
        emit this->printInfo(
            m_stringTable->getString("STR_MESSAGE_PORT_CLOSED").arg(name));
        this->updateOpenStatus();
    }
}

/**
 * @brief       Update open status.
 */
void BoardController::updateOpenStatus()
{
    if (m_serialPort->isOpen()) {
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
    if (! m_serialPort->isOpen()) {
        emit this->printError(
            m_stringTable->getString("STR_MESSAGE_OPERATION_FAILED"));
        return;
    }
    m_serialPort->clear();

    // Send command.
    CMDGetMode command;
    command.header.cmdBegin = CMD_BEGIN;
    command.header.cmdType  = CMDType::GetMode;

    if (this->sendCommand(reinterpret_cast<const uint8_t *>(&command),
                          sizeof(command))
        < 0) {
        emit this->printError(
            m_stringTable->getString("STR_MESSAGE_COMMAND_SEND_FAILED"));
        emit this->printError(
            m_stringTable->getString("STR_MESSAGE_OPERATION_FAILED"));
        return;
    }

    emit this->printInfo(
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
            m_stringTable->getString("STR_MESSAGE_REPLY_RECV_FAILED"));
        emit this->printError(
            m_stringTable->getString("STR_MESSAGE_OPERATION_FAILED"));
        return;
    }
    switch (reply.header.replyType) {
        case ReplyType::Success:
            break;

        case ReplyType::Failed:
            emit this->printInfo(
                m_stringTable->getString("STR_MESSAGE_REPLY")
                    .arg(QString::fromUtf8(
                        QByteArray(reinterpret_cast<char *>(pReply),
                                   sizeof(uint8_t))
                            .toHex(' ')
                            .toUpper())));
            emit this->printError(
                m_stringTable->getString("STR_MESSAGE_OPERATION_FAILED"));
            return;

        default:
            emit this->printError(
                m_stringTable->getString("STR_MESSAGE_REPLY_PARSE_ERROR"));
            emit this->printError(
                m_stringTable->getString("STR_MESSAGE_OPERATION_FAILED"));
            return;
    }

    // Receive remaining data,
    if (this->receiveReply(pReply + 1, sizeof(reply) - sizeof(uint8_t)) < 0) {
        emit this->printError(
            m_stringTable->getString("STR_MESSAGE_REPLY_RECV_FAILED"));
        emit this->printError(
            m_stringTable->getString("STR_MESSAGE_OPERATION_FAILED"));
        return;
    }

    emit this->printInfo(
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
            emit this->printError(
                m_stringTable->getString("STR_MESSAGE_OPERATION_SUCCEED"));
            emit this->firmwareModeUpdated(reply.mode);
            return;

        default:
            emit this->printError(
                m_stringTable->getString("STR_MESSAGE_REPLY_PARSE_ERROR"));
            emit this->printError(
                m_stringTable->getString("STR_MESSAGE_OPERATION_FAILED"));
            return;
    }
}

/**
 * @brief       Set firmware mode.
 */
void BoardController::setFirmwareMode(FirmwareMode mode)
{
    if (! m_serialPort->isOpen()) {
        emit this->printError(
            m_stringTable->getString("STR_MESSAGE_OPERATION_FAILED"));
        return;
    }
    m_serialPort->clear();

    // Send command.
    CMDSetMode command;
    command.header.cmdBegin = CMD_BEGIN;
    command.header.cmdType  = CMDType::SetMode;
    command.mode            = mode;

    if (this->sendCommand(reinterpret_cast<const uint8_t *>(&command),
                          sizeof(command))
        < 0) {
        emit this->printError(
            m_stringTable->getString("STR_MESSAGE_COMMAND_SEND_FAILED"));
        emit this->printError(
            m_stringTable->getString("STR_MESSAGE_OPERATION_FAILED"));
        return;
    }

    emit this->printInfo(
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
            m_stringTable->getString("STR_MESSAGE_REPLY_RECV_FAILED"));
        emit this->printError(
            m_stringTable->getString("STR_MESSAGE_OPERATION_FAILED"));
        return;
    }

    emit this->printInfo(
        m_stringTable->getString("STR_MESSAGE_REPLY")
            .arg(QString::fromUtf8(
                QByteArray(reinterpret_cast<char *>(&reply), sizeof(reply))
                    .toHex(' ')
                    .toUpper())));

    switch (reply.header.replyType) {
        case ReplyType::Success:
            emit this->printError(
                m_stringTable->getString("STR_MESSAGE_OPERATION_SUCCEED"));
            break;

        case ReplyType::Failed:
            emit this->printError(
                m_stringTable->getString("STR_MESSAGE_OPERATION_FAILED"));
            return;

        default:
            emit this->printError(
                m_stringTable->getString("STR_MESSAGE_REPLY_PARSE_ERROR"));
            emit this->printError(
                m_stringTable->getString("STR_MESSAGE_OPERATION_FAILED"));
            return;
    }
}

/**
 * @brief       Send command.
 */
qint64 BoardController::sendCommand(const uint8_t *data, size_t size)
{
    // Send data.
    qint64 ret = 0;
    while (ret < static_cast<qint64>(size)) {
        qint64 sizeWritten = m_serialPort->write(
            reinterpret_cast<const char *>(data + ret), size - ret);
        if (sizeWritten > 0) {
            ret += sizeWritten;

        } else {
            return -1;
        }
    }

    return ret;
}

/**
 * @brief       Receive reply.
 */
qint64 BoardController::receiveReply(uint8_t *data, size_t size)
{
    // Receive data.
    qint64 received = 0;
    while (received < static_cast<qint64>(size)) {
        if (! m_serialPort->waitForReadyRead(10000)) {
            emit this->printInfo(
                m_stringTable->getString("STR_MESSAGE_REPLY_OUT_OF_TIME"));
            return -1;
        }
        qint64 sizeRead = m_serialPort->read(
            reinterpret_cast<char *>(data + received), size - received);
        if (sizeRead < 0) {
            emit this->printInfo(
                m_stringTable->getString("STR_MESSAGE_REPLY_RECV_FAILED"));
            return -1;
        } else {
            received += sizeRead;
        }
    }
    m_serialPort->flush();

    return received;
}
