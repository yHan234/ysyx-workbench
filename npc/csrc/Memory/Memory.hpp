#pragma once

#include "common.hpp"
#include <fstream>
#include <memory>

class Memory {
public:
  Memory(size_t size, size_t base);

  size_t LoadImage(char *path);

  word_t ReadPMem(paddr_t addr, int len);

  void WritePMem(paddr_t addr, int len, word_t data);

private:
  uint8_t *GuestToHost(paddr_t paddr);
  paddr_t HostToGuest(uint8_t *haddr);

  word_t ReadHost(void *addr, int len);

  void WriteHost(void *addr, int len, word_t data);

private:
  size_t size;
  size_t base;
  std::unique_ptr<uint8_t[]> pmem;
};
