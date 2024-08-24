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

typedef struct watchpoint {
  int NO;
  struct watchpoint *prev;
  struct watchpoint *next;

  char *expr;
} WP;

static WP wp_pool[NR_WP] = {};
static WP *head = NULL, *free_ = NULL;

void init_wp_pool() {
  int i;
  for (i = 0; i < NR_WP; i ++) {
    wp_pool[i].NO = i;
    wp_pool[i].prev = (i == 0 ? NULL : &wp_pool[i - 1]);
    wp_pool[i].next = (i == NR_WP - 1 ? NULL : &wp_pool[i + 1]);
  }

  head = NULL;
  free_ = wp_pool;
}

/* Implement the functionality of watchpoint */

WP* new_wp() {
  if (!free_) {
    return NULL;
  }

  WP *wp = free_;

  free_ = free_->next;
  free_->prev = NULL;

  Assert(wp->prev == NULL, "Invalid List Node");
  wp->next = head;

  head = wp;
  return wp;
}

void free_wp(WP *wp) {
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
}

