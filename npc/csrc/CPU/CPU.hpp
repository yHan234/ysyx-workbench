#pragma once

#include "Vtop.h"
#include "Vtop_GPR.h"
#include "Vtop___024root.h"
#include "Vtop_top.h"
#include "common.hpp"

class CPU {
public:
  using Regs = VlUnpacked<IData /*31:0*/, 32>;

  void Reset(uint64_t n);

  void Exec(uint64_t n);

  const Regs &GetRegs();

  vaddr_t GetPC();

  word_t GetInst();

  bool IsResetting();

  std::function<int()> before_exec;
  std::function<int()> before_step;
  std::function<int()> after_step;
  std::function<int()> after_exec;

private:
  void SingleCycle();

private:
  Vtop dut;
};
