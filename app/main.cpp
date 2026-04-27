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
    // DATABASE STARTUP (IMPORTANT)
    // =========================
    Database::getInstance().open();
    Database::getInstance().initTables();

    // load persisted user name
    std::string name;
    if (Database::getInstance().loadUserName(name))
    {
        Database::getInstance().setUserName(name);
    }

    MainWindow w;
    w.show();

    int result = app.exec();

    // =========================
    // CLEAN SHUTDOWN
    // =========================
    Database::getInstance().close();

    return result;
}