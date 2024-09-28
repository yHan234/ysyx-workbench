#include "Memory/Memory.hpp"

extern "C" void pmem_read(u_int32_t pc, u_int32_t *instr) {
  extern Memory mem;
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
