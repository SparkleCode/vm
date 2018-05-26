#include "common.h"
#include "chunk.h"
#include "vm.h"
#include "debug.h"

int main(int argc, const char* argv[]) {
  // new Virtual machine
  initVM();

  // create debug chunk
  Chunk chunk;
  initChunk(&chunk);
  
  int constant = addConstant(&chunk, 1.2);
  writeChunk(&chunk, OP_CONSTANT, 123);
  writeChunk(&chunk, constant, 123);
  writeChunk(&chunk, OP_RETURN, 123);

  // run chunk
  disassembleChunk(&chunk, "test chunk");
  interpretChunk(&chunk);

  // free everything
  freeChunk(&chunk);
  freeVM();

  return 0;
}