/***************************************************************************************
* Copyright (c) 2014-2022 Zihao Yu, Nanjing University
*
* NEMU is licensed under Mulan PSL v2.
* You can use this software according to the terms and conditions of the Mulan PSL v2.
* You may obtain a copy of Mulan PSL v2 at:
*          http://license.coscl.org.cn/MulanPSL2
*
* THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND,
* EITHER EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT,
* MERCHANTABILITY OR FIT FOR A PARTICULAR PURPOSE.
*
* See the Mulan PSL v2 for more details.
***************************************************************************************/

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <assert.h>
#include <string.h>

// this should be enough
static char buf[65536] = {};
static char code_buf[65536 + 128] = {}; // a little larger than `buf`
static char *code_format =
"#include <stdio.h>\n"
"int main() { "
"  unsigned result = %s; "
"  printf(\"%%u\", result); "
"  return 0; "
"}";

static uint32_t buf_p = 0;

void gen(char c) {
  if (buf_p == 65535) return;
  buf[buf_p++] = c;
}

void gen_num() {
  for (int w = 0; w < rand() % 7 + 1; ++w) {
    if (w == 0) gen('1' + rand() % 9);
    else gen('0' + rand() % 10);
  }
  if (rand() % 2) gen(' ');
}

void gen_rand_bop() {
  static char *ops = "+-*/";
  gen(ops[rand() % 4]);
  if (rand() % 2) gen(' ');
}

void gen_rand_uop() {
  static char *ops = "+-";
  gen(ops[rand() % 2]);
  if (rand() % 2) gen(' ');
}

static void gen_rand_expr() {
  switch (rand() % 3) {
    case 0: gen_num(); break;
    case 1: gen('('); gen_rand_expr(); gen(')'); break;
    case 2: gen_rand_expr(); gen_rand_bop(); gen_rand_expr(); break;
    case 3: gen_rand_uop(); gen_rand_expr(); break;
  }
}

int main(int argc, char *argv[]) {
  int seed = time(0);
  srand(seed);
  int loop = 1;
  if (argc > 1) {
    sscanf(argv[1], "%d", &loop);
  }
  int i;
  for (i = 0; i < loop; i ++) {
    buf_p = 0;
    gen_rand_expr();
    buf[buf_p] = '\0';

    sprintf(code_buf, code_format, buf);

    FILE *fp = fopen("/tmp/.code.c", "w");
    assert(fp != NULL);
    fputs(code_buf, fp);
    fclose(fp);

    int ret = system("gcc /tmp/.code.c -o /tmp/.expr > /tmp/gen-expr.log 2>&1");
    if (ret != 0) continue;

    fp = fopen("/tmp/gen-expr.log", "r");
    fseek(fp, 0, SEEK_END);
    if (ftell(fp) != 0) {
      i--;
      continue;
    }

    fp = popen("/tmp/.expr", "r");
    assert(fp != NULL);

    int result;
    ret = fscanf(fp, "%d", &result);
    pclose(fp);

    printf("%u %s\n", result, buf);
  }
  return 0;
}
