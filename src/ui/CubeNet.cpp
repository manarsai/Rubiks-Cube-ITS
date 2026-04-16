#include "CubeNet.h"
#include <QPainter>
#include "../core/domain/Cube.h"
#include <map>

#include <sstream>
#include "../core/domain/CubeModel.h"

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
    auto drawFace = [&](int startX, int startY, int faceIndex) {

        for (int r = 0; r < 3; r++) {
            for (int c = 0; c < 3; c++) {

                QRect rect(
                    startX + c * cellSize,
                    startY + r * cellSize,
                    cellSize,
                    cellSize
                );

                QColor qcolor = Qt::gray;

                if (faceFilled[faceIndex])   // ? correct check
                {
                    int index = r * 3 + c;

                    switch (CubeModel::get().getFaces()[faceIndex][index])
                    {
                    case Color::WHITE:  qcolor = Qt::white; break;
                    case Color::RED:    qcolor = Qt::red; break;
                    case Color::BLUE:   qcolor = Qt::blue; break;
                    case Color::GREEN:  qcolor = Qt::green; break;
                    case Color::YELLOW: qcolor = Qt::yellow; break;
                    case Color::ORANGE: qcolor = QColor(255, 165, 0); break;
                    default: qcolor = Qt::gray; break;
                    }
                }

                painter.fillRect(rect, qcolor);
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
// Layout:
//     [0]
//     [1]
// [2] [3] [4]
//     [5]

    drawFace(x(0), y(0), 0); // TOP
    drawFace(x(0), y(1), 1); // FRONT (or whatever order you choose)

    drawFace(x(-1), y(2), 2); // LEFT
    drawFace(x(0), y(2), 3); // CENTER
    drawFace(x(1), y(2), 4); // RIGHT

    drawFace(x(0), y(3), 5); // BOTTOM
}

void CubeNet::setFaceColors(int faceIndex, const std::array<Color, 9>& colors)
{
    if (faceIndex < 0 || faceIndex >= 6)
        return;

    CubeModel::get().setFace(faceIndex, colors);
    faceFilled[faceIndex] = true;

    update(); // repaint
}


bool CubeNet::validateCube() const
{
    std::map<Color, int> counts;

    // init counts
    counts[Color::WHITE] = 0;
    counts[Color::RED] = 0;
    counts[Color::BLUE] = 0;
    counts[Color::GREEN] = 0;
    counts[Color::YELLOW] = 0;
    counts[Color::ORANGE] = 0;

    // count all stickers
    for (int f = 0; f < 6; f++)
    {
        if (!faceFilled[f])
            return false;

        for (int i = 0; i < 9; i++)
        {
            Color c = cubeFaces[f][i];

            if (c == Color::UNKNOWN)
                return false;

            if (counts.find(c) == counts.end())
                return false;

            counts[c]++;
        }
    }
    //// each color must appear exactly 9 times
    //for (const auto& [color, count] : counts)
    //{
    //    if (count != 9)
    //        return false;
    //}

    return true;
}


const std::array<std::array<Color, 9>, 6>& CubeNet::getCubeState() const
{
    return cubeFaces;
}


std::string CubeNet::serializeState()
{
    std::ostringstream out;

    for (int f = 0; f < 6; f++)
    {
        for (int i = 0; i < 9; i++)
        {
            out << static_cast<int>(cubeFaces[f][i]);

            if (i < 8) out << ",";
        }

        if (f < 5) out << ";";
    }

    return out.str();
}

void CubeNet::restoreState(const std::string& state)
{
    std::istringstream ss(state);
    std::string faceStr;

    int f = 0;

    while (std::getline(ss, faceStr, ';') && f < 6)
    {
        std::istringstream faceStream(faceStr);
        std::string cell;

        int i = 0;

        while (std::getline(faceStream, cell, ',') && i < 9)
        {
            cubeFaces[f][i] = static_cast<Color>(std::stoi(cell));
            i++;
        }

        faceFilled[f] = true;
        f++;
    }

    update();
}

void CubeNet::reset()
{
    for (int f = 0; f < 6; f++)
    {
        faceFilled[f] = false;

        for (int i = 0; i < 9; i++)
        {
            cubeFaces[f][i] = Color::UNKNOWN;
        }
    }

    update();
}