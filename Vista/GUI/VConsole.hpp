#pragma once

#include <type_traits>
#include <sstream>
#include <iostream>
#include <VString>
#include <codecvt>
#include <locale>
#include <iomanip>

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


inline void VPrint() {
    std::cout << std::endl;
}

// Recursive case: print the first argument and recursively call VPrint with the rest
template<typename T, typename... Args>
void VPrint(const T& first, const Args&... rest) {
    std::cout << first;
    if constexpr (sizeof...(rest) > 0) {
        std::cout << " ";
    }
    VPrint(rest...);
}