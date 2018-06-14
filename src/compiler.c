#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "common.h"
#include "compiler.h"
#include "scanner.h"

static void printStr(const char* start, int length);

void compile(const char* source) {
  initScanner(source);

  int line = -1;
  for(;;){
    Token token = scanToken();
    if(token.line != line) {
      printf("%4d ", token.line);
      line = token.line;
    } else {
      printf("   | ");
    }
    printf("%s '", tokenNames[token.type]);
    //printf("%.*s", token.length, token.start);
    printStr(token.start, token.length);
    printf("'\n");

    if(token.type == TOKEN_EOF) break;
  }
}

static void printStr(const char* start, int length) {
  char* buffer = malloc(length * sizeof(char));
  int processedCount = 0;
  for(int i = 0; i < length; i++) {
    char c = start[i];
    if(c == '\n') {
      printf("%.*s\\n", processedCount, buffer);
      processedCount = 0;
    } else {
      buffer[processedCount] = c;
      processedCount++;
    }
  }
  printf("%.*s", processedCount, buffer);
  free(buffer);
}