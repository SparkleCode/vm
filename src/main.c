#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "common.h"
#include "chunk.h"
#include "vm.h"
#include "debug.h"

static void repl();
static void runFile(const char*);
static char* readFile(const char*);

int main(int argc, const char* argv[]) {
  // new Virtual machine
  initVM();

  if(argc == 1) {
    repl();
  } else if(argc == 2) {
    runFile(argv[1]);
  } else {
    fprintf(stderr, "Usage: SparkleCode [path]\n");
    exit(65);
  }

  freeVM();

  return 0;
}

static void repl() {
  char line[1024];
  for(;;){
    printf("> ");

    if(!fgets(line, sizeof(line), stdin)) {
      printf("\n");
      break;
    }

    interpret(line);
  }
}

static void runFile(const char* path) {
  char* file = readFile(path);
  InterpretResult result = interpret(file);
  free(file);

  if(result == INTERPRET_COMPILE_ERROR) exit(65);
  if(result == INTERPRET_RUNTIME_ERROR) exit(70);
}

static char* readFile(const char* path) {
  FILE* file = fopen(path, "rb");
  if(file == NULL){
    fprintf(stderr, "Could not open file \"%s\".\n", path);
    exit(74);
  }
  
  fseek(file, 0L, SEEK_END);
  size_t fileSize = ftell(file);
  rewind(file);

  char* buffer = (char*)malloc(fileSize + 1);
  if(buffer == NULL) {
    fprintf(stderr, "Not enough memory to read \"%s\".\n", path);
    exit(74);
  }
  size_t bytesRead = fread(buffer, sizeof(char), fileSize, file);
  if(bytesRead < fileSize){
    fprintf(stderr, "Could not read file \"%s\".\n", path);
    exit(74);
  }
  buffer[bytesRead] = '\0';

  fclose(file);
  return buffer;
}