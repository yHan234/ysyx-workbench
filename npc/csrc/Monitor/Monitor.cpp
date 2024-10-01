#include "Monitor.hpp"

std::string InstInfo::ToString() {
  return string_format("0x%08x: %08x %s", pc, inst, disasm.c_str());
}

void Monitor::ITrace(vaddr_t pc, uint32_t inst) {
#ifdef ITRACE
  InstInfo info;
  info.pc = pc;
  info.inst = inst;
  info.disasm = disassemble(pc, reinterpret_cast<uint8_t *>(&inst), 4);
  ibuf.Write(std::move(info));
#endif
}

void Monitor::PrintITrace() {
#ifdef ITRACE
  for (auto &inst : ibuf) {
    std::cout << inst.ToString() << std::endl;
  }
#endif
}
