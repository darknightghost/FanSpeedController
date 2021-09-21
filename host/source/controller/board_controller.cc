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
 * @brief       Get firmware mode.
 */
void BoardController::getMode(FirmwareMode *mode) {}

/**
 * @brief       Set firmware mode.
 */
void BoardController::SetMode(FirmwareMode mode) {}
