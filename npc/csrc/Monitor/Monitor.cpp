#include "Monitor.hpp"

std::string InstInfo::ToString() {
  return string_format("0x%08x: %08x %s", pc, inst, disasm.c_str());
}

Monitor::Monitor(CPU &cpu, Memory &mem)
    : cpu(cpu), mem(mem), state(State::STOP) {
  // init_disasm("riscv32");

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
    DiffTestStep();
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
  // info.disasm = disassemble(info.pc, reinterpret_cast<uint8_t *>(&info.inst), 4);
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

void Monitor::LoadDiffTestRef(const std::string &file) {
#ifdef DIFFTEST
  static word_t regs_pc[33];

  void *handle;
  handle = dlopen(file.c_str(), RTLD_LAZY);
  if (!handle) {
    throw("Failed to open DiffTest ref so file.");
  }

  void (*DTRefInit)(int) = (void (*)(int))dlsym(handle, "difftest_init");
  DTRefMemCpy = (void (*)(paddr_t addr, void *buf, size_t n, bool direction))dlsym(handle, "difftest_memcpy");
  DTRefRegCpy = (void (*)(void *dut, bool direction))dlsym(handle, "difftest_regcpy");
  DTRefExec = (void (*)(uint64_t n))dlsym(handle, "difftest_exec");
  DTRefRaiseIntr = (void (*)(uint64_t NO))dlsym(handle, "difftest_raise_intr");
  if (!DTRefInit || !DTRefMemCpy || !DTRefRegCpy || !DTRefExec || !DTRefRaiseIntr) {
    throw("Failed to load DiffTest ref so file.");
  }

  DTRefInit(0);
  DTRefMemCpy(INITIAL_PC, mem.GuestToHost(INITIAL_PC), mem.img_size, DUT_TO_REF);

  memset(regs_pc, 0, sizeof(CPU::Regs));
  regs_pc[32] = 0x80000000;
  DTRefRegCpy(regs_pc, DUT_TO_REF);
#else
  if (!file.empty()) {
    std::cerr << "DiffTest is not enabled" << std::endl;
  }
#endif
}

void Monitor::DiffTestStep() {
#ifdef DIFFTEST
  static word_t ref_regs[33];
  static word_t ref_pc;

  DTRefExec(1);
  DTRefRegCpy(ref_regs, REF_TO_DUT);
  ref_pc = ref_regs[32];
  auto &dut_regs = cpu.GetRegs();
  auto dut_pc = cpu.GetPC();

  if (dut_pc != ref_pc) {
    state = State::ABORT;
    std::cerr << string_format("Diff: PC DUT=%#08x Ref=%#08x", dut_pc, ref_pc) << std::endl;
  }
  for (int i = 0; i < 32; ++i) {
    if (dut_regs[i] != ref_regs[i]) {
      state = State::ABORT;
      std::cerr << string_format("Diff: Reg[%d] DUT=%d Ref=%d", i, dut_regs[i], ref_regs[i]) << std::endl;
    }
  }

#endif
}
