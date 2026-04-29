#include "cubeView.h"
#include <QTimer>
#include <QCoreApplication>
#include <cmath>
#include <QMouseEvent>
#include <src/core/domain/Moves.h>
#include <src/core/domain/Cube.h>
#include <iostream>

// =========================
// Constructor
// =========================
cubeView::cubeView(Cube& cubeModel, QWidget* parent)
    : QOpenGLWidget(parent),
    cube(cubeModel),
    angleX(20.0f),
    angleY(30.0f)
{
    for (int x = -1; x <= 1; x++)
        for (int y = -1; y <= 1; y++)
            for (int z = -1; z <= 1; z++)
                cubies.push_back({ x, y, z });

    auto* timer = new QTimer(this);

    connect(timer, &QTimer::timeout, this, [this]()
        {
            if (!currentMove.active) return;

            currentMove.angle += currentMove.speed * 0.016f;

            if (currentMove.angle >= 90.0f)
            {
                currentMove.active = false;

                const std::string& mv = currentMove.move;

                // Apply move safely
                if (mv == "R")       cube.applyMove(R);
                else if (mv == "R'") cube.applyMove(R_prime);
                else if (mv == "U")  cube.applyMove(U);
                else if (mv == "U'") cube.applyMove(U_prime);
                else if (mv == "F")  cube.applyMove(F);
                else if (mv == "F'") cube.applyMove(F_prime);
                else if (mv == "D")  cube.applyMove(D);
                else if (mv == "D'") cube.applyMove(D_prime);
                else if (mv == "B")  cube.applyMove(B);
                else if (mv == "B'") cube.applyMove(B_prime);
                else if (mv == "L")  cube.applyMove(L);
                else if (mv == "L'") cube.applyMove(L_prime);

                startNextMove();
            }

            update();
            QCoreApplication::processEvents();
        });

    timer->start(16);
}

// =========================
// Move queue
// =========================
void cubeView::setMoves(const std::vector<std::string>& moves)
{
    std::queue<std::string> empty;
    std::swap(moveQueue, empty);

    for (const auto& m : moves)
        moveQueue.push(m);

    if (!currentMove.active)
        startNextMove();
}

void cubeView::startNextMove()
{
    if (moveQueue.empty())
        return;

    currentMove.move = moveQueue.front();
    moveQueue.pop();

    currentMove.active = true;
    currentMove.angle = 0.0f;
    currentMove.speed = 90.0f;
}

// =========================
// DRAW CUBE EDGES
// =========================
void cubeView::drawCubeEdges()
{
    glColor3f(0, 0, 0);
    glLineWidth(10.0f);

    float s = 0.5f;

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

    // vertical
    glVertex3f(-s, -s, -s); glVertex3f(-s, s, -s);
    glVertex3f(s, -s, -s); glVertex3f(s, s, -s);
    glVertex3f(s, -s, s); glVertex3f(s, s, s);
    glVertex3f(-s, -s, s); glVertex3f(-s, s, s);

    glEnd();
}

// =========================
// MAIN RENDER LOOP
// =========================
void cubeView::drawAllCubies()
{
    for (const auto& c : cubies)
    {
        glPushMatrix();

        bool inLayer = false;
        float axisX = 0, axisY = 0, axisZ = 0;
        bool highlight = false;

        if (currentMove.active)
        {
            const std::string& mv = currentMove.move;

            if ((mv == "R" || mv == "R'") && c.x == 1)
            {
                inLayer = true; axisX = 1; highlight = true;
            }

            if ((mv == "U" || mv == "U'") && c.y == 1)
            {
                inLayer = true; axisY = 1; highlight = true;
            }

            if ((mv == "D" || mv == "D'") && c.y == -1)
            {
                inLayer = true; axisY = 1; highlight = true;
            }

            if ((mv == "F" || mv == "F'") && c.z == 1)
            {
                inLayer = true; axisZ = 1; highlight = true;
            }

            if ((mv == "B" || mv == "B'") && c.z == -1)
            {
                inLayer = true; axisZ = 1; highlight = true;
            }

            if ((mv == "L" || mv == "L'") && c.x == -1)
            {
                inLayer = true; axisX = 1; highlight = true;
            }
        }

        if (inLayer)
        {
            bool prime = (currentMove.move.size() > 1 && currentMove.move[1] == '\'');
            float dir = prime ? 1.0f : -1.0f;

            glRotatef(dir * currentMove.angle, axisX, axisY, axisZ);
        }

        glTranslatef(c.x * 1.05f, c.y * 1.05f, c.z * 1.05f);

        drawCubie(c, highlight);
        drawCubeEdges();

        glPopMatrix();
    }
}

// =========================
// CUBIE DRAW
// =========================
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
        apply(convertColour(safeAt(2, -c.y + 1, c.x + 1)));
        glVertex3f(-s, -s, s); glVertex3f(-s, s, s);
        glVertex3f(s, s, s);   glVertex3f(s, -s, s);
    }

    if (c.z == -1)
    {
        apply(convertColour(safeAt(4, -c.y + 1, -c.x + 1)));
        glVertex3f(s, -s, -s); glVertex3f(s, s, -s);
        glVertex3f(-s, s, -s); glVertex3f(-s, -s, -s);
    }

    if (c.x == -1)
    {
        apply(convertColour(safeAt(1, -c.y + 1, c.z + 1)));
        glVertex3f(-s, -s, -s); glVertex3f(-s, -s, s);
        glVertex3f(-s, s, s);   glVertex3f(-s, s, -s);
    }

    if (c.x == 1)
    {
        apply(convertColour(safeAt(3, -c.y + 1, -c.z + 1)));
        glVertex3f(s, -s, s); glVertex3f(s, -s, -s);
        glVertex3f(s, s, -s); glVertex3f(s, s, s);
    }

    if (c.y == 1)
    {
        apply(convertColour(safeAt(0, c.z + 1, c.x + 1)));
        glVertex3f(-s, s, -s); glVertex3f(s, s, -s);
        glVertex3f(s, s, s);   glVertex3f(-s, s, s);
    }

    if (c.y == -1)
    {
        apply(convertColour(safeAt(5, -c.z + 1, c.x + 1)));
        glVertex3f(-s, -s, s); glVertex3f(s, -s, s);
        glVertex3f(s, -s, -s); glVertex3f(-s, -s, -s);
    }

    glEnd();

    if (highlight)
    {
        glColor3f(0.0f, 1.0f, 1.0f);
        glLineWidth(3.0f);

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

// =========================
void cubeView::initializeGL()
{
    initializeOpenGLFunctions();
    glEnable(GL_DEPTH_TEST);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);
    glClearColor(1, 1, 1, 1);
}

// =========================
void cubeView::resizeGL(int w, int h)
{
    glViewport(0, 0, w, h);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    float aspect = float(w) / float(h);
    float fov = 45.0f;

    float nearP = 0.1f;
    float top = nearP * tan(fov * 3.1415926f / 360.0f);
    float right = top * aspect;

    glFrustum(-right, right, -top, top, nearP, 100.0f);

    glMatrixMode(GL_MODELVIEW);
}

// =========================
void cubeView::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glLoadIdentity();
    glTranslatef(0, 0, -8);

    glRotatef(angleX, 1, 0, 0);
    glRotatef(angleY, 0, 1, 0);

    drawAllCubies();
}

// =========================
// SAFE ACCESS
// =========================
Colour cubeView::safeAt(int face, int row, int col)
{
    if (face < 0 || face > 5 || row < 0 || row > 2 || col < 0 || col > 2)
        return Colour::UNKNOWN;

    return cube.at(face, row, col);
}

// =========================
inline int toFace(int v)
{
    return v + 1;
}

// =========================
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

// =========================
// Mouse controls (unchanged)
// =========================
void cubeView::mousePressEvent(QMouseEvent* event)
{
    if (event->button() == Qt::LeftButton)
    {
        dragging = true;
        lastMousePos = event->pos();
    }
}

void cubeView::mouseMoveEvent(QMouseEvent* event)
{
    if (!dragging) return;

    QPoint delta = event->pos() - lastMousePos;
    lastMousePos = event->pos();

    float sensitivity = 0.5f;
    angleY += delta.x() * sensitivity;
    angleX += delta.y() * sensitivity;

    update();
}

void cubeView::mouseReleaseEvent(QMouseEvent* event)
{
    if (event->button() == Qt::LeftButton)
        dragging = false;
}

void cubeView::testMoves()
{
   
}