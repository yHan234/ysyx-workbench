#include "Debugger.hpp"

#define TRY_STRING_TO_UL(str, res, name)         \
  do {                                           \
    try {                                        \
      res = std::stoul(str, nullptr, 0);         \
    } catch (std::invalid_argument const &ex) {  \
      std::cout << "invalid " name << std::endl; \
      return 0;                                  \
    } catch (std::out_of_range const &ex) {      \
      std::cout << "too big " name << std::endl; \
      return 0;                                  \
    }                                            \
  } while (0)

Debugger::Command::Command(const char *name,
        const char *description,
        int (Debugger::*handler)(std::string &))
    : name(name), description(description), handler(handler) {}

Debugger::Debugger(CPU &cpu, MemoryManager &mem_mgr, Monitor &monitor, bool batch_mode)
    : cpu(cpu), mem_mgr(mem_mgr), monitor(monitor), batch_mode(batch_mode) {
  cmd_table.emplace_back("help", "Display information about all supported commands", &Debugger::CMD_help);
  cmd_table.emplace_back("c", "Continue the execution of the program", &Debugger::CMD_c);
  cmd_table.emplace_back("q", "Exit", &Debugger::CMD_q);
  cmd_table.emplace_back("si", "Step instruction. Usage: si [N](dec/oct/hex)(uint 64)(default N=1)", &Debugger::CMD_si);
  cmd_table.emplace_back("info", "Print register information. Usage: info r", &Debugger::CMD_info);
  cmd_table.emplace_back("x", "Scan N*4 bytes from address. Usage: x N ADDR", &Debugger::CMD_x);

  // TODO:
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
    std::cout << "> ";
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
      std::cout << "Unknown command '" << cmd << "'" << std::endl;
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
  monitor.state = Monitor::State::QUIT;
  return -1;
}

int Debugger::CMD_si(std::string &args) {
  std::istringstream iss(args);
  std::string arg;
  iss >> arg;

  unsigned long num_steps = 1;
  if (!arg.empty()) {
    TRY_STRING_TO_UL(arg, num_steps, "N");
  }

  cpu.Exec(num_steps);

  return 0;
}

int Debugger::CMD_info(std::string &args) {
  std::istringstream iss(args);
  std::string arg;
  iss >> arg;

  if (arg == "r") {
    std::cout << cpu.GetRegs().to_string() << std::endl;
  } else {
    std::cout << "Usage: info r" << std::endl;
  }

  return 0;
}

int Debugger::CMD_x(std::string &args) {
  std::istringstream iss(args);
  std::string n_s, base_s;
  iss >> n_s >> base_s;

  unsigned long n, base;
  TRY_STRING_TO_UL(n_s, n, "N");
  TRY_STRING_TO_UL(base_s, base, "ADDR");

  for (unsigned long i = 0; i < n; ++i) {
    vaddr_t addr = base + i * 4;
    try {
      std::cout << string_format("0x%08x\t\t0x%08x", addr, mem_mgr.PAddrRead(addr, 4)) << std::endl;
    } catch (std::string &msg) {
      std::cout << msg << std::endl;
      return 0;
    }
  }

  return 0;
}
