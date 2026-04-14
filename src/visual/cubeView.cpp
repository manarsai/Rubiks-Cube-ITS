#include "cubeView.h"
#include <QTimer>
#include <cmath>

cubeView::cubeView(QWidget* parent)
    : QOpenGLWidget(parent),
    angleX(20.0f),
    angleY(30.0f)
{
    // ?? Create 27 cubies
    for (int x = -1; x <= 1; x++) {
        for (int y = -1; y <= 1; y++) {
            for (int z = -1; z <= 1; z++) {
                cubies.push_back({ x, y, z });
            }
        }
    }

    // ?? Animation loop
    QTimer* timer = new QTimer(this);

    connect(timer, &QTimer::timeout, this, [this]() {
        angleX += 0.3f;
        angleY += 0.6f;
        update();
        });

    timer->start(16);
}

void cubeView::initializeGL()
{
    initializeOpenGLFunctions();

    glEnable(GL_DEPTH_TEST);
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
}

void cubeView::resizeGL(int w, int h)
{
    glViewport(0, 0, w, h);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    float aspect = float(w) / float(h);

    float fov = 45.0f;
    float nearPlane = 0.1f;
    float farPlane = 100.0f;

    float top = nearPlane * tan(fov * 3.1415926f / 360.0f);
    float bottom = -top;
    float right = top * aspect;
    float left = -right;

    glFrustum(left, right, bottom, top, nearPlane, farPlane);

    glMatrixMode(GL_MODELVIEW);
}

void cubeView::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glLoadIdentity();
    glTranslatef(0.0f, 0.0f, -8.0f);

    glRotatef(angleX, 1.0f, 0.0f, 0.0f);
    glRotatef(angleY, 0.0f, 1.0f, 0.0f);

    drawAllCubies();
}

void cubeView::drawAllCubies()
{
    for (auto& c : cubies)
    {
        glPushMatrix();

        glTranslatef(c.x * 1.1f, c.y * 1.1f, c.z * 1.1f);

        // 1. draw colored cube
        drawUnitCube();

        // 2. draw black edges on top
        drawCubeEdges();

        glPopMatrix();
    }
}

void cubeView::drawUnitCube()
{
    glBegin(GL_QUADS);

    // FRONT (red)
    glColor3f(1, 0, 0);
    glVertex3f(-0.5f, -0.5f, 0.5f);
    glVertex3f(0.5f, -0.5f, 0.5f);
    glVertex3f(0.5f, 0.5f, 0.5f);
    glVertex3f(-0.5f, 0.5f, 0.5f);

    // BACK (orange)
    glColor3f(1, 0.5f, 0);
    glVertex3f(-0.5f, -0.5f, -0.5f);
    glVertex3f(-0.5f, 0.5f, -0.5f);
    glVertex3f(0.5f, 0.5f, -0.5f);
    glVertex3f(0.5f, -0.5f, -0.5f);

    // LEFT (green)
    glColor3f(0, 1, 0);
    glVertex3f(-0.5f, -0.5f, -0.5f);
    glVertex3f(-0.5f, -0.5f, 0.5f);
    glVertex3f(-0.5f, 0.5f, 0.5f);
    glVertex3f(-0.5f, 0.5f, -0.5f);

    // RIGHT (blue)
    glColor3f(0, 0, 1);
    glVertex3f(0.5f, -0.5f, -0.5f);
    glVertex3f(0.5f, 0.5f, -0.5f);
    glVertex3f(0.5f, 0.5f, 0.5f);
    glVertex3f(0.5f, -0.5f, 0.5f);

    // TOP (white)
    glColor3f(1, 1, 1);
    glVertex3f(-0.5f, 0.5f, -0.5f);
    glVertex3f(-0.5f, 0.5f, 0.5f);
    glVertex3f(0.5f, 0.5f, 0.5f);
    glVertex3f(0.5f, 0.5f, -0.5f);

    // BOTTOM (yellow)
    glColor3f(1, 1, 0);
    glVertex3f(-0.5f, -0.5f, -0.5f);
    glVertex3f(0.5f, -0.5f, -0.5f);
    glVertex3f(0.5f, -0.5f, 0.5f);
    glVertex3f(-0.5f, -0.5f, 0.5f);

    glEnd();
}

void cubeView::drawCubeEdges()
{

    glColor3f(0, 0, 0); // black edges
    glLineWidth(10.0f);

    glBegin(GL_LINES);

    float s = 0.5f;

    // bottom square
    glVertex3f(-s, -s, -s); glVertex3f(s, -s, -s);
    glVertex3f(s, -s, -s); glVertex3f(s, -s, s);
    glVertex3f(s, -s, s); glVertex3f(-s, -s, s);
    glVertex3f(-s, -s, s); glVertex3f(-s, -s, -s);

    // top square
    glVertex3f(-s, s, -s); glVertex3f(s, s, -s);
    glVertex3f(s, s, -s); glVertex3f(s, s, s);
    glVertex3f(s, s, s); glVertex3f(-s, s, s);
    glVertex3f(-s, s, s); glVertex3f(-s, s, -s);

    // vertical lines
    glVertex3f(-s, -s, -s); glVertex3f(-s, s, -s);
    glVertex3f(s, -s, -s); glVertex3f(s, s, -s);
    glVertex3f(s, -s, s); glVertex3f(s, s, s);
    glVertex3f(-s, -s, s); glVertex3f(-s, s, s);

    glEnd();
}