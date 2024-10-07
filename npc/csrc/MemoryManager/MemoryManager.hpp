#pragma once

#include "common.hpp"
#include <functional>
#include <vector>

class MemoryManager {
public:
  using callback_t = std::function<void(uint32_t offset, int len, bool is_write)>;

  struct MapEntry {
    paddr_t begin;
    paddr_t end;
    callback_t callback;
    byte *host_begin;
  };

  std::function<void(paddr_t addr, int len, word_t data)> trace_read;
  std::function<void(paddr_t addr, int len, word_t data)> trace_write;

public:
  void Map(paddr_t begin, paddr_t end, callback_t callback, byte *host_begin);

  void PAddrWrite(paddr_t paddr, int len, word_t data);

  word_t PAddrRead(paddr_t paddr, int len);

private:
  bool CheckOverlap(paddr_t begin, paddr_t end);

  word_t HostRead(void *addr, int len);

  void HostWrite(void *addr, int len, word_t data);

private:
  std::vector<MapEntry> maps;
};
