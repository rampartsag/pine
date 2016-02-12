#ifndef GUI_H
#define GUI_H

#include "menu.h"
#include <GLES2/gl2.h>
#include <string>

class Gui
{
public:
    Gui();
    ~Gui();
    Menu *options, *menu, *help, *button, *shapes;
    Menu *menus[5];
    Menu *current;
    void draw(bool *);
    //void draw();
    //GLuint createTexture(int w, int h, unsigned char *img);
    void setSize(int w, int h);
    int checkRegion(float x, float y);
    void test();

private:
    enum Actions{toggle, swtch, close, optGo, shapeGo,
                 helpGo, menuGo, exit};
    enum Windows{optWin, shapeWin, menuWin, menuButton, helpWin};
    GLuint menuprogram;
    unsigned char* unzip(unsigned char *zim, int zsize, int size);
    unsigned char *getFile(const char *);
    unsigned char *oIm, *xIm, *optIm, *shapesIm;
    unsigned char *menuIm, *helpIm, *buttonIm;
    int screenw, screenh;
    int menuNum;
    bool *state;
    std::string sImage;
    struct TextInfo {
        unsigned char *im;
        int w, h;
        float ls, rs, top, bottom;
    } ti;
    void drawTexture(struct TextInfo *);
    void executeAction(int, int);

};

const float optRegions[28] = {
    //model
    32.0f, 72.0f,
    32.0f, 128.0f,
    32.0f, 184.0f,
    232.0f, 72.0f,
    232.0f, 128.0f,
    //view
    32.0f, 296.0f,
    32.0f, 352.0f,
    32.0f, 408.0f,
    //touch
    512.0f, 72.0f,
    512.0f, 128.0f,
    512.0f, 184.0f,
    512.0f, 240.0f,
    //motion
    392.0f, 352.0f,
    //close
    736.0f, 16.0f,

};

const float menuRegions[8] = {
    0.0f, 0.0f,
    0.0f, 72.0f,
    0.0f, 136.0f,
    0.0f, 200.0f,
};

const float shapeRegions[14] = {
    33.0f, 72.0f,
    33.0f, 128.0f,
    33.0f, 184.0f,
    33.0f, 240.0f,
    33.0f, 296.0f,
    33.0f, 352.0f,
    320.0f, 16.0f,

};

const float helpRegions[2] =
{
    736.0f, 16.0f,

};

#endif // GUI_H
