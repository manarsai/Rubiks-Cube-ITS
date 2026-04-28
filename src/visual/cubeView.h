#ifndef CUBEVIEW_H
#define CUBEVIEW_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QMouseEvent>
#include <vector>
#include <queue>
#include <string>

#include "../core/domain/Cube.h"
#include "../core/domain/Moves.h"   // ? REQUIRED for Move

class cubeView : public QOpenGLWidget, protected QOpenGLFunctions
{
    Q_OBJECT

public:
    explicit cubeView(Cube& cubeModel, QWidget* parent = nullptr);

    // optional direct move trigger
    void startMove(const Move& m);

    // feed move sequences like {"R", "U'", "F"}
    void setMoves(const std::vector<std::string>& moves);
    void testMoves();

protected:
    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;


    void mousePressEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;

private:
    Cube& cube;

    // =========================
    // Cubie representation
    // =========================
    struct Cubie
    {
        int x, y, z;
    };

    std::vector<Cubie> cubies;

    // =========================
    // Camera rotation
    // =========================
    float angleX;
    float angleY;

    QPoint lastMousePos;
    bool dragging = false;

    // =========================
    // Animation system (ONLY ONE currentMove)
    // =========================
    struct ActiveMove
    {
        char move = 0;        // 'R', 'U', 'F', 'B'
        bool prime = false;   // inverse move
        bool active = false;
        float angle = 0.0f;
        float speed = 3.0f;
    };

    ActiveMove currentMove;
    std::queue<std::string> moveQueue;

    void startNextMove();

    // =========================
    // Rendering
    // =========================
    void drawAllCubies();
    void drawCubie(const Cubie& c, bool highlight = false);
    void drawCubeEdges();

    QColor convertColour(Colour c);

    // safe cube access
    Colour safeAt(int face, int row, int col);
};

#endif