#pragma once

#include <QWidget>
#include <array>
#include "../core/domain/Cube.h"

class CubeNet : public QWidget
{
    Q_OBJECT

public:
    explicit CubeNet(Cube& cube, QWidget* parent = nullptr);

    // Updates a face externally (scanner/debug/tools)
    void setFaceColours(int faceIndex, const std::array<Colour, 9>& colors);

protected:
    void paintEvent(QPaintEvent* event) override;

private:
    Cube& cube;  // single source of truth (correct design)
};