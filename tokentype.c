#include "tokentype.h"

char *tokentype_string(TokenType type) {
  switch (type) {
  case LEFT_PAREN:
    return "LEFT_PAREN";
  case RIGHT_PAREN:
    return "RIGHT_PAREN";
  case LEFT_BRACE:
    return "LEFT_BRACE";
  case RIGHT_BRACE:
    return "RIGHT_BRACE";
  case COMMA:
    return "COMMA";
  case DOT:
    return "DOT";
  case MINUS:
    return "MINUS";
  case PLUS:
    return "PLUS";
  case SEMICOLON:
    return "SEMICOLON";
  case SLASH:
    return "SLASH";
  case STAR:
    return "STAR";
  case BANG:
    return "BANG";
  case BANGEQ:
    return "BANG_EQUAL";
  case EQUAL:
    return "EQUAL";
  case EQEQ:
    return "EQUAL_EQUAL";
  case GT:
    return "GREATER";
  case GTEQ:
    return "GREATER_EQUAL";
  case LT:
    return "LESS";
  case LTEQ:
    return "LESS_EQUAL";
  case IDENTIFIER:
    return "IDENTIFIER";
  case STRING:
    return "STRING";
  case NUMBER:
    return "NUMBER";
  case AND:
    return "AND";
  case CLASS:
    return "CLASS";
  case ELSE:
    return "ELSE";
  case FALSE:
    return "FALSE";
  case FUN:
    return "FUN";
  case FOR:
    return "FOR";
  case IF:
    return "IF";
  case NIL:
    return "NIL";
  case OR:
    return "OR";
  case PRINT:
    return "PRINT";
  case RETURN:
    return "RETURN";
  case TRUE:
    return "TRUE";
  case VAR:
    return "VAR";
  case END_OF_FILE:
    return "END_OF_FILE";
  default:
    return "UNKNOWN";
  }
  return "UNKNOWN";
}

// If not a keyword, we default to EOF
TokenType type_from_kw(char *optKw) {
  if (strcmp(optKw, "fun") == 0)
    return FUN;
  if (strcmp(optKw, "for") == 0)
    return FOR;
  if (strcmp(optKw, "if") == 0)
    return IF;
  if (strcmp(optKw, "false") == 0)
    return FALSE;
  if (strcmp(optKw, "true") == 0)
    return TRUE;
  if (strcmp(optKw, "var") == 0)
    return VAR;
  if (strcmp(optKw, "nil") == 0)
    return NIL;
  if (strcmp(optKw, "and") == 0)
    return AND;
  if (strcmp(optKw, "or") == 0)
    return OR;
  return END_OF_FILE;
}
