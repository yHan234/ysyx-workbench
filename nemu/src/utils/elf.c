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

  Elf64_Ehdr ehdr;
  Assert(read(fd, &ehdr, sizeof(ehdr)) == sizeof(ehdr), "Failed to read ELF header");
  Assert(memcmp(ehdr.e_ident, ELFMAG, SELFMAG) == 0, "Not a valid ELF file");

  // 寻找符号表和字符串表的节
  Elf64_Half shentsize = ehdr.e_shentsize;
  printf("%d", shentsize);
  fflush(stdout);
  Elf64_Shdr *shdr = (Elf64_Shdr *)malloc(shentsize);
  lseek(fd, ehdr.e_shoff, SEEK_SET);
  Elf64_Shdr *symtab_hdr = (Elf64_Shdr *)malloc(shentsize);
  Elf64_Shdr *strtab_hdr = (Elf64_Shdr *)malloc(shentsize);

  for (int i = 0; i < ehdr.e_shnum; i++) {
    Assert(read(fd, shdr, sizeof(shdr)) == sizeof(shdr), "Failed to read section header");

    if (shdr->sh_type == SHT_SYMTAB) {
      memcpy(symtab_hdr, shdr, shentsize);
    } else if (shdr->sh_type == SHT_STRTAB && i == ehdr.e_shstrndx) {
      memcpy(strtab_hdr, shdr, shentsize);
    }
  }

  Elf64_Sym *symtab = malloc(symtab_hdr->sh_size);
  lseek(fd, symtab_hdr->sh_offset, SEEK_SET); // 定位到符号表的偏移
  Assert(read(fd, symtab, symtab_hdr->sh_size) > 0, "Failed to read symbol table");

  char *strtab = malloc(strtab_hdr->sh_size);
  lseek(fd, strtab_hdr->sh_offset, SEEK_SET); // 定位到字符串表的偏移
  Assert(read(fd, strtab, strtab_hdr->sh_size) > 0, "Failed to read string table");

  int symcount = symtab_hdr->sh_size / sizeof(Elf64_Sym);
  for (int i = 0; i < symcount; i++) {
    Elf64_Sym sym = symtab[i];
    const char *name = &strtab[sym.st_name];

    // 判断符号是否为函数
    if (ELF64_ST_TYPE(sym.st_info) == STT_FUNC) {
      printf("Function: %s, Address: 0x%lx\n", name, sym.st_value);
    }
  }

  free(symtab);
  free(strtab);
  close(fd);
}
