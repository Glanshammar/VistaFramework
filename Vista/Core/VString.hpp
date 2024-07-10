#pragma once

#include <VObject>
#include <cstring>

class VString {
public:
    // Constructors
    VString();
    VString(const char *str);
    VString(const VString &other);

    // Destructor
    ~VString();

    // Assignment operator
    VString& operator=(const VString &other);

    // Conversion to const char *
    operator const char *() const { return m_data; }

    // Concatenation operator
    VString operator+(const VString &other) const;

    // Append function
    void append(const char *str);

    // Accessors
    const char *data() const { return m_data; }

    // Utility function to get length
    int length() const { return m_length; }

private:
    char *m_data; // Pointer to the actual string data
    int m_length; // Length of the string
};
