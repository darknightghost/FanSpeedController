#include <QtWidgets/QVBoxLayout>

#include <view/main_window.h>

/**
 * @brief       Constructor.
 */
MainWindow::MainWindow() :
    QWidget(nullptr), m_stringTable(new StringTable(this))
{
    this->setWindowTitle(m_stringTable->getString("STR_TITLE"));
    m_boardController = new BoardController(m_stringTable);
    m_boardController->start();

    QVBoxLayout *layout = new QVBoxLayout();
    this->setLayout(layout);

    m_serialWidget = new SerialWidget(this, m_boardController, m_stringTable);
    layout->addWidget(m_serialWidget);

    m_firmwareModeWidget
        = new FirmwareModeWidget(this, m_boardController, m_stringTable);
    layout->addWidget(m_firmwareModeWidget);

    m_genericOperationWidget
        = new GenericOperationWidget(this, m_boardController, m_stringTable);
    layout->addWidget(m_genericOperationWidget);

    m_manualModeWidget
        = new ManualModeWidget(this, m_boardController, m_stringTable);
    layout->addWidget(m_manualModeWidget);

    m_testModeWidget
        = new TestModeWidget(this, m_boardController, m_stringTable);
    layout->addWidget(m_testModeWidget);

    m_messageWidget = new MessageWidget(this, m_stringTable);
    layout->addWidget(m_messageWidget);
    m_boardController->connect(m_boardController, &BoardController::printInfo,
                               m_messageWidget, &MessageWidget::onPrintInfo);
    m_boardController->connect(m_boardController, &BoardController::printError,
                               m_messageWidget, &MessageWidget::onPrintError);
}

/**
 * @brief       Destructor.
 */
MainWindow::~MainWindow()
{
    delete m_boardController;
}

/**
 * @brief         Close event.
 */
void MainWindow::closeEvent(QCloseEvent *)
{
    m_boardController->quit();
}
