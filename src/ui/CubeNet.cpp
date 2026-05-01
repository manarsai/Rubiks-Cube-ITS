#include "CubeNet.h"
#include <QPainter>
#include <QMouseEvent>
#include <algorithm>

// =====================================================
// CONSTRUCTOR
// =====================================================
CubeNet::CubeNet(Cube& c, QWidget* parent)
    : QWidget(parent), cube(c)
{
    setMinimumSize(400, 350);
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    setAttribute(Qt::WA_Hover, true);
    setMouseTracking(true);          // IMPORTANT for hover
    setFocusPolicy(Qt::StrongFocus);
}

QColor invertColor(const QColor& c)
{
    return QColor(255 - c.red(),
        255 - c.green(),
        255 - c.blue());
}

// =====================================================
// LAYOUT
// =====================================================
void CubeNet::updateLayout()
{
    const int margin = 25;

    int w = width() - margin * 2;
    int h = height() - margin * 2;

    layout.faceSize = std::min(w / 4, h / 3);
    layout.cellSize = layout.faceSize / 3;

    layout.centerX = width() / 2;
    layout.centerY = height() / 2;
}

// =====================================================
// PAINT
// =====================================================
void CubeNet::paintEvent(QPaintEvent*)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setPen(QPen(Qt::black, 2));

    updateLayout();

    auto x = [&](int col)
        {
            return layout.centerX - (layout.faceSize * 2) + col * layout.faceSize;
        };

    auto y = [&](int row)
        {
            return layout.centerY - (layout.faceSize * 3 / 2) + row * layout.faceSize;
        };

    auto drawFace = [&](int startX, int startY, int faceIndex)
        {
            auto face = cube.getFace(faceIndex);

            for (int r = 0; r < 3; r++)
            {
                for (int c = 0; c < 3; c++)
                {
                    QRect rect(
                        startX + c * layout.cellSize,
                        startY + r * layout.cellSize,
                        layout.cellSize,
                        layout.cellSize
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

                    if (faceIndex == hoverFace &&
                        r == hoverRow &&
                        c == hoverCol)
                    {
                        QColor highlight = invertColor(qcolor);

                        painter.setPen(QPen(highlight, 7));
                        painter.drawRect(rect);

                        painter.setPen(QPen(Qt::black, 2)); // restore default
                    }
                }
            }
        };

    // =====================================================
    // NET LAYOUT
    // =====================================================
    drawFace(x(1), y(0), 0); // UP

    drawFace(x(0), y(1), 1); // LEFT
    drawFace(x(1), y(1), 2); // FRONT
    drawFace(x(2), y(1), 3); // RIGHT
    drawFace(x(3), y(1), 4); // BACK

    drawFace(x(1), y(2), 5); // DOWN
}

// =====================================================
// MOUSE MOVE (HOVER)
// =====================================================
void CubeNet::mouseMoveEvent(QMouseEvent* event)
{
    updateLayout();

    hoverFace = -1;
    hoverRow = -1;
    hoverCol = -1;

    const int margin = 25;

    int w = width() - margin * 2;
    int h = height() - margin * 2;

    int faceSize = std::min(w / 4, h / 3);
    int cellSize = faceSize / 3;

    auto x = [&](int col)
        {
            return width() / 2 - (faceSize * 2) + col * faceSize;
        };

    auto y = [&](int row)
        {
            return height() / 2 - (faceSize * 3 / 2) + row * faceSize;
        };

    struct Face { QRect rect; int index; };

    Face faces[] =
    {
        { QRect(x(1), y(0), faceSize, faceSize), 0 },
        { QRect(x(0), y(1), faceSize, faceSize), 1 },
        { QRect(x(1), y(1), faceSize, faceSize), 2 },
        { QRect(x(2), y(1), faceSize, faceSize), 3 },
        { QRect(x(3), y(1), faceSize, faceSize), 4 },
        { QRect(x(1), y(2), faceSize, faceSize), 5 },
    };

    for (const auto& f : faces)
    {
        if (!f.rect.contains(event->pos()))
            continue;

        int localX = event->pos().x() - f.rect.x();
        int localY = event->pos().y() - f.rect.y();

        hoverFace = f.index;
        hoverCol = localX / cellSize;
        hoverRow = localY / cellSize;

        update();
        return;
    }
}

// =====================================================
// CLICK EDITING
// =====================================================
void CubeNet::mousePressEvent(QMouseEvent* event)
{
    updateLayout();

    const int faceSize = layout.faceSize;
    const int cellSize = layout.cellSize;

    auto x = [&](int col)
        {
            return layout.centerX - (faceSize * 2) + col * faceSize;
        };

    auto y = [&](int row)
        {
            return layout.centerY - (faceSize * 3 / 2) + row * faceSize;
        };

    struct Face { QRect rect; int index; };

    Face faces[] =
    {
        { QRect(x(1), y(0), faceSize, faceSize), 0 },
        { QRect(x(0), y(1), faceSize, faceSize), 1 },
        { QRect(x(1), y(1), faceSize, faceSize), 2 },
        { QRect(x(2), y(1), faceSize, faceSize), 3 },
        { QRect(x(3), y(1), faceSize, faceSize), 4 },
        { QRect(x(1), y(2), faceSize, faceSize), 5 },
    };

    for (const auto& f : faces)
    {
        if (!f.rect.contains(event->pos()))
            continue;

        int localX = event->pos().x() - f.rect.x();
        int localY = event->pos().y() - f.rect.y();

        int col = localX / cellSize;
        int row = localY / cellSize;

        if (col < 0 || col >= 3 || row < 0 || row >= 3)
            return;

        cycleColor(f.index, row, col);
        update();
        return;
    }
}

// =====================================================
// COLOR CYCLE
// =====================================================
void CubeNet::cycleColor(int face, int row, int col)
{
    auto faceData = cube.getFace(face);

    int index = row * 3 + col;

    Colour current = faceData[index];
    Colour next;

    switch (current)
    {
    case Colour::WHITE:  next = Colour::RED; break;
    case Colour::RED:    next = Colour::BLUE; break;
    case Colour::BLUE:   next = Colour::GREEN; break;
    case Colour::GREEN:  next = Colour::YELLOW; break;
    case Colour::YELLOW: next = Colour::ORANGE; break;
    case Colour::ORANGE: next = Colour::WHITE; break;
    default:             next = Colour::WHITE; break;
    }

    faceData[index] = next;
    cube.setFace(face, faceData);
}

// =====================================================
// SAFE UPDATE API
// =====================================================
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

