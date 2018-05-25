#ifndef VM_H
#define VM_H

#include "chunk.h"

typedef struct {
  Chunk* chunk;
  uint8_t* ip;
} VM;

typedef enum {
  INTERPRET_OK,
  INTERPRET_COMPILE_ERROR,
  INTERPRET_RUNTIME_ERROR
} InterpretResult;

void initVM();
void freeVM();

InterpretResult interpretChunk(Chunk* chunk);

#endif // VM_H