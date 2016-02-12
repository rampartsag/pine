#include "vector.h"
#include <cmath>
#include <iostream>

Vector::Vector()
{

}

Vector::~Vector()
{

}

void Vector::cross(Vector::Vec *v0, Vector::Vec *v1, Vector::Vec *v2, Vector::Vec *out)
{
    union Vector::Vec a, b;
    for(int i=0; i<3; i++) {
        a.v[i] = v1->v[i] - v0->v[i];
        b.v[i] = v2->v[i] - v0->v[i];
    }

    out->v[0] = a.y*b.z - a.z*b.y;
    out->v[1] = a.z*b.x - a.x*b.z;
    out->v[2] = a.x*b.y - a.y*b.x;

}

void Vector::add(Vector::Vec *v0, Vector::Vec *v1, Vector::Vec *v2, Vector::Vec *out)
{
    union Vector::Vec a, b;
    for(int i=0; i<4; i++) {
        a.v[i] = v1->v[i] - v0->v[i];
        b.v[i] = v2->v[i] - v0->v[i];
    }
    out->x = a.x + b.x;
    out->y = a.y + b.y;
    out->z = a.z + b.z;
    out->w = a.w + b.w;

}

void Vector::dot(Vector::Vec *v0, Vector::Vec *v1, Vector::Vec *v2, float *out)
{
    union Vector::Vec a, b;
    for(int i=0; i<4; i++) {
        a.v[i] = v1->v[i] - v0->v[i];
        b.v[i] = v2->v[i] - v0->v[i];
    }
    float c = a.x*b.x + a.y*b.y + a.z*b.z + a.w*b.w;
    *out = c;



}

void Vector::length(Vector::Vec *v0, Vector::Vec *v1, float *out)
{
    union Vector::Vec a;
    for(int i=0; i<4; i++)
        a.v[i] = v1->v[i] - v0->v[i];
    float c = a.x*a.x + a.y*a.y + a.z*a.z + a.w*a.w;
    *out = sqrt(c);

}

void Vector::angle(Vector::Vec *v0, Vector::Vec *v1, Vector::Vec *v2, float *out)
{
    float dotab, la, lb;
    dot(v0, v1, v2, &dotab);
    length(v0, v1, &la);
    length(v0, v2, &lb);
    float c = acos(dotab/(la*lb));
    float degrees = c*180/M_PI;
    *out = degrees;

}

void Vector::norm(Vector::Vec *in, Vector::Vec *out)
{
    out = in;
    GLfloat norm = sqrt(out->x*out->x + out->y*out->y + out->z*out->z);
    out->x /= norm;
    out->y /= norm;
    out->z /= norm;


}

void Vector::norm(Vector::Vec *norm) {
    GLfloat n = sqrt(norm->x*norm->x + norm->y*norm->y + norm->z*norm->z);
    norm->x /= n;
    norm->y /= n;
    norm->z /= n;

}

void Vector::rotate(GLfloat angle, Vector::Vec *v0, Vector::Vec *out, Vector::Vec *v3)
{

    Vector::Vec axis;
    cross(v0, out, v3, &axis);
    norm(&axis);
    angle *= M_PI/180;
    double ux = axis.x;
    double uy = axis.y;
    double uz = axis.z;
    double tp = 1 - cos(angle);

    double rot[9] = {
            cos(angle) + ux*ux*tp,		ux*uy*tp - uz*sin(angle),		ux*uz*tp + uy*sin(angle),
            uy*ux*tp+uz*sin(angle),		cos(angle)+uy*uy*tp,			uy*uz*tp - ux*sin(angle),
            uz*ux*tp-uy*sin(angle),		uz*uy*tp + ux*sin(angle),		cos(angle) + uz*uz*tp,
    };

    out->x = rot[0]*out->x + rot[1]*out->y + rot[2]*out->z;
    out->y = rot[3]*out->x + rot[4]*out->y + rot[5]*out->z;
    out->z = rot[6]*out->x + rot[7]*out->y + rot[8]*out->z;
}

void Vector::add(Vector::Vec *v1, Vector::Vec *v2, Vector::Vec *out)
{
    out->x = v1->x + v2->x;
    out->y = v1->y + v2->y;
    out->z = v1->z + v2->z;
    out->w = v1->w + v2->w;

}

