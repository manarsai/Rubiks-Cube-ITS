#include "MainWindow.h"
#include "CameraWidget.h"
#include "../visual/cubeView.h"
#include "CubeNet.h"
#include "../core/domain/Cube.h"
#include "../database/Database.h"
#include "../core/tutor/StageDefinitions.h"

#include <QStackedWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QMessageBox>
#include <QApplication>



// CONSTRUCTOR
MainWindow::MainWindow()
{

    bool ok = Database::getInstance().open();

    if (!ok)
    {
        qDebug() << "? DATABASE FAILED TO OPEN";
    }
    else
    {
        qDebug() << "? DATABASE OPENED SUCCESSFULLY";
        Database::getInstance().initTables();
    }

    cube = new Cube(); // ? SINGLE SOURCE OF TRUTH

    stack = new QStackedWidget(this);
    setCentralWidget(stack);

    setupStartScreen();
    setupMainScreen();
    setupCameraScreen();
    applyStyles();
    setupConnections();

    Database::getInstance().open();
    Database::getInstance().initTables();

    stack->addWidget(startScreen);
    stack->addWidget(mainScreen);
    stack->addWidget(cameraScreen);

    stack->setCurrentWidget(startScreen);

    resize(1000, 600);
}


// DESTRUCTOR
MainWindow::~MainWindow()
{
    Database::getInstance().close();
    delete cube;
}


// START SCREEN
void MainWindow::setupStartScreen()
{
    startScreen = new QWidget();

    QHBoxLayout* root = new QHBoxLayout(startScreen);

    QWidget* nav = new QWidget();
    QVBoxLayout* navLayout = new QVBoxLayout(nav);

    newButton = new QPushButton("New");
    continueButton = new QPushButton("Continue");
    exitButton = new QPushButton("Exit");

    navLayout->addWidget(newButton);
    navLayout->addWidget(continueButton);
    navLayout->addWidget(exitButton);

    cubeStart = new cubeView(startScreen);

    root->addWidget(nav);
    root->addWidget(cubeStart);
}


// MAIN SCREEN
void MainWindow::setupMainScreen()
{
    mainScreen = new QWidget();
    QVBoxLayout* layout = new QVBoxLayout(mainScreen);

    cubeMain = new cubeView(mainScreen);

    scanButton = new QPushButton("Scan Cube");
    backButtonmain = new QPushButton("Back");

    layout->addWidget(cubeMain);
    layout->addWidget(scanButton);
    layout->addWidget(backButtonmain);
}


// CAMERA SCREEN
void MainWindow::setupCameraScreen()
{
    cameraScreen = new QWidget();

    QVBoxLayout* mainLayout = new QVBoxLayout(cameraScreen);
    QHBoxLayout* topBar = new QHBoxLayout();
    QHBoxLayout* content = new QHBoxLayout();

    backButton = new QPushButton("Back");
    scanFaceButton = new QPushButton("Scan Face");

    topBar->addWidget(backButton);
    topBar->addWidget(scanFaceButton);

    cameraWidget = new CameraWidget(cameraScreen);

    grid = new CubeNet(*cube, cameraScreen); // ? SAME CUBE

    content->addWidget(cameraWidget, 3);
    content->addWidget(grid, 2);

    mainLayout->addLayout(topBar);
    mainLayout->addLayout(content);

    currentFace = 0;
}

// =========================
// VALIDATION
// =========================
bool MainWindow::isValidFace(const std::array<Colour, 9>& face)
{
    for (auto c : face)
        if (c == Colour::UNKNOWN)
            return false;

    return true;
}

// =========================
// SCAN LOGIC
// =========================
void MainWindow::handleScan()
{
    if (currentFace >= 6)
        return;

    auto face = cameraWidget->captureFace();

    if (!isValidFace(face))
        return;

    cube->setFace(currentFace, face);
    grid->update();

    currentFace++;

    Database::getInstance().saveSession(
        currentFace,
        cube->serialize()
    );

    continueButton->setVisible(true);

    if (currentFace == 6)
    {
        std::cout << "RUNNING WHITE CROSS CHECK...\n";

        bool ok = StageDefinitions::get(Stage::WHITE_CROSS)
            .isComplete(*cube);

        std::cout << "WHITE CROSS RESULT: " << ok << "\n";

        QMessageBox::information(
            this,
            "Stage Check",
            ok ? "White cross COMPLETE" : "White cross NOT complete"
        );
    }
}
// =========================
// RESET
// =========================
void MainWindow::resetScan()
{
    currentFace = 0;
    scanFaceButton->setEnabled(true);
    scanFaceButton->setText("Scan Face");

    cube->reset();
    grid->update();
}

// =========================
// CONNECTIONS
// =========================
void MainWindow::setupConnections()
{
    connect(newButton, &QPushButton::clicked, this, [this]() {
        Database::getInstance().resetSession();
        resetScan();
        stack->setCurrentWidget(mainScreen);
        });

    connect(exitButton, &QPushButton::clicked, qApp, &QApplication::quit);

    connect(scanButton, &QPushButton::clicked, this, [this]() {

        // Only reset if starting fresh (optional safety check)
        if (currentFace == 0)
        {
            cube->reset();
        }

        currentFace = 0;

        stack->setCurrentWidget(cameraScreen);
        cameraWidget->startCamera();

        grid->update();
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

            // ? LOAD INTO CUBE ONLY
            cube->deserialize(state);
            grid->update();
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
        "       stop:1 #b3daff"
        "   );"
        "}"
    );

    newButton->setStyleSheet(buttonStyle);
    continueButton->setStyleSheet(buttonStyle);
    exitButton->setStyleSheet(buttonStyle);

    scanButton->setStyleSheet(buttonStyle);
    scanFaceButton->setStyleSheet(buttonStyle);
    backButton->setStyleSheet(buttonStyle);
    backButtonmain->setStyleSheet(buttonStyle);
}