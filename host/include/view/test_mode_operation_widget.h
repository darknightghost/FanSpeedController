#pragma once

#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>

#include <controller/board_controller.h>
#include <locale/string_table.h>

/**
 * @brief       Test mode widget.
 */
class TestModeWidget : public QWidget {
    Q_OBJECT;

  private:
    BoardController *m_boardController; ///< Board controller.
    StringTable *    m_stringTable;     ///< String table.

    QPushButton *m_btnStartStopReadSpeedInput; ///< Button start/stop to read
                                               ///< speed input port.
    QLineEdit
        *m_txtSpeedInputValue; ///< Text to show the value of speed input port.

    QPushButton *m_btnStartStopReadPWMInput; ///< Button start/stop to read
                                             ///< PWM input port.
    QLineEdit
        *m_txtPWMInputValue; ///< Text to show the value of wpm input port.

    QPushButton
        *m_btnSetSpeedOutputWrite0; ///< Button to set speed output port to 0.
    QPushButton
        *m_btnSetSpeedOutputWrite1; ///< Button to set speed output port to 1.

    QPushButton
        *m_btnSetPWMOutputWrite0; ///< Button to set PWM output port to 0.
    QPushButton
        *m_btnSetPWMOutputWrite1; ///< Button to set PWM output port to 1.

    QTimer *m_updateTimer;      ///< Update timer.
    bool    m_updateSpeedInput; ///< Update speed input port.
    bool    m_updatePWMInput;   ///< Update PWM input port.

  public:
    /**
     * @brief       Constructor.
     *
     * @param[in]   parent              Parent widget.
     * @param[in]   boardController     Board controller.
     * @param[in]   stringTable         String table.
     */
    TestModeWidget(QWidget *        parent,
                   BoardController *boardController,
                   StringTable *    stringTable);

    /**
     * @brief       Destructor.
     */
    virtual ~TestModeWidget();

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
