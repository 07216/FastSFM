#ifndef OPENGL_H
#define OPENGL_H

//#include <QGLFormat>
//#include <QGLWidget>
#include <QMouseEvent>
#include <QEvent>
#include "arcball.h"
#include <string>
//#include "mve/libs/mve/mesh.h"

class COpenGL : public QGLWidget
{
public:
    COpenGL(const QGLFormat &format, QWidget *parent = 0);
    ~COpenGL();

    void read_mesh(char *filename);
//    void read_mesh(mve::TriangleMesh mesh);
protected:

    void initializeGL();
    void resizeGL(int w, int h);
    void paintGL();
    void mousePressEvent(QMouseEvent *e);
    void mouseMoveEvent(QMouseEvent *e);
    void mouseReleaseEvent(QMouseEvent *e);

public:

    void Render();

private:
    ArcBall *arcball;
    bool has_mesh_;

//    mve::TriangleMesh mesh_;
};

#endif
