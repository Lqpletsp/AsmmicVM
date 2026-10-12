#pragma once 
#include <string>
#include <vector>

namespace Dat{
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
    STRING, 
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
    DECV, 
    DECM,
    OUTPUT,
    INPUT,
    MLC,
    EndInstructionFlag,
    EndExecutionFlag,
    ChangeModeFlag,
    };

    struct ByteCodeType{
        std::string LT; 
        BCT TType;  
        union{
            Instructions VMInstruction; 
            KeyWords KeyWord;
            DataTypes DT; 
        } Sub_TType;
    };

    inline std::vector<ByteCodeType> ByteCode;


};
