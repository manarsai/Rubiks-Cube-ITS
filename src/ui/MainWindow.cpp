#include "MainWindow.h"
#include "CameraWidget.h"
#include "../visual/cubeView.h"

#include <QStackedWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QWidget>
#include <QApplication>
#include "CubeNet.h"

MainWindow::MainWindow()
{
    // =========================
    // STACK (screen manager)
    // =========================
    stack = new QStackedWidget(this);
    setCentralWidget(stack);


    qApp->setStyleSheet(
        "QMainWindow {"
        "   background: qlineargradient("
        "       x1:0, y1:0, x2:0, y2:1,"
        "       stop:0 #ffffff,"
        "       stop:0.8 #ffffff,"
        "       stop:1 #b3daff"
        "   );"
        "}"


    );

    // START SCREEN
    startScreen = new QWidget();
    QHBoxLayout* startLayout = new QHBoxLayout(startScreen);

    // =========================
    // LEFT PANEL (navigation)
    // =========================
    QWidget* navPanel = new QWidget();
    QVBoxLayout* navLayout = new QVBoxLayout(navPanel);

    QPushButton* newButton = new QPushButton("New");
    QPushButton* continueButton = new QPushButton("Continue");
    QPushButton* settingsButton = new QPushButton("Settings");
    QPushButton* exitButton = new QPushButton("Exit");

    QString style =
        "QPushButton {"
        "   background: transparent;"
        "   color: black;"
        "   border-radius: 10px;"
        "   padding: 10px;"
        "   font-size: 28px;"
        "}"
        "QPushButton:hover {"
        "   color: #81b0de;"
        "}";

    // apply to all
    newButton->setStyleSheet(style);
    continueButton->setStyleSheet(style);
    settingsButton->setStyleSheet(style);
    exitButton->setStyleSheet(style);

    navLayout->addStretch();
    navLayout->addWidget(newButton);
    navLayout->addWidget(continueButton);
    navLayout->addWidget(settingsButton);
    navLayout->addWidget(exitButton);
    navLayout->addStretch();

    navPanel->setFixedWidth(200);


    // =========================
    // RIGHT PANEL (cube)
    // =========================
    cubeStart = new cubeView();
    cubeStart->setMinimumSize(200, 200);
    cubeStart->setMaximumSize(300, 300);

    // optional: make cube expand nicely
    cubeStart->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    // =========================
    // ADD TO MAIN LAYOUT
    // =========================
    startLayout->addWidget(navPanel);   // LEFT
    startLayout->addWidget(cubeStart);  // RIGHT




    // MAIN SCREEN
    mainScreen = new QWidget();
    mainScreen = new QWidget();

    QVBoxLayout* mainLayout = new QVBoxLayout(mainScreen);

    mainLayout->setContentsMargins(10, 10, 10, 10);
    mainLayout->setSpacing(10);

    // =========================
    // TOP (CUBE AREA)
    // =========================
    QWidget* topPanel = new QWidget();
    QHBoxLayout* topLayout = new QHBoxLayout(topPanel);

    // LEFT SPACER (small)
    topLayout->addSpacing(30);

    // CUBE
    cubeMain = new cubeView();
    cubeMain->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    cubeMain->setMinimumWidth(350);
    cubeMain->setMaximumWidth(600);

    topLayout->addWidget(cubeMain);

    // RIGHT STRETCH (pushes slightly left, but not extreme)
    topLayout->addStretch();
 
    // =========================
    // BOTTOM (SCAN AREA)
    // =========================
    QWidget* bottomPanel = new QWidget();
    QHBoxLayout* bottomLayout = new QHBoxLayout(bottomPanel);

    QPushButton* scanButton = new QPushButton("Scan Cube");
    scanButton->setFixedHeight(50);

    // optional alignment
    bottomLayout->addWidget(scanButton);
    bottomLayout->addStretch();

    // =========================
    // ADD TO MAIN LAYOUT
    // =========================
    mainLayout->addWidget(cubeMain, 5);     // BIG TOP AREA
    mainLayout->addWidget(bottomPanel, 1);  // SMALL BOTTOM AREA


    // CAMERA SCREEN
    QWidget* cameraScreen = new QWidget();
    QHBoxLayout* layout = new QHBoxLayout(cameraScreen);

    // create widgets
    cameraWidget = new CameraWidget();
    CubeNet* grid = new CubeNet();

    // add to layout
    layout->addWidget(cameraWidget, 3); // LEFT (bigger)
    layout->addWidget(grid, 2);         // RIGHT (smaller)

    


    // ADD SCREENS TO STACK
    stack->addWidget(startScreen);   // 0
    stack->addWidget(mainScreen);    // 1
    stack->addWidget(cameraScreen);

    stack->setCurrentWidget(startScreen);

    // NAVIGATION
    // START ? MAIN
    connect(newButton, &QPushButton::clicked, this, [=]() {
        stack->setCurrentWidget(mainScreen);
        });

    connect(exitButton, &QPushButton::clicked, qApp, &QApplication::quit);


    // MAIN ? CAMERA
    connect(scanButton, &QPushButton::clicked, this, [=]() {
        stack->setCurrentWidget(cameraScreen);
        cameraWidget->startCamera();
        });

    // CAMERA ? MAIN
    connect(cameraWidget, &CameraWidget::backRequested, this, [=]() {
        cameraWidget->stopCamera();
        stack->setCurrentWidget(mainScreen);
        });

    resize(1000, 600);
}