#pragma once

#include <QtCore/QDateTime>
#include <QtWidgets/QTextEdit>

#include <locale/string_table.h>

/**
 * @brief       Message widget.
 */
class MessageWidget : public QWidget {
    Q_OBJECT;

  private:
    StringTable *m_stringTable; ///< String table.

    QTextEdit *m_textEdit; ///< Text edit.

  public:
    /**
     * @brief       Constructor.
     *
     * @param[in]   parent              Parent widget.
     * @param[in]   stringTable         String table.
     */
    MessageWidget(QWidget *parent, StringTable *stringTable);

    /**
     * @brief       Destructor.
     */
    virtual ~MessageWidget();

  public slots:
    /**
     * @brief       On button clear clicked.
     */
    void onBtnClearClicked();

    /**
     * @brief       Print info message.
     *
     * @param[in]   time        Current time.
     * @param[in]   message     Message.
     */
    void onPrintInfo(QDateTime time, QString message);

    /**
     * @brief       Print error message.
     *
     * @param[in]   time        Current time.
     * @param[in]   message     Message.
     */
    void onPrintError(QDateTime time, QString message);
};
