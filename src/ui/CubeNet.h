#pragma once

#include <QWidget>
#include <array>
#include "../core/domain/Cube.h"
#include "../core/domain/Colour.h"   // IMPORTANT (missing in your header)

class CubeNet : public QWidget
{
    Q_OBJECT

public:
    explicit CubeNet(Cube& c, QWidget* parent = nullptr);

    void setFaceColours(int faceIndex, const std::array<Colour, 9>& colors);

signals:
    void cubeChanged();
    void scanCompleted();

protected:
    void paintEvent(QPaintEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;

private:
    Cube& cube;
    Colour selectedColor = Colour::WHITE;

    // =========================
    // LAYOUT
    // =========================
    struct Layout
    {
        int faceSize = 0;
        int cellSize = 0;
        int centerX = 0;
        int centerY = 0;
    };

    Layout layout;
    void updateLayout();

    // =========================
    // HOVER STATE
    // =========================
    int hoverFace = -1;
    int hoverRow = -1;
    int hoverCol = -1;

    // =========================
    // EDITING (MATCH CPP)
    // =========================
    void setColor(int face, int row, int col);
};