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

#include <common.h>

void init_monitor(int, char *[]);
void am_init_monitor();
void engine_start();
int is_exit_status_bad();

void test_expr();

int main(int argc, char *argv[]) {
  /* Initialize the monitor. */
#ifdef CONFIG_TARGET_AM
  am_init_monitor();
#else
  init_monitor(argc, argv);
#endif

  // test_expr();

  /* Start engine. */
  engine_start();

  void cleanup_sdl();
  cleanup_sdl();
  return is_exit_status_bad();
}

word_t expr(char *e, bool *success);

void test_expr() {
  FILE *fp = fopen("input", "r");
  int passed = 0;

  while (1) {
    word_t std;
    if (fscanf(fp, "%u", &std) != 1) break;
    printf("std: %d\n", std);

    char *e = NULL;
    size_t sz = 0;
    Assert(getline(&e, &sz, fp) != -1, "read expr fail");
    printf("exp: %s\n", e);

    bool success;
    word_t ans = expr(e, &success);
    Assert(success, "Eval fail");
    Assert(ans == std, "Wrong answer: std = %u, ans = %u\n", std, ans);
    printf("Accepted\n");

    passed += 1;
  }

  Log("Passed %d passes.", passed);
}
