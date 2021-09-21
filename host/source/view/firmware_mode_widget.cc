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
    m_comboFirmwareMode->setItemData(
        0, (quint8)(BoardController::FirmwareMode::Normal));
    m_comboFirmwareMode->setItemData(
        1, (quint8)(BoardController::FirmwareMode::Manual));
    m_comboFirmwareMode->setItemData(
        2, (quint8)(BoardController::FirmwareMode::Test));
    m_comboFirmwareMode->setCurrentIndex(0);
    m_comboFirmwareMode->setEnabled(false);

    m_btnSet = new QPushButton(m_stringTable->getString("STR_BTN_SET"));
    layout->addWidget(m_btnSet, 0, 2);
    m_btnSet->setEnabled(false);

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
}

/**
 * @brief       Destructor.
 */
FirmwareModeWidget::~FirmwareModeWidget() {}

/**
 * @brief       Opened slots.
 */
void FirmwareModeWidget::onOpened() {}

/**
 * @brief       Closed slots.
 */
void FirmwareModeWidget::onClosed() {}

/**
 * @brief       On button open clicked.
 */
void FirmwareModeWidget::onBtnSetClicked() {}

/**
 * @brief       Firmware mode updated.
 */
void FirmwareModeWidget::onFirmwareModeUpdated(
    BoardController::FirmwareMode mode)
{}
