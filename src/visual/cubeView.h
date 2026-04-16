#ifndef CUBEVIEW_H
#define CUBEVIEW_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <vector>
#include "../core/domain/Cube.h"

class cubeView : public QOpenGLWidget, protected QOpenGLFunctions
{
    Q_OBJECT

public:
    explicit cubeView(QWidget* parent = nullptr);
    //void setCubeState(const std::array<std::array<Color, 9>, 6>& state);

protected:
    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;

private:
    // ?? Represents one small cube
    struct Cubie {
        int x, y, z; // position in cube (-1, 0, 1)
    };

    std::vector<Cubie> cubies;
    std::array<std::array<Color, 9>, 6> cubeState;

    void drawAllCubies();
    void drawUnitCube();
    void drawCubeEdges();

 /*   QColor mapColor(Color c);*/

    //void syncFromModel();

    float angleX;
    float angleY;
};

#endif // CUBEVIEW_H