#include "MemoryManager.hpp"

void MemoryManager::Map(paddr_t begin, paddr_t end, callback_t callback, byte *host_begin) {
  if (CheckOverlap(begin, end)) {
    throw string_format("MemoryManager::Map addr overlap [%lx, %lx]", begin, end);
  }
  maps.push_back({begin, end, callback, host_begin});
}

void MemoryManager::PAddrWrite(paddr_t addr, int len, word_t data) {
  bool succ = false; // for trace
  word_t pre_data;   // for trace
  for (const auto &[b, e, callback, hbegin] : maps) {
    if (addr < b || e <= addr) {
      continue;
    }
    succ = true; // 假设 HostRead HostWrite 不会有问题
    uint32_t offset = addr - b;
    callback(offset, len, true);
    pre_data = HostRead(hbegin + offset, len);
    printf("read %d %d\n", len, pre_data);
    HostWrite(hbegin + offset, len, data);
    break;
  }
  trace_write(succ, addr, len, data, pre_data);
}

word_t MemoryManager::PAddrRead(paddr_t addr, int len) {
  bool succ = false; // for trace
  word_t data;       // for trace
  for (const auto &[b, e, callback, hbegin] : maps) {
    if (addr < b || e <= addr) {
      continue;
    }
    succ = true; // 假设 HostRead 不会有问题
    uint32_t offset = addr - b;
    callback(offset, len, true);
    data = HostRead(hbegin + offset, len);
  }
  trace_read(succ, addr, len, data);
  return data;
}

bool MemoryManager::CheckOverlap(paddr_t begin, paddr_t end) {
  for (const auto &[b, e, _, __] : maps) {
    if (b <= end && begin <= e) {
      return true;
    }
  }
  return false;
}

word_t MemoryManager::HostRead(void *addr, int len) {
  switch (len) {
  case 1:
    return *(uint8_t *)addr;
  case 2:
    return *(uint16_t *)addr;
  case 4:
    return *(uint32_t *)addr;
  default:
    throw "HostRead: bad len";
  }
}

void MemoryManager::HostWrite(void *addr, int len, word_t data) {
  switch (len) {
  case 1:
    *(uint8_t *)addr = data;
    return;
  case 2:
    *(uint16_t *)addr = data;
    return;
  case 4:
    *(uint32_t *)addr = data;
    return;
  default:
    throw std::string("HostWrite: bad len");
  }
}
