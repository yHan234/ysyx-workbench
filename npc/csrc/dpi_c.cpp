#include "CPU/CPU.hpp"
#include "Memory/Memory.hpp"
#include "Monitor/Monitor.hpp"

extern Monitor monitor;
extern Memory mem;
extern CPU cpu;

extern "C" void pmem_read(u_int32_t pc, u_int32_t *instr) {
  if (pc == 0) {
    *instr = 0;
    return;
  }
  *instr = mem.PRead(pc, 4);
}

extern "C" void ebreak() {
  monitor.state = Monitor::State::END;
  monitor.ret = cpu.GetRegs()[10]; // $a0
}
