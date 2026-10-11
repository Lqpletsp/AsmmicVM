#include <iostream>
#include <fstream>
#include <string>
#include <unordered_map>
#include <vector>
#include <filesystem>
#include "../VMTypes.hpp"

namespace fs = std::filesystem;
namespace{
enum class DataTypesForDecoder{
  INTEGER, 
  CHAR, 
  STRING, 
  VID, 
  BOOLEAN,   
  DOUBLE,
};
std::unordered_map<std::string, KeyWords> KeywordMap = {
    {"TRUE", TRUE},
    {"FALSE", FALSE},
    {"Arr", Arr},
    {"ArrEND", ArrEND},
    {"EndInstructionFlag", EndInstructionFlag},
    {"EndExecutionFlag", EndExecutionFlag},
    {"ChangeModeFlag", ChangeModeFlag},
    {"StopFlag", StopFlag},
    {"LESSTHAN", LESSTHAN},
    {"GREATERTHAN", GREATERTHAN},
    {"LESSEQUAL", LESSEQUAL},
    {"GREATEREQUAL", GREATEREQUAL},
    {"EQUAL", EQUAL},
    {"NOTEQUAL", NOTEQUAL},
    {"AND", AND},
    {"OR", OR},
    {"ADD", ADD},
    {"MINUS", MINUS},
    {"MULTIPLY", MULTIPLY},
    {"DIVIDE", DIVIDE},
    {"EXPONENT", EXPONENT}
};
std::unordered_map<std::string,DataTypesForDecoder> DataTypesMap = {
    {"INT", DataTypesForDecoder::INTEGER}, 
    {"CHAR", DataTypesForDecoder::CHAR},
    {"BOOLEAN", DataTypesForDecoder::BOOLEAN}, 
    {"DOUBLE", DataTypesForDecoder::DOUBLE}
};
std::unordered_map<std::string, Instructions> InstructionsMap = {
    {"SET", SET}, 
    {"OUT", OUTPUT},
    {"INP", INPUT}, 
    {"GOTO", GOTOLN}, 
    {"MLC", MLC},
};
using DTFD = DataTypesForDecoder;
bool ValidateDataType(const std::string& dat, const DTFD dtp){
    try{
        std::stod(dat); 
        if (dtp == DTFD::VID){
            auto it = IdentifierMap.find(std::stoi(dat));
            if (it == IdentifierMap.end()) return false; 
        }
    } catch(...){
        if (dtp == DTFD::INTEGER || dtp == DTFD::DOUBLE || dtp == DTFD::VID) return false; 
        else if (!((dat == "T" || dat == "F") && dtp == DTFD::BOOLEAN)) return false; 
        else if (!(dtp == DTFD::CHAR && dat.size() == 1)) return false;   
    }
    // this function also needs to validate if dtp is VID, whether dat is a valid variable ID or not

    return true;
}
} // namespace
struct rtrVal {
    int result; 
    std::string eval;
};
rtrVal InitByteCode(const std::string& IR) {
    bool CommandMode = true;
    std::string token;
    DTFD DT; 
    for (size_t i = 0; i < IR.size(); ++i) {
        char ch = IR.at(i);
        BCToken BT; 
        if (ch == ' ' || ch == '\n') {
            if (CommandMode) {
                auto it = InstructionsMap.find(token); 
                if (it == InstructionsMap.end()) {
                    auto it2 = DataTypesMap.find(token); 
                    if (it2 == DataTypesMap.end())
                        return {1, "SECURITY: Invalid IR format. Appropriate command not found"};
                    DT = it2->second; 
                    continue;
                }
                BT.ByteCodeType = VMInstruction; 
                BT.Sub_Type.Instruction = it->second; 
                ByteCode.push_back(BT);  
                CommandMode = false; 
            } else{
                auto it = KeywordMap.find(token); 
                if (it == KeywordMap.end()){ 
                    if (!ValidateDataType(token, DT)) 
                        return {1,"SECURITY: Invalid type for IR when decoding"};
                    if (DT == DTFD::VID) BT.ByteCodeType = Identifier;
                    else BT.ByteCodeType = Constant; 
                    BT.LT = token; 
                }
                ByteCode.push_back(BT);
            }
            token.clear();
            if (ch == '\n') CommandMode = true;
        } else {
            token.push_back(ch); 
        }
    }
    BCP = 0; 
    return {0, ""}; 
}
rtrVal InitVariables(const std::string &fc){
    bool VIDToken = true; 
    VDT VariableDecl;  
    int  VariableID;
    std::string token; 
    for (size_t i = 0; i < fc.size(); ++i){
        char ch = fc.at(i); 
        if (ch == ' ' || ch == '\n'){
            if (VIDToken){
                if (!ValidateDataType(token, DTFD::INTEGER)) 
                    return {.result = 1, .eval = "SECURITY: Invalid type for identifier ID"};
                VariableID = std::stoi(token);
                IdentifierMap[VariableID] = {};
                VIDToken = false; 
            } else {
                if (token == "I") VariableDecl.Immutable = true; 
                else if (token == "M") VariableDecl.Immutable = false; 
                else{
                    auto it = DataTypesMap.find(token);
                    if (it == DataTypesMap.end()) 
                        return {.result = 0, .eval = "SECURITY: Invalid type for identifier"};
                    switch(it->second){
                        case (DTFD::BOOLEAN): 
                            VariableDecl.DT = BOOLEAN; 
                            break; 
                        case (DTFD::CHAR): 
                            VariableDecl.DT = CHAR; 
                            break;
                        case (DTFD::INTEGER): 
                            VariableDecl.DT = INTEGER; 
                            break;
                        case (DTFD::DOUBLE): 
                            VariableDecl.DT = DOUBLE;  
                            break;
                        case (DTFD::STRING): 
                            return {.result = 0, .eval = "SECURITY: Invalid type of \"STRING\" for identifiers"};
                            break;
                        case (DTFD::VID): 
                            return {.result = 1, .eval = "SECURITY: Invalid type of \"VID\" for identifiers"};
                            break;
                    }  
                }
                if (ch == '\n'){
                    try{
                        IdentifierMap.at(VariableID) = VariableDecl;  
                    } catch(...){
                        return {.result = 1, .eval = "SECURITY: Undeclared VID initialized"};
                    }
                    VIDToken = true;
                }
            }
        } else {
            token.push_back(ch);
        }
    }
    return {0,""};
}
rtrVal DecodeVariables(const std::string& fp){
    std::ifstream file(fp); 
    if (!file.is_open()) 
        return {1, "Cannot open file\n"}; 

    std::string file_contents((std::istreambuf_iterator<char>(file)),
                               std::istreambuf_iterator<char>());
    file.close();
    rtrVal returned = InitVariables(file_contents); 
    if (returned.result > 0) return returned; 
    return {0, ""}; 
}

rtrVal DecodeIR(const std::string& fp) {
    std::ifstream file(fp); 
    if (!file.is_open()) 
        return {1, "Cannot open file\n"}; 

    std::string file_contents((std::istreambuf_iterator<char>(file)),
                               std::istreambuf_iterator<char>());
    file.close();
    rtrVal returned = InitByteCode(file_contents); 
    if (returned.result > 0) return returned; 
    return {0, ""}; 
}
rtrVal InitMemory(const std::string& fc){
    try{
        TotalMemoryDeclared = std::stoi(fc); 
        for (size_t i = TotalMemoryDeclared; i > 0; --i){
            MemoryPool.push(i);
        }
    } catch(...){
        return {.result = 1, .eval = "Memory declaration failed during sandbox initialization"};
    }
    return {0};
}
rtrVal DecodeMemory(const std::string& fp) {
    std::ifstream file(fp); 
    if (!file.is_open()) return {1, "Cannot open file\n"}; 
    
    std::string buffer; 
    if (!(file >> buffer))
        return {1, "SECURITY: Memory file empty\n"}; 
    
    rtrVal rtr = InitMemory(buffer);
    if (rtr.result > 0) return rtr;
    return {0, ""};
}

int Decode_main(const std::string& dir_path) {
    if (!fs::exists(dir_path) || !fs::is_directory(dir_path)) {
        std::cout << "Error opening directory \"" << dir_path << "\"\n"; 
        return 1; 
    }

    // CheckList at 0:=Memory, 1:=Variables, 2:=IR
    bool CheckList[3] = {false, false, false}; 

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

        std::cerr << "File recognized: " << filename << " \n";
        std::string filepath = entry.path().string();

        if (filename == "memory.dsm") {
            rtrVal rtr = DecodeMemory(filepath); 
            if (rtr.result > 0) {
                std::cerr << rtr.eval; 
                return 1;
            }
            CheckList[0] = true; 
            std::cout << "[Memory decoding complete]\n";
        }

        else if (filename == "ir.dsm") {
            if (!(CheckList[0] && CheckList[1])){
                std::cerr << "SECURITY: Invalid format for decoding caused by invalid file placement\n";
                return 1;
            }
            rtrVal rtr = DecodeIR(filepath);
            if (rtr.result > 0) {
                std::cerr << rtr.eval << '\n';
                return 1;
            }
            CheckList[2] = true;
            std::cout << "[IR decoding complete]\n";
        } 
        else if (filename == "variables.dsm") {
            if (!CheckList[0]){
                std::cerr << "SECURITY: Invalid format for decoding caused by invalid file placement\n";
                return 1; 
            }
            rtrVal rtr = DecodeVariables(filepath);
            if (rtr.result > 0){
                std::cerr << rtr.eval << '\n'; 
                return 1;
            }
            CheckList[1] = true;
            std::cout << "[Variables decoding complete]\n";
        } 
        else {
            std::cerr << "SECURITY: Directory does not conform to the rules of the VM decoder\n"; 
            return 1; 
        }
    }
    std::cout << "[All checks passed]\n";
    return 0; 
}