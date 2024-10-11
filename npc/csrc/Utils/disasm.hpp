// #if defined(__GNUC__) && !defined(__clang__)
// #pragma GCC diagnostic push
// #pragma GCC diagnostic ignored "-Wmaybe-uninitialized"
// #endif

// #include "llvm/MC/MCAsmInfo.h"
// #include "llvm/MC/MCContext.h"
// #include "llvm/MC/MCDisassembler/MCDisassembler.h"
// #include "llvm/MC/MCInstPrinter.h"
// #if LLVM_VERSION_MAJOR >= 14
// #include "llvm/MC/TargetRegistry.h"
// #if LLVM_VERSION_MAJOR >= 15
// #include "llvm/MC/MCSubtargetInfo.h"
// #endif
// #else
// #include "llvm/Support/TargetRegistry.h"
// #endif
// #include "llvm/Support/TargetSelect.h"

// #if defined(__GNUC__) && !defined(__clang__)
// #pragma GCC diagnostic pop
// #endif

// #if LLVM_VERSION_MAJOR < 11
// #error Please use LLVM with major version >= 11
// #endif

// #pragma once

// void init_disasm(const char *triple);

// std::string disassemble(uint64_t pc, uint8_t *code, int nbyte);
