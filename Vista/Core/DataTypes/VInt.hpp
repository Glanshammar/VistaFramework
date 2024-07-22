#pragma once

#include <iostream>
#include <stdexcept>
#include <cstdint>

template <typename T>
class VIntBase {

};

// Type definitions for specific integer types
using VInt = VIntBase<int32_t>;
using VInt64 = VIntBase<int64_t>;
using VInt32 = VIntBase<int32_t>;
using VInt16 = VIntBase<int16_t>;
using VInt8 = VIntBase<int8_t>;

#include "VInt.tpp"  // Include template implementation