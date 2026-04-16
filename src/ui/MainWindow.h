#pragma once

#include <QMainWindow>
#include <vector>
#include "../core/domain/Cube.h"

class QStackedWidget;
class QWidget;
class QPushButton;
class CameraWidget;
class cubeView;
class CubeNet;

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow();
    ~MainWindow();

private:
    // =========================
    // UI STACK
    // =========================
    QStackedWidget* stack;

    // Screens
    QWidget* startScreen;
    QWidget* mainScreen;
    QWidget* cameraScreen;

    // =========================
    // WIDGETS
    // =========================
    CameraWidget* cameraWidget;
    cubeView* cubeStart;
    cubeView* cubeMain;
    CubeNet* grid;

    // Buttons
    QPushButton* newButton;
    QPushButton* continueButton;
    QPushButton* settingsButton;
    QPushButton* exitButton;

    QPushButton* scanButton;
    QPushButton* backButtonmain;
    QPushButton* backButton;
    QPushButton* scanFaceButton;



    // =========================
    // STATE
    // =========================
    int currentFace;

    // =========================
    // SETUP METHODS
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
    bool isValidFace(const std::array<Color, 9>& face);
};