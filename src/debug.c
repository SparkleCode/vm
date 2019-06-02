#include <stdio.h>

#include "debug.h"
#include "value.h"

// print dissasembaly of a chunk of code
void disassembleChunk(Chunk* chunk, const char* name) {
  printf("=== %s ===\n", name);

  // iterate, increasing i by length of printed instruction in bytecode
  for(int i = 0; i < chunk->count;){
    i = disassembleInstruction(chunk, i);
  }

  printf("=== end %s ===\n", name);
}

// print simple instruction, return new offset
static int SimpleInstruction(const char* name, int offset) {
  printf("%s\n", name);
  return offset + 1;
}

// print constant instruction and its constant from the value array
// return new offset
static int ConstantInstruction(const char* name, Chunk* chunk, int offset) {
  uint8_t constant = chunk->code[offset + 1];
  printf("%-16s %4d '", name, constant);
  printValue(chunk->constants.values[constant]);
  printf("'\n");
  return offset + 2;
}

// disasemble single instruction, print dissasembaly and return new offset
int disassembleInstruction(Chunk* chunk, int offset) {
  // print offset
  printf("%04d ", offset);

  // print line neumber of instruction
  int line = getLine(chunk, offset);
  if(offset > 0 && line == getLine(chunk, offset - 1)){
    printf("   | ");
  } else {
    printf("%4d ", line);
  }

  // map op code to instruction
  uint8_t instruction = chunk->code[offset];
  switch(instruction){
    case OP_RETURN:
      return SimpleInstruction("OP_RETURN", offset);
    case OP_NEGATE:
      return SimpleInstruction("OP_NEGATE", offset);
    case OP_ADD:
      return SimpleInstruction("OP_ADD", offset);
    case OP_SUBTRACT:
      return SimpleInstruction("OP_SUBTRACT", offset);
    case OP_MULTIPLY:
      return SimpleInstruction("OP_MULTIPLY", offset);
    case OP_DIVIDE:
      return SimpleInstruction("OP_DIVIDE", offset);
    case OP_NOT:
      return SimpleInstruction("OP_NOT", offset);
    case OP_CONSTANT:
      return ConstantInstruction("OP_CONSTANT", chunk, offset);
    case OP_NIL:
      return SimpleInstruction("OP_NIL", offset);
    case OP_TRUE:
      return SimpleInstruction("OP_TRUE", offset);
    case OP_FALSE:
      return SimpleInstruction("OP_FALSE", offset);
    case OP_EQUAL:
      return SimpleInstruction("OP_EQUAL", offset);
    case OP_GREATER:
      return SimpleInstruction("OP_GREATER", offset);
    case OP_LESS:
      return SimpleInstruction("OP_LESS", offset);
    default:
      printf("Unknown Opcode %d\n", instruction);
      return offset + 1;
  }
}