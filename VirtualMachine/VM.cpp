#include "VM.hpp"
#include "DTP.hpp"
#include <iostream>
#include <cstdlib>
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
void VirtualMachine::ReallocVMemory(int addr){g_MemoryPool.push(addr);}
void VirtualMachine::AllocateMemoryInWBuffer(){WBuffer.push_back(AllocateMemory());}
void VirtualMachine::AllocateMemoryInCBuffer(){CBuffer.push(AllocateMemory());}
void VirtualMachine::AllocateMemoryInIBuffer(){IBuffer.push_back(AllocateMemory());}
Dat::RawMemoryAddress* VirtualMachine::GetMemorySpace(int Address){return  &VirtualVolatileMemory.at(Address);} 
void VirtualMachine::SetIdentifierMap(std::unordered_map<int, Dat::VDT> *Map){IdentifierMap = Map;}

int VirtualMachine::ExecuteByteCode(){return 0;} 

int VirtualMachine::GetTopWBufferAddress(){
    if (WBuffer.empty()) HandleException(VirtualMachine::VMExceptions::EmptyWBuffer);
    return WBuffer.front();
}
int VirtualMachine::GetTopCBufferAdddress(){
    if (CBuffer.empty()) HandleException(VirtualMachine::VMExceptions::EmptyCBuffer); 
    return CBuffer.top(); 
}
int VirtualMachine::GetTopIBufferAddress(){
    if (CBuffer.empty()) HandleException(VirtualMachine::VMExceptions::EmptyIBuffer); 
    return IBuffer.front();
}
void VirtualMachine::output(){
    int addr = GetTopWBufferAddress();  
    Dat::RawMemoryAddress* M_Space = GetMemorySpace(addr); 
    std::cout << M_Space->Data;
}
int VirtualMachine::AllocateMemory(){
    if (g_MemoryPool.empty()) HandleException(VirtualMachine::VMExceptions::RanOutOfVirtualVolatileMemory); 
    int addr = g_MemoryPool.top();
    return addr; 
}
void VirtualMachine::ClearCBuffer(){
    while (!CBuffer.empty()){
        ReallocVMemory(CBuffer.top());
        CBuffer.pop();  
    }
}
void VirtualMachine::ClearIBuffer(){
    while (!IBuffer.empty()){
        ReallocVMemory(IBuffer.back()); 
        IBuffer.pop_back();
    }
}
void VirtualMachine::ClearWBuffer(){
    while (!WBuffer.empty()){
        ReallocVMemory(WBuffer.back()); 
        WBuffer.pop_back();
    }
}
void VirtualMachine::StoreInIBuffer(const std::string &Data, const Dat::DataTypes &DTP){
    AllocateMemoryInIBuffer(); 
    if (DTP == Dat::STRING) HandleException(VirtualMachine::VMExceptions::StringDataInIBuffer);
    VirtualVolatileMemory.at(IBuffer.back()).Data = Data; 
    VirtualVolatileMemory.at(IBuffer.back()).DT = DTP; 
}
void VirtualMachine::StoreInWBuffer(const std::string &Data, const Dat::DataTypes &DTP){
    AllocateMemoryInWBuffer(); 
    if (DTP == Dat::STRING) HandleException(VirtualMachine::VMExceptions::StringDataInWBuffer);
    VirtualVolatileMemory.at(WBuffer.back()).Data = Data; 
    VirtualVolatileMemory.at(WBuffer.back()).DT = DTP; 
}
void VirtualMachine::StoreInCBuffer(const std::string &Data, const Dat::DataTypes &DTP){
    AllocateMemoryInCBuffer(); 
    if (DTP == Dat::STRING) HandleException(VirtualMachine::VMExceptions::StringDataInCBuffer);
    VirtualVolatileMemory.at(CBuffer.top()).Data = Data; 
    VirtualVolatileMemory.at(CBuffer.top()).DT = DTP; 
}
void VirtualMachine::LoadInCBuffer(const std::string &Data, const Dat::DataTypes &DTP){
    if (DTP == Dat::DataTypes::STRING){
        for (const char &ch : Data){
            StoreInCBuffer(std::string(1,ch), Dat::DataTypes::CHAR);
        }
    } else{
        StoreInCBuffer(Data, DTP);
    }
}
void VirtualMachine::LoadInWBuffer(const std::string &Data, const Dat::DataTypes &DTP){
    if (DTP == Dat::DataTypes::STRING){
        for (const char &ch : Data){
            StoreInWBuffer(std::string(1,ch), Dat::DataTypes::CHAR);
        }
    } else{
        StoreInWBuffer(Data, DTP);
    }
}
void VirtualMachine::LoadInIBuffer(const std::string &Data, const Dat::DataTypes &DTP){
    if (DTP == Dat::DataTypes::STRING){
        for (const char &ch : Data){
            StoreInIBuffer(std::string(1,ch), Dat::DataTypes::CHAR);
        }
    } else{
        StoreInIBuffer(Data, DTP);
    }
}
void VirtualMachine::BufferCleanUps(){
    ClearCBuffer(); 
    ClearIBuffer(); 
    ClearWBuffer(); 
}
void VirtualMachine::HandleException(VirtualMachine::VMExceptions exp){
    BufferCleanUps();
    LoadInIBuffer(VirtualMachine::ExceptionMessage.at(exp), Dat::STRING);
    LoadInWBuffer("0", Dat::INTEGER); 
    LoadInWBuffer(std::to_string(VirtualMachine::ExceptionMessage.at(exp).size()), Dat::INTEGER); 
    for (size_t i = 0; i < VirtualMachine::ExceptionMessage.at(exp).size(); ++i){
        output(); 
    }
    std::exit(1);
}
}