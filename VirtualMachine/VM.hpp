#include <deque>
#include <stack>
#include <unordered_map>
#include "../DTP.hpp"
namespace VMF{

    class VirtualMachine{
        public:
            //methods
            bool iniMemmory(int x);
            int ExecuteByteCode();
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
            std::vector<Dat::RawMemoryAddress>* GetVariableMAA(int VID);
            Dat::RawMemoryAddress* GetMemorySpace(int Address); 
            void StoreInIBuffer(const std::string &Data, const Dat::DataTypes &DTP); // for non string values  
            void StoreInWBuffer(const std::string &Data, const Dat::DataTypes &DTP); // for non string values 
            void StoreInCBuffer(const std::string &Data, const Dat::DataTypes &DTP); // for non string values 
            void StoreInAddr(const std::string &Data, const Dat::DataTypes &DTP, const int addr); 
            void LoadInWBuffer(const std::string &Data, const Dat::DataTypes &DTP); // for all types of values
            void LoadInIBuffer(const std::string &Data, const Dat::DataTypes &DTP); // for all types of values
            void LoadInCBuffer(const std::string &Data, const Dat::DataTypes &DTP); // for all types of values
            void AllocateMemoryInCBuffer(); 
            void AllocateMemoryInWBuffer(); 
            void AllocateMemoryInIBuffer(); 
            void ReallocVMemory(int addr);
            void ClearIBuffer(); 
            void ClearWBuffer(); 
            void ClearCBuffer();
            void BufferCleanUps(); 
            void ClearTopWBuffer(); 
            void ClearTopCBuffer(); 
            void ClearTopIBuffer(); 
            int AllocateMemory(); 
            int GetTopWBufferAddress();
            int GetTopIBufferAddress(); 
            int GetTopCBufferAdddress(); 
            int GetXWBufferAddress(int idx); 
            int GetXIBufferAddress(int idx); 
            std::string GetAddressData(int address); 
            // exceptions
            enum class VMExceptions{
                EmptyWBuffer, 
                EmptyIBuffer, 
                EmptyCBuffer,  
                RanOutOfVirtualVolatileMemory, 
                StringDataInMemory, 
                StringDataInIBuffer, 
                StringDataInWBuffer, 
                StringDataInCBuffer, 
                OutOfBoundsInIBuffer, 
                OutOfBoundsInWBuffer, 
                UnIdentifiedInstruction, 
            };
            const std::unordered_map<VMExceptions,std::string> ExceptionMessage{
                {VMExceptions::EmptyCBuffer, "Computation buffer empty"},
                {VMExceptions::EmptyIBuffer, "Input/Output buffer empty"}, 
                {VMExceptions::EmptyWBuffer, "Working buffer empty"}, 
                {VMExceptions::RanOutOfVirtualVolatileMemory, "All virtual volatile memory used"},
                {VMExceptions::StringDataInMemory, "String data cannot be stored in memory as a whole"},
                {VMExceptions::OutOfBoundsInIBuffer, "Index given is out of bounds for IBuffer"},
                {VMExceptions::OutOfBoundsInWBuffer, "Index given is out of bounds for WBuffer"}, 
                {VMExceptions::UnIdentifiedInstruction, "Invalid bytecode instruction"}, 
            };
            void HandleException(VirtualMachine::VMExceptions exp); 
    };
}
