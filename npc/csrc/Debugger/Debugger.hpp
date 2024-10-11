#pragma once

#include "CPU/CPU.hpp"
#include "MemoryManager/MemoryManager.hpp"
#include "Monitor/Monitor.hpp"
#include <iostream>
#include <regex>
#include <vector>

class Debugger {
public:
  Debugger(CPU &cpu, MemoryManager &mem_mgr, Monitor &monitor, bool batch_mode = false);
  void SetBatchMode();
  void MainLoop();

private:
  int CMD_help(std::string &args);
  int CMD_c(std::string &args);
  int CMD_q(std::string &args);
  int CMD_si(std::string &args);
  int CMD_info(std::string &args);
  int CMD_x(std::string &args);

private:
  struct Command {
    const char *name;
    const char *description;
    int (Debugger::*handler)(std::string &);

    Command(const char *name,
            const char *description,
            int (Debugger::*handler)(std::string &));
  };
  std::vector<Command> cmd_table;

  CPU &cpu;
  MemoryManager &mem_mgr;
  Monitor &monitor;
  bool batch_mode;
};
