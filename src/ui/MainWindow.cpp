#include "MainWindow.h"
#include "CameraWidget.h"
#include "../visual/cubeView.h"
#include "CubeNet.h"

#include <QStackedWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QWidget>
#include <QApplication>
#include <QMessageBox>
#include "../database/Database.h"

MainWindow::MainWindow()
{
    stack = new QStackedWidget(this);
    setCentralWidget(stack);

    setupStartScreen();
    setupMainScreen();
    setupCameraScreen();
    applyStyles();
    setupConnections();

    if (!Database::getInstance().open())
    {
        QMessageBox::critical(this, "Error", "Database failed to open!");
    }
    else
    {
        Database::getInstance().initTables();
    }




    stack->addWidget(startScreen);
    stack->addWidget(mainScreen);
    stack->addWidget(cameraScreen);

    stack->setCurrentWidget(startScreen);

    resize(1000, 600);
}

MainWindow::~MainWindow()
{
    Database::getInstance().close();
}

// =========================
// SETUP SCREENS
// =========================

void MainWindow::setupStartScreen()
{
    startScreen = new QWidget();
    startScreen->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    QHBoxLayout* rootLayout = new QHBoxLayout(startScreen);
    rootLayout->setContentsMargins(0, 0, 0, 0);
    rootLayout->setSpacing(0);

    // ---- NAV PANEL ----
    QWidget* navPanel = new QWidget(startScreen);
    QVBoxLayout* navLayout = new QVBoxLayout(navPanel);

    newButton = new QPushButton("New");
    continueButton = new QPushButton("Continue");
    settingsButton = new QPushButton("Settings");
    exitButton = new QPushButton("Exit");

    QString style =
        "QPushButton {"
        " background: transparent;"
        " color: black;"
        " border-radius: 10px;"
        " padding: 10px;"
        " font-size: 28px;"
        "}"
        "QPushButton:hover {"
        " color: #81b0de;"
        "}";

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

    // CUBE 
    cubeStart = new cubeView(startScreen);
    cubeStart->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    //cubeStart->setMinimumSize(200, 200); 
     cubeStart->setMaximumSize(300, 300);

    
    rootLayout->addWidget(navPanel, 0);
    rootLayout->addWidget(cubeStart, 1);
}

void MainWindow::setupMainScreen()
{
    mainScreen = new QWidget();
    QVBoxLayout* layout = new QVBoxLayout(mainScreen);

    cubeMain = new cubeView(mainScreen);

    scanButton = new QPushButton("Scan Cube");
    scanButton->setFixedHeight(50);

    backButtonmain = new QPushButton("Back");

    layout->addWidget(cubeMain, 5);
    layout->addWidget(scanButton, 1);
    layout->addWidget(backButtonmain, 1);
}

void MainWindow::setupCameraScreen()
{
    cameraScreen = new QWidget();
    QVBoxLayout* mainLayout = new QVBoxLayout(cameraScreen);

    QHBoxLayout* topBar = new QHBoxLayout();

    backButton = new QPushButton("Back");
    scanFaceButton = new QPushButton("Scan Face");

    topBar->addWidget(backButton);
    topBar->addWidget(scanFaceButton);
    topBar->addStretch();

    QHBoxLayout* contentLayout = new QHBoxLayout();

    cameraWidget = new CameraWidget(cameraScreen);
    grid = new CubeNet(cameraScreen);

    contentLayout->addWidget(cameraWidget, 3);
    contentLayout->addWidget(grid, 2);

    mainLayout->addLayout(topBar);
    mainLayout->addLayout(contentLayout);

    currentFace = 0;
}


// LOGIC

bool MainWindow::isValidFace(const std::array<Colour, 9>& face)
{
    for (auto c : face)
    {
        if (c == Colour::UNKNOWN)
            return false;
    }
    return true;
}

void MainWindow::handleScan()
{
    if (currentFace >= 6)
        return;

    auto face = cameraWidget->captureFace();

    if (!isValidFace(face))
        return;

    grid->setFaceColours(currentFace, face);

    currentFace++;

    Database::getInstance().saveSession(
        currentFace,
        grid->serializeState()
    );

    continueButton->setVisible(true);

    if (currentFace == 6)
    {
        bool ok = grid->validateCube();

        if (!ok)
        {
            QMessageBox::warning(this, "Invalid Cube",
                "Cube scan is invalid!\nEach color must appear exactly 9 times.");
        }
        else
        {
            QMessageBox::information(this, "Success",
                "Cube scanned correctly!");

        }

     /*   cubeMain->setCubeState(grid->getCubeState());*/
        scanFaceButton->setEnabled(false);
        scanFaceButton->setText("Done");
    }


}

void MainWindow::resetScan()
{
    currentFace = 0;
    scanFaceButton->setEnabled(true);
    scanFaceButton->setText("Scan Face");
}

// =========================
// CONNECTIONS
// =========================

void MainWindow::setupConnections()
{
    connect(newButton, &QPushButton::clicked, this, [this]() {

        Database::getInstance().resetSession();

        continueButton->setVisible(false); 

        resetScan();
        stack->setCurrentWidget(mainScreen);
        });

    connect(exitButton, &QPushButton::clicked, qApp, &QApplication::quit);

    connect(scanButton, &QPushButton::clicked, this, [this]() {
        stack->setCurrentWidget(cameraScreen);
        cameraWidget->startCamera();
        resetScan();
        });

    connect(backButton, &QPushButton::clicked, this, [this]() {
        cameraWidget->stopCamera();
        stack->setCurrentWidget(mainScreen);
        });

    connect(scanFaceButton, &QPushButton::clicked, this, [this]() {
        handleScan();
        });

    connect(backButtonmain, &QPushButton::clicked, this, [this]() {
        stack->setCurrentWidget(startScreen);
        });

    connect(continueButton, &QPushButton::clicked, this, [this]() {

        int face = 0;
        std::string state;

        if (Database::getInstance().loadSession(face, state))
        {
            currentFace = face;
            grid->restoreState(state);
        }

        stack->setCurrentWidget(mainScreen);
        });
}

void MainWindow::applyStyles()
{
    QString buttonStyle =
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

    newButton->setStyleSheet(buttonStyle);
    continueButton->setStyleSheet(buttonStyle);
    settingsButton->setStyleSheet(buttonStyle);
    exitButton->setStyleSheet(buttonStyle);

    scanButton->setStyleSheet(buttonStyle);
    scanFaceButton->setStyleSheet(buttonStyle);
    backButton->setStyleSheet(buttonStyle);
}