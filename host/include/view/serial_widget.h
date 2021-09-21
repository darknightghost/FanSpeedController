#pragma once

#include <QtWidgets/QComboBox>
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
     */
    SerialWidget(QWidget *parent, BoardController *boardController);

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
};
