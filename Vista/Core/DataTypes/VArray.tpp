#pragma once
#include <stdexcept>
#include <vector>

template<typename T>
class VArray {
public:
    VArray() = default;
    ~VArray() = default;

    void append(const T& value) {
        _data.push_back(value);
    }

    T& at(int index) {
        if (index < 0 || index >= static_cast<int>(_data.size())) {
            throw std::out_of_range("Index out of range");
        }
        return _data[index];
    }

    const T& at(int index) const {
        if (index < 0 || index >= static_cast<int>(_data.size())) {
            throw std::out_of_range("Index out of range");
        }
        return _data[index];
    }

    T& operator[](int index) {
        return at(index);
    }

    const T& operator[](int index) const {
        return at(index);
    }

    bool contains(const T& value) const {
        return std::find(_data.begin(), _data.end(), value) != _data.end();
    }

    void removeItem(int index) {
        if (index < 0 || index >= static_cast<int>(_data.size())) {
            throw std::out_of_range("Index out of range");
        }
        _data.erase(_data.begin() + index);
    }

    void clear() {
        _data.clear();
    }

    bool empty() const {
        return _data.empty();
    }

    void removeFirst() {
        if (!empty()) {
            removeItem(0);
        }
    }

    void removeLast() {
        if (!empty()) {
            removeItem(_data.size() - 1);
        }
    }

    [[nodiscard]] int size() const {
        return static_cast<int>(_data.size());
    }

    using iterator = typename std::vector<T>::iterator;
    using const_iterator = typename std::vector<T>::const_iterator;

    iterator begin() {
        return _data.begin();
    }

    iterator end() {
        return _data.end();
    }

    const_iterator begin() const {
        return _data.begin();
    }

    const_iterator end() const {
        return _data.end();
    }

private:
    std::vector<T> _data;
};