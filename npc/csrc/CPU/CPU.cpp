#include "CPU.hpp"

void CPU::SingleCycle() {
  dut.clk = 0;
  dut.eval();
  dut.clk = 1;
  dut.eval();
}

void CPU::Reset(uint64_t n) {
  dut.rst = 1;
  while (n--) {
    SingleCycle();
  }
  dut.rst = 0;
}

void CPU::Exec(uint64_t n) {
#define CALLBACK(func, if_err) \
  do {                         \
    if (func() != 0) {         \
      if_err;                  \
    }                          \
  } while (0)

  CALLBACK(before_exec, return);
  while (n--) {
    CALLBACK(before_step, break);
    SingleCycle();
    CALLBACK(after_step, break);
  }
  CALLBACK(after_exec, return);

#undef CALLBACK
}

const CPU::Regs &CPU::GetRegs() {
  return dut.top->gpr->gpr;
}

vaddr_t CPU::GetPC() {
  return dut.top->pc;
}

word_t CPU::GetInst() {
  return dut.top->inst;
}
