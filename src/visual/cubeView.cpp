#include "cubeView.h"
#include <QTimer>
#include <cmath>

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
            angleX += 0.3f;
            angleY += 0.6f;
            update();
        });
    timer->start(16);
}

// =========================
void cubeView::initializeGL()
{
    initializeOpenGLFunctions();

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);

    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

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
// MAIN LOOP
// =========================
void cubeView::drawAllCubies()
{
    for (const auto& c : cubies)
    {
        glPushMatrix();
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
// UNIFIED MAPPING (FIX)
// =========================
inline int toFace(int v)
{
    return v + 1; // -1..1 ? 0..2
}

// =========================
void cubeView::drawCubie(const Cubie& c)
{
    float s = 0.5f;

    glBegin(GL_QUADS);

    // ================= FRONT (z = +1)
    if (c.z == 1)
    {
        Colour col = safeAt(2, toFace(-c.y), toFace(c.x));
        QColor qc = convertColour(col == Colour::UNKNOWN ? Colour::WHITE : col);

        glColor3f(qc.redF(), qc.greenF(), qc.blueF());

        glVertex3f(-s, -s, s);
        glVertex3f(s, -s, s);
        glVertex3f(s, s, s);
        glVertex3f(-s, s, s);
    }

    // ================= BACK (z = -1)
    if (c.z == -1)
    {
        Colour col = safeAt(4, toFace(-c.y), toFace(-c.x));
        QColor qc = convertColour(col == Colour::UNKNOWN ? Colour::WHITE : col);

        glColor3f(qc.redF(), qc.greenF(), qc.blueF());

        glVertex3f(-s, -s, -s);
        glVertex3f(-s, s, -s);
        glVertex3f(s, s, -s);
        glVertex3f(s, -s, -s);
    }

    // ================= LEFT (x = -1)
    if (c.x == -1)
    {
        Colour col = safeAt(1, toFace(-c.y), toFace(c.z));
        QColor qc = convertColour(col == Colour::UNKNOWN ? Colour::WHITE : col);

        glColor3f(qc.redF(), qc.greenF(), qc.blueF());

        glVertex3f(-s, -s, -s);
        glVertex3f(-s, -s, s);
        glVertex3f(-s, s, s);
        glVertex3f(-s, s, -s);
    }

    // ================= RIGHT (x = 1)
    if (c.x == 1)
    {
        Colour col = safeAt(3, toFace(-c.y), toFace(-c.z));
        QColor qc = convertColour(col == Colour::UNKNOWN ? Colour::WHITE : col);

        glColor3f(qc.redF(), qc.greenF(), qc.blueF());

        glVertex3f(s, -s, -s);
        glVertex3f(s, s, -s);
        glVertex3f(s, s, s);
        glVertex3f(s, -s, s);
    }

    // ================= UP (y = 1)
    if (c.y == 1)
    {
        Colour col = safeAt(0, toFace(c.z), toFace(c.x));
        QColor qc = convertColour(col == Colour::UNKNOWN ? Colour::WHITE : col);

        glColor3f(qc.redF(), qc.greenF(), qc.blueF());

        glVertex3f(-s, s, -s);
        glVertex3f(-s, s, s);
        glVertex3f(s, s, s);
        glVertex3f(s, s, -s);
    }

    // ================= DOWN (y = -1)
    if (c.y == -1)
    {
        Colour col = safeAt(5, toFace(-c.z), toFace(c.x));
        QColor qc = convertColour(col == Colour::UNKNOWN ? Colour::WHITE : col);

        glColor3f(qc.redF(), qc.greenF(), qc.blueF());

        glVertex3f(-s, -s, -s);
        glVertex3f(s, -s, -s);
        glVertex3f(s, -s, s);
        glVertex3f(-s, -s, s);
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
// EDGES (unchanged per cubie)
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

    // verticals
    glVertex3f(-s, -s, -s); glVertex3f(-s, s, -s);
    glVertex3f(s, -s, -s); glVertex3f(s, s, -s);
    glVertex3f(s, -s, s); glVertex3f(s, s, s);
    glVertex3f(-s, -s, s); glVertex3f(-s, s, s);

    glEnd();
}