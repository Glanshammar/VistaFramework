#include "VConsole.hpp"



VConsole::~VConsole() {
    std::cerr << oss.str() << std::endl;
}

void VConsole::setTextColor(Color color) {
#if defined(_WIN32) || defined(_WIN64)
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    switch (color) {
        case Color::RED:
            SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_INTENSITY);
            break;
        case Color::GREEN:
            SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
            break;
        case Color::BLUE:
            SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE | FOREGROUND_INTENSITY);
            break;
        case Color::YELLOW:
            SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
            break;
        case Color::CYAN:
            SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
            break;
        case Color::MAGENTA:
            SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
            break;
        case Color::WHITE:
            SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
            break;
        case Color::DEFAULT:
        default:
            SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
            break;
    }
#else
    switch (color) {
        case Color::RED:
            std::cout << "\033[31m";
            break;
        case Color::GREEN:
            std::cout << "\033[32m";
            break;
        case Color::BLUE:
            std::cout << "\033[34m";
            break;
        case Color::YELLOW:
            std::cout << "\033[33m";
            break;
        case Color::CYAN:
            std::cout << "\033[36m";
            break;
        case Color::MAGENTA:
            std::cout << "\033[35m";
            break;
        case Color::WHITE:
            std::cout << "\033[37m";
            break;
        case Color::DEFAULT:
        default:
            std::cout << "\033[0m";
            break;
    }
#endif
}

void VConsole::log() {}

template <typename First, typename... Rest>
void VConsole::log(const First& first, const Rest&... rest) {
    handleArgument(first);
    if constexpr (sizeof...(rest) > 0) {
        oss << ", "; // Separate with comma
        log(rest...); // Recursively call log for remaining arguments
    }
}

void VConsole::handleArgument(const std::string& text) {
    oss << text;
}

void VConsole::handleArgument(Color color) {
    setTextColor(color);
}