#pragma once
#include "VAny.hpp"
#include <stdexcept>
#include <typeinfo>
#include <sstream>
#include <VString>

inline VAny::VAny() = default;

template<typename T>
VAny::VAny(const T& value) : _value(std::make_unique<Model<T>>(value)) {}

inline VAny::VAny(const VAny& other) : _value(other._value ? other._value->clone() : nullptr) {}

inline VAny::VAny(VAny&& other) noexcept : _value(std::move(other._value)) {}

inline VAny::~VAny() = default;

inline VAny& VAny::operator=(const VAny& other) {
    VAny temp(other);
    swap(temp);
    return *this;
}

inline VAny& VAny::operator=(VAny&& other) noexcept {
    _value = std::move(other._value);
    return *this;
}

template<typename T>
VAny& VAny::operator=(const T& value) {
    _value = std::make_unique<Model<T>>(value);
    return *this;
}

inline bool VAny::isEmpty() const {
    return _value == nullptr;
}

inline std::type_index VAny::type() const {
    if (isEmpty()) {
        return std::type_index(typeid(void));
    }
    return _value->type();
}

inline std::string VAny::typeName() const {
    if (isEmpty()) {
        return "empty";
    }
    return _value->type().name();
}

template<typename T>
bool VAny::isType() const {
    if (isEmpty()) {
        return false;
    }
    return _value->type() == std::type_index(typeid(T));
}

template<typename T>
T& VAny::cast() {
    if (!isType<T>()) {
        throw std::bad_cast();
    }
    return static_cast<Model<T>*>(_value.get())->_value;
}

template<typename T>
const T& VAny::cast() const {
    if (!isType<T>()) {
        throw std::bad_cast();
    }
    return static_cast<const Model<T>*>(_value.get())->_value;
}

template<typename T>
T VAny::castOr(const T& defaultValue) const {
    if (isType<T>()) {
        return cast<T>();
    }
    return defaultValue;
}

inline void VAny::reset() {
    _value.reset();
}

inline void VAny::swap(VAny& other) noexcept {
    _value.swap(other._value);
}

// Generic toString implementation - falls back to type name
template<typename T>
std::string VAny::Model<T>::toString() const {
    return "[" + std::string(typeid(T).name()) + "]";
}

// Specializations for common types
template<>
inline std::string VAny::Model<int>::toString() const {
    return std::to_string(_value);
}

template<>
inline std::string VAny::Model<unsigned int>::toString() const {
    return std::to_string(_value);
}

template<>
inline std::string VAny::Model<long>::toString() const {
    return std::to_string(_value);
}

template<>
inline std::string VAny::Model<unsigned long>::toString() const {
    return std::to_string(_value);
}

template<>
inline std::string VAny::Model<long long>::toString() const {
    return std::to_string(_value);
}

template<>
inline std::string VAny::Model<unsigned long long>::toString() const {
    return std::to_string(_value);
}

template<>
inline std::string VAny::Model<float>::toString() const {
    return std::to_string(_value);
}

template<>
inline std::string VAny::Model<double>::toString() const {
    return std::to_string(_value);
}

template<>
inline std::string VAny::Model<bool>::toString() const {
    return _value ? "true" : "false";
}

template<>
inline std::string VAny::Model<std::string>::toString() const {
    return _value;
}

template<>
inline std::string VAny::Model<const char*>::toString() const {
    return _value ? _value : "";
}

template<>
inline std::string VAny::Model<VString>::toString() const {
    return _value.toStdString();
} 