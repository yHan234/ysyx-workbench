#include "Monitor.hpp"
#include <dlfcn.h>
#include <iostream>

std::string InstInfo::ToString() {
  return string_format("%#08x: %08x %s", pc, inst, disasm.c_str());
}

std::string MemInfo::ToString() {
  if (is_write) {
    return string_format("%#08x: %08x %d [%#0*x => %#0*x]", pc, addr, len, len * 2 + 2, w_pre_data, len * 2 + 2, data);
  } else {
    return string_format("%#08x: %08x %d [%#0*x]", pc, addr, len, len * 2, data);
  }
}

Monitor::Monitor(CPU &cpu, MemoryManager &mem_mgr)
    : cpu(cpu), mem_mgr(mem_mgr), state(State::STOP) {
  // init_disasm("riscv32");

  cpu.before_exec = [&]() -> int {
    switch (state) {
    case State::END:
    case State::ABORT:
      std::cout << "Program execution has ended. To restart the program, exit and run again." << std::endl;
      return 1;
    default:
      state = State::RUNNING;
      return 0;
    }
  };

  cpu.before_step = [&]() -> int {
    pc = cpu.GetPC(); // 本次执行的 PC
    return 0;
  };

  cpu.after_step = [&]() -> int {
    inst = cpu.GetInst(); // 本次执行的 inst
    ITrace();
    if (state == State::RUNNING) {
      DiffTestStep();
      return 0;
    } else {
      return 1;
    }
  };

  cpu.after_exec = [&]() -> int {
    switch (state) {
    case State::RUNNING:
      state = State::STOP;
      break;
    case State::END:
    case State::ABORT:
      LogITrace();
      LogMTrace();
      if (state == State::ABORT) {
        std::cout << "ABORT" << std::endl;
      } else if (ret == 0) {
        std::cout << "HIT GOOD TRAP" << std::endl;
      } else {
        std::cout << "HIT BAD TRAP" << std::endl;
      }
    }
    return 0;
  };

  mem_mgr.trace_write = [&](bool succ, paddr_t addr, int len, word_t cur_data, word_t pre_data) {
    // 未开启内存读写错误检查，因为电路设计原因，经常有实际不需要的错误内存读写行为
    // TODO: 修正电路设计
    // if (!succ) {
    //   state = State::ABORT;
    //   std::cerr << "Memory write failed. Check the last MTrace." << std::endl;
    // }
    MTrace(1, addr, len, cur_data, pre_data);
  };

  mem_mgr.trace_read = [&](bool succ, paddr_t addr, int len, word_t data) {
    // 未开启内存读写错误检查，因为电路设计原因，经常有实际不需要的错误内存读写行为
    // TODO: 修正电路设计
    // if (!succ) {
    //   state = State::ABORT;
    //   std::cerr << "Memory read failed. Check the last MTrace." << std::endl;
    // }
    MTrace(0, addr, len, data, 0);
  };
}

bool Monitor::IsExitStatusBad() {
  bool good = (state == State::END && ret == 0) || state == State::QUIT;
  return !good;
}

void Monitor::OpenLogFile(const std::string &log_file) {
  // 没有关闭文件，程序退出自动关闭
  log.open(log_file, std::ios_base::out);
  if (!log.is_open()) {
    throw string_format("Log file %s open failed.", log_file.c_str());
  }
}

void Monitor::ITrace() {
#ifdef ITRACE
  InstInfo info;
  info.pc = pc;
  info.inst = inst;
  // info.disasm = disassemble(info.pc, reinterpret_cast<uint8_t *>(&info.inst), 4);
  ibuf.Write(std::move(info));
#endif
}

void Monitor::LogITrace() {
#ifdef ITRACE
  if (!log.is_open()) {
    return;
  }

  log << "ITRACE:" << '\n';
  for (auto &info : ibuf) {
    log << info.ToString() << '\n';
  }
  log << '\n';
#endif
}

void Monitor::MTrace(bool is_write, vaddr_t addr, int len, word_t data, word_t w_pre_data) {
#ifdef MTRACE
  if (!is_write && addr == pc) { // 取指令不记录
    return;
  }
  mbuf.Write({is_write, pc, addr, len, data, w_pre_data});
#endif
}

void Monitor::LogMTrace() {
#ifdef MTRACE
  if (!log.is_open()) {
    return;
  }

  log << "MTRACE:" << '\n';
  for (auto &info : mbuf) {
    log << info.ToString() << '\n';
  }
  log << '\n';
#endif
}

void Monitor::LoadDiffTestRef(const std::string &ref_so_file, char *img_addr, size_t img_size) {
#ifdef DIFFTEST
  static word_t regs_pc[33];

  void *handle;
  handle = dlopen(ref_so_file.c_str(), RTLD_LAZY);
  if (!handle) {
    throw(std::string("Failed to open DiffTest ref so file."));
  }

  void (*DTRefInit)(int) = (void (*)(int))dlsym(handle, "difftest_init");
  DTRefMemCpy = (void (*)(paddr_t addr, void *buf, size_t n, bool direction))dlsym(handle, "difftest_memcpy");
  DTRefRegCpy = (void (*)(void *dut, bool direction))dlsym(handle, "difftest_regcpy");
  DTRefExec = (void (*)(uint64_t n))dlsym(handle, "difftest_exec");
  DTRefRaiseIntr = (void (*)(uint64_t NO))dlsym(handle, "difftest_raise_intr");
  if (!DTRefInit || !DTRefMemCpy || !DTRefRegCpy || !DTRefExec || !DTRefRaiseIntr) {
    throw(std::string("Failed to load DiffTest ref so file."));
  }

  DTRefInit(0);
  DTRefMemCpy(INITIAL_PC, img_addr, img_size, DUT_TO_REF);

  memset(regs_pc, 0, sizeof(CPU::Regs));
  regs_pc[32] = 0x80000000;
  DTRefRegCpy(regs_pc, DUT_TO_REF);
#else
  if (!ref_so_file.empty()) {
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
  for (int i = 1; i < 32; ++i) { // 不比较 reg0
    if (dut_regs[i] != ref_regs[i]) {
      state = State::ABORT;
      std::cerr << string_format("Diff: Reg[%d] DUT=%x Ref=%x", i, dut_regs[i], ref_regs[i]) << std::endl;
    }
  }

#endif
}
