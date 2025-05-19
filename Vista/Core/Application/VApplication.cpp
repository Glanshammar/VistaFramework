#include "VApplication.hpp"

VApplication::VApplication() {
    setName("VApplication");
}

VApplication::~VApplication() = default;

std::filesystem::path VApplication::getExecutablePath() {
#if defined(_WIN32) || defined(_WIN64)
    char path[MAX_PATH];
    GetModuleFileNameA(nullptr, path, MAX_PATH);
    return std::filesystem::path(path);
#elif defined(__linux__)
    char path[PATH_MAX];
    ssize_t count = readlink("/proc/self/exe", path, PATH_MAX);
    if (count == -1) {
        throw std::runtime_error("Failed to get executable path");
    }
    path[count] = '\0';
    return std::filesystem::path(path);
#elif defined(__APPLE__)
    char path[PROC_PIDPATHINFO_MAXSIZE];
    pid_t pid = getpid();
    if (proc_pidpath(pid, path, sizeof(path)) <= 0) {
        throw std::runtime_error("Failed to get executable path");
    }
    return std::filesystem::path(path);
#else
    throw std::runtime_error("Unsupported platform");
#endif
}

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