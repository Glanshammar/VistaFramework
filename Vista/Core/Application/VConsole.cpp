#include "VConsole.hpp"

void setTextColor(Color color) {
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

void VDebug(Level level, const std::string& message, bool newline) {
    std::ostringstream formattedMessage;

    switch (level) {
    case Level::Info:
        setTextColor(Color::GREEN);
        formattedMessage << "[INFO] ";
        break;
    case Level::Warning:
        setTextColor(Color::YELLOW);
        formattedMessage << "[WARNING] ";
        break;
    case Level::Error:
        setTextColor(Color::RED);
        formattedMessage << "[ERROR] ";
        break;
    default:
        break;
    }

    formattedMessage << message;

    if (newline) {
        formattedMessage << std::endl;
    }

    std::cout << formattedMessage.str();
    setTextColor(Color::DEFAULT);
}

