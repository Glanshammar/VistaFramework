#pragma once
#include <memory>
#include <typeindex>
#include <functional>
#include <stdexcept>
#include <string>
#include <ostream>

class VAny {
public:
    // Constructors and destructor
    VAny();
    template<typename T>
    VAny(const T& value);
    VAny(const VAny& other);
    VAny(VAny&& other) noexcept;
    ~VAny();

    // Assignment operators
    VAny& operator=(const VAny& other);
    VAny& operator=(VAny&& other) noexcept;
    template<typename T>
    VAny& operator=(const T& value);

    // Type checking
    [[nodiscard]] bool isEmpty() const;
    [[nodiscard]] std::type_index type() const;
    [[nodiscard]] std::string typeName() const;
    template<typename T>
    [[nodiscard]] bool isType() const;

    // Value retrieval
    template<typename T>
    T& cast();
    template<typename T>
    const T& cast() const;
    
    // Safe casting with default value
    template<typename T>
    T castOr(const T& defaultValue) const;

    // Reset
    void reset();
    
    // Swap
    void swap(VAny& other) noexcept;

    // Stream output
    friend std::ostream& operator<<(std::ostream& os, const VAny& any);

private:
    class Concept {
    public:
        virtual ~Concept() = default;
        [[nodiscard]] virtual std::type_index type() const = 0;
        [[nodiscard]] virtual std::unique_ptr<Concept> clone() const = 0;
        [[nodiscard]] virtual std::string toString() const = 0;
    };

    template<typename T>
    class Model : public Concept {
    public:
        explicit Model(const T& value) : _value(value) {}
        [[nodiscard]] std::type_index type() const override { return std::type_index(typeid(T)); }
        [[nodiscard]] std::unique_ptr<Concept> clone() const override { return std::make_unique<Model<T>>(_value); }
        [[nodiscard]] std::string toString() const override;
        T _value;
    };

    std::unique_ptr<Concept> _value;
};

// Stream output operator
inline std::ostream& operator<<(std::ostream& os, const VAny& any) {
    if (any.isEmpty()) {
        return os << "[empty]";
    }
    return os << any._value->toString();
}
#include "VAny.tpp" 