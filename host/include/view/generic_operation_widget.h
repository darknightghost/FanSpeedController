#pragma once

#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>

#include <controller/board_controller.h>
#include <locale/string_table.h>

/**
 * @brief       Generic operation widget.
 */
class GenericOperationWidget : public QWidget {
    Q_OBJECT;

  private:
    BoardController *m_boardController; ///< Board controller.
    StringTable *    m_stringTable;     ///< String table.

    QPushButton
        *      m_btnStartStopGetSpeed; ///< Button start/stop to get fan speed.
    QLineEdit *m_txtSpeedHz;           ///< Fan speed in Herz.
    QLineEdit *m_txtSpeedRPM;          ///< Fan speed in rpm.

    QPushButton
        *      m_btnStartStopGetBootTime; ///< Button start/stop get boot time.
    QLineEdit *m_txtBootTime;             ///< Text to show boot time.

    QTimer *m_updateTimer;    ///< Update timer.
    bool    m_updateSpeed;    ///< Update speed.
    bool    m_updateBootTime; ///< Update boot time.

  public:
    /**
     * @brief       Constructor.
     *
     * @param[in]   parent              Parent widget.
     * @param[in]   boardController     Board controller.
     * @param[in]   stringTable         String table.
     */
    GenericOperationWidget(QWidget *        parent,
                           BoardController *boardController,
                           StringTable *    stringTable);

    /**
     * @brief       Destructor.
     */
    virtual ~GenericOperationWidget();

  signals:
    /**
     * @brief       Update fan speed.
     */
    void updateSpeed();

    /**
     * @brief       Update clock.
     */
    void updateClock();

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
     * @brief       On button start get speed clicked.
     */
    void onBtnStartGetSpeedClicked();

    /**
     * @brief       On button stop get speed clicked.
     */
    void onBtnStopGetSpeedClicked();

    /**
     * @brief       On button start get boot time clicked.
     */
    void onBtnStartGetBootTimeClicked();

    /**
     * @brief       On button stop get boot time clicked.
     */
    void onBtnStopGetBootTimeClicked();

    /**
     * @brief       On timer.
     */
    void onTimer();

    /**
     * @brief       Firmware mode signal.
     *
     * @param[in]   speed   Speed(HZ).
     */
    void onSpeedUpdated(quint16 speed);

    /**
     * @brief       Firmware mode signal.
     *
     * @param[in]   time    Boot time(microseconds).
     */
    void onClockUpdated(quint32 time);
};
