#ifndef value_h
#define value_h

#include "common.h"

typedef enum ValueType {
  VAL_BOOL,
  VAL_NIL,
  VAL_NUMBER
} ValueType;

typedef struct Value {
  ValueType type;
  union {
    bool boolean;
    double number;
  } as;
} Value;

#define BOOL_VAL(value)   ((Value){VAL_BOOL, {.boolean = value}})
#define NIL_VAL           ((Value){VAL_NIL, {.number = 0}})
#define NUMBER_VAL(value) ((Value){VAL_NUMBER, {.number = value}})

#define AS_BOOL(value)   ((value).as.boolean)
#define AS_NUMBER(value) ((value).as.number)

#define IS_BOOL(value)   ((value).type == VAL_BOOL)
#define IS_NIL(value)    ((value).type == VAL_NIL)
#define IS_NUMBER(value) ((value).type == VAL_NUMBER)

// list of values - dynamic array
typedef struct ValueArray {
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