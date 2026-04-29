#pragma once

#include <QMainWindow>
#include <QLineEdit>
#include <QString>
#include <vector>
#include <string>

#include "../../app/AppController.h"
#include "../core/domain/Solver.h"   // ? FIXED

// =========================
// FORWARD DECLARATIONS
// =========================
class QStackedWidget;
class QWidget;
class QPushButton;
class QLabel;
class QProgressBar;

class CameraWidget;
class cubeView;
class CubeNet;
class AppController;

// =========================
// STAGE SYSTEM
// =========================
#include "../core/domain/StageDefinitions.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow();
    ~MainWindow();

private:
    // =========================
    // CORE
    // =========================
    AppController* controller = nullptr;
    Solver* solver = nullptr;

    QStackedWidget* stack = nullptr;

    QWidget* startScreen = nullptr;
    QWidget* nameScreen = nullptr;
    QWidget* mainScreen = nullptr;
    QWidget* cameraScreen = nullptr;

    // =========================
    // CUBE VIEWS
    // =========================
    cubeView* cubeStart = nullptr;
    cubeView* cubeMain = nullptr;
    CubeNet* grid = nullptr;

    // =========================
    // INPUT / LABELS
    // =========================
    QLineEdit* nameInput = nullptr;

    QLabel* userLabel = nullptr;
    QLabel* guidanceLabel = nullptr;
    QLabel* solverOutputLabel = nullptr;
    QLabel* scanInstruction = nullptr;

    // =========================
    // STATE
    // =========================
    int currentFace = 0;
    int currentStage = 0;

    std::string currentInstruction;
    std::vector<std::string> lastSolution;

    bool solverMode = false;

    // =========================
    // CAMERA
    // =========================
    CameraWidget* cameraWidget = nullptr;

    // =========================
    // UI ELEMENTS
    // =========================
    QProgressBar* stageBar = nullptr;

    // =========================
    // BUTTONS
    // =========================
    QPushButton* newButton = nullptr;
    QPushButton* continueButton = nullptr;
    QPushButton* exitButton = nullptr;

    QPushButton* startButton = nullptr;
    QPushButton* scanButton = nullptr;
    QPushButton* pauseButton = nullptr;
    QPushButton* retryButton = nullptr;
    QPushButton* backButton = nullptr;

    // =========================
    // PREVIEWS
    // =========================
    QLabel* preview1 = nullptr;
    QLabel* preview2 = nullptr;
    QLabel* preview3 = nullptr;

    // =========================
    // SETUP
    // =========================
    void setupStartScreen();
    void setupNameScreen();
    void setupMainScreen();
    void setupCameraScreen();
    void setupConnections();

    void updatePreviews(Stage stage);
    void completeScanAndSolve();

    // =========================
    // LOGIC
    // =========================
    void handleScan();
    void resetScan();
};