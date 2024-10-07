#pragma once

#include "MemoryManager/MemoryManager.hpp"
#include "common.hpp"

class Memory {
public:
  Memory(MemoryManager &mgr);
  ~Memory();

  // public 为了方便 main.cpp:LoadImage
  byte *mem;
};
