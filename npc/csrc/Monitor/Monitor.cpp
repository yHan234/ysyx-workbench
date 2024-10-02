#include "Monitor.hpp"

std::string InstInfo::ToString() {
  return string_format("0x%08x: %08x %s", pc, inst, disasm.c_str());
}

Monitor::Monitor(CPU &cpu, Memory &mem)
    : cpu(cpu), mem(mem), state(State::STOP) {
  init_disasm("riscv32");

  cpu.before_exec = [&]() -> int {
    switch (state) {
    case State::END:
    case State::ABORT:
      std::cout << "Program execution has ended. To restart the program, exit NEMU and run again." << std::endl;
      return 1;
    default:
      state = State::RUNNING;
      return 0;
    }
  };

  cpu.after_step = [&]() -> int {
    ITrace();
    return state == State::RUNNING ? 0 : 1;
  };

  cpu.after_exec = [&]() {
    switch (state) {
    case State::RUNNING:
      state = State::STOP;
      break;
    case State::END:
    case State::ABORT:
      PrintITrace();
      if (state == State::ABORT) {
        std::cout << "ABORT" << std::endl;
      } else if (ret == 0) {
        std::cout << "HIT GOOD TRAP" << std::endl;
      } else {
        std::cout << "HIT BAD TRAP" << std::endl;
      }
    }
  };
}

void Monitor::ITrace() {
#ifdef ITRACE
  InstInfo info;
  info.pc = cpu.GetPC();
  info.inst = cpu.GetInst();
  info.disasm = disassemble(info.pc, reinterpret_cast<uint8_t *>(&info.inst), 4);
  ibuf.Write(std::move(info));
#endif
}

void Monitor::PrintITrace() {
#ifdef ITRACE
  std::cout << "ITRACE:" << std::endl;
  for (auto &inst : ibuf) {
    std::cout << inst.ToString() << std::endl;
  }
#endif
}
