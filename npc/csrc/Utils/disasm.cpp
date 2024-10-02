// #include <Utils/disasm.hpp>

// static llvm::MCDisassembler *gDisassembler = nullptr;
// static llvm::MCSubtargetInfo *gSTI = nullptr;
// static llvm::MCInstPrinter *gIP = nullptr;

// void init_disasm(const char *triple) {
//   llvm::InitializeAllTargetInfos();
//   llvm::InitializeAllTargetMCs();
//   llvm::InitializeAllAsmParsers();
//   llvm::InitializeAllDisassemblers();

//   std::string errstr;
//   std::string gTriple(triple);

//   llvm::MCInstrInfo *gMII = nullptr;
//   llvm::MCRegisterInfo *gMRI = nullptr;
//   auto target = llvm::TargetRegistry::lookupTarget(gTriple, errstr);
//   if (!target) {
//     llvm::errs() << "Can't find target for " << gTriple << ": " << errstr << "\n";
//     assert(0);
//   }

//   llvm::MCTargetOptions MCOptions;
//   gSTI = target->createMCSubtargetInfo(gTriple, "", "");
//   std::string isa = target->getName();
//   if (isa == "riscv32" || isa == "riscv64") {
//     gSTI->ApplyFeatureFlag("+m");
//     gSTI->ApplyFeatureFlag("+a");
//     gSTI->ApplyFeatureFlag("+c");
//     gSTI->ApplyFeatureFlag("+f");
//     gSTI->ApplyFeatureFlag("+d");
//   }
//   gMII = target->createMCInstrInfo();
//   gMRI = target->createMCRegInfo(gTriple);
//   auto AsmInfo = target->createMCAsmInfo(*gMRI, gTriple, MCOptions);
// #if LLVM_VERSION_MAJOR >= 13
//   auto llvmTripleTwine = llvm::Twine(triple);
//   auto llvmtriple = llvm::Triple(llvmTripleTwine);
//   auto Ctx = new llvm::MCContext(llvmtriple, AsmInfo, gMRI, nullptr);
// #else
//   auto Ctx = new llvm::MCContext(AsmInfo, gMRI, nullptr);
// #endif
//   gDisassembler = target->createMCDisassembler(*gSTI, *Ctx);
//   gIP = target->createMCInstPrinter(llvm::Triple(gTriple),
//                                     AsmInfo->getAssemblerDialect(), *AsmInfo, *gMII, *gMRI);
//   gIP->setPrintImmHex(true);
//   gIP->setPrintBranchImmAsAddress(true);
//   if (isa == "riscv32" || isa == "riscv64")
//     gIP->applyTargetSpecificCLOption("no-aliases");
// }

// std::string disassemble(uint64_t pc, uint8_t *code, int nbyte) {
//   llvm::MCInst inst;
//   llvm::ArrayRef<uint8_t> arr(code, nbyte);
//   uint64_t dummy_size = 0;
//   gDisassembler->getInstruction(inst, dummy_size, arr, pc, llvm::nulls());

//   std::string s;
//   llvm::raw_string_ostream os(s);
//   gIP->printInst(&inst, pc, "", *gSTI, os);

//   int skip = s.find_first_not_of('\t');
//   return s.substr(skip);
// }
