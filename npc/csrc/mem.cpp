#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#define uint32_t unsigned int

void *img;
long img_size;

void load_img(char *img_file) {
  FILE *fp = fopen(img_file, "rb");
  if (!fp) {
    printf("Can not open '%s'\n", img_file);
    exit(1);
  }

  fseek(fp, 0, SEEK_END);
  img_size = ftell(fp);

  printf("The image is %s, size = %ld\n", img_file, img_size);

  img = malloc(img_size);

  fseek(fp, 0, SEEK_SET);
  int ret = fread(img, img_size, 1, fp);
  assert(ret == 1);

  fclose(fp);
}

extern "C" void pmem_read(u_int32_t pc, u_int32_t *instr) {
  if (pc == 0) {
    *instr = 0;
    return;
  }

  uint32_t img_idx = (pc - 0x80000000) / 4;
  if (img_idx >= img_size / 4) {
    printf("Image read out of bounds\n");
    exit(1);
  }

  *instr = ((u_int32_t *)img)[img_idx];
}
