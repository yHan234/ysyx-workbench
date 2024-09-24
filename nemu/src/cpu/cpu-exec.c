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

#include <cpu/cpu.h>
#include <cpu/decode.h>
#include <cpu/difftest.h>
#include <locale.h>

/* The assembly code of instructions executed is only output to the screen
 * when the number of instructions executed is less than this value.
 * This is useful when you use the `si' command.
 * You can modify this value as you want.
 */
#define MAX_INST_TO_PRINT 10

CPU_state cpu = {};
uint64_t g_nr_guest_inst = 0;
static uint64_t g_timer = 0; // unit: us
static bool g_print_step = false;

void device_update();
void check_watchpoints();

#define IRINGBUF_LEN 128
#define IRINGBUF_MNEMONIC_LEN 16
#define IRINGBUF_OP_STR_LEN 64
static struct {
  vaddr_t pc;
  int inst_len;
  uint32_t inst;
  char mnemonic[IRINGBUF_MNEMONIC_LEN];
  char op_str[IRINGBUF_OP_STR_LEN];
} iringbuf[IRINGBUF_LEN];
static uint iringbuf_wptr = 0;
static uint iringbuf_size = 0;

static void trace_and_difftest(Decode *_this, vaddr_t dnpc) {
#ifdef CONFIG_ITRACE
  iringbuf[iringbuf_wptr].pc = _this->pc;
  iringbuf[iringbuf_wptr].inst_len = _this->snpc - _this->pc;
  iringbuf[iringbuf_wptr].inst = _this->isa.inst.val;

  void disassemble(char *mnemonic, int size_mnemonic, char *op_str, int size_op_str, uint64_t pc, uint8_t *code, int nbyte);
  disassemble(iringbuf[iringbuf_wptr].mnemonic, IRINGBUF_MNEMONIC_LEN, iringbuf[iringbuf_wptr].op_str, IRINGBUF_OP_STR_LEN,
      MUXDEF(CONFIG_ISA_x86, _this->snpc, _this->pc), (uint8_t *)&_this->isa.inst.val, iringbuf[iringbuf_wptr].inst_len);

  // print step for tiny program
  if (g_print_step) {
    // printf("0x%08x:\t%16s\t%8s\t%s\n", iringbuf[iringbuf_wptr].pc, iringbuf[iringbuf_wptr].inst_str, iringbuf[iringbuf_wptr].mnemonic, iringbuf[iringbuf_wptr].op_str);
  }

  // iringbuf move write pointer and update size
  iringbuf_wptr = (iringbuf_wptr + 1) % IRINGBUF_LEN;
  if (iringbuf_size < IRINGBUF_LEN) {
    iringbuf_size += 1;
  }
#endif
  IFDEF(CONFIG_DIFFTEST, difftest_step(_this->pc, dnpc));
  IFDEF(CONFIG_WATCHPOINT, check_watchpoints());
}

static void exec_once(Decode *s, vaddr_t pc) {
  s->pc = pc;
  s->snpc = pc;
  isa_exec_once(s);
  cpu.pc = s->dnpc;
}

static void execute(uint64_t n) {
  Decode s;
  for (;n > 0; n --) {
    exec_once(&s, cpu.pc);
    g_nr_guest_inst ++;
    trace_and_difftest(&s, cpu.pc);
    if (nemu_state.state != NEMU_RUNNING) break;
    IFDEF(CONFIG_DEVICE, device_update());
  }
}

static void statistic() {
  IFNDEF(CONFIG_TARGET_AM, setlocale(LC_NUMERIC, ""));
#define NUMBERIC_FMT MUXDEF(CONFIG_TARGET_AM, "%", "%'") PRIu64
  Log("host time spent = " NUMBERIC_FMT " us", g_timer);
  Log("total guest instructions = " NUMBERIC_FMT, g_nr_guest_inst);
  if (g_timer > 0) Log("simulation frequency = " NUMBERIC_FMT " inst/s", g_nr_guest_inst * 1000000 / g_timer);
  else Log("Finish running in less than 1 us and can not calculate the simulation frequency");
}

void assert_fail_msg() {
  isa_reg_display();
  statistic();
}

/* Simulate how the CPU works. */
void cpu_exec(uint64_t n) {
  g_print_step = (n < MAX_INST_TO_PRINT);
  switch (nemu_state.state) {
    case NEMU_END: case NEMU_ABORT:
      printf("Program execution has ended. To restart the program, exit NEMU and run again.\n");
      return;
    default: nemu_state.state = NEMU_RUNNING;
  }

  uint64_t timer_start = get_time();

  execute(n);

  uint64_t timer_end = get_time();
  g_timer += timer_end - timer_start;

  switch (nemu_state.state) {
    case NEMU_RUNNING: nemu_state.state = NEMU_STOP; break;

    case NEMU_END: case NEMU_ABORT:
      extern FILE *log_fp;
      log_write("\n");
#ifdef CONFIG_ITRACE_COND
      log_write("==================== ITRACE ====================\n");
      if (ITRACE_COND && iringbuf_size) {
        uint i = iringbuf_size == IRINGBUF_LEN ? iringbuf_wptr : 0;
        do {
          log_write("0x%08x:", iringbuf[i].pc);
          uint8_t *inst = (uint8_t *)&iringbuf[i].inst;
          for (int j = iringbuf[i].inst_len - 1; j >= 0; j--) {
            log_write(" %02x", inst[j]);
          }
          log_write("%8s\t%s\n", iringbuf[i].mnemonic, iringbuf[i].op_str);
          i = (i + 1) % IRINGBUF_LEN;
        } while(i != iringbuf_wptr);
      }
      log_write("==================== ITRACE ====================\n");
      log_write("\n");
#endif
#ifdef CONFIG_MTRACE
      void mringbuf_print();
      log_write("==================== MTRACE ====================\n");
      mringbuf_print();
      log_write("==================== MTRACE ====================\n");
      log_write("\n");
#endif

      Log("nemu: %s at pc = " FMT_WORD,
          (nemu_state.state == NEMU_ABORT ? ANSI_FMT("ABORT", ANSI_FG_RED) :
           (nemu_state.halt_ret == 0 ? ANSI_FMT("HIT GOOD TRAP", ANSI_FG_GREEN) :
            ANSI_FMT("HIT BAD TRAP", ANSI_FG_RED))),
          nemu_state.halt_pc);
      // fall through
    case NEMU_QUIT: statistic();
  }
}
