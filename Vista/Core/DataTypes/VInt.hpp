#pragma once

#include <cstdint>
#include <iostream>
#include <type_traits>

template <typename T>
class VInt : public VObject{
    static_assert(std::is_integral<T>::value, "VInt must use an integral type");

public:
    // Constructors
    VInt();
    explicit VInt(T val);

    // Conversion operator
    explicit operator T() const;

    // Assignment operator
    VInt& operator=(T val);

    // Arithmetic operators
    VInt operator+(const VInt& other) const;
    VInt operator-(const VInt& other) const;
    VInt operator*(const VInt& other) const;
    VInt operator/(const VInt& other) const;

    // Increment and decrement operators
    VInt& operator++();
    VInt operator++(int);
    VInt& operator--();
    VInt operator--(int);

    // Comparison operators
    bool operator==(const VInt& other) const;
    bool operator!=(const VInt& other) const;
    bool operator<(const VInt& other) const;
    bool operator>(const VInt& other) const;
    bool operator<=(const VInt& other) const;
    bool operator>=(const VInt& other) const;

    // Stream insertion operator
    friend std::ostream& operator<<(std::ostream& os, const VInt& vint);

private:
    T value;
};

// Type aliases for different bit sizes
using vint8 = VInt<int8_t>;
using vint16 = VInt<int16_t>;
using vint32 = VInt<int32_t>;
using vint64 = VInt<int64_t>;

// Unsigned versions
using vint8u = VInt<uint8_t>;
using vint16u = VInt<uint16_t>;
using vint32u = VInt<uint32_t>;
using vint64u = VInt<uint64_t>;

#include "VInt.tpp"