#include <iostream>
#include <VApplication>
#include "../Graphics/VistaGUI.hpp"

int main(int argc, char** argv) {
    // Create a platform-independent GUI
    VistaGUI gui;
    
    // Display platform information
    VistaGUI::Platform platform = VistaGUI::detectPlatform();
    std::cout << "Detected platform: ";
    switch (platform) {
        case VistaGUI::Platform::Windows:
            std::cout << "Windows";
            break;
        case VistaGUI::Platform::LinuxX11:
            std::cout << "Linux (X11)";
            break;
        case VistaGUI::Platform::LinuxWayland:
            std::cout << "Linux (Wayland)";
            break;
        case VistaGUI::Platform::MacOS:
            std::cout << "macOS";
            break;
        default:
            std::cout << "Unknown";
            break;
    }
    std::cout << std::endl;
    
    // Initialize the window with default parameters
    if (!gui.init(1280, 720, "Vista Cross-Platform Window")) {
        std::cerr << "Failed to initialize GUI window" << std::endl;
        return 1;
    }
    
    // Run the GUI event loop
    gui.run();
    
    return 0;
}