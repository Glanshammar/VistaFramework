#pragma once
#include <stdexcept>

template <typename T>
class VArray {
public:
    VArray();
    ~VArray();

    void append(const T& value);
    void removeItem(int index);
    void clear();
    bool contains(const T& value) const;
    bool empty() const;
    void removeFirst();
    void removeLast();
    void replace(int index, const T& newValue);

    [[nodiscard]] int size() const;

    T& operator[](int index);
    const T& operator[](int index) const;

    class iterator {
    public:
        iterator(T* ptr);

        T& operator*() const;
        T* operator->();

        iterator& operator++();
        iterator operator++(int);

        bool operator==(const iterator& other) const;
        bool operator!=(const iterator& other) const;

    private:
        T* _ptr;
    };

    iterator begin();
    iterator end();

private:
    void resize(int newCapacity);

    T* _data;
    int _size;
    int _capacity;
};

#include "VArray.tpp"