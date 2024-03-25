#include "token.h"

Token *token_new(TokenType type, int line, int column, char *lexeme,
                 Literal *literal)
{
  Token *token = malloc(sizeof(Token));
  token->type = type;
  token->line = line;
  token->column = column;
  token->lexeme = lexeme;
  token->literal = literal;
  return token;
}

Literal *literal_new(DataType type, void *data)
{
  Literal *literal = malloc(sizeof(Literal));
  literal->type = type;
  if (type == TYPE_STRING)
  {
    literal->data.string = (char *)data;
  }
  else if (type == TYPE_NUMBER)
  {
    literal->data.number = *(double *)data;
  }
  return literal;
}

char *literal_string(Literal *literal)
{
  if (literal->type == TYPE_STRING)
  {
    return literal->data.string;
  }
  else if (literal->type == TYPE_NUMBER)
  {
    char *result = malloc(32 * sizeof(char));
    sprintf(result, "%g", literal->data.number);
    return result;
  }
  return NULL;
}

char *token_string(Token *token, bool debug)
{
  if(debug) return token_debug_string(token);
  bool hasLiteral = token->literal != NULL;
  char *type = tokentype_string(token->type);
  char *literal = hasLiteral ? literal_string(token->literal) : NULL;
  char *result = malloc(256);
  sprintf(result, hasLiteral ? "%s(%s)" : "%s", type, literal);
  return result;
}

char *token_debug_string(Token *token)
{
  bool hasLiteral = token->literal != NULL;
  char *type = tokentype_string(token->type);
  char *lexeme = token->lexeme;
  int line = token->line;
  int column = token->column;
  char *result = malloc(256);
  char *literal = hasLiteral ? literal_string(token->literal) : "n/a";
  sprintf(result,
          "Token(type=%s, lexeme='%s', line=%d, column=%d, literal='%s')", type,
          lexeme, line, column, literal);
  return result;
}
