#pragma once
#include <QWidget>
#include <array>
#include "../core/domain/Cube.h"

class CubeNet : public QWidget
{
    Q_OBJECT

public:
    CubeNet(QWidget* parent = nullptr);
    bool validateCube() const;

    // ?? NEW: specify which face
    void setFaceColors(int faceIndex, const std::array<Color, 9>& colors);

    const std::array<std::array<Color, 9>, 6>& getCubeState() const;

    std::string serializeState();
    void restoreState(const std::string& state);
    void reset();

protected:
    void paintEvent(QPaintEvent* event) override;

private:
    // ?? store ALL 6 faces
    std::array<std::array<Color, 9>, 6> cubeFaces = {};
    // track which faces are filled
    std::array<bool, 6> faceFilled = { false, false, false, false, false, false };
};