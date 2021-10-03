#include <view/manual_mode_operation_widget.h>

/**
 * @brief       Constructor.
 */
ManualModeWidget::ManualModeWidget(QWidget *        parent,
                                   BoardController *boardController,
                                   StringTable *    stringTable) :
    QWidget(parent),
    m_boardController(boardController), m_stringTable(stringTable)
{
    // Connect signals.
    this->connect(m_boardController, &BoardController::opened, this,
                  &ManualModeWidget::onOpened, Qt::QueuedConnection);
    this->connect(m_boardController, &BoardController::closed, this,
                  &ManualModeWidget::onClosed, Qt::QueuedConnection);
    this->connect(m_boardController, &BoardController::firmwareModeUpdated,
                  this, &ManualModeWidget::onFirmwareModeUpdated,
                  Qt::QueuedConnection);

    this->setVisible(false);
}

/**
 * @brief       Destructor.
 */
ManualModeWidget::~ManualModeWidget() {}

/**
 * @brief       Opened slots.
 */
void ManualModeWidget::onOpened() {}

/**
 * @brief       Closed slots.
 */
void ManualModeWidget::onClosed()
{
    this->setVisible(false);
}

/**
 * @brief       Firmware mode updated.
 */
void ManualModeWidget::onFirmwareModeUpdated(bool success, FirmwareMode mode)
{
    if (success && mode == FirmwareMode::Manual) {
        this->setVisible(true);
    } else {
        this->setVisible(false);
    }
}
