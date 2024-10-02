#include "CPU/CPU.hpp"
#include "Debugger/Debugger.hpp"
#include "Memory/Memory.hpp"
#include "Monitor/Monitor.hpp"
#include <ctime>
#include <iostream>

Memory mem;
CPU cpu;
Monitor monitor(cpu, mem);
Debugger dbg(cpu, mem, monitor);

char *img_file;
size_t img_size;

void parse_args(int argc, char *argv[]) {
  img_file = argv[1];
}

int main(int argc, char *argv[]) {
  std::srand(time(nullptr));
  parse_args(argc, argv);

  mem.LoadImage(img_file);
  cpu.Reset(10);

  try {
    dbg.MainLoop();
  } catch (std::string msg) {
    std::cerr << msg << std::endl;
    return 1;
  }
}
