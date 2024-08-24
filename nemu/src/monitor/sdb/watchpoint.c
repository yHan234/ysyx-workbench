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

#include "sdb.h"

#define NR_WP 32
#define EXPR_LEN 65536

typedef struct watchpoint {
  int NO;
  struct watchpoint *prev;
  struct watchpoint *next;

  bool is_free;
  char expr[EXPR_LEN];

  word_t val;
  bool val_err;
} WP;

static WP wp_pool[NR_WP] = {};
static WP *head = NULL, *free_ = NULL;

void init_wp_pool() {
  int i;
  for (i = 0; i < NR_WP; i ++) {
    wp_pool[i].NO = i + 1;
    wp_pool[i].prev = (i == 0 ? NULL : &wp_pool[i - 1]);
    wp_pool[i].next = (i == NR_WP - 1 ? NULL : &wp_pool[i + 1]);
    wp_pool[i].is_free = true;
  }

  head = NULL;
  free_ = wp_pool;
}

/* Implement the functionality of watchpoint */

static WP* new_wp_node() {
  if (!free_) {
    return NULL;
  }

  WP *wp = free_;

  free_ = free_->next;
  free_->prev = NULL;

  Assert(wp->prev == NULL, "Invalid watchpoint list node");
  wp->next = head;

  if (head) {
    head->prev = wp;
  }
  head = wp;
  wp->is_free = false;

  return wp;
}

static void free_wp_node(WP *wp) {
  if (wp->is_free) {
    return;
  }

  if (wp->prev) {
    wp->prev->next = wp->next;
  } else {
    Assert(wp == head, "Invalid watchpoint list node");
    head = head->next;
  }

  if (wp->next) {
    wp->next->prev = wp->prev;
  }

  wp->prev = NULL;
  wp->next = free_;

  free_ = wp;
  wp->is_free = true;
}

bool set_wp(char *e) {
  WP* wp = new_wp_node();
  if (wp) {
    Assert(strlen(e) < EXPR_LEN, "Expression is too long.");
    strcpy(wp->expr, e);

    bool success;
    wp->val = expr(wp->expr, &success);
    wp->val_err = !success;
    if (!success) {
      printf("watchpoint warning: watchpoint is set, but eval failed.\n");
    }

    return true;
  } else {
    return false;
  }
}

bool del_wp(uint32_t no) {
  if (no >= 32) return false;
  if (wp_pool[no - 1].is_free) {
    return false;
  } else {
    free_wp_node(&wp_pool[no - 1]);
    return true;
  }
}

void display_watchpoints() {
  if (head == NULL) {
    printf("No watchpoint.\n");
    return;
  }
  printf("%-16s%-16s%-16s%-8s\n", "NO.", "dec", "hex", "expr");
  for (WP *p = head; p; p = p->next) {
    if (p->val_err) {
      printf("%-16d%-16s%-16s%s\n", p->NO, "err", "err", p->expr);
    } else {
      printf("%-16d%-16u0x%08x%6s%s\n", p->NO, p->val, p->val, "", p->expr);
    }
  }
}

void display_watchpoint(WP *p) {
  if (p->val_err) {
    printf("NO.%-13ddec:%-12shex:%-12s%s\n", p->NO, "err", "err", p->expr);
  } else {
    printf("NO.%-13ddec:%-12uhex:0x%08x%2s%s\n", p->NO, p->val, p->val, "", p->expr);
  }
}

void check_watchpoints() {
  for (WP *p = head; p; p = p->next) {
    bool success;
    word_t val = expr(p->expr, &success);
    bool val_err = !success;
    if (val_err != p->val_err || (!val_err && val != p->val)) {
      nemu_state.state = NEMU_STOP;
      printf("pre: "); display_watchpoint(p);
      p->val = val; p->val_err = val_err;
      printf("now: "); display_watchpoint(p);
    }
  }
}
