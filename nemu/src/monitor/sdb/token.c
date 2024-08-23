#include "token.h"

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
