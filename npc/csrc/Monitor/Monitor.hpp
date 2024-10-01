#pragma once

#include "Utils/CircularBuffer.hpp"
#include "Utils/disasm.hpp"
#include <iostream>
#include <type_traits>

#define BUF_SIZE 128
#define ITRACE

struct InstInfo {
  vaddr_t pc;
  uint32_t inst;
  std::string disasm;

  std::string ToString();
};

class Monitor {
public:
  Monitor()
      : state(State::STOP) {
    init_disasm("riscv32");
  }

  // State
  enum class State {
    RUNNING,
    STOP,
    END,
    ABORT,
    QUIT,
  };

  State state;
  int ret; // valid when state == END

  // Instruction Trace
public:
  void ITrace(vaddr_t pc, uint32_t inst);

  void PrintITrace();

private:
#ifdef ITRACE
  WriteOnlyCircularBuffer<InstInfo, BUF_SIZE> ibuf;
#endif
};
