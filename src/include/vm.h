#ifndef VM_H
#define VM_H

#include "chunk.h"

// data required for excecution of a chunk of code
typedef struct {
  Chunk* chunk;
  uint8_t* ip;
} VM;

// possible results of running code, used for proper exit code
typedef enum {
  INTERPRET_OK,
  INTERPRET_COMPILE_ERROR,
  INTERPRET_RUNTIME_ERROR
} InterpretResult;

// create/destroy virtual machine
void initVM();
void freeVM();

// run chunk of code!
InterpretResult interpretChunk(Chunk* chunk);

#endif // VM_H