#include "token.h"
#include "operator.h"

bool tk_is_lparen(Token tk) {
  return tk.type == TK_LPAREN;
}

bool tk_is_rparen(Token tk) {
  return tk.type == TK_RPAREN;
}

bool tk_is_int(Token tk) {
  return tk.type == TK_INT;
}

bool tk_is_reg(Token tk) {
  return tk.type == TK_REG;
}

bool tk_is_op(Token tk) {
  // 所有一元运算符字面上也可以被看做二元运算符，所以 unary 可以直接传 false。
  return tk_to_op(tk, false) != OP_NOTOP;
}
