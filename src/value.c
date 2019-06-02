#include <stdio.h>

#include "memory.h"
#include "value.h"


// initialize value array struct
void initValueArray(ValueArray* array){
  array->capacity = 0;
  array->count = 0;
  array->values = NULL;
}

// add value to value array
void writeValueArray(ValueArray* array, Value value){
  // does the array need to get bigger, if so realloc it
  if(array->capacity < array->count + 1) {
    int oldCapacity = array->capacity;
    array->capacity = GROW_CAPACITY(oldCapacity);
    array->values = (Value*)GROW_ARRAY(array->values, uint8_t, oldCapacity, array->capacity);
  }
  array->values[array->count] = value;
  array->count++;
}

// free the array
void freeValueArray(ValueArray* array){
  FREE_ARRAY(uint8_t, array->values, array->capacity);
  initValueArray(array);
}

// print a value, used in the dissasembly
void printValue(Value value) {
  switch(value.type) {
    case VAL_BOOL:   printf(AS_BOOL(value)? "true": "false"); break;
    case VAL_NIL:    printf("nill"); break;
    case VAL_NUMBER: printf("%g", AS_NUMBER(value)); break;
  }
}