#include "CPU.hpp"

void CPU::SingleCycle() {
  printf("\n\n===== clk => 0 =====\n");

  dut.clk = 0;
  dut.eval();

  printf("\n\n===== clk => 1 =====\n");

  dut.clk = 1;
  dut.eval();
}

void CPU::Reset(int n) {
  dut.rst = 1;
  while (n-- > 0) {
    SingleCycle();
  }
  dut.rst = 0;
}

void CPU::Exec(uint64_t n) {
  while (n--) {
    SingleCycle();
  }
}
