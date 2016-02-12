#ifndef SHAPES_H
#define SHAPES_H
#include "shape4d.h"

class Simplex : public Shape4D
{
public:
    Simplex();
    ~Simplex();
    bool tsides[10];

private:
    float *cell5verts;
    int *cf;

};

class Tesseract : public Shape4D
{
public:
    Tesseract();
    ~Tesseract();
private:
    float *cell8verts;

};

class Cell16 : public Shape4D
{
public:
    Cell16();
    ~Cell16();
};

class Cell24 : public Shape4D
{
public:
    Cell24();
    ~Cell24();
};

class Cell120 : public Shape4D
{
public:
    Cell120();
    ~Cell120();

};

class Cell600 : public Shape4D
{
public:
    Cell600();
    ~Cell600();

};

class Cube : public Shape4D
{
public:
    Cube();
    ~Cube();
private:
    float *cubeverts;
    int *cubeedges;
    int *cubefaces;

};

class Dodec : public Shape4D
{
public:
    Dodec();
    ~Dodec();

};

class Square : public Shape4D
{
public:
    Square();
    ~Square();
private:
    float *squareverts;
    int *squareedges;
    int *squarefaces;
};

class Pentagon : public Shape4D
{
public:
    Pentagon();
    ~Pentagon();
private:
    float *pv;
    int *pe, *pf;
};


#endif // SHAPES_H
