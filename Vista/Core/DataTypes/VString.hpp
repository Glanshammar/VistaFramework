#pragma once

#include <cstring>
#include <stdexcept>
#include <iostream>
#include <string>
#include <codecvt>
#include <locale>
#include <memory>

class VString {
public:
    // Constructors
    VString();
    explicit VString(const char16_t* str);
    VString(const char* str);
    VString(const std::string& str);
    VString(const VString& other);
    VString(VString&& other) noexcept;

    // Destructor
    ~VString();

    // Assignment Operators
    VString& operator=(const VString& other);
    VString& operator=(VString&& other) noexcept;

    // Member functions
    [[nodiscard]] const char16_t* c_str() const;
    [[nodiscard]] size_t size() const;
    [[nodiscard]] bool isEmpty() const;
    [[nodiscard]] std::string toStdString() const;

    void clear();
    void append(const char16_t* str);
    void append(const VString& other);
    void insert(size_t pos, const char16_t* str);
    void insert(size_t pos, const VString& other);
    [[nodiscard]] VString substring(size_t pos, size_t len) const;

    // Overloaded Operators
    friend std::ostream& operator<<(std::ostream& os, const VString& str);
    friend std::wostream& operator<<(std::wostream& os, const VString& str);
    VString operator+(const VString& other) const;
    VString& operator+=(const VString& other);
    bool operator==(const VString& other) const;
    bool operator!=(const VString& other) const;
    char16_t operator[](size_t index) const;
    char16_t& operator[](size_t index);

private:
    char16_t* data;
    size_t length;

    void copyData(const char16_t* str, size_t len);
    void copyData(const char* str);
    void copyData(const std::string& str);
    static size_t utf16Length(const char16_t* str);
};

// Additional helper functions
VString toVString(const char* str);
VString toVString(const std::string& str);