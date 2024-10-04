#include "CPU/CPU.hpp"
#include "Debugger/Debugger.hpp"
#include "Memory/Memory.hpp"
#include "Monitor/Monitor.hpp"
#include "Utils/argparse.hpp"
#include <ctime>
#include <iostream>

Memory mem;
CPU cpu;
Monitor monitor(cpu, mem);
Debugger dbg(cpu, mem, monitor);

int main(int argc, char *argv[]) {
  // Parse Arguments
  argparse::ArgumentParser args("npc");
  args.add_argument("img")
      .help("Image file to execute.");
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
    std::srand(time(nullptr));
    cpu.Reset(10);
    mem.LoadImage(args.get("img"));
    monitor.LoadDiffTestRef(args.get("-d"));
    // Start
    dbg.MainLoop();
  } catch (std::string msg) {
    std::cerr << msg << std::endl;
    return 1;
  }
}
