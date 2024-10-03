#include "CPU/CPU.hpp"
#include "Memory/Memory.hpp"
#include "Monitor/Monitor.hpp"

extern Monitor monitor;
extern Memory mem;
extern CPU cpu;

extern "C" void get_inst(u_int32_t *inst) {
  *inst = mem.PRead(cpu.GetPC(), 4);
}

extern "C" void ebreak() {
  monitor.state = Monitor::State::END;
  monitor.ret = cpu.GetRegs()[10]; // $a0
}
