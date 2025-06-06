#pragma once

#include <type_traits>
#include <sstream>
#include <iostream>
#include <codecvt>
#include <locale>
#include <iomanip>
#include <string>
#include <string_view>
#include <source_location>
#include <chrono>
#include <VAny>

// Type trait to detect stream manipulators
template<typename T>
struct is_stream_manipulator : std::false_type {};

template<>
struct is_stream_manipulator<std::ostream& (*)(std::ostream&)> : std::true_type {};

template<typename T>
inline constexpr bool is_stream_manipulator_v = is_stream_manipulator<T>::value;

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
    bool newline = false;
    
    PrintConfig& withTimestamp(bool value = true) { timestamp = value; return *this; }
    PrintConfig& withSourceLocation(bool value = true) { sourceLocation = value; return *this; }
    PrintConfig& withColor(Color value) { color = value; return *this; }
    PrintConfig& withBold(bool value = true) { bold = value; return *this; }
    PrintConfig& withNewline(bool value = true) { newline = value; return *this; }
    PrintConfig withoutNewline() const { return PrintConfig().withTimestamp(timestamp).withSourceLocation(sourceLocation).withColor(color).withBold(bold); }
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
    // Always reset color at the end
    setTextColor(Color::DEFAULT);
}

// Main implementation with config
template<typename T, typename... Args>
void VPrint(const PrintConfig& config, const T& first, const Args&... rest) {
    // Only apply formatting for non-manipulators
    if constexpr (!is_stream_manipulator_v<T>) {
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
    } else {
        // Apply manipulator directly
        first(std::cout);
    }
    
    if constexpr (sizeof...(rest) > 0) {
        // Check if we need to add a space between arguments
        if constexpr (!is_stream_manipulator_v<T> && !is_stream_manipulator_v<std::decay_t<std::tuple_element_t<0, std::tuple<Args...>>>>) {
            std::cout << " ";
        }
        VPrint(config, rest...);
    } else {
        if (config.newline) {
            std::cout << std::endl;
        }
        // Reset color
        setTextColor(Color::DEFAULT);
    }
}

// Specialization for void* (used by VArray iterator)
template<typename... Args>
void VPrint(const PrintConfig& config, void* ptr, const Args&... rest) {
    std::stringstream output;
    
    if (config.timestamp) {
        output << formatTimestamp();
    }
    
    if (config.sourceLocation) {
        std::source_location location = std::source_location::current();
        output << "[" << location.file_name() << ":" << location.line() << "] ";
    }
    
    if (config.color != Color::DEFAULT) {
        setTextColor(config.color);
    }
    
    output << *static_cast<int*>(ptr);
    std::cout << output.str();
    
    if constexpr (sizeof...(rest) > 0) {
        if constexpr (!is_stream_manipulator_v<std::decay_t<std::tuple_element_t<0, std::tuple<Args...>>>>) {
            std::cout << " ";
        }
        VPrint(config, rest...);
    } else {
        if (config.newline) {
            std::cout << std::endl;
        }
        setTextColor(Color::DEFAULT);
    }
}

// Specialization for VAny
template<typename... Args>
void VPrint(const PrintConfig& config, const VAny& any, const Args&... rest) {
    std::stringstream output;
    
    if (config.timestamp) {
        output << formatTimestamp();
    }
    
    if (config.sourceLocation) {
        std::source_location location = std::source_location::current();
        output << "[" << location.file_name() << ":" << location.line() << "] ";
    }
    
    if (config.color != Color::DEFAULT) {
        setTextColor(config.color);
    }
    
    output << any;
    std::cout << output.str();
    
    if constexpr (sizeof...(rest) > 0) {
        if constexpr (!is_stream_manipulator_v<std::decay_t<std::tuple_element_t<0, std::tuple<Args...>>>>) {
            std::cout << " ";
        }
        VPrint(config, rest...);
    } else {
        if (config.newline) {
            std::cout << std::endl;
        }
        setTextColor(Color::DEFAULT);
    }
}

// Simplified version for backward compatibility
template<typename T, typename... Args>
void VPrint(const T& first, const Args&... rest) {
    PrintConfig defaultConfig;
    VPrint(defaultConfig, first, rest...);
}

// Explicit overloads for manipulators at the end

// Explicit overloads for common cases with manipulators
inline void VPrint(const char* str, std::ostream& (*manip)(std::ostream&)) {
    VPrint(PrintConfig().withNewline(false), str);
    manip(std::cout);
}

inline void VPrint(const std::string& str, std::ostream& (*manip)(std::ostream&)) {
    VPrint(PrintConfig().withNewline(false), str);
    manip(std::cout);
}

inline void VPrint(const char* first, const char* second, std::ostream& (*manip)(std::ostream&)) {
    PrintConfig config;
    config.withNewline(false);
    VPrint(config, first);
    std::cout << " ";
    VPrint(config, second);
    manip(std::cout);
}

inline void VPrint(const std::string& first, const std::string& second, std::ostream& (*manip)(std::ostream&)) {
    PrintConfig config;
    config.withNewline(false);
    VPrint(config, first);
    std::cout << " ";
    VPrint(config, second);
    manip(std::cout);
}

inline void VPrint(const char* first, const char* second, const char* third, std::ostream& (*manip)(std::ostream&)) {
    PrintConfig config;
    config.withNewline(false);
    VPrint(config, first);
    std::cout << " ";
    VPrint(config, second);
    std::cout << " ";
    VPrint(config, third);
    manip(std::cout);
}

inline void VPrint(const char* first, const std::string& second, std::ostream& (*manip)(std::ostream&)) {
    PrintConfig config;
    config.withNewline(false);
    VPrint(config, first);
    std::cout << " ";
    VPrint(config, second);
    manip(std::cout);
}

inline void VPrint(const std::string& first, const char* second, std::ostream& (*manip)(std::ostream&)) {
    PrintConfig config;
    config.withNewline(false);
    VPrint(config, first);
    std::cout << " ";
    VPrint(config, second);
    manip(std::cout);
}

// Overloads for mixed manipulator usage
inline void VPrint(const char* first, std::ostream& (*manip)(std::ostream&), const char* second) {
    PrintConfig config;
    config.withNewline(false);
    VPrint(config, first);
    manip(std::cout);
    VPrint(config, second);
}

inline void VPrint(const std::string& first, std::ostream& (*manip)(std::ostream&), const std::string& second) {
    PrintConfig config;
    config.withNewline(false);
    VPrint(config, first);
    manip(std::cout);
    VPrint(config, second);
}

inline void VPrint(const char* first, std::ostream& (*manip)(std::ostream&), const std::string& second) {
    PrintConfig config;
    config.withNewline(false);
    VPrint(config, first);
    manip(std::cout);
    VPrint(config, second);
}

inline void VPrint(const std::string& first, std::ostream& (*manip)(std::ostream&), const char* second) {
    PrintConfig config;
    config.withNewline(false);
    VPrint(config, first);
    manip(std::cout);
    VPrint(config, second);
}

// Complex case for test
inline void VPrint(const char* first, const char* second, std::ostream& (*manip)(std::ostream&), const char* third) {
    PrintConfig config;
    config.withNewline(false);
    VPrint(config, first);
    std::cout << " ";
    VPrint(config, second);
    manip(std::cout);
    VPrint(config, third);
}

// VString overload with manipulators
inline void VPrint(const VString& str, std::ostream& (*manip)(std::ostream&)) {
    PrintConfig config;
    config.withNewline(false);
    VPrint(config, str);
    manip(std::cout);
}

// Overload for multiple arguments ending with manipulator
template<typename... Args>
inline void VPrint(const Args&... args, std::ostream& (*manip)(std::ostream&)) {
    PrintConfig config;
    config.withNewline(false);
    VPrint(config, args...);
    manip(std::cout);
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

// Overload for just manipulator
inline void VPrint(std::ostream& (*manip)(std::ostream&)) {
    manip(std::cout);
}