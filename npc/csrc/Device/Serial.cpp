#include "Serial.hpp"
#include <iostream>

Serial::Serial(MemoryManager &mem_mgr) {
  mem = new byte[SIZE];
  mem_mgr.Map(SERIAL_ADDR, SERIAL_ADDR + SIZE, [&](uint32_t offset, int len, bool is_write) { Handle(offset, len, is_write); }, mem);
}

Serial::~Serial() {
  delete[] mem;
}

void Serial::Handle(uint32_t offset, int len, bool is_write) {
  if (offset != 0 || len != 1 || is_write != true) {
    throw std::string("Serial::Handle panic");
  }
  std::cout << *mem;
}
