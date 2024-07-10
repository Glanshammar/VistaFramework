#include "VString.hpp"

VString::VString() {
    m_data = new char[1];
    m_data[0] = '\0';
    m_length = 0;
}

VString::VString(const char *str) {
    m_length = std::strlen(str);
    m_data = new char[m_length + 1];
    std::strncpy(m_data, str, m_length + 1); // Ensure null termination
}

VString::VString(const VString &other) {
    m_length = other.m_length;
    m_data = new char[m_length + 1];
    std::strncpy(m_data, other.m_data, m_length + 1); // Ensure null termination
}

// Destructor
VString::~VString() {
    delete[] m_data;
}

// Assignment operator
VString& VString::operator=(const VString &other) {
    if (this != &other) {
        delete[] m_data;
        m_length = other.m_length;
        m_data = new char[m_length + 1];
        std::strncpy(m_data, other.m_data, m_length + 1); // Ensure null termination
    }
    return *this;
}

// Concatenation operator
VString VString::operator+(const VString &other) const {
    VString result;
    result.m_length = m_length + other.m_length;
    delete[] result.m_data;
    result.m_data = new char[result.m_length + 1];
    std::strncpy(result.m_data, m_data, m_length + 1); // Copy first part
    std::strncpy(result.m_data + m_length, other.m_data, other.m_length + 1); // Copy second part and null termination
    return result;
}

// Append function
void VString::append(const char *str) {
    int newLength = m_length + std::strlen(str);
    char *newData = new char[newLength + 1];
    std::strncpy(newData, m_data, m_length + 1); // Copy existing data
    std::strncpy(newData + m_length, str, std::strlen(str) + 1); // Append new data and null termination
    delete[] m_data;
    m_data = newData;
    m_length = newLength;
}