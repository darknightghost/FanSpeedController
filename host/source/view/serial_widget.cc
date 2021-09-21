#include <view/serial_widget.h>

/**
 * @brief       Constructor.
 */
SerialWidget::SerialWidget(QWidget *parent, BoardController *boardController) :
    QWidget(parent), m_boardController(boardController)
{}

/**
 * @brief       Destructor.
 */
SerialWidget::~SerialWidget() {}

/**
 * @brief       Opened slots.
 */
void SerialWidget::onOpened() {}

/**
 * @brief       Closed slots.
 */
void SerialWidget::onClosed() {}
