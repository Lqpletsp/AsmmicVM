#pragma once 
#include <string>
#include <unordered_map>
#include "../DTP.hpp"

namespace DF{
    struct rtrVal {
        int result; 
        std::string eval;
    };
    enum class DTFD{
        INTEGER, 
        DOUBLE, 
        BOOLEAN, 
        CHAR, 
        STRING, 
        VID, 
    };
    inline std::unordered_map<std::string,Dat::Instructions> InstMap = {
        {"SET", Dat::SET},
        {"GOTOLN", Dat::GOTOLN}, 
        {"DECV", Dat::DECV},
        {"DECM", Dat::DECM}, 
        {"OUTPUT", Dat::OUTPUT}, 
        {"INPUT", Dat::INPUT}, 
        {"MLC", Dat::MLC}, 
        {"EndInstructionFlag", Dat::EndInstructionFlag}, 
        {"EndExecutionFlag", Dat::EndExecutionFlag}, 
        {"ChangeModeFlag", Dat::ChangeModeFlag},
    };
    inline std::unordered_map<std::string,DTFD> DataTypesMap = {
        {"INTEGER", DTFD::INTEGER}, 
        {"DOUBLE", DTFD::DOUBLE},
        {"BOOLEAN", DTFD::BOOLEAN}, 
        {"CHAR", DTFD::CHAR}, 
        {"STRING", DTFD::STRING}, 
        {"VID", DTFD::VID},
    };
    inline std::unordered_map<std::string, Dat::KeyWords> KeyWordMaps = {
        {"TRUE", Dat::TRUE}, 
        {"FALSE", Dat::FALSE}, 
        {"LESSTHAN", Dat::LESSTHAN}, 
        {"GREATEREQUAL", Dat::GREATEREQUAL}, 
        {"GREATERTHAN", Dat::GREATERTHAN}, 
        {"EQUAL", Dat::EQUAL}, 
        {"NOTEQUAL", Dat::NOTEQUAL},
        {"AND", Dat::AND}, 
        {"OR", Dat::OR}, 
        {"ADD", Dat::ADD}, 
        {"MINUS", Dat::MINUS}, 
        {"MULTIPLY", Dat::MULTIPLY}, 
        {"DIVIDE", Dat::DIVIDE}, 
        {"EXPONENT", Dat::EXPONENT}
    };
    class Decoder{
        public: 
            // attributes
            std::string DecodedIR; 
            std::string DecodedMemory; 
            std::string DecodedVariable; 
            int MemoryDeclared; 
            // methods
            int Decode_main(const std::string& dir_path); 
            int GenerateByteCode(); 
        private: 
            // used by Decode_main
            rtrVal DecodeIR(const std::string &fp); 
            // used by GenerateByteCode
            rtrVal iniIR(); 
            bool ValidateTypes(std::string &Data, DTFD DTP);

    };
}
