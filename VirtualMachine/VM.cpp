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
void VirtualMachine::AllocateMemoryInWBuffer(){WBuffer.push_back(GBuffer0);}
void VirtualMachine::AllocateMemoryInCBuffer(){CBuffer.push(GBuffer1);}
void VirtualMachine::AllocateMemoryInIBuffer(){IBuffer.push_back(GBuffer2);}
Dat::RawMemoryAddress* VirtualMachine::GetMemorySpace(int Address){return  &VirtualVolatileMemory.at(Address);} 
void VirtualMachine::SetIdentifierMap(std::unordered_map<int, Dat::VDT> *Map){IdentifierMap = Map;}

void VirtualMachine::ClearTopCBuffer(){
    if (CBuffer.empty()) HandleException(VirtualMachine::VMExceptions::EmptyCBuffer); 
    ReallocVMemory(CBuffer.top());
    CBuffer.pop(); 
}
void VirtualMachine::ClearTopIBuffer(){
    if (IBuffer.empty()) HandleException(VirtualMachine::VMExceptions::EmptyIBuffer); 
    ReallocVMemory(IBuffer.front());
    IBuffer.pop_front(); 
}

void VirtualMachine::ClearTopWBuffer(){
    if (WBuffer.empty()) HandleException(VirtualMachine::VMExceptions::EmptyWBuffer); 
    ReallocVMemory(WBuffer.front());
    WBuffer.pop_front(); 
}

void VirtualMachine::GetTopWBufferAddress(){
    if (WBuffer.empty()) HandleException(VirtualMachine::VMExceptions::EmptyWBuffer);
    GBuffer1 = WBuffer.front(); 
}
void VirtualMachine::GetTopCBufferAdddress(){
    if (CBuffer.empty()) HandleException(VirtualMachine::VMExceptions::EmptyCBuffer); 
    GBuffer1 = CBuffer.top(); 
}
void VirtualMachine::GetTopIBufferAddress(){
    if (CBuffer.empty()) HandleException(VirtualMachine::VMExceptions::EmptyIBuffer); 
    GBuffer1 = IBuffer.front(); 
}
void VirtualMachine::GetXIBufferAddress(int idx){
    if (idx >= IBuffer.size()) HandleException(VMExceptions::OutOfBoundsInIBuffer); 
    GBuffer1 = WBuffer.at(idx); 
}
void VirtualMachine::GetXWBufferAddress(int idx){
    if (idx >= WBuffer.size()) HandleException(VMExceptions::OutOfBoundsInWBuffer); 
    GBuffer1 = WBuffer.at(idx); 
}

void VirtualMachine::AllocateMemory(){
    if (g_MemoryPool.empty()) HandleException(VirtualMachine::VMExceptions::RanOutOfVirtualVolatileMemory); 
    GBuffer0 = g_MemoryPool.top();
    g_MemoryPool.pop(); 
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
std::string VirtualMachine::GetAddressData(int address){
    return GetMemorySpace(address)->Data;  
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
void VirtualMachine::StoreInAddr(const std::string &Data, const Dat::DataTypes &DTP, const int addr){
    VirtualVolatileMemory.at(addr).Data = Data; 
    VirtualVolatileMemory.at(addr).DT = DTP; 
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
    LoadInIBuffer("[EXCP@BCP" + std::to_string(BCP) + "] : " + VirtualMachine::ExceptionMessage.at(exp), Dat::STRING);
    GBuffer1 = std::string("[EXCP@BCP]" + std::to_string(BCP) + "] : ").size(); 
    GBuffer2 = 0; 
    GBuffer3 = VirtualMachine::ExceptionMessage.at(exp).size() + GBuffer1;
    while (GBuffer2 != GBuffer3){
        OUTPUT(); 
        ++GBuffer2; 
    }
    std::exit(1);
}
void VirtualMachine::GetByteCodeValue(){

}
void VirtualMachine::OUTPUT(){
    GetTopIBufferAddress(); 
    std::cout << GetAddressData(GBuffer1); 
    ClearTopIBuffer(); 
}
void VirtualMachine::SET_IN_BUFFER(){
    ++BCP;
}
int VirtualMachine::ExecuteByteCode(){
    while (BCP < Dat::ByteCode.size()){
        Dat::ByteCodeType BCT = Dat::ByteCode.at(BCP); 
        switch(BCT.TType){
            case (Dat::VMInstruction):{
                switch(BCT.Sub_TType.VMInstruction){
                    case Dat::OUTPUT: 
                        OUTPUT(); 
                        break; 
                    case Dat::SETV: 
                    case Dat::SETB:
                    case Dat::MLC: 
                    case Dat::EndExecutionFlag: 
                    case Dat::EndInstructionFlag: 
                    case Dat::GOTOLN: 
                    case Dat::INPUT: 
                    case Dat::ChangeModeFlag:
                        break;
                }
            }
            default: break; 
        }
        ++BCP; 
    }
    return 0;} 
}