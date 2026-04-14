#pragma once
#include "../vision/Vision.h"
#include <QWidget>
#include <QLabel>
#include <QTimer>

class CameraScreen : public QWidget {
    Q_OBJECT

public:
    CameraScreen(QWidget* parent = nullptr);
    ~CameraScreen();

    void startCamera();
    void stopCamera();

private:
    QLabel* imageLabel;   // ? ADD THIS
    QTimer* timer;        // (you probably already have this)
    Vision* vision;
};