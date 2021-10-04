#include <QtCore/QTimer>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QVBoxLayout>

#include <view/test_mode_operation_widget.h>

/**
 * @brief       Constructor.
 */
TestModeWidget::TestModeWidget(QWidget *        parent,
                               BoardController *boardController,
                               StringTable *    stringTable) :
    QWidget(parent),
    m_boardController(boardController), m_stringTable(stringTable),
    m_updateSpeedInput(false), m_updatePWMInput(false)
{
    QVBoxLayout *layout = new QVBoxLayout();
    this->setLayout(layout);

    // Read.
    QGridLayout *readLayout = new QGridLayout();
    layout->addLayout(readLayout);

    m_btnStartStopReadSpeedInput = new QPushButton(
        m_stringTable->getString("STR_BTN_START_READING_SPEED_INPUT_PORT"));
    readLayout->addWidget(m_btnStartStopReadSpeedInput, 0, 0);
    this->connect(m_btnStartStopReadSpeedInput, &QPushButton::clicked, this,
                  &TestModeWidget::onBtnStartReadSpeedInputClicked);

    QLabel *label
        = new QLabel(m_stringTable->getString("STR_LABEL_CURRENT_VALUE"));
    readLayout->addWidget(label, 0, 1);

    m_txtSpeedInputValue = new QLineEdit();
    readLayout->addWidget(m_txtSpeedInputValue, 0, 2);
    m_txtSpeedInputValue->setReadOnly(true);

    m_btnStartStopReadPWMInput = new QPushButton(
        m_stringTable->getString("STR_BTN_START_READING_PWM_INPUT_PORT"));
    readLayout->addWidget(m_btnStartStopReadPWMInput, 1, 0);
    this->connect(m_btnStartStopReadPWMInput, &QPushButton::clicked, this,
                  &TestModeWidget::onBtnStartReadPWMInputClicked);

    label = new QLabel(m_stringTable->getString("STR_LABEL_CURRENT_VALUE"));
    readLayout->addWidget(label, 1, 1);

    m_txtPWMInputValue = new QLineEdit();
    readLayout->addWidget(m_txtPWMInputValue, 1, 2);
    m_txtPWMInputValue->setReadOnly(true);

    readLayout->setColumnStretch(0, 0);
    readLayout->setColumnStretch(1, 0);
    readLayout->setColumnStretch(2, 100);

    // Write.
    QGridLayout *writeLayout = new QGridLayout();
    layout->addLayout(writeLayout);

    label = new QLabel(
        m_stringTable->getString("STR_LABEL_WRITE_SPEED_OUTPUT_PORT"));
    writeLayout->addWidget(label, 0, 0);

    m_btnSetSpeedOutputWrite0 = new QPushButton("0");
    writeLayout->addWidget(m_btnSetSpeedOutputWrite0, 0, 1);
    this->connect(m_btnSetSpeedOutputWrite0, &QPushButton::clicked, this,
                  &TestModeWidget::onBtnSetSpeedOutputWrite0);

    m_btnSetSpeedOutputWrite1 = new QPushButton("1");
    writeLayout->addWidget(m_btnSetSpeedOutputWrite1, 0, 2);
    this->connect(m_btnSetSpeedOutputWrite1, &QPushButton::clicked, this,
                  &TestModeWidget::onBtnSetSpeedOutputWrite1);

    label = new QLabel(
        m_stringTable->getString("STR_LABEL_WRITE_PWM_OUTPUT_PORT"));
    writeLayout->addWidget(label, 1, 0);

    m_btnSetPWMOutputWrite0 = new QPushButton("0");
    writeLayout->addWidget(m_btnSetPWMOutputWrite0, 1, 1);
    this->connect(m_btnSetPWMOutputWrite0, &QPushButton::clicked, this,
                  &TestModeWidget::onBtnSetPWMOutputWrite0);

    m_btnSetPWMOutputWrite1 = new QPushButton("1");
    writeLayout->addWidget(m_btnSetPWMOutputWrite1, 1, 2);
    this->connect(m_btnSetPWMOutputWrite1, &QPushButton::clicked, this,
                  &TestModeWidget::onBtnSetPWMOutputWrite1);

    writeLayout->setColumnStretch(0, 0);
    writeLayout->setColumnStretch(1, 0);
    writeLayout->setColumnStretch(2, 0);
    writeLayout->setColumnStretch(3, 100);

    m_updateTimer = new QTimer(this);
    this->connect(m_updateTimer, &QTimer::timeout, this,
                  &TestModeWidget::onTimer);
    m_updateTimer->setInterval(1000);
    m_updateTimer->setSingleShot(false);

    // Connect signals.
    this->connect(m_boardController, &BoardController::opened, this,
                  &TestModeWidget::onOpened, Qt::QueuedConnection);
    this->connect(m_boardController, &BoardController::closed, this,
                  &TestModeWidget::onClosed, Qt::QueuedConnection);
    this->connect(m_boardController, &BoardController::firmwareModeUpdated,
                  this, &TestModeWidget::onFirmwareModeUpdated,
                  Qt::QueuedConnection);
    this->connect(m_boardController, &BoardController::portRead, this,
                  &TestModeWidget::onPortRead, Qt::QueuedConnection);
    this->connect(this, &TestModeWidget::readPort, m_boardController,
                  &BoardController::readPort, Qt::QueuedConnection);
    this->connect(this, &TestModeWidget::writedPort, m_boardController,
                  &BoardController::writedPort, Qt::QueuedConnection);

    this->disable();
}

/**
 * @brief       Destructor.
 */
TestModeWidget::~TestModeWidget() {}

/**
 * @brief       Opened slots.
 */
void TestModeWidget::onOpened()
{
    m_txtSpeedInputValue->setText("");
    m_txtPWMInputValue->setText("");
}

/**
 * @brief       Closed slots.
 */
void TestModeWidget::onClosed()
{
    this->disable();
}

/**
 * @brief       Firmware mode updated.
 */
void TestModeWidget::onFirmwareModeUpdated(bool success, FirmwareMode mode)
{
    if (success && mode == FirmwareMode::Test) {
        this->enable();

    } else {
        this->disable();
    }
}

/**
 * @brief       On button start reading speed input clicked.
 */
void TestModeWidget::onBtnStartReadSpeedInputClicked()
{
    this->disconnect(m_btnStartStopReadSpeedInput);
    m_btnStartStopReadSpeedInput->setText(
        m_stringTable->getString("STR_BTN_STOP_READING_SPEED_INPUT_PORT{"));
    this->connect(m_btnStartStopReadSpeedInput, &QPushButton::clicked, this,
                  &TestModeWidget::onBtnStopReadSpeedInputClicked);
    m_updateSpeedInput = true;
}

/**
 * @brief       On button stop reading speed input clicked.
 */
void TestModeWidget::onBtnStopReadSpeedInputClicked()
{
    this->disconnect(m_btnStartStopReadSpeedInput);
    m_btnStartStopReadSpeedInput->setText(
        m_stringTable->getString("STR_BTN_START_READING_SPEED_INPUT_PORT{"));
    this->connect(m_btnStartStopReadSpeedInput, &QPushButton::clicked, this,
                  &TestModeWidget::onBtnStartReadSpeedInputClicked);
    m_updateSpeedInput = false;
}

/**
 * @brief       On button start reading PWM input clicked.
 */
void TestModeWidget::onBtnStartReadPWMInputClicked()
{
    this->disconnect(m_btnStartStopReadPWMInput);
    m_btnStartStopReadPWMInput->setText(
        m_stringTable->getString("STR_BTN_STOP_READING_PWM_INPUT_PORT{"));
    this->connect(m_btnStartStopReadPWMInput, &QPushButton::clicked, this,
                  &TestModeWidget::onBtnStopReadPWMInputClicked);
    m_updatePWMInput = true;
}

/**
 * @brief       On button stop reading PWM input clicked.
 */
void TestModeWidget::onBtnStopReadPWMInputClicked()
{
    this->disconnect(m_btnStartStopReadPWMInput);
    m_btnStartStopReadPWMInput->setText(
        m_stringTable->getString("STR_BTN_START_READING_PWM_INPUT_PORT{"));
    this->connect(m_btnStartStopReadPWMInput, &QPushButton::clicked, this,
                  &TestModeWidget::onBtnStartReadPWMInputClicked);
    m_updatePWMInput = false;
}

/**
 * @brief       On button set speed output port to 0 clicked.
 */
void TestModeWidget::onBtnSetSpeedOutputWrite0()
{
    emit this->writedPort(WritablePort::SpeedOutput, false);
}

/**
 * @brief       On button set speed output port to 1 clicked.
 */
void TestModeWidget::onBtnSetSpeedOutputWrite1()
{
    emit this->writedPort(WritablePort::SpeedOutput, true);
}

/**
 * @brief       On button set PWM output port to 0 clicked.
 */
void TestModeWidget::onBtnSetPWMOutputWrite0()
{
    emit this->writedPort(WritablePort::PWMOutput, false);
}

/**
 * @brief       On button set PWM output port to 1 clicked.
 */
void TestModeWidget::onBtnSetPWMOutputWrite1()
{
    emit this->writedPort(WritablePort::PWMOutput, true);
}

/**
 * @brief       Port has been read.
 */
void TestModeWidget::onPortRead(ReadablePort port, bool value)
{
    QLineEdit *edit = nullptr;
    switch (port) {
        case ReadablePort::SpeedInput:
            edit = m_txtSpeedInputValue;
            break;

        case ReadablePort::PWMInput:
            edit = m_txtPWMInputValue;
            break;

        default:
            return;
    }

    edit->setText(value ? "1" : "0");
}

/**
 * @brief       On timer.
 */
void TestModeWidget::onTimer()
{
    if (m_updateSpeedInput) {
        emit this->readPort(ReadablePort::SpeedInput);
    }

    if (m_updatePWMInput) {
        emit this->readPort(ReadablePort::PWMInput);
    }
}

/**
 * @brief       Enable widget.
 */
void TestModeWidget::enable()
{
    m_btnStartStopReadSpeedInput->setEnabled(true);
    m_btnStartStopReadPWMInput->setEnabled(true);
    m_btnSetSpeedOutputWrite0->setEnabled(true);
    m_btnSetSpeedOutputWrite1->setEnabled(true);
    m_btnSetPWMOutputWrite0->setEnabled(true);
    m_btnSetPWMOutputWrite1->setEnabled(true);
    this->setVisible(true);
    m_updateTimer->start();
}

/**
 * @brief       Disable widget.
 */
void TestModeWidget::disable()
{
    m_btnStartStopReadSpeedInput->setEnabled(false);
    m_btnStartStopReadPWMInput->setEnabled(false);
    m_btnSetSpeedOutputWrite0->setEnabled(false);
    m_btnSetSpeedOutputWrite1->setEnabled(false);
    m_btnSetPWMOutputWrite0->setEnabled(false);
    m_btnSetPWMOutputWrite1->setEnabled(false);
    m_txtSpeedInputValue->setText("");
    m_txtPWMInputValue->setText("");
    this->setVisible(false);
    m_updateTimer->stop();
}
