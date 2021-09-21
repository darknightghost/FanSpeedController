#pragma once

#include <QtWidgets/QWidget>

#include <controller/board_controller.h>
#include <locale/string_table.h>

class MainWindow : public QWidget {
    Q_OBJECT
  private:
    BoardController *m_boardController; ///< Board controller.
    StringTable *    m_stringTable;     ///< String table.

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
