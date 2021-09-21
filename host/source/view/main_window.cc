#include <QtWidgets/QVBoxLayout>

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

    QVBoxLayout *layout = new QVBoxLayout();
    this->setLayout(layout);

    m_serialWidget = new SerialWidget(this, m_boardController, m_stringTable);
    layout->addWidget(m_serialWidget);

    m_messageWidget = new MessageWidget(this, m_stringTable);
    layout->addWidget(m_messageWidget);
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
