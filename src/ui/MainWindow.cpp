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
#include <QLabel>
#include "Styles.h"



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
    setupConnections();

    Styles::apply();

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
    root->setContentsMargins(20, 20, 20, 20);
    root->setSpacing(30);

    QWidget* nav = new QWidget();
    QVBoxLayout* navLayout = new QVBoxLayout(nav);
    navLayout->setSpacing(10);

    QLabel* navLabel = new QLabel("RUBIK'S ITS");
    navLabel->setAlignment(Qt::AlignCenter);
    navLabel->setObjectName("navLabel");

    newButton = new QPushButton("New");
    continueButton = new QPushButton("Continue");
    exitButton = new QPushButton("Exit");

    newButton->setObjectName("newButton");
    continueButton->setObjectName("continueButton");
    exitButton->setObjectName("exitButton");

    navLayout->addStretch();
    navLayout->addWidget(navLabel);
    navLayout->addWidget(newButton);
    navLayout->addWidget(continueButton);
    navLayout->addWidget(exitButton);
    navLayout->addStretch();

    cubeStart = new cubeView(startScreen);
    cubeStart->setFixedSize(300, 300);

    root->addStretch();
    root->addWidget(nav);
    root->addWidget(cubeStart);
    root->addStretch();
}


// MAIN SCREEN
void MainWindow::setupMainScreen()
{
    mainScreen = new QWidget();

    QGridLayout* layout = new QGridLayout(mainScreen);
    layout->setContentsMargins(20, 20, 20, 20);
    layout->setSpacing(15);

    cubeMain = new cubeView(mainScreen);

    scanButton = new QPushButton("Scan Cube");
    backButtonmain = new QPushButton("Back");

    scanButton->setObjectName("scanButton");
    backButtonmain->setObjectName("backButtonMain");

    // Bottom-left container (group buttons)
    QWidget* bottomLeftContainer = new QWidget();
    QVBoxLayout* bottomLeftLayout = new QVBoxLayout(bottomLeftContainer);
    bottomLeftLayout->setContentsMargins(0, 0, 0, 0);
    bottomLeftLayout->setSpacing(10);

    bottomLeftLayout->addWidget(scanButton);
    bottomLeftLayout->addWidget(backButtonmain);
    bottomLeftLayout->addStretch();

    // Placeholders
    QWidget* topRight = new QWidget();
    QWidget* bottomRight = new QWidget();

    // Grid placement (true 2x2)
    layout->addWidget(cubeMain, 0, 0);              // top-left
    layout->addWidget(topRight, 0, 1);              // top-right
    layout->addWidget(bottomLeftContainer, 1, 0);   // bottom-left
    layout->addWidget(bottomRight, 1, 1);           // bottom-right

    // ? sizing: top bigger than bottom
    layout->setRowStretch(0, 3);
    layout->setRowStretch(1, 1);

    layout->setColumnStretch(0, 1);
    layout->setColumnStretch(1, 1);
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

    // ? OBJECT NAMES
    backButton->setObjectName("backButton");
    scanFaceButton->setObjectName("scanFaceButton");

    topBar->addWidget(backButton);
    topBar->addWidget(scanFaceButton);

    cameraWidget = new CameraWidget(cameraScreen);
    grid = new CubeNet(*cube, cameraScreen);

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

