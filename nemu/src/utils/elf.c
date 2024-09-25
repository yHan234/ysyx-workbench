#include <common.h>
#include <debug.h>
#include <elf.h>
#include <fcntl.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

char *strtab;

typedef struct {
  vaddr_t addr;
  char *name;
} Function;
uint num_functions;
Function *functions;

void init_elf(const char *elf_file) {
  if (elf_file == NULL) {
    return;
  }

  int fd = open(elf_file, O_RDONLY);
  Assert(fd >= 0, "Failed to open ELF file");

  Elf32_Ehdr ehdr;
  Assert(read(fd, &ehdr, sizeof(ehdr)) == sizeof(ehdr), "Failed to read ELF header");
  Assert(memcmp(ehdr.e_ident, ELFMAG, SELFMAG) == 0, "Not a valid ELF file");

  // 寻找符号表和字符串表的节
  Elf32_Half shentsize = ehdr.e_shentsize;
  Elf32_Shdr *shdr = malloc(shentsize);
  Elf32_Shdr *symtab_hdr = NULL;
  Elf32_Shdr *strtab_hdr = NULL;

  lseek(fd, ehdr.e_shoff, SEEK_SET);
  for (int i = 0; i < ehdr.e_shnum; i++) {
    Assert(read(fd, shdr, shentsize) == shentsize, "Failed to read section header");

    if (shdr->sh_type == SHT_SYMTAB) {
      symtab_hdr = malloc(shentsize);
      memcpy(symtab_hdr, shdr, shentsize);
    } else if (shdr->sh_type == SHT_STRTAB && i != ehdr.e_shstrndx) {
      strtab_hdr = malloc(shentsize);
      memcpy(strtab_hdr, shdr, shentsize);
    }
  }

  Assert(symtab_hdr, "Failed to find symbol table");
  Elf32_Sym *symtab = malloc(symtab_hdr->sh_size);
  lseek(fd, symtab_hdr->sh_offset, SEEK_SET);
  Assert(read(fd, symtab, symtab_hdr->sh_size) == symtab_hdr->sh_size, "Failed to read symbol table");

  Assert(strtab_hdr, "Failed to find string table");
  strtab = malloc(strtab_hdr->sh_size);
  printf("strtab offset %d size %d\n", strtab_hdr->sh_offset, strtab_hdr->sh_size);
  lseek(fd, strtab_hdr->sh_offset, SEEK_SET);
  Assert(read(fd, strtab, strtab_hdr->sh_size) == strtab_hdr->sh_size, "Failed to read string table");

  // 寻找符号表中的函数
  int sym_cnt = symtab_hdr->sh_size / sizeof(Elf32_Sym);
  num_functions = 0;
  for (int i = 0; i < sym_cnt; i++) {
    num_functions += ELF32_ST_TYPE(symtab[i].st_info) == STT_FUNC;
  }

  functions = malloc(sizeof(Function) * num_functions);
  for (int i = 0, func_idx = 0; i < sym_cnt; i++) {
    Elf32_Sym sym = symtab[i];

    if (ELF32_ST_TYPE(sym.st_info) == STT_FUNC) {
      functions[func_idx].name = &strtab[sym.st_name];
      functions[func_idx].addr = sym.st_info;
      func_idx += 1;
    }
  }

  free(shdr);
  free(symtab_hdr);
  free(strtab_hdr);
  free(symtab);
  close(fd);
}
