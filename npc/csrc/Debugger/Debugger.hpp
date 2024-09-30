#pragma once

#include "CPU/CPU.hpp"
#include "ExprToken.hpp"
#include <iostream>
#include <regex>
#include <vector>

class Debugger {
public:
  Debugger(CPU &cpu, bool batch_mode = false);
  void SetBatchMode();
  void MainLoop();

private:
  int CMD_help(std::string &args);
  int CMD_c(std::string &args);

private:
  struct Command {
    const char *name;
    const char *description;
    int (Debugger::*handler)(std::string &);
  };
  std::vector<Command> cmd_table;

  CPU &cpu;
  bool batch_mode;
};
