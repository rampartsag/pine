#include "shapes.h"
#include "geometry.h"
#include "../math/vector.h"
#include <bitset>
#include <iostream>
#include <cmath>

using namespace std;

int k;
Simplex::Simplex() : Shape4D()
{
    cell5verts = new float[20];
    cf = new int[30];
    float m[20] = {
        1.0f, 1.0f, 1.0f, -1.0f/(float)sqrt(5.0f),
        1.0f, -1.0f, -1.0f, -1.0f/(float)sqrt(5.0f),
        -1.0f, 1.0f, -1.0f, -1.0f/(float)sqrt(5.0f),
        -1.0f, -1.0f, 1.0f, -1.0f/(float)sqrt(5.0f),
        0.0f, 0.0f, 0.0f, 4.0f/(float)sqrt(5.0f),
    };
    for(int i=0; i<20; i++)
        cell5verts[i] = m[i];
    int ces[30] =
    {
            0, 1, 2,
            0, 1, 3,
            0, 1, 4,
            0, 2, 3,
            0, 2, 4,
            0, 3, 4,
            1, 2, 3,
            1, 2, 4,
            1, 3, 4,
            2, 3, 4,
    };
    for(int i=0; i<30; i++)
        cf[i] = ces[i];
    //geom.metric = 2.0f*sqrt(2.0f)* 2*M_PI;
    geom.metric = 0.50f;
    geom.vertNum = 5;
    geom.edgeNum = 10;
    geom.faceNum = 10;
    geom.edgesPerFace = 3;
    geom.verts = cell5verts;
    geom.edges = cell5edges;
    geom.faces = cf;
    setGeometry();

}

Simplex::~Simplex()
{
    delete[] cell5verts;

}

Tesseract::Tesseract() : Shape4D()
{

    cell8verts = new float[64];
    std::bitset<4> b;
    for(int i=0; i<16; i++) {
        b = i;
        for(int j=0; j<4; j++)
            cell8verts[(4*i) + j] = b[j] ? -1.0f : 1.0f;

    }
    geom.metric = 0.50f;
    geom.vertNum = 16;
    geom.edgeNum = 32;
    geom.faceNum = 24;
    geom.edgesPerFace = 4;
    geom.verts = cell8verts;
    geom.edges = cell8edges;
    geom.faces = cell8faces;
    setGeometry();

}

Tesseract::~Tesseract()
{
    delete[] cell8verts;

}


Cell16::Cell16()
{
    geom.metric = 1.333333f;
    geom.vertNum = 8;
    geom.edgeNum = 24;
    geom.faceNum = 32;
    geom.edgesPerFace = 3;
    geom.verts = cell16verts;
    geom.edges = cell16edges;
    geom.faces = cell16faces;
    setGeometry();


}

Cell16::~Cell16()
{

}


Cell24::Cell24()
{
    geom.metric = 0.666667f;
    geom.vertNum = 24;
    geom.edgeNum = 96;
    geom.faceNum = 96;
    geom.edgesPerFace = 3;
    geom.verts = cell24verts;
    geom.edges = cell24edges;
    geom.faces = cell24faces;
    setGeometry();


}

Cell24::~Cell24()
{

}

Cell120::Cell120() : Shape4D()
{
    //geom.metric = (3.0f - sqrt(5.0f));
    geom.metric = 0.333333f;
    geom.vertNum = 600;
    geom.edgeNum = 1200;
    geom.faceNum = 720;
    geom.edgesPerFace = 5;
    geom.verts = cell120verts;
    geom.edges = cell120edges;
    geom.faces = cell120faces;
    setGeometry();


}

Cell120::~Cell120()
{

}

Cell600::Cell600()
{
    geom.metric = 0.50f;
    geom.vertNum = 120;
    geom.edgeNum = 720;
    geom.faceNum = 1200;
    geom.edgesPerFace = 3;
    geom.verts = cell600verts;
    geom.edges = cell600edges;
    geom.faces = cell600faces;
    setGeometry();

}

Cell600::~Cell600()
{

}


Cube::Cube()
{
    cubeverts = new float[32];
    std::bitset<4> b;
    for(int i=0; i<8; i++) {
        b = i;
        cubeverts[(4*i) + 0] = b[0] ? -1.0f : 1.0f;
        cubeverts[(4*i) + 1] = b[1] ? -1.0f : 1.0f;
        cubeverts[(4*i) + 2] = b[2] ? -1.0f : 1.0f;
        cubeverts[(4*i) + 3] = 0.0f;

    }
    cubeedges = new int[24];
    cubefaces = new int[24];
    int k[24] =
    {
        0,  1,
        2,  3,
        4,  5,
        6,  7,
        0,  2,
        4,  6,
        1,  3,
        5,  7,
        0,  4,
        1,  5,
        2,  6,
        3,  7,
    };
    for(int i=0; i<24; i++)
        cubeedges[i] = k[i];
    int j[24] =
    {
        0,  1,  3,  2,
        4,  5,  7,  6,
        0,  1,  5,  4,
        2,  3,  7,  6,
        0,  2,  6,  4,
        1,  3,  7,  5,
    };
    for(int i=0; i<24; i++)
        cubefaces[i] = j[i];

    geom.vertNum = 8;
    geom.edgeNum = 12;
    geom.faceNum = 6;
    geom.edgesPerFace = 4;
    geom.verts = cubeverts;
    geom.edges = cubeedges;
    geom.faces = cubefaces;
    setGeometry();
}

Cube::~Cube()
{
    delete[] cubeverts;
    delete[] cubeedges;
    delete[] cubefaces;

}


Dodec::Dodec()
{
    geom.vertNum = 20;
    geom.edgeNum = 30;
    geom.faceNum = 12;
    geom.edgesPerFace = 5;
    geom.verts = dodecverts;
    geom.edges = dodecedges;
    geom.faces = dodecfaces;
    setGeometry();
}

Dodec::~Dodec()
{

}


Square::Square()
{
    squareverts = new float[16];
    float q[16] = {
        -1.0f, -1.0f, 0, 0,
        -1.0f, 1.0f, 0, 0,
        1.0f, -1.0f, 0, 0,
        1.0f, 1.0f, 0, 0,
    };
    for(int i=0; i<16; i++)
        squareverts[i] = q[i];
    squareedges = new int[8];

    for(int i=0; i<8; i++)
        squareedges[i] = q[i];
    squarefaces = new int[4];
    squarefaces[0] = 2;
    squarefaces[1] = 3;
    squarefaces[2] = 1;
    squarefaces[3] = 0;

    geom.vertNum = 4;
    geom.edgeNum = 4;
    geom.faceNum = 1;
    geom.edgesPerFace = 4;
    geom.verts = squareverts;
    geom.edges = squareedges;
    geom.faces = squarefaces;
    setGeometry();
}

Square::~Square()
{
    delete[] squareverts;
    delete[] squareedges;
    delete[] squarefaces;

}


Pentagon::Pentagon()
{
    float s1 = (sqrt(10+(2*sqrt(5))))/4.0f;
    float s2 = (sqrt(10-(2*sqrt(5))))/4.0f;
    float c1 = (sqrt(5)-1)/4.0f;
    float c2 = (sqrt(5)+1)/4.0f;
    pv = new float[20];
    float q[20] = {
        -s1, c1, 0, 0,
        -s2, -c2, 0, 0,
        s2, -c2, 0, 0,
        s1, c1, 0, 0,
        0, 1.0f, 0, 0,
    };
    for(int i=0; i<20; i++)
        pv[i] = q[i];
    pe = new int[10];
    int y[10] = {
        0, 1,
        1, 2,
        2, 3,
        3, 4,
        4, 0,
    };
    for(int i=0; i<10; i++)
        pe[i] = y[i];

    pf = new int[5];
    pf[0] = 0;
    pf[1] = 1;
    pf[2] = 2;
    pf[3] = 3;
    pf[4] = 4;

    geom.vertNum = 5;
    geom.edgeNum = 5;
    geom.faceNum = 1;
    geom.edgesPerFace = 5;
    geom.verts = pv;
    geom.edges = pe;
    geom.faces = pf;
    setGeometry();
}

Pentagon::~Pentagon()
{
    delete[] pv;
    delete[] pe;
    delete[] pf;

}

