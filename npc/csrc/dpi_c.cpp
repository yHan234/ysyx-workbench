#include "Memory/Memory.hpp"
#include "Monitor/Monitor.hpp"

extern Memory mem;

extern Monitor monitor;

extern "C" void pmem_read(u_int32_t pc, u_int32_t *instr) {
  if (pc == 0) {
    *instr = 0;
    return;
  }
  *instr = mem.PRead(pc, 4);
}

extern "C" void ebreak() {
  monitor.state = Monitor::State::END;
}
