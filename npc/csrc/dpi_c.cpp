#include "CPU/CPU.hpp"
#include "Memory/Memory.hpp"
#include "Monitor/Monitor.hpp"

extern Monitor monitor;
extern Memory mem;
extern CPU cpu;

extern "C" void get_inst(u_int32_t *inst) {
  vaddr_t pc = cpu.GetPC();

  if (pc == 0 || pc == 0x7ffffffc) {
    *inst = 0;
    return;
  }

  *inst = mem.PRead(pc, 4);
}

extern "C" void ebreak() {
  monitor.state = Monitor::State::END;
  monitor.ret = cpu.GetRegs()[10]; // $a0
}
