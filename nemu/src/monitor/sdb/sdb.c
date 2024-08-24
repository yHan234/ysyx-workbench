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

#include <errno.h>

#include <isa.h>
#include <cpu/cpu.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <memory/vaddr.h>
#include "sdb.h"

static int is_batch_mode = false;

void init_regex();
void init_wp_pool();

/* We use the `readline' library to provide more flexibility to read from stdin. */
static char* rl_gets() {
  static char *line_read = NULL;

  if (line_read) {
    free(line_read);
    line_read = NULL;
  }

  line_read = readline("(nemu) ");

  if (line_read && *line_read) {
    add_history(line_read);
  }

  return line_read;
}

static int cmd_c(char *args) {
  cpu_exec(-1);
  return 0;
}

static int cmd_q(char *args) {
  nemu_state.state = NEMU_QUIT;
  return -1;
}

static int cmd_si(char *args) {
  char *arg = strtok(NULL, " ");
  uint64_t  num_steps;
  if (!arg) {
    num_steps = 1;
  } else {
    num_steps = strtoul(arg, NULL, 0);
    if (errno == ERANGE) {
      errno = 0;
      printf("si: too large\n");
      return 0;
    }
  }
  printf("step %lu instruction(s)\n", num_steps);
  cpu_exec(num_steps);
  return 0;
}

static int cmd_info(char *args) {
  char *arg = strtok(NULL, " ");
  if (strlen(arg) != 1) {
    puts("Usage: info r/w");
  } else if (arg[0] == 'r') {
    isa_reg_display();
  } else if (arg[0] == 'w') {
    print_watchpoints();
  } else {
    puts("Usage: info r/w");
  }
  return 0;
}

static int cmd_x(char *args) {
  char *arg_n = strtok(NULL, " ");
  char *arg_expr = strtok(NULL, "\0");
  char *endptr = NULL;

  uint64_t n = strtoul(arg_n, &endptr, 0);
  if (*endptr) {
    puts("command x: Invalid arg N");
    return 0;
  }

  bool success = false;
  word_t e = expr(arg_expr, &success);
  if (!success) {
    puts("command x: Invalid expression.");
    return 0;
  }

  for (uint64_t i = 0; i < n; ++i) {
    vaddr_t addr = e + i * 4;
    printf("0x%08x\t\t0x%08x\n", addr, vaddr_read(addr, 4));
  }

  return 0;
}

static int cmd_p(char *args) {
  char *arg_expr = strtok(NULL, "\0");

  bool success = false;
  word_t e = expr(arg_expr, &success);
  if (!success) {
    puts("command p: Invalid expression.");
    return 0;
  }
  printf("%u\n", e);

  return 0;
}

static int cmd_w(char *args) {
  char *arg_expr = strtok(NULL, "\0");
  bool success = false;
  expr(arg_expr, &success);
  if (!success) {
    puts("command w: Invalid EXPR.");
  } else if (!set_wp(arg_expr)) {
    puts("command w: Too much watchpoints(32).");
  }
  return 0;
}

static int cmd_d(char *args) {
  char *arg_n = strtok(NULL, " ");
  uint64_t n = strtouq(arg_n, NULL, 0);
  if (n > 32) {
    puts("command d: Watchpoint numbers range from 1 to 32.");
  } else if (!del_wp(n)) {
    printf("command d: Watchpoint %lu is free\n", n);
  }
  return 0;
}

static int cmd_help(char *args);

static struct {
  const char *name;
  const char *description;
  int (*handler) (char *);
} cmd_table [] = {
  { "help", "Display information about all supported commands", cmd_help },
  { "c", "Continue the execution of the program", cmd_c },
  { "q", "Exit NEMU", cmd_q },
  { "si", "Step instruction. Usage: si [N](dec/oct/hex)(uint 64)(default N=1)", cmd_si },
  { "info", "Print register or watchpoint information. Usage: info r/w", cmd_info },
  { "x", "Scan N*4 bytes from address EXPR. Usage: x N EXPR", cmd_x },
  { "p", "Print value. Usage: p EXPR", cmd_p},
  { "w", "Set up watchpoint. Usage: w EXPR", cmd_w},
  { "d", "Delete watchpoint. Usage: d N", cmd_d},
};

#define NR_CMD ARRLEN(cmd_table)

static int cmd_help(char *args) {
  /* extract the first argument */
  char *arg = strtok(NULL, " ");
  int i;

  if (arg == NULL) {
    /* no argument given */
    for (i = 0; i < NR_CMD; i ++) {
      printf("%s - %s\n", cmd_table[i].name, cmd_table[i].description);
    }
  }
  else {
    for (i = 0; i < NR_CMD; i ++) {
      if (strcmp(arg, cmd_table[i].name) == 0) {
        printf("%s - %s\n", cmd_table[i].name, cmd_table[i].description);
        return 0;
      }
    }
    printf("Unknown command '%s'\n", arg);
  }
  return 0;
}

void sdb_set_batch_mode() {
  is_batch_mode = true;
}

void sdb_mainloop() {
  if (is_batch_mode) {
    cmd_c(NULL);
    return;
  }

  for (char *str; (str = rl_gets()) != NULL; ) {
    char *str_end = str + strlen(str);

    /* extract the first token as the command */
    char *cmd = strtok(str, " ");
    if (cmd == NULL) { continue; }

    /* treat the remaining string as the arguments,
     * which may need further parsing
     */
    char *args = cmd + strlen(cmd) + 1;
    if (args >= str_end) {
      args = NULL;
    }

#ifdef CONFIG_DEVICE
    extern void sdl_clear_event_queue();
    sdl_clear_event_queue();
#endif

    int i;
    for (i = 0; i < NR_CMD; i ++) {
      if (strcmp(cmd, cmd_table[i].name) == 0) {
        if (cmd_table[i].handler(args) < 0) { return; }
        break;
      }
    }

    if (i == NR_CMD) { printf("Unknown command '%s'\n", cmd); }
  }
}

void init_sdb() {
  /* Compile the regular expressions. */
  init_regex();

  /* Initialize the watchpoint pool. */
  init_wp_pool();
}
