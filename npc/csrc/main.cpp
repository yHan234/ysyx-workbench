#include <Vtop.h>

static Vtop dut;

static void single_cycle() {
  printf("\n\n===== clk = 0 =====\n");

  dut.clk = 0;
  dut.eval();

  printf("\n\n===== clk = 1 =====\n");

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

void load_img(char *img_file);

int main() {
  load_img("tests/addi.bin");

  reset(10);

  while (true) {
    single_cycle();
  }
}
