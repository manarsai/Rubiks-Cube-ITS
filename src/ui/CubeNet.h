#pragma once

#include <QWidget>
#include <array>
#include "../core/domain/Cube.h"

class CubeNet : public QWidget
{
    Q_OBJECT

public:
    explicit CubeNet(Cube& c, QWidget* parent = nullptr);

    void setFaceColours(int faceIndex, const std::array<Colour, 9>& colors);

signals:
    void cubeChanged();

protected:
    void paintEvent(QPaintEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;   // ? FIX
    void mouseMoveEvent(QMouseEvent* event) override;    // ? FIX

private:
    Cube& cube;

    // =====================================================
    // LAYOUT CACHE
    // =====================================================
    struct Layout
    {
        int faceSize = 0;
        int cellSize = 0;
        int centerX = 0;
        int centerY = 0;
    };

    Layout layout;

    void updateLayout();

    // =====================================================
    // EDITING STATE (WAS MISSING)
    // =====================================================
    int hoverFace = -1;   // ? FIX
    int hoverRow = -1;    // ? FIX
    int hoverCol = -1;    // ? FIX

    // =====================================================
    // INTERNAL HELPERS
    // =====================================================
    void cycleColor(int face, int row, int col); // ? FIX (used in cpp)
};