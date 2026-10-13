#include "VM.hpp"
#include "DTP.hpp"
#include <iostream>
namespace VMF{
bool VirtualMachine::iniMemmory(int x){
    try{
       TotalMemoryDeclared = x; 
       for (size_t i = TotalMemoryDeclared; i > 0; --i){
            VirtualVolatileMemory.push_back({});
            g_MemoryPool.push(i);
       }
       BCP = 0; 
    } catch(...){
        return false; 
    }
    return true; 
}
Dat::RawMemoryAddress* VirtualMachine::GetMemorySpace(int Address){return  &VirtualVolatileMemory.at(Address);} 
void VirtualMachine::SetIdentifierMap(std::unordered_map<int, Dat::VDT> *Map){IdentifierMap = Map;}

void VirtualMachine::ExecuteByteCode(){} 

int VirtualMachine::GetTopWBufferAddress(){
    if (WBuffer.empty()) {}
    return WBuffer.front();
}

void VirtualMachine::output(){
    int addr = GetTopWBufferAddress();  
    Dat::RawMemoryAddress* M_Space = GetMemorySpace(addr); 
    std::cout << M_Space->Data;
}
}