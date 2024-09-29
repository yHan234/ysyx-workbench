#include "Memory/Memory.hpp"

extern constexpr size_t MEM_SIZE = 0x80000000;
extern constexpr size_t MEM_BASE = 0x80000000;
extern Memory<MEM_SIZE, MEM_BASE> mem;

extern "C" void pmem_read(u_int32_t pc, u_int32_t *instr) {
  if (pc == 0) {
    *instr = 0;
    return;
  }
  *instr = mem.ReadPMem(pc, 4);
}

extern "C" void ebreak() {
  printf("ebreak\n");
  exit(0);
}
