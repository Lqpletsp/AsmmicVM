#include "VM.hpp"

bool VMF::VirtualMachine::iniMemmory(int x){
    try{
       VMF::VirtualMachine::TotalMemoryDeclared = x; 
       for (size_t i = TotalMemoryDeclared; i > 0; --i){
            VMF::VirtualMachine::VirtualVolatileMemory.push_back({});
            VMF::VirtualMachine::g_MemoryPool.push(i);
       }
       VMF::VirtualMachine::BCP = 0; 
    } catch(...){
        return false; 
    }
    return true; 
}

void VMF::VirtualMachine::SetIdentifierMap(std::unordered_map<int, Dat::VDT> *Map){
    IdentifierMap = Map;
}

void VMF::VirtualMachine::ExecuteByteCode(){} 