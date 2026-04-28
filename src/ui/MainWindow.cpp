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
#include <QShortcut>
#include <src/visual/StagePreviews.h>
#include <vector>

// CONSTRUCTOR
MainWindow::MainWindow()
{
    controller = new AppController();

    stack = new QStackedWidget(this);
    setCentralWidget(stack);

    setupStartScreen();
    setupNameScreen();
    setupMainScreen();
    setupCameraScreen();
    setupConnections();

    Styles::apply();

    stack->addWidget(startScreen);
    stack->addWidget(nameScreen);
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
    Database::instance().close();
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

    cubeStart->testMoves();

    cubeLayout->addStretch();
    cubeLayout->addWidget(cubeStart, 0, Qt::AlignCenter);
    cubeLayout->addStretch();

    // ===== LAYOUT =====
    root->addStretch();
    root->addWidget(nav);
    root->addWidget(cubeContainer);
    root->addStretch();
}


void MainWindow::setupNameScreen()
{
    nameScreen = new QWidget();
    QVBoxLayout* layout = new QVBoxLayout(nameScreen);

    QLabel* label = new QLabel("Enter your name:");
    label->setAlignment(Qt::AlignCenter);

    nameInput = new QLineEdit();
    nameInput->setPlaceholderText("Your name...");

    startButton = new QPushButton("Start");

    layout->addStretch();
    layout->addWidget(label);
    layout->addWidget(nameInput);
    layout->addWidget(startButton);
    layout->addStretch();
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
    stageBar->setFormat("Scan your cube to begin");

    layout->addWidget(stageBar, 0, 0, 1, 2);

    // =====================================================
    // LEFT: CUBE VIEW (FIXED SIZE CONTAINER)
    // =====================================================
    cubeMain = new cubeView(controller->getCube(), mainScreen);

    QWidget* cubeContainer = new QWidget();
    QVBoxLayout* cubeLayout = new QVBoxLayout(cubeContainer);
    cubeLayout->setContentsMargins(0, 0, 0, 0);

    cubeMain->setMinimumSize(200, 200);
    cubeMain->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    cubeLayout->addWidget(cubeMain);

    // =====================================================
    // RIGHT: GUIDANCE + SOLVER PANEL
    // =====================================================
    QWidget* rightPanel = new QWidget();
    QVBoxLayout* rightLayout = new QVBoxLayout(rightPanel);

    QWidget* guidancePanel = new QWidget();
    guidancePanel->setObjectName("guidanceBox");

    QVBoxLayout* panelLayout = new QVBoxLayout(guidancePanel);

    QLabel* guidanceTitle = new QLabel("Guidance");
    guidanceTitle->setAlignment(Qt::AlignCenter);

    guidanceLabel = new QLabel(
        "Welcome!\n\n"
        "1. Click 'Scan Cube'\n"
        "2. Scan all 6 faces\n"
        "3. Follow step-by-step guidance\n\n"
        "Goal: Solve using CFOP"
    );
    guidanceLabel->setWordWrap(true);
    guidanceLabel->setAlignment(Qt::AlignTop);

    QLabel* solverTitle = new QLabel("Solver Output");
    solverTitle->setAlignment(Qt::AlignCenter);

    solverOutputLabel = new QLabel("No solution yet");
    solverOutputLabel->setWordWrap(true);
    solverOutputLabel->setAlignment(Qt::AlignTop);

    // FONT STYLING
    QFont titleFont;
    titleFont.setPointSize(16);
    titleFont.setBold(true);

    guidanceTitle->setFont(titleFont);
    solverTitle->setFont(titleFont);

    QFont bodyFont;
    bodyFont.setPointSize(13);

    guidanceLabel->setFont(bodyFont);
    solverOutputLabel->setFont(bodyFont);

    panelLayout->addWidget(guidanceTitle);
    panelLayout->addWidget(guidanceLabel, 1);
    panelLayout->addSpacing(10);
    panelLayout->addWidget(solverTitle);
    panelLayout->addWidget(solverOutputLabel, 1);

    rightLayout->addWidget(guidancePanel);

    // =====================================================
    // BUTTON PANEL
    // =====================================================
    scanButton = new QPushButton("Scan Cube");
    pauseButton = new QPushButton("Pause");

    retryButton = new QPushButton("Retry Stage");
    QPushButton* metricsButton = new QPushButton("Metrics");

    scanButton->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    pauseButton->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

    retryButton->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    metricsButton->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);



    QVBoxLayout* leftButtons = new QVBoxLayout();
    leftButtons->addWidget(scanButton);
    leftButtons->addWidget(pauseButton);

    QVBoxLayout* rightButtons = new QVBoxLayout();
    rightButtons->addWidget(retryButton);
    rightButtons->addWidget(metricsButton);

    QHBoxLayout* buttonRow = new QHBoxLayout();
    buttonRow->addLayout(leftButtons);
    buttonRow->addLayout(rightButtons);
    buttonRow->setSpacing(10);

    leftButtons->setSpacing(10);
    rightButtons->setSpacing(10);

    // =====================================================
    // USER LABEL
    // =====================================================
    userLabel = new QLabel();
    userLabel->setObjectName("userLabel");

    // =====================================================
    // GOAL PANEL (BOTTOM CENTER-RIGHT)
    // =====================================================
    QWidget* goalPanel = new QWidget();
    QVBoxLayout* goalLayout = new QVBoxLayout(goalPanel);

    // =====================================================
    // PREVIEW PANEL (BOTTOM RIGHT - FIXED)
    // =====================================================
    QWidget* previewPanel = new QWidget();
    QHBoxLayout* previewLayout = new QHBoxLayout(previewPanel);
    previewPanel->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);

    previewLayout->setContentsMargins(0, 0, 0, 0);

    preview1 = new QLabel();
    preview2 = new QLabel();
    preview3 = new QLabel();

    for (QLabel* p : { preview1, preview2, preview3 })
    {
        p->setFixedSize(100, 100);
        p->setScaledContents(true);
    }

    previewLayout->addWidget(preview1);
    previewLayout->addWidget(preview2);
    previewLayout->addWidget(preview3);

    // force it to hug the right side
    previewLayout->addStretch();

    // =====================================================
    // RIGHT GROUP (goal + previews stacked properly)
    // =====================================================
    QWidget* rightBottomGroup = new QWidget();
    QVBoxLayout* rightGroupLayout = new QVBoxLayout(rightBottomGroup);

    rightGroupLayout->addWidget(goalPanel);
    rightGroupLayout->addWidget(previewPanel);

    // =====================================================
    // BOTTOM CONTAINER
    // =====================================================
    QWidget* bottomContainer = new QWidget();
    QHBoxLayout* bottomLayout = new QHBoxLayout(bottomContainer);

    leftButtons->setContentsMargins(0, 0, 0, 0);
    rightButtons->setContentsMargins(0, 0, 0, 0);
    rightGroupLayout->setContentsMargins(0, 0, 0, 0);
    bottomLayout->setContentsMargins(10, 5, 10, 5);
    bottomLayout->setSpacing(20);

    // LEFT
    bottomLayout->addWidget(userLabel, 1);

    // CENTER
    bottomLayout->addLayout(buttonRow);
    bottomLayout->addStretch(1);

    // RIGHT (stable block)
    bottomLayout->addWidget(rightBottomGroup, 2);

    bottomLayout->addLayout(buttonRow, 0);
    bottomLayout->setAlignment(buttonRow, Qt::AlignBottom);

    // =====================================================
    // MAIN GRID
    // =====================================================
    layout->addWidget(cubeContainer, 1, 0);
    layout->addWidget(rightPanel, 1, 1);
    layout->addWidget(bottomContainer, 2, 0, 1, 2);
    layout->setRowStretch(1, 3);
    layout->setRowStretch(2, 1);

    layout->setColumnStretch(0, 3);
    layout->setColumnStretch(1, 2);
}

// =====================================================
// CAMERA SCREEN
// =====================================================
void MainWindow::setupCameraScreen()
{
    cameraScreen = new QWidget();
    QVBoxLayout* layout = new QVBoxLayout(cameraScreen);


    // ===== CONTENT =====
    cameraWidget = new CameraWidget(cameraScreen);
    grid = new CubeNet(controller->getCube(), cameraScreen);

    QWidget* content = new QWidget();
    QHBoxLayout* contentLayout = new QHBoxLayout(content);

    contentLayout->addWidget(cameraWidget, 3);
    contentLayout->addWidget(grid, 2);

    // ===== INSTRUCTION =====
    scanInstruction = new QLabel("Press SPACE to scan face");
    scanInstruction->setObjectName("scanInstruction");
    scanInstruction->setAlignment(Qt::AlignCenter);

    // ===== BOTTOM BAR =====
    backButton = new QPushButton("Back");
    backButton->setObjectName("backButton");

    QWidget* bottomBar = new QWidget();
    QHBoxLayout* bottomLayout = new QHBoxLayout(bottomBar);

    bottomLayout->addWidget(backButton);
    bottomLayout->addStretch();  // keeps both anchored to bottom-left

    // ===== LAYOUT =====
    layout->addWidget(content);
    layout->addWidget(scanInstruction);
    layout->addWidget(bottomBar);

    cameraScreen->setFocusPolicy(Qt::StrongFocus);
    cameraScreen->setFocus();
}


// SCAN LOGIC
// =====================================================
// SCAN LOGIC
// =====================================================
void MainWindow::handleScan()
{
    std::cout << "HANDLE SCAN CALLED\n";

    auto face = cameraWidget->captureFace();

    // =====================================================
    // 1. PROCESS SCAN
    // =====================================================
    auto result = controller->processScan(face);

    // ONLY RUN IF VALID SCAN
    if (!result.success)
    {
        scanInstruction->setText(QString::fromStdString(result.message));
        return;
    }

    // increment AFTER successful scan
    if (result.success)
    {
        currentFace++;

        Session s;
        s.face = currentFace;
        s.cubeState = controller->getCube().serialize();
        s.stage = result.stageValue;
        s.instruction = result.message;
        s.solverMode = solverMode;

        Database::instance().saveSession(s);
    }



    // =====================================================
    // 5. PARTIAL SCANS (faces 1–5)
    // =====================================================
    if (!result.finished)
    {
        scanInstruction->setText("Face recorded. Continue scanning...");

        grid->update();
        QCoreApplication::processEvents();
        return;
    }

    // =====================================================
    // 6. FINAL FACE
    // =====================================================
    grid->update();
    QCoreApplication::processEvents();

    Stage stage = static_cast<Stage>(result.stageValue);

    QString stageName;
    switch (stage)
    {
    case Stage::WHITE_CROSS: stageName = "White Cross"; break;
    case Stage::F2L:         stageName = "F2L"; break;
    case Stage::OLL:         stageName = "OLL"; break;
    case Stage::PLL:         stageName = "PLL"; break;
    case Stage::COMPLETE:    stageName = "Complete"; break;
    default:                 stageName = "Scrambled";
    }

    // =====================================================
    // 7. UPDATE UI
    // =====================================================
    stageBar->setValue(result.stageValue);
    stageBar->setFormat(stageName + " (%v/%m)");

    updatePreviews(stage);

    scanInstruction->setText("Cube scanned successfully");

    if (solverMode)
    {
        std::vector<std::string> moves = result.moves;

        if (moves.empty())
        {
            solverOutputLabel->setText("Solver returned no moves.");
            guidanceLabel->setText("Solver failed to generate solution.");
            return;
        }

        QString moveText;
        for (const auto& m : moves)
            moveText += QString::fromStdString(m) + " ";

        solverOutputLabel->setText(moveText.trimmed());

        if (!result.message.empty())
            guidanceLabel->setText(QString::fromStdString(result.message));

        cubeMain->setMoves(moves);
    }
    else
    {
        guidanceLabel->setText(
            "Scan complete.\n\n"
            "Stage detected: " + stageName +
            "\n\nContinue solving manually."
        );
    }

    // =====================================================
    // 8. RESET FACE TRACKING
    // =====================================================
    currentFace = 0;

    // =====================================================
    // 9. RETURN TO MAIN
    // =====================================================
    cameraWidget->stopCamera();

    cubeMain->update();
    cubeStart->update();

    stack->setCurrentWidget(mainScreen);
}
// =====================================================
// RESET
// =====================================================
void MainWindow::resetScan()
{
    controller->resetScan();   // you should add this if not already
    controller->resetCube();

    grid->update();
    cubeMain->update();
    cubeStart->update();
}


void MainWindow::updatePreviews(Stage stage)
{
    auto previews = StagePreviews::getStagePreviews(stage);

    QLabel* labels[] = { preview1, preview2, preview3 };

    int i = 0;
    for (auto it = previews.begin(); it != previews.end() && i < 3; ++it, ++i)
    {
        labels[i]->setPixmap(*it);
        labels[i]->show();
    }

    // hide unused labels
    for (; i < 3; ++i)
    {
        labels[i]->clear();
        labels[i]->hide();
    }
}


// =====================================================
// CONNECTIONS
// =====================================================
void MainWindow::setupConnections()
{
    connect(newButton, &QPushButton::clicked, this, [this]()
        {
            Database::instance().resetSession();
            resetScan();
            stack->setCurrentWidget(nameScreen);
        });


    connect(startButton, &QPushButton::clicked, this, [this]()
        {
            try
            {
                QString name = nameInput->text().trimmed();

                if (name.isEmpty())
                {
                    QMessageBox::warning(this, "Invalid Name", "Please enter a name.");
                    return;
                }

                Database::instance().setUserName(name.toStdString());

                userLabel->setText(name);

                stack->setCurrentWidget(mainScreen);
            }
            catch (const std::exception& e)
            {
                QMessageBox::critical(this, "Database Error", e.what());
            }
        });

    connect(exitButton, &QPushButton::clicked, qApp, &QApplication::quit);

    connect(scanButton, &QPushButton::clicked, this, [this]()
        {
            solverMode = false;  // ? tutoring mode

            controller->resetScan();
            controller->resetCube();

            guidanceLabel->setText(
                "Scanning mode:\n\n"
                "• Show one face\n"
                "• Press SPACE to scan\n"
                "• Repeat for all 6 faces"
            );

            stack->setCurrentWidget(cameraScreen);
            cameraWidget->startCamera();
        });

    connect(retryButton, &QPushButton::clicked, this, [this]()
        {
            solverMode = true;  // ? solver mode

            controller->resetScan();  // optional depending on your logic

            guidanceLabel->setText(
                "Retrying stage:\n\n"
                "• Scan cube again\n"
                "• Solver will generate moves"
            );

            stack->setCurrentWidget(cameraScreen);
            cameraWidget->startCamera();
        });

    connect(backButton, &QPushButton::clicked, this, [this]()
        {
            cameraWidget->stopCamera();
            stack->setCurrentWidget(mainScreen);
        });

    QShortcut* spaceShortcut = new QShortcut(QKeySequence(Qt::Key_Space), cameraScreen);

    connect(spaceShortcut, &QShortcut::activated, this, [this]()
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
            int stage = 0;
            std::string instruction;
            bool solver = false;

            bool ok = Database::instance().loadSession(
                face,
                state,
                stage,
                instruction,
                solver
            );

            if (!ok)
            {
                QMessageBox::information(
                    this,
                    "No Saved Game",
                    "No previous session found. Start a new game first."
                );
                return;
            }

            // restore cube
            controller->loadState(state);

            currentFace = face;
            currentStage = stage;
            solverMode = solver;

            stageBar->setValue(stage);

            if (!instruction.empty())
                guidanceLabel->setText(QString::fromStdString(instruction));
            else
                guidanceLabel->setText("Resumed session.");

            grid->update();
            cubeMain->update();
            cubeStart->update();

            stack->setCurrentWidget(mainScreen);
        });

    connect(stack, &QStackedWidget::currentChanged, this, [this](int)
        {
            if (stack->currentWidget() == mainScreen)
            {
                int face = 0;
                std::string state;
                int stage = 0;
                std::string instruction;
                bool loadedSolverMode = false;

                // Load session
                if (Database::instance().loadSession(face, state, stage, instruction, loadedSolverMode))
                {
                    stageBar->setValue(stage);

                    if (!instruction.empty())
                        guidanceLabel->setText(QString::fromStdString(instruction));
                }

                // =========================
                // USER NAME (FIXED)
                // =========================
                auto nameOpt = Database::instance().getUserName();

                std::string name = "Guest";

                if (nameOpt.has_value())
                {
                    name = nameOpt.value();
                }

                userLabel->setText(QString::fromStdString(name));
            }
        });

}

