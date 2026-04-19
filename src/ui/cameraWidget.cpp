#include "CameraWidget.h"
#include <QKeyEvent>
#include <QPushButton>
#include "Camerascreen.h"


CameraWidget::CameraWidget(QWidget* parent)
    : QLabel(parent), vision(nullptr)
{
    setAlignment(Qt::AlignCenter);
    setFixedSize(500, 350);

    // =========================
    // Timer (NO camera start yet)
    // =========================
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &CameraWidget::updateFrame);
}

void CameraWidget::updateFrame()
{
    if (!running || !vision)
        return;

    cv::Mat frame = vision->getProcessedFrame();
    if (frame.empty())
        return;

    if (vision)
    {
        auto face = vision->getFaceColours();

        bool valid = true;
        for (auto c : face)
        {
            if (c == Colour::WHITE) // or better: introduce UNKNOWN
            {
                valid = false;
                break;
            }
        }

        if (valid)
            emit faceScanned(face);

        // Only emit if valid (9 detected)
        if (!face.empty())
        {
            emit faceScanned(face);
        }
    }

    cv::cvtColor(frame, frame, cv::COLOR_BGR2RGB);

    QImage img(frame.data, frame.cols, frame.rows,
        frame.step, QImage::Format_RGB888);

    setPixmap(QPixmap::fromImage(img));
}

void CameraWidget::startCamera()
{
    if (!vision)
        vision = new Vision(0);

    running = true;
    timer->start(30);
}

void CameraWidget::stopCamera()
{
    running = false;

    timer->stop();

    if (vision) {
        delete vision;
        vision = nullptr;
    }

    clear(); // QLabel itself
}

std::array<Colour, 9> CameraWidget::captureFace()
{
    if (!vision)
        return {};

    return vision->getFaceColours();
}
