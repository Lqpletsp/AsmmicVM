#include "VM.hpp"
#include "DTP.hpp"
#include <iostream>
#include <cstdlib>
#include <cstring>
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
void VirtualMachine::ReallocVMemory(){g_MemoryPool.push(GBuffer1);}
void VirtualMachine::AllocateMemoryInWBuffer(){WBuffer.push_back(GBuffer1);}
void VirtualMachine::AllocateMemoryInCBuffer(){CBuffer.push(GBuffer1);}
void VirtualMachine::AllocateMemoryInIBuffer(){IBuffer.push_back(GBuffer1);}
void VirtualMachine::SetIdentifierMap(std::unordered_map<int, Dat::VDT> *Map){IdentifierMap = Map;}

void VirtualMachine::ClearTopCBuffer(){
    if (CBuffer.empty()) {
        GBuffer0 = static_cast<int>(VMExceptions::EmptyCBuffer);
        HandleException();
    }
    GBuffer1 = CBuffer.top();
    ReallocVMemory();
    CBuffer.pop(); 
}

void VirtualMachine::ClearTopIBuffer(){
    if (IBuffer.empty()) {
        GBuffer0 = static_cast<int>(VMExceptions::EmptyIBuffer);
        HandleException();
    }
    GBuffer1 = IBuffer.front();
    ReallocVMemory();
    IBuffer.pop_front(); 
}

void VirtualMachine::ClearTopWBuffer(){
    if (WBuffer.empty()) {
        GBuffer0 = static_cast<int>(VMExceptions::EmptyWBuffer);
        HandleException();
    }
    GBuffer1 = WBuffer.front();
    ReallocVMemory();
    WBuffer.pop_front(); 
}

void VirtualMachine::GetTopWBufferAddress(){
    if (WBuffer.empty()) {
        GBuffer0 = static_cast<int>(VMExceptions::EmptyWBuffer);
        HandleException();
    }
    GBuffer1 = WBuffer.front(); 
}

void VirtualMachine::GetTopCBufferAddress(){
    if (CBuffer.empty()) {
        GBuffer0 = static_cast<int>(VMExceptions::EmptyCBuffer);
        HandleException();
    }
    GBuffer1 = CBuffer.top(); 
}

void VirtualMachine::GetTopIBufferAddress(){
    if (IBuffer.empty()) {
        GBuffer0 = static_cast<int>(VMExceptions::EmptyIBuffer);
        HandleException();
    }
    GBuffer1 = IBuffer.front(); 
}

void VirtualMachine::GetXIBufferAddress(){
    if (GBuffer2 >= IBuffer.size()) {
        GBuffer0 = static_cast<int>(VMExceptions::OutOfBoundsInIBuffer);
        HandleException();
    }
    GBuffer1 = IBuffer.at(GBuffer2); 
}

void VirtualMachine::GetXWBufferAddress(){
    if (GBuffer2 >= WBuffer.size()) {
        GBuffer0 = static_cast<int>(VMExceptions::OutOfBoundsInWBuffer);
        HandleException();
    }
    GBuffer1 = WBuffer.at(GBuffer2); 
}

void VirtualMachine::AllocateMemory(){
    if (g_MemoryPool.empty()) {
        GBuffer0 = static_cast<int>(VMExceptions::RanOutOfVirtualVolatileMemory);
        HandleException();
    }
    GBuffer0 = g_MemoryPool.top();
    g_MemoryPool.pop(); 
}
void VirtualMachine::ClearCBuffer(){
    while (!CBuffer.empty()){
        GBuffer1 = CBuffer.top(); 
        ReallocVMemory();
        CBuffer.pop();  
    }
}
void VirtualMachine::ClearIBuffer(){
    while (!IBuffer.empty()){
        GBuffer1 = CBuffer.top(); 
        ReallocVMemory(); 
        IBuffer.pop_back();
    }
}
void VirtualMachine::ClearWBuffer(){
    while (!WBuffer.empty()){
        GBuffer1 = CBuffer.top(); 
        ReallocVMemory(); 
        WBuffer.pop_back();
    }
}

void VirtualMachine::LoadInCBuffer(){
    if (DPBuffer.DTP == Dat::DataTypes::STRING){
        GBuffer3 = 0; 
        while (GBuffer3 < 256){
            AllocateMemory(); 
            GBuffer1 = GBuffer0;  
            AllocateMemoryInCBuffer(); 
            VirtualVolatileMemory.at(GBuffer1).Data = DPBuffer.Data[GBuffer3];
            VirtualVolatileMemory.at(GBuffer1).DT = Dat::CHAR;  
            ++GBuffer3; 
        }
    } else{
        AllocateMemory(); 
        GBuffer1 = GBuffer0;  
        AllocateMemoryInCBuffer(); 
        VirtualVolatileMemory.at(GBuffer1).Data = DPBuffer.Data[0]; 
        VirtualVolatileMemory.at(GBuffer1).DT = DPBuffer.DTP; 
    }
}
void VirtualMachine::LoadInWBuffer(){
    if (DPBuffer.DTP == Dat::DataTypes::STRING){
        GBuffer3 = 0; 
        while (GBuffer3 < 256){
            AllocateMemory(); 
            GBuffer1 = GBuffer0;  
            AllocateMemoryInWBuffer(); 
            VirtualVolatileMemory.at(GBuffer1).Data = DPBuffer.Data[GBuffer3];
            VirtualVolatileMemory.at(GBuffer1).DT = Dat::CHAR;  
            ++GBuffer3; 
        }
    } else{
        AllocateMemory(); 
        GBuffer1 = GBuffer0;  
        AllocateMemoryInWBuffer(); 
        VirtualVolatileMemory.at(GBuffer1).Data = DPBuffer.Data[0]; 
        VirtualVolatileMemory.at(GBuffer1).DT = DPBuffer.DTP; 
    }
}
void VirtualMachine::StoreInAddr(){
    VirtualVolatileMemory.at(GBuffer1).Data = DPBuffer.Data[0];  
    VirtualVolatileMemory.at(GBuffer1).DT = DPBuffer.DTP; 
}
void VirtualMachine::LoadInIBuffer(){
    if (DPBuffer.DTP == Dat::DataTypes::STRING){
        GBuffer3 = 0; 
        while (GBuffer3 < 256){
            AllocateMemory(); 
            GBuffer1 = GBuffer0;  
            AllocateMemoryInIBuffer(); 
            VirtualVolatileMemory.at(GBuffer1).Data = DPBuffer.Data[GBuffer3];
            VirtualVolatileMemory.at(GBuffer1).DT = Dat::CHAR;  
            ++GBuffer3; 
        }
    } else{
        AllocateMemory(); 
        GBuffer1 = GBuffer0;  
        AllocateMemoryInIBuffer(); 
        VirtualVolatileMemory.at(GBuffer1).Data = DPBuffer.Data[0]; 
        VirtualVolatileMemory.at(GBuffer1).DT = DPBuffer.DTP; 
    }
}
void VirtualMachine::LoadBCInWBuffer(){
    switch(Dat::ByteCode.at(BCP).TType){
        case Dat::Constant:
            std::strcpy(DPBuffer.Data.data(), Dat::ByteCode.at(BCP).LT.c_str());
            DPBuffer.DTP = Dat::ByteCode.at(BCP).Sub_TType.DT; 
            LoadInWBuffer();
            break;
        case Dat::Identifier: 
            VBuffer = &IdentifierMap->at(std::stoi(Dat::ByteCode.at(BCP).LT)); 
            if (VBuffer->MAA.empty()){
                GBuffer0 = static_cast<int>(VMExceptions::AttemptedAccessToUninitializedVariable); 
                HandleException(); 
            }
            GBuffer2 = 0;
            GBuffer3 = VBuffer->MAA.size();  
            while (GBuffer2 < GBuffer3){
                std::strcpy(DPBuffer.Data.data(), Dat::ByteCode.at(BCP).LT.c_str());
                DPBuffer.DTP = Dat::ByteCode.at(BCP).Sub_TType.DT; 
                LoadInWBuffer();
                ++GBuffer2; 
            }
            break; 
        default: break;  
    }
}
void VirtualMachine::BufferCleanUps(){
    ClearCBuffer(); 
    ClearIBuffer(); 
    ClearWBuffer(); 
}
void VirtualMachine::HandleException(){
    BufferCleanUps();
    GBuffer1 = 0; 
    GBuffer2 = VirtualMachine::ExceptionMessage.at(VirtualMachine::MapExceptionCodeAndMessage.at(GBuffer0)).size(); 
    std::snprintf(
        DPBuffer.Data.data(), 
        DPBuffer.Data.size(), 
        "[EXCP@BCP%d] : %s", 
        BCP, 
        VirtualMachine::ExceptionMessage.at(VirtualMachine::MapExceptionCodeAndMessage.at(GBuffer0)).c_str()
    );
    DPBuffer.DTP = Dat::STRING;
    LoadInIBuffer();
    GBuffer1 = std::string("[EXCP@BCP" + std::to_string(BCP) + "] : ").size(); 
    GBuffer2 = 0; 
    GBuffer3 = VirtualMachine::ExceptionMessage.at(VirtualMachine::MapExceptionCodeAndMessage.at(GBuffer0)).size() + GBuffer1;
    while (GBuffer2 != GBuffer3){
        OUTPUT(); 
        ++GBuffer2; 
    }
    std::exit(1);
}

void VirtualMachine::OUTPUT(){
    GetTopIBufferAddress(); 
    std::cout << VirtualVolatileMemory.at(GBuffer1).Data; 
    ClearTopIBuffer(); 
}
void VirtualMachine::SET_IN_BUFFER(){
    ++BCP;
    LoadBCInWBuffer(); 
}
void VirtualMachine::SET_IN_IDENTIFIER(){
    ++BCP; 
}
void VirtualMachine::INPUT(){}
void VirtualMachine::CHANGE_BCP(){}
void VirtualMachine::MLC(){}
int VirtualMachine::ExecuteByteCode(){
    while (BCP < Dat::ByteCode.size()){
        switch(Dat::ByteCode.at(BCP).TType){
            case (Dat::VMInstruction):{
                switch(Dat::ByteCode.at(BCP).Sub_TType.VMInstruction){
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