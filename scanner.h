#ifndef SCANNER_H
#define SCANNER_H
#include "token.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
  int line;
  int column;
  char *message;
} ScannerError;

typedef struct {
  char *buffer;
  int line;
  int column;
  int error_count;
  ScannerError **errors;
} Scanner;

typedef struct {
  size_t scanner_error_count;
  ScannerError **scanner_errors;
  size_t token_capacity;
  Token **tokens;
} ScannerResult;

Scanner *scanner_from_file(const char *filename);
Scanner *scanner_from_string(char *code);
ScannerResult *scanner_run(Scanner *scanner);
void scanner_print_tokens(Token **tokens, bool simpleToken);
void scanner_print_errors(ScannerResult *result);

#endif
