#pragma once

#include <QMainWindow>
#include <QStringList>

class QStackedWidget;
class QWidget;
class QPushButton;
class QLabel;
class QProgressBar;

class CameraWidget;
class cubeView;
class CubeNet;
class AppController;

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
    int currentFace = 0;

    QStackedWidget* stack = nullptr;

    QWidget* startScreen = nullptr;
    QWidget* mainScreen = nullptr;
    QWidget* cameraScreen = nullptr;

    // =========================
    // UI ELEMENTS
    // =========================
    cubeView* cubeStart = nullptr;
    cubeView* cubeMain = nullptr;
    CubeNet* grid = nullptr;

    CameraWidget* cameraWidget = nullptr;

    QLabel* solverOutputLabel = nullptr;
    QLabel* scanInstruction = nullptr;

    QProgressBar* stageBar = nullptr;

    // =========================
    // BUTTONS
    // =========================
    QPushButton* newButton = nullptr;
    QPushButton* continueButton = nullptr;
    QPushButton* exitButton = nullptr;

    QPushButton* scanButton = nullptr;
    QPushButton* pauseButton = nullptr;

    QPushButton* backButton = nullptr;
    QPushButton* scanFaceButton = nullptr;

    // =========================
    // SETUP
    // =========================
    void setupStartScreen();
    void setupMainScreen();
    void setupCameraScreen();
    void setupConnections();

    // =========================
    // UI BUILDERS
    // =========================
    QWidget* createNavPanel();
    QWidget* createCubePanel();

    // =========================
    // LOGIC
    // =========================
    void handleScan();
    void resetScan();
};