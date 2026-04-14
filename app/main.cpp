#include <QApplication>
#include "../src/ui/MainWindow.h"
#include <qopenglcontext.h>

int main(int argc, char* argv[])
{
    QSurfaceFormat format;
    format.setAlphaBufferSize(8);
    QSurfaceFormat::setDefaultFormat(format);
    QApplication app(argc, argv);
    QFont font("Inter", 10);
    app.setFont(font);

    MainWindow w;
    w.show();

    return app.exec();
}