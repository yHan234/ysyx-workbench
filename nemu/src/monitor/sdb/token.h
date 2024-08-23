#ifndef __MONITOR_SDB_TOKEN_H__
#define __MONITOR_SDB_TOKEN_H__

#include <stdbool.h>

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
  TK_ANDAND,
};

typedef struct token {
  enum TK type;
  char str[32];
  int pos;
} Token;

bool tk_is_lparen(Token tk);
bool tk_is_rparen(Token tk);
bool tk_is_int(Token tk);
bool tk_is_reg(Token tk);
bool tk_is_terminal(Token tk);
bool tk_is_op(Token tk);

#endif