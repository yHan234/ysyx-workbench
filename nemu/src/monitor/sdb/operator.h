#ifndef __MONITOR_SDB_OPERATOR_H__
#define __MONITOR_SDB_OPERATOR_H__

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

#endif