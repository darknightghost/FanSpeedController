#include <QtCore/QTimer>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QLabel>

#include <view/generic_operation_widget.h>

/**
 * @brief       Constructor.
 */
GenericOperationWidget::GenericOperationWidget(QWidget *        parent,
                                               BoardController *boardController,
                                               StringTable *    stringTable) :
    QWidget(parent),
    m_boardController(boardController), m_stringTable(stringTable),
    m_updateSpeed(false), m_updateBootTime(false)
{
    QGridLayout *layout = new QGridLayout();
    this->setLayout(layout);

    // Fan speed.
    m_btnStartStopGetSpeed = new QPushButton(
        m_stringTable->getString("STR_BTN_START_READING_FAN_SPEED"));
    layout->addWidget(m_btnStartStopGetSpeed, 0, 0);
    m_btnStartStopGetSpeed->setEnabled(false);
    this->connect(m_btnStartStopGetSpeed, &QPushButton::clicked, this,
                  &GenericOperationWidget::onBtnStartGetSpeedClicked);

    layout->addWidget(
        new QLabel(m_stringTable->getString("STR_LABEL_FAN_SPEED")), 0, 1);

    m_txtSpeedHz = new QLineEdit("0");
    layout->addWidget(m_txtSpeedHz, 0, 2);
    m_txtSpeedHz->setReadOnly(true);

    layout->addWidget(new QLabel("Hz"), 0, 3);

    m_txtSpeedRPM = new QLineEdit("0");
    layout->addWidget(m_txtSpeedRPM, 0, 4);
    m_txtSpeedRPM->setReadOnly(true);

    layout->addWidget(new QLabel("RPM"), 0, 5);

    // Clock.
    m_btnStartStopGetBootTime = new QPushButton(
        m_stringTable->getString("STR_BTN_START_READING_BOOT_TIME"));
    layout->addWidget(m_btnStartStopGetBootTime, 1, 0);
    m_btnStartStopGetBootTime->setEnabled(false);
    this->connect(m_btnStartStopGetBootTime, &QPushButton::clicked, this,
                  &GenericOperationWidget::onBtnStartGetBootTimeClicked);

    layout->addWidget(
        new QLabel(m_stringTable->getString("STR_LABEL_BOOT_TIME")), 1, 1);

    m_txtBootTime = new QLineEdit("0");
    layout->addWidget(m_txtBootTime, 1, 2, 1, 3);
    m_txtBootTime->setReadOnly(true);
    layout->addWidget(new QLabel("μs"), 1, 5);

    layout->setColumnStretch(0, 0);
    layout->setColumnStretch(1, 0);
    layout->setColumnStretch(2, 100);
    layout->setColumnStretch(3, 0);
    layout->setColumnStretch(4, 100);
    layout->setColumnStretch(5, 0);

    // Timer.
    m_updateTimer = new QTimer(this);
    m_updateTimer->setSingleShot(false);
    m_updateTimer->setInterval(1000);
    m_updateTimer->stop();
    this->connect(m_updateTimer, &QTimer::timeout, this,
                  &GenericOperationWidget::onTimer);

    // Connect.
    this->connect(m_boardController, &BoardController::speedUpdated, this,
                  &GenericOperationWidget::onSpeedUpdated);
    this->connect(m_boardController, &BoardController::clockUpdated, this,
                  &GenericOperationWidget::onClockUpdated);
    this->connect(m_boardController, &BoardController::opened, this,
                  &GenericOperationWidget::onOpened);
    this->connect(m_boardController, &BoardController::closed, this,
                  &GenericOperationWidget::onClosed);
    this->connect(this, &GenericOperationWidget::updateSpeed, m_boardController,
                  &BoardController::updateSpeed);
    this->connect(this, &GenericOperationWidget::updateClock, m_boardController,
                  &BoardController::updateClock);
}

/**
 * @brief       Destructor.
 */
GenericOperationWidget::~GenericOperationWidget() {}

/**
 * @brief       Opened slots.
 */
void GenericOperationWidget::onOpened()
{
    m_updateTimer->start();
    m_btnStartStopGetSpeed->setEnabled(true);
    m_btnStartStopGetBootTime->setEnabled(true);
}

/**
 * @brief       Closed slots.
 */
void GenericOperationWidget::onClosed()
{
    m_updateTimer->stop();
    m_btnStartStopGetSpeed->setEnabled(false);
    m_btnStartStopGetBootTime->setEnabled(false);
}

/**
 * @brief       On button start get speed clicked.
 */
void GenericOperationWidget::onBtnStartGetSpeedClicked()
{
    m_updateSpeed = true;
    m_btnStartStopGetSpeed->setText(
        m_stringTable->getString("STR_BTN_STOP_READING_FAN_SPEED"));
    this->disconnect(m_btnStartStopGetSpeed);
    this->connect(m_btnStartStopGetSpeed, &QPushButton::clicked, this,
                  &GenericOperationWidget::onBtnStopGetSpeedClicked);
}

/**
 * @brief       On button stop get speed clicked.
 */
void GenericOperationWidget::onBtnStopGetSpeedClicked()
{
    m_updateSpeed = false;
    m_btnStartStopGetSpeed->setText(
        m_stringTable->getString("STR_BTN_START_READING_FAN_SPEED"));
    this->disconnect(m_btnStartStopGetSpeed);
    this->connect(m_btnStartStopGetSpeed, &QPushButton::clicked, this,
                  &GenericOperationWidget::onBtnStartGetSpeedClicked);
}

/**
 * @brief       On button start get boot time clicked.
 */
void GenericOperationWidget::onBtnStartGetBootTimeClicked()
{
    m_updateBootTime = true;
    m_btnStartStopGetBootTime->setText(
        m_stringTable->getString("STR_BTN_STOP_READING_BOOT_TIME"));
    this->disconnect(m_btnStartStopGetBootTime);
    this->connect(m_btnStartStopGetBootTime, &QPushButton::clicked, this,
                  &GenericOperationWidget::onBtnStopGetBootTimeClicked);
}

/**
 * @brief       On button stop get boot time clicked.
 */
void GenericOperationWidget::onBtnStopGetBootTimeClicked()
{
    m_updateBootTime = false;
    m_btnStartStopGetBootTime->setText(
        m_stringTable->getString("STR_BTN_START_READING_BOOT_TIME"));
    this->disconnect(m_btnStartStopGetBootTime);
    this->connect(m_btnStartStopGetBootTime, &QPushButton::clicked, this,
                  &GenericOperationWidget::onBtnStartGetBootTimeClicked);
}

/**
 * @brief       On timer.
 */
void GenericOperationWidget::onTimer()
{
    if (m_updateSpeed) {
        emit this->updateSpeed();
    }
    if (m_updateBootTime) {
        emit this->updateClock();
    }
}

/**
 * @brief       Firmware mode signal.
 */
void GenericOperationWidget::onSpeedUpdated(quint16 speed)
{
    m_txtSpeedHz->setText(QString("%1").arg(speed));
    m_txtSpeedRPM->setText(
        QString("%1").arg(static_cast<quint64>(speed) / 2 * 60));
}

/**
 * @brief       Firmware mode signal.
 */
void GenericOperationWidget::onClockUpdated(quint32 time)
{
    m_txtBootTime->setText(QString("%1").arg(time));
}
