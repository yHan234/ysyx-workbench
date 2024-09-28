#include "CPU/CPU.hpp"
#include "Memory/Memory.hpp"
#include <ctime>
#include <iostream>

constexpr size_t MEM_SIZE = 0x80000000;
constexpr size_t MEM_BASE = 0x80000000;
Memory mem(MEM_SIZE, MEM_BASE);

char *img_file;
size_t img_size;

void parse_args(int argc, char *argv[]) {
  img_file = argv[1];
}

extern "C" void pmem_read(u_int32_t pc, u_int32_t *instr) {
  if (pc == 0) {
    *instr = 0;
    return;
  }
  *instr = mem.ReadPMem(pc, 4);
}

int main(int argc, char *argv[]) {
  std::srand(time(nullptr));

  img_size = mem.LoadImage(img_file);

  CPU cpu;
  cpu.Reset(10);
  try {
    cpu.Exec(-1);
  } catch (std::string msg) {
    std::cout << msg << std::endl;
  }
}
