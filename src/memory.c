#include <stdlib.h>

#include "common.h"
#include "memory.h"

// realoc array or free it if realloc'ing to 0 size
void* reallocate(void* previous, size_t oldSize, size_t newSize){
  if(newSize == 0) {
    free(previous);
    return NULL;
  }
  return realloc(previous, newSize);
}