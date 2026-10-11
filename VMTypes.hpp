#pragma once
#include <string>
#include <vector>
#include <deque>
#include <unordered_map>
#include <stack>

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
  EndInstructionFlag,
  EndExecutionFlag,
  ChangeModeFlag,
};
struct RawMemoryAddress{
    std::string dat; 
    DataTypes DTP; 
};

struct BCToken{
  std::string LT;
  enum BCT ByteCodeType;
  union{
    enum Instructions Instruction;
    enum KeyWords KeyWord;
    enum DataTypes DataType;
  } Sub_Type;
};

struct VDT{
  int VID; 
  DataTypes DT; // Data Type
  std::deque<int> MAA; // Memory Adresses Assigned
  bool Immutable = false;  
};
namespace VMM{
inline int TotalMemoryDeclared; 
inline std::stack<int> MemoryPool;  
inline std::vector<BCToken> ByteCode; 
inline int BCP;  
inline std::unordered_map<int, VDT> IdentifierMap;
};
