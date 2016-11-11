#include "arcball.h"
#include <math.h>

ArcBall::ArcBall()
{
    reset();
}

ArcBall::~ArcBall()
{

}

ArcBall::Quaternion ArcBall::qConj(const Quaternion &q)
{
    Quaternion qconj;
    qconj.x = -q.x;
    qconj.y = -q.y;
    qconj.z = -q.z;
    qconj.w = q.w;
    return qconj;
}

ArcBall::Quaternion ArcBall::qMul(const Quaternion &qL, const Quaternion &qR)
{
    Quaternion qmul;
    qmul.x = qL.w*qR.x + qL.x*qR.w + qL.y*qR.z - qL.z*qR.y;
    qmul.y = qL.w*qR.y + qL.y*qR.w + qL.z*qR.x - qL.x*qR.z;
    qmul.z = qL.w*qR.z + qL.z*qR.w + qL.x*qR.y - qL.y*qR.x;
    qmul.w = qL.w*qR.w - qL.x*qR.x - qL.y*qR.y - qL.z*qR.z;
    return (qmul);
}

float ArcBall::vDot(HVec v, HVec w)
{
    return (v.x*w.x + v.y*w.y + v.z*w.z);
}

ArcBall::HVec ArcBall::vCross(HVec v, HVec w)
{
    HVec c;
    c.x = v.y*w.z - v.z*w.y;
    c.y = v.z*w.x - v.x*w.z;
    c.z = v.x*w.y - v.y*w.x;
    return c;
}

ArcBall::HVec ArcBall::toVec(int x, int y, int width, int height)
{
    float vx = (float)x/width*2-1;
    float vy = (float)y/height*2-1;
    float r = sqrtf(vx*vx+vy*vy);
    HVec v;
    if (r > 1)
    {
        v.x = vx/r;
        v.y = vy/r;
        v.z = 0;
    }
    else
    {
        v.x = vx;
        v.y = vy;
        v.z = sqrtf(1-r*r);
    }
    v.w = 0;
    return v;
}

void ArcBall::toMat(Quaternion q, HMatrix &out)
{
    float Nq = q.x*q.x + q.y*q.y + q.z*q.z + q.w*q.w;
    float s = (Nq > 0.0) ? (2.0 / Nq) : 0.0;
    float xs = q.x*s,	      ys = q.y*s,	  zs = q.z*s;
    float wx = q.w*xs,	      wy = q.w*ys,	  wz = q.w*zs;
    float xx = q.x*xs,	      xy = q.x*ys,	  xz = q.x*zs;
    float yy = q.y*ys,	      yz = q.y*zs,	  zz = q.z*zs;
    out[0][0] = 1.0 - (yy + zz);
    out[1][0] = xy + wz;
    out[2][0] = xz - wy;
    out[0][1] = xy - wz;
    out[1][1] = 1.0 - (xx + zz);
    out[2][1] = yz + wx;
    out[0][2] = xz + wy;
    out[1][2] = yz - wx;
    out[2][2] = 1.0 - (xx + yy);
    out[0][3] = out[1][3] = out[2][3] = out[3][0] = out[3][1] = out[3][2] = 0.0f;
    out[3][3] = 1.0f;
}

ArcBall::Quaternion ArcBall::toQuat(HVec pFirst, HVec pSecond)
{
    Quaternion q = vCross(pFirst, pSecond);
    float w = vDot(pFirst, pSecond);
    q.w = w;
    return q;
}

void ArcBall::mouseDown(QPoint pos, int width, int height)
{
    pLast = pCur = toVec(pos.x(), height - pos.y(), width, height);
    isDragging = true;
}

void ArcBall::mouseMove(QPoint pos, int width, int height)
{
    if(isDragging)
    {
        pCur = toVec(pos.x(), height - pos.y(), width, height);
        update();
    }
}

void ArcBall::mouseUp(QPoint pos, int width, int height)
{
    pCur = toVec(pos.x(), height - pos.y(), width, height);
    isDragging = false;
    update();
}

void ArcBall::reset()
{
    makeIdentity(rotationMatrix);

    qDrag.x = 0;
    qDrag.y = 0;
    qDrag.z = 0;
    qDrag.w = 1;

    isDragging = false;
}

float* ArcBall::getRotationMatrix()
{
    float *ptr = (float *)rotationMatrix;
    return ptr;
}

void ArcBall::update()
{
    qDrag = qMul(toQuat(pLast, pCur), qDrag);
    toMat(qConj(qDrag), rotationMatrix);
    pLast = pCur;
}

void ArcBall::makeIdentity(HMatrix &m)
{
    for (int i = 0; i < 4; ++i)
    {
        for (int j = 0; j < 4; ++j)
        {
            if (i==j)
            {
                m[i][j] = 1;
            }
            else
            {
                m[i][j] = 0;
            }
        }
    }
}

float* ArcBall::getInverseRotationMatrix()
{
        int i, j, k, swap;
        double t;
        float temp[4][4];
        float *src = getRotationMatrix();

        for (i = 0; i < 4; i++)
        {
            for (j = 0; j < 4; j++)
            {
                temp[i][j] = src[i * 4 + j];
            }
        }

        makeIdentity(invRotationMatrix);

        // colume major Gaussian elimination
        for (i = 0; i < 4; i++)
        {
            swap = i;
            for (j = i + 1; j < 4; j++)
            {
                if (fabs(temp[j][i]) > fabs(temp[i][i]))
                {
                    swap = j;
                }
            }

            if (swap != i)
            {
                for (k = 0; k < 4; k++)
                {
                    t = temp[i][k];
                    temp[i][k] = temp[swap][k];
                    temp[swap][k] = (float)t;

                    t = invRotationMatrix[i][k];
                    invRotationMatrix[i][k] = invRotationMatrix[swap][k];
                    invRotationMatrix[swap][k] = (float)t;
                }
            }
            t = temp[i][i];
            for (k = 0; k < 4; k++)
            {
                temp[i][k] /= (float)t;
                invRotationMatrix[i][k] /= (float)t;
            }
            for (j = 0; j < 4; j++)
            {
                if (j != i)
                {
                    t = temp[j][i];
                    for (k = 0; k < 4; k++)
                    {
                        temp[j][k] -= temp[i][k] * (float)t;
                        invRotationMatrix[j][k] -= invRotationMatrix[i][k] * (float)t;
                    }
                }
            }
        }
        float *ptr = (float *)invRotationMatrix;
        return ptr;
}


