#include "VistaGUI.hpp"
#include "VXamlParser.hpp"
#include <iostream>
#include <cstdlib>

#if defined(__linux__)
#include <unistd.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#endif

// ----------------------------------------
// VistaGUI Implementation
// ----------------------------------------

VistaGUI::VistaGUI() : platform(Platform::Unknown) {
    setName("VistaGUI");
    platform = detectPlatform();
}

VistaGUI::~VistaGUI() = default;

void VistaGUI::setXamlWindow(std::shared_ptr<XamlWindow> xamlWindow) {
    this->xamlWindow = xamlWindow;
    if (window) {
        window->setXamlWindow(xamlWindow);
    }
}

bool VistaGUI::init(int width, int height, const char* title) {
    platform = detectPlatform();
    
    // Create the appropriate window based on the detected platform
    switch (platform) {
        case Platform::Windows:
#if defined(_WIN32) || defined(_WIN64)
            window = std::make_unique<WindowsWindow>();
#endif
            break;
            
        case Platform::LinuxX11:
#if defined(__linux__)
            window = std::make_unique<X11Window>();
#endif
            break;
            
        case Platform::LinuxWayland:
#if defined(__linux__)
            window = std::make_unique<WaylandWindow>();
#endif
            break;
            
        case Platform::MacOS:
#if defined(__APPLE__)
            window = std::make_unique<MacOSWindow>();
#endif
            break;
            
        default:
            std::cerr << "Unsupported platform detected." << std::endl;
            return false;
    }
    
    if (!window) {
        std::cerr << "Failed to create window for the current platform." << std::endl;
        return false;
    }
    
    // Initialize the window
    if (!window->init(width, height, title)) {
        return false;
    }
    
    // Set the XAML window if one exists
    if (xamlWindow) {
        window->setXamlWindow(xamlWindow);
    }
    
    return true;
}

void VistaGUI::run() {
    if (window) {
        window->run();
    }
}

VistaGUI::Platform VistaGUI::detectPlatform() {
#if defined(_WIN32) || defined(_WIN64)
    return Platform::Windows;
#elif defined(__linux__)
    if (isWaylandSession()) {
        return Platform::LinuxWayland;
    } else {
        return Platform::LinuxX11;
    }
#elif defined(__APPLE__)
    return Platform::MacOS;
#else
    return Platform::Unknown;
#endif
}

bool VistaGUI::isWaylandSession() {
#if defined(__linux__)
    // Check for Wayland by examining the XDG_SESSION_TYPE environment variable
    const char* sessionType = std::getenv("XDG_SESSION_TYPE");
    if (sessionType && std::string(sessionType) == "wayland") {
        return true;
    }
    
    // Also check WAYLAND_DISPLAY as a fallback
    const char* waylandDisplay = std::getenv("WAYLAND_DISPLAY");
    return waylandDisplay != nullptr;
#else
    return false;
#endif
}

// ----------------------------------------
// X11Window Implementation
// ----------------------------------------
#if defined(__linux__)
X11Window::X11Window() : display(nullptr), window(0), screen(0) {}

X11Window::~X11Window() {
    if (display) {
        XDestroyWindow(display, window);
        XCloseDisplay(display);
    }
}

bool X11Window::init(int width, int height, const char* title) {
    display = XOpenDisplay(nullptr);
    if (!display) {
        std::cerr << "Cannot open X display" << std::endl;
        return false;
    }

    screen = DefaultScreen(display);
    int blackColor = BlackPixel(display, screen);
    int whiteColor = WhitePixel(display, screen);

    window = XCreateSimpleWindow(
        display,
        RootWindow(display, screen),
        100, 100,    // position
        width, height, // size
        1,           // border width
        blackColor,  // border color
        whiteColor   // background color
    );

    // Set window title
    XStoreName(display, window, title);

    // Select kind of events
    XSelectInput(display, window,
        ExposureMask | KeyPressMask | ButtonPressMask | StructureNotifyMask);

    // Show the window
    XMapWindow(display, window);
    XFlush(display);

    return true;
}

void X11Window::run() {
    XEvent event;
    bool running = true;

    while (running) {
        XNextEvent(display, &event);

        switch (event.type) {
            case Expose:
                redraw();
                break;

            case KeyPress:
                running = false;
                break;

            case ButtonPress:
                handleMouseClick(event.xbutton.x, event.xbutton.y);
                break;

            case ConfigureNotify:
                // Window was resized
                break;
        }
    }
}

void X11Window::redraw() {
    if (!xamlWindow) return;

    GC gc = XCreateGC(display, window, 0, nullptr);
    XSetForeground(display, gc, 0x000000); // Black text

    // Draw all XAML elements
    for (const auto& child : xamlWindow->getChildren()) {
        drawXamlElement(child, 0, 0);
    }

    XFreeGC(display, gc);
}

void X11Window::drawXamlElement(const std::shared_ptr<XamlElement>& element, int x, int y) {
    if (!element) return;

    if (auto grid = std::dynamic_pointer_cast<XamlGrid>(element)) {
        // Draw grid children
        for (const auto& child : grid->getChildren()) {
            drawXamlElement(child, x, y);
        }
    } else if (auto textBlock = std::dynamic_pointer_cast<XamlTextBlock>(element)) {
        drawTextBlock(textBlock, x, y);
    } else if (auto textBox = std::dynamic_pointer_cast<XamlTextBox>(element)) {
        drawTextBox(textBox, x, y);
    } else if (auto button = std::dynamic_pointer_cast<XamlButton>(element)) {
        drawButton(button, x, y);
    }
}

void X11Window::drawTextBlock(const std::shared_ptr<XamlTextBlock>& textBlock, int x, int y) {
    if (!textBlock) return;

    GC gc = XCreateGC(display, window, 0, nullptr);
    XSetForeground(display, gc, 0x000000); // Black text

    // Draw the text
    XDrawString(display, window, gc, x, y + 15, textBlock->getText().c_str(), textBlock->getText().length());

    XFreeGC(display, gc);
}

void X11Window::drawTextBox(const std::shared_ptr<XamlTextBox>& textBox, int x, int y) {
    if (!textBox) return;

    GC gc = XCreateGC(display, window, 0, nullptr);
    XSetForeground(display, gc, 0x000000); // Black border

    // Draw text box border
    XDrawRectangle(display, window, gc, x, y, textBox->getWidth(), 25);

    XFreeGC(display, gc);
}

void X11Window::drawButton(const std::shared_ptr<XamlButton>& button, int x, int y) {
    if (!button) return;

    GC gc = XCreateGC(display, window, 0, nullptr);
    XSetForeground(display, gc, 0x000000); // Black border

    // Draw button border
    XDrawRectangle(display, window, gc, x, y, button->getWidth(), button->getHeight());

    // Draw button text
    XDrawString(display, window, gc, x + 10, y + 20, button->getContent().c_str(), button->getContent().length());

    XFreeGC(display, gc);
}

void X11Window::handleMouseClick(int x, int y) {
    std::cout << "Mouse clicked at: (" << x << ", " << y << ")" << std::endl;
}

void X11Window::setXamlWindow(std::shared_ptr<XamlWindow> xamlWindow) {
    this->xamlWindow = xamlWindow;
}

// ----------------------------------------
// WaylandWindow Implementation (placeholder)
// ----------------------------------------
struct WaylandWindow::WaylandData {
    // Wayland-specific data would go here
};

WaylandWindow::WaylandWindow() : data(std::make_unique<WaylandData>()) {}

WaylandWindow::~WaylandWindow() = default;

bool WaylandWindow::init(int width, int height, const char* title) {
    std::cout << "Initializing Wayland window: " << title << " (" << width << "x" << height << ")" << std::endl;
    // Wayland initialization would go here
    // This is just a placeholder for now
    return true;
}

void WaylandWindow::run() {
    std::cout << "Running Wayland window (placeholder)" << std::endl;
    // Wayland event loop would go here
    // For now, just sleep for demonstration
    sleep(5);
}

void WaylandWindow::redraw() {
    // Wayland redraw would go here
}

void WaylandWindow::setXamlWindow(std::shared_ptr<XamlWindow> xamlWindow) {
    // Wayland implementation will be added later
    std::cout << "Wayland XAML support not implemented yet" << std::endl;
}
#endif // __linux__

// ----------------------------------------
// WindowsWindow Implementation
// ----------------------------------------
#if defined(_WIN32) || defined(_WIN64)
WindowsWindow::WindowsWindow() : hwnd(nullptr), hdc(nullptr), hInstance(GetModuleHandle(nullptr)) {}

WindowsWindow::~WindowsWindow() {
    // Cleanup resources
    if (hwnd) {
        DestroyWindow(hwnd);
    }
}

LRESULT CALLBACK WindowsWindow::WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    WindowsWindow* window = nullptr;
    
    if (uMsg == WM_CREATE) {
        CREATESTRUCT* createStruct = reinterpret_cast<CREATESTRUCT*>(lParam);
        window = static_cast<WindowsWindow*>(createStruct->lpCreateParams);
        SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(window));
    } else {
        window = reinterpret_cast<WindowsWindow*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
    }
    
    switch (uMsg) {
        case WM_PAINT:
            if (window) {
                window->redraw();
            }
            return 0;
            
        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;
            
        default:
            return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }
}

bool WindowsWindow::init(int width, int height, const char* title) {
    // Register window class
    WNDCLASSEX wc = {};
    wc.cbSize = sizeof(WNDCLASSEX);
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wc.hbrBackground = reinterpret_cast<HBRUSH>(COLOR_WINDOW + 1);
    wc.lpszClassName = "VistaWindowClass";
    
    if (!RegisterClassEx(&wc)) {
        std::cerr << "Failed to register window class" << std::endl;
        return false;
    }
    
    // Create window
    hwnd = CreateWindowEx(
        0,
        "VistaWindowClass",
        title,
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT,
        width, height,
        nullptr,
        nullptr,
        hInstance,
        this  // Pass 'this' pointer to WM_CREATE
    );
    
    if (!hwnd) {
        std::cerr << "Failed to create window" << std::endl;
        return false;
    }
    
    // Show window
    ShowWindow(hwnd, SW_SHOW);
    UpdateWindow(hwnd);
    
    // Get DC for painting
    hdc = GetDC(hwnd);
    
    return true;
}

void WindowsWindow::run() {
    MSG msg = {};
    while (GetMessage(&msg, nullptr, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
}

void WindowsWindow::redraw() {
    if (!hdc) return;
    
    // Example drawing code
    RECT rect;
    GetClientRect(hwnd, &rect);
    
    // Draw a blue rectangle
    HBRUSH blueBrush = CreateSolidBrush(RGB(0, 0, 255));
    RECT blueRect = {50, 50, 250, 150};
    FillRect(hdc, &blueRect, blueBrush);
    DeleteObject(blueBrush);
    
    // Draw a red rectangle outline
    HPEN redPen = CreatePen(PS_SOLID, 1, RGB(255, 0, 0));
    HPEN oldPen = (HPEN)SelectObject(hdc, redPen);
    RECT redRect = {300, 100, 500, 250};
    Rectangle(hdc, redRect.left, redRect.top, redRect.right, redRect.bottom);
    SelectObject(hdc, oldPen);
    DeleteObject(redPen);
}
#endif // _WIN32 || _WIN64

// ----------------------------------------
// MacOSWindow Implementation (placeholder)
// ----------------------------------------
#if defined(__APPLE__)
MacOSWindow::MacOSWindow() : nsWindow(nullptr), nsView(nullptr) {}

MacOSWindow::~MacOSWindow() {
    // Cleanup Cocoa resources would go here
}

bool MacOSWindow::init(int width, int height, const char* title) {
    std::cout << "Initializing macOS window: " << title << " (" << width << "x" << height << ")" << std::endl;
    // Cocoa initialization would go here
    // This is just a placeholder
    return true;
}

void MacOSWindow::run() {
    std::cout << "Running macOS window (placeholder)" << std::endl;
    // Cocoa event loop would go here
}

void MacOSWindow::redraw() {
    // Cocoa drawing would go here
}
#endif // __APPLE__