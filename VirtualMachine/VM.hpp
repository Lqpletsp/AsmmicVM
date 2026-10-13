#include <stack>
#include <unordered_map>
#include "../DTP.hpp"
namespace VMF{


    class VirtualMachine{
        public:
            //methods
            bool iniMemmory(int x);
            void ExecuteByteCode();
            int SetMemory(int MemoryDeclared); 
            void SetIdentifierMap(std::unordered_map<int, Dat::VDT> *Map); 
        private: 
            // attributes
            int TotalMemoryDeclared; 
            int BCP; 
            std::vector<Dat::RawMemoryAddress> VirtualVolatileMemory; 
            std::unordered_map<int, Dat::VDT> *IdentifierMap; 
            std::stack<int> g_MemoryPool;
            //methods
            void set();
            void input(); 
            void output(); 
    };
}
