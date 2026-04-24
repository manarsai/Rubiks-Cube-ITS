#include "MainWindow.h"

#include "../../app/AppController.h"
#include "../database/Database.h"
#include "../visual/cubeView.h"
#include "CubeNet.h"
#include "CameraWidget.h"
#include "Styles.h"

#include <QStackedWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QMessageBox>
#include <QApplication>
#include <QLabel>
#include <QProgressBar>

// =========================
// CONSTRUCTOR
// =========================
MainWindow::MainWindow()
{
    controller = new AppController();

    stack = new QStackedWidget(this);
    setCentralWidget(stack);

    setupStartScreen();
    setupMainScreen();
    setupCameraScreen();
    setupConnections();

    Styles::apply();

    stack->addWidget(startScreen);
    stack->addWidget(mainScreen);
    stack->addWidget(cameraScreen);

    stack->setCurrentWidget(startScreen);

    resize(1000, 600);
}

// =========================
// DESTRUCTOR
// =========================
MainWindow::~MainWindow()
{
    delete controller;
    Database::getInstance().close();
}

// =====================================================
// START SCREEN
// =====================================================
void MainWindow::setupStartScreen()
{
    startScreen = new QWidget();
    QHBoxLayout* root = new QHBoxLayout(startScreen);

    // ===== NAV SECTION =====
    QWidget* nav = new QWidget();
    QVBoxLayout* navLayout = new QVBoxLayout(nav);

    QLabel* title = new QLabel("RUBIK'S ITS");
    title->setObjectName("navLabel");
    title->setAlignment(Qt::AlignCenter);

    newButton = new QPushButton("New");
    continueButton = new QPushButton("Continue");
    exitButton = new QPushButton("Exit");

    newButton->setObjectName("newButton");
    continueButton->setObjectName("continueButton");
    exitButton->setObjectName("exitButton");

    navLayout->addStretch();
    navLayout->addWidget(title);
    navLayout->addWidget(newButton);
    navLayout->addWidget(continueButton);
    navLayout->addWidget(exitButton);
    navLayout->addStretch();

    // ===== CUBE SECTION =====
    QWidget* cubeContainer = new QWidget();
    QVBoxLayout* cubeLayout = new QVBoxLayout(cubeContainer);

    cubeStart = new cubeView(controller->getCube(), cubeContainer);
    cubeStart->setFixedSize(300, 300);

    cubeLayout->addStretch();
    cubeLayout->addWidget(cubeStart, 0, Qt::AlignCenter);
    cubeLayout->addStretch();

    // ===== LAYOUT =====
    root->addStretch();
    root->addWidget(nav);
    root->addWidget(cubeContainer);
    root->addStretch();
}

// =====================================================
// MAIN SCREEN
// =====================================================
void MainWindow::setupMainScreen()
{
    mainScreen = new QWidget();
    QGridLayout* layout = new QGridLayout(mainScreen);

    // =====================================================
    // TOP: STAGE BAR
    // =====================================================
    stageBar = new QProgressBar(mainScreen);
    stageBar->setObjectName("stageBar");
    stageBar->setRange(0, 5);
    stageBar->setTextVisible(true);
    stageBar->setFormat("Stage %v / %m");


    layout->addWidget(stageBar, 0, 0, 1, 2);

    // =====================================================
    // LEFT: CUBE VIEW
    // =====================================================
    cubeMain = new cubeView(controller->getCube(), mainScreen);

    // =====================================================
    // RIGHT: SOLVER OUTPUT
    // =====================================================
    QWidget* solverPanel = new QWidget();
    QVBoxLayout* solverLayout = new QVBoxLayout(solverPanel);

    QLabel* solverTitle = new QLabel("Solver Output");
    solverTitle->setAlignment(Qt::AlignCenter);

    solverOutputLabel = new QLabel("No solution yet");
    solverOutputLabel->setWordWrap(true);
    solverOutputLabel->setAlignment(Qt::AlignTop);

    solverLayout->addWidget(solverTitle);
    solverLayout->addWidget(solverOutputLabel);

    // =====================================================
    // BUTTONS PANEL
    // =====================================================
    scanButton = new QPushButton("Scan Cube");
    pauseButton = new QPushButton("Pause");

    QPushButton* retryButton = new QPushButton("Retry Stage");
    QPushButton* metricsButton = new QPushButton("Metrics");

    scanButton->setObjectName("scanButton");
    pauseButton->setObjectName("backButtonMain");
    retryButton->setObjectName("RetryButton");
    metricsButton->setObjectName("MetricButton");

    QWidget* leftButtons = new QWidget();
    QVBoxLayout* leftButtonsLayout = new QVBoxLayout(leftButtons);
    leftButtonsLayout->setSpacing(5);
    leftButtonsLayout->addWidget(scanButton);
    leftButtonsLayout->addWidget(pauseButton);

    QWidget* rightButtons = new QWidget();
    QVBoxLayout* rightButtonsLayout = new QVBoxLayout(rightButtons);
    rightButtonsLayout->setSpacing(5);
    rightButtonsLayout->addWidget(retryButton);
    rightButtonsLayout->addWidget(metricsButton);

    QWidget* buttonRow = new QWidget();
    QHBoxLayout* buttonRowLayout = new QHBoxLayout(buttonRow);
    buttonRowLayout->setContentsMargins(0, 0, 0, 0);
    buttonRowLayout->setSpacing(20);
    buttonRowLayout->addWidget(leftButtons);
    buttonRowLayout->addWidget(rightButtons);

    // =====================================================
    // BOTTOM RIGHT: GOAL PANEL (NO HARDCODED PREVIEWS)
    // =====================================================
    QWidget* goalPanel = new QWidget();
    QVBoxLayout* goalLayout = new QVBoxLayout(goalPanel);

    QLabel* goalTitle = new QLabel("Goal");
    goalTitle->setAlignment(Qt::AlignCenter);

    goalLayout->addWidget(goalTitle);

    // (optional future: dynamic previews go here)
    // goalLayout->addWidget(previewContainer);

    // =====================================================
    // BOTTOM CONTAINER
    // =====================================================
    QWidget* bottomContainer = new QWidget();
    QHBoxLayout* bottomLayout = new QHBoxLayout(bottomContainer);

    bottomLayout->addWidget(buttonRow);
    bottomLayout->addWidget(goalPanel);

    // =====================================================
    // MAIN GRID LAYOUT
    // =====================================================
    layout->addWidget(cubeMain, 1, 0);
    layout->addWidget(solverPanel, 1, 1);
    layout->addWidget(bottomContainer, 2, 0, 1, 2);

    layout->setRowStretch(0, 0);
    layout->setRowStretch(1, 3);
    layout->setRowStretch(2, 1);

    layout->setColumnStretch(0, 1);
    layout->setColumnStretch(1, 1);
}

// =====================================================
// CAMERA SCREEN
// =====================================================
void MainWindow::setupCameraScreen()
{
    cameraScreen = new QWidget();
    QVBoxLayout* layout = new QVBoxLayout(cameraScreen);

    // ===== TOP BAR =====
    backButton = new QPushButton("Back");
    scanFaceButton = new QPushButton("Scan Face");

    backButton->setObjectName("backButton");
    scanFaceButton->setObjectName("scanFaceButton");

    QWidget* topBar = new QWidget();
    QHBoxLayout* topLayout = new QHBoxLayout(topBar);

    topLayout->addWidget(backButton);
    topLayout->addWidget(scanFaceButton);

    // ===== CONTENT =====
    cameraWidget = new CameraWidget(cameraScreen);
    grid = new CubeNet(controller->getCube(), cameraScreen);

    QWidget* content = new QWidget();
    QHBoxLayout* contentLayout = new QHBoxLayout(content);

    contentLayout->addWidget(cameraWidget, 3);
    contentLayout->addWidget(grid, 2);

    // ===== INSTRUCTION =====
    scanInstruction = new QLabel("Scan cube face");

    // ===== LAYOUT =====
    layout->addWidget(topBar);
    layout->addWidget(content);
    layout->addWidget(scanInstruction);
}

// =====================================================
// SCAN LOGIC
// =====================================================
void MainWindow::handleScan()
{
    auto face = cameraWidget->captureFace();
    auto result = controller->processScan(face);

    if (!result.success)
    {
        QMessageBox::warning(
            this,
            "Scan Error",
            QString::fromStdString(result.message)
        );
        return;
    }

    grid->update();

    if (result.finished)
    {
        stageBar->setValue(result.stageValue);
        solverOutputLabel->setText(
            QString::fromStdString(result.solutionText)
        );

        stack->setCurrentWidget(mainScreen);
    }
}

// =====================================================
// RESET
// =====================================================
void MainWindow::resetScan()
{
    currentFace = 0;
    controller->resetCube();
    grid->update();
}

// =====================================================
// CONNECTIONS
// =====================================================
void MainWindow::setupConnections()
{
    connect(newButton, &QPushButton::clicked, this, [this]()
        {
            Database::getInstance().resetSession();
            resetScan();
            stack->setCurrentWidget(mainScreen);
        });

    connect(exitButton, &QPushButton::clicked, qApp, &QApplication::quit);

    connect(scanButton, &QPushButton::clicked, this, [this]()
        {
            controller->resetCube();
            currentFace = 0;
            stack->setCurrentWidget(cameraScreen);
            cameraWidget->startCamera();
        });

    connect(backButton, &QPushButton::clicked, this, [this]()
        {
            cameraWidget->stopCamera();
            stack->setCurrentWidget(mainScreen);
        });

    connect(scanFaceButton, &QPushButton::clicked, this, [this]()
        {
            handleScan();
        });

    connect(pauseButton, &QPushButton::clicked, this, [this]()
        {
            stack->setCurrentWidget(startScreen);
        });

    connect(continueButton, &QPushButton::clicked, this, [this]()
        {
            int face = 0;
            std::string state;

            if (Database::getInstance().loadSession(face, state))
            {
                controller->loadState(state);
                grid->update();
            }

            stack->setCurrentWidget(mainScreen);
        });
}