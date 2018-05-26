#ifndef debug_h
#define debug_h

#include "chunk.h"

// print a dissassembly of a chunk with its name
void disassembleChunk(Chunk* chunk, const char* name);

// prints the dissasembly of an instruction at an offset
// returns a new offset for the start of the next instruction
int disassembleInstruction(Chunk* chunk, int offset);

#endif // debug_h