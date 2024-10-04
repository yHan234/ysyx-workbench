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
  if (before_exec() != 0) {
    return;
  }

  while (n--) {
    SingleCycle();

    if (after_step() != 0) {
      break;
    }
  }

  after_exec();
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
