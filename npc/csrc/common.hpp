#pragma once

#include "Utils/string_format.hpp"
#include <cstddef>

using byte = char;
using std::size_t;

using word_t = uint32_t;
using paddr_t = word_t;
using vaddr_t = word_t;

#define MEM_BASE 0x80000000
#define MEM_SIZE 0x80000000
#define INITIAL_PC 0x80000000
