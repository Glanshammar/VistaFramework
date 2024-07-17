#pragma once
#include "VArray.hpp"

template <typename T>
VArray<T>::VArray() : _size(0), _capacity(10) {
    _data = new T[_capacity];
}

template <typename T>
VArray<T>::~VArray() {
    delete[] _data;
}

template <typename T>
void VArray<T>::append(const T& value) {
    if (_size == _capacity) {
        resize(_capacity * 2);
    }
    _data[_size++] = value;
}

template <typename T>
void VArray<T>::removeItem(int index) {
    if (index < 0 || index >= _size) {
        throw std::out_of_range("Index out of range");
    }
    for (int i = index; i < _size - 1; ++i) {
        _data[i] = _data[i + 1];
    }
    --_size;
}

template <typename T>
void VArray<T>::clear() {
    _size = 0;
}

template <typename T>
bool VArray<T>::contains(const T& value) const {
    for (int i = 0; i < _size; ++i) {
        if (_data[i] == value) {
            return true;
        }
    }
    return false;
}

template <typename T>
bool VArray<T>::empty() const {
    return _size == 0;
}

template <typename T>
void VArray<T>::removeFirst() {
    if (_size == 0) {
        throw std::out_of_range("Array is empty");
    }
    removeItem(0);
}

template <typename T>
void VArray<T>::removeLast() {
    if (_size == 0) {
        throw std::out_of_range("Array is empty");
    }
    --_size;
}

template <typename T>
void VArray<T>::replace(int index, const T& newValue) {
    if (index < 0 || index >= _size) {
        throw std::out_of_range("Index out of range");
    }
    _data[index] = newValue;
}

template <typename T>
int VArray<T>::size() const {
    return _size;
}

template <typename T>
T& VArray<T>::operator[](int index) {
    if (index < 0 || index >= _size) {
        throw std::out_of_range("Index out of range");
    }
    return _data[index];
}

template <typename T>
const T& VArray<T>::operator[](int index) const {
    if (index < 0 || index >= _size) {
        throw std::out_of_range("Index out of range");
    }
    return _data[index];
}

template <typename T>
typename VArray<T>::iterator VArray<T>::begin() {
    return iterator(_data);
}

template <typename T>
typename VArray<T>::iterator VArray<T>::end() {
    return iterator(_data + _size);
}

template <typename T>
void VArray<T>::resize(int newCapacity) {
    T* newData = new T[newCapacity];
    for (int i = 0; i < _size; ++i) {
        newData[i] = _data[i];
    }
    delete[] _data;
    _data = newData;
    _capacity = newCapacity;
}

template <typename T>
VArray<T>::iterator::iterator(T* ptr) : _ptr(ptr) {}

template <typename T>
T& VArray<T>::iterator::operator*() const {
    return *_ptr;
}

template <typename T>
T* VArray<T>::iterator::operator->() {
    return _ptr;
}

template <typename T>
typename VArray<T>::iterator& VArray<T>::iterator::operator++() {
    _ptr++;
    return *this;
}

template <typename T>
typename VArray<T>::iterator VArray<T>::iterator::operator++(int) {
    iterator temp = *this;
    ++(*this);
    return temp;
}

template <typename T>
bool VArray<T>::iterator::operator==(const iterator& other) const {
    return _ptr == other._ptr;
}

template <typename T>
bool VArray<T>::iterator::operator!=(const iterator& other) const {
    return _ptr != other._ptr;
}