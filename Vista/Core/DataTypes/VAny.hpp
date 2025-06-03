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
    VAny();
    
    template<typename T>
    explicit VAny(const T& value);
    
    VAny(const VAny& other);
    VAny(VAny&& other) noexcept;
    ~VAny();
    
    VAny& operator=(const VAny& other);
    VAny& operator=(VAny&& other) noexcept;
    
    template<typename T>
    T& cast();
    
    template<typename T>
    const T& cast() const;
    
    template<typename T>
    bool is() const;
    
    const std::type_info& type() const;
    
    friend std::ostream& operator<<(std::ostream& os, const VAny& any);
};

#include "VAny.tpp" 