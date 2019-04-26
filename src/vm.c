#include <stdio.h>
#include "common.h"
#include "vm.h"
#include "debug.h"
#include "compiler.h"

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

  // consume binary operator's arguments and then do the operator
  #define BINARY_OP(op) \
    do { \
      double b = pop(); \
      double a = pop(); \
      push(a op b); \
    } while(false)
  
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
        // get constant value and add to the stack
        Value constant = READ_CONSTANT();
        push(constant);
        break;
      }
      case OP_NEGATE: {
        push(-pop());
        break;
      }
      case OP_ADD: BINARY_OP(+); break;
      case OP_SUBTRACT: BINARY_OP(-); break;
      case OP_MULTIPLY: BINARY_OP(*); break;
      case OP_DIVIDE: BINARY_OP(/); break;
      case OP_RETURN: {
        printValue(pop());
        printf("\n");
        return INTERPRET_OK;
      }
    }
  }

  #undef BINARY_OP
  #undef READ_CONSTANT
  #undef READ_BYTE
}

InterpretResult interpret(const char* source) {
  Chunk chunk;
  initChunk(&chunk);

  if(!compile(source, &chunk)) {
    freeChunk(&chunk);
    return INTERPRET_COMPILE_ERROR;
  }
  vm.chunk = &chunk;
  vm.ip = vm.chunk->code;

  InterpretResult result = run();

  freeChunk(&chunk);
  return result;
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