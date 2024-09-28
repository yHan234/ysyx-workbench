#pragma once

#include "Memory/Memory.hpp"
#include "Vtop.h"

class CPU {

public:
  void SingleCycle();

  void Reset(int n);

  void Exec(uint64_t n);

private:
  Vtop dut;
};
