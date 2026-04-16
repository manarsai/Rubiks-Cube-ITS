#pragma once

#include <QLabel>
#include <QTimer>
#include <opencv2/opencv.hpp>

#include "../vision/vision.h"


class CameraWidget : public QLabel {
    Q_OBJECT

public:
    explicit CameraWidget(QWidget* parent = nullptr);

    void startCamera();
    void stopCamera();

    std::array<Color, 9> captureFace();

signals:
    void backRequested();

signals:
    void faceScanned(const std::array<Color, 9>& colors);

private slots:
    void updateFrame();

private:
    QTimer* timer;
    Vision* vision;
    bool running = false;
};