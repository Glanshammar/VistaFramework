#pragma once
#include <stdexcept>
#include <cstring>
#include <typeinfo>

class VArray {
public:
    VArray();
    ~VArray();

    // Type-safe operations
    template<typename T>
    void append(const T& value) {
        if (typeid(T) != elementType) {
            throw std::runtime_error("Type mismatch in VArray");
        }
        if (_size == _capacity) {
            resize(_capacity * 2);
        }
        T* newElement = new T(value);
        _data[_size++] = newElement;
    }

    template<typename T>
    T& at(int index) const {
        if (typeid(T) != elementType) {
            throw std::runtime_error("Type mismatch in VArray");
        }
        if (index < 0 || index >= _size) {
            throw std::out_of_range("Index out of range");
        }
        return *static_cast<T*>(_data[index]);
    }

    void removeItem(int index);
    void clear();
    bool empty() const;
    void removeFirst();
    void removeLast();

    [[nodiscard]] int size() const;
    [[nodiscard]] const std::type_info& type() const { return elementType; }

    class iterator {
    public:
        iterator(void** ptr, const std::type_info& type);
        iterator& operator++();
        iterator operator++(int);
        bool operator==(const iterator& other) const;
        bool operator!=(const iterator& other) const;
        void* operator*() const;

    private:
        void** _ptr;
        const std::type_info& elementType;
    };

    iterator begin();
    iterator end();

private:
    void resize(int newCapacity);
    void cleanup();

    void** _data;
    int _size;
    int _capacity;
    const std::type_info& elementType;
};