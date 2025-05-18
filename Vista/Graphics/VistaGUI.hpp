#pragma once
#include <cstdint>
#include <VThread>
#include <VGlobals>
#include <VApplication>
#include <X11/Xlib.h>
#include <X11/Xutil.h>

class X11Window {
public:
    X11Window();
    ~X11Window();

    bool init(int width = 800, int height = 600, const char* title = "Vista X11 Window");
    void run();

private:
    void redraw();
    void handleMouseClick(int x, int y);

    Display* display;
    Window window;
    int screen;
};
