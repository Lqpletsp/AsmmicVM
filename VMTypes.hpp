#include <string>
#include <vector>
#include <deque>
#include <unordered_map>

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

extern int TotalMemoryDeclared; 
extern std::vector<int> MemoryPool;  
extern std::vector<BCToken> ByteCode; 
extern int BCP;  

extern std::unordered_map<int, VDT> IdentifierMap;

