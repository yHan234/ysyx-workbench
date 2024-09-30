#include "CPU/CPU.hpp"
#include "Debugger/Debugger.hpp"
#include "Memory/Memory.hpp"
#include <ctime>
#include <iostream>

constexpr size_t MEM_SIZE = 0x80000000;
constexpr size_t MEM_BASE = 0x80000000;
Memory<MEM_SIZE, MEM_BASE> mem;

CPU cpu;

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

  Debugger dbg(cpu);

  try {
    dbg.MainLoop();
  } catch (std::string msg) {
    std::cerr << msg << std::endl;
    return 1;
  }
}
