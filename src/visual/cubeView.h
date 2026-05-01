#ifndef CUBEVIEW_H
#define CUBEVIEW_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QMouseEvent>
#include <vector>
#include <queue>
#include <string>

#include "../core/domain/Cube.h"
#include "../core/domain/Moves.h"

class cubeView : public QOpenGLWidget, protected QOpenGLFunctions
{
    Q_OBJECT

public:
    explicit cubeView(Cube& cubeModel, QWidget* parent = nullptr, bool idleMode = false);

    void setMoves(const std::vector<std::string>& moves);
    void playMoves();

    void setCubeState(const Cube& state);
    void resetAnimation();

    void testMoves();

protected:
    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;

    void mousePressEvent(QMouseEvent*) override;
    void mouseMoveEvent(QMouseEvent*) override;
    void mouseReleaseEvent(QMouseEvent*) override;

private:
    // =========================
    // MODEL
    // =========================
    Cube cube;

    // =========================
    // CAMERA
    // =========================
    float angleX = 0.0f;
    float angleY = 0.0f;

    QPoint lastMousePos;
    bool dragging = false;

    bool idleMode = false;

    // =========================
    // CUBIES
    // =========================
    struct Cubie { int x, y, z; };
    std::vector<Cubie> cubies;

    // =========================
    // ANIMATION
    // =========================
    struct ActiveMove
    {
        std::string move;
        bool active = false;
        float angle = 0.0f;
        float speed = 90.0f;
    };

    ActiveMove currentMove;
    std::queue<std::string> moveQueue;

    QTimer* timer;

    void startNextMove();

    void applyMoveToModel(const std::string& mv);

    // =========================
    // RENDER
    // =========================
    void drawAllCubies();
    void drawCubie(const Cubie& c, bool highlight = false);
    void drawCubeEdges();

    QColor convertColour(Colour c);
    Colour safeAtModel(int face, int row, int col);
};

#endif