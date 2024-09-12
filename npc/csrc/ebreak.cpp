#include <stdio.h>
#include <stdlib.h>

extern "C" void ebreak() {
  printf("ebreak\n");
  exit(0);
}
