#include "scanner.h"
#include "token.h"
#include "tokentype.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

////////////
// Static //
////////////

static ScannerError *scanner_error_init(int line, int column, char *message) {
  ScannerError *error = malloc(sizeof(ScannerError));
  error->line = line;
  error->column = column;
  error->message = malloc(sizeof(char) * strlen(message));
  error->message = message;
  return error;
}

static bool scanner_is_exhausted(Scanner *scanner) {

  // If the buffer is at the end, return 1
  if (scanner->buffer != NULL && scanner->buffer[0] == '\0') {
    return 1;
  }

  return 0;
}

static char scanner_peek(Scanner *scanner) {

  if (scanner_is_exhausted(scanner)) {
    return '\0';
  }

  if (scanner->buffer != NULL) {
    return scanner->buffer[0];
  }

  return '\0';
}

static char scanner_advance(Scanner *scanner) {
  if (scanner_is_exhausted(scanner)) {
    return '\0';
  }

  if (scanner->buffer != NULL) {
    char c = scanner->buffer[0];
    scanner->buffer++;
    if (c == '\n') {
      scanner->line++;
      scanner->column = 1;
    } else {
      scanner->column++;
    }
    return c;
  }

  return '\0';
}

static void scanner_error(Scanner *scanner, char *message) {
  ScannerError *serror =
      scanner_error_init(scanner->line, scanner->column, message);
  scanner->errors[scanner->errorCount++] = serror;
}

static Token *scanner_string(Scanner *scanner) {
  char *start = scanner->buffer - 1;
  int column = scanner->column - 1;
  while (!scanner_is_exhausted(scanner) && scanner_peek(scanner) != '"') {
    if (scanner_advance(scanner) == '\n')
      scanner->line++;
  }

  if (scanner_is_exhausted(scanner)) {
    scanner_error(scanner, "Encountered EOF before string termination");
  }

  // consume '"'
  scanner_advance(scanner);
  size_t strLen = scanner->buffer - start;
  char *str = malloc((strLen + 1) * sizeof(char));
  strncpy(str, start, strLen);
  Literal *literal = literal_new(TYPE_STRING, str);
  return token_new(STRING, scanner->line, column, str, literal);
}

static bool scanner_is_digit(char c) { return c >= '0' && c <= '9'; }

static bool scanner_is_alpha(char c) {
  return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_';
}

static bool scanner_is_alphanumeric(char c) {
  return scanner_is_alpha(c) || scanner_is_digit(c);
}

static bool scanner_match(Scanner *scanner, char expected) {

  if (scanner_peek(scanner) != expected) {
    return false;
  }
  scanner_advance(scanner);
  return true;
}

static Token *scanner_number(Scanner *scanner) {
  int column = scanner->column - 1;
  char *numberStart = scanner->buffer - 1;
  while (scanner_is_digit(scanner_peek(scanner))) {
    scanner_advance(scanner);
  }
  if (scanner_match(scanner, '.')) {
    while (scanner_is_digit(scanner_peek(scanner))) {
      scanner_advance(scanner);
    }
  }
  size_t numStrLen = scanner->buffer - numberStart;
  char *numStr = malloc((numStrLen + 1) * sizeof(char));
  strncpy(numStr, numberStart, numStrLen);

  double *parsedNumber = malloc(sizeof(double));
  *parsedNumber = atof(numStr);
  Literal *numberLiteral = literal_new(TYPE_NUMBER, parsedNumber);
  return token_new(NUMBER, scanner->line, column, numStr, numberLiteral);
}

static ScannerResult *scannerresult_init() {
  ScannerResult *result = malloc(sizeof(ScannerResult));
  result->errorCount = 0;
  result->tokenCapacity = 8;
  result->tokens = malloc(result->tokenCapacity * sizeof(Token *));
  result->scannerErrors = malloc(8 * sizeof(ScannerError *));
  return result;
}

static void scanner_free(Scanner *scanner) { free(scanner); }

static void scannerPrintError(ScannerError *error) {
  printf("Error at %d,%d: %s\n", error->line, error->column, error->message);
}

static Token *scanner_new_token(TokenType type, Scanner *scanner,
                                char *lexeme) {
  size_t lexemeSize = strlen(lexeme);
  return token_new(type, scanner->line, scanner->column - lexemeSize, lexeme,
                   NULL);
}

static char *scanner_read_alphanum(Scanner *scanner) {
  char *start = scanner->buffer - 1;
  while (!scanner_is_exhausted(scanner) &&
         scanner_is_alphanumeric(scanner_peek(scanner))) {
    scanner_advance(scanner);
  }

  size_t len = scanner->buffer - start;
  char *result = malloc((len + 1) * sizeof(char));
  strncpy(result, start, len);
  return result;
}

static Token *scanner_identifier(Scanner *scanner) {

  int column = scanner->column - 1;
  char *identifier = scanner_read_alphanum(scanner);

  if (identifier != NULL && scanner_is_alpha(*identifier)) {
    // Test if possibly a keyword (for, fun, ...)
    TokenType optType = type_from_kw(identifier);
    if (optType != END_OF_FILE) {
      return token_new(optType, scanner->line, scanner->column, identifier,
                       NULL);
    }
  }

  Literal *identLiteral = literal_new(TYPE_STRING, identifier);
  return token_new(IDENTIFIER, scanner->line, column, identifier, identLiteral);
}

static Token *scanner_scan_token(Scanner *scanner) {
  if (scanner_is_exhausted(scanner)) {
    return NULL;
  }
  while (!scanner_is_exhausted(scanner)) {
    char symbol = scanner_advance(scanner);
    switch (symbol) {
    case '(':
      return scanner_new_token(LEFT_PAREN, scanner, "(");
    case ')':
      return scanner_new_token(RIGHT_PAREN, scanner, ")");
    case '{':
      return scanner_new_token(LEFT_BRACE, scanner, "{");
    case '}':
      return scanner_new_token(RIGHT_BRACE, scanner, "}");
    case ',':
      return scanner_new_token(COMMA, scanner, ",");
    case '.':
      return scanner_new_token(DOT, scanner, ".");
    case '+':
      return scanner_match(scanner, '+')
                 ? scanner_new_token(PLUSPLUS, scanner, "++")
                 : scanner_new_token(PLUS, scanner, "+");
    case '-':
      return scanner_match(scanner, '-')
                 ? scanner_new_token(MINUSMINUS, scanner, "--")
                 : scanner_new_token(MINUS, scanner, "-");
    case ';':
      return scanner_new_token(SEMICOLON, scanner, ";");
    case '*':
      return scanner_new_token(STAR, scanner, "*");
    case '!':
      return scanner_match(scanner, '=')
                 ? scanner_new_token(BANGEQ, scanner, "!=")
                 : scanner_new_token(BANG, scanner, "!");
    case '=':
      return scanner_match(scanner, '=')
                 ? scanner_new_token(EQEQ, scanner, "==")
                 : scanner_new_token(EQUAL, scanner, "=");
    case '<':
      return scanner_match(scanner, '=')
                 ? scanner_new_token(LTEQ, scanner, "<=")
                 : scanner_new_token(LT, scanner, "<");
    case '>':
      return scanner_match(scanner, '=')
                 ? scanner_new_token(GTEQ, scanner, ">=")
                 : scanner_new_token(GT, scanner, ">");
    case '/':
      if (scanner_match(scanner, '/')) {
        while (scanner_peek(scanner) != '\n' &&
               !scanner_is_exhausted(scanner)) {
          scanner_advance(scanner);
        }
      } else {
        return scanner_new_token(SLASH, scanner, "/");
      }
      break;
    case ' ':
    case '\r':
    case '\t':
      // ignore whitespace
      break;
    case '\n':
      scanner->line++;
      scanner->column = 0;
      break;
    case '"':
      return scanner_string(scanner);
    default:
      if (scanner_is_digit(symbol)) {
        return scanner_number(scanner);
      }

      if (symbol == '_' || scanner_is_alpha(symbol)) {
        return scanner_identifier(scanner);
      }
    }
  }
  return token_new(END_OF_FILE, scanner->line, scanner->column, "", NULL);
}

////////////////////////
// Header definitions //
////////////////////////

ScannerResult *scanner_run(Scanner *scanner) {
  ScannerResult *result = scannerresult_init();
  size_t index = 0;
  while (!scanner_is_exhausted(scanner)) {
    Token *token = scanner_scan_token(scanner);
    if (token == NULL) {
      assert(token != NULL);
    }
    if (index > result->tokenCapacity) {
      result->tokenCapacity = result->tokenCapacity * 2;
      Token **newTokenBlock = malloc(sizeof(Token *) * result->tokenCapacity);
      for (size_t i = 0; i < index; i++) {
        newTokenBlock[i] = result->tokens[i];
      }
      free(result->tokens);
      result->tokens = newTokenBlock;
    }
    result->tokens[index++] = token;
  }
  result->errorCount = scanner->errorCount;
  result->scannerErrors = scanner->errors;
  scanner_free(scanner);
  return result;
}

Scanner *scanner_from_file(const char *filename) {
  Scanner *scanner = malloc(sizeof(Scanner));
  FILE *file = fopen(filename, "r");
  char c;
  size_t idx = 0;
  fseek(file, 0L, SEEK_END);
  long fileSize = ftell(file);
  scanner->buffer = malloc(sizeof(char) * (fileSize + 1));
  rewind(file);
  while ((c = fgetc(file)) != EOF)
    scanner->buffer[idx++] = c;
  scanner->buffer[fileSize] = '\0';
  scanner->line = 1;
  scanner->column = 1;
  scanner->errorCount = 0;
  scanner->errors = malloc(16 * sizeof(ScannerError *));
  return scanner;
}

Scanner *scanner_from_string(char *code) {
  Scanner *scanner = malloc(sizeof(Scanner));
  scanner->buffer = code;
  scanner->line = 1;
  scanner->column = 1;
  scanner->errorCount = 0;
  scanner->errors = malloc(16 * sizeof(ScannerError *));
  return scanner;
}

void scanner_print_errors(ScannerResult *result) {
  for (size_t i = 0; i < result->errorCount; i++)
    scannerPrintError(result->scannerErrors[i]);
}

void scanner_print_tokens(Token **tokens, bool debug) {
  printf("[");
  for (size_t i = 0; tokens[i] != NULL && tokens[i]->type != END_OF_FILE; i++) {
    printf(" %s%s", token_string(tokens[i], debug), debug ? "\n" : "");
  }
  printf("]\n");
}
