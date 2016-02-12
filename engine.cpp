#include "engine.h"
#include "shaders.h"
#include "gl_common.h"
#include <zlib.h>
#include <cstdio>
#include <iostream>
#include <cmath>
#include <vector>
#include <set>
#include <string>


Engine::Engine()
{
    useGyro = false;
    optNum = 20;
    for(int i=0; i<optNum; i++) {
        state[i] = false;
        oldState[i] = false;
    }
    for(int i=0; i<6; i++)
        angle[i] = 0;


}

Engine::~Engine()
{

}

void Engine::setupModel(int type, float w, float h)
{
    shaderMatrixL.reset();
    shaderMatrixR.reset();

    float y = type == single ? 0.0f : -0.5f;
    float u = type == cross ? 3.4f : -3.4f;
    float offset = 1.70f;
    shaderMatrixL.translate3d(y, 0, 0);
    // need normal matrix here
    shaderMatrixL.convert3x3(normMatL);
    Matrix::invert3x3(normMatL, normMatL);

    shaderMatrixL.perspective3D(60, w/h, 0.1, 100);
    shaderMatrixL.translate3d(0, 0, -offset);

    shaderMatrixR.translate3d(0.5f, 0, 0);
    // need normal matrix here
    shaderMatrixR.convert3x3(normMatR);
    Matrix::invert3x3(normMatR, normMatR);

    shaderMatrixR.perspective3D(60, w/h, 0.1, 100);
    shaderMatrixR.translate3d(0, 0, -offset);

    shaderMatrixL.rotate4D(u, 0, Matrix::ZX);
    //shaderMatrixL.rotate4D(45, Matrix::XY);
    //shaderMatrixL.rotate4D(45, Matrix::YZ);

    shaderMatrixL.transpose();
    shaderMatrixR.transpose();

}

void Engine::init()
{
    Shaders s;
    modelprogram = GL_common::createProgram((const char *)s.refl_vsh.c_str(), (const char *)s.refl_fsh.c_str());
    if(!modelprogram)
        LOGE("Could not create program!");
    frameprogram = GL_common::createProgram((const char *)s.frame_vsh.c_str(), (const char *)s.frame_fsh.c_str());
    if(!frameprogram)
        LOGE("Could not create program!");

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    for(int i=0; i<optNum; i++)
        state[i] = false;

    state[colors] = true;
    state[refl] = true;
    state[sides] = true;
    state[single] = true;
    state[cell_8] = true;
    state[frame] = false;
    blenda = GL_SRC_ALPHA;
    blendb = GL_ONE_MINUS_SRC_ALPHA;
    blendc = GL_FUNC_ADD;

    Matrix m;
    Shape4D *sh = getShape();
    //m.rotate4D(3.2, 0, Matrix::ZX);
    //m.rotate4D(3.2, 0, Matrix::YZ);
    Matrix::vmult(m.get(), sh->get(), sh->size());
    Matrix::vmult(m.get(), sh->getAxes(), 8);
}

void Engine::resize(int w, int h)
{
    if(width != w || height != h) {
        width = w; height = h;
        gui.setSize(width, height);
        glViewport (0, 0, w, h);

        int s = single;
        for(int i=cross; i<=single; i++)
            if(state[i]) s = i;

        setupModel(s, (float)w, (float)h);

    }

}

Shape4D* Engine::getShape() {
    Shape4D *s[6] = {
        &c5, &c8, &c16, &c24, &c120, &c600
    };
    for(int i=cell_5; i<=cell_600; i++)
        if(state[i])
            return s[i-cell_5];

    return &c8;

}

int Engine::update()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    //glClearColor(0.1, 0.1, 0.1, 0);
    Matrix mrot;

    if(!useGyro)
        state[gyro] = false;

    for(int i=cross; i<=single; i++) {
        if(state[i] && !oldState[i]) {
            setupModel(i, (float)width, (float)height);
        }
    }
    for(int i=0; i<optNum; i++)
        oldState[i] = state[i];

    for(int i=lockx3d; i<=locky4d; i++) {
        if(state[i]) {
            switch(i) {
            case lockx3d:
                angle[2] = 0;
                break;
            case locky3d:
                angle[1] = 0;
                break;
            case lockx4d:
                angle[3] = 0;
                break;
            case locky4d:
                angle[4] = 0;
                break;
            }
        }
    }

    for(int i=0; i<6; i++) {
        mrot.rotate4D(-angle[i], 0, (Matrix::plane)i);
        // the closer this is to one, the longer the object will rotate
        // after being released.
        angle[i] *= 0.974f;
    }

    Shape4D *s = getShape();
    Matrix::vmult(mrot.get(), s->get(), s->size());
    Matrix::vmult(mrot.get(), s->getAxes(), 8);
    s->collapse();

    if(!state[single]) {
        if(state[frame]) drawFrame(s, shaderMatrixR);
        if(state[axis]) drawAxes(s, shaderMatrixR);
        if(state[refl] || state[sides])
            drawModel(s, shaderMatrixR);
    }
    if(state[frame]) drawFrame(s, shaderMatrixL);
    if(state[axis]) drawAxes(s, shaderMatrixL);
    if(state[refl] || state[sides])
        drawModel(s, shaderMatrixL);

    gui.draw(state);
    if(state[19]) {
        return 1;
    }
    else return 0;
}

void Engine::drawFrame(Shape4D *shape, Matrix shaderMatrix) {
    glUseProgram(0);
    glUseProgram(frameprogram);
    glLineWidth(2);
    glEnable(GL_DEPTH_TEST);
    GLuint mvpmat = glGetUniformLocation(frameprogram, "u_mvpMatrix");
    GLuint vpos = glGetAttribLocation(frameprogram, "a_position");
    GLuint vcol = glGetAttribLocation(frameprogram, "a_color");

    struct Shape4D::Vertex *vertices;
    int *indices;
    int indNum;
    shape->toGLedges(&vertices, &indices, &indNum);
    int stride = sizeof(struct Shape4D::Vertex);

    GLuint bw = glGetUniformLocation(frameprogram, "BW");
    int bwbool = state[colors];
    glUniform1i(bw, bwbool);

    glUniformMatrix4fv(mvpmat, 1, GL_FALSE, shaderMatrix.get());
    glVertexAttribPointer(vpos, 3, GL_FLOAT, GL_FALSE, stride, &vertices->pos);
    glVertexAttribPointer(vcol, 4, GL_FLOAT, GL_FALSE, stride, &vertices->colors);

    glEnableVertexAttribArray(vpos);
    glEnableVertexAttribArray(vcol);
    glDrawElements(GL_LINES, indNum, GL_UNSIGNED_INT, indices);
    glDisableVertexAttribArray(vpos);
    glDisableVertexAttribArray(vcol);
    glDisable(GL_DEPTH_TEST);
}

void Engine::drawAxes(Shape4D *shape, Matrix shaderMatrix)
{
    glUseProgram(0);
    glUseProgram(frameprogram);
    glLineWidth(4);
    glEnable(GL_DEPTH_TEST);
    GLuint mvpmat = glGetUniformLocation(frameprogram, "u_mvpMatrix");
    GLuint vpos = glGetAttribLocation(frameprogram, "a_position");
    GLuint vcol = glGetAttribLocation(frameprogram, "a_color");

    struct Shape4D::Vertex *vertices;
    int stride = sizeof(struct Shape4D::Vertex);
    shape->toGLaxes(&vertices);
    int indices[8] =
    {
            0,1,
            2,3,
            4,5,
            6,7,
    };

    GLuint bw = glGetUniformLocation(frameprogram, "BW");
    int bwbool = true;
    glUniform1i(bw, bwbool);

    glUniformMatrix4fv(mvpmat, 1, GL_FALSE, shaderMatrix.get());
    glVertexAttribPointer(vpos, 3, GL_FLOAT, GL_FALSE, stride, &vertices->pos);
    glVertexAttribPointer(vcol, 4, GL_FLOAT, GL_FALSE, stride, &vertices->colors);

    glEnableVertexAttribArray(vpos);
    glEnableVertexAttribArray(vcol);
    glDrawElements(GL_LINES, 8, GL_UNSIGNED_INT, indices);
    glDisableVertexAttribArray(vpos);
    glDisableVertexAttribArray(vcol);
    glDisable(GL_DEPTH_TEST);
}

void Engine::drawModel(Shape4D *shape, Matrix shaderMatrix)
{
    glUseProgram(0);
    glUseProgram(modelprogram);
    glDisable(GL_DEPTH_TEST);
    glBlendFunc(blenda, blendb);
    glBlendEquation(blendc);

    GLuint vpos = glGetAttribLocation(modelprogram, "VertexPosition");
    GLuint vcol = glGetAttribLocation(modelprogram, "VertexColor");
    GLuint vnorm = glGetAttribLocation(modelprogram, "VertexNormal");
    GLuint mvpmat = glGetUniformLocation(modelprogram, "MVPMatrix");
    GLuint nmat = glGetUniformLocation(modelprogram, "NormalMatrix");

    // fragment uniforms
    GLuint ambientloc = glGetUniformLocation(modelprogram, "Ambient");
    GLuint lcolorloc = glGetUniformLocation(modelprogram, "LightColor");
    GLuint ldirloc = glGetUniformLocation(modelprogram, "LightDirection");
    GLuint hvecloc = glGetUniformLocation(modelprogram, "HalfVector");
    GLuint shineloc = glGetUniformLocation(modelprogram, "Shininess");
    GLuint strongloc = glGetUniformLocation(modelprogram, "Strength");
    GLuint bw = glGetUniformLocation(modelprogram, "BW");

    GLfloat litevec[3] = { -1.0f/sqrt(3.0f), 1.0f/sqrt(3.0f), 1.0f/sqrt(3.0f) };
    GLfloat halfvec[3]; // = { 1/sqrt(6.0f), 1/sqrt(6.0f), 2/sqrt(6.0f) };
    GLfloat hv = sqrt(litevec[0]*litevec[0] + litevec[1]*litevec[1] +
                    (litevec[2]+1.0f) * (litevec[2]+1.0f));
    halfvec[0] = litevec[0]/hv;
    halfvec[1] = litevec[1]/hv;
    halfvec[2] = (litevec[2]+1.0f)/hv;
    GLfloat ambrgb[4] = {0.0f, 0.0f, 0.0f, 0.0f };
    GLfloat lcolorrgb[4] = {0.00f, 0.00f, 0.00f, 0.00f};
    GLfloat shine = 10.0f;
    GLfloat strong = 18.0f;

    if(state[refl]) {
        lcolorrgb[0] = 0.07f;
        lcolorrgb[1] = 0.07f;
        lcolorrgb[2] = 0.07f;
        lcolorrgb[3] = 0.07f;
    }

    if(state[sides]) {
        ambrgb[0] = 1.0f;
        ambrgb[1] = 1.0f;
        ambrgb[2] = 1.0f;
        ambrgb[3] = 1.0f;
    }

    int bwbool = state[colors];

    struct Shape4D::Vertex *vertices;
    int *indices;
    int indNum;
    shape->toGLfaces(&vertices, &indices, &indNum);
    int stride = sizeof(struct Shape4D::Vertex);

    GLuint *vbov, *vboi;
    vbov = &shape->vbov;
    vboi = &shape->vboi;
    if(*vbov == 0 && *vboi == 0) {
        glGenBuffers(1, vbov);
        glGenBuffers(1, vboi);
        glBindBuffer(GL_ARRAY_BUFFER, *vbov);
        glBufferData(GL_ARRAY_BUFFER, stride*shape->size(), vertices, GL_DYNAMIC_DRAW);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, *vboi);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint)*indNum, indices, GL_DYNAMIC_DRAW);
    }

    glUniformMatrix4fv(mvpmat, 1, GL_FALSE, shaderMatrix.get());
    glUniformMatrix3fv(nmat, 1, GL_FALSE, normMatL);
    glVertexAttribPointer(vpos, 3, GL_FLOAT, GL_FALSE, stride, &vertices->pos);
    glVertexAttribPointer(vcol, 4, GL_FLOAT, GL_FALSE, stride, &vertices->colors);
    glVertexAttribPointer(vnorm, 3, GL_FLOAT, GL_FALSE, stride, &vertices->norm);
    glUniform4fv(ambientloc, 1, ambrgb);
    glUniform4fv(lcolorloc, 1, lcolorrgb);
    glUniform3fv(ldirloc, 1, litevec);
    glUniform3fv(hvecloc, 1, halfvec);
    glUniform1fv(shineloc, 1, &shine);
    glUniform1fv(strongloc, 1, &strong);
    glUniform1i(bw, bwbool);

    glBindBuffer(GL_ARRAY_BUFFER, *vbov);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, *vboi);
    glEnableVertexAttribArray(vpos);
    glEnableVertexAttribArray(vcol);
    glEnableVertexAttribArray(vnorm);
    glDrawElements(GL_TRIANGLES, indNum, GL_UNSIGNED_INT, 0);
    glDisableVertexAttribArray(vpos);
    glDisableVertexAttribArray(vcol);
    glDisableVertexAttribArray(vnorm);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    glEnable(GL_DEPTH_TEST);
}


float xprev, yprev;
float xprev2, yprev2;
bool rot3;
float xdown, ydown;
int count = 0;
int num = -1;
void Engine::processTouch(int mtype, float x, float y) {
    float x1 = x;
    float y1 = y;
    x /= width;
    y /= height;
    count++;
    if(mtype == 1) {
        xdown = x1; ydown = y1;
        num = gui.checkRegion(x1, y1);
        rot3 = x1 < width/2;
        xprev = x;
        yprev = y;
        xprev2 = x;
        yprev2 = y;
        count = 0;
    }
    if(mtype == 2 && num < 0) {

        float dx = x - xprev2;
        float dy = y - yprev2;
        if(rot3) {
            angle[Matrix::ZX] = 50*dx;
            angle[Matrix::YZ] = 50*dy;
        }
        else {
            angle[Matrix::XW] = 50*dx;
            angle[Matrix::YW] = 50*dy;

        }
        xprev2 = xprev;
        yprev2 = yprev;
        xprev = x;
        yprev = y;
    }
    if(mtype == 4) {
        if(num < 0) {
            if(count < 3) {
                if(rot3)
                    angle[Matrix::ZW] += 1.0f;
                else
                    angle[Matrix::ZW] -= 1.0f;
            }
            count = 0;
        }
        num = -1;
    }

}

void Engine::rotate(float x, float y, float z)
{

    x = x * std::abs(x);
    y = y * std::abs(y);
    z = z * std::abs(z);

    if(state[gyro]) {
        angle[1] -= y;
        angle[2] += x;
        angle[0] += z;
    }

}

int t1 = 4;
int t2 = 5;
int t3 = 0;

void Engine::test(int b1)
{
    using namespace std;

    int a[15] =
    {
            GL_ZERO,
            GL_ONE,
            GL_SRC_COLOR,
            GL_ONE_MINUS_SRC_COLOR,
            GL_SRC_ALPHA,
            GL_ONE_MINUS_SRC_ALPHA,
            GL_DST_COLOR,
            GL_ONE_MINUS_DST_COLOR,
            GL_DST_ALPHA,
            GL_ONE_MINUS_DST_ALPHA,
            GL_CONSTANT_COLOR,
            GL_ONE_MINUS_CONSTANT_COLOR,
            GL_CONSTANT_ALPHA,
            GL_ONE_MINUS_CONSTANT_ALPHA,
            GL_SRC_ALPHA_SATURATE,
    };
    int c[3] =
    {
            GL_FUNC_ADD,
            GL_FUNC_SUBTRACT,
            GL_FUNC_REVERSE_SUBTRACT,
    };
    string s1[15] =
    {
            "GL_ZERO",
            "GL_ONE",
            "GL_SRC_COLOR",
            "GL_ONE_MINUS_SRC_COLOR",
            "GL_SRC_ALPHA",
            "GL_ONE_MINUS_SRC_ALPHA",
            "GL_DST_COLOR",
            "GL_ONE_MINUS_DST_COLOR",
            "GL_DST_ALPHA",
            "GL_ONE_MINUS_DST_ALPHA",
            "GL_CONSTANT_COLOR",
            "GL_ONE_MINUS_CONSTANT_COLOR",
            "GL_CONSTANT_ALPHA",
            "GL_ONE_MINUS_CONSTANT_ALPHA",
            "GL_SRC_ALPHA_SATURATE",
    };
    string s2[3] =
    {
            "GL_FUNC_ADD",
            "GL_FUNC_SUBTRACT",
            "GL_FUNC_REVERSE_SUBTRACT",
    };

    Matrix m;
    Shape4D *s = getShape();
    switch (b1) {
    case 1:
        t1++;
        if(t1 > 14) t1 = 0;
        blenda = a[t1];
        break;
    case 2:
        t2++;
        if(t2 > 14) t2 = 0;
        blendb = a[t2];;
        break;
    case 4:
        t3++;
        if(t3 > 2) t3 = 0;
        blendc = c[t3];
        break;
    case 8:
        //glFrontFace(GL_CW);
        glEnable(GL_CULL_FACE);
        break;
    case 16:
        //glFrontFace(GL_CCW);
        glDisable(GL_CULL_FACE);
        break;
    case 32:
        glFrontFace(GL_CCW);
        break;
    case 64:
        glFrontFace(GL_CW);
        break;
    case 128:
        m.rotate4D(-45, 0, Matrix::YZ);
        m.rotate4D(-45, 0, Matrix::ZX);
        Matrix::vmult(m.get(), s->get(), s->size());
        Matrix::vmult(m.get(), s->getAxes(), 8);
        break;

    default:
        break;
    }

    cout << s1[t1] << endl << s1[t2] << endl << s2[t3] <<endl << endl;

}
