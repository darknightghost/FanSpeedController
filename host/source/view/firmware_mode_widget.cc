#include <QtWidgets/QGridLayout>
#include <QtWidgets/QLabel>

#include <view/firmware_mode_widget.h>

/**
 * @brief       Constructor.
 */
FirmwareModeWidget::FirmwareModeWidget(QWidget *        parent,
                                       BoardController *boardController,
                                       StringTable *    stringTable) :
    QWidget(parent),
    m_boardController(boardController), m_stringTable(stringTable)
{
    // Initialize UI
    QGridLayout *layout = new QGridLayout();
    this->setLayout(layout);

    QLabel *label
        = new QLabel(m_stringTable->getString("STR_LABEL_SET_FIRMWARE_MODE"));
    layout->addWidget(label, 0, 0);

    m_comboFirmwareMode = new QComboBox();
    layout->addWidget(m_comboFirmwareMode, 0, 1);
    m_comboFirmwareMode->setEnabled(false);
    m_comboFirmwareMode->addItems(
        {m_stringTable->getString("STR_FIRMWARE_MODE_NORMAL"),
         m_stringTable->getString("STR_FIRMWARE_MODE_MANUAL"),
         m_stringTable->getString("STR_FIRMWARE_MODE_TEST")});
    m_comboFirmwareMode->setItemData(0,
                                     static_cast<quint8>(FirmwareMode::Normal));
    m_comboFirmwareMode->setItemData(1,
                                     static_cast<quint8>(FirmwareMode::Manual));
    m_comboFirmwareMode->setItemData(2,
                                     static_cast<quint8>(FirmwareMode::Test));
    m_comboFirmwareMode->setCurrentIndex(0);
    m_comboFirmwareMode->setEnabled(false);

    m_btnSet = new QPushButton(m_stringTable->getString("STR_BTN_SET"));
    layout->addWidget(m_btnSet, 0, 2);
    m_btnSet->setEnabled(false);
    this->connect(m_btnSet, &QPushButton::clicked, this,
                  &FirmwareModeWidget::onBtnSetClicked);

    label = new QLabel(
        m_stringTable->getString("STR_LABEL_CURRENT_FIRMWARE_MODE"));
    layout->addWidget(label, 1, 0);

    m_txtCurrentMode = new QLineEdit();
    layout->addWidget(m_txtCurrentMode, 1, 1, 1, 2);
    m_txtCurrentMode->setReadOnly(true);
    m_txtCurrentMode->setEnabled(false);

    layout->setColumnStretch(0, 0);
    layout->setColumnStretch(1, 100);
    layout->setColumnStretch(2, 0);

    // Connect signals.
    this->connect(m_boardController, &BoardController::opened, this,
                  &FirmwareModeWidget::onOpened, Qt::QueuedConnection);
    this->connect(m_boardController, &BoardController::closed, this,
                  &FirmwareModeWidget::onClosed, Qt::QueuedConnection);
    this->connect(m_boardController, &BoardController::firmwareModeUpdated,
                  this, &FirmwareModeWidget::onFirmwareModeUpdated,
                  Qt::QueuedConnection);
    this->connect(this, &FirmwareModeWidget::updateFirmwareMode,
                  m_boardController, &BoardController::updateFirmwareMode,
                  Qt::QueuedConnection);
    this->connect(this, &FirmwareModeWidget::setFirmwareMode, m_boardController,
                  &BoardController::setFirmwareMode, Qt::QueuedConnection);
}

/**
 * @brief       Destructor.
 */
FirmwareModeWidget::~FirmwareModeWidget() {}

/**
 * @brief       Opened slots.
 */
void FirmwareModeWidget::onOpened()
{
    m_comboFirmwareMode->setEnabled(true);
    m_btnSet->setEnabled(true);
    m_txtCurrentMode->setEnabled(true);
    emit this->updateFirmwareMode();
}

/**
 * @brief       Closed slots.
 */
void FirmwareModeWidget::onClosed()
{
    m_comboFirmwareMode->setEnabled(false);
    m_btnSet->setEnabled(false);
    m_txtCurrentMode->setEnabled(false);
    m_txtCurrentMode->setText("");
}

/**
 * @brief       On button open clicked.
 */
void FirmwareModeWidget::onBtnSetClicked()
{
    emit this->setFirmwareMode(
        static_cast<FirmwareMode>(m_comboFirmwareMode->currentData().toUInt()));
    emit this->updateFirmwareMode();
}

/**
 * @brief       Firmware mode updated.
 */
void FirmwareModeWidget::onFirmwareModeUpdated(FirmwareMode mode)
{
    switch (mode) {
        case FirmwareMode::Normal:
            m_txtCurrentMode->setText(
                m_stringTable->getString("STR_FIRMWARE_MODE_NORMAL"));
            break;

        case FirmwareMode::Manual:
            m_txtCurrentMode->setText(
                m_stringTable->getString("STR_FIRMWARE_MODE_MANUAL"));
            break;

        case FirmwareMode::Test:
            m_txtCurrentMode->setText(
                m_stringTable->getString("STR_FIRMWARE_MODE_TEST"));
            break;
    }
}
