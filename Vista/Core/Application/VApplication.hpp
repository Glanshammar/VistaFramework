#pragma once

#include <cstdint>
#include <atomic>
#include <string>
#include <iostream>
#include <filesystem>
#include <queue>
#include <functional>
#include <memory>
#include <mutex>
#include <VObject>

#if defined(_WIN32) || defined(_WIN64)
#include <windows.h>
#elif defined(__linux__)
#include <unistd.h>
#include <libgen.h>
#elif defined(__APPLE__)
#include <unistd.h>
#include <libproc.h>
#endif

// Forward declarations
class VEvent;
class VEventLoop;

class VApplication : public VObject {
public:
    VApplication();
    ~VApplication() override;

    // Core application methods
    static void setTitleBar(const std::string& title = "VistaCore Application");
    static void setIcon(const std::string& iconPath = "icon.ico");
    static std::filesystem::path getExecutablePath();
    
    // Application lifecycle
    int exec();
    void quit();
    void exit(int exitCode = 0);
    
    // Event handling
    void postEvent(VObject* receiver, std::unique_ptr<VEvent> event);
    void sendEvent(VObject* receiver, VEvent* event);
    void processEvents();
    bool hasPendingEvents() const;
    
    // Application state
    bool isRunning() const;
    int exitCode() const;
    
    // Application information
    static std::string applicationName();
    static void setApplicationName(const std::string& name);
    static std::string applicationVersion();
    static void setApplicationVersion(const std::string& version);
    static std::string organizationName();
    static void setOrganizationName(const std::string& name);
    
    // Platform-specific methods
    static void setStyle(const std::string& style);
    static std::string style();
    
private:
    static VApplication* instance;
    std::unique_ptr<VEventLoop> eventLoop;
    std::atomic<bool> running{false};
    std::atomic<int32_t> exitCode_{0};
    static std::string appName;
    static std::string appVersion;
    static std::string orgName;
    static std::string currentStyle;
    
    void initialize();
    void cleanup();
};