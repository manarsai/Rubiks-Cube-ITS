#pragma once

#include <QMainWindow>

class QStackedWidget;
class QWidget;
class CameraWidget;
class cubeView;   // ? correct place (forward declaration)

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow();

private:
    QStackedWidget* stack;

    QWidget* startScreen;
    QWidget* mainScreen;
    CameraWidget* cameraWidget;

    cubeView* cubeStart;   // ? member pointer
    cubeView* cubeMain;
};