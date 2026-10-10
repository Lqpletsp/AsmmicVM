#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

enum BCT { // BCT -> Bytecode Types
  Constant,
  Identifier,
  VMInstruction,
  VMKeywords,
};

enum DataTypes {
  INTEGER,
  DOUBLE,
  BOOLEAN,
  CHAR,
};

enum KeyWords {
  TRUE,
  FALSE,
  Arr,
  ArrEND,
  EndInstructionFlag,
  EndExecutionFlag,
  ChangeModeFlag,
  StopFlag,
  LESSTHAN,
  GREATERTHAN,
  LESSEQUAL,
  GREATEREQUAL,
  EQUAL,
  NOTEQUAL,
  AND,
  OR,
  ADD,
  MINUS,
  MULTIPLY,
  DIVIDE,
  EXPONENT,
};

enum Instructions {
  SET,
  GOTOLN,
  OUTPUT,
  INPUT,
  MLC,
};

typedef struct {
  char *dat;
  enum DataTypes DTP;
} RawMemoryAddress;

typedef struct {
  char *LT;
  enum BCT ByteCodeType;
  union {
    enum Instructions Instruction;
    enum KeyWords KeyWord;
    enum DataTypes DataType;
  } Sub_Type;
} BCToken;

extern int TotalMemoryDeclared; 
extern int *MemoryPool;  
extern BCToken *ByteCode;
extern int BCP;  


inline void CleanupPointers() {
  free(ByteCode);
  free(MemoryPool);
}