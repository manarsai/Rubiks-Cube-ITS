#ifndef CUBEVIEW_H
#define CUBEVIEW_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <vector>

class cubeView : public QOpenGLWidget, protected QOpenGLFunctions
{
    Q_OBJECT

public:
    explicit cubeView(QWidget* parent = nullptr);

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

    void drawAllCubies();
    void drawUnitCube();
    void drawCubeEdges();

    float angleX;
    float angleY;
};

#endif // CUBEVIEW_H