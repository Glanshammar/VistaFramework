#pragma once

#include <type_traits>
#include <sstream>
#include <iostream>
#include <VString>

#if defined(_WIN32) || defined(_WIN64)
#include <windows.h>
#endif


enum class Color {
    RED,
    GREEN,
    BLUE,
    YELLOW,
    CYAN,
    MAGENTA,
    WHITE,
    DEFAULT
};

enum class Level {
    Info,
    Warning,
    Error
};

void setTextColor(Color color);
void VDebug(Level level, const std::string& message, bool newline = true);
