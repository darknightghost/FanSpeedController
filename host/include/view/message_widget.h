#pragma once

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
     * @brief       Print info message.
     *
     * @param[in]   message     Message.
     */
    void onPrintInfo(QString message);

    /**
     * @brief       Print error message.
     *
     * @param[in]   message     Message.
     */
    void onPrintError(QString message);
};
