#ifndef chunk_h
#define chunk_h

#include "common.h"
#include "value.h"

// all operators in the virtual machine
typedef enum OpCode {
    OP_RETURN,
    OP_CONSTANT
} OpCode;

// start of a line in runtime length encoding - used for debug location
typedef struct LineStart {
    int offset;
    int line;
} LineStart;

// a chunk of compiled code, its constants
// and the code->line mapping infomation for debugging
// count/capacity and lineCount/lineCapacity are the array infomation
// for the opcode list and the debugging map list respectively
typedef struct Chunk {
    int count;
    int capacity;
    uint8_t* code;
    ValueArray constants;
    int lineCount;
    int lineCapacity;
    LineStart* lines;
} Chunk;

// create/free a new chunk
void initChunk(Chunk* chunk);
void freeChunk(Chunk* chunk);

// add operation to chunk along with its line number for debugging if
// the user's code has an error.
void writeChunk(Chunk* chunk, uint8_t byte, int line);

// add constant value to chunk and return its id
int addConstant(Chunk* chunk, Value value);

int getLine(Chunk* chunk, int instruction);

#endif // chunk_h