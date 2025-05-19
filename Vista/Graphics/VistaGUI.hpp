#pragma once
#include <cstdint>
#include <memory>
#include <string>
#include <VThread>
#include <VGlobals>
#include <VApplication>
#include "VXamlParser.hpp"

// Define platform-specific includes based on OS
#if defined(_WIN32) || defined(_WIN64)
    #include <windows.h>
#elif defined(__linux__)
    // Linux-specific includes
    #include <X11/Xlib.h>
    #include <X11/Xutil.h>
    // We'll check for Wayland at runtime
#elif defined(__APPLE__)
    // macOS-specific includes
    #include <Cocoa/Cocoa.h>
#endif

// Forward declarations
class WindowBase;
class X11Window;
class WaylandWindow;
class WindowsWindow;
class MacOSWindow;

// Platform-agnostic GUI class
class VistaGUI : public VApplication {
public:
    VistaGUI();
    ~VistaGUI();

    // Platform enum for detecting the current platform
    enum class Platform {
        Unknown,
        Windows,
        LinuxX11,
        LinuxWayland,
        MacOS
    };

    // Initialize with common parameters
    bool init(int width = 800, int height = 600, const char* title = "Vista Window");
    void run();
    
    // Platform detection methods
    static Platform detectPlatform();
    static bool isWaylandSession();

    // Set the XAML window to be rendered
    void setXamlWindow(std::shared_ptr<XamlWindow> xamlWindow);

private:
    std::unique_ptr<WindowBase> window;
    Platform platform;
    std::shared_ptr<XamlWindow> xamlWindow;
};

// Base window interface
class WindowBase {
public:
    virtual ~WindowBase() = default;
    virtual bool init(int width, int height, const char* title) = 0;
    virtual void run() = 0;
    virtual void redraw() = 0;
    virtual void setXamlWindow(std::shared_ptr<XamlWindow> xamlWindow) = 0;
};

#if defined(__linux__)
// X11 Window implementation
class X11Window : public WindowBase {
public:
    X11Window();
    ~X11Window() override;

    bool init(int width, int height, const char* title) override;
    void run() override;
    void redraw() override;
    void setXamlWindow(std::shared_ptr<XamlWindow> xamlWindow) override;

private:
    void handleMouseClick(int x, int y);
    void drawXamlElement(const std::shared_ptr<XamlElement>& element, int x, int y);
    void drawTextBlock(const std::shared_ptr<XamlTextBlock>& textBlock, int x, int y);
    void drawTextBox(const std::shared_ptr<XamlTextBox>& textBox, int x, int y);
    void drawButton(const std::shared_ptr<XamlButton>& button, int x, int y);

    Display* display;
    Window window;
    int screen;
    std::shared_ptr<XamlWindow> xamlWindow;
};

// Wayland Window implementation
class WaylandWindow : public WindowBase {
public:
    WaylandWindow();
    ~WaylandWindow() override;

    bool init(int width, int height, const char* title) override;
    void run() override;
    void redraw() override;
    void setXamlWindow(std::shared_ptr<XamlWindow> xamlWindow) override;

private:
    // Wayland specific members would go here
    // This is a placeholder for now
    struct WaylandData;
    std::unique_ptr<WaylandData> data;
};
#endif

#if defined(_WIN32) || defined(_WIN64)
// Windows Window implementation
class WindowsWindow : public WindowBase {
public:
    WindowsWindow();
    ~WindowsWindow() override;

    bool init(int width, int height, const char* title) override;
    void run() override;
    void redraw() override;
    void setXamlWindow(std::shared_ptr<XamlWindow> xamlWindow) override;

private:
    static LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
    
    HWND hwnd;
    HDC hdc;
    HINSTANCE hInstance;
};
#endif

#if defined(__APPLE__)
// macOS Window implementation
class MacOSWindow : public WindowBase {
public:
    MacOSWindow();
    ~MacOSWindow() override;

    bool init(int width, int height, const char* title) override;
    void run() override;
    void redraw() override;
    void setXamlWindow(std::shared_ptr<XamlWindow> xamlWindow) override;

private:
    // Cocoa specific members would go here
    // This is a placeholder for now
    void* nsWindow;  // NSWindow*
    void* nsView;    // NSView*
};
#endif