#include "Styles.h"
#include <QApplication>

void Styles::apply()
{
    const QString style = R"(
    QMainWindow {
        background: qlineargradient(
            x1:0, y1:0, x2:0, y2:1,
            stop:0 #ffffff,
            stop:0.75 #ffffff,
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

    QPushButton#scanButton,
    QPushButton#RetryButton,
    QPushButton#MetricButton,
    QPushButton#backButtonMain {
        font-size: 25px;
    }

    QPushButton#scanFaceButton {
        color: #2c7be5;
    }

    QPushButton#backButton,
    QPushButton#backButtonmain {
        font-size: 20px;
    }

    /* ===== NAV LABEL STYLE ===== */
    QLabel#navLabel {
        color: black;
        font-size: 40px;
        margin-bottom: 10px;
    }

QProgressBar#stageBar {
    border: 1px solid #2c7be5; 
    border-radius: 12px;
    background-color: #e1effc;
    height: 15px;
    text-align: center;
    color: black;
    font-size: 16px;
  
}

QProgressBar#stageBar::chunk {
    background-color: #b3daff;
    border-radius: 12px;
}

QLabel#scanInstruction {
    color: #2c7be5;
    font-size: 26px;
    font-weight: bold;
}

)";

    qApp->setStyleSheet(style);
}