#pragma once

#include <string_view>

enum class OpType {
  // unary
  POS,
  NEG,
  DEREF,

  // binary
  MUL,
  DIV,

  ADD,
  SUB,

  EQ,
  NE,

  LAND, // logical and

  // not an operator
  NOTOP,
};

struct ExprToken {
  enum class Type {
    NOTYPE,
    INT,
    REG,
    LPAREN,
    RPAREN,
    PLUS,
    MINUS,
    STAR,
    DIV,
    EQ,
    NE,
    ANDAND,
  };

  Type type;
  // std::string_view str;
  // int pos;

  bool is_op;
  OpType op_type;

  ExprToken(Type type);

  bool IsLParen();
  bool IsRParen();
  bool IsInt();
  bool IsReg();
  bool IsTerminal();
};

struct TokenRule {
  const char *regex;
  ExprToken::Type type;
};

extern TokenRule TOKEN_RULES[];
