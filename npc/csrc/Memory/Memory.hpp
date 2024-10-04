#pragma once

#include "common.hpp"
#include <fstream>
#include <memory>

constexpr size_t MEM_BASE = 0x80000000;
constexpr size_t MEM_SIZE = 0x80000000;
constexpr size_t INITIAL_PC = 0x80000000;

template <size_t Size, size_t Base>
class BaseMemory {
public:
  size_t img_size;

  std::function<void(paddr_t addr, int len, word_t data)> trace_pread;
  std::function<void(paddr_t addr, int len, word_t data)> trace_pwrite;

  BaseMemory() : pmem(new byte[Size]) {}

  void LoadImage(const std::string &path) {
    std::ifstream file(path, std::ios::binary | std::ios::ate);
    if (!file.is_open()) {
      throw string_format("Failed to open image file: %s", path);
    }

    img_size = file.tellg();
    if (img_size > Size) {
      throw std::string("Image file is too big.");
    }

    file.seekg(0, std::ios::beg);
    file.read(reinterpret_cast<char *>(pmem.get()), img_size);
    file.close();
  }

  byte *GuestToHost(paddr_t paddr) { return pmem.get() + paddr - Base; }
  paddr_t HostToGuest(byte *haddr) { return haddr - pmem.get() + Base; }

  word_t PRead(paddr_t addr, int len) {
    if (!IsInPMem(addr)) {
      throw string_format("PRead: addr %#lx out of bound", addr);
    }

    return ReadPMem(addr, len);
  }

  void PWrite(paddr_t addr, int len, word_t data) {
    if (!IsInPMem(addr)) {
      throw string_format("PWrite: addr %#lx out of bound", addr);
    }

    WritePMem(addr, len, data);
  }

  word_t VRead(paddr_t addr, int len) {
    return PRead(addr, len);
  }

  word_t VWrite(paddr_t addr, int len) {
    return PWrite(addr, len);
  }

private:
  word_t ReadHost(void *addr, int len) {
    switch (len) {
    case 1:
      return *(uint8_t *)addr;
    case 2:
      return *(uint16_t *)addr;
    case 4:
      return *(uint32_t *)addr;
    default:
      throw "Read Host: bad len";
      return -1;
    }
  }

  void WriteHost(void *addr, int len, word_t data) {
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
      throw "Write Host: bad len";
    }
  }

  bool IsInPMem(paddr_t addr) {
    return addr - Base < Size;
  }

  word_t ReadPMem(paddr_t addr, int len) {
    word_t data = ReadHost(GuestToHost(addr), len);
    trace_pread(addr, len, data);
    return data;
  }

  void WritePMem(paddr_t addr, int len, word_t data) {
    trace_pwrite(addr, len, data);
    WriteHost(GuestToHost(addr), len, data);
  }

private:
  std::unique_ptr<byte[]> pmem;
};

using Memory = BaseMemory<MEM_SIZE, MEM_BASE>;
