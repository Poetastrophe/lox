// getline
#ifdef __STDC_ALLOC_LIB__
#define __STDC_WANT_LIB_EXT2__ 1
#else
#define _POSIX_C_SOURCE 200809L
#endif

#include "scanner.h"
#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Her tænker jeg er source ville aldrig være lig null. Heraf kan man gøre dette for at dokumentere det tydeligt
static void execute(char source[static 1]) {
  Scanner *scanner = scanner_from_string(source);
  ScannerResult *result = scanner_run(scanner);
  if (result->errorCount > 0) {
    scanner_print_errors(result);
  } else
    scanner_print_tokens(result->tokens, false);
}

static void run_prompt() {
  char *code = {0}; // getline burde kun virke hvis buffer er null, nu er den null.
  size_t bufferLen = {0}; //bare zero-initialize alting - compilers er kloge nok til at regne ud at det kan optimeres væk og hvis du kommer til at misbruge variablen, så er det nemmere at finde fejlen.
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
