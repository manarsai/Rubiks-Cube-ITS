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
    explicit cubeView(Cube& cubeModel, QWidget* parent = nullptr);

    // ? FIXED: string-based move input
    void startMove(const std::string& move);

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
    // Cubies
    // =========================
    struct Cubie
    {
        int x, y, z;
    };

    std::vector<Cubie> cubies;

    // =========================
    // Camera
    // =========================
    float angleX = 0.0f;
    float angleY = 0.0f;

    QPoint lastMousePos;
    bool dragging = false;

    // =========================
    // Animation system
    // =========================
    struct ActiveMove
    {
        std::string move;   // "R", "U'", etc.
        bool active = false;
        float angle = 0.0f;
        float speed = 90.0f;
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

    Colour safeAt(int face, int row, int col);
};

#endif