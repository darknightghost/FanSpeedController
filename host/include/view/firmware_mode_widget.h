#pragma once

#include <QtWidgets/QComboBox>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>

#include <controller/board_controller.h>
#include <locale/string_table.h>

/**
 * @brief       Message widget.
 */
class FirmwareModeWidget : public QWidget {
    Q_OBJECT;

  private:
    BoardController *m_boardController; ///< Board controller.
    StringTable *    m_stringTable;     ///< String table.

    QComboBox *  m_comboFirmwareMode; ///< Combobox to select mode.
    QPushButton *m_btnSet;            ///< Button open/close.
    QLineEdit *  m_txtCurrentMode;    ///< Text to show current mode.

  public:
    /**
     * @brief       Constructor.
     *
     * @param[in]   parent              Parent widget.
     * @param[in]   boardController     Board controller.
     * @param[in]   stringTable         String table.
     */
    FirmwareModeWidget(QWidget *        parent,
                       BoardController *boardController,
                       StringTable *    stringTable);

    /**
     * @brief       Destructor.
     */
    virtual ~FirmwareModeWidget();

  signals:
    /**
     * @brief       Update firmware mode.
     */
    void updateFirmwareMode();

    /**
     * @brief       Set firmware mode.
     *
     * @param[in]   ode        Mode to set.
     */
    void setFirmwareMode(FirmwareMode mode);

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
    void onBtnSetClicked();

    /**
     * @brief       Firmware mode updated.
     *
     * @param[in]   mode    Firmware mode.
     */
    void onFirmwareModeUpdated(FirmwareMode mode);
};
