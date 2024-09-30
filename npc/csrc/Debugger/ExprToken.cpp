#include "ExprToken.hpp"

TokenRule TOKEN_RULES[] {
  // Pay attention to the precedence level of different rules.
  {"\\s+",            ExprToken::Type::NOTYPE },
  {"\\$\\w+|\\$\\$0", ExprToken::Type::REG    },
  {"(0x)?[0-9]+u?",   ExprToken::Type::INT    },
  {"\\(",             ExprToken::Type::LPAREN },
  {"\\)",             ExprToken::Type::RPAREN },
  {"\\+",             ExprToken::Type::PLUS   },
  {"-",               ExprToken::Type::MINUS  },
  {"\\*",             ExprToken::Type::STAR   },
  {"/",               ExprToken::Type::DIV    },
  {"==",              ExprToken::Type::EQ     },
  {"!=",              ExprToken::Type::NE     },
  {"&&",              ExprToken::Type::ANDAND },
};

ExprToken::ExprToken(Type type)
    : type(type) {}

bool ExprToken::IsLParen() {
  return type == ExprToken::Type::LPAREN;
}

bool ExprToken::IsRParen() {
  return type == ExprToken::Type::RPAREN;
}

bool ExprToken::IsInt() {
  return type == ExprToken::Type::INT;
}

bool ExprToken::IsReg() {
  return type == ExprToken::Type::REG;
}

bool ExprToken::IsTerminal() {
  return IsInt() || IsReg();
}
