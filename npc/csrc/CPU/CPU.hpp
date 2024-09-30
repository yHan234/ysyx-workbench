#pragma once

#include "Monitor/Monitor.hpp"
#include "Vtop.h"
#include "Vtop___024root.h"
#include <iostream>

class CPU {
public:
  using Regs = VlUnpacked<IData /*31:0*/, 32>;

  CPU(Monitor &monitor);

  void Reset(uint64_t n);

  void Exec(uint64_t n);

  Regs &GetRegs();

private:
  void SingleCycle();

private:
  Monitor &monitor;
  Vtop dut;
};
