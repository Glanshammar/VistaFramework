#pragma once

#include "../VObject.hpp"

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

// Base case for the variadic template function
inline void VPrint(const std::ostringstream &oss) {
    std::cout << oss.str() << std::endl;
}

// Recursive variadic template function
template<typename T, typename... Args>
void VPrint(std::ostringstream &oss, T first, Args... args) {
    oss << first;
    VPrint(oss, args...);
}

// Entry point for the variadic print function
template<typename... Args>
void VPrint(Args... args) {
    std::ostringstream oss;
    VPrint(oss, args...);
}