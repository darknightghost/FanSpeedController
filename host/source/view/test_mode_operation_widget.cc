#include <QtWidgets/QGridLayout>

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
    QGridLayout *layout = new QGridLayout();
    this->setLayout(layout);

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
