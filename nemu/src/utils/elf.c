#include <debug.h>
#include <elf.h>
#include <fcntl.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

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
    Assert(read(fd, shdr, sizeof(shdr)) == sizeof(shdr), "Failed to read section header");

    if (shdr->sh_type == SHT_SYMTAB) {
      symtab_hdr = (Elf32_Shdr *)malloc(shentsize);
      memcpy(symtab_hdr, shdr, shentsize);
    } else if (shdr->sh_type == SHT_STRTAB && i == ehdr.e_shstrndx) {
      strtab_hdr = (Elf32_Shdr *)malloc(shentsize);
      memcpy(strtab_hdr, shdr, shentsize);
    }
  }

  Assert(symtab_hdr, "Failed to find symbol table");
  Elf32_Sym *symtab = malloc(symtab_hdr->sh_size);
  lseek(fd, symtab_hdr->sh_offset, SEEK_SET); // 定位到符号表的偏移
  Assert(read(fd, symtab, symtab_hdr->sh_size) >= 0, "Failed to read symbol table");

  char *strtab = malloc(strtab_hdr->sh_size);
  lseek(fd, strtab_hdr->sh_offset, SEEK_SET); // 定位到字符串表的偏移
  Assert(read(fd, strtab, strtab_hdr->sh_size) >= 0, "Failed to read string table");

  int symcount = symtab_hdr->sh_size / sizeof(Elf32_Sym);
  for (int i = 0; i < symcount; i++) {
    Elf32_Sym sym = symtab[i];
    const char *name = &strtab[sym.st_name];

    // 判断符号是否为函数
    if (ELF32_ST_TYPE(sym.st_info) == STT_FUNC) {
      printf("Function: %s, Address: 0x%x\n", name, sym.st_value);
    }
  }

  free(symtab);
  free(strtab);
  close(fd);
}
