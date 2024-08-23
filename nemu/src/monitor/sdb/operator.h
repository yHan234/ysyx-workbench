#ifndef __MONITOR_SDB_OPERATOR_H__
#define __MONITOR_SDB_OPERATOR_H__

#include <stdbool.h>
#include <stdint.h>
#include "token.h"

enum OP{
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

// https://en.cppreference.com/w/c/language/operator_precedence
int op_precedence(enum OP op);
bool op_is_unary(enum OP op);
bool op_is_binary(enum OP op);
enum OP tk_to_op(Token tk, bool unary);

#endif