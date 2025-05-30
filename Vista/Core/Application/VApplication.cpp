#include "VApplication.hpp"
#include "VEventLoop.hpp"
#include <stdexcept>

// Static member initialization
VApplication* VApplication::instance = nullptr;
std::string VApplication::appName = "Vista Application";
std::string VApplication::appVersion = "1.0.0";
std::string VApplication::orgName = "Vista";
std::string VApplication::currentStyle = "default";

VApplication::VApplication() {
    if (instance) {
        throw std::runtime_error("VApplication instance already exists");
    }
    instance = this;
    setName("VApplication");
    initialize();
}

VApplication::~VApplication() {
    cleanup();
    instance = nullptr;
}

void VApplication::initialize() {
    running = true;
    exitCode_ = 0;
    eventLoop = std::make_unique<VEventLoop>();
}

void VApplication::cleanup() {
    if (eventLoop) {
        eventLoop->quit();
    }
    running = false;
}

int VApplication::exec() {
    if (!instance) {
        throw std::runtime_error("No VApplication instance exists");
    }
    
    while (running && eventLoop->isRunning()) {
        eventLoop->processEvents();
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
    
    return exitCode_;
}

void VApplication::quit() {
    if (instance) {
        instance->exit();
    }
}

void VApplication::exit(int exitCode) {
    if (instance) {
        instance->exitCode_ = exitCode;
        instance->running = false;
        if (instance->eventLoop) {
            instance->eventLoop->quit();
        }
    }
}

void VApplication::postEvent(VObject* receiver, std::unique_ptr<VEvent> event) {
    if (instance && instance->eventLoop) {
        instance->eventLoop->postEvent(receiver, std::move(event));
    }
}

void VApplication::sendEvent(VObject* receiver, VEvent* event) {
    if (instance && instance->eventLoop) {
        instance->eventLoop->sendEvent(receiver, event);
    }
}

void VApplication::processEvents() {
    if (instance && instance->eventLoop) {
        instance->eventLoop->processEvents();
    }
}

bool VApplication::hasPendingEvents() const {
    return eventLoop && eventLoop->hasPendingEvents();
}

bool VApplication::isRunning() const {
    return running;
}

int VApplication::exitCode() const {
    return exitCode_;
}

std::string VApplication::applicationName() {
    return appName;
}

void VApplication::setApplicationName(const std::string& name) {
    appName = name;
}

std::string VApplication::applicationVersion() {
    return appVersion;
}

void VApplication::setApplicationVersion(const std::string& version) {
    appVersion = version;
}

std::string VApplication::organizationName() {
    return orgName;
}

void VApplication::setOrganizationName(const std::string& name) {
    orgName = name;
}

void VApplication::setStyle(const std::string& style) {
    currentStyle = style;
}

std::string VApplication::style() {
    return currentStyle;
}

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
    SetConsoleTitleA(title.c_str());
}

void VApplication::setIcon(const std::string& iconPath) {
    std::wstring wIconPath(iconPath.begin(), iconPath.end());
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

    HWND hwnd = GetConsoleWindow();
    if (hwnd) {
        SendMessage(hwnd, WM_SETICON, ICON_SMALL, reinterpret_cast<LPARAM>(hIcon));
        SendMessage(hwnd, WM_SETICON, ICON_BIG, reinterpret_cast<LPARAM>(hIcon));
    } else {
        std::cerr << "Failed to get console window handle" << std::endl;
    }
}

#elif defined(__linux__) || defined(__APPLE__)
void VApplication::setTitleBar(const std::string& title) {
    std::cout << "\033]0;" << title << "\007";
}

void VApplication::setIcon(const std::string& iconPath) {
    // Icon setting not supported on Linux/Unix terminals
    std::cerr << "Setting icon is not supported on this platform" << std::endl;
}
#endif