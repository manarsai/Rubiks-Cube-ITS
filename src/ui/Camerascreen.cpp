#include "CameraScreen.h"
#include <QVBoxLayout>
#include <QLabel>
#include <QTimer>

CameraScreen::CameraScreen(QWidget* parent)
    : QWidget(parent)
{
    QVBoxLayout* layout = new QVBoxLayout(this);



    imageLabel = new QLabel("Camera Feed");
    layout->addWidget(imageLabel);


    timer = new QTimer(this);
    vision = nullptr;

    connect(timer, &QTimer::timeout, this, [this]() {
        if (!vision) return;

        cv::Mat frame = vision->getProcessedFrame();
        if (frame.empty()) return;

        cv::cvtColor(frame, frame, cv::COLOR_BGR2RGB);

        QImage img(frame.data, frame.cols, frame.rows, frame.step, QImage::Format_RGB888);
        imageLabel->setPixmap(QPixmap::fromImage(img));
        });
}

CameraScreen::~CameraScreen() {
    stopCamera();
}

void CameraScreen::startCamera() {
    if (!vision)
        vision = new Vision(0);

    timer->start(30);
}

void CameraScreen::stopCamera() {
    timer->stop();

    if (vision) {
        delete vision;
        vision = nullptr;
    }
}