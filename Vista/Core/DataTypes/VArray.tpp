#pragma once
#include "VArray.hpp"
#include <random>
#include <sstream>
#include <VString>

// Constructors
template<typename T>
VArray<T>::VArray() = default;

template<typename T>
VArray<T>::VArray(size_type count, const T& value) : _data(count, value) {}

template<typename T>
VArray<T>::VArray(std::initializer_list<T> init) : _data(init) {}

template<typename T>
template<typename InputIt>
VArray<T>::VArray(InputIt first, InputIt last) : _data(first, last) {}

template<typename T>
VArray<T>::VArray(const VArray& other) : _data(other._data) {}

template<typename T>
VArray<T>::VArray(VArray&& other) noexcept : _data(std::move(other._data)) {}

// Assignment operators
template<typename T>
VArray<T>& VArray<T>::operator=(const VArray& other) {
    _data = other._data;
    return *this;
}

template<typename T>
VArray<T>& VArray<T>::operator=(VArray&& other) noexcept {
    _data = std::move(other._data);
    return *this;
}

template<typename T>
VArray<T>& VArray<T>::operator=(std::initializer_list<T> ilist) {
    _data = ilist;
    return *this;
}

// Element access
template<typename T>
typename VArray<T>::reference VArray<T>::at(size_type pos) {
    return _data.at(pos);
}

template<typename T>
typename VArray<T>::const_reference VArray<T>::at(size_type pos) const {
    return _data.at(pos);
}

template<typename T>
typename VArray<T>::reference VArray<T>::operator[](size_type pos) {
    return _data[pos];
}

template<typename T>
typename VArray<T>::const_reference VArray<T>::operator[](size_type pos) const {
    return _data[pos];
}

template<typename T>
typename VArray<T>::reference VArray<T>::front() {
    if (isEmpty()) {
        throw std::out_of_range("VArray is empty");
    }
    return _data.front();
}

template<typename T>
typename VArray<T>::const_reference VArray<T>::front() const {
    if (isEmpty()) {
        throw std::out_of_range("VArray is empty");
    }
    return _data.front();
}

template<typename T>
typename VArray<T>::reference VArray<T>::back() {
    if (isEmpty()) {
        throw std::out_of_range("VArray is empty");
    }
    return _data.back();
}

template<typename T>
typename VArray<T>::const_reference VArray<T>::back() const {
    if (isEmpty()) {
        throw std::out_of_range("VArray is empty");
    }
    return _data.back();
}

template<typename T>
T* VArray<T>::data() noexcept {
    return _data.data();
}

template<typename T>
const T* VArray<T>::data() const noexcept {
    return _data.data();
}

// Iterators
template<typename T>
typename VArray<T>::iterator VArray<T>::begin() noexcept {
    return _data.begin();
}

template<typename T>
typename VArray<T>::const_iterator VArray<T>::begin() const noexcept {
    return _data.begin();
}

template<typename T>
typename VArray<T>::const_iterator VArray<T>::cbegin() const noexcept {
    return _data.cbegin();
}

template<typename T>
typename VArray<T>::iterator VArray<T>::end() noexcept {
    return _data.end();
}

template<typename T>
typename VArray<T>::const_iterator VArray<T>::end() const noexcept {
    return _data.end();
}

template<typename T>
typename VArray<T>::const_iterator VArray<T>::cend() const noexcept {
    return _data.cend();
}

template<typename T>
typename VArray<T>::reverse_iterator VArray<T>::rbegin() noexcept {
    return _data.rbegin();
}

template<typename T>
typename VArray<T>::const_reverse_iterator VArray<T>::rbegin() const noexcept {
    return _data.rbegin();
}

template<typename T>
typename VArray<T>::const_reverse_iterator VArray<T>::crbegin() const noexcept {
    return _data.crbegin();
}

template<typename T>
typename VArray<T>::reverse_iterator VArray<T>::rend() noexcept {
    return _data.rend();
}

template<typename T>
typename VArray<T>::const_reverse_iterator VArray<T>::rend() const noexcept {
    return _data.rend();
}

template<typename T>
typename VArray<T>::const_reverse_iterator VArray<T>::crend() const noexcept {
    return _data.crend();
}

// Capacity
template<typename T>
bool VArray<T>::isEmpty() const noexcept {
    return _data.empty();
}

template<typename T>
typename VArray<T>::size_type VArray<T>::size() const noexcept {
    return _data.size();
}

template<typename T>
typename VArray<T>::size_type VArray<T>::max_size() const noexcept {
    return _data.max_size();
}

template<typename T>
void VArray<T>::reserve(size_type new_cap) {
    _data.reserve(new_cap);
}

template<typename T>
typename VArray<T>::size_type VArray<T>::capacity() const noexcept {
    return _data.capacity();
}

template<typename T>
void VArray<T>::shrink_to_fit() {
    _data.shrink_to_fit();
}

// Modifiers
template<typename T>
void VArray<T>::clear() noexcept {
    _data.clear();
}

template<typename T>
typename VArray<T>::iterator VArray<T>::insert(const_iterator pos, const T& value) {
    return _data.insert(pos, value);
}

template<typename T>
typename VArray<T>::iterator VArray<T>::insert(const_iterator pos, T&& value) {
    return _data.insert(pos, std::move(value));
}

template<typename T>
typename VArray<T>::iterator VArray<T>::insert(const_iterator pos, size_type count, const T& value) {
    return _data.insert(pos, count, value);
}

template<typename T>
template<typename InputIt>
typename VArray<T>::iterator VArray<T>::insert(const_iterator pos, InputIt first, InputIt last) {
    return _data.insert(pos, first, last);
}

template<typename T>
typename VArray<T>::iterator VArray<T>::insert(const_iterator pos, std::initializer_list<T> ilist) {
    return _data.insert(pos, ilist);
}

template<typename T>
template<typename... Args>
typename VArray<T>::iterator VArray<T>::emplace(const_iterator pos, Args&&... args) {
    return _data.emplace(pos, std::forward<Args>(args)...);
}

template<typename T>
template<typename... Args>
typename VArray<T>::reference VArray<T>::emplace_back(Args&&... args) {
    return _data.emplace_back(std::forward<Args>(args)...);
}

template<typename T>
typename VArray<T>::iterator VArray<T>::erase(const_iterator pos) {
    return _data.erase(pos);
}

template<typename T>
typename VArray<T>::iterator VArray<T>::erase(const_iterator first, const_iterator last) {
    return _data.erase(first, last);
}

template<typename T>
void VArray<T>::push_back(const T& value) {
    _data.push_back(value);
}

template<typename T>
void VArray<T>::push_back(T&& value) {
    _data.push_back(std::move(value));
}

template<typename T>
void VArray<T>::pop_back() {
    if (isEmpty()) {
        throw std::out_of_range("VArray is empty");
    }
    _data.pop_back();
}

template<typename T>
void VArray<T>::resize(size_type count) {
    _data.resize(count);
}

template<typename T>
void VArray<T>::resize(size_type count, const value_type& value) {
    _data.resize(count, value);
}

template<typename T>
void VArray<T>::swap(VArray& other) noexcept {
    _data.swap(other._data);
}

// Enhanced functionality
template<typename T>
typename VArray<T>::iterator VArray<T>::find(const T& value) {
    return std::find(begin(), end(), value);
}

template<typename T>
typename VArray<T>::const_iterator VArray<T>::find(const T& value) const {
    return std::find(begin(), end(), value);
}

template<typename T>
template<typename Predicate>
typename VArray<T>::iterator VArray<T>::findIf(Predicate pred) {
    return std::find_if(begin(), end(), pred);
}

template<typename T>
template<typename Predicate>
typename VArray<T>::const_iterator VArray<T>::findIf(Predicate pred) const {
    return std::find_if(begin(), end(), pred);
}

template<typename T>
bool VArray<T>::contains(const T& value) const {
    return find(value) != end();
}

template<typename T>
typename VArray<T>::size_type VArray<T>::remove(const T& value) {
    auto oldSize = size();
    auto it = std::remove(begin(), end(), value);
    _data.erase(it, end());
    return oldSize - size();
}

template<typename T>
template<typename Predicate>
typename VArray<T>::size_type VArray<T>::removeIf(Predicate pred) {
    auto oldSize = size();
    auto it = std::remove_if(begin(), end(), pred);
    _data.erase(it, end());
    return oldSize - size();
}

template<typename T>
typename VArray<T>::size_type VArray<T>::count(const T& value) const {
    return std::count(begin(), end(), value);
}

template<typename T>
template<typename Predicate>
typename VArray<T>::size_type VArray<T>::countIf(Predicate pred) const {
    return std::count_if(begin(), end(), pred);
}

template<typename T>
template<typename UnaryOperation>
VArray<T>& VArray<T>::transform(UnaryOperation op) {
    std::transform(begin(), end(), begin(), op);
    return *this;
}

template<typename T>
template<typename UnaryOperation>
auto VArray<T>::map(UnaryOperation op) const {
    using ReturnType = std::decay_t<decltype(op(std::declval<T>()))>;
    VArray<ReturnType> result;
    result.reserve(size());
    for (const auto& item : *this) {
        result.push_back(op(item));
    }
    return result;
}

template<typename T>
template<typename Predicate>
VArray<T> VArray<T>::filter(Predicate pred) const {
    VArray<T> result;
    std::copy_if(begin(), end(), std::back_inserter(result._data), pred);
    return result;
}

template<typename T>
template<typename BinaryOperation>
T VArray<T>::reduce(const T& init, BinaryOperation op) const {
    return std::accumulate(begin(), end(), init, op);
}

template<typename T>
template<typename CharT>
std::basic_string<CharT> VArray<T>::join(const std::basic_string<CharT>& separator) const {
    if (isEmpty()) {
        return {};
    }
    
    std::basic_ostringstream<CharT> oss;
    auto it = begin();
    
    if constexpr (std::is_convertible_v<T, std::basic_string<CharT>> || 
                 std::is_convertible_v<T, const CharT*> ||
                 std::is_same_v<T, CharT>) {
        oss << *it++;
        for (; it != end(); ++it) {
            oss << separator << *it;
        }
    } else {
        oss << *it++;
        for (; it != end(); ++it) {
            oss << separator << *it;
        }
    }
    
    return oss.str();
}

template<typename T>
template<typename Predicate>
std::optional<T> VArray<T>::firstOrNone(Predicate pred) const {
    auto it = findIf(pred);
    if (it != end()) {
        return *it;
    }
    return std::nullopt;
}

template<typename T>
void VArray<T>::sort() {
    std::sort(begin(), end());
}

template<typename T>
template<typename Compare>
void VArray<T>::sort(Compare comp) {
    std::sort(begin(), end(), comp);
}

template<typename T>
void VArray<T>::reverse() {
    std::reverse(begin(), end());
}

template<typename T>
void VArray<T>::shuffle() {
    static std::random_device rd;
    static std::mt19937 g(rd());
    std::shuffle(begin(), end(), g);
}

template<typename T>
VArray<T> VArray<T>::slice(size_type start, size_type end) const {
    if (start > size() || end > size() || start > end) {
        throw std::out_of_range("Invalid slice range");
    }
    
    auto first = begin() + start;
    auto last = begin() + end;
    
    return VArray<T>(first, last);
}

template<typename T>
VArray<T> VArray<T>::concat(const VArray& other) const {
    VArray<T> result;
    result.reserve(size() + other.size());
    result._data.insert(result.end(), begin(), end());
    result._data.insert(result.end(), other.begin(), other.end());
    return result;
}

template<typename T>
bool VArray<T>::operator==(const VArray& other) const {
    return _data == other._data;
}

template<typename T>
bool VArray<T>::operator!=(const VArray& other) const {
    return _data != other._data;
}

// Generic implementation for join with VString separator
template<typename T>
VString VArray<T>::join(const VString& separator) const {
    if (isEmpty()) {
        return VString();
    }
    
    // Convert first element to string and initialize the result
    std::ostringstream oss;
    oss << (*this)[0];
    VString result(oss.str().c_str());
    
    // Append remaining elements with separator
    for (size_t i = 1; i < size(); ++i) {
        oss.str("");  // Clear the stream for next element
        oss << (*this)[i];
        result += separator;
        result += VString(oss.str().c_str());
    }
    
    return result;
}