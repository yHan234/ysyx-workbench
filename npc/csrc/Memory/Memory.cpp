#include "Memory/Memory.hpp"

Memory::Memory(MemoryManager &mgr) {
  mgr.Map(MEM_BASE, MEM_BASE + MEM_SIZE, {}, mem);
}
