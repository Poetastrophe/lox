#ifndef TOKENTYPE_H
#define TOKENTYPE_H
#include "string.h"

typedef enum {

  // Symbols.
  LEFT_PAREN,
  RIGHT_PAREN,
  LEFT_BRACE,
  RIGHT_BRACE,
  LEFT_BRACKET,
  RIGHT_BRACKET,
  SLASH,
  PLUS,
  MINUS,
  STAR,
  UPTICK,
  DQUOTE,
  SQUOTE,
  TICK,
  DOLLAR,
  COMMA,
  DOT,
  SEMICOLON,
  EQUAL,
  LT,
  GT,
  BANG,
  TILDE,
  LTEQ,
  GTEQ,
  BANGEQ,
  TILDEEQ,
  EQEQ,

  // Literals
  IDENTIFIER,
  NUMBER,
  STRING,

  // Keywords
  VAR,
  FUN,
  FOR,
  IN,
  DATA,
  CLASS,
  PRINT,
  AND,
  OR,
  IF,
  TRUE,
  FALSE,
  ELSE,
  NIL,
  RETURN,

  // End of file
  END_OF_FILE
} TokenType;

char *tokentype_string(TokenType type);
TokenType type_from_kw(char *optKw);

#endif
