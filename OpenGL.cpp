#include "OpenGL.h"
#include <algorithm>
#include <iostream>

COpenGL::COpenGL(const QGLFormat &format, QWidget *parent)
    : QGLWidget(format, parent)
{ 
    arcball = new ArcBall;
    has_mesh_ = false;
}

COpenGL::~COpenGL()
{
}

void COpenGL::initializeGL()
{
    glClearColor(1.0, 1.0, 1.0, 0.0);
    glShadeModel(GL_SMOOTH);
    glEnable(GL_DOUBLEBUFFER);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_POINT_SMOOTH);
    glEnable(GL_LINE_SMOOTH);
}

void COpenGL::resizeGL(int w, int h)
{
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    float xSpan = 1;
    float ySpan = 1;

    // Make sure the shorter side has length 1
    if(w > h){
    	xSpan = float(w) / h;
    }
    else{
    	ySpan = float(h) / w;
    }

    glOrtho(-xSpan, xSpan, -ySpan, ySpan, -1, 1);

    // Use the entire window for rendering.
    glViewport(0, 0, w, h);
}

void COpenGL::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glPushMatrix();
    glMultMatrixf(arcball->getInverseRotationMatrix());
    Render();
    glPopMatrix();
}

void COpenGL::Render()
{
    if (has_mesh_)
    {
        glBegin(GL_TRIANGLES);

        glEnd();
    }
}

void COpenGL::mousePressEvent(QMouseEvent *e)
{
    if (e->button() == Qt::LeftButton)
    {
        if (has_mesh_)
        {
            arcball->mouseDown(e->pos(),width(),height());
        }

        updateGL();
    }
}

void COpenGL::mouseMoveEvent(QMouseEvent *e)
{
    if (has_mesh_)
    {
        arcball->mouseMove(e->pos(),width(),height());
        updateGL();

    }
}

void COpenGL::mouseReleaseEvent(QMouseEvent *e)
{
    if (e->button() == Qt::LeftButton)
    {
        if (has_mesh_)
        {
            arcball->mouseUp(e->pos(), width(), height());
        }
        updateGL();
    }
    else if (e->button() == Qt::RightButton)
    {
        if (has_mesh_)
        {
            arcball->reset();
        }
        updateGL();
    }
}

void COpenGL::read_mesh(char* filename)
{
    // read from file...

    has_mesh_ = true;
    updateGL();
}

//void COpenGL::read_mesh(mve::TriangleMesh mesh)
//{
//    mesh_ = mesh;
//    has_mesh_ = true;
//    updateGL();
//}
