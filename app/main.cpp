#include <QApplication>
#include "../src/ui/MainWindow.h"
#include "../src/ui/Styles.h"
#include "../src/database/Database.h"
#include <QSurfaceFormat>
#include <QFont>

int main(int argc, char* argv[])
{
    QSurfaceFormat format;
    format.setAlphaBufferSize(8);
    QSurfaceFormat::setDefaultFormat(format);

    QApplication app(argc, argv);

    QFont font("Inter", 10);
    app.setFont(font);

    // =========================
    // DATABASE STARTUP
    // =========================
    Database::instance().open();
    Database::instance().initTables();

    MainWindow w;
    w.show();

    int result = app.exec();

    // NO close() EXISTS → do nothing or add later if needed

    return result;
}