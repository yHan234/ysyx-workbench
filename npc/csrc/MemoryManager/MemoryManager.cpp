#include "MemoryManager.hpp"

void MemoryManager::Map(paddr_t begin, paddr_t end, callback_t callback, byte *host_begin) {
  if (CheckOverlap(begin, end)) {
    throw string_format("MemoryManager::Map addr overlap [%lx, %lx]", begin, end);
  }
  maps.push_back({begin, end, callback, host_begin});
}

void MemoryManager::PAddrWrite(paddr_t paddr, int len, word_t data) {
  for (const auto &[b, e, callback, hbegin] : maps) {
    if (paddr < b || e <= paddr) {
      continue;
    }
    uint32_t offset = paddr - b;
    callback(offset, len, true);
    HostWrite(hbegin + offset, len, data);
    return;
  }
  throw string_format("PAddrWrite: addr %#lx out of bound", paddr);
}

word_t MemoryManager::PAddrRead(paddr_t paddr, int len) {
  for (const auto &[b, e, callback, hbegin] : maps) {
    if (paddr < b || e <= paddr) {
      continue;
    }
    uint32_t offset = paddr - b;
    callback(offset, len, true);
    word_t data = HostRead(hbegin + offset, len);
    return data;
  }
  throw string_format("PAddrRead: addr %#lx out of bound", paddr);
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
    return -1;
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
