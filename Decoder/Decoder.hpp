#pragma once 
#include <string>

namespace DF{
    struct rtrVal {
        int result; 
        std::string eval;
    };
    struct BCT{
        std::string LT; 
    };
    class Decoder{
        public: 
            // attributes
            std::string DecodedIR; 
            std::string DecodedMemory; 
            std::string DecodedVariable; 

            // methods
            int Decode_main(const std::string& dir_path); 
            int GenerateByteCode(); 
        private: 
            // used by Decode_main
            rtrVal DecodeMemory(const std::string &fp); 
            rtrVal DecodeIR(const std::string &fp); 
            rtrVal DecodeVariables(const std::string &fp);
            // used by GenerateByteCode
            rtrVal iniMemory(); 
            rtrVal iniIR(); 
            rtrVal iniVariables(); 

    };
}
