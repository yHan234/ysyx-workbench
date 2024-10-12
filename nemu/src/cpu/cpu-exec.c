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

// ITRACE

#ifdef CONFIG_ITRACE
#define IRINGBUF_LEN 128
#define IRINGBUF_MNEMONIC_LEN 8
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

#define iringbuf_print(printf)                                                 \
  do {                                                                         \
    if (iringbuf_size) {                                                       \
      uint i = iringbuf_size == IRINGBUF_LEN ? iringbuf_wptr : 0;              \
      do {                                                                     \
        printf("0x%08x:", iringbuf[i].pc);                                     \
        uint8_t *inst = (uint8_t *)&iringbuf[i].inst;                          \
        for (int j = iringbuf[i].inst_len - 1; j >= 0; j--) {                  \
          printf(" %02x", inst[j]);                                            \
        }                                                                      \
        printf("%*s\t%s\n", IRINGBUF_MNEMONIC_LEN, iringbuf[i].mnemonic,       \
               iringbuf[i].op_str);                                            \
        i = (i + 1) % IRINGBUF_LEN;                                            \
      } while (i != iringbuf_wptr);                                            \
    }                                                                          \
  } while (0)
#endif

// FTRACE

#ifdef CONFIG_FTRACE
#define FRINGBUF_LEN 128
typedef struct {
  vaddr_t addr;
  char *name;
} Function;
extern Function *functions;
static struct {
  vaddr_t pc;
  Function *func;
  int op; // 0: ret, 1: call
  int dep;
} fringbuf[FRINGBUF_LEN];
static uint fringbuf_dep = 0;
static uint fringbuf_wptr = 0;
static uint fringbuf_size = 0;
extern uint num_functions;

static void fringbuf_call(vaddr_t pc, vaddr_t addr) {
  fringbuf[fringbuf_wptr].pc = pc;
  fringbuf[fringbuf_wptr].dep = fringbuf_dep++;
  fringbuf[fringbuf_wptr].op = 1;
  // TODO: 可优化为二分查找
  for (int i = 0; i < num_functions; ++i) {
    if (addr == functions[i].addr) {
      fringbuf[fringbuf_wptr].func = &functions[i];
      break;
    }
  }
  Assert(fringbuf[fringbuf_wptr].func, "FTRACE: function at %x not found in elf file", addr);

  fringbuf_wptr = (fringbuf_wptr + 1) % FRINGBUF_LEN;
  if (fringbuf_size < FRINGBUF_LEN) {
    fringbuf_size += 1;
  }
}

static void fringbuf_ret(vaddr_t pc) {
  fringbuf[fringbuf_wptr].pc = pc;
  fringbuf[fringbuf_wptr].dep = --fringbuf_dep;
  fringbuf[fringbuf_wptr].op = 0;
  // TODO: 可优化为二分查找
  for (int i = 1; i < num_functions; ++i) {
    if (pc < functions[i].addr) {
      fringbuf[fringbuf_wptr].func = &functions[i - 1];
      break;
    }
  }
  if (fringbuf[fringbuf_wptr].func == NULL) {
    fringbuf[fringbuf_wptr].func = &functions[num_functions - 1];
  }

  fringbuf_wptr = (fringbuf_wptr + 1) % FRINGBUF_LEN;
  if (fringbuf_size < FRINGBUF_LEN) {
    fringbuf_size += 1;
  }
}
#endif

static void trace_and_difftest(Decode *_this, vaddr_t dnpc) {
#ifdef CONFIG_ITRACE
  iringbuf[iringbuf_wptr].pc = _this->pc;
  iringbuf[iringbuf_wptr].inst_len = _this->snpc - _this->pc;
  iringbuf[iringbuf_wptr].inst = _this->isa.inst.val;

  void disassemble(char *mnemonic, int size_mnemonic, char *op_str, int size_op_str, uint64_t pc, uint8_t *code, int nbyte);
  disassemble(iringbuf[iringbuf_wptr].mnemonic, IRINGBUF_MNEMONIC_LEN, iringbuf[iringbuf_wptr].op_str, IRINGBUF_OP_STR_LEN,
      MUXDEF(CONFIG_ISA_x86, _this->snpc, _this->pc), (uint8_t *)&_this->isa.inst.val, iringbuf[iringbuf_wptr].inst_len);

  if (g_print_step) {
    iringbuf_print(printf);
  }

  iringbuf_wptr = (iringbuf_wptr + 1) % IRINGBUF_LEN;
  if (iringbuf_size < IRINGBUF_LEN) {
    iringbuf_size += 1;
  }
#endif

#ifdef CONFIG_FTRACE
#ifdef CONFIG_ISA_riscv
  char *last_inst = iringbuf[(iringbuf_wptr + IRINGBUF_LEN - 1) % IRINGBUF_LEN].mnemonic;
  if (strcmp(last_inst, "jal") == 0 && cpu.gpr[1] == _this->snpc) {
    fringbuf_call(_this->pc, _this->dnpc);
  }
  if (strcmp(last_inst, "jalr") == 0 && cpu.gpr[1] == _this->snpc) {
    fringbuf_call(_this->pc, _this->dnpc);
  }
  if (strcmp(last_inst, "ret") == 0) {
    fringbuf_ret(_this->pc);
  }
#endif
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

void assert_fail() {
  nemu_state.state = NEMU_ABORT;
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
      if (ITRACE_COND) {
        log_write("==================== ITRACE ====================\n");
        iringbuf_print(log_write);
        log_write("==================== ITRACE ====================\n");
        log_write("\n");
      }
#endif
#ifdef CONFIG_MTRACE
      void mringbuf_print();
      log_write("==================== MTRACE ====================\n");
      mringbuf_print();
      log_write("==================== MTRACE ====================\n");
      log_write("\n");
#endif
#ifdef CONFIG_DTRACE
      void dringbuf_print();
      log_write("==================== DTRACE ====================\n");
      dringbuf_print();
      log_write("==================== DTRACE ====================\n");
      log_write("\n");
#endif
#ifdef CONFIG_FTRACE
      log_write("==================== FTRACE ====================\n");
      if (fringbuf_size) {
        uint i = fringbuf_size == FRINGBUF_LEN ? fringbuf_wptr : 0;
        do {
          log_write("0x%08x: ", fringbuf[i].pc);
          if (fringbuf[i].op) {
            log_write("%*scall [%s@%#010x]\n", fringbuf[i].dep * 2, "", fringbuf[i].func->name, fringbuf[i].func->addr);
          } else {
            log_write("%*sret  [%s]\n", fringbuf[i].dep * 2, "", fringbuf[i].func->name);
          }
          i = (i + 1) % FRINGBUF_LEN;
        } while (i != fringbuf_wptr);
      }
      log_write("==================== FTRACE ====================\n");
      log_write("\n");
#endif
#ifdef CONFIG_ETRACE
      log_write("==================== ETRACE ====================\n");
      void eringbuf_print();
      eringbuf_print();
      log_write("==================== ETRACE ====================\n");
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
