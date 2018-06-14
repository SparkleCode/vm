#ifndef VM_H
#define VM_H

#include "chunk.h"
#include "value.h"

#define STACK_MAX 256

// data required for excecution of a chunk of code
typedef struct VM {
  Chunk* chunk;
  uint8_t* ip;
  Value stack[STACK_MAX];
  Value* stackTop;
} VM;

// possible results of running code, used for proper exit code
typedef enum InterpretResult {
  INTERPRET_OK,
  INTERPRET_COMPILE_ERROR,
  INTERPRET_RUNTIME_ERROR
} InterpretResult;

// create/destroy virtual machine
void initVM();
void freeVM();

// run source code!
InterpretResult interpret(const char* source);

// stack manipulation methods
void push(Value value);
Value pop();

#endif // VM_H