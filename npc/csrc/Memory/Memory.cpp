#include "Memory/Memory.hpp"

Memory::Memory(MemoryManager &mgr) {
  mem = new byte[MEM_BASE];
  mgr.Map(MEM_BASE, MEM_BASE + MEM_SIZE, [](uint32_t, int, bool) {}, mem);
}

Memory::~Memory() {
  delete[] mem;
}
