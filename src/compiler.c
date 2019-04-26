#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "common.h"
#include "compiler.h"
#include "scanner.h"

#ifdef DEBUG_PRINT_CODE
#include "debug.h"
#endif

typedef struct Parser {
  Token current;
  Token previous;
  bool hadError;
  bool panicMode;
  Chunk* compilingChunk;
} Parser;

typedef enum Precidence {
  PREC_NONE,
  PREC_ASSIGNMENT,
  PREC_OR,
  PREC_AND,
  PREC_EQUALITY,
  PREC_COMPARISON,
  PREC_TERM,
  PREC_FACTOR,
  PREC_UNARY,
  PREC_CALL,
  PREC_PRIMARY
} Precidence;

typedef void (*ParseFn)();

typedef struct ParseRule {
  ParseFn prefix;
  ParseFn infix;
  Precidence precidence;
} ParseRule;

Parser parser;

static void advance();
static void consume(TokenType type, const char* message);
static void error(const char* message);
static void errorAtCurrent(const char* message);
static void errorAt(Token* token, const char* message);
static Chunk* currentChunk();
static void endCompiler();
static uint8_t makeConstant(Value value);
static void emitByte(uint8_t byte);
static void emitBytes(uint8_t byte1, uint8_t byte2);
static void emitReturn();
static void emitConstant(Value value);
void expression();
static void parsePrecidence(Precidence precidence);
static void number();
static void grouping();
static void unary();
static void binary();
static ParseRule* getRule(TokenType type);

bool compile(const char* source, Chunk* chunk) {
  initScanner(source);

  parser.compilingChunk = chunk;
  parser.hadError = false;
  parser.panicMode = false;

  advance();
  expression();
  consume(TOKEN_EOF, "Expect end of expression");

  endCompiler();
  return !parser.hadError;
}

void expression() {
  parsePrecidence(PREC_ASSIGNMENT);
}

static void advance() {
  parser.previous = parser.current;

  for(;;) {
    parser.current = scanToken();
    if(parser.current.type != TOKEN_ERROR) break;

    errorAtCurrent(parser.current.start);
  }
}

static void consume(TokenType type, const char* message) {
  if(parser.current.type == type) {
    advance();
    return;
  }

  errorAtCurrent(message);
}

static void errorAtCurrent(const char* message) {
  errorAt(&parser.current, message);
}

static void error(const char* message) {
  errorAt(&parser.previous, message);
}

static void errorAt(Token* token, const char* message) {
  parser.panicMode = true;

  fprintf(stderr, "[line %d] Error", token->line);

  if(token->type == TOKEN_EOF) {
    fprintf(stderr, " at end");
  } else if (token->type == TOKEN_ERROR) {

  } else {
    fprintf(stderr, " at '%.*s'", token->length, token->start);
  }
  fprintf(stderr, ": %s\n", message);
  parser.hadError = true;
}

static Chunk* currentChunk() {
  return parser.compilingChunk;
}

static void endCompiler() {
  emitReturn();
  #ifdef DEBUG_PRINT_CODE
  if(!parser.hadError) {
    disassembleChunk(currentChunk(), "code");
  }
  #endif
}

static uint8_t makeConstant(Value value) {
  int constant = addConstant(currentChunk(), value);
  if(constant > UINT8_MAX) {
    error("Too many constants in one chunk.");
    return 0;
  }
  return (uint8_t)constant;
}

static void parsePrecidence(Precidence precidence) {
  advance();
  ParseFn prefixRule = getRule(parser.previous.type)->prefix;
  if(prefixRule == NULL) {
    error("Expect expression.");
  }

  prefixRule();

  while(precidence <= getRule(parser.current.type)->precidence) {
    advance();
    ParseFn infixRule = getRule(parser.previous.type)->infix;
    infixRule();
  }
}

// parslets
static void number() {
  double value = strtod(parser.previous.start, NULL);
  emitConstant(value);
}
static void grouping() {
  expression();
  consume(TOKEN_RIGHT_PAREN, "Expect ')' after expression.");
}
static void unary() {
  TokenType operatorType = parser.previous.type;

  parsePrecidence(PREC_UNARY);

  switch(operatorType){
    case TOKEN_MINUS: emitByte(OP_NEGATE); break;
  }
}
static void binary() {
  TokenType operatorType = parser.previous.type;

  ParseRule* rule = getRule(operatorType);
  parsePrecidence((Precidence)(rule->precidence + 1));

  switch(operatorType) {
    case TOKEN_PLUS:  emitByte(OP_ADD); break;
    case TOKEN_MINUS: emitByte(OP_SUBTRACT); break;
    case TOKEN_STAR:  emitByte(OP_MULTIPLY); break;
    case TOKEN_SLASH: emitByte(OP_DIVIDE); break;
  }
}

// parser
ParseRule rules[] = {
  { grouping, NULL,    PREC_CALL },       // TOKEN_LEFT_PAREN      
  { NULL,     NULL,    PREC_NONE },       // TOKEN_RIGHT_PAREN     
  { NULL,     NULL,    PREC_NONE },       // TOKEN_LEFT_BRACE
  { NULL,     NULL,    PREC_NONE },       // TOKEN_RIGHT_BRACE     
  { NULL,     NULL,    PREC_NONE },       // TOKEN_COMMA           
  { NULL,     NULL,    PREC_CALL },       // TOKEN_DOT             
  { unary,    binary,  PREC_TERM },       // TOKEN_MINUS           
  { NULL,     binary,  PREC_TERM },       // TOKEN_PLUS            
  { NULL,     NULL,    PREC_NONE },       // TOKEN_SEMICOLON       
  { NULL,     binary,  PREC_FACTOR },     // TOKEN_SLASH           
  { NULL,     binary,  PREC_FACTOR },     // TOKEN_STAR            
  { NULL,     NULL,    PREC_NONE },       // TOKEN_BANG            
  { NULL,     NULL,    PREC_EQUALITY },   // TOKEN_BANG_EQUAL      
  { NULL,     NULL,    PREC_NONE },       // TOKEN_EQUAL           
  { NULL,     NULL,    PREC_EQUALITY },   // TOKEN_EQUAL_EQUAL     
  { NULL,     NULL,    PREC_COMPARISON }, // TOKEN_GREATER         
  { NULL,     NULL,    PREC_COMPARISON }, // TOKEN_GREATER_EQUAL   
  { NULL,     NULL,    PREC_COMPARISON }, // TOKEN_LESS            
  { NULL,     NULL,    PREC_COMPARISON }, // TOKEN_LESS_EQUAL      
  { NULL,     NULL,    PREC_NONE },       // TOKEN_IDENTIFIER      
  { NULL,     NULL,    PREC_NONE },       // TOKEN_STRING          
  { number,   NULL,    PREC_NONE },       // TOKEN_NUMBER          
  { NULL,     NULL,    PREC_AND },        // TOKEN_AND             
  { NULL,     NULL,    PREC_NONE },       // TOKEN_CLASS           
  { NULL,     NULL,    PREC_NONE },       // TOKEN_ELSE            
  { NULL,     NULL,    PREC_NONE },       // TOKEN_FALSE           
  { NULL,     NULL,    PREC_NONE },       // TOKEN_FOR             
  { NULL,     NULL,    PREC_NONE },       // TOKEN_FUN             
  { NULL,     NULL,    PREC_NONE },       // TOKEN_IF              
  { NULL,     NULL,    PREC_NONE },       // TOKEN_NIL             
  { NULL,     NULL,    PREC_OR },         // TOKEN_OR              
  { NULL,     NULL,    PREC_NONE },       // TOKEN_PRINT           
  { NULL,     NULL,    PREC_NONE },       // TOKEN_RETURN          
  { NULL,     NULL,    PREC_NONE },       // TOKEN_SUPER           
  { NULL,     NULL,    PREC_NONE },       // TOKEN_THIS            
  { NULL,     NULL,    PREC_NONE },       // TOKEN_TRUE            
  { NULL,     NULL,    PREC_NONE },       // TOKEN_VAR             
  { NULL,     NULL,    PREC_NONE },       // TOKEN_WHILE           
  { NULL,     NULL,    PREC_NONE },       // TOKEN_ERROR           
  { NULL,     NULL,    PREC_NONE },       // TOKEN_EOF
};

static ParseRule* getRule(TokenType type) {
  return &rules[type];
}

// emitters
static void emitByte(uint8_t byte) {
  writeChunk(currentChunk(), byte, parser.previous.line);
}
static void emitBytes(uint8_t byte1, uint8_t byte2) {
  emitByte(byte1);
  emitByte(byte2);
}
static void emitReturn() {
  emitByte(OP_RETURN);
}
static void emitConstant(Value value) {
  emitBytes(OP_CONSTANT, makeConstant(value));
}