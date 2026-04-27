#pragma once

#include <QMainWindow>
#include <QStringList>
#include <QLineEdit>
#include "../../app/AppController.h"
#include <vector>
#include <QPixmap>


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
    QWidget* nameScreen;
    QLineEdit* nameInput;
    QPushButton* startButton;
    QLabel* userLabel;
    QLabel* guidanceLabel;
    bool solverMode = false;
    QPushButton* retryButton;

    int currentStage = 0;
    std::string currentInstruction;


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
// PREVIEW IMAGES
// =========================
    QLabel* preview1 = nullptr;
    QLabel* preview2 = nullptr;
    QLabel* preview3 = nullptr;
    QLabel* previewLabel = nullptr;

    // =========================
    // SETUP
    // =========================
    void setupStartScreen();
    void setupNameScreen();
    void setupMainScreen();
    void setupCameraScreen();
    void setupConnections();

    void updatePreviews(Stage stage);

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