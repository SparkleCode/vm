#include <stdio.h>
#include "common.h"
#include "vm.h"
#include "debug.h"

// single static VM instance
VM vm; // TODO - make all vm methods take a vm pointer and get rid of this global variable

static void resetStack() {
  vm.stackTop = vm.stack;
}

// initialize the vm struct
void initVM() {
  resetStack();
}

// free the vm struct
void freeVM() {

}

// run code, core excecution loop - main section of program
// highly performance critical!
static InterpretResult run() {
  // get next byte from chunk
  #define READ_BYTE() (*vm.ip++)
  // get next byte from chunk and use it to get a constant value
  #define READ_CONSTANT() (vm.chunk->constants.values[READ_BYTE()])
  
  for(;;){
    // if debugging, print opcode excecuted
    #ifdef DEBUG_TRACE_EXCECUTION
      printf("Stack: "); // print contents of stack
      for(Value* slot = vm.stack; slot < vm.stackTop; slot++) {
        printf("[ ");
        printValue(*slot);
        printf(" ]");
      }
      printf("\n");
      disassembleInstruction(vm.chunk, (int)(vm.ip - vm.chunk->code));
    #endif

    uint8_t instruction;
    switch(instruction = READ_BYTE()) {
      // interpret each op code!
      case OP_CONSTANT: {
        Value constant = READ_CONSTANT();
        push(constant);
        printf("\n");
        break;
      }
      case OP_RETURN: {
        printValue(pop());
        printf("\n");
        return INTERPRET_OK;
      }
    }
  }

  #undef READ_CONSTANT
  #undef READ_BYTE
}

// run chunk of code in a virtual machine
InterpretResult interpretChunk(Chunk* chunk) {
  // link the chunk and instruction pointer to the virtual machine
  vm.chunk = chunk;
  vm.ip = vm.chunk->code;
  // run the chunk
  return run();
}

// push value to top of stack and update pointer
void push(Value value) {
  *vm.stackTop = value;
  vm.stackTop++;
}

// return value from tope of stack and update pointer
Value pop() {
  vm.stackTop--;
  return *vm.stackTop;
}