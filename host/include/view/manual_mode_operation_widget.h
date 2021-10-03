#pragma once

#include <QtWidgets/QWidget>

#include <controller/board_controller.h>
#include <locale/string_table.h>

/**
 * @brief       Manual mode widget.
 */
class ManualModeWidget : public QWidget {
    Q_OBJECT;

  private:
    BoardController *m_boardController; ///< Board controller.
    StringTable *    m_stringTable;     ///< String table.

  public:
    /**
     * @brief       Constructor.
     *
     * @param[in]   parent              Parent widget.
     * @param[in]   boardController     Board controller.
     * @param[in]   stringTable         String table.
     */
    ManualModeWidget(QWidget *        parent,
                     BoardController *boardController,
                     StringTable *    stringTable);

    /**
     * @brief       Destructor.
     */
    virtual ~ManualModeWidget();

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
     * @brief       Firmware mode updated.
     *
     * @param[in]   success Success flag.
     * @param[in]   mode    Firmware mode.
     */
    void onFirmwareModeUpdated(bool success, FirmwareMode mode);
};
