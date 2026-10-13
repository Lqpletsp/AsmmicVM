#include <deque>
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
            // all buffer store virtual memory addresses
            std::deque<int> IBuffer; // Input/Ouput Buffer
            std::deque<int> WBuffer; // Working Buffer
            std::stack<int> CBuffer; // Computational Buffer 
            std::unordered_map<int, Dat::VDT> *IdentifierMap; 
            std::stack<int> g_MemoryPool;
            //methods
            void output();
            int GetTopWBufferAddress();
            std::vector<Dat::RawMemoryAddress>* GetVariableMAA(int VID);
            Dat::RawMemoryAddress* GetMemorySpace(int Address); 
    };
}
