#pragma once

#include "../VObject.hpp"

enum class Color {
    DEFAULT,
    RED,
    GREEN,
    BLUE,
    YELLOW,
    CYAN,
    MAGENTA,
    WHITE
};


class VConsole : public VObject {
public:
    VConsole() = default;
    ~VConsole();

    void setTextColor(Color color);

    void log();

    // Variadic template function to handle multiple arguments
    template <typename First, typename... Rest>
    void log(const First& first, const Rest&... rest);

    template <typename... Args>
    void Print(Args&&... args) {
        log(std::forward<Args>(args)...);
    }

private:
    std::ostringstream oss;

    // Handle argument and check if it's a color
    void handleArgument(const std::string& text);
    void handleArgument(Color color);
};