#pragma once

#include "Utils/string_format.hpp"
#include <cstddef>

using std::byte;
using std::size_t;

using word_t = uint32_t;
using paddr_t = word_t;
using vaddr_t = word_t;

constexpr size_t MEM_BASE = 0x80000000;
constexpr size_t MEM_SIZE = 0x80000000;
