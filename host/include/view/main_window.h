#pragma once

#include <QtWidgets/QTabWidget>
#include <QtWidgets/QWidget>

#include <controller/board_controller.h>
#include <locale/string_table.h>

#include <view/firmware_mode_widget.h>
#include <view/generic_operation_widget.h>
#include <view/message_widget.h>
#include <view/serial_widget.h>

class MainWindow : public QWidget {
    Q_OBJECT
  private:
    BoardController *m_boardController; ///< Board controller.
    StringTable *    m_stringTable;     ///< String table.

    SerialWidget *      m_serialWidget;       ///< Serial widget.
    FirmwareModeWidget *m_firmwareModeWidget; ///< Firmware mode widget.
    GenericOperationWidget
        *m_genericOperationWidget; ///< Generic operation widget.

    MessageWidget *m_messageWidget; ///< Message widget.

  public:
    /**
     * @brief       Constructor.
     */
    MainWindow();

    /**
     * @brief       Destructor.
     */
    virtual ~MainWindow();

  private slots:
    /**
     * @brief         Close event.
     *
     * @param[in]     event       Event.
     */
    virtual void closeEvent(QCloseEvent *event) override;
};
