#pragma once

#include <type_traits>
#include <sstream>
#include <iostream>
#include <codecvt>
#include <locale>
#include <iomanip>
#include <string_view>
#include <source_location>
#include <chrono>

#if defined(_WIN32) || defined(_WIN64)
#include <windows.h>
#endif

enum class Color {
    RED,
    GREEN,
    BLUE,
    YELLOW,
    CYAN,
    MAGENTA,
    WHITE,
    DEFAULT
};

enum class Level {
    Info,
    Warning,
    Error
};

// Forward declarations
void setTextColor(Color color);
void VDebug(Level level, const std::string& message);

// Configuration struct for VPrint
struct PrintConfig {
    bool timestamp = false;
    bool sourceLocation = false;
    Color color = Color::DEFAULT;
    bool bold = false;
    bool newline = true;
    
    PrintConfig& withTimestamp(bool value = true) { timestamp = value; return *this; }
    PrintConfig& withSourceLocation(bool value = true) { sourceLocation = value; return *this; }
    PrintConfig& withColor(Color value) { color = value; return *this; }
    PrintConfig& withBold(bool value = true) { bold = value; return *this; }
    PrintConfig& withoutNewline() { newline = false; return *this; }
};

// Helper to format timestamp
inline std::string formatTimestamp() {
    auto now = std::chrono::system_clock::now();
    auto time = std::chrono::system_clock::to_time_t(now);
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(
        now.time_since_epoch()) % 1000;
    
    std::stringstream ss;
    ss << std::put_time(std::localtime(&time), "%H:%M:%S") << '.' 
       << std::setfill('0') << std::setw(3) << ms.count() << " ";
    return ss.str();
}

// Base case for recursion
inline void VPrint(const PrintConfig& config = {}) {
    if (config.newline) {
        std::cout << std::endl;
    }
}

// Main implementation with config
template<typename T, typename... Args>
void VPrint(const PrintConfig& config, const T& first, const Args&... rest) {
    std::stringstream output;
    
    // Apply timestamp if enabled
    if (config.timestamp) {
        output << formatTimestamp();
    }
    
    // Apply source location if enabled
    if (config.sourceLocation) {
        std::source_location location = std::source_location::current();
        output << "[" << location.file_name() << ":" << location.line() << "] ";
    }
    
    // Set color
    if (config.color != Color::DEFAULT) {
        setTextColor(config.color);
    }
    
    // Write the actual content
    output << first;
    std::cout << output.str();
    
    if constexpr (sizeof...(rest) > 0) {
        std::cout << " ";
        VPrint(config, rest...);
    } else {
        if (config.newline) {
            std::cout << std::endl;
        }
        // Reset color
        setTextColor(Color::DEFAULT);
    }
}

// Simplified version for backward compatibility
template<typename T, typename... Args>
void VPrint(const T& first, const Args&... rest) {
    PrintConfig defaultConfig;
    VPrint(defaultConfig, first, rest...);
}

// Colored print shortcuts
template<typename... Args>
void VPrintInfo(const Args&... args) {
    PrintConfig config;
    config.withColor(Color::GREEN).withTimestamp();
    VPrint(config, args...);
}

template<typename... Args>
void VPrintWarning(const Args&... args) {
    PrintConfig config;
    config.withColor(Color::YELLOW).withTimestamp();
    VPrint(config, args...);
}

template<typename... Args>
void VPrintError(const Args&... args) {
    PrintConfig config;
    config.withColor(Color::RED).withTimestamp().withSourceLocation();
    VPrint(config, args...);
}