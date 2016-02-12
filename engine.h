#ifndef ENGINE_H
#define ENGINE_H

#ifdef ANDROID_NDK
#include <jni.h>
#include <android/log.h>
#endif

#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#include "gui/gui.h"
#include "shapes/shapes.h"
#include "math/matrix.h"

class Engine
{
public:
    Engine();
    ~Engine();
    void init();
    void resize(int, int);
    int update();
    void processTouch(int, float, float);
    void rotate(float, float, float);
    void test(int b1);
    bool useGyro;
    bool state[20];
    bool oldState[20];
    enum Items{
        frame, sides, axis, refl, colors,
        cross, apart, single,
        lockx3d, locky3d, lockx4d, locky4d,
        gyro,
        cell_5, cell_8, cell_16, cell_24,
        cell_120, cell_600,
    };

private:
    void drawFrame(Shape4D *, Matrix);
    void drawAxes(Shape4D *, Matrix);
    void drawModel(Shape4D *, Matrix);
    void setupModel(int type, float w, float h);
    Shape4D *getShape();

    // Shapes
    Cell120 c120;
    Cell600 c600;
    Cell24 c24;
    Simplex c5;
    Cell16 c16;
    Tesseract c8;

    int optNum;
    float angle[6];
    Matrix shaderMatrixL, shaderMatrixR, matrixL, matrixR;
    GLfloat normMatL[9], normMatR[9];
    int width, height;
    GLuint modelprogram, frameprogram;
    float *bwcolors;
    Gui gui;
    int blenda, blendb, blendc;

};

#endif // ENGINE_H
