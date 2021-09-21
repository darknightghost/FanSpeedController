#include <controller/board_controller.h>

/**
 * @brief       Constructor.
 */
BoardController::BoardController(QObject *parent) : QThread(parent) {}

/**
 * @brief       Destructor.
 */
BoardController::~BoardController() {}

/**
 * @brief       Update open status.
 */
void BoardController::updateOpenStatus() {}

/**
 * @brief       Update firmware mode.
 */
void BoardController::updatetFirmwareMode() {}

/**
 * @brief       Set firmware mode.
 */
void BoardController::setFirmwareMode(FirmwareMode mode) {}
