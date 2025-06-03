#include "VArray.hpp"

VArray::VArray() : _size(0), _capacity(10), elementType(typeid(void)) {
    _data = new void*[_capacity];
    std::memset(_data, 0, _capacity * sizeof(void*));
}

VArray::VArray(const std::type_info& type) : _size(0), _capacity(10), elementType(type) {
    _data = new void*[_capacity];
    std::memset(_data, 0, _capacity * sizeof(void*));
}

VArray::~VArray() {
    cleanup();
}

void VArray::removeItem(int index) {
    if (index < 0 || index >= _size) {
        throw std::out_of_range("Index out of range");
    }
    delete static_cast<VAny*>(_data[index]);
    for (int i = index; i < _size - 1; ++i) {
        _data[i] = _data[i + 1];
    }
    --_size;
}

void VArray::clear() {
    cleanup();
    _size = 0;
    _capacity = 10;
    _data = new void*[_capacity];
    std::memset(_data, 0, _capacity * sizeof(void*));
}

bool VArray::empty() const {
    return _size == 0;
}

void VArray::removeFirst() {
    if (_size == 0) {
        throw std::out_of_range("Array is empty");
    }
    removeItem(0);
}

void VArray::removeLast() {
    if (_size == 0) {
        throw std::out_of_range("Array is empty");
    }
    delete static_cast<VAny*>(_data[--_size]);
}

int VArray::size() const {
    return _size;
}

VArray::iterator VArray::begin() {
    return iterator(_data, elementType);
}

VArray::iterator VArray::end() {
    return iterator(_data + _size, elementType);
}

void VArray::resize(int newCapacity) {
    void** newData = new void*[newCapacity];
    std::memset(newData, 0, newCapacity * sizeof(void*));
    for (int i = 0; i < _size; ++i) {
        newData[i] = _data[i];
    }
    delete[] _data;
    _data = newData;
    _capacity = newCapacity;
}

void VArray::cleanup() {
    for (int i = 0; i < _size; ++i) {
        delete static_cast<VAny*>(_data[i]);
    }
    delete[] _data;
    _data = nullptr;
}

// Iterator implementation
VArray::iterator::iterator(void** ptr, const std::type_info& type) 
    : _ptr(ptr), elementType(type) {}

VArray::iterator& VArray::iterator::operator++() {
    ++_ptr;
    return *this;
}

VArray::iterator VArray::iterator::operator++(int) {
    iterator temp = *this;
    ++(*this);
    return temp;
}

bool VArray::iterator::operator==(const iterator& other) const {
    return _ptr == other._ptr;
}

bool VArray::iterator::operator!=(const iterator& other) const {
    return _ptr != other._ptr;
}

VAny VArray::iterator::operator*() const {
    return *static_cast<VAny*>(*_ptr);
} 