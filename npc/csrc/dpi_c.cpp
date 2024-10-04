#include "CPU/CPU.hpp"
#include "Memory/Memory.hpp"
#include "Monitor/Monitor.hpp"

extern Monitor monitor;
extern Memory mem;
extern CPU cpu;

extern "C" int pmem_read(u_int32_t addr) {
  std::cout << std::hex << addr << std::endl;
  try {
    return mem.PRead(addr & ~0x3u, 4);
  } catch (std::string &msg) {
    return 0;
  }
}

extern "C" void pmem_write(u_int32_t addr, u_int32_t data, char mask) {
  addr &= ~0x3u;
  for (int i = 0; i < 4; ++i) {
    if (((mask >> (3 - i)) & 1) == 0) {
      continue;
    }
    mem.PWrite(addr + i, 1, data >> ((3 - i) * 8));
  }
}

extern "C" int get_inst() {
  return mem.PRead(cpu.GetPC(), 4);
}

extern "C" void ebreak() {
  monitor.state = Monitor::State::END;
  monitor.ret = cpu.GetRegs()[10]; // $a0
}
