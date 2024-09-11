#include <Vtop.h>

static Vtop dut;

void *img;
long img_size;

static void load_img(char *img_file) {
  FILE *fp = fopen(img_file, "rb");
  if (!fp) {
    printf("Can not open '%s'", img_file);
    exit(1);
  }

  fseek(fp, 0, SEEK_END);
  img_size = ftell(fp);

  printf("The image is %s, size = %ld", img_file, img_size);

  img = malloc(img_size);

  fseek(fp, 0, SEEK_SET);
  int ret = fread(img, img_size, 1, fp);
  assert(ret == 1);

  fclose(fp);
}

uint32_t pmem_read(uint32_t pc) {
  if (pc == 0)
    return 0;

  uint32_t img_idx = (pc - 0x80000000) / 4;
  if (img_idx >= img_size / 4) {
    printf("image execution is complete\n");
    exit(0);
  }

  return ((uint32_t *)img)[img_idx];
}

static void single_cycle() {
  printf("\n\npc: %#x\n", dut.pc);
  dut.instr = pmem_read(dut.pc);

  dut.clk = 0;
  dut.eval();

  dut.clk = 1;
  dut.eval();
}

static void reset(int n) {
  dut.rst = 1;
  while (n-- > 0) {
    single_cycle();
  }
  dut.rst = 0;
}

int main() {
  load_img("tests/addi.bin");

  reset(10);

  while (true) {
    single_cycle();
  }
}
