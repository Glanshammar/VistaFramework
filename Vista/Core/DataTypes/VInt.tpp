#pragma once
#include "VInt.hpp"

template <typename T>
VInt<T>::VInt() : value(0) {}

template <typename T>
VInt<T>::VInt(T val) : value(val) {}

template <typename T>
VInt<T>::operator T() const { return value; }

template <typename T>
VInt<T>& VInt<T>::operator=(T val) {
    value = val;
    return *this;
}

template <typename T>
VInt<T> VInt<T>::operator+(const VInt& other) const { return VInt(value + other.value); }

template <typename T>
VInt<T> VInt<T>::operator-(const VInt& other) const { return VInt(value - other.value); }

template <typename T>
VInt<T> VInt<T>::operator*(const VInt& other) const { return VInt(value * other.value); }

template <typename T>
VInt<T> VInt<T>::operator/(const VInt& other) const { return VInt(value / other.value); }

template <typename T>
VInt<T>& VInt<T>::operator++() { ++value; return *this; }

template <typename T>
VInt<T> VInt<T>::operator++(int) { VInt temp = *this; ++value; return temp; }

template <typename T>
VInt<T>& VInt<T>::operator--() { --value; return *this; }

template <typename T>
VInt<T> VInt<T>::operator--(int) { VInt temp = *this; --value; return temp; }

template <typename T>
bool VInt<T>::operator==(const VInt& other) const { return value == other.value; }

template <typename T>
bool VInt<T>::operator!=(const VInt& other) const { return value != other.value; }

template <typename T>
bool VInt<T>::operator<(const VInt& other) const { return value < other.value; }

template <typename T>
bool VInt<T>::operator>(const VInt& other) const { return value > other.value; }

template <typename T>
bool VInt<T>::operator<=(const VInt& other) const { return value <= other.value; }

template <typename T>
bool VInt<T>::operator>=(const VInt& other) const { return value >= other.value; }

template <typename T>
std::ostream& operator<<(std::ostream& os, const VInt<T>& vint) {
    os << vint.value;
    return os;
}