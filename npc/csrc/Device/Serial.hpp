#pragma once

#include "MemoryManager/MemoryManager.hpp"
#include "common.hpp"

class Serial {
public:
  Serial(MemoryManager &mem_mgr);
  ~Serial();

  void Handle(uint32_t offset, int len, bool is_write);

private:
  static constexpr size_t SIZE = 1;
  byte *mem;
};
