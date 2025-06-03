#pragma once
#include <stdexcept>
#include <cstring>
#include <typeinfo>
#include <VAny>

class VArray {
public:
    VArray();
    explicit VArray(const std::type_info& type);
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
        _data[_size++] = new VAny(value);
    }

    template<typename T>
    T& at(int index) const {
        if (typeid(T) != elementType) {
            throw std::runtime_error("Type mismatch in VArray");
        }
        if (index < 0 || index >= _size) {
            throw std::out_of_range("Index out of range");
        }
        return static_cast<VAny*>(_data[index])->cast<T>();
    }

    template<typename T>
    void replace(int index, const T& value) {
        if (typeid(T) != elementType) {
            throw std::runtime_error("Type mismatch in VArray");
        }
        if (index < 0 || index >= _size) {
            throw std::out_of_range("Index out of range");
        }
        delete static_cast<VAny*>(_data[index]);
        _data[index] = new VAny(value);
    }

    template<typename T>
    bool contains(const T& value) const {
        if (typeid(T) != elementType) {
            throw std::runtime_error("Type mismatch in VArray");
        }
        for (int i = 0; i < _size; ++i) {
            if (static_cast<VAny*>(_data[i])->cast<T>() == value) {
                return true;
            }
        }
        return false;
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
        VAny operator*() const;

    private:
        void** _ptr;
        const std::type_info& elementType;
    };

    iterator begin();
    iterator end();

    const std::type_info& elementType;

private:
    void resize(int newCapacity);
    void cleanup();

    void** _data;
    int _size;
    int _capacity;
};