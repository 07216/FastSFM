#ifndef ARCBALL_H
#define ARCBALL_H

#include <QPoint>

class ArcBall
{
public:
    ArcBall();
    ~ArcBall();

    void mouseDown(QPoint pos, int width, int height);
    void mouseMove(QPoint pos, int width, int height);
    void mouseUp(QPoint pos, int width, int height);

    void reset();

    float* getRotationMatrix();
    float* getInverseRotationMatrix();

private:
    struct Quaternion
    {
        float x, y, z, w;
    };
    typedef float HMatrix[4][4];
    typedef Quaternion HVec;

    Quaternion qDrag;
    HVec pLast, pCur;
    HMatrix rotationMatrix;
    HMatrix invRotationMatrix;

    bool isDragging;

private:
    Quaternion qConj(const Quaternion &q);
    Quaternion qMul(const Quaternion &qL, const Quaternion &qR);

    float vDot(HVec v, HVec w);
    HVec vCross(HVec v, HVec w);

    HVec toVec(int x, int y, int width, int height);
    Quaternion toQuat(HVec pFirst, HVec pSecond);
    void toMat(Quaternion q, HMatrix &out);

    void makeIdentity(HMatrix &m);

    void update();
};

#endif // ARCBALL_H
