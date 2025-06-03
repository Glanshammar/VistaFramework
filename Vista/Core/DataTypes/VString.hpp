#pragma once

#include <cstring>
#include <stdexcept>
#include <iostream>
#include <string>
#include <codecvt>
#include <locale>
#include <memory>
#include <vector>

class VString {
public:
    // Constructors
    VString();
    explicit VString(const char32_t* str);
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
    [[nodiscard]] const char32_t* c_str() const;
    [[nodiscard]] size_t size() const;
    [[nodiscard]] bool isEmpty() const;
    [[nodiscard]] std::string toStdString() const;
    [[nodiscard]] std::u32string toU32String() const;
    [[nodiscard]] std::u16string toU16String() const;

    void clear();
    void append(const char32_t* str);
    void append(const VString& other);
    void insert(size_t pos, const char32_t* str);
    void insert(size_t pos, const VString& other);
    [[nodiscard]] VString substring(size_t pos, size_t len) const;

    // Emoji and Unicode support
    [[nodiscard]] bool isEmoji(size_t index) const;
    [[nodiscard]] size_t emojiCount() const;
    [[nodiscard]] std::vector<size_t> emojiPositions() const;
    [[nodiscard]] VString extractEmojis() const;

    // Overloaded Operators
    friend std::ostream& operator<<(std::ostream& os, const VString& str);
    friend std::wostream& operator<<(std::wostream& os, const VString& str);
    VString operator+(const VString& other) const;
    VString& operator+=(const VString& other);
    bool operator==(const VString& other) const;
    bool operator!=(const VString& other) const;
    char32_t operator[](size_t index) const;
    char32_t& operator[](size_t index);

private:
    char32_t* data;
    size_t length;

    void copyData(const char32_t* str, size_t len);
    void copyData(const char* str);
    void copyData(const std::string& str);
    static size_t utf32Length(const char32_t* str);
    static bool isEmojiChar(char32_t c);
};

// Additional helper functions
VString toVString(const char* str);
VString toVString(const std::string& str);
VString toVString(const std::u32string& str);