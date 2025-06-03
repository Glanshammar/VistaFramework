#pragma once
#include <vector>
#include <algorithm>
#include <functional>
#include <stdexcept>
#include <optional>
#include <initializer_list>
#include <type_traits>
#include <numeric>

// Forward declaration of VString
class VString;

template<typename T>
class VArray {
public:
    // Type definitions
    using value_type = T;
    using reference = T&;
    using const_reference = const T&;
    using pointer = T*;
    using const_pointer = const T*;
    using iterator = typename std::vector<T>::iterator;
    using const_iterator = typename std::vector<T>::const_iterator;
    using reverse_iterator = typename std::vector<T>::reverse_iterator;
    using const_reverse_iterator = typename std::vector<T>::const_reverse_iterator;
    using size_type = typename std::vector<T>::size_type;
    using difference_type = typename std::vector<T>::difference_type;

    // Constructors
    VArray();
    explicit VArray(size_type count, const T& value = T());
    VArray(std::initializer_list<T> init);
    template<typename InputIt>
    VArray(InputIt first, InputIt last);
    VArray(const VArray& other);
    VArray(VArray&& other) noexcept;
    
    // Assignment operators
    VArray& operator=(const VArray& other);
    VArray& operator=(VArray&& other) noexcept;
    VArray& operator=(std::initializer_list<T> ilist);

    // Element access
    reference at(size_type pos);
    const_reference at(size_type pos) const;
    reference operator[](size_type pos);
    const_reference operator[](size_type pos) const;
    reference front();
    const_reference front() const;
    reference back();
    const_reference back() const;
    T* data() noexcept;
    const T* data() const noexcept;
    
    // Iterators
    iterator begin() noexcept;
    const_iterator begin() const noexcept;
    const_iterator cbegin() const noexcept;
    iterator end() noexcept;
    const_iterator end() const noexcept;
    const_iterator cend() const noexcept;
    reverse_iterator rbegin() noexcept;
    const_reverse_iterator rbegin() const noexcept;
    const_reverse_iterator crbegin() const noexcept;
    reverse_iterator rend() noexcept;
    const_reverse_iterator rend() const noexcept;
    const_reverse_iterator crend() const noexcept;

    // Capacity
    [[nodiscard]] bool isEmpty() const noexcept;
    [[nodiscard]] size_type size() const noexcept;
    [[nodiscard]] size_type max_size() const noexcept;
    void reserve(size_type new_cap);
    [[nodiscard]] size_type capacity() const noexcept;
    void shrink_to_fit();

    // Modifiers
    void clear() noexcept;
    iterator insert(const_iterator pos, const T& value);
    iterator insert(const_iterator pos, T&& value);
    iterator insert(const_iterator pos, size_type count, const T& value);
    template<typename InputIt>
    iterator insert(const_iterator pos, InputIt first, InputIt last);
    iterator insert(const_iterator pos, std::initializer_list<T> ilist);
    
    template<typename... Args>
    iterator emplace(const_iterator pos, Args&&... args);
    template<typename... Args>
    reference emplace_back(Args&&... args);
    
    iterator erase(const_iterator pos);
    iterator erase(const_iterator first, const_iterator last);
    void push_back(const T& value);
    void push_back(T&& value);
    void pop_back();
    void resize(size_type count);
    void resize(size_type count, const value_type& value);
    void swap(VArray& other) noexcept;

    // Enhanced functionality
    // Find element and return iterator
    iterator find(const T& value);
    const_iterator find(const T& value) const;
    
    // Find element with predicate
    template<typename Predicate>
    iterator findIf(Predicate pred);
    template<typename Predicate>
    const_iterator findIf(Predicate pred) const;
    
    // Contains element
    [[nodiscard]] bool contains(const T& value) const;
    
    // Remove element(s) by value
    size_type remove(const T& value);
    
    // Remove elements that satisfy predicate
    template<typename Predicate>
    size_type removeIf(Predicate pred);
    
    // Count elements that match value
    [[nodiscard]] size_type count(const T& value) const;
    
    // Count elements that satisfy predicate
    template<typename Predicate>
    [[nodiscard]] size_type countIf(Predicate pred) const;
    
    // Transform all elements
    template<typename UnaryOperation>
    VArray& transform(UnaryOperation op);
    
    // Create a new array by transforming elements
    template<typename UnaryOperation>
    auto map(UnaryOperation op) const;
    
    // Filter elements based on predicate
    template<typename Predicate>
    VArray filter(Predicate pred) const;
    
    // Reduce to single value
    template<typename BinaryOperation>
    T reduce(const T& init, BinaryOperation op) const;
    
    // Join elements with separator
    template<typename CharT = char>
    std::basic_string<CharT> join(const std::basic_string<CharT>& separator = {}) const;
    
    // Join elements with VString separator (this is a non-template method)
    VString join(const VString& separator) const;
    
    // Get first element that satisfies predicate or default
    template<typename Predicate>
    std::optional<T> firstOrNone(Predicate pred) const;
    
    // Sort the array
    void sort();
    
    // Sort with custom comparator
    template<typename Compare>
    void sort(Compare comp);
    
    // Reverse the array
    void reverse();
    
    // Shuffle the array
    void shuffle();
    
    // Get a slice of the array
    VArray slice(size_type start, size_type end) const;
    
    // Concatenate with another array
    VArray concat(const VArray& other) const;
    
    // Equality operators
    bool operator==(const VArray& other) const;
    bool operator!=(const VArray& other) const;

private:
    std::vector<T> _data;
};

#include "VArray.tpp" 