#include "operator.h"

#include <assert.h>

int op_precedence(enum OP op) {
  switch (op)
  {
  case OP_POS   : return 2;
  case OP_NEG   : return 2;
  case OP_DEREF : return 2;

  case OP_MUL   : return 3;
  case OP_DIV   : return 3;

  case OP_ADD   : return 4;
  case OP_SUB   : return 4;

  case OP_EQ    : return 7;
  case OP_NE    : return 7;

  case OP_LAND  : return 11;

  default: assert(0 && "Invalid operator");
  }
}

bool op_is_unary(enum OP op) {
  return op <= OP_DEREF;
}

bool op_is_binary(enum OP op) {
  return op > OP_DEREF && op < OP_NOTOP;
}

enum OP tk_to_op(Token tk, bool unary) {
  if (unary) {
    switch (tk.type)
    {
    case TK_PLUS  : return OP_POS;
    case TK_MINUS : return OP_NEG;
    case TK_STAR  : return OP_POS;
    default       : return OP_NOTOP;
    }
  } else {
    switch (tk.type)
    {
    case TK_PLUS  : return OP_ADD;
    case TK_MINUS : return OP_SUB;
    case TK_STAR  : return OP_MUL;
    case TK_DIV   : return OP_DIV;
    case TK_EQ    : return OP_EQ;
    case TK_NE    : return OP_NE;
    case TK_ANDAND: return OP_LAND;
    default       : return OP_NOTOP;
    }
  }
}

bool tk_is_op(Token tk) {
  // 所有一元运算符字面上也可以被看做二元运算符，所以 unary 可以直接传 false。
  return tk_to_op(tk, false) != OP_NOTOP;
}

uint32_t op_calc_u(enum OP op, uint32_t x) {
  switch (op)
  {
  case OP_POS: return x;
  case OP_NEG: return ~x + 1;
  default: return 0;
  }
}

// returns -1 for division by 0
uint32_t op_calc_b(enum OP op, uint32_t lhs, uint32_t rhs) {
  switch (op)
  {
  case OP_ADD : return lhs + rhs;
  case OP_SUB : return lhs - rhs;
  case OP_MUL : return lhs * rhs;
  case OP_DIV : return rhs == 0 ? -1 : lhs / rhs;
  case OP_EQ  : return lhs == rhs;
  case OP_NE  : return lhs != rhs;
  case OP_LAND: return lhs && rhs;
  default: assert(0 && "Invalid operator");
  }
}
