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

enum {
  TK_NOTYPE = 256, TK_EQ, TK_INT

  /* Add more token types */

};

static struct rule {
  const char *regex;
  int token_type;
} rules[] = {

  /*
   * Pay attention to the precedence level of different rules.
   */

  {"\\s+", TK_NOTYPE},    // spaces
  {"\\+", '+'},         // plus
  {"-", '-'},           // minus
  {"\\*", '*'},         // multiplication
  {"/", '/'},           // division
  {"==", TK_EQ},        // equal
  {"\\(", '('},         // left parenthesis
  {"\\)", ')'},         // right parenthesis
  {"[0-9]+u?", TK_INT},   // decimal integers
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
  int type;
  char str[32];
  int bo;
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
            tokens[nr_token].bo = position;
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

// evaluate

enum OP{
  OP_NOOP,

  // list by priority
  OP_ADD,
  OP_SUB,
  OP_MUL,
  OP_DIV,
  OP_POS,
  OP_NEG,

  // special operation
  OP_LPAREN,
  OP_RPAREN,
};

enum OP token_to_op(Token token, bool may_be_unary) {

  if (token.type == '+' && may_be_unary) return OP_POS;
  if (token.type == '-' && may_be_unary) return OP_NEG;

  if (token.type == '+') return OP_ADD;
  if (token.type == '-') return OP_SUB;
  if (token.type == '*') return OP_MUL;
  if (token.type == '/') return OP_DIV;
  if (token.type == '(') return OP_LPAREN;
  if (token.type == ')') return OP_RPAREN;

  panic("Unexpected token \"%s\" at position %d\n%s\n%*.s^\n", token.str, token.bo, expression, token.bo, "");
}

bool check_parentheses(int bo, int eo) {
  if (token_to_op(tokens[bo], false) != OP_LPAREN || token_to_op(tokens[eo - 1], false) != OP_RPAREN) {
    return false;
  }

  int paren_cnt = 0;
  for (int i = 0; i < eo - 1; ++i) {
    enum OP op = token_to_op(tokens[i], false);
    if (op == OP_LPAREN) {
      paren_cnt += 1;
    } else {
      paren_cnt -= 1;
      if (paren_cnt == 0)
        return false;
    }
  }

  return paren_cnt == 1;
}


word_t eval(bool *success, int bo, int eo) {
#define FAIL(format, ...) do { printf(format, __VA_ARGS__); *success = false; return 0; } while(0)

  if (bo >= eo) {
    panic("eval: The algorithm is buggy.");
  } else if (bo + 1 == eo) {
    // only one token, it must be a number.
    if (tokens[bo].type == TK_INT) {
      errno = 0;
      word_t num = strtoul(tokens[bo].str, NULL, 10);

      if (errno == ERANGE) {
        errno = 0;
        FAIL("eval: The number is too long at position %d\n%s\n%*.s^\n", tokens[bo].bo, expression, tokens[bo].bo, "");
      } else return num;
    } else {
      FAIL("eval: Expect a number at position %d\n%s\n%*.s^\n", tokens[bo].bo, expression, tokens[bo].bo, "");
    }
  } else if (check_parentheses(bo, eo)) {
    // remove the parentheses.
    if (bo + 1 == eo - 1) FAIL("eval: Expressions should be in parentheses at position %d\n%s\n%*.s^\n", tokens[bo].bo, expression, tokens[bo].bo, "");
    return eval(success, bo + 1, eo - 1);
  } else {
    // divide and conquer

    // find the highest priority binary operator
    int p = -1;
    enum OP op = OP_NOOP;
    int paren_cnt = 0;
    bool may_be_unary = true;
    for (int i = bo; i < eo; ++i) {
      if (tokens[i].type == TK_INT) {
        may_be_unary = false;
        continue;
      }

      enum OP cur_op = token_to_op(tokens[i], may_be_unary);
      if (cur_op == OP_POS || cur_op == OP_NEG) {
        may_be_unary = true;
      } else if (cur_op == OP_ADD || cur_op == OP_SUB) {
        may_be_unary = true;
        if (!paren_cnt && p == -1) {
          p = i;
          op = cur_op;
        }
      } else if (cur_op == OP_MUL || cur_op == OP_DIV) {
        may_be_unary = true;
        if (!paren_cnt && (p == -1 || op == OP_ADD || op == OP_SUB)) {
          p = i;
          op = cur_op;
        }
      } else if (cur_op == OP_LPAREN) {
        paren_cnt += 1;
        may_be_unary = true;
      } else if (cur_op == OP_RPAREN) {
        paren_cnt -= 1;
        may_be_unary = false;
      }
    }

    // If there is no binary operator, then the beginning must be a unary operator
    if (p == -1) {
      op = token_to_op(tokens[bo], true);
      if (op == OP_POS) {
        return eval(success, bo + 1, eo);
      } else if (op == OP_NEG) {
        return ~eval(success, bo + 1, eo) + 1;
      } else {
        FAIL("eval: Invalid expression at position %d\n%s\n%*.s^\n", tokens[bo].bo, expression, tokens[bo].bo, "");
      }
    }

    // there is a binary operator
    word_t lhs = eval(success, bo, p);
    word_t rhs = eval(success, p + 1, eo);
    switch (op)
    {
    case OP_ADD: return lhs + rhs;
    case OP_SUB: return lhs - rhs;
    case OP_MUL: return lhs * rhs;
    case OP_DIV:
      if (rhs == 0) FAIL("eval: Divide by zero at position %d\n%s\n%*.s^\n", tokens[p].bo, expression, tokens[p].bo, "");
      return lhs / rhs;
    default: panic("eval: Invalid oeprator");
    }
  }

#undef FAIL
}

word_t expr(char *e, bool *success) {
  if (!make_token(e)) {
    *success = false;
    return 0;
  }
  return eval(success, 0, nr_token);
}
