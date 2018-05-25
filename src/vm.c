#include <stdio.h>
#include "common.h"
#include "vm.h"
#include "debug.h"

VM vm;

void initVM() {

}

void freeVM() {

}

static InterpretResult run() {
  #define READ_BYTE() (*vm.ip++)
  #define READ_CONSTANT() (vm.chunk->constants.values[READ_BYTE()])
  
  for(;;){
    #ifdef DEBUG_TRACE_EXCECUTION
      disassembleInstruction(vm.chunk, (int)(vm.ip - vm.chunk->code));
    #endif

    uint8_t instruction;
    switch(instruction = READ_BYTE()) {
      case OP_CONSTANT: {
        Value constant = READ_CONSTANT();
        printValue(constant);
        printf("\n");
        break;
      }
      case OP_RETURN: {
        return INTERPRET_OK;
      }
    }
  }

  #undef READ_CONSTANT
  #undef READ_BYTE
}

InterpretResult interpretChunk(Chunk* chunk) {
  vm.chunk = chunk;
  vm.ip = vm.chunk->code;
  return run();
}