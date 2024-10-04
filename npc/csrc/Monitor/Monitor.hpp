#pragma once

#include "CPU/CPU.hpp"
#include "Memory/Memory.hpp"
#include "Utils/CircularBuffer.hpp"
#include "Utils/disasm.hpp"
#include <dlfcn.h>
#include <iostream>

#define BUF_SIZE 32
#define ITRACE
#define DIFFTEST

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

  bool IsExitStatusBad();
  void LoadDiffTestRef(const std::string &file);

private:
  CPU &cpu;
  Memory &mem;

  // Instruction Trace
  void ITrace();
  void PrintITrace();
#ifdef ITRACE
  WriteOnlyCircularBuffer<InstInfo, BUF_SIZE> ibuf;
#endif

  // Differential Test
  void DiffTestStep();
#ifdef DIFFTEST
  enum { REF_TO_DUT,
         DUT_TO_REF };
  void (*DTRefMemCpy)(paddr_t addr, void *buf, size_t n, bool direction) = NULL;
  void (*DTRefRegCpy)(void *dut, bool direction) = NULL;
  void (*DTRefExec)(uint64_t n) = NULL;
  void (*DTRefRaiseIntr)(uint64_t NO) = NULL;
#endif
};
