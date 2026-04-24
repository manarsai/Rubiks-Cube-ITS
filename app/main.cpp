#include <QApplication>
#include "../src/ui/MainWindow.h"
#include "../src/ui/Styles.h"
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
  // ✅ ADD THIS LINE

    MainWindow w;
    w.show();

    return app.exec();
}