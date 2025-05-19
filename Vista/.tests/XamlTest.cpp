#include <iostream>
#include <fstream>
#include <sstream>
#include <filesystem>
#include <VApplication>
#include "../Graphics/VXamlParser.hpp"
#include "../Graphics/VistaGUI.hpp"

int main(int argc, char** argv) {
    // Get the executable path
    std::filesystem::path exePath = VApplication::getExecutablePath();
    std::filesystem::path exeDir = exePath.parent_path();
    
    // Get the XAML file path from command line or use default
    std::string xamlFile;
    if (argc > 1) {
        xamlFile = argv[1];
    } else {
        // Use example.xaml in the same directory as the executable
        xamlFile = (exeDir / "example.xaml").string();
    }
    
    // Create a XAML parser
    VXamlParser parser;
    
    // Parse the XAML file
    std::cout << "Parsing XAML file: " << xamlFile << std::endl;
    auto xamlWindow = parser.parseFile(xamlFile);
    
    if (!xamlWindow) {
        std::cerr << "Failed to parse XAML file" << std::endl;
        return 1;
    }
    
    // Create the VistaGUI window
    VistaGUI gui;
    
    // Set the XAML window
    gui.setXamlWindow(xamlWindow);
    
    // Initialize the window with the dimensions from XAML
    if (!gui.init(xamlWindow->getWidth(), xamlWindow->getHeight(), xamlWindow->getTitle().c_str())) {
        std::cerr << "Failed to initialize GUI window" << std::endl;
        return 1;
    }
    
    // Run the GUI event loop
    gui.run();
    
    return 0;
} 