#include <cmath>
#include <iostream>
#include "matrix.h"


Matrix::Matrix()
{
    reset();
}

Matrix::~Matrix()
{

}

void Matrix::reset()
{
    int n = 4;
    for(int i=0; i<n; i++){
        for(int j=0; j<n; j++){
            mat[(n*i)+j] = i == j ? 1 : 0;

        };
    };
}

void Matrix::rotate4D(float angle, float angle2, Matrix::plane p)
{
    angle *= M_PI/180;
    angle2 *= M_PI/180;
    Matrix rmat;
    switch (p) {
    case Matrix::XY:
        rmat.set(
                    cos(angle), sin(angle), 0, 0,
                    -sin(angle), cos(angle), 0, 0,
                    0, 0, cos(angle2), -sin(angle2),
                    0, 0, sin(angle2), cos(angle2)
                );
        break;
    case Matrix::YZ:
        rmat.set(
                    cos(angle2), 0, 0, sin(angle2),
                    0, cos(angle), sin(angle), 0,
                    0, -sin(angle), cos(angle), 0,
                    -sin(angle2), 0, 0, cos(angle2)
                );
        break;
    case Matrix::ZX:
        rmat.set(
                    cos(angle), 0, -sin(angle), 0,
                    0, cos(angle2), 0, -sin(angle2),
                    sin(angle), 0, cos(angle), 0,
                    0, sin(angle2), 0, cos(angle2)
                );
        break;
    case Matrix::XW:
        rmat.set(
                    cos(angle), 0, 0, sin(angle),
                    0, cos(angle2), sin(angle2), 0,
                    0, -sin(angle2), cos(angle2), 0,
                    -sin(angle), 0, 0, cos(angle)
                );
        break;
    case Matrix::YW:
        rmat.set(
                    cos(angle2), 0, -sin(angle2), 0,
                    0, cos(angle), 0, -sin(angle),
                    sin(angle2), 0, cos(angle2), 0,
                    0, sin(angle), 0, cos(angle)
                );
        break;
    case Matrix::ZW:
        rmat.set(
                    cos(angle2), sin(angle2), 0, 0,
                    -sin(angle2), cos(angle2), 0, 0,
                    0, 0, cos(angle), -sin(angle),
                    0, 0, sin(angle), cos(angle)
                );
        break;
    }
    mmult(mat, rmat.get());
    return;


}

void Matrix::rotate4D_any(float angle, float x, float y, float z)
{
    angle *= M_PI/180;
    Matrix rmat;
    float A = x*x + y*y + z*z;
    float ux = x/sqrt(A);
    float uy = y/sqrt(A);
    float uz = z/sqrt(A);
    float tp = 1 - cos(angle);
    rmat.set(
                cos(angle) + ux*ux*tp,		ux*uy*tp - uz*sin(angle),		ux*uz*tp + uy*sin(angle),	0,
                uy*ux*tp+uz*sin(angle),		cos(angle)+uy*uy*tp,			uy*uz*tp - ux*sin(angle),	0,
                uz*ux*tp-uy*sin(angle),		uz*uy*tp + ux*sin(angle),		cos(angle) + uz*uz*tp,		0,
                0,							0,								0,							1
                );

    mmult(mat, rmat.get());

}

void Matrix::rotate(float angle, float x, float y, float z, float w)
{
    angle *= M_PI/180;
    Matrix rmat;
    float A = x*x + y*y + z*z + w*w;
    float ux = x/sqrt(A);
    float uy = y/sqrt(A);
    float uz = z/sqrt(A);
    float uw = w/sqrt(A);
    float vs = 1 - cos(angle);
    float cs = cos(angle);
    float sn = sin(angle);

    float c11 = cs + ux*ux*vs;
    float c12 = ux*uy*uz*vs - uw*sn;
    float c13 = ux*uy*uw*vs + uz*sn;
    float c14 = ux*uz*uw*vs + uy*sn;

    float c21 = ux*uy*uz*vs + uw*sn;
    float c22 = cs + uy*uy*vs;
    float c23 = ux*uy*uw*vs - uz*sn;
    float c24 = uy*uz*uw*vs + ux*sn;

    float c31 = ux*uy*uz*vs + uw*sn;
    float c32 = ux*uz*uw*vs + uy*sn;
    float c33 = cs + uz*uz*vs;
    float c34 = uy*uz*uw*vs - ux*sn;

    float c41 = ux*uy*uw*vs - uz*sn;
    float c42 = ux*uz*uw*vs + uy*sn;
    float c43 = uy*uz*uw*vs + ux*sn;
    float c44 = cs + uw*uw*vs;

    rmat.set(
                c11, c12, c13, c14,
                c21, c22, c23, c24,
                c31, c32, c33, c34,
                c41, c42, c43, c44
                );

    mmult(mat, rmat.get());



}

void Matrix::perspective3D(float angle, float ratio, float near, float far)
{
    angle *= M_PI/180;
    Matrix pmat;
    pmat.set(
                (2*sin(angle))/ratio, 0, 0, 0,
                 0, 2*sin(angle), 0, 0,
                 0, 0, -(far+near)/(far-near), -(2*far*near)/(far-near),
                 0, 0, -1, 0
                );
    mmult(mat, pmat.get());
    return;


}

void Matrix::translate3d(float x, float y, float z)
{
    Matrix tmat;
    tmat.set(
                1, 0, 0, x,
                0, 1, 0, y,
                0, 0, 1, z,
                0, 0, 0, 1
                );
    mmult(mat, tmat.get());
    return;

}

void Matrix::mmult(GLfloat *matrixA, GLfloat *matrixB)
{
    int msize = 4;
    GLfloat c[16];
    for(int i=0; i<16; i++)
        c[i] = 0.0f;
    for(int i=0; i<msize; i++)
        for(int j=0; j<msize; j++)
            for(int k=0; k<msize; k++)
                c[(msize*i)+j] += matrixA[(msize*i)+k]*matrixB[(msize*k)+j];
    // copy c to matrixA
    for(int i=0; i<16; i++)
        matrixA[i] = c[i];

}

void Matrix::vmult(GLfloat *matrixA, GLfloat *vectorB, int count)
{
    for(int m=0; m<count; m++) {
        GLfloat c[4];
        for(int i=0; i<4; i++)
            c[i] = 0.0f;
        for(int i=0; i<4; i++)
            for(int j=0; j<4; j++)
                c[i] += matrixA[(4*i)+j] * vectorB[(4*m)+j];

        for(int i=0; i<4; i++)
            vectorB[(4*m)+i] = c[i];

    };
}

void Matrix::transpose()
{
    GLfloat temp;
    temp = mat[4]; mat[4] = mat[1]; mat[1] = temp;
    temp = mat[8]; mat[8] = mat[2]; mat[2] = temp;
    temp = mat[12]; mat[12] = mat[3]; mat[3] = temp;
    temp = mat[9]; mat[9] = mat[6]; mat[6] = temp;
    temp = mat[13]; mat[13] = mat[7]; mat[7] = temp;
    temp = mat[14]; mat[14] = mat[11]; mat[11] = temp;
}

void Matrix::transpose(GLfloat *out) {
    out[1] = mat[4]; out[4] = mat[1];
    out[2] = mat[8]; out[8] = mat[2];
    out[3] = mat[12]; out[12] = mat[3];

    out[6] = mat[9]; out[9] = mat[6];
    out[7] = mat[13]; out[13] = mat[7];

    out[11] = mat[14]; out[14] = mat[11];
}

void Matrix::transpose3x3(GLfloat *in, GLfloat *out) {
    out[1] = in[3]; out[3] = in[1];
    out[2] = in[6]; out[6] = in[2];
    out[5] = in[7]; out[7] = in[5];

}

void Matrix::convert3x3(GLfloat *out) {
    out[0] = mat[0]; out[1] = mat[1]; out[2] = mat[2];
    out[3] = mat[4]; out[4] = mat[5]; out[5] = mat[6];
    out[6] = mat[8]; out[7] = mat[9]; out[8] = mat[10];
}

bool Matrix::invert3x3(GLfloat *in, GLfloat *out) {
    GLfloat inv[9], det;

    inv[0] = in[4] * in[8] - in[5] * in[7];
    inv[1] = in[2] * in[7] - in[1] * in[8];
    inv[2] = in[1] * in[5] - in[2] * in[4];

    inv[3] = in[5] * in[6] - in[3] * in[8];
    inv[4] = in[0] * in[8] - in[2] * in[6];
    inv[5] = in[2] * in[3] - in[0] * in[5];

    inv[6] = in[3] * in[7] - in[4] * in[6];
    inv[7] = in[1] * in[6] - in[0] * in[7];
    inv[8] = in[0] * in[4] - in[1] * in[3];

    det = in[0] * inv[0] + in[1] * inv[3] + in[2] * inv[6];

    if (det == 0)
        return false;

    det = 1.0 / det;

    for (int i = 0; i < 9; i++)
        out[i] = inv[i] * det;

    return true;

}

bool Matrix::invert(Matrix *matOut)
{
    GLfloat *m = mat;
    GLfloat inv[16], det;

    inv[0] = m[5]  * m[10] * m[15] -
             m[5]  * m[11] * m[14] -
             m[9]  * m[6]  * m[15] +
             m[9]  * m[7]  * m[14] +
             m[13] * m[6]  * m[11] -
             m[13] * m[7]  * m[10];

    inv[4] = -m[4]  * m[10] * m[15] +
              m[4]  * m[11] * m[14] +
              m[8]  * m[6]  * m[15] -
              m[8]  * m[7]  * m[14] -
              m[12] * m[6]  * m[11] +
              m[12] * m[7]  * m[10];

    inv[8] = m[4]  * m[9] * m[15] -
             m[4]  * m[11] * m[13] -
             m[8]  * m[5] * m[15] +
             m[8]  * m[7] * m[13] +
             m[12] * m[5] * m[11] -
             m[12] * m[7] * m[9];

    inv[12] = -m[4]  * m[9] * m[14] +
               m[4]  * m[10] * m[13] +
               m[8]  * m[5] * m[14] -
               m[8]  * m[6] * m[13] -
               m[12] * m[5] * m[10] +
               m[12] * m[6] * m[9];

    inv[1] = -m[1]  * m[10] * m[15] +
              m[1]  * m[11] * m[14] +
              m[9]  * m[2] * m[15] -
              m[9]  * m[3] * m[14] -
              m[13] * m[2] * m[11] +
              m[13] * m[3] * m[10];

    inv[5] = m[0]  * m[10] * m[15] -
             m[0]  * m[11] * m[14] -
             m[8]  * m[2] * m[15] +
             m[8]  * m[3] * m[14] +
             m[12] * m[2] * m[11] -
             m[12] * m[3] * m[10];

    inv[9] = -m[0]  * m[9] * m[15] +
              m[0]  * m[11] * m[13] +
              m[8]  * m[1] * m[15] -
              m[8]  * m[3] * m[13] -
              m[12] * m[1] * m[11] +
              m[12] * m[3] * m[9];

    inv[13] = m[0]  * m[9] * m[14] -
              m[0]  * m[10] * m[13] -
              m[8]  * m[1] * m[14] +
              m[8]  * m[2] * m[13] +
              m[12] * m[1] * m[10] -
              m[12] * m[2] * m[9];

    inv[2] = m[1]  * m[6] * m[15] -
             m[1]  * m[7] * m[14] -
             m[5]  * m[2] * m[15] +
             m[5]  * m[3] * m[14] +
             m[13] * m[2] * m[7] -
             m[13] * m[3] * m[6];

    inv[6] = -m[0]  * m[6] * m[15] +
              m[0]  * m[7] * m[14] +
              m[4]  * m[2] * m[15] -
              m[4]  * m[3] * m[14] -
              m[12] * m[2] * m[7] +
              m[12] * m[3] * m[6];

    inv[10] = m[0]  * m[5] * m[15] -
              m[0]  * m[7] * m[13] -
              m[4]  * m[1] * m[15] +
              m[4]  * m[3] * m[13] +
              m[12] * m[1] * m[7] -
              m[12] * m[3] * m[5];

    inv[14] = -m[0]  * m[5] * m[14] +
               m[0]  * m[6] * m[13] +
               m[4]  * m[1] * m[14] -
               m[4]  * m[2] * m[13] -
               m[12] * m[1] * m[6] +
               m[12] * m[2] * m[5];

    inv[3] = -m[1] * m[6] * m[11] +
              m[1] * m[7] * m[10] +
              m[5] * m[2] * m[11] -
              m[5] * m[3] * m[10] -
              m[9] * m[2] * m[7] +
              m[9] * m[3] * m[6];

    inv[7] = m[0] * m[6] * m[11] -
             m[0] * m[7] * m[10] -
             m[4] * m[2] * m[11] +
             m[4] * m[3] * m[10] +
             m[8] * m[2] * m[7] -
             m[8] * m[3] * m[6];

    inv[11] = -m[0] * m[5] * m[11] +
               m[0] * m[7] * m[9] +
               m[4] * m[1] * m[11] -
               m[4] * m[3] * m[9] -
               m[8] * m[1] * m[7] +
               m[8] * m[3] * m[5];

    inv[15] = m[0] * m[5] * m[10] -
              m[0] * m[6] * m[9] -
              m[4] * m[1] * m[10] +
              m[4] * m[2] * m[9] +
              m[8] * m[1] * m[6] -
              m[8] * m[2] * m[5];

    det = m[0] * inv[0] + m[1] * inv[4] + m[2] * inv[8] + m[3] * inv[12];

    if (det == 0)
        return false;

    det = 1.0 / det;

    matOut->set(
                inv[0]*det, inv[1]*det, inv[2]*det, inv[3]*det,
                inv[4]*det, inv[5]*det, inv[6]*det, inv[7]*det,
                inv[8]*det, inv[9]*det, inv[10]*det, inv[11]*det,
                inv[12]*det, inv[13]*det, inv[14]*det, inv[15]*det
               );

    return true;
}


void Matrix::set(GLfloat a11, GLfloat a12, GLfloat a13, GLfloat a14,
                 GLfloat a21, GLfloat a22, GLfloat a23, GLfloat a24,
                 GLfloat a31, GLfloat a32, GLfloat a33, GLfloat a34,
                 GLfloat a41, GLfloat a42, GLfloat a43, GLfloat a44)
{
    mat[0]  = a11; mat[1]  = a12; mat[2]  = a13; mat[3]  = a14;
    mat[4]  = a21; mat[5]  = a22; mat[6]  = a23; mat[7]  = a24;
    mat[8]  = a31; mat[9]  = a32; mat[10] = a33; mat[11] = a34;
    mat[12] = a41; mat[13] = a42; mat[14] = a43; mat[15] = a44;

}

GLfloat *Matrix::get()
{
    return mat;

}

