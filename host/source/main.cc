#include <cstdlib>

#include <QtCore/QTextCodec>
#include <QtWidgets/QApplication>

#include <view/main_window.h>

/**
 * @brief		Entery.
 *
 * @param[in]	argc		Count of arguments.
 * @param[in]	argv		Values of arguments.
 *
 * @return		Exit code.
 */
int main(int argc, char *argv[])
{
    // Force UTF-8.
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));

    // High DPI support.
    QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QApplication app(argc, argv);
    app.setApplicationName("Fan speed controller");

    // Show main window.
    MainWindow mainWindow;
    mainWindow.show();

    return app.exec();
}
