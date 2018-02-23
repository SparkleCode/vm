#include <stdio.h>

#include "debug.h"

void disassembleChunk(Chunk* chunk, const char* name) {
  printf("=== %s ===\n", name);

  for(int i = 0; i < chunk->count;){
    i = disassembleInstruction(chunk, i);
  }
}

static int SimpleInstruction(const char* name, int offset) {
  printf("%s\n", name);
  return offset + 1;
}

int disassembleInstruction(Chunk* chunk, int offset) {
  printf("%04d ", offset);
  uint8_t instruction = chunk->code[offset];
  switch(instruction){
    case OP_RETURN:
      return SimpleInstruction("OP_RETURN", offset);
    default:
      printf("Unknown Opcode %d\n", instruction);
      return offset + 1;
  }
}