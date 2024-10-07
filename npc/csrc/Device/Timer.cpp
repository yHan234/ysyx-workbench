#include "Timer.hpp"

Timer::Timer(MemoryManager &mem_mgr)
    : boot_time(std::chrono::high_resolution_clock::now()) {
  uptime_mem = new byte[UPTIME_SIZE];
  mem_mgr.Map(UPTIME_ADDR, UPTIME_ADDR + UPTIME_SIZE, [&](uint32_t offset, int len, bool is_write) { HandleUptime(offset, len, is_write); }, uptime_mem);
}

Timer::~Timer() {
  delete[] uptime_mem;
}
#include <iostream>
void Timer::HandleUptime(uint32_t offset, int len, bool is_write) {
  if (!is_write) {
    time_point now = std::chrono::high_resolution_clock::now();
    int64_t duration = std::chrono::duration_cast<std::chrono::microseconds>(now - boot_time).count();
    *reinterpret_cast<int64_t *>(uptime_mem) = duration;
    std::cout << *reinterpret_cast<int64_t *>(uptime_mem) << std::endl;
  }
}
