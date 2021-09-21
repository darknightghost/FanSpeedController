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
        m_serialPort->clear();
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
        m_serialPort->close();
        m_serialPort->clear();
        emit this->printInfo(
            m_stringTable->getString("STR_MESSAGE_PORT_CLOSED").arg(name));
    }
}

/**
 * @brief       Update open status.
 */
void BoardController::updateOpenStatus() {}

/**
 * @brief       Update firmware mode.
 */
void BoardController::updatetFirmwareMode()
{
    if (m_serialPort->isOpen()) {
        emit this->opened();
    } else {
        emit this->closed();
    }
}

/**
 * @brief       Set firmware mode.
 */
void BoardController::setFirmwareMode(FirmwareMode mode) {}
