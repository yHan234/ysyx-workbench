#pragma once

#include "MemoryManager/MemoryManager.hpp"
#include "common.hpp"

class Memory {
public:
  Memory(MemoryManager &mgr);

  // public 因为方便 main.cpp:LoadImage
  byte mem[MEM_SIZE];
};
