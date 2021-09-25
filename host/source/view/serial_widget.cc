#include <QtCore/QDebug>
#include <QtSerialPort/QSerialPortInfo>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QLabel>

#include <view/serial_widget.h>

/**
 * @brief       Constructor.
 */
SerialWidget::SerialWidget(QWidget *        parent,
                           BoardController *boardController,
                           StringTable *    stringTable) :
    QWidget(parent),
    m_boardController(boardController), m_stringTable(stringTable)
{
    // Initialize UI
    QGridLayout *layout = new QGridLayout();
    this->setLayout(layout);

    QLabel *label = new QLabel(m_stringTable->getString("STR_LABEL_SERIAL"));
    layout->addWidget(label, 0, 0);

    m_comboSerial = new QComboBox();
    layout->addWidget(m_comboSerial, 0, 1);
    m_comboSerial->setEditable(false);

    m_btnOpenClose = new QPushButton(m_stringTable->getString("STR_BTN_OPEN"));
    layout->addWidget(m_btnOpenClose, 0, 2);
    this->connect(m_btnOpenClose, &QPushButton::clicked, this,
                  &SerialWidget::onBtnOpenClicked);

    m_btnRefresh = new QPushButton(m_stringTable->getString("STR_BTN_REFRESH"));
    layout->addWidget(m_btnRefresh, 0, 3);
    this->connect(m_btnRefresh, &QPushButton::clicked, this,
                  &SerialWidget::onBtnRefreshClicked);
    this->onBtnRefreshClicked();

    layout->setColumnStretch(0, 0);
    layout->setColumnStretch(1, 100);
    layout->setColumnStretch(2, 0);
    layout->setColumnStretch(3, 0);

    // Connect signals.
    this->connect(m_boardController, &BoardController::opened, this,
                  &SerialWidget::onOpened);
    this->connect(m_boardController, &BoardController::closed, this,
                  &SerialWidget::onClosed);

    this->connect(this, &SerialWidget::open, m_boardController,
                  &BoardController::open, Qt::QueuedConnection);
    this->connect(this, &SerialWidget::close, m_boardController,
                  &BoardController::close, Qt::QueuedConnection);
}

/**
 * @brief       Destructor.
 */
SerialWidget::~SerialWidget() {}

/**
 * @brief       Opened slots.
 */
void SerialWidget::onOpened()
{
    m_btnOpenClose->setText(m_stringTable->getString("STR_BTN_CLOSE"));
    m_btnOpenClose->disconnect();
    this->connect(m_btnOpenClose, &QPushButton::clicked, this,
                  &SerialWidget::onBtnCloseClicked);
    m_btnOpenClose->setEnabled(true);
    m_btnRefresh->setEnabled(false);
}

/**
 * @brief       Closed slots.
 */
void SerialWidget::onClosed()
{
    m_btnOpenClose->setText(m_stringTable->getString("STR_BTN_OPEN"));
    m_btnOpenClose->disconnect();
    this->connect(m_btnOpenClose, &QPushButton::clicked, this,
                  &SerialWidget::onBtnOpenClicked);
    m_btnOpenClose->setEnabled(true);
    m_btnRefresh->setEnabled(true);
}

/**
 * @brief       On button open clicked.
 */
void SerialWidget::onBtnOpenClicked()
{
    m_btnOpenClose->setEnabled(false);
    m_btnRefresh->setEnabled(false);
    emit this->open(m_comboSerial->currentText());
}

/**
 * @brief       On button close clicked.
 */
void SerialWidget::onBtnCloseClicked()
{
    m_btnOpenClose->setEnabled(false);
    m_btnRefresh->setEnabled(false);
    emit this->close();
}

/**
 * @brief       On button refresh clicked.
 */
void SerialWidget::onBtnRefreshClicked()
{
    QString oldSelection = m_comboSerial->currentText();
    m_comboSerial->clear();

    // Search serial ports.
    for (auto &port : QSerialPortInfo::availablePorts()) {
        m_comboSerial->addItem(port.portName());
        qDebug() << "Serial port found :" << port.portName();
    }

    int index = m_comboSerial->findText(oldSelection);
    if (index < 0) {
        index = 0;
    }
    if (m_comboSerial->count() > 0) {
        m_comboSerial->setCurrentIndex(index);
        m_btnOpenClose->setEnabled(true);
    } else {
        m_btnOpenClose->setEnabled(false);
    }
}
