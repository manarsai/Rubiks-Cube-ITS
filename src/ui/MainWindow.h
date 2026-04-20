#pragma once

#include <QMainWindow>
#include <array>
#include "../core/domain/Cube.h"

// Forward declarations
class QStackedWidget;
class QWidget;
class QPushButton;
class CameraWidget;
class cubeView;
class CubeNet;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow();
    ~MainWindow();

private:
    // =========================
    // CORE STATE (SINGLE SOURCE OF TRUTH)
    // =========================
    Cube* cube = nullptr;   // ? safer initialization

    int currentFace = 0;    // ? safe default

    // =========================
    // UI STACK
    // =========================
    QStackedWidget* stack = nullptr;

    QWidget* startScreen = nullptr;
    QWidget* mainScreen = nullptr;
    QWidget* cameraScreen = nullptr;

    // =========================
    // WIDGETS
    // =========================
    CameraWidget* cameraWidget = nullptr;
    cubeView* cubeStart = nullptr;
    cubeView* cubeMain = nullptr;
    CubeNet* grid = nullptr;

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