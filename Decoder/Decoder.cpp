#include "Decoder.hpp"
#include <iostream>
#include <filesystem>
#include <fstream>
#include "../DTP.hpp"

namespace fs = std::filesystem;

DF::rtrVal DF::Decoder::DecodeIR(const std::string& fp) {
    std::ifstream file(fp); 
    if (!file.is_open()) {
        return {1, "Cannot open file\n"}; 
    }

    DecodedIR = std::string(
        (std::istreambuf_iterator<char>(file)),
        std::istreambuf_iterator<char>()
    );
    file.close();
    return {0, ""}; 
}


int DF::Decoder::Decode_main(const std::string& dir_path) {
    if (!fs::exists(dir_path) || !fs::is_directory(dir_path)) {
        std::cout << "Error opening directory \"" << dir_path << "\"\n"; 
        return 1; 
    }
    for (const auto& entry : fs::directory_iterator(dir_path)) {
        if (!entry.is_regular_file()) continue;

        std::string filename = entry.path().filename().string();
        std::string ext = entry.path().extension().string();

        if (ext.empty()) {
            std::cerr << "SECURITY: Directory does not conform to the rules of the VM decoder\n"
                      << "\"" << filename << "\" has no extension\n"; 
            return 1; 
        }

        if (ext != ".dsm") {
            std::cerr << "SECURITY: Directory does not conform to the rules of the VM decoder\n"
                      << "\"" << filename << "\" has an invalid extension\n"; 
            return 1; 
        } 

        if (filename != "memory.dsm" && filename != "variables.dsm" && filename != "ir.dsm") {
            std::cerr << "SECURITY: Directory does not conform to the rules of the VM decoder\n"; 
            return 1; 
        }
    }
    fs::path ir_path = fs::path(dir_path) / "ir.dsm";
    if (!fs::exists(ir_path) || !fs::is_regular_file(ir_path)) {
        std::cerr << "SECURITY: IR file not decoded\n";
        return 1;
    }
    std::cerr << "File recognized: ir.dsm \n";
    rtrVal rtr_ir = DecodeIR(ir_path.string());
    if (rtr_ir.result > 0) {
        std::cerr << rtr_ir.eval << '\n'; 
        return 1;
    }
    std::cout << "[IR decoding complete]\n";
    return 0; 
}
bool DF::Decoder::ValidateTypes(std::string &Data, DTFD DTP){
    try{
        std::stod(Data);
        if (DTP == DF::DTFD::BOOLEAN) return false; 
        if (DTP == DF::DTFD::CHAR && Data.size() > 1) return false;  
    } catch(...){
        if (DTP == DF::DTFD::INTEGER || DTP == DF::DTFD::DOUBLE || DTP == DF::DTFD::VID)  return false; 
        if (DTP == DF::DTFD::BOOLEAN  && (Data != "T" && Data != "F")) return false; 
        if (DTP == DF::DTFD::CHAR && Data.size() != 1) return false; 
    }
    return true; 
}
int DF::Decoder::GenerateByteCode(){
    if (DecodedIR.empty()) return 0;
    bool CommandMode = true, VID = false;  
    std::string token; 
    Dat::ByteCodeType BT; 
    DF::DTFD DT; 
    Dat::DataTypes BCType; 
    for (const char ch: DecodedIR){
        if (ch == ' ' || ch == '\n'){
            if (CommandMode){
                auto it = DF::InstMap.find(token); 
                if (it == DF::InstMap.end()){
                    auto it = DF::DataTypesMap.find(token); 
                    if (it == DF::DataTypesMap.end()) {
                        std::cerr << "SECURITY: Command \"" << token << "\" not found";
                        return 1; 
                    }
                    switch (it->second) {
                        case DF::DTFD::BOOLEAN: 
                            BCType = Dat::BOOLEAN; 
                            break; 
                        case DF::DTFD::INTEGER:
                            BCType = Dat::INTEGER; 
                            break; 
                        case DF::DTFD::CHAR: 
                            BCType = Dat::CHAR; 
                            break; 
                        case DF::DTFD::STRING: 
                            BCType = Dat::STRING; 
                            break; 
                        case DF::DTFD::VID: 
                            VID = true; 
                            break;
                        case DF::DTFD::DOUBLE: 
                            BCType = Dat::DOUBLE; 
                            break;
                    }
                    token.clear();
                    CommandMode = false;
                    continue; 
                }
                BT.TType = Dat::VMInstruction; 
                BT.Sub_TType.VMInstruction = it->second;
                token.clear();
                Dat::ByteCode.push_back(BT);
                CommandMode = false;
            } else {
                if(!ValidateTypes(token, DT)){ 
                    std::cerr << "SECURITY: Invalid data types for token";
                    return 1;
                }
                BT.LT = token; 
                BT.TType = Dat::Constant;
                BT.Sub_TType.DT = BCType;
                token.clear();
                Dat::ByteCode.push_back(BT);
            }
            if (ch == '\n') CommandMode = true;
        } else token.push_back(ch);
    }
    return 0;
}
