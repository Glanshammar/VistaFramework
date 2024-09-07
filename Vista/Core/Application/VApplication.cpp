#include "VApplication.hpp"

VApplication::VApplication() {
    setName("VApplication");
}

VApplication::~VApplication() = default;

#if defined(_WIN32) || defined(_WIN64)
void VApplication::setTitleBar(const std::string& title) {
    SetConsoleTitleA(title.c_str()); // Use SetConsoleTitleA for narrow string
}

void VApplication::setIcon(const std::string& iconPath) {
    // Convert iconPath to wide string
    std::wstring wIconPath(iconPath.begin(), iconPath.end());

    // Load the icon from file
    auto hIcon = static_cast<HICON>(LoadImage(
            nullptr,
            wIconPath.c_str(),
            IMAGE_ICON,
            0,
            0,
            LR_LOADFROMFILE | LR_DEFAULTSIZE | LR_SHARED
    ));

    if (!hIcon) {
        std::cerr << "Failed to load icon from " << iconPath << std::endl;
        return;
    }

    // Set the icon for the console window
    HWND hwnd = GetConsoleWindow();
    if (hwnd) {
        // Set both small and large icons
        SendMessage(hwnd, WM_SETICON, ICON_SMALL, reinterpret_cast<LPARAM>(hIcon));
        SendMessage(hwnd, WM_SETICON, ICON_BIG, reinterpret_cast<LPARAM>(hIcon));
    } else {
        std::cerr << "Failed to get console window handle" << std::endl;
    }
}

#elif defined(__linux__)
void VApplication::setTitleBar(const std::string& title) {
    // Set console title (if applicable)
    std::cout << "\033]0;" << title << "\007"; // ANSI escape sequence to set terminal title
}

#elif defined(__APPLE__)
void VApplication::setTitleBar(const std::string& title) {
    // Set console title (if applicable)
    std::cout << "\033]0;" << title << "\007"; // ANSI escape sequence to set terminal title
}
#endif