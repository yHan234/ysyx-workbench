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
#include <errno.h>

/* We use the POSIX regex functions to process regular expressions.
 * Type 'man regex' for more information about POSIX regex functions.
 */
#include <regex.h>

#include <memory/vaddr.h>

#include "token.h"
#include "operator.h"

static struct rule {
  const char *regex;
  int token_type;
} rules[] = {

  /*
   * Pay attention to the precedence level of different rules.
   */

  {"\\s+",          TK_NOTYPE},
  {"(0x)?[0-9]+u?", TK_INT},
  {"\\$\\w+",         TK_REG},
  {"\\(",           TK_LPAREN},
  {"\\)",           TK_RPAREN},
  {"\\+",           TK_PLUS},
  {"-",             TK_MINUS},
  {"\\*",           TK_STAR},
  {"/",             TK_DIV},
  {"==",            TK_EQ},
  {"!=",            TK_NE},
  {"&&",            TK_ANDAND},
};

#define NR_REGEX ARRLEN(rules)

static regex_t re[NR_REGEX] = {};

/* Rules are used for many times.
 * Therefore we compile them only once before any usage.
 */
void init_regex() {
  int i;
  char error_msg[128];
  int ret;

  for (i = 0; i < NR_REGEX; i ++) {
    ret = regcomp(&re[i], rules[i].regex, REG_EXTENDED);
    if (ret != 0) {
      regerror(ret, &re[i], error_msg, 128);
      panic("regex compilation failed: %s\n%s", error_msg, rules[i].regex);
    }
  }
}

static Token tokens[65536] __attribute__((used)) = {};
static int nr_token __attribute__((used))  = 0;

static char *expression;

static bool make_token(char *e) {
  int position = 0;
  int i;
  regmatch_t pmatch;

  expression = e;
  nr_token = 0;

  while (e[position] != '\0') {
    /* Try all rules one by one. */
    for (i = 0; i < NR_REGEX; i ++) {
      if (regexec(&re[i], e + position, 1, &pmatch, 0) == 0 && pmatch.rm_so == 0) {
        char *substr_start = e + position;
        int substr_len = pmatch.rm_eo;

        Log("match rules[%d] = \"%s\" at position %d with len %d: %.*s",
            i, rules[i].regex, position, substr_len, substr_len, substr_start);

        /* Now a new token is recognized with rules[i]. Add codes
         * to record the token in the array `tokens'. For certain types
         * of tokens, some extra actions should be performed.
         */

        switch (rules[i].token_type) {
          case TK_NOTYPE: break;
          default:
            if (nr_token == 65536) {
              puts("Too many tokens!\n");
              return false;
            }
            if (pmatch.rm_eo > 31) {
              printf("The token is too long at position %d\n%s\n%*.s^\n", position, e, position, "");
              return false;
            }
            tokens[nr_token].type = rules[i].token_type;
            memcpy(tokens[nr_token].str, e + position, pmatch.rm_eo);
            tokens[nr_token].str[pmatch.rm_eo] = '\0';
            tokens[nr_token].pos = position;
            nr_token += 1;
        }

        position += substr_len;

        break;
      }
    }

    if (i == NR_REGEX) {
      printf("no match at position %d\n%s\n%*.s^\n", position, e, position, "");
      return false;
    }
  }

  return true;
}

static bool is_paren_paring(int bo, int eo) {
  if (!tk_is_lparen(tokens[bo]) || !tk_is_rparen(tokens[eo - 1])) {
    return false;
  }

  int paren_cnt = 0;
  for (int i = bo; i < eo - 1; ++i) {
    if (tk_is_lparen(tokens[i])) {
      paren_cnt += 1;
    } else if (tk_is_rparen(tokens[i])) {
      paren_cnt -= 1;
      if (paren_cnt == 0)
        return false;
    }
  }

  return paren_cnt == 1;
}

// evaluate

#define FAIL(format, ...) do { printf(format, __VA_ARGS__); *success = false; return 0; } while(0)
#define INDICATE_TK_FMT " at position %d\n%s\n%*.s^\n"
#define INDICATE_TK_ARG(tk) tk.pos, expression, tk.pos, ""

static word_t tk_to_int(Token tk, bool *success) {
  if (!tk_is_int(tk)) { success = false; return 0; }
  errno = 0;
  word_t num = strtoul(tk.str, NULL, 0);

  if (errno == ERANGE) {
    errno = 0;
    FAIL("eval: The number is too long at position" INDICATE_TK_FMT, INDICATE_TK_ARG(tk));
  } else {
    Log("Terminal integer: %u at position" INDICATE_TK_FMT, num, INDICATE_TK_ARG(tk));
    return num;
  }
}

static word_t eval(bool *success, int bo, int eo) {
  if (bo >= eo) {
    panic("eval internal error: Errors need to be caught before.");
  } else if (bo + 1 == eo) {
    // only one token, it must be a number.
    if (tk_is_int(tokens[bo])) {
      return tk_to_int(tokens[bo], success);
    } else {
      FAIL("eval: Expect a number" INDICATE_TK_FMT, INDICATE_TK_ARG(tokens[bo]));
    }
  } else if (is_paren_paring(bo, eo)) {
    // remove the parentheses.
    if (bo + 1 == eo - 1) {
      FAIL("eval: Expressions should be in parentheses" INDICATE_TK_FMT, INDICATE_TK_ARG(tokens[bo]));
    } else {
      Log("Enter parentheses" INDICATE_TK_FMT, INDICATE_TK_ARG(tokens[bo]));
      return eval(success, bo + 1, eo - 1);
    }
  } else {
    // divide and conquer:
    // find the last lowest priority binary operator, then divide
    // 1. 正序寻找，方便判断一元运算符
    // 2. 先找最低优先级的二元运算符，高优先级的会先递归执行
    // 3. 同优先级找右边的二元运算符，左边的会先递归执行

    enum OP op = OP_NOTOP;  // the last lowest priority binary operator
    int p = -1;             // the position of "the last lowest priority binary operator"
    int paren_cnt = 0;
    bool may_be_unary = true;
    for (int i = bo; i < eo; ++i) {
      Token tk = tokens[i];
      if (tk_is_int(tk)) {
        may_be_unary = false;
      } else if (tk_is_rparen(tk)) {
        paren_cnt -= 1;
        may_be_unary = false;
      } else if (tk_is_lparen(tk)) {
        paren_cnt += 1;
        may_be_unary = true;
      } else if (tk_is_op(tk)) {
        enum OP cur_op = tk_to_op(tk, may_be_unary);
        may_be_unary = true;  // 一元二元运算符后都可能是一元运算符
        if (op_is_binary(cur_op)) {
          // 不能从内层括号中的二元运算符分开
          if (!paren_cnt && (p == -1 || op_precedence(cur_op) >= op_precedence(op))) {
            p = i;
            op = cur_op;
          }
        }
      }
    }

    // If there is no binary operator, then the beginning must be a unary operator
    if (op == OP_NOTOP) {
      if (tk_is_op(tokens[bo]) && op_is_unary(op = tk_to_op(tokens[bo], true))) {
        Log("Process unary operator %s" INDICATE_TK_FMT, tokens[bo].str, INDICATE_TK_ARG(tokens[bo]));
        word_t x = eval(success, bo + 1, eo), res;
        switch (op)
        {
        case OP_POS   : res = x; break;
        case OP_NEG   : res = ~x + 1; break;
        case OP_DEREF : res = vaddr_read(x, 4); break;
        default       : panic("eval: Invalid unary operator");
        }
        Log("Calculate: %s%u => %u" INDICATE_TK_FMT, tokens[bo].str, x, res, INDICATE_TK_ARG(tokens[bo]));
        return res;
      } else {
        FAIL("eval: Invalid expression"  INDICATE_TK_FMT, INDICATE_TK_ARG(tokens[bo]));
      }
    }

    // there is a binary operator
    Log("Divide from binary operator %s" INDICATE_TK_FMT, tokens[p].str, INDICATE_TK_ARG(tokens[p]));
    if (bo >= p) FAIL("eval: Expect a expression" INDICATE_TK_FMT, INDICATE_TK_ARG(tokens[bo]));
    if (p + 1 >= eo) FAIL("eval: Expect a expression" INDICATE_TK_FMT, INDICATE_TK_ARG(tokens[p + 1]));
    word_t lhs = eval(success, bo, p), rhs, res;
    switch (op)
    {
    case OP_ADD : rhs = eval(success, p + 1, eo); res = lhs + rhs; break;
    case OP_SUB : rhs = eval(success, p + 1, eo); res = lhs - rhs; break;
    case OP_MUL : rhs = eval(success, p + 1, eo); res = lhs * rhs; break;
    case OP_DIV :
      rhs = eval(success, p + 1, eo);
      if (rhs == 0) FAIL("eval: Divide by zero" INDICATE_TK_FMT, INDICATE_TK_ARG(tokens[p]));
      res = lhs / rhs;
      break;
    case OP_EQ  : rhs = eval(success, p + 1, eo); res = lhs == rhs; break;
    case OP_NE  : rhs = eval(success, p + 1, eo); res = lhs != rhs; break;
    case OP_LAND: res = lhs ? rhs = !!eval(success, p + 1, eo) : 0; break;
    default: assert(0 && "Invalid operator");
    }
    Log("Calculate: %u %s %u => %u" INDICATE_TK_FMT, lhs, tokens[p].str, rhs, res, INDICATE_TK_ARG(tokens[p]));
    return res;
  }
}

word_t expr(char *e, bool *success) {
  *success = true;
  if (!make_token(e)) {
    *success = false;
    return 0;
  }
  return eval(success, 0, nr_token);
}

#undef FAIL
#undef INDICATE_TK_FMT
#undef INDICATE_TK_ARG
