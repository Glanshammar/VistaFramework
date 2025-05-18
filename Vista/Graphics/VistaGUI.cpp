#include "VistaGUI.hpp"
#include <iostream>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <unistd.h>

X11Window::X11Window() : display(nullptr), window(0), screen(0) {}

X11Window::~X11Window() {
    if (display) {
        XDestroyWindow(display, window);
        XCloseDisplay(display);
    }
}

bool X11Window::init(int width, int height, const char* title) {
    display = XOpenDisplay(nullptr);
    if (!display) {
        std::cerr << "Cannot open X display" << std::endl;
        return false;
    }

    screen = DefaultScreen(display);
    int blackColor = BlackPixel(display, screen);
    int whiteColor = WhitePixel(display, screen);

    window = XCreateSimpleWindow(
        display,
        RootWindow(display, screen),
        100, 100,    // position
        width, height, // size
        1,           // border width
        blackColor,  // border color
        whiteColor   // background color
    );

    // Set window title
    XStoreName(display, window, title);

    // Select kind of events
    XSelectInput(display, window,
        ExposureMask | KeyPressMask | ButtonPressMask | StructureNotifyMask);

    // Show the window
    XMapWindow(display, window);
    XFlush(display);

    return true;
}

void X11Window::run() {
    XEvent event;
    bool running = true;

    while (running) {
        XNextEvent(display, &event);

        switch (event.type) {
            case Expose:
                redraw();
                break;

            case KeyPress:
                running = false;
                break;

            case ButtonPress:
                handleMouseClick(event.xbutton.x, event.xbutton.y);
                break;

            case ConfigureNotify:
                // Window was resized
                break;
        }
    }
}

void X11Window::redraw() {
    GC gc = XCreateGC(display, window, 0, nullptr);

    XSetForeground(display, gc, 0x0000FF);
    XFillRectangle(display, window, gc, 50, 50, 200, 100);
    XSetForeground(display, gc, 0xFF0000);
    XDrawRectangle(display, window, gc, 300, 100, 200, 150);
    XFreeGC(display, gc);
}

void X11Window::handleMouseClick(int x, int y) {
    std::cout << "Mouse clicked at: (" << x << ", " << y << ")" << std::endl;

    GC gc = XCreateGC(display, window, 0, nullptr);
    XSetForeground(display, gc, 0x00FF00); // Green
    XFillRectangle(display, window, gc, x-5, y-5, 10, 10);
    XFreeGC(display, gc);
}