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
#include <cpu/difftest.h>
#include "../local-include/reg.h"

bool isa_difftest_checkregs(CPU_state *ref_r, vaddr_t pc) {
  extern const char *regs[];
  extern CPU_state cpu;

  if (cpu.pc != pc) {
    Log("PC is different"
        ", right = " FMT_WORD ", wrong = " FMT_WORD ", diff = " FMT_WORD,
        pc, cpu.pc, pc ^ cpu.pc);
    return false;
  }
  for (int i = 0; i < sizeof(cpu.gpr) / sizeof(word_t); i++) {
    if (!difftest_check_reg(regs[i], pc, ref_r->gpr[i], cpu.gpr[i])) {
      return false;
    }
  }

  return true;
}

void isa_difftest_attach() {
}
