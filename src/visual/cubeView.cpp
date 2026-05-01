#include "cubeView.h"
#include <QTimer>
#include <cmath>
#include <QMouseEvent>
#include <iostream>


// =====================================================
// Constructor
// =====================================================
cubeView::cubeView(Cube& cubeModel, QWidget* parent, bool idle)
    : QOpenGLWidget(parent),
    cube(cubeModel),
    idleMode(idle)
{
    for (int x = -1; x <= 1; x++)
        for (int y = -1; y <= 1; y++)
            for (int z = -1; z <= 1; z++)
                cubies.push_back({ x, y, z });

    timer = new QTimer(this);

    connect(timer, &QTimer::timeout, this, [this]()
        {
            if (!currentMove.active)
                return;

            currentMove.angle += currentMove.speed * 0.016f;

            if (currentMove.angle >= 90.0f)
            {
                currentMove.angle = 90.0f;
                currentMove.active = false;

                // ? APPLY MODEL ONLY HERE (ONE SOURCE OF TRUTH)
                applyMoveToModel(currentMove.move);

                QTimer::singleShot(0, this, [this]()
                    {
                        startNextMove();
                    });
            }

            update();
        });

    timer->start(16);
}



// =====================================================
// MOVE QUEUE
// =====================================================
void cubeView::setMoves(const std::vector<std::string>& moves)
{
    resetAnimation();

    std::queue<std::string> empty;
    std::swap(moveQueue, empty);

    for (auto& m : moves)
        moveQueue.push(m);

    playMoves();
}

void cubeView::playMoves()
{
    if (!currentMove.active)
        startNextMove();
}

void cubeView::startNextMove()
{
    if (moveQueue.empty())
        return;

    currentMove.move = moveQueue.front();
    moveQueue.pop();

    currentMove.angle = 0.0f;
    currentMove.speed = 30.0f;
    currentMove.active = true;
}
// =====================================================
// MAIN RENDER LOOP
// =====================================================
void cubeView::drawAllCubies()
{
    for (const auto& c : cubies)
    {
        glPushMatrix();

        // =========================
        // MOVE TRANSFORM
        // =========================
        bool inLayer = false;
        float axisX = 0, axisY = 0, axisZ = 0;

        bool highlight = inLayer;


        if (currentMove.active)
        {
            const std::string& mv = currentMove.move;

            if ((mv == "R" || mv == "R'") && c.x == 1) { inLayer = true; axisX = 1; }
            //if ((mv == "L" || mv == "L'") && c.x == -1) { inLayer = true; axisX = 1; }

            if ((mv == "U" || mv == "U'") && c.y == 1) { inLayer = true; axisY = 1; }
            if ((mv == "D" || mv == "D'") && c.y == -1) { inLayer = true; axisY = 1; }

            if ((mv == "F" || mv == "F'") && c.z == 1) { inLayer = true; axisZ = 1; }
            if ((mv == "B" || mv == "B'") && c.z == -1) { inLayer = true; axisZ = 1; }
        }

        if (inLayer)
        {
            bool prime = (currentMove.move.size() > 1 &&
                currentMove.move[1] == '\'');

            std::string base = std::string(1, currentMove.move[0]);

            float dir = -1.0f; // default for R, U, F

            // ?? invert for L, B, D
            if (base == "L" || base == "B" || base == "D")
                dir *= -1.0f;

            if (prime)
                dir *= -1.0f;
            glRotatef(dir * currentMove.angle, axisX, axisY, axisZ);
        }

        // =========================
        // POSITION
        // =========================
        glTranslatef(c.x * 1.05f, c.y * 1.05f, c.z * 1.05f);

        // =========================
        // DRAW CUBIE (FACES + EDGES TOGETHER)
        // =========================
        drawCubie(c, highlight);

        drawCubeEdges();   // ?? FIX: per cubie, not global

        glPopMatrix();
    }
}

// =====================================================
// CUBIE DRAW (faces only)
// =====================================================
void cubeView::drawCubie(const Cubie& c, bool highlight)
{
    float s = 0.5f;

    glBegin(GL_QUADS);

    auto apply = [&](QColor qc)
        {
            glColor3f(qc.redF(), qc.greenF(), qc.blueF());
        };

    if (c.z == 1)
    {
        apply(convertColour(safeAtModel(2, -c.y + 1, c.x + 1)));
        glVertex3f(-s, -s, s);
        glVertex3f(-s, s, s);
        glVertex3f(s, s, s);
        glVertex3f(s, -s, s);
    }

    if (c.z == -1)
    {
        apply(convertColour(safeAtModel(4, -c.y + 1, -c.x + 1)));
        glVertex3f(s, -s, -s);
        glVertex3f(s, s, -s);
        glVertex3f(-s, s, -s);
        glVertex3f(-s, -s, -s);
    }

    if (c.x == -1)
    {
        apply(convertColour(safeAtModel(1, -c.y + 1, c.z + 1)));
        glVertex3f(-s, -s, -s);
        glVertex3f(-s, -s, s);
        glVertex3f(-s, s, s);
        glVertex3f(-s, s, -s);
    }

    if (c.x == 1)
    {
        apply(convertColour(safeAtModel(3, -c.y + 1, -c.z + 1)));
        glVertex3f(s, -s, s);
        glVertex3f(s, -s, -s);
        glVertex3f(s, s, -s);
        glVertex3f(s, s, s);
    }

    if (c.y == 1)
    {
        apply(convertColour(safeAtModel(0, c.z + 1, c.x + 1)));
        glVertex3f(-s, s, -s);
        glVertex3f(s, s, -s);
        glVertex3f(s, s, s);
        glVertex3f(-s, s, s);
    }

    if (c.y == -1)
    {
        apply(convertColour(safeAtModel(5, -c.z + 1, c.x + 1)));
        glVertex3f(-s, -s, s);
        glVertex3f(s, -s, s);
        glVertex3f(s, -s, -s);
        glVertex3f(-s, -s, -s);
    }

    glEnd();


    if (highlight)
    {
        glColor3f(0.0f, 1.0f, 1.0f);
        glLineWidth(10.0f);


        glBegin(GL_LINES);

        glVertex3f(-s, -s, -s); glVertex3f(s, -s, -s);
        glVertex3f(s, -s, -s); glVertex3f(s, -s, s);
        glVertex3f(s, -s, s); glVertex3f(-s, -s, s);
        glVertex3f(-s, -s, s); glVertex3f(-s, -s, -s);

        glVertex3f(-s, s, -s); glVertex3f(s, s, -s);
        glVertex3f(s, s, -s); glVertex3f(s, s, s);
        glVertex3f(s, s, s); glVertex3f(-s, s, s);
        glVertex3f(-s, s, s); glVertex3f(-s, s, -s);

        glVertex3f(-s, -s, -s); glVertex3f(-s, s, -s);
        glVertex3f(s, -s, -s); glVertex3f(s, s, -s);
        glVertex3f(s, -s, s); glVertex3f(s, s, s);
        glVertex3f(-s, -s, s); glVertex3f(-s, s, s);

        glEnd();

    }
}

// =====================================================
// EDGE OUTLINE (FIXED: per-cube clean wireframe)
// =====================================================
void cubeView::drawCubeEdges()
{
    float s = 0.5f;

    glDisable(GL_LIGHTING);
    glColor3f(0, 0, 0);
    glLineWidth(10.0f);

    glBegin(GL_LINES);

    // bottom
    glVertex3f(-s, -s, -s); glVertex3f(s, -s, -s);
    glVertex3f(s, -s, -s); glVertex3f(s, -s, s);
    glVertex3f(s, -s, s); glVertex3f(-s, -s, s);
    glVertex3f(-s, -s, s); glVertex3f(-s, -s, -s);

    // top
    glVertex3f(-s, s, -s); glVertex3f(s, s, -s);
    glVertex3f(s, s, -s); glVertex3f(s, s, s);
    glVertex3f(s, s, s); glVertex3f(-s, s, s);
    glVertex3f(-s, s, s); glVertex3f(-s, s, -s);

    // vertical edges
    glVertex3f(-s, -s, -s); glVertex3f(-s, s, -s);
    glVertex3f(s, -s, -s); glVertex3f(s, s, -s);
    glVertex3f(s, -s, s); glVertex3f(s, s, s);
    glVertex3f(-s, -s, s); glVertex3f(-s, s, s);

    glEnd();
}

// =====================================================
void cubeView::initializeGL()
{
    initializeOpenGLFunctions();
    glEnable(GL_DEPTH_TEST);
    glClearColor(1, 1, 1, 1);
}

void cubeView::resizeGL(int w, int h)
{
    glViewport(0, 0, w, h);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    float aspect = float(w) / float(h);
    float nearP = 0.1f;
    float top = nearP * tan(45.0f * M_PI / 360.0f);
    float right = top * aspect;

    glFrustum(-right, right, -top, top, nearP, 100.0f);

    glMatrixMode(GL_MODELVIEW);
}

void cubeView::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glLoadIdentity();
    glTranslatef(0, 0, -8);

    glRotatef(angleX, 1, 0, 0);
    glRotatef(angleY, 0, 1, 0);

    drawAllCubies();
}

// =====================================================
// Mouse
// =====================================================
void cubeView::mousePressEvent(QMouseEvent* e)
{
    if (e->button() == Qt::LeftButton)
    {
        dragging = true;
        lastMousePos = e->pos();
    }
}

void cubeView::mouseMoveEvent(QMouseEvent* e)
{
    if (!dragging) return;

    QPoint d = e->pos() - lastMousePos;
    lastMousePos = e->pos();

    angleY += d.x() * 0.5f;
    angleX += d.y() * 0.5f;

    update();
}

void cubeView::mouseReleaseEvent(QMouseEvent* e)
{
    if (e->button() == Qt::LeftButton)
        dragging = false;
}

void cubeView::testMoves()
{
    setMoves({

        });
}

QColor cubeView::convertColour(Colour c)
{
    switch (c)
    {
    case Colour::WHITE:  return QColor("#FFFFFF");
    case Colour::YELLOW: return QColor("#FFD500");
    case Colour::RED:    return QColor("#B90000");
    case Colour::ORANGE: return QColor("#FF5900");
    case Colour::BLUE:   return QColor("#0045AD");
    case Colour::GREEN:  return QColor("#009B48");
    default:             return Qt::gray;
    }
}

Colour cubeView::safeAtModel(int face, int row, int col)
{
    return cube.at(face, row, col);
}

void cubeView::applyMoveToModel(const std::string& mv)
{
    for (const auto& [name, move] : MOVES)
    {
        if (name == mv)
        {
            cube.apply(move);
            return;
        }
    }
}

void cubeView::setCubeState(const Cube& state)
{
    cube = state;
}

void cubeView::resetAnimation()
{
    currentMove.active = false;
    currentMove.angle = 0.0f;
    currentMove.speed = 45.0f;
    currentMove.move = "";

    while (!moveQueue.empty())
        moveQueue.pop();
}


