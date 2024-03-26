#ifndef TOKEN_H
#define TOKEN_H

#include "stdbool.h"
#include "tokentype.h"
#include <stdio.h>
#include <stdlib.h>

typedef enum { TYPE_STRING, TYPE_NUMBER } DataType;

typedef struct {
  DataType type;
  union {
    char *string;
    double number;
  } data;
} Literal;

typedef struct {
  TokenType type;
  int line;
  int column;
  char *lexeme;
  Literal *literal;
} Token;

Token *token_new(TokenType type, int line, int column, char *lexeme,
                 Literal *literal);
void token_free(Token *token);
Literal *literal_new(DataType type, void *data);
char *token_debug_string(Token *token);
char *token_string(Token *token, bool debug);
char *literal_string(Literal *literal);

#endif
