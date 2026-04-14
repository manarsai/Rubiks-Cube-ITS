#include "CubeNet.h"
#include <QPainter>

CubeNet::CubeNet(QWidget* parent)
    : QWidget(parent)
{
    setMinimumSize(300, 300);
}

void CubeNet::paintEvent(QPaintEvent*)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    int w = width();
    int h = height();

    int faceSize = std::min(w, h) / 4;
    int cellSize = faceSize / 3;

    int centerX = w / 2;
    int centerY = h / 2;

    painter.setPen(QPen(Qt::black, 2));

    auto drawFace = [&](int startX, int startY) {
        for (int r = 0; r < 3; r++) {
            for (int c = 0; c < 3; c++) {

                QRect rect(
                    startX + c * cellSize,
                    startY + r * cellSize,
                    cellSize,
                    cellSize
                );

                painter.drawRect(rect);
            }
        }
        };

    // NEW LAYOUT (4 vertical + 2 sides in middle row)

    int originX = centerX - faceSize / 2;
    int originY = centerY - faceSize * 2;

    auto x = [&](int col) {
        return originX + col * faceSize;
        };

    auto y = [&](int row) {
        return originY + row * faceSize;
        };

    // TOP
    drawFace(x(0), y(0));

    // SECOND
    drawFace(x(0), y(1));

    // MIDDLE ROW (LEFT + CENTER + RIGHT)
    drawFace(x(-1), y(2)); // LEFT
    drawFace(x(0), y(2)); // CENTER
    drawFace(x(1), y(2)); // RIGHT

    // BOTTOM
    drawFace(x(0), y(3));
}