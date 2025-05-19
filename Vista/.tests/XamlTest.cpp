#include <iostream>
#include <fstream>
#include <sstream>
#include <VApplication>
#include "../Graphics/VXamlParser.hpp"
#include "../Graphics/VistaGUI.hpp"

// Example XAML string
const char* exampleXaml = R"(
<Window
    xmlns="http://schemas.microsoft.com/winfx/2006/xaml/presentation"
    xmlns:x="http://schemas.microsoft.com/winfx/2006/xaml"
    Title="Welcome to Vista"
    Height="250"
    Width="400">
    <Grid Margin="20">
        <Grid.RowDefinitions>
            <RowDefinition Height="Auto"/>
            <RowDefinition Height="Auto"/>
            <RowDefinition Height="Auto"/>
            <RowDefinition Height="*"/>
        </Grid.RowDefinitions>
        <TextBlock
            Text="Welcome to Vista Framework!"
            FontSize="18"
            FontWeight="Bold"
            HorizontalAlignment="Center"
            Margin="0,0,0,15"
            Grid.Row="0"/>
        <TextBlock
            Text="Enter your name:"
            Grid.Row="1"
            VerticalAlignment="Center"/>
        <TextBox
            Name="NameInput"
            Width="200"
            Margin="10,5"
            Grid.Row="2"
            HorizontalAlignment="Left"/>
        <Button
            Content="Submit"
            Width="100"
            Height="30"
            Grid.Row="3"
            HorizontalAlignment="Right"
            Margin="0,15,0,0"/>
    </Grid>
</Window>
)";

// Function to save the example XAML to a file
void saveExampleXamlToFile(const std::string& filename) {
    std::ofstream file(filename);
    if (file.is_open()) {
        file << exampleXaml;
        file.close();
        std::cout << "Saved example XAML to " << filename << std::endl;
    } else {
        std::cerr << "Failed to open file for writing: " << filename << std::endl;
    }
}

int main(int argc, char** argv) {
    // Create a XAML parser
    VXamlParser parser;
    
    // Save the example XAML to a file
    std::string filename = "example.xaml";
    saveExampleXamlToFile(filename);
    
    // Parse the XAML file
    std::cout << "Parsing XAML file..." << std::endl;
    auto xamlWindow = parser.parseFile(filename);
    
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