#include "VirtualMachine/VM.hpp"
#include "Decoder/Decoder.hpp"
#include <iostream>

int main(int argc, char *argv[]){
    // execution format: ./amcVM <Asmmic directory> 
    if (argc != 2) {
        std::cerr << "Invalid execution format\nExpected: ./ASMMICVM <Asmmic directory>";
        return 1;
    }
    DF::Decoder Decoder_main;
    int x = Decoder_main.Decode_main(argv[1]);
    if (x > 1) return 1; 
    Decoder_main.GenerateByteCode(); 
    std::cout << "[BRIDGE: Bytecode generation complete]";

    VMF::VirtualMachine VirtualMachine_main;
    VirtualMachine_main.iniMemmory(Decoder_main.GetMemoryDeclared());
    VirtualMachine_main.SetIdentifierMap(Decoder_main.GetIdenTable());

    return 0; 
}