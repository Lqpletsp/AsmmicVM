#include "Decoder.hpp"
#include <iostream>
#include <filesystem>
#include <fstream>
#include <unordered_map>
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
DF::rtrVal DF::Decoder::DecodeVar(const std::string& fp) {
    std::ifstream file(fp); 
    if (!file.is_open()) {
        return {1, "Cannot open file\n"}; 
    }

    DecodedVariable = std::string(
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
bool DF::Decoder::ValidateVID(int VarID){
    auto it = IdenMap.find(VarID); 
    if (it == IdenMap.end()) return false; 
    return true;
}
DF::rtrVal DF::Decoder::iniVar(){
    bool IDMode = true;
    bool Immutable = false; 
    Dat::VDT Variable; 
    int ID = -1;   
    std::string token; 
    for (const char ch : DecodedVariable){
        if (ch == ' ' || ch == '\n'){
            if (IDMode){
                auto it = DF::DataTypesMap.find(token);
                if (!ValidateTypes(token, DF::DTFD::INTEGER))  
                    return {1, "Non integer value was given for variable ID during identifier initialization"};
                ID = std::stoi(token); 
            } else {
                if (ID == -1) return {1, "Invalid variable file syntax"}; 
                auto it = DF::DataTypesMap.find(token); 
                if (it == DF::DataTypesMap.end()) 
                    return {1, "Unrecognized type \"" + token + "\" given"}; 
                switch(it->second){
                    case DF::DTFD::BOOLEAN:
                        Variable.DT = Dat::BOOLEAN; 
                        break; 
                    case DF::DTFD::DOUBLE: 
                        Variable.DT = Dat::DOUBLE; 
                        break;
                    case DF::DTFD::CHAR:
                        Variable.DT = Dat::CHAR; 
                        break; 
                    case DF::DTFD::INTEGER:
                        Variable.DT = Dat::INTEGER; 
                        break; 
                    case DF::DTFD::IINTEGER: 
                        Variable.DT = Dat::INTEGER; 
                        Immutable = true; 
                        break; 
                    case DF::DTFD::IBOOLEAN: 
                        Variable.DT = Dat::BOOLEAN; 
                        Immutable = true; 
                        break; 
                    case DF::DTFD::ICHAR: 
                        Variable.DT = Dat::CHAR;
                        Immutable = true; 
                        break; 
                    case DF::DTFD::IDOUBLE: 
                        Variable.DT = Dat::DOUBLE; 
                        Immutable = true; 
                        break;
                    default: 
                        return {1, "Unrecognized type \"" + token + "\" given"}; 
                }
                if (Immutable) Variable.Immutable = true; 
                else Variable.Immutable = false; 
                Variable.VID = ID; 
                IdenMap[ID] = Variable;
            } 
        } else { 
           token.push_back(ch); 
        }
    }
    return {0};
}
DF::rtrVal DF::Decoder::iniIR(){
    if (DecodedIR.empty()) return {0};
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
                        return {1,"SECURITY: Command \"" + token + "\" not found" };
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
                        default: 
                            return {1, "Unidentified data type \"" + token + "\" used"};
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
                    return {.result = 0, .eval = "SECURITY: Invalid data types for token"};
                }
                BT.LT = token; 
                if (VID){
                    try{
                        ValidateVID(std::stoi(token));
                        BT.TType = Dat::Identifier; 
                    } catch(...){
                        return {.result = 0, .eval = "SECURITY: Expected INTEGER for variable ID but was not given"};
                    }
                } else{
                    BT.TType = Dat::Constant;
                    BT.Sub_TType.DT = BCType;
                }
                token.clear();
                Dat::ByteCode.push_back(BT);
            }
            if (ch == '\n') CommandMode = true;
        } else token.push_back(ch);
    }
    return {0};
}
int DF::Decoder::GenerateByteCode(){ 
    rtrVal rtr = iniVar(); 
    if (rtr.result > 0) {
        std::cerr << rtr.eval << '\n'; 
        return 1 ; 
    }
    rtr = iniIR(); 
    if (rtr.result > 0){
        std::cerr << rtr.eval << '\n'; 
        return 1; 
    }
    return 0;
}

std::unordered_map<int, Dat::VDT> *DF::Decoder::GetIdenTable(){return &IdenMap;}
int DF::Decoder::GetMemoryDeclared(){return MemoryDeclared;}