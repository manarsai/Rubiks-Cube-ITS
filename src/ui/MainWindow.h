#pragma once

#include <QMainWindow>
#include <array>
#include "../core/domain/Cube.h"
#include <QLabel>

// =========================
// FORWARD DECLARATIONS
// =========================
class QStackedWidget;
class QWidget;
class QPushButton;
class CameraWidget;
class cubeView;
class CubeNet;
class ProgressWheel;   // ? Progress wheel added

// =========================
// MAIN WINDOW
// =========================
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow();
    ~MainWindow();

private:
    // =========================
    // CORE STATE
    // =========================
    Cube* cube = nullptr;
    int currentFace = 0;

    // =========================
    // UI STACK
    // =========================
    QStackedWidget* stack = nullptr;

    QWidget* startScreen = nullptr;
    QWidget* mainScreen = nullptr;
    QWidget* cameraScreen = nullptr;
    QLabel* scanInstruction;
    QStringList scanSteps;

    // =========================
    // WIDGETS
    // =========================
    CameraWidget* cameraWidget = nullptr;
    cubeView* cubeStart = nullptr;
    cubeView* cubeMain = nullptr;
    CubeNet* grid = nullptr;
    ProgressWheel* progressWheel = nullptr;   // ? progress wheel

    // =========================
    // BUTTONS
    // =========================
    QPushButton* newButton = nullptr;
    QPushButton* continueButton = nullptr;
    QPushButton* settingsButton = nullptr;
    QPushButton* exitButton = nullptr;

    QPushButton* scanButton = nullptr;
    QPushButton* backButtonmain = nullptr;
    QPushButton* backButton = nullptr;
    QPushButton* scanFaceButton = nullptr;

    // =========================
    // SETUP
    // =========================
    void setupStartScreen();
    void setupMainScreen();
    void setupCameraScreen();
    void setupConnections();
    void applyStyles();

    // =========================
    // LOGIC
    // =========================
    void handleScan();
    void resetScan();
    bool isValidFace(const std::array<Colour, 9>& face);
};