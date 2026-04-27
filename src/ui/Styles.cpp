#include "Styles.h"
#include <QApplication>

void Styles::apply()
{
    const QString style = R"(
    /* =====================================================
       MAIN WINDOW BACKGROUND
    ===================================================== */
    QMainWindow {
        background: qlineargradient(
            x1:0, y1:0, x2:0, y2:1,
            stop:0 #ffffff,
            stop:0.75 #ffffff,
            stop:1 #b3daff
        );
    }

    /* =====================================================
       GLOBAL BUTTON STYLE
    ===================================================== */
    QPushButton {
        background: #2c7be5;
        color: white;
        border-radius: 5px;
        padding: 5px;
        font-size: 20px;
    }

    QPushButton:hover {
        background: #81b0de;
    }

    /* Small buttons (secondary actions) */
    QPushButton#backButton,
    QPushButton#backButtonMain,
    QPushButton#RetryButton,
    QPushButton#MetricButton {
        font-size: 20px;
    }

    /* Primary action buttons */
    QPushButton#scanButton,
    QPushButton#scanFaceButton {
        font-size: 15px;
    }

    /* =====================================================
       NAV PANEL TITLE
    ===================================================== */
    QLabel#navLabel {
        color: black;
        font-size: 40px;
        margin-bottom: 10px;
    }

    /* =====================================================
       STAGE PROGRESS BAR
    ===================================================== */
    QProgressBar#stageBar {
        border: 1px solid #2c7be5;
        border-radius: 12px;
        background-color: #f5f9ff;
        height: 35px;
        text-align: center;
        color: black;
        font-size: 16px;
    }

    QProgressBar#stageBar::chunk {
        background-color: #b3daff;
        border-radius: 12px;
    }

    /* =====================================================
       CAMERA / INSTRUCTION TEXT
    ===================================================== */
    QLabel#scanInstruction {
        color: #2c7be5;
        font-size: 26px;
        font-weight: bold;
    }

    /* =====================================================
       OPTIONAL: SOLVER OUTPUT
    ===================================================== */
    QLabel {
        color: #111;
    }

    QLabel#userLabel {
        color: #2c7be5;
        font-size: 22px;

        padding-left: 10px;
    }


    QWidget#guidanceBox {
    background: #f5f9ff;
    font-size: 20px;
    border: 1px solid #2c7be5;
    border-radius: 12px;
    padding: 10px;
}



    )";

    qApp->setStyleSheet(style);
}