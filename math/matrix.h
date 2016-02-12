#include <GLES2/gl2.h>
#ifndef MATRIX_H
#define MATRIX_H


class Matrix
{
public:
    Matrix();
    ~Matrix();
    enum plane{XY, YZ, ZX, XW, YW, ZW};
    void reset();
    void rotate4D(float angle, float angle2, plane p);
    void rotate4D_any(float, float, float, float);
    void rotate(float, float, float, float, float);
    void perspective3D(float angle, float ratio, float near, float far);
    void translate3d(float x, float y, float z);
    void mmult(GLfloat *matrixA, GLfloat *matrixB);
    static void vmult(GLfloat *matrixA, GLfloat *vectorB, int count);
    void transpose();
    void transpose(GLfloat *out);
    static void transpose3x3(GLfloat *in, GLfloat *out);
    void convert3x3(GLfloat *out);
    bool invert(Matrix *matOut);
    static bool invert3x3(GLfloat *in, GLfloat *out);
    void set(
            GLfloat a11, GLfloat a12, GLfloat a13, GLfloat a14,
            GLfloat a21, GLfloat a22, GLfloat a23, GLfloat a24,
            GLfloat a31, GLfloat a32, GLfloat a33, GLfloat a34,
            GLfloat a41, GLfloat a42, GLfloat a43, GLfloat a44
             );
    GLfloat* get();


private:
    GLfloat mat[16];
};

#endif // MATRIX_H
