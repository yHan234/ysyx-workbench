#pragma once

#include "CPU/CPU.hpp"
#include "Memory/Memory.hpp"
#include "Utils/CircularBuffer.hpp"
#include "Utils/disasm.hpp"
#include <iostream>

#define BUF_SIZE 32
#define ITRACE

struct InstInfo {
  vaddr_t pc;
  uint32_t inst;
  std::string disasm;

  std::string ToString();
};

class Monitor {
public:
  Monitor(CPU &cpu, Memory &mem);

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

private:
  CPU &cpu;
  Memory &mem;

  // Instruction Trace
  void ITrace();
  void PrintITrace();
#ifdef ITRACE
  WriteOnlyCircularBuffer<InstInfo, BUF_SIZE> ibuf;
#endif
};
