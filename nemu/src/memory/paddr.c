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

#include <memory/host.h>
#include <memory/paddr.h>
#include <device/mmio.h>
#include <isa.h>

#if   defined(CONFIG_PMEM_MALLOC)
static uint8_t *pmem = NULL;
#else // CONFIG_PMEM_GARRAY
static uint8_t pmem[CONFIG_MSIZE] PG_ALIGN = {};
#endif

#ifdef CONFIG_MTRACE
extern CPU_state cpu;
#define MRINGBUF_LEN 128
struct {
  vaddr_t pc;
  char op;
  paddr_t addr;
  int len;
  word_t data;
} mringbuf[MRINGBUF_LEN];
word_t mtrace_begin = MTRACE_BEGIN;
word_t mtrace_end = MTRACE_END;
uint mringbuf_wptr = 0;
uint mringbuf_size = 0;

static void mringbuf_write(char op, paddr_t addr, int len, word_t data) {
  if (addr < mtrace_begin || mtrace_end < addr) {
    return;
  }

  mringbuf[mringbuf_wptr].pc = cpu.pc;
  mringbuf[mringbuf_wptr].op = op;
  mringbuf[mringbuf_wptr].addr = addr;
  mringbuf[mringbuf_wptr].len = len;
  mringbuf[mringbuf_wptr].data = data;

  mringbuf_wptr = (mringbuf_wptr + 1) % MRINGBUF_LEN;
  if (mringbuf_size < MRINGBUF_LEN) {
    mringbuf_size += 1;
  }
}

void mringbuf_print() {
  extern FILE *log_fp;
  if (mringbuf_size) {
    uint i = mringbuf_size == MRINGBUF_LEN ? mringbuf_wptr : 0;
    do {
      extern FILE *log_fp;
      log_write("0x%08x: %c 0x%08x %d 0x%08x\n", mringbuf[i].pc, mringbuf[i].op, mringbuf[i].addr, mringbuf[i].len, mringbuf[i].data);
      i = (i + 1) % MRINGBUF_LEN;
    } while (i != mringbuf_wptr);
  }
}
#endif

uint8_t* guest_to_host(paddr_t paddr) { return pmem + paddr - CONFIG_MBASE; }
paddr_t host_to_guest(uint8_t *haddr) { return haddr - pmem + CONFIG_MBASE; }

static word_t pmem_read(paddr_t addr, int len) {
  word_t ret = host_read(guest_to_host(addr), len);
  IFDEF(CONFIG_MTRACE, mringbuf_write('R', addr, len, ret));
  return ret;
}

static void pmem_write(paddr_t addr, int len, word_t data) {
  host_write(guest_to_host(addr), len, data);
  IFDEF(CONFIG_MTRACE, mringbuf_write('W', addr, len, data));
}

static void out_of_bound(paddr_t addr) {
  panic("address = " FMT_PADDR " is out of bound of pmem [" FMT_PADDR ", " FMT_PADDR "] at pc = " FMT_WORD,
      addr, PMEM_LEFT, PMEM_RIGHT, cpu.pc);
}

void init_mem() {
#if   defined(CONFIG_PMEM_MALLOC)
  pmem = malloc(CONFIG_MSIZE);
  assert(pmem);
#endif
  IFDEF(CONFIG_MEM_RANDOM, memset(pmem, rand(), CONFIG_MSIZE));
  Log("physical memory area [" FMT_PADDR ", " FMT_PADDR "]", PMEM_LEFT, PMEM_RIGHT);
}

word_t paddr_read(paddr_t addr, int len) {
  if (likely(in_pmem(addr))) return pmem_read(addr, len);
  IFDEF(CONFIG_DEVICE, return mmio_read(addr, len));
  out_of_bound(addr);
  return 0;
}

void paddr_write(paddr_t addr, int len, word_t data) {
  if (likely(in_pmem(addr) && addr > 0x80002544)) { pmem_write(addr, len, data); return; }
  IFDEF(CONFIG_DEVICE, mmio_write(addr, len, data); return);
  out_of_bound(addr);
}
