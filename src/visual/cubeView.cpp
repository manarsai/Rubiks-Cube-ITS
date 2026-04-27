#include "cubeView.h"
#include <QTimer>
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
            if (currentMove.active)
            {
                currentMove.angle += currentMove.speed;

                if (currentMove.angle >= 90.0f)
                {
                    currentMove.active = false;

                    Move m{};

                    switch (currentMove.move)
                    {
                    case 'R':
                        m = currentMove.prime ? R_prime : R;
                        break;
                    case 'U':
                        m = currentMove.prime ? U_prime : U;
                        break;
                    case 'F':
                        m = currentMove.prime ? F_prime : F;
                        break;
                    case 'B':
                        m = currentMove.prime ? B_prime : B;
                        break;
                    case 'D':
                        m = currentMove.prime ? D_prime : D;
                        break;
                    }

                    cube.applyMove(m);
                    startNextMove();
                }

                update();
                QCoreApplication::processEvents();
            }


        });

    timer->start(16);
}

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

    // verticals
    glVertex3f(-s, -s, -s); glVertex3f(-s, s, -s);
    glVertex3f(s, -s, -s); glVertex3f(s, s, -s);
    glVertex3f(s, -s, s); glVertex3f(s, s, s);
    glVertex3f(-s, -s, s); glVertex3f(-s, s, s);

    glEnd();
}

// =========================
// Move queue entry
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

    std::string m = moveQueue.front();
    moveQueue.pop();

    currentMove.active = true;
    currentMove.angle = 0.0f;
    currentMove.speed = 3.0f;

    currentMove.move = m[0];
    currentMove.prime = (m.size() > 1 && m[1] == '\'');
}

// =========================
void cubeView::initializeGL()
{
    initializeOpenGLFunctions();

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

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
    float farP = 100.0f;

    float top = nearP * tan(fov * 3.1415926f / 360.0f);
    float right = top * aspect;

    glFrustum(-right, right, -top, top, nearP, farP);

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
// ?? MAIN RENDER LOOP (FIXED)
// =========================
void cubeView::drawAllCubies()
{
    for (const auto& c : cubies)
    {
        glPushMatrix();

        bool inLayer = false;
        float axisX = 0, axisY = 0, axisZ = 0;

        if (currentMove.active)
        {
            if (currentMove.move == 'R' && c.x == 1) { inLayer = true; axisX = 1; }
  

            if (currentMove.move == 'U' && c.y == 1) { inLayer = true; axisY = 1; }
            if (currentMove.move == 'D' && c.y == -1) { inLayer = true; axisY = 1; }

            if (currentMove.move == 'F' && c.z == 1) { inLayer = true; axisZ = 1; }
            if (currentMove.move == 'B' && c.z == -1) { inLayer = true; axisZ = 1; }


            if (inLayer)
            {
                float dir = currentMove.prime ? 1.0f : -1.0f;
                glRotatef(dir * currentMove.angle, axisX, axisY, axisZ);
            }
        }

        glTranslatef(c.x * 1.05f, c.y * 1.05f, c.z * 1.05f);

        drawCubie(c);
        drawCubeEdges();

        glPopMatrix();
    }
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
// Draw cubie (UNCHANGED)
// =========================
void cubeView::drawCubie(const Cubie& c)
{
    float s = 0.5f;

    glBegin(GL_QUADS);

    if (c.z == 1)
    {
        QColor qc = convertColour(safeAt(2, toFace(-c.y), toFace(c.x)));
        glColor3f(qc.redF(), qc.greenF(), qc.blueF());
        glVertex3f(-s, -s, s); glVertex3f(s, -s, s); glVertex3f(s, s, s); glVertex3f(-s, s, s);
    }

    if (c.z == -1)
    {
        QColor qc = convertColour(safeAt(4, toFace(-c.y), toFace(-c.x)));
        glColor3f(qc.redF(), qc.greenF(), qc.blueF());
        glVertex3f(-s, -s, -s); glVertex3f(-s, s, -s); glVertex3f(s, s, -s); glVertex3f(s, -s, -s);
    }

    if (c.x == -1)
    {
        QColor qc = convertColour(safeAt(1, toFace(-c.y), toFace(c.z)));
        glColor3f(qc.redF(), qc.greenF(), qc.blueF());
        glVertex3f(-s, -s, -s); glVertex3f(-s, -s, s); glVertex3f(-s, s, s); glVertex3f(-s, s, -s);
    }

    if (c.x == 1)
    {
        QColor qc = convertColour(safeAt(3, toFace(-c.y), toFace(-c.z)));
        glColor3f(qc.redF(), qc.greenF(), qc.blueF());
        glVertex3f(s, -s, -s); glVertex3f(s, s, -s); glVertex3f(s, s, s); glVertex3f(s, -s, s);
    }

    if (c.y == 1)
    {
        QColor qc = convertColour(safeAt(0, toFace(c.z), toFace(c.x)));
        glColor3f(qc.redF(), qc.greenF(), qc.blueF());
        glVertex3f(-s, s, -s); glVertex3f(-s, s, s); glVertex3f(s, s, s); glVertex3f(s, s, -s);
    }

    if (c.y == -1)
    {
        QColor qc = convertColour(safeAt(5, toFace(-c.z), toFace(c.x)));
        glColor3f(qc.redF(), qc.greenF(), qc.blueF());
        glVertex3f(-s, -s, -s); glVertex3f(s, -s, -s); glVertex3f(s, -s, s); glVertex3f(-s, -s, s);
    }

    glEnd();
}

// =========================
QColor cubeView::convertColour(Colour c)
{
    switch (c)
    {
    case Colour::WHITE: return Qt::white;
    case Colour::RED: return Qt::red;
    case Colour::BLUE: return Qt::blue;
    case Colour::GREEN: return Qt::green;
    case Colour::YELLOW: return Qt::yellow;
    case Colour::ORANGE: return QColor(255, 165, 0);
    default: return Qt::gray;
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
    setMoves({ "R","D"});
}