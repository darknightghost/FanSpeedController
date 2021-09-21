#include <QtWidgets/QVBoxLayout>

#include <view/message_widget.h>

/**
 * @brief       Constructor.
 */
MessageWidget::MessageWidget(QWidget *parent) : QWidget(parent)
{
    QVBoxLayout *layout = new QVBoxLayout();
    this->setLayout(layout);

    m_textEdit = new QTextEdit(this);
    layout->addWidget(m_textEdit);
    m_textEdit->setReadOnly(true);
    m_textEdit->document()->setMaximumBlockCount(8192);
}

/**
 * @brief       Destructor.
 */
MessageWidget::~MessageWidget() {}

/**
 * @brief       Print info message.
 */
void MessageWidget::printInfo(QString message)
{
    m_textEdit->append(QString("INFO :").arg(message));
    this->m_textEdit->moveCursor(QTextCursor::End);
}

/**
 * @brief       Print error message.
 */
void MessageWidget::printError(QString message)
{
    m_textEdit->append(QString("ERROR:").arg(message));
    this->m_textEdit->moveCursor(QTextCursor::End);
}
