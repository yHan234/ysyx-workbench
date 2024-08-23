#ifndef __MONITOR_SDB_OPERATOR_H__
#define __MONITOR_SDB_OPERATOR_H__

#include <stdbool.h>
#include <stdint.h>
#include "token.h"

// The smaller the number, the higher the priority.
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

bool op_is_unary(enum OP op);
bool op_is_binary(enum OP op);
enum OP tk_to_op(Token token, bool unary);
bool tk_is_op(Token token);
uint32_t op_calc_u(enum OP op, uint32_t x);
uint32_t op_calc_b(enum OP op, uint32_t lhs, uint32_t rhs);

#endif