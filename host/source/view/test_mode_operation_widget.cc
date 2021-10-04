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
    m_btnStartStopReadSpeedInput->setEnabled(false);

    QLabel *label
        = new QLabel(m_stringTable->getString("STR_LABEL_CURRENT_VALUE"));
    readLayout->addWidget(label, 0, 1);

    m_txtSpeedInputValue = new QLineEdit();
    readLayout->addWidget(m_txtSpeedInputValue, 0, 2);

    m_btnStartStopReadPWMInput = new QPushButton(
        m_stringTable->getString("STR_BTN_START_READING_PWM_INPUT_PORT"));
    readLayout->addWidget(m_btnStartStopReadPWMInput, 1, 0);
    m_btnStartStopReadPWMInput->setEnabled(false);

    label = new QLabel(m_stringTable->getString("STR_LABEL_CURRENT_VALUE"));
    readLayout->addWidget(label, 1, 1);

    m_txtPWMInputValue = new QLineEdit();
    readLayout->addWidget(m_txtPWMInputValue, 1, 2);

    // Connect signals.
    this->connect(m_boardController, &BoardController::opened, this,
                  &TestModeWidget::onOpened, Qt::QueuedConnection);
    this->connect(m_boardController, &BoardController::closed, this,
                  &TestModeWidget::onClosed, Qt::QueuedConnection);
    this->connect(m_boardController, &BoardController::firmwareModeUpdated,
                  this, &TestModeWidget::onFirmwareModeUpdated,
                  Qt::QueuedConnection);

    this->setVisible(false);
}

/**
 * @brief       Destructor.
 */
TestModeWidget::~TestModeWidget() {}

/**
 * @brief       Opened slots.
 */
void TestModeWidget::onOpened() {}

/**
 * @brief       Closed slots.
 */
void TestModeWidget::onClosed()
{
    this->setVisible(false);
}

/**
 * @brief       Firmware mode updated.
 */
void TestModeWidget::onFirmwareModeUpdated(bool success, FirmwareMode mode)
{
    if (success && mode == FirmwareMode::Test) {
        this->setVisible(true);
    } else {
        this->setVisible(false);
    }
}
