#include "CPU/CPU.hpp"
#include "Memory/Memory.hpp"
#include "Monitor/Monitor.hpp"

extern Monitor monitor;
extern MemoryManager mem_mgr;
extern CPU cpu;

extern "C" int pmem_read(u_int32_t addr) {
  try {
  return mem_mgr.PAddrRead(addr & ~0x3u, 4);
  } catch (std::string &msg) { std::cerr << msg << std::endl; }
}

extern "C" void pmem_write(u_int32_t addr, u_int32_t data, char mask) {
  std::cerr << "write" << std::endl;
  addr &= ~0x3u;
  for (int i = 0; i < 4; ++i) {
    if (((mask >> i) & 1) == 0) {
      continue;
    }
    mem_mgr.PAddrWrite(addr + i, 1, data >> (i * 8));
  }
}

extern "C" int get_inst() {
  return mem_mgr.PAddrRead(cpu.GetPC(), 4);
}

extern "C" void ebreak() {
  monitor.state = Monitor::State::END;
  monitor.ret = cpu.GetRegs()[10]; // $a0
}
