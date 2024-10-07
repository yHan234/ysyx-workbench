#include "CPU/CPU.hpp"
#include "Debugger/Debugger.hpp"
#include "Memory/Memory.hpp"
#include "Monitor/Monitor.hpp"
#include "Utils/argparse.hpp"
#include <ctime>
#include <fstream>
#include <iostream>

MemoryManager mem_mgr;
Memory mem(mem_mgr);
CPU cpu;
Monitor monitor(cpu, mem_mgr);
Debugger dbg(cpu, mem_mgr, monitor);

size_t img_size;

void LoadImage(const std::string &path, char *addr, const std::string &ref_so_path) {
  std::ifstream file(path, std::ios::binary | std::ios::ate);
  if (!file.is_open()) {
    throw string_format("Failed to open image file: %s", path);
  }

  img_size = file.tellg();
  if (img_size > MEM_SIZE) {
    throw std::string("Image file is too big.");
  }

  file.seekg(0, std::ios::beg);
  file.read(addr, img_size);
  file.close();

  monitor.LoadDiffTestRef(ref_so_path, addr, img_size);
}

int main(int argc, char *argv[]) {
  // Parse Arguments
  argparse::ArgumentParser args("npc");
  args.add_argument("img")
      .help("Image file to execute.");
  args.add_argument("-b", "--batch")
      .flag()
      .help("run with batch mode");
  args.add_argument("-d", "--diff")
      .default_value("")
      .help("run DiffTest with reference REF_SO");

  try {
    args.parse_args(argc, argv);
  } catch (const std::exception &err) {
    std::cerr << err.what() << std::endl;
    std::cerr << args;
    return 1;
  };

  try {
    // Initialize
    if (args["-b"] == true) {
      dbg.SetBatchMode();
    }

    cpu.Reset(10);

    LoadImage(args.get("img"), mem.mem, args.get("-d"));
    // Start
    dbg.MainLoop();
  } catch (std::string &msg) {
    std::cerr << msg << std::endl;
    return 1;
  }

  return monitor.IsExitStatusBad();
}
