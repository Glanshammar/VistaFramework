#pragma once
#include <typeinfo>
#include <memory>
#include <stdexcept>
#include <string>
#include <iostream>

class VAny {
private:
    struct Base {
        virtual ~Base() = default;
        virtual const std::type_info& type() const = 0;
        virtual Base* clone() const = 0;
        virtual void print(std::ostream& os) const = 0;
    };

    template<typename T>
    struct Holder;

    Base* content;

public:
    VAny() : content(nullptr) {}
    
    template<typename T>
    explicit VAny(const T& value) : content(new Holder<T>(value)) {}
    
    VAny(const VAny& other) : content(other.content ? other.content->clone() : nullptr) {}
    
    VAny(VAny&& other) noexcept : content(other.content) {
        other.content = nullptr;
    }
    
    ~VAny() {
        delete content;
    }
    
    VAny& operator=(const VAny& other) {
        if (this != &other) {
            delete content;
            content = other.content ? other.content->clone() : nullptr;
        }
        return *this;
    }
    
    VAny& operator=(VAny&& other) noexcept {
        if (this != &other) {
            delete content;
            content = other.content;
            other.content = nullptr;
        }
        return *this;
    }
    
    template<typename T>
    T& cast() {
        if (!content) {
            throw std::bad_cast();
        }
        if (typeid(T) != content->type()) {
            throw std::bad_cast();
        }
        return static_cast<Holder<T>*>(content)->value;
    }
    
    template<typename T>
    const T& cast() const {
        if (!content) {
            throw std::bad_cast();
        }
        if (typeid(T) != content->type()) {
            throw std::bad_cast();
        }
        return static_cast<Holder<T>*>(content)->value;
    }
    
    template<typename T>
    bool is() const {
        return content && typeid(T) == content->type();
    }
    
    const std::type_info& type() const {
        if (!content) {
            return typeid(void);
        }
        return content->type();
    }
    
    friend std::ostream& operator<<(std::ostream& os, const VAny& any) {
        if (any.content) {
            any.content->print(os);
        }
        return os;
    }
};

template<typename T>
struct VAny::Holder : VAny::Base {
    T value;
    
    explicit Holder(const T& v) : value(v) {}
    
    const std::type_info& type() const override {
        return typeid(T);
    }
    
    Base* clone() const override {
        return new Holder(value);
    }
    
    void print(std::ostream& os) const override {
        os << value;
    }
};