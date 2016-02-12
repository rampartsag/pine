#include "gl_window.h"

GL_Window::GL_Window()
{

}

GL_Window::GL_Window(int w, int h)
{
    int width = w;
    int height = h;
    XSetWindowAttributes swa;
    x_display = XOpenDisplay(NULL);
    DefaultScreen(x_display);
    root = DefaultRootWindow(x_display);
    swa.event_mask = ExposureMask | PointerMotionMask | KeyPressMask | ButtonPressMask |
                     MotionNotify | ButtonReleaseMask | StructureNotifyMask;

    win = XCreateWindow(x_display, root, 0, 0, width, height, 0,
                        CopyFromParent, InputOutput, CopyFromParent, CWEventMask, &swa);

    // handle window close correctly

    XMapWindow(x_display, win);
    Atom delmsg = XInternAtom(x_display, "WM_DELETE_WINDOW", False);
    XSetWMProtocols(x_display, win, &delmsg, 1);

    // limit resize to 16:9 ratio

    XSizeHints *hints = XAllocSizeHints();
    hints->flags |= PAspect;
    hints->min_aspect.x = 16;
    hints->min_aspect.y = 9;
    hints->max_aspect.x = 16;
    hints->max_aspect.y = 9;
    XSetWMNormalHints(x_display, win, hints);
    XFree(hints);


    // EGL stuff

    EGLint major, minor;
    EGLint numConfigs;
    EGLConfig config;
    const EGLint configAttribs[] = {
            EGL_RENDERABLE_TYPE, EGL_WINDOW_BIT,
            EGL_RED_SIZE, 8,
            EGL_GREEN_SIZE, 8,
            EGL_BLUE_SIZE, 8,
            EGL_DEPTH_SIZE, 24,
            EGL_NONE
    };
    const EGLint contextAttribs[] = {
            EGL_CONTEXT_CLIENT_VERSION, 2,
            EGL_NONE
    };
    display = eglGetDisplay((EGLNativeDisplayType)x_display);
    eglInitialize(display, &major, &minor);
    eglChooseConfig(display, configAttribs, &config, 1, &numConfigs);
    surface = eglCreateWindowSurface(display, config,
                    (EGLNativeWindowType)win, NULL);
    context = eglCreateContext(display, config,
                    EGL_NO_CONTEXT, contextAttribs);
    eglMakeCurrent(display, surface, surface, context);



}

GL_Window::~GL_Window()
{
    eglMakeCurrent(display, NULL, NULL, NULL);
    eglDestroyContext(display, context);
    eglDestroySurface(display, surface);
    eglTerminate(display);

}

void GL_Window::update()
{
    eglSwapBuffers(display, surface);

}

void GL_Window::destroy()
{
    XDestroyWindow(x_display, win);
    XCloseDisplay(x_display);
}

void GL_Window::resize(unsigned int w, unsigned int h)
{
    XResizeWindow(x_display, win, w, h);

}

Display *GL_Window::xdisplay()
{
    return x_display;

}

Window *GL_Window::window()
{
    return &win;

}

