#include "CubeNet.h"
#include <QPainter>
#include "../core/domain/Cube.h"
#include "../core/domain/Validator.h"

CubeNet::CubeNet(Cube& c, QWidget* parent)
    : QWidget(parent), cube(c)
{
    setMinimumSize(400, 350);
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
}

// ===============================
// PAINT (FIXED + RESPONSIVE)
// ===============================
void CubeNet::paintEvent(QPaintEvent*)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    const int margin = 25;

    int w = width() - margin * 2;
    int h = height() - margin * 2;

    int faceSize = std::min(w / 4, h / 3);
    int cellSize = faceSize / 3;

    int centerX = width() / 2;
    int centerY = height() / 2;

    painter.setPen(QPen(Qt::black, 2));

    auto drawFace = [&](int startX, int startY, int faceIndex)
        {
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

                    QColor qcolor;
                    switch (col)
                    {
                    case Colour::WHITE:  qcolor = Qt::white; break;
                    case Colour::RED:    qcolor = Qt::red; break;
                    case Colour::BLUE:   qcolor = Qt::blue; break;
                    case Colour::GREEN:  qcolor = Qt::green; break;
                    case Colour::YELLOW: qcolor = Qt::yellow; break;
                    case Colour::ORANGE: qcolor = QColor(255, 165, 0); break;
                    default:              qcolor = QColor(180, 180, 180); break;
                    }

                    painter.fillRect(rect, qcolor);
                    painter.drawRect(rect);
                }
            }
        };

    auto x = [&](int col)
        {
            return centerX - (faceSize * 2) + col * faceSize;
        };

    auto y = [&](int row)
        {
            return centerY - (faceSize * 3 / 2) + row * faceSize;
        };

    // ===============================
    // NET LAYOUT (CENTERED + SAFE)
    // ===============================
    drawFace(x(1), y(0), 0); // UP

    drawFace(x(0), y(1), 1); // LEFT
    drawFace(x(1), y(1), 2); // FRONT
    drawFace(x(2), y(1), 3); // RIGHT
    drawFace(x(3), y(1), 4); // BACK

    drawFace(x(1), y(2), 5); // DOWN
}

// ===============================
// SAFE UPDATE
// ===============================
void CubeNet::setFaceColours(int faceIndex, const std::array<Colour, 9>& colors)
{
    if (faceIndex < 0 || faceIndex >= 6)
        return;

    int valid = 0;
    for (auto c : colors)
        if (c != Colour::UNKNOWN)
            valid++;

    if (valid < 1)
        return;

    cube.setFace(faceIndex, colors);
    update();
}