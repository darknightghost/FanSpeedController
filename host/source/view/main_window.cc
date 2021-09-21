#include <view/main_window.h>

/**
 * @brief       Constructor.
 */
MainWindow::MainWindow() :
    QWidget(nullptr), m_stringTable(new StringTable(this))
{
    this->setWindowTitle(m_stringTable->getString("STR_TITLE"));
    m_boardController = new BoardController(this);
    m_boardController->start();
}

/**
 * @brief       Destructor.
 */
MainWindow::~MainWindow() {}

/**
 * @brief         Close event.
 */
void MainWindow::closeEvent(QCloseEvent *)
{
    m_boardController->quit();
}
