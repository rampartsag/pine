#include "shape4d.h"
#include <iostream>
#include <random>

Shape4D::Shape4D() :
    vbov(0),
    vboi(0)
{
    axes4D = new float[32];
    axes3D = new float[24];

    for(int i=0; i<32; i++)
        axes4D[i] = 0.0f;
    axes4D[4] = 1.0f;
    axes4D[13] = 1.0f;
    axes4D[22] = 1.0f;
    axes4D[31] = 1.0f;

    float alpha = 0.115f;
    std::vector<std::vector<float>> clrs =
    {
        { 0.0f, 0.0f, 1.0f, alpha },
        { 0.0f, 1.0f, 0.0f, alpha },
        { 0.0f, 1.0f, 1.0f, alpha },
        { 1.0f, 0.0f, 0.0f, alpha },
        { 1.0f, 0.0f, 1.0f, alpha },
        { 1.0f, 1.0f, 0.0f, alpha },

        { 0.0f, 0.5f, 1.0f, alpha },
        { 0.0f, 1.0f, 0.5f, alpha },
        { 0.5f, 1.0f, 1.0f, alpha },
        { 1.0f, 0.5f, 0.0f, alpha },
        { 1.0f, 0.5f, 1.0f, alpha },
        { 1.0f, 1.0f, 0.5f, alpha },

        { 0.5f, 0.0f, 1.0f, alpha },
        { 0.5f, 1.0f, 0.0f, alpha },
        { 1.0f, 0.0f, 0.5f, alpha },
        { 0.5f, 0.5f, 1.0f, alpha },
        { 0.5f, 1.0f, 0.5f, alpha },
        { 1.0f, 0.5f, 0.5f, alpha },
    };
    std::random_device rd;
    std::mt19937 rng(rd());
    for(int i=0; i<18; i++) {
        int l = clrs.size() - 1;
        std::uniform_int_distribution<int> uni(0,l);
        int k = uni(rng);
        for(int j=0; j<4; j++)
            colorSet[i].v[j] = clrs[k][j];
        if(k < l)
            clrs.erase(clrs.begin() + k); //, clrs.begin()+k);
        else
            clrs.pop_back();
    }

}

Shape4D::~Shape4D()
{
    delete[] vectors3D;
    delete[] vectors4D;
    delete[] axes4D;
    delete[] axes3D;

}

void Shape4D::toGLfaces(Shape4D::Vertex **v, int **ind, int *indexTotal)
{
    for(int i=0; i<geom.faceNum; i++) {
        for(int j=0; j<geom.edgesPerFace; j++) {
            int k = geom.faces[(geom.edgesPerFace*i)+j];
            struct Vertex *tmp = &GLfaceVerts[(geom.edgesPerFace*i)+j];
            tmp->pos.x = vectors3D[(3*k)+0];
            tmp->pos.y = vectors3D[(3*k)+1];
            tmp->pos.z = vectors3D[(3*k)+2];
        }

        for(int j=0; j<geom.edgesPerFace; j++) {
            int next = j == geom.edgesPerFace - 1 ? 0 : j+1;
            int prev = j == 0 ? geom.edgesPerFace - 1 : j-1;

            union Vector::Vec *v0, *a, *b, c, *n;
            v0 = &GLfaceVerts[(geom.edgesPerFace*i)+j].pos;
            a = &GLfaceVerts[(geom.edgesPerFace*i)+next].pos;
            b = &GLfaceVerts[(geom.edgesPerFace*i)+prev].pos;
            Vector::add(v0, a, b, &c);
            n = &GLfaceVerts[(geom.edgesPerFace*i)+j].norm;
            Vector::cross(v0, a, b, n);
            Vector::norm(n);
        }
    }

    *v = GLfaceVerts;
    *ind = GLfaceIndices;
    *indexTotal = GLfaceIndexNum;
}

void Shape4D::toGLedges(Shape4D::Vertex **v, int **ind, int *indexTotal)
{
    for(int i=0; i<geom.edgeNum; i++) {
        for(int j=0; j<2; j++) {
            int k = geom.edges[(2*i)+j];
            struct Vertex *tmp = &GLedgeVerts[(2*i)+j];
            tmp->pos.x = vectors3D[(3*k)+0];
            tmp->pos.y = vectors3D[(3*k)+1];
            tmp->pos.z = vectors3D[(3*k)+2];
        }
    }

    *v = GLedgeVerts;
    *ind = GLedgeIndices;
    *indexTotal = GLedgeIndexNum;

}

void Shape4D::toGLaxes(Shape4D::Vertex **v)
{
    for(int i=0; i<8; i++) {
        struct Vertex *tmp = &GLaxes[i];
        tmp->pos.x = axes3D[(3*i)+0];
        tmp->pos.y = axes3D[(3*i)+1];
        tmp->pos.z = axes3D[(3*i)+2];
    }
    *v = GLaxes;

}

float* Shape4D::get() {
    return vectors4D;
}

float* Shape4D::getAxes() {
    return axes4D;
}

int Shape4D::size()
{
    return geom.vertNum;
}

void Shape4D::collapse()
{
    // "collapse" 4D points into 3-space
    float offset = 1.7f;
    for(int i=0; i<geom.vertNum; i++) {
        vectors3D[(3*i) + 0] = vectors4D[(4*i) + 0] / (offset + vectors4D[(4*i) + 3]);
        vectors3D[(3*i) + 1] = vectors4D[(4*i) + 1] / (offset + vectors4D[(4*i) + 3]);
        vectors3D[(3*i) + 2] = vectors4D[(4*i) + 2] / (offset + vectors4D[(4*i) + 3]);
    }
    for(int i=0; i<8; i++) {
        axes3D[(3*i) + 0] = axes4D[(4*i) + 0] / (offset + axes4D[(4*i) + 3]);
        axes3D[(3*i) + 1] = axes4D[(4*i) + 1] / (offset + axes4D[(4*i) + 3]);
        axes3D[(3*i) + 2] = axes4D[(4*i) + 2] / (offset + axes4D[(4*i) + 3]);

    }
}

void Shape4D::test(int)
{

}

float Shape4D::longest()
{
    float ret = 0;
    for(int i=0; i<geom.vertNum; i++) {
        float a = geom.verts[(4*i) + 0];
        float b = geom.verts[(4*i) + 1];
        float c = geom.verts[(4*i) + 2];
        float d = geom.verts[(4*i) + 3];
        float s = sqrt(a*a + b*b + c*c + d*d);
        if(s > ret) ret = s;
    }
    return ret;

}

void Shape4D::setGeometry()
{
    using namespace std;

    GLfaceIndexNum = geom.faceNum * 3 * (geom.edgesPerFace - 2);
    GLedgeIndexNum = geom.edgeNum * 2;
    GLfaceVerts = new struct Vertex[geom.faceNum * geom.edgesPerFace];
    GLedgeVerts = new struct Vertex[geom.edgeNum * 2];
    GLaxes = new struct Vertex[8];
    GLfaceIndices = new int[GLfaceIndexNum];
    GLedgeIndices = new int[GLedgeIndexNum];
    vectors4D = new float[geom.vertNum * 4];
    vectors3D = new float[geom.vertNum * 3];

    for(int i=0; i<geom.vertNum*4; i++) {
        vectors4D[i] = geom.verts[i]/(longest());
    }
    for(int i=0; i<geom.edgeNum; i++) {
        vector<int> v;
        v.push_back(geom.edges[(2*i) + 0]);
        v.push_back(geom.edges[(2*i) + 1]);
    }
    for(int i=0; i<geom.faceNum; i++) {
        vector<int> v;
        for(int j=0; j<geom.edgesPerFace; j++) {
            v.push_back(geom.faces[(geom.edgesPerFace * i) + j]);

        }
    }

    for(int i=0; i<geom.edgeNum*2; i++)
        GLedgeIndices[i] = i;

    if(geom.edgesPerFace == 3)
        for(int i=0; i<geom.faceNum*3; i++)
            GLfaceIndices[i] = i;

    if(geom.edgesPerFace == 4) {
        for(int i=0; i<geom.faceNum; i++) {
            GLfaceIndices[i*6+0] = (4*i)+0;
            GLfaceIndices[i*6+1] = (4*i)+1;
            GLfaceIndices[i*6+2] = (4*i)+2;
            GLfaceIndices[i*6+3] = (4*i)+0;
            GLfaceIndices[i*6+4] = (4*i)+3;
            GLfaceIndices[i*6+5] = (4*i)+2;
        }
    }
    if(geom.edgesPerFace == 5) {
        for(int i=0; i<geom.faceNum; i++) {
            GLfaceIndices[i*9+0] = (5*i)+0;
            GLfaceIndices[i*9+1] = (5*i)+1;
            GLfaceIndices[i*9+2] = (5*i)+2;
            GLfaceIndices[i*9+3] = (5*i)+0;
            GLfaceIndices[i*9+4] = (5*i)+3;
            GLfaceIndices[i*9+5] = (5*i)+2;
            GLfaceIndices[i*9+6] = (5*i)+0;
            GLfaceIndices[i*9+7] = (5*i)+3;
            GLfaceIndices[i*9+8] = (5*i)+4;
        }
    }

    // color faces
    for(int i=0; i<geom.faceNum; i++) {
        for(int j=0; j<geom.edgesPerFace; j++) {
            struct Vertex *tmp = &GLfaceVerts[(geom.edgesPerFace*i)+j];
            for(int l=0; l<4; l++) {
                tmp->colors.v[l] = colorSet[i%18].v[l];
            }
        }
    }

    // color lines
    for(int i=0; i<geom.edgeNum; i++) {
        for(int j=0; j<2; j++) {
            struct Vertex *tmp = &GLedgeVerts[(2*i)+j];
            for(int l=0; l<4; l++) {
                tmp->colors.v[l] = colorSet[i%18].v[l];
            }
        }
    }
    // color axes
    float axescolors[16] =
    {
        1.0f, 0.0f, 0.0f, 1.0f,
        0.0f, 1.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 1.0f, 1.0f,
    };
    for(int i=0; i<4; i++) {
        for(int j=0; j<2; j++) {
            struct Vertex *tmp = &GLaxes[(2*i)+j];
            for(int l=0; l<4; l++) {
                tmp->colors.v[l] = axescolors[(4*i)+l];
            }
        }
    }

}

