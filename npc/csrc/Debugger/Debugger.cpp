#include "Debugger.hpp"

Debugger::Debugger(CPU &cpu, bool batch_mode)
    : cpu(cpu), batch_mode(batch_mode) {
  cmd_table.emplace_back(Command{"help", "Display information about all supported commands", &Debugger::CMD_help});
  cmd_table.emplace_back(Command{"c", "Continue the execution of the program", &Debugger::CMD_c});
  cmd_table.emplace_back(Command{"q", "Exit", &Debugger::CMD_q});
  // {"si", "Step instruction. Usage: si [N](dec/oct/hex)(uint 64)(default N=1)", cmd_si},
  // {"info", "Print register or watchpoint information. Usage: info r/w", cmd_info},
  // {"x", "Scan N*4 bytes from address EXPR. Usage: x N EXPR", cmd_x},
  // {"p", "Print value. Usage: p EXPR", cmd_p},
  // {"w", "Set up watchpoint. Usage: w EXPR", cmd_w},
  // {"d", "Delete watchpoint. Usage: d N", cmd_d},
}

void Debugger::SetBatchMode() {
  batch_mode = true;
}

void Debugger::MainLoop() {
  if (batch_mode) {
    std::string empty;
    CMD_c(empty);
    return;
  }

  std::string input;
  while (true) {
    std::cout << "(npc) ";
    std::getline(std::cin, input);
    if (input.empty()) {
      continue; // 若输入为空则继续
    }

    std::istringstream iss(input);
    std::string cmd;
    iss >> cmd; // 提取第一个 token 作为命令

    // 提取剩余的参数作为 args
    std::string args;
    std::getline(iss, args);

    // 遍历命令表，查找对应命令
    bool found = false;
    for (const auto &command : cmd_table) {
      if (command.name == cmd) {
        if ((this->*command.handler)(args) < 0) {
          return;
        }
        found = true;
        break;
      }
    }

    if (!found) {
      std::cout << "Unknown command '" << cmd << "'\n";
    }
  }
}

int Debugger::CMD_help(std::string &args) {
  /* extract the first argument */
  std::istringstream iss(args);
  std::string arg;
  iss >> arg;

  if (arg.empty()) {
    /* no argument given */
    for (const auto &command : cmd_table) {
      std::cout << command.name << "\t-\t" << command.description << std::endl;
    }
  } else {
    for (const auto &command : cmd_table) {
      if (arg == command.name) {
        std::cout << command.name << "\t-\t" << command.description << std::endl;
        return 0;
      }
    }
    std::cout << "Unknown command " << '\'' << arg << '\'' << std::endl;
  }
  return 0;
}

int Debugger::CMD_c(std::string &args) {
  cpu.Exec(-1);
  return 0;
}

int Debugger::CMD_q(std::string &args) {
  return -1;
}
