#pragma once

#include <string>
#include <iostream>
#include <filesystem>
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

class VApplication : public VObject {
public:
    VApplication();
    ~VApplication();
    static void setTitleBar(const std::string& title = "VistaCore Application");
    static void setIcon(const std::string& iconPath = "icon.ico");
    
    // Get the path to the current executable
    static std::filesystem::path getExecutablePath();
};