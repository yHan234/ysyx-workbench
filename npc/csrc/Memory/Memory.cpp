#include "Memory/Memory.hpp"

Memory::Memory(MemoryManager &mgr) {
  mem = new byte[MEM_BASE];
  mgr.Map(MEM_BASE, MEM_BASE + MEM_SIZE, {}, mem);
}

Memory::~Memory() {
  delete[] mem;
}
