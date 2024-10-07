#pragma once

#include "MemoryManager/MemoryManager.hpp"
#include "common.hpp"
#include <chrono>

class Timer {
public:
  Timer(MemoryManager &mem_mgr);
  ~Timer();

  void HandleUptime(uint32_t offset, int len, bool is_write);

private:
  static constexpr size_t UPTIME_SIZE = 8;
  byte *uptime_mem;

  using time_point = std::chrono::time_point<std::chrono::high_resolution_clock>;
  time_point boot_time;
};
