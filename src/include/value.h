#ifndef value_h
#define value_h

#include "common.h"


typedef double Value;

// list of values - dynamic array
typedef struct {
  int capacity;
  int count;
  Value* values;
} ValueArray;

// value dynamic array manipulation methods
void initValueArray(ValueArray* array);
void writeValueArray(ValueArray* array, Value value);
void freeValueArray(ValueArray* array);
void printValue(Value value);

#endif // value_h