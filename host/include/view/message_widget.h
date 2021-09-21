#pragma once

#include <QtWidgets/QTextEdit>

/**
 * @brief       Message widget.
 */
class MessageWidget : public QWidget {
    Q_OBJECT;

  private:
    QTextEdit *m_textEdit; ///< Text edit.

  public:
    /**
     * @brief       Constructor.
     *
     * @param[in]   parent      Parent widget.
     */
    MessageWidget(QWidget *parent);

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
    void printInfo(QString message);

    /**
     * @brief       Print error message.
     *
     * @param[in]   message     Message.
     */
    void printError(QString message);
};
