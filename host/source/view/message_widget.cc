#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QVBoxLayout>

#include <view/message_widget.h>

/**
 * @brief       Constructor.
 */
MessageWidget::MessageWidget(QWidget *parent, StringTable *stringTable) :
    QWidget(parent), m_stringTable(stringTable)
{
    QVBoxLayout *layout = new QVBoxLayout();
    this->setLayout(layout);

    QHBoxLayout *btnLayout = new QHBoxLayout();
    layout->addLayout(btnLayout);
    QPushButton *button
        = new QPushButton(m_stringTable->getString("STR_BTN_CLEAR"));
    btnLayout->addWidget(button);
    btnLayout->addStretch();
    this->connect(button, &QPushButton::clicked, this,
                  &MessageWidget::onBtnClearClicked);

    m_textEdit = new QTextEdit(this);
    layout->addWidget(m_textEdit);
    m_textEdit->setReadOnly(true);
    m_textEdit->document()->setMaximumBlockCount(16 * 1024);
}

/**
 * @brief       Destructor.
 */
MessageWidget::~MessageWidget() {}

/**
 * @brief       On button clear clicked.
 */
void MessageWidget::onBtnClearClicked()
{
    m_textEdit->setText("");
    this->m_textEdit->moveCursor(QTextCursor::End);
}

/**
 * @brief       Print info message.
 */
void MessageWidget::onPrintInfo(QDateTime time, QString message)
{
    m_textEdit->append(m_stringTable->getString("STR_MESSAGE_INFO")
                           .arg(time.toString("yyyy-MM-dd hh:mm:ss.zzz"))
                           .arg(message));
    this->m_textEdit->moveCursor(QTextCursor::End);
}

/**
 * @brief       Print error message.
 */
void MessageWidget::onPrintError(QDateTime time, QString message)
{
    m_textEdit->append(m_stringTable->getString("STR_MESSAGE_ERROR")
                           .arg(time.toString("yyyy-MM-dd hh:mm:ss.zzz"))
                           .arg(message));
    this->m_textEdit->moveCursor(QTextCursor::End);
}
