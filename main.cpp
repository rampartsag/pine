#include "engine.h"
#include <iostream>
#include <cstdio>
#include "gl_window.h"

using namespace std;

// get options here later
//int main(int argc, char **argv) {
int main() {
    int w = 1000;
    int h = w*9/16;
    GL_Window window(w, h);
    XEvent event;
    Display *x_display = window.xdisplay();
    bool running = true;
    Engine *engine = new Engine();
    engine->init();
    engine->resize(w, h);
    int x, y;

    while(running) {
        while (XPending(x_display)) {
            XNextEvent(x_display, &event);
            if (event.type == Expose) {
            }
            if (event.type == ConfigureNotify) {
                XConfigureEvent xce = event.xconfigure;
                if(xce.width != w || xce.height != h) {
                    w = xce.width;
                    h = xce.height;
                    engine->resize(w, h);
                    window.update();
                }
            }

            if(event.type == ButtonPress) {
                x = event.xmotion.x;
                y = event.xmotion.y;
                engine->processTouch(1, (float)x, (float)y);
            }
            if(event.type == ButtonRelease) {
                x = event.xmotion.x;
                y = event.xmotion.y;
                engine->processTouch(4, (float)x, (float)y);
            }
            if(event.type == MotionNotify) {
                if (event.xmotion.state & Button1Mask) {
                    x = event.xmotion.x;
                    y = event.xmotion.y;
                    engine->processTouch(2, (float)x, (float)y);

                }
            }
            if (event.type == KeyPress) {
                switch(XLookupKeysym(&event.xkey,0)) {
                case XK_q:
                    running = false;
                    break;
                case XK_1:
                    engine->test(1);
                    break;
                case XK_2:
                    engine->test(2);
                    break;
                case XK_3:
                    engine->test(128);
                    break;
                case XK_4:
                    engine->test(8);
                    break;
                case XK_5:
                    engine->test(16);
                    break;
                case XK_6:
                    engine->test(32);
                    break;
                case XK_7:
                    engine->test(64);
                    break;
                case XK_8:
                    int x = 800;
                    int y = 450;
                    window.resize(x, y);
                    engine->resize(x, y);
                    break;
                }
            }
            if (event.type == ClientMessage) {
                Atom msg = XInternAtom(x_display, "WM_DELETE_WINDOW", False);
                if ((unsigned int)event.xclient.data.l[0] == msg)
                    running = false;
                //break;
            }
            if(event.type == DestroyNotify) {
                running = false;
            }
            XFlush(x_display);

        }
        window.update();
        if(engine->update() == 1) running = false;
    }
    delete engine;
    return 0;
}
