#ifndef VECTOR_H
#define VECTOR_H
#include <GLES2/gl2.h>


class Vector
{
public:
    Vector();
    ~Vector();
    union Vec{
        struct {
            float x, y, z, w;
        };
        struct {
            float r, g, b, a;
        };
        float v[4];
    };
    static void cross(Vec *v0, Vec *v1, Vec *v2, Vec *out);
    static void add(Vec *v0, Vec *v1, Vec *v2, Vec *out);
    static void dot(Vec *v0, Vec *v1, Vec *v2, float *out);
    static void length(Vec *v0, Vec *v1, float *out);
    static void angle(Vec *v0, Vec *v1, Vec *v2, float *out);
    static void norm(Vec *in, Vec *out);
    static void norm(Vec *norm);
    static void rotate(GLfloat angle, Vec *v0, Vec *out, Vec *v3);
    static void add(Vec *v1, Vec *v2, Vec *out);
};

#endif // VECTOR_H
