#include "Memory.hpp"

Memory::Memory(size_t size, size_t base)
    : size(size), base(base), pmem(new uint8_t[size]) {
}

size_t Memory::LoadImage(char *path) {
  std::ifstream file("example.bin", std::ios::binary | std::ios::ate);
  if (!file.is_open()) {
    throw std::string("Failed to open image file.");
  }

  std::streampos img_size = file.tellg();
  if (img_size > size) {
    throw std::string("Image file is too big.");
  }

  file.seekg(0, std::ios::beg);
  file.read(reinterpret_cast<char *>(pmem.get()), img_size);
  file.close();

  return img_size;
}

word_t Memory::ReadPMem(paddr_t addr, int len) {
  word_t ret = ReadHost(GuestToHost(addr), len);
  return ret;
}

void Memory::WritePMem(paddr_t addr, int len, word_t data) {
  WriteHost(GuestToHost(addr), len, data);
}

uint8_t *Memory::GuestToHost(paddr_t paddr) { return pmem.get() + paddr - base; }
paddr_t Memory::HostToGuest(uint8_t *haddr) { return haddr - pmem.get() + base; }

word_t Memory::ReadHost(void *addr, int len) {
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

void Memory::WriteHost(void *addr, int len, word_t data) {
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
