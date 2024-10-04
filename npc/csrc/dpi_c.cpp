#include "CPU/CPU.hpp"
#include "Memory/Memory.hpp"
#include "Monitor/Monitor.hpp"

extern Monitor monitor;
extern Memory mem;
extern CPU cpu;

extern "C" int pmem_read(u_int32_t addr) {
  if (monitor.state == Monitor::State::RESETTING) {
    return 0;
  }
  try {
    return mem.PRead(addr & ~0x3u, 4);
  } catch (std::string &msg) {
    std::cerr << msg << std::endl;
    return 0;
  }
}

extern "C" void pmem_write(u_int32_t addr, u_int32_t data, char mask) {
  addr &= ~0x3u;
  if (mask == 0b1)
    mem.PWrite(addr + 3, 1, data << 24);
  else if (mask == 0b11)
    mem.PWrite(addr + 2, 2, data << 16);
  else if (mask == 0b1111)
    mem.PWrite(addr, 4, data);
}

extern "C" int get_inst() {
  return mem.PRead(cpu.GetPC(), 4);
}

extern "C" void ebreak() {
  monitor.state = Monitor::State::END;
  monitor.ret = cpu.GetRegs()[10]; // $a0
}
