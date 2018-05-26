#include <stdlib.h>

#include "chunk.h"
#include "memory.h"
#include "value.h"

// create a new chunk, intitalize the code array,
// line number runtime-length encoded array and the value array.
void initChunk(Chunk* chunk) {
  chunk->count = 0;
  chunk->capacity = 0;
  chunk->code = NULL;
  chunk->lineCount = 0;
  chunk->lineCapacity = 0;
  chunk->lines = NULL;
  initValueArray(&chunk->constants);
}

// free a chunk and all its included arrays
void freeChunk(Chunk* chunk){
  FREE_ARRAY(uint8_t, chunk->code, chunk->capacity);
  FREE_ARRAY(int, chunk->lines, chunk->lineCapacity);
  freeValueArray(&chunk->constants);
  initChunk(chunk);
}

// add operation to chunk along with its line number for debugging if
// the user's code has an error.
void writeChunk(Chunk* chunk, uint8_t byte, int line) {
  // does the code array need to grow, if so increase its size
  if(chunk->capacity < chunk->count + 1) {
    int oldCapacity = chunk->capacity;
    chunk->capacity = GROW_CAPACITY(oldCapacity);
    chunk->code = GROW_ARRAY(chunk->code, uint8_t, oldCapacity, chunk->capacity);
  }

  // add byte to the code
  chunk->code[chunk->count] = byte;
  chunk->count++;

  // if code originated on the same line as last byte, return
  if(chunk->lineCount > 0 && chunk->lines[chunk->lineCount - 1].line == line){
    return;
  }

  // otherwise need to add a new line marker for the runtime-length encoding
  // does the line number array need to grow? If so, increase its size.
  if(chunk->lineCapacity < chunk->lineCount + 1) {
    int oldCapacity = chunk->lineCapacity;
    chunk->lineCapacity = GROW_CAPACITY(oldCapacity);
    chunk->lines = GROW_ARRAY(chunk->lines, LineStart, oldCapacity, chunk->lineCapacity);
  }

  // add the new line marker to the array
  LineStart* linestart = &chunk->lines[chunk->lineCount++];
  linestart->offset = chunk->count - 1;
  linestart->line = line;
}

// ad a constant to the value array and return its id
int addConstant(Chunk* chunk, Value value){
  writeValueArray(&chunk->constants, value);
  return chunk->constants.count - 1;
}

// return the line that an instruction is on,
// decode the runtime length encodeing
// this is slightly slow as it has to iterate the chunk, 
//however it only needs to occur on an error so it is not performance critical
int getLine(Chunk* chunk, int instructionOffset) {
  int start = 0;
  int end = chunk->lineCount - 1;

  // loop through chunk using binary search until instruction offset reached
  for(;;){
    int mid = (start + end) / 2;
    LineStart* line = &chunk->lines[mid];
    if(instructionOffset < line->offset){
      end = mid - 1;
    } else if(mid == chunk->lineCount-1 || instructionOffset < chunk->lines[mid+1].offset){
      return line->line;
    } else {
      start = mid + 1;
    }
  }
}