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
    explicit cubeView(Cube& cubeModel, QWidget* parent = nullptr);

protected:
    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;

private:
    Cube& cube;

    struct Cubie
    {
        int x, y, z;
    };

    std::vector<Cubie> cubies;

    float angleX;
    float angleY;

    void drawAllCubies();
    void drawCubie(const Cubie& c);
    void drawCubeEdges();

    QColor convertColour(Colour c);

    // ?? safety helper
    Colour safeAt(int face, int row, int col);
};

#endif