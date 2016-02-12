#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/keysym.h>
#include <GLES2/gl2.h>
#include <EGL/egl.h>
#include <EGL/eglext.h>
#ifndef GL_WINDOW_H
#define GL_WINDOW_H


class GL_Window
{
public:
    GL_Window();
    GL_Window(int w, int h);
    ~GL_Window();
    void update();
    void destroy();
    void resize(unsigned int, unsigned int);
    Display *xdisplay();
    Window *window();

private:
    Window root, win;
    Display *x_display;
    EGLDisplay display;
    EGLContext context;
    EGLSurface surface;
};

#endif // GL_WINDOW_H
