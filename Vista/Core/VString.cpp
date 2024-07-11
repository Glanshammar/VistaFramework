#include "VString.hpp"

// Default Constructor
VString::VString() : data(nullptr), length(0) {}

// Constructor with UTF-16 string
VString::VString(const char16_t* str) : length(utf16Length(str)) {
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

// Copy Data Helper Function for UTF-16 string
void VString::copyData(const char16_t* str, size_t len) {
    data = new char16_t[len + 1];
    std::memcpy(data, str, len * sizeof(char16_t));
    data[len] = u'\0';
}

// Copy Data Helper Function for C-string (const char*)
void VString::copyData(const char* str) {
    size_t len = std::strlen(str);
    data = new char16_t[len + 1];
    for (size_t i = 0; i < len; ++i) {
        data[i] = static_cast<char16_t>(str[i]);
    }
    data[len] = u'\0';
}

// Copy Data Helper Function for std::string
void VString::copyData(const std::string& str) {
    size_t len = str.length();
    data = new char16_t[len + 1];
    for (size_t i = 0; i < len; ++i) {
        data[i] = static_cast<char16_t>(str[i]);
    }
    data[len] = u'\0';
}

// Return UTF-16 string
const char16_t* VString::c_str() const {
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

// Append UTF-16 string
void VString::append(const char16_t* str) {
    size_t newLength = length + utf16Length(str);
    auto* newData = new char16_t[newLength + 1];
    std::memcpy(newData, data, length * sizeof(char16_t));
    std::memcpy(newData + length, str, (utf16Length(str) + 1) * sizeof(char16_t));
    delete[] data;
    data = newData;
    length = newLength;
}

// Append another VString
void VString::append(const VString& other) {
    append(other.data);
}

// Insert UTF-16 string at position
void VString::insert(size_t pos, const char16_t* str) {
    if (pos > length) {
        throw std::out_of_range("Position out of range");
    }
    size_t insertLen = utf16Length(str);
    size_t newLength = length + insertLen;
    auto* newData = new char16_t[newLength + 1];
    std::memcpy(newData, data, pos * sizeof(char16_t));
    std::memcpy(newData + pos, str, insertLen * sizeof(char16_t));
    std::memcpy(newData + pos + insertLen, data + pos, (length - pos + 1) * sizeof(char16_t));
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
    auto* substr = new char16_t[actualLen + 1];
    std::memcpy(substr, data + pos, actualLen * sizeof(char16_t));
    substr[actualLen] = u'\0';
    VString result(substr);
    delete[] substr;
    return result;
}

// Output Stream Operator
std::ostream& operator<<(std::ostream& os, const VString& str) {
    std::u16string u16str(str.data);
    std::string u8str(u16str.begin(), u16str.end());
    os << u8str;
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
    return std::u16string(data) == std::u16string(other.data);
}

// Inequality Check
bool VString::operator!=(const VString& other) const {
    return !(*this == other);
}

// Indexing Operators
char16_t VString::operator[](size_t index) const {
    if (index >= length) {
        throw std::out_of_range("Index out of range");
    }
    return data[index];
}

char16_t& VString::operator[](size_t index) {
    if (index >= length) {
        throw std::out_of_range("Index out of range");
    }
    return data[index];
}

// Helper function to calculate the length of a UTF-16 string
size_t VString::utf16Length(const char16_t* str) {
    size_t len = 0;
    while (str[len] != u'\0') {
        ++len;
    }
    return len;
}

// Conversion function to VString for C-string (const char*)
VString toVString(const char* str) {
    return {str};
}

// Conversion function to VString for std::string
VString toVString(const std::string& str) {
    return {str};
}

// Convert VString to std::string (UTF-8)
std::string VString::toStdString() const {
    std::u16string u16str(data, length);
    std::wstring_convert<std::codecvt_utf8_utf16<char16_t>, char16_t> convert;
    return convert.to_bytes(u16str);
}