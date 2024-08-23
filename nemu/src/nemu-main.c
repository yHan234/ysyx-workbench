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

  test_expr();

  /* Start engine. */
  engine_start();

  return is_exit_status_bad();
}

word_t expr(char *e, bool *success);

void test_expr() {
  // FILE *fp = popen("tools/gen-expr/build/gen-expr", "r");

  while (1) {
    word_t std;
    Assert(scanf("%u", &std) == 1, "read std fail");
    printf("std: %d\n", std);

    char *e;
    size_t sz;
    Assert(getline(&e, &sz, stdin) != -1, "read expr fail");
    printf("exp: %s\n", e);

    bool success = false;
    word_t ans = expr(e, &success);

    Assert(ans == std, "Wrong answer: std = %u, ans = %u\n", std, ans);
    printf("Accepted\n");
  }
}
