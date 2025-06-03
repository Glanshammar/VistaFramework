#include <VArray>
#include <VString>
#include <string>

// Specialized implementation for VArray<VString>::join
template<>
VString VArray<VString>::join(const VString& separator) const {
    if (isEmpty()) {
        return VString();
    }
    
    // Create a new VString for the result - copying the first element
    VString result((*this)[0]);
    
    // Add the remaining elements with the separator
    for (size_t i = 1; i < size(); ++i) {
        // Append separator and the next element
        result += separator;
        result += (*this)[i];  // This will use the VString::operator+= which handles memory correctly
    }
    
    return result;
}

// Specialized implementation for VArray<int>::join
template<>
VString VArray<int>::join(const VString& separator) const {
    if (isEmpty()) {
        return VString();
    }
    
    // Create result string with the first element
    VString result(std::to_string((*this)[0]).c_str());
    
    // Add the remaining elements with the separator
    for (size_t i = 1; i < size(); ++i) {
        result += separator;
        result += VString(std::to_string((*this)[i]).c_str());
    }
    
    return result;
} 