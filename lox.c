#include "scanner.h"
#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void execute(char *source) {
  Scanner *scanner = scanner_from_string(source);
  ScannerResult *result = scanner_run(scanner);
  if (result->errorCount > 0) {
    scanner_print_errors(result);
  } else
    scanner_print_tokens(result->tokens, false);
}

static void run_prompt() {
  char *code;
  size_t bufferLen;
  while (true) {
    printf("> ");
    if (getline(&code, &bufferLen, stdin) != -1) {
      size_t length = strlen(code);
      assert(length > 0 && code[length - 1] == '\n');
      code[length - 1] = '\0';
      if (strcmp(code, "exit") == 0) {
        exit(EXIT_SUCCESS);
      }
      execute(code);
    }
  }
}

static void run_file(char *filename) {
  Scanner *scanner = scanner_from_file(filename);
  ScannerResult *result = scanner_run(scanner);
  if (result->errorCount > 0) {
    scanner_print_errors(result);
  } else
    scanner_print_tokens(result->tokens, false);
}

int main(int argc, char **argv) {

  if (argc >= 3) {
    printf("usage: ./lox [program.lox]");
    return 64;
  } else if (argc == 2) {
    run_file(argv[1]);
  } else {
    run_prompt();
  }

  return 0;
}
