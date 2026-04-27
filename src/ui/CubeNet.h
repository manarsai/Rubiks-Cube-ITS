#pragma once

#include <QWidget>
#include <array>
#include "../core/domain/Cube.h"

class Cube;

class CubeNet : public QWidget
{
    Q_OBJECT

public:
    explicit CubeNet(Cube& c, QWidget* parent = nullptr);

    void setFaceColours(int faceIndex, const std::array<Colour, 9>& colors);

protected:
    void paintEvent(QPaintEvent* event) override;

private:
    Cube& cube;
};