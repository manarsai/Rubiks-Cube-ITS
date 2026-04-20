#include "CubeNet.h"
#include <QPainter>
#include "../core/domain/Cube.h"

CubeNet::CubeNet(Cube& c, QWidget* parent)
    : QWidget(parent), cube(c)
{
    setMinimumSize(300, 300);
}

// ===============================
// PAINT
// ===============================
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

    auto drawFace = [&](int startX, int startY, int faceIndex)
        {
            // still valid — Cube already abstracts indexing
            auto face = cube.getFace(faceIndex);

            for (int r = 0; r < 3; r++)
            {
                for (int c = 0; c < 3; c++)
                {
                    QRect rect(
                        startX + c * cellSize,
                        startY + r * cellSize,
                        cellSize,
                        cellSize
                    );

                    Colour col = face[r * 3 + c];

                    QColor qcolor = Qt::gray;

                    switch (col)
                    {
                    case Colour::WHITE:  qcolor = Qt::white; break;
                    case Colour::RED:    qcolor = Qt::red; break;
                    case Colour::BLUE:   qcolor = Qt::blue; break;
                    case Colour::GREEN:  qcolor = Qt::green; break;
                    case Colour::YELLOW: qcolor = Qt::yellow; break;
                    case Colour::ORANGE: qcolor = QColor(255, 165, 0); break;
                    default: qcolor = Qt::gray; break;
                    }

                    painter.fillRect(rect, qcolor);
                    painter.drawRect(rect);
                }
            }
        };

    int originX = centerX - faceSize / 2;
    int originY = centerY - faceSize * 2;

    auto x = [&](int col) { return originX + col * faceSize; };
    auto y = [&](int row) { return originY + row * faceSize; };

    // ===============================
    // NET LAYOUT (unchanged visually)
    // ===============================
    drawFace(x(0), y(0), 0); // UP

    drawFace(x(-1), y(1), 1); // LEFT
    drawFace(x(0), y(1), 2);  // FRONT
    drawFace(x(1), y(1), 3);  // RIGHT
    drawFace(x(2), y(1), 4);  // BACK

    drawFace(x(0), y(2), 5);  // DOWN
}

// ===============================
// External update API (safe)
// ===============================
void CubeNet::setFaceColours(int faceIndex, const std::array<Colour, 9>& colors)
{
    if (faceIndex < 0 || faceIndex >= 6)
        return;

    cube.setFace(faceIndex, colors);
    update();
}