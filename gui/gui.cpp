#include "gui.h"
#include "../gl_common.h"
#include "../shaders.h"
#include <zlib.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include "image.h"


Gui::Gui()
{
    menuNum = 5;
    Shaders s;
    // cannot create program here because opengl context has not been
    // established yet on android. Create in setSize().
    //menuprogram = GL_common::createProgram((const char *)s.menu_vsh.c_str(), (const char *)s.menu_fsh.c_str());

    oIm = unzip(dot_im.data, dot_im.z_len, dot_im.len);
    xIm = unzip(eks_im.data, eks_im.z_len, 9216);

    optIm = unzip(options_im.data, options_im.z_len, options_im.len);
    options = new Menu(optIm, options_im.width, options_im.height, 14);
    options->setSwtchGrps(1);
    for(int i=0; i<14; i++) {
        float ww = (float)options->width;
        float hh = (float)options->height;
        struct Menu::Region *reg = &options->region[i];
        reg->x = optRegions[2*i]/ww;
        reg->y = optRegions[(2*i)+1]/hh;
        reg->w = eks_im.width/ww;
        reg->h = eks_im.height/hh;
        if(i >= 5 && i <= 7) {
            reg->im = oIm;
            reg->action = swtch;
        } else {
            reg->im = xIm;
            reg->action = toggle;
        }
    }
    options->region[13].action = close;
    options->region[13].im = nullptr;
    int itmp[3] = { 5, 6, 7};
    options->setSwtchIds(0, itmp, 3);


    shapesIm = unzip(shapes_im.data, shapes_im.z_len, shapes_im.len);
    shapes = new Menu(shapesIm, shapes_im.width, shapes_im.height, 7);
    // line below is bad design
    shapes->sStart = 13;
    shapes->setSwtchGrps(1);
    for(int i=0; i<7; i++) {
        float ww = (float)shapes->width;
        float hh = (float)shapes->height;
        struct Menu::Region *reg = &shapes->region[i];
        reg->x = shapeRegions[2*i]/ww;
        reg->y = shapeRegions[(2*i)+1]/hh;
        reg->w = eks_im.width/ww;
        reg->h = eks_im.height/hh;
        reg->im = oIm;
        reg->action = swtch;
    }
    shapes->region[6].action = close;
    shapes->region[6].im = nullptr;
    int stmp[6] = { 13, 14, 15, 16, 17, 18 };
    shapes->setSwtchIds(0, stmp, 6);


    menuIm = unzip(menu_im.data, menu_im.z_len, menu_im.len);
    menu = new Menu(menuIm, menu_im.width, menu_im.height, 4);
    for(int i=0; i<4; i++) {
        struct Menu::Region *reg = &menu->region[i];
        float ww = (float)menu->width;
        float hh = (float)menu->height;
        reg->x = menuRegions[2*i]/ww;
        reg->y = menuRegions[(2*i)+1]/hh;
        reg->w = menu_im.width/ww;
        reg->h = 64.0f/hh;
        reg->im = nullptr;

    }
    menu->region[0].h = 72.0f/(float)menu->height;
    menu->region[0].action = optGo;
    menu->region[1].action = shapeGo;
    menu->region[2].action = helpGo;
    menu->region[3].action = exit;


    buttonIm = unzip(menubutton_im.data, menubutton_im.z_len, menubutton_im.len);
    button = new Menu(buttonIm, menubutton_im.width, menubutton_im.height, 1);
    struct Menu::Region *reg = &button->region[0];
    float ww = (float)button->width;
    float hh = (float)button->height;
    reg->x = 0;
    reg->y = 0;
    reg->w = menubutton_im.width/ww;
    reg->h = menubutton_im.height/hh;
    reg->im = nullptr;
    reg->action = menuGo;

    helpIm = unzip(help_im.data, help_im.z_len, help_im.len);
    help = new Menu(helpIm, help_im.width, help_im.height, 1);
    struct Menu::Region *reg1 = &help->region[0];
    ww = (float)help->width;
    hh = (float)help->height;
    reg1->x = helpRegions[0]/ww;
    reg1->y = helpRegions[1]/hh;
    reg1->w = eks_im.width/ww;
    reg1->h = eks_im.height/hh;
    reg1->im = nullptr;
    reg1->action = close;


    menus[optWin] = options;
    menus[shapeWin] = shapes;
    menus[menuWin] = menu;
    menus[menuButton] = button;
    menus[helpWin] = help;
    current = button;
}

Gui::~Gui()
{
    delete[] oIm;
    delete[] xIm;
    delete[] optIm;
    delete[] shapesIm;
    // delete everything else here eventually

}

int fg = 0;
void Gui::drawTexture(Gui::TextInfo *t)
{

    glUseProgram(menuprogram);
    float ls, rs, top, bottom;
    // convert coords to "texture-space"
    ls = (2*t->ls) - 1;
    rs = (2*t->rs) - 1;
    top = 1 - (2*t->top);
    bottom = 1 - (2*t->bottom);
    GLfloat quadverts[] = { ls, top,
                            ls, bottom,
                            rs, bottom,
                            rs, top };
    GLfloat textverts[] = { 0.0f, 0.0f, 0.0f, 1.0f,
                            1.0f, 1.0f, 1.0f, 0.0f,
                          };
    GLuint ind[] = { 0, 1, 2, 0, 2, 3 };
    GLuint pos = glGetAttribLocation(menuprogram, "a_position");
    GLuint tex = glGetAttribLocation(menuprogram, "a_texCoord");

    glDisable(GL_CULL_FACE);
    glDisable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glBlendEquation(GL_FUNC_ADD);

    glVertexAttribPointer(pos, 2, GL_FLOAT, GL_FALSE, 0, quadverts);
    glVertexAttribPointer(tex, 2, GL_FLOAT, GL_FALSE, 0, textverts);
    glEnableVertexAttribArray(pos);
    glEnableVertexAttribArray(tex);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, GL_common::createTexture(t->w, t->h, t->im));
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, ind);
    glUseProgram(0);

}

void Gui::executeAction(int act, int r)
{
    switch(act) {
    case toggle:
        state[r] = !state[r];
        break;
    case swtch:
        if(state[r]) break;
        for(int i=0; i<current->swtchGrps; i++) {
            for(int j=0; j<current->swtchSizes[i]; j++) {
                if(current->swtchIds[i][j] == r && !state[r]) {
                    for(int k=0; k<current->swtchSizes[i]; k++) {
                        state[current->swtchIds[i][k]] = false;
                    }
                    state[r] = true;
                }
            }
        }
        break;
    case close:
        current = button;
        break;
    case optGo:
        current = options;
        break;
    case shapeGo:
        current = shapes;
        break;
    case menuGo:
        current = menu;
        break;
    case helpGo:
        current = help;
        break;
    case exit:
        state[19] = true;
        break;
    }

}

void Gui::test() {
    unsigned char *r;
    r = (unsigned char *)state;

    ti.im = r;
    ti.w = 128;
    ti.h = 32;
    ti.ls = 0;
    ti.top = 0;
    ti.rs = 0.5f;
    ti.bottom = 0.3f;
    drawTexture(&ti);
}

void Gui::draw(bool *state)
{
    this->state = state;
    ti.im = current->getImage();
    ti.w = current->width;
    ti.h = current->height;
    ti.ls = current->x;
    ti.top = current->y;
    ti.rs = ti.ls + current->ssw;
    ti.bottom = ti.top + current->ssh;
    drawTexture(&ti);

    for(int i=0; i<current->rNum; i++) {
        int e = i + current->sStart;
        if(state[e] && current->region[i].im != nullptr) {
            ti.im = current->region[i].im;
            ti.w = current->region[i].w * current->width;
            ti.h = current->region[i].h * current->height;
            ti.ls = (current->region[i].x * current->ssw) + current->x;
            ti.top = (current->region[i].y * current->ssh) + current->y;
            ti.rs = current->x + ((current->region[i].x + current->region[i].w)
                                  * current->ssw);
            ti.bottom = current->y + ((current->region[i].y + current->region[i].h)
                                      * current->ssh);
            drawTexture(&ti);
        }

    }

}

unsigned char *Gui::unzip(unsigned char *zim, int zsize, int size)
{
    unsigned char *unpacked = new unsigned char[size];
    z_stream strm;
    strm.zalloc = Z_NULL;
    strm.zfree = Z_NULL;
    strm.opaque = Z_NULL;
    strm.avail_in = zsize;
    strm.next_in = zim;
    strm.avail_out = size;
    strm.next_out = unpacked;
    inflateInit(&strm);
    inflate(&strm, -15);
    return unpacked;


}

unsigned char *Gui::getFile(const char *rgba)
{
    std::fstream imageFile(rgba, std::ios::in);
    //std::string sImage;

    if (imageFile.is_open())
    {
        std::stringstream buffer;
        buffer << imageFile.rdbuf();
        sImage = buffer.str();
    }
    return (unsigned char *)sImage.c_str();

}

void Gui::setSize(int w, int h) {
    Shaders s;
    menuprogram = GL_common::createProgram((const char *)s.menu_vsh.c_str(), (const char *)s.menu_fsh.c_str());
    screenw = w;
    screenh = h;
    // set a standard size here so all windows' text is the same size
    GLfloat pad = 0.085f;
    float sRatio = (float)w/(float)h;
    // find largest window
    int length = 0;
    int biggest = 0;
    for(int i=0; i<menuNum; i++) {
        int l = sRatio < 1.0f ? menus[i]->width : menus[i]->height;
        //int l = menus[i]->height;
        if(l > length) {
            biggest = i;
            length = l;
        }
    }
    //biggest = 0;

    float mRatio = (float)menus[biggest]->width/
            (float)menus[biggest]->height;
    // if screen ratio is less than window ratio, window is
    // wider (proportionally) than the screen
    if(sRatio < mRatio) {
        float a = 1 - (2*pad);
        float b = a/mRatio;
        menus[biggest]->ssw = a;
        menus[biggest]->ssh = b*sRatio;
        menus[biggest]->x = pad;
        menus[biggest]->y = (1 - (b*sRatio))/2.0;
    } else {
        float b = 1 - (2*pad);
        float a = b*mRatio;
        menus[biggest]->ssw = a/sRatio;
        menus[biggest]->ssh = b;
        menus[biggest]->x = (1 - (a/sRatio))/2.0;
        menus[biggest]->y = pad;
    }
    // fill in the screen space values and positions for the
    // remaining windows here
    for(int i=0; i<menuNum; i++) {
        if(i != biggest) {
            menus[i]->ssw = ((float)menus[i]->width /
                    (float)menus[biggest]->width) *
                    menus[biggest]->ssw;
            menus[i]->ssh = ((float)menus[i]->height /
                    (float)menus[biggest]->height) *
                    menus[biggest]->ssh;
            menus[i]->x = (1 - menus[i]->ssw)/2.0f;
            menus[i]->y = (1 - menus[i]->ssh)/2.0f;

        }
    }
    // menu and button's right side should be flush against left side of screen
    menus[menuWin]->x = 1 - menus[menuWin]->ssw;
    menus[menuWin]->y = 0;
    menus[menuButton]->x = 1 - menus[menuButton]->ssw;
    menus[menuButton]->y = 0;
    menus[shapeWin]->y = pad;
}

int Gui::checkRegion(float x, float y)
{
    float ls, rs, top, bottom;
    ls = current->x;
    top = current->y;
    rs = ls + current->ssw;
    bottom = top + current->ssh;
    x /= screenw; y /= screenh;
    bool b = x > ls && x < rs && y > top && y < bottom;
    if(!b) {
        // ideally, the outside of the menu region should exist as a separate
        // region with it's own action.
        if(current == menu)
            current = button;
        return -1;
    }

    // put coordinates in "window-space" then send to menu class to check
    x -= ls;
    y -= top;
    x /= current->ssw;
    y /= current->ssh;

    int ret = current->checkRegion(x, y);
    if(ret > -1)
        executeAction(current->region[ret].action, ret + current->sStart);
    return ret;

}
