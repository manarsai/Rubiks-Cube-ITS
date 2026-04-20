#include "Styles.h"
#include <QApplication>

void Styles::apply()
{
    const QString style = R"(
    QMainWindow {
        background: qlineargradient(
            x1:0, y1:0, x2:0, y2:1,
            stop:0 #ffffff,
            stop:0.8 #ffffff,
            stop:1 #b3daff
        );
    }

    /* Base button style */
    QPushButton {
        background: transparent;
        color: black;
        border-radius: 10px;
        padding: 10px;
        font-size: 28px;
    }

    QPushButton:hover {
        color: #81b0de;
    }

    QPushButton#scanButton {
        font-size: 30px;
    }

    QPushButton#scanFaceButton {
        color: #2c7be5;
    }

    QPushButton#backButton,
    QPushButton#backButtonmain {
        font-size: 24px;
    }

    /* ===== NAV LABEL STYLE ===== */
    QLabel#navLabel {
        color: black;
        font-size: 40px;
        margin-bottom: 10px;
    }

)";

    qApp->setStyleSheet(style);
}