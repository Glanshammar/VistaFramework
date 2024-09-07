#pragma once
#include <cstdint>
#include <VThread>
#include <VGlobals>
#include <GLFW/glfw3.h>
#include <iostream>

class VGL {
private:
public:
    VGL();
    ~VGL();
    void OpenWindow(int32_t x, int32_t y, const char* windowTitle, const char* iconPath);
};
