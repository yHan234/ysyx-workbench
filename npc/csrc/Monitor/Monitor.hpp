#pragma once

#include "CPU/CPU.hpp"
#include "MemoryManager/MemoryManager.hpp"
#include "Utils/CircularBuffer.hpp"
#include "Utils/disasm.hpp"
#include <dlfcn.h>
#include <iostream>

#define BUF_SIZE 128
#define ITRACE
#define MTRACE
// #define DIFFTEST

struct InstInfo {
  vaddr_t pc;
  uint32_t inst;
  std::string disasm;

  std::string ToString();
};

struct MemInfo {
  bool is_write;
  vaddr_t pc;
  vaddr_t addr;
  int len;
  word_t data;
  word_t w_pre_data;

  std::string ToString();
};

class Monitor {
public:
  Monitor(CPU &cpu, MemoryManager &mem_mgr);

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
  void LoadDiffTestRef(const std::string &ref_so_file, char *img_addr, size_t img_size);

private:
  CPU &cpu;
  MemoryManager &mem_mgr;

  paddr_t pc;
  word_t inst;

  // Instruction Trace
  void ITrace();
  void PrintITrace();
#ifdef ITRACE
  WriteOnlyCircularBuffer<InstInfo, BUF_SIZE> ibuf;
#endif

  // Memory Trace
  void MTrace(bool is_write, vaddr_t addr, int len, word_t data, word_t w_pre_data);
  void PrintMTrace();
#ifdef MTRACE
  WriteOnlyCircularBuffer<MemInfo, BUF_SIZE> mbuf;
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
