#include "CPU.hpp"

CPU::CPU(Monitor &monitor)
    : monitor(monitor) {}

void CPU::SingleCycle() {
  printf("\n\n===== clk => 0 =====\n");

  dut.clk = 0;
  dut.eval();

  printf("\n\n===== clk => 1 =====\n");

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
  using State = Monitor::State;

  switch (monitor.state) {
  case State::END:
  case State::ABORT:
    std::cout << "Program execution has ended. To restart the program, exit NEMU and run again." << std::endl;
    return;
  default:
    monitor.state = State::RUNNING;
  }

  while (n--) {
    SingleCycle();

    if (monitor.state != State::RUNNING) {
      break;
    }
  }

  switch (monitor.state) {
  case State::RUNNING:
    monitor.state = State::STOP;
    break;
  }
}

CPU::Regs CPU::GetRegs() {
  return dut.rootp->top__DOT__gpr__DOT__gpr;
}
