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

enum TK {
  TK_NOTYPE,
  TK_INT,
  TK_REG,
  TK_LPAREN,
  TK_RPAREN,
  TK_PLUS,
  TK_MINUS,
  TK_STAR,
  TK_DIV,
  TK_EQ,
  TK_NE,
  TK_ANDAND
};

static struct rule {
  const char *regex;
  int token_type;
} rules[] = {

  /*
   * Pay attention to the precedence level of different rules.
   */

  {"\\s+",          TK_NOTYPE},
  {"(0x)?[0-9]+u?", TK_INT},
  {"$\\w+",         TK_REG},
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

typedef struct token {
  enum TK type;
  char str[32];
  int pos;
} Token;

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

static bool tk_is_lparen(Token tk) {
  return tk.type == TK_LPAREN;
}

static bool tk_is_rparen(Token tk) {
  return tk.type == TK_RPAREN;
}

static bool is_paren_paring(int bo, int eo) {
  if (tk_is_lparen(tokens[bo]) || tk_is_rparen(tokens[eo - 1])) {
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

static bool tk_is_int(Token tk) {
  return tk.type == TK_INT;
}

// evaluate

enum OP{
  // list by precedence

  // unary
  OP_POS,
  OP_NEG,
  OP_DEREF,

  // binary
  OP_MUL,
  OP_DIV,

  OP_ADD,
  OP_SUB,

  OP_EQ,
  OP_NE,

  OP_LAND,  // logical and

  // not an operator
  OP_NOTOP,
};

// static bool op_is_unary(enum OP op) {
//   return op <= OP_DEREF;
// }

// static bool op_is_binary(enum OP op) {
//   return op > OP_DEREF && op < OP_NOTOP;
// }

static enum OP tk_to_op(Token token, bool may_be_unary) {
  switch (token.type)
  {
  case TK_PLUS:   return may_be_unary ? OP_POS    : OP_ADD;
  case TK_MINUS:  return may_be_unary ? OP_NEG    : OP_SUB;
  case TK_STAR:   return may_be_unary ? OP_DEREF  : OP_MUL;

  case TK_DIV   : return OP_DIV;
  case TK_EQ    : return OP_EQ;
  case TK_NE    : return OP_NE;
  case TK_ANDAND: return OP_LAND;

  default: return OP_NOTOP;
  }
}

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
    Log("Terminal integer: %d at position" INDICATE_TK_FMT, num, INDICATE_TK_ARG(tk));
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
    // divide and conquer

    // find the last lowest priority binary operator
    // 1. 正序寻找，方便判断一元运算符
    // 2. 先找最低优先级的二元运算符，高优先级的会先递归执行
    // 3. 同优先级找右边的二元运算符，左边的会先递归执行
    int p = -1;
    enum OP op = OP_NOTOP;
    int paren_cnt = 0;
    bool may_be_unary = true;
    for (int i = bo; i < eo; ++i) {
      Token tk = tokens[i];
      if (tk_is_int(tk)) {
        may_be_unary = false;
        continue;
      } else if (tk_is_lparen(tk)) {
        paren_cnt += 1;
        may_be_unary = true;
      } else if (tk_is_rparen(tk)) {
        paren_cnt -= 1;
        may_be_unary = false;
      } else if (tk_to_op(tokens[i], may_be_unary) != OP_NOTOP) {
        enum OP cur_op = tk_to_op(tk, may_be_unary);
        if (cur_op == OP_POS || cur_op == OP_NEG) {
          may_be_unary = true;
        } else if (cur_op == OP_ADD || cur_op == OP_SUB) {
          may_be_unary = true;
          if (!paren_cnt && (p == -1 || op == OP_MUL || op == OP_DIV || op == OP_ADD || op == OP_SUB)) {
            p = i;
            op = cur_op;
          }
        } else if (cur_op == OP_MUL || cur_op == OP_DIV) {
          may_be_unary = true;
          if (!paren_cnt && (p == -1 || op == OP_MUL || op == OP_DIV)) {
            p = i;
            op = cur_op;
          }
        }
      }
    }

    // If there is no binary operator, then the beginning must be a unary operator
    if (p == -1) {
      op = tk_to_op(tokens[bo], true);
      Log("Process unary operator %s at position %d\n%s\n%*.s^\n", tokens[bo].str, tokens[bo].pos, expression, tokens[bo].pos, "");
      if (op == OP_POS) {
        return eval(success, bo + 1, eo);
      } else if (op == OP_NEG) {
        return ~eval(success, bo + 1, eo) + 1;
      } else {
        FAIL("eval: Invalid expression at position %d\n%s\n%*.s^\n", tokens[bo].pos, expression, tokens[bo].pos, "");
      }
    }

    // there is a binary operator
    Log("Divide from binary operator %s at position %d\n%s\n%*.s^\n", tokens[p].str, tokens[p].pos, expression, tokens[p].pos, "");
    if (bo >= p) FAIL("eval: Expect a expression at position %d\n%s\n%*.s^\n", tokens[bo].pos, expression, tokens[bo].pos, "");
    if (p + 1 >= eo) FAIL("eval: Expect a expression at position %d\n%s\n%*.s^\n", tokens[p + 1].pos, expression, tokens[p + 1].pos, "");
    word_t lhs = eval(success, bo, p);
    word_t rhs = eval(success, p + 1, eo);
    switch (op)
    {
    case OP_ADD: return lhs + rhs;
    case OP_SUB: return lhs - rhs;
    case OP_MUL: return lhs * rhs;
    case OP_DIV:
      if (rhs == 0) FAIL("eval: Divide by zero at position %d\n%s\n%*.s^\n", tokens[p].pos, expression, tokens[p].pos, "");
      return lhs / rhs;
    default: panic("eval: Invalid oeprator");
    }
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
