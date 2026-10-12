#include <stack>
#include <vector>
#include <string>
#include <deque>
#include <unordered_map>
namespace VMF{
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
        DataTypes DT; 
        std::string Data;
    };

    struct VDT{
        int VID; // Variable ID 
        DataTypes DT; // Data Type
        std::deque<int> MAA; // Memory Adresses Assigned
        bool Immutable = false;  
    };

    class VirtualMachine{
        public:
            // attributes
            int TotalMemoryDeclared; 
            int BCP; 
            std::vector<RawMemoryAddress> VirtualVolatileMemory; 
            std::unordered_map<int, VDT> IdentifierMap; 
            std::stack<int> g_MemoryPool;
            //methods
            bool iniMemmory(int x);
            bool AppendIdentifierMap(int VID, std::string &DataType);
            void ExecuteByteCode();
        private: 
            void set();
            void input(); 
            void output(); 
    };
}
