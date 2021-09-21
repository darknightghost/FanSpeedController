#pragma once

#include <QtWidgets/QComboBox>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>

#include <controller/board_controller.h>
#include <locale/string_table.h>

/**
 * @brief       Message widget.
 */
class SerialWidget : public QWidget {
    Q_OBJECT;

  private:
    BoardController *m_boardController; ///< Board controller.

    QComboBox *  m_comboSerial;  ///< Combobox to select serial.
    QPushButton *m_btnOpenClose; ///< Button open/close.
    QPushButton *m_btnRefresh;   ///< Button refresh.

    StringTable *m_stringTable; ///< String table.

  public:
    /**
     * @brief       Constructor.
     *
     * @param[in]   parent              Parent widget.
     * @param[in]   boardController     Board controller.
     * @param[in]   stringTable         String table.
     */
    SerialWidget(QWidget *        parent,
                 BoardController *boardController,
                 StringTable *    stringTable);

    /**
     * @brief       Destructor.
     */
    virtual ~SerialWidget();

  private slots:
    /**
     * @brief       Opened slots.
     */
    void onOpened();

    /**
     * @brief       Closed slots.
     */
    void onClosed();

    /**
     * @brief       On button open clicked.
     */
    void onBtnOpenClicked();

    /**
     * @brief       On button close clicked.
     */
    void onBtnCloseClicked();

    /**
     * @brief       On button refresh clicked.
     */
    void onBtnRefreshClicked();
};
