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
#include "../visual/StagePreviews.h"
#include <qprogressbar.h>





// CONSTRUCTOR
MainWindow::MainWindow()
{
    // =========================
    // DATABASE INIT (ONCE)
    // =========================
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

    // =========================
    // CUBE (LOAD EARLY STATE)
    // =========================
    cube = new Cube();

    int face = 0;
    std::string state;

    // Try to restore last session immediately
    if (Database::getInstance().loadSession(face, state))
    {
        cube->deserialize(state);
        currentFace = face;
        qDebug() << "? SESSION LOADED AT STARTUP";
    }
    else
    {
        cube->reset();
        currentFace = 0;
        qDebug() << "? NO SESSION FOUND - USING SOLVED CUBE";
    }

    // =========================
    // STACK SETUP
    // =========================
    stack = new QStackedWidget(this);
    setCentralWidget(stack);

    // =========================
    // UI SETUP (NOW USES LOADED CUBE)
    // =========================
    setupStartScreen();
    setupMainScreen();
    setupCameraScreen();
    setupConnections();

    Styles::apply();

    // =========================
    // ADD SCREENS
    // =========================
    stack->addWidget(startScreen);
    stack->addWidget(mainScreen);
    stack->addWidget(cameraScreen);

    stack->setCurrentWidget(startScreen);

    // =========================
    // FORCE INITIAL SYNC
    // =========================
    if (cubeStart) cubeStart->update();
    if (cubeMain) cubeMain->update();
    if (grid) grid->update();

    // =========================
    // FULLSCREEN
    // =========================
    showFullScreen();
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

    cubeStart = new cubeView(*cube, startScreen);
    cubeStart->setFixedSize(300, 300);

    root->addStretch();
    root->addWidget(nav);
    root->addWidget(cubeStart);
    root->addStretch();
}


void MainWindow::setupMainScreen()
{
    mainScreen = new QWidget();

    QGridLayout* layout = new QGridLayout(mainScreen);

    // =========================
    // STAGE PROGRESS BAR (NEW TOP UI)
    // =========================
    QProgressBar* stageBar = new QProgressBar(mainScreen);
    stageBar->setObjectName("stageBar");
    stageBar->setRange(0, 5); // Cross, F2L, OLL, PLL
    stageBar->setValue(1);    // current stage (example)
    stageBar->setTextVisible(true);
    stageBar->setFormat("Stage %v / %m");
    stageBar->setFixedHeight(40);

    layout->addWidget(stageBar, 0, 0, 1, 2);

    // =========================
    // TOP LEFT (CUBE)
    // =========================
    cubeMain = new cubeView(*cube, mainScreen);

    QWidget* topRight = new QWidget();

    // =========================
    // BUTTONS
    // =========================
    scanButton = new QPushButton("Scan Cube");
    backButtonmain = new QPushButton("Pause");

    scanButton->setObjectName("scanButton");
    backButtonmain->setObjectName("backButtonMain");

    QPushButton* btn3 = new QPushButton("Retry Stage");
    QPushButton* btn4 = new QPushButton("Metrics");

    btn3->setObjectName("RetryButton");
    btn4->setObjectName("MetricButton");

    QWidget* bottomLeft = new QWidget();
    QVBoxLayout* leftLayout = new QVBoxLayout(bottomLeft);
    leftLayout->setSpacing(5);
    leftLayout->addWidget(scanButton);
    leftLayout->addWidget(backButtonmain);

    QWidget* rightCol = new QWidget();
    QVBoxLayout* rightLayoutCol = new QVBoxLayout(rightCol);
    rightLayoutCol->setSpacing(5);
    rightLayoutCol->addWidget(btn3);
    rightLayoutCol->addWidget(btn4);

    QWidget* bottomButtonsRow = new QWidget();
    QHBoxLayout* bottomButtonsLayout = new QHBoxLayout(bottomButtonsRow);
    bottomButtonsLayout->setContentsMargins(0, 0, 0, 0);
    bottomButtonsLayout->setSpacing(20);
    bottomButtonsLayout->addWidget(bottomLeft);
    bottomButtonsLayout->addWidget(rightCol);

    // =========================
    // PREVIEWS
    // =========================
    QWidget* previewContainer = new QWidget();
    QHBoxLayout* previewLayout = new QHBoxLayout(previewContainer);
    previewLayout->setSpacing(10);

    QStringList stages = {
        "WWWWWWWWW",
        "GGGGGGGGG",
        "RRRRRRRRR"
    };

    for (const QString& state : stages)
    {
        QLabel* preview = new QLabel();
        preview->setPixmap(StagePreviews::createFaceImage(state));
        preview->setFixedSize(100, 100);
        preview->setScaledContents(true);
        preview->setAlignment(Qt::AlignCenter);
        previewLayout->addWidget(preview);
    }

    QWidget* bottomRight = new QWidget();
    QVBoxLayout* rightLayout = new QVBoxLayout(bottomRight);

    QLabel* title = new QLabel("Goal");
    title->setAlignment(Qt::AlignCenter);

    rightLayout->addWidget(title);
    rightLayout->addWidget(previewContainer);

    // =========================
    // BOTTOM CONTAINER
    // =========================
    QWidget* bottomContainer = new QWidget();
    QVBoxLayout* bottomLayout = new QVBoxLayout(bottomContainer);
    bottomLayout->setSpacing(0);
    bottomLayout->setContentsMargins(0, 0, 0, 0);

    QWidget* bottomRow = new QWidget();
    QHBoxLayout* bottomRowLayout = new QHBoxLayout(bottomRow);
    //bottomRowLayout->setSpacing(20);
    bottomRowLayout->setContentsMargins(0, 0, 0, 0);

    bottomLayout->addStretch();
    bottomLayout->addStretch();
    bottomLayout->addStretch();
    bottomRowLayout->addWidget(bottomButtonsRow);
    bottomRowLayout->addWidget(bottomRight);
       // pushes content UP

    bottomLayout->addWidget(bottomRow);


    // =========================
    // GRID PLACEMENT (SHIFTED DOWN)
    // =========================

    layout->addWidget(cubeMain, 1, 0);
    layout->addWidget(topRight, 1, 1);
    layout->addWidget(bottomContainer, 2, 0, 1, 2);

    // =========================
    // SIZE CONTROL
    // =========================
    layout->setRowStretch(0, 0); // stage bar
    layout->setRowStretch(1, 3); // main content
    layout->setRowStretch(2, 1); // bottom area

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

    // =========================
    // REMOVE GLOBAL SPACING (IMPORTANT FIX)
    // =========================
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(5);

    topBar->setContentsMargins(0, 0, 0, 0);
    content->setContentsMargins(0, 0, 0, 0);
    content->setSpacing(10);

    // =========================
    // SCAN ORDER
    // =========================
    scanSteps = {
        "Scan UP face (Yellow)",
        "Scan LEFT face (Blue)",
        "Scan FRONT face (Red)",
        "Scan RIGHT face (Green)",
        "Scan BACK face (Orange)",
        "Scan DOWN face (White)"
    };

    currentFace = 0;

    // =========================
    // INSTRUCTION LABEL
    // =========================
    scanInstruction = new QLabel(scanSteps[0]);
    scanInstruction->setAlignment(Qt::AlignCenter);
    scanInstruction->setObjectName("scanInstruction");

    // ?? REMOVE INTERNAL LABEL SPACING
    scanInstruction->setContentsMargins(0, 0, 0, 0);

    QFont font = scanInstruction->font();
    font.setPointSize(10);
    font.setBold(true);
    scanInstruction->setFont(font);

    // =========================
    // BUTTONS
    // =========================
    backButton = new QPushButton("Back");
    scanFaceButton = new QPushButton("Scan Face");

    backButton->setObjectName("backButton");
    scanFaceButton->setObjectName("scanFaceButton");

    topBar->addWidget(backButton);
    topBar->addWidget(scanFaceButton);

    // =========================
    // CAMERA + GRID
    // =========================
    cameraWidget = new CameraWidget(cameraScreen);
    grid = new CubeNet(*cube, cameraScreen);

    content->addWidget(cameraWidget, 3);
    content->addWidget(grid, 2);

    // =========================
    // FINAL LAYOUT ORDER (CLEAN STACK)
    // =========================
    mainLayout->addLayout(topBar);
    mainLayout->addLayout(content);
    mainLayout->addWidget(scanInstruction); // ?? now tightly under camera/grid
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

