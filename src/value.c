#include <stdio.h>

#include "memory.h"
#include "value.h"

void initValueArray(ValueArray* array){
  array->capacity = 0;
  array->count = 0;
  array->values = NULL;
}

void writeValueArray(ValueArray* array, Value value){
  if(array->capacity < array->count + 1) {
    int oldCapacity = array->capacity;
    array->capacity = GROW_CAPACITY(oldCapacity);
    array->values = (Value*)GROW_ARRAY(array->values, uint8_t, oldCapacity, array->capacity);
  }
  array->values[array->count] = value;
  array->count++;
}

void freeValueArray(ValueArray* array){
  FREE_ARRAY(uint8_t, array->values, array->capacity);
  initValueArray(array);
}

void printValue(Value value) {
  printf("%g", value);
}