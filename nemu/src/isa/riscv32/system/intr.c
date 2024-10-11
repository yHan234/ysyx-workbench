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

#include <isa.h>

#ifdef CONFIG_ETRACE
#define ERINGBUF_LEN 128
static struct {
  vaddr_t pc;
  vaddr_t mtvec;
  word_t cause;
} eringbuf[ERINGBUF_LEN];
static uint eringbuf_wptr = 0;
static uint eringbuf_size = 0;

static void eringbuf_write() {
  eringbuf[eringbuf_wptr].pc = cpu.pc;
  eringbuf[eringbuf_wptr].mtvec = cpu.csr[CSR_MTVEC];
  eringbuf[eringbuf_wptr].cause = cpu.csr[CSR_MCAUSE];
  eringbuf_wptr = (eringbuf_wptr + 1) % ERINGBUF_LEN;
  if (eringbuf_size < 128) {
    eringbuf_size += 1;
  }
}

void eringbuf_print() {
  extern FILE *log_fp;
  if (eringbuf_size) {
    uint i = eringbuf_size == ERINGBUF_LEN ? eringbuf_wptr : 0;
    do {
      log_write("0x%08x: %08x %08x\n", eringbuf[i].pc, eringbuf[i].mtvec, eringbuf[i].cause);
      i = (i + 1) % ERINGBUF_LEN;
    } while (i != eringbuf_wptr);
  }
}
#endif

word_t isa_raise_intr(word_t NO, vaddr_t epc) {
  cpu.csr[CSR_MCAUSE] = NO;
  cpu.csr[CSR_MEPC] = epc;
  eringbuf_write();
  return cpu.csr[CSR_MTVEC];
}

word_t isa_query_intr() {
  return INTR_EMPTY;
}
