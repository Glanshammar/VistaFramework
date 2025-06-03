#include "VString.hpp"

// Default Constructor
VString::VString() : data(nullptr), length(0) {
    // Initialize with an empty string (just a null terminator)
    data = new char32_t[1];
    data[0] = U'\0';
}

// Constructor with UTF-32 string
VString::VString(const char32_t* str) : length(utf32Length(str)) {
    copyData(str, length);
}

// Constructor with C-string (const char*)
VString::VString(const char* str) {
    copyData(str);
}

// Constructor with std::string
VString::VString(const std::string& str) {
    copyData(str);
}

// Copy Constructor
VString::VString(const VString& other) : length(other.length) {
    copyData(other.data, length);
}

// Move Constructor
VString::VString(VString&& other) noexcept : data(other.data), length(other.length) {
    other.data = nullptr;
    other.length = 0;
}

// Destructor
VString::~VString() {
    delete[] data;
}

// Copy Assignment Operator
VString& VString::operator=(const VString& other) {
    if (this != &other) {
        delete[] data;
        length = other.length;
        copyData(other.data, length);
    }
    return *this;
}

// Move Assignment Operator
VString& VString::operator=(VString&& other) noexcept {
    if (this != &other) {
        delete[] data;
        data = other.data;
        length = other.length;
        other.data = nullptr;
        other.length = 0;
    }
    return *this;
}

// Copy Data Helper Function for UTF-32 string
void VString::copyData(const char32_t* str, size_t len) {
    data = new char32_t[len + 1];
    std::memcpy(data, str, len * sizeof(char32_t));
    data[len] = U'\0';
}

// Copy Data Helper Function for C-string (const char*)
void VString::copyData(const char* str) {
    std::wstring_convert<std::codecvt_utf8<char32_t>, char32_t> converter;
    std::u32string u32str = converter.from_bytes(str);
    length = u32str.length();
    data = new char32_t[length + 1];
    std::memcpy(data, u32str.c_str(), (length + 1) * sizeof(char32_t));
}

// Copy Data Helper Function for std::string
void VString::copyData(const std::string& str) {
    copyData(str.c_str());
}

// Return UTF-32 string
const char32_t* VString::c_str() const {
    return data;
}

// Return size of the string
size_t VString::size() const {
    return length;
}

// Check if the string is empty
bool VString::isEmpty() const {
    return length == 0;
}

// Clear the string
void VString::clear() {
    delete[] data;
    data = nullptr;
    length = 0;
}

// Append UTF-32 string
void VString::append(const char32_t* str) {
    size_t newLength = length + utf32Length(str);
    auto* newData = new char32_t[newLength + 1];
    std::memcpy(newData, data, length * sizeof(char32_t));
    std::memcpy(newData + length, str, (utf32Length(str) + 1) * sizeof(char32_t));
    delete[] data;
    data = newData;
    length = newLength;
}

// Append another VString
void VString::append(const VString& other) {
    append(other.data);
}

// Insert UTF-32 string at position
void VString::insert(size_t pos, const char32_t* str) {
    if (pos > length) {
        throw std::out_of_range("Position out of range");
    }
    size_t insertLen = utf32Length(str);
    size_t newLength = length + insertLen;
    auto* newData = new char32_t[newLength + 1];
    std::memcpy(newData, data, pos * sizeof(char32_t));
    std::memcpy(newData + pos, str, insertLen * sizeof(char32_t));
    std::memcpy(newData + pos + insertLen, data + pos, (length - pos + 1) * sizeof(char32_t));
    delete[] data;
    data = newData;
    length = newLength;
}

// Insert another VString at position
void VString::insert(size_t pos, const VString& other) {
    insert(pos, other.data);
}

// Get substring
VString VString::substring(size_t pos, size_t len) const {
    if (pos > length) {
        throw std::out_of_range("Position out of range");
    }
    size_t actualLen = (len < length - pos) ? len : (length - pos);
    auto* substr = new char32_t[actualLen + 1];
    std::memcpy(substr, data + pos, actualLen * sizeof(char32_t));
    substr[actualLen] = U'\0';
    VString result(substr);
    delete[] substr;
    return result;
}

// Convert to std::string (UTF-8)
std::string VString::toStdString() const {
    std::wstring_convert<std::codecvt_utf8<char32_t>, char32_t> converter;
    return converter.to_bytes(data, data + length);
}

// Convert to std::u32string
std::u32string VString::toU32String() const {
    return std::u32string(data, length);
}

// Convert to std::u16string
std::u16string VString::toU16String() const {
    std::wstring_convert<std::codecvt_utf16<char32_t>, char32_t> converter;
    std::string bytes = converter.to_bytes(data, data + length);
    return std::u16string(reinterpret_cast<const char16_t*>(bytes.c_str()), bytes.length() / 2);
}

// Check if character at index is an emoji
bool VString::isEmoji(size_t index) const {
    if (index >= length) return false;
    return isEmojiChar(data[index]);
}

// Count emojis in the string
size_t VString::emojiCount() const {
    size_t count = 0;
    for (size_t i = 0; i < length; ++i) {
        if (isEmojiChar(data[i])) ++count;
    }
    return count;
}

// Get positions of all emojis
std::vector<size_t> VString::emojiPositions() const {
    std::vector<size_t> positions;
    for (size_t i = 0; i < length; ++i) {
        if (isEmojiChar(data[i])) positions.push_back(i);
    }
    return positions;
}

// Extract all emojis into a new string
VString VString::extractEmojis() const {
    std::vector<char32_t> emojis;
    for (size_t i = 0; i < length; ++i) {
        if (isEmojiChar(data[i])) emojis.push_back(data[i]);
    }
    if (emojis.empty()) return VString();
    
    auto* result = new char32_t[emojis.size() + 1];
    std::memcpy(result, emojis.data(), emojis.size() * sizeof(char32_t));
    result[emojis.size()] = U'\0';
    VString str(result);
    delete[] result;
    return str;
}

// Helper function to check if a character is an emoji
bool VString::isEmojiChar(char32_t c) {
    // Basic emoji ranges
    return (c >= 0x1F300 && c <= 0x1F9FF) || // Miscellaneous Symbols and Pictographs
           (c >= 0x2600 && c <= 0x26FF) ||   // Miscellaneous Symbols
           (c >= 0x2700 && c <= 0x27BF) ||   // Dingbats
           (c >= 0x1F000 && c <= 0x1F02F) || // Mahjong Tiles
           (c >= 0x1F0A0 && c <= 0x1F0FF) || // Playing Cards
           (c >= 0x1F100 && c <= 0x1F1FF) || // Enclosed Alphanumeric Supplement
           (c >= 0x1F200 && c <= 0x1F2FF) || // Enclosed Ideographic Supplement
           (c >= 0x1F600 && c <= 0x1F64F) || // Emoticons
           (c >= 0x1F680 && c <= 0x1F6FF) || // Transport and Map Symbols
           (c >= 0x1F900 && c <= 0x1F9FF);   // Supplemental Symbols and Pictographs
}

// Helper function to calculate the length of a UTF-32 string
size_t VString::utf32Length(const char32_t* str) {
    size_t len = 0;
    while (str[len] != U'\0') {
        ++len;
    }
    return len;
}

// Output Stream Operator for std::ostream
std::ostream& operator<<(std::ostream& os, const VString& str) {
    os << str.toStdString();
    return os;
}

// Output Stream Operator for std::wostream
std::wostream& operator<<(std::wostream& os, const VString& str) {
    std::wstring_convert<std::codecvt_utf16<wchar_t>, wchar_t> converter;
    std::string bytes = str.toStdString();
    os << converter.from_bytes(bytes);
    return os;
}

// Concatenate Strings
VString VString::operator+(const VString& other) const {
    VString result(*this);
    result.append(other);
    return result;
}

// Append and Assign
VString& VString::operator+=(const VString& other) {
    append(other);
    return *this;
}

// Equality Check
bool VString::operator==(const VString& other) const {
    return std::u32string(data) == std::u32string(other.data);
}

// Inequality Check
bool VString::operator!=(const VString& other) const {
    return !(*this == other);
}

// Indexing Operators
char32_t VString::operator[](size_t index) const {
    if (index >= length) {
        throw std::out_of_range("Index out of range");
    }
    return data[index];
}

char32_t& VString::operator[](size_t index) {
    if (index >= length) {
        throw std::out_of_range("Index out of range");
    }
    return data[index];
}

// Conversion functions
VString toVString(const char* str) {
    return VString(str);
}

VString toVString(const std::string& str) {
    return VString(str);
}

VString toVString(const std::u32string& str) {
    return VString(str.c_str());
}