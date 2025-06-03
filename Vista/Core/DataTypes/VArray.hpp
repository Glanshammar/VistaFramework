#pragma once
#include <stdexcept>
#include <vector>
#include <algorithm>

template<typename T>
class VArray {
public:
    VArray();
    ~VArray();

    void append(const T& value);
    T& at(int index);
    const T& at(int index) const;
    T& operator[](int index);
    const T& operator[](int index) const;
    bool contains(const T& value) const;
    void removeItem(int index);
    void clear();
    bool empty() const;
    void removeFirst();
    void removeLast();
    [[nodiscard]] int size() const;

    using iterator = typename std::vector<T>::iterator;
    using const_iterator = typename std::vector<T>::const_iterator;

    iterator begin();
    iterator end();
    const_iterator begin() const;
    const_iterator end() const;

private:
    std::vector<T> _data;
};

#include "VArray.tpp" 