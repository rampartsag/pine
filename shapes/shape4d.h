#ifndef SHAPE4D_H
#define SHAPE4D_H

#include "../math/vector.h"
#include <vector>

class Shape4D
{
public:
    Shape4D();
    virtual ~Shape4D();
    //virtual void get(struct Vertex*, int*, int*) = 0;
    struct Vertex {
        union Vector::Vec pos;
        union Vector::Vec colors;
        union Vector::Vec norm;
    };
    void toGLfaces(struct Vertex**, int**, int*);
    void toGLedges(struct Vertex**, int**, int*);
    void toGLaxes(Vertex **v);
    float* get();
    float* getAxes();
    int size();
    void collapse();
    unsigned int vbov, vboi;
    virtual void test(int);

protected:
    float longest();
    float *vectors4D, *vectors3D;
    float *axes4D, *axes3D;
    union Vector::Vec colorSet[18];
    struct Geom {
        float metric;
        int vertNum, edgeNum, faceNum, edgesPerFace;
        const float *verts;
        const int *edges, *faces;
    } geom;
    void setGeometry();
    struct Vertex *GLfaceVerts, *GLedgeVerts, *GLaxes;
    int *GLfaceIndices, *GLedgeIndices;
    int GLfaceIndexNum, GLedgeIndexNum;
    int vertNum;

};

#endif // SHAPE4D_H
