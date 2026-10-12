#include "Decoder.hpp"
#include <iostream>
#include <filesystem>
#include <fstream>
#include "../DTP.hpp"

namespace fs = std::filesystem;

DF::rtrVal DF::Decoder::DecodeVariables(const std::string& fp){
    std::ifstream file(fp); 
    if (!file.is_open()) 
        return {1, "Cannot open file\n"}; 

    DecodedVariable = std::string(
        (std::istreambuf_iterator<char>(file)),
        std::istreambuf_iterator<char>()
    );
    file.close();
    return {0, ""}; 
}

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

DF::rtrVal DF::Decoder::DecodeMemory(const std::string& fp) {
    std::ifstream file(fp); 
    if (!file.is_open()) {
        return {1, "Cannot open file\n"}; 
    }
    if (!(file >> DecodedMemory)) {
        return {1, "SECURITY: Memory file empty\n"}; 
    }
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
    // Checking the dir in the order: memory.dsm, variables.dsm, and then ir.dsm
    fs::path mem_path = fs::path(dir_path) / "memory.dsm";
    if (!fs::exists(mem_path) || !fs::is_regular_file(mem_path)) {
        std::cerr << "SECURITY: Memory file not decoded\n";
        return 1;
    }
    std::cerr << "File recognized: memory.dsm \n";
    rtrVal rtr_mem = DecodeMemory(mem_path.string()); 
    if (rtr_mem.result > 0) {
        std::cerr << rtr_mem.eval; 
        return 1;
    }
    std::cout << "[Memory decoding complete]\n";
    fs::path var_path = fs::path(dir_path) / "variables.dsm";
    if (!fs::exists(var_path) || !fs::is_regular_file(var_path)) {
        std::cerr << "SECURITY: Variable file not decoded\n";
        return 1;
    }
    std::cerr << "File recognized: variables.dsm \n";
    rtrVal rtr_var = DecodeVariables(var_path.string());
    if (rtr_var.result > 0) {
        std::cerr << rtr_var.eval << '\n'; 
        return 1;
    }
    std::cout << "[Variables decoding complete]\n";
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

    std::cout << "[All checks passed]\n";
    return 0; 
}

int DF::Decoder::GenerateByteCode(){
    if (!DecodedVariable.empty() && DecodedMemory.empty()) {
        std::cerr << "SECURITY: Variable declaration present but memory was not declared";
        return 1; 
    }
    return 0;
}

DF::rtrVal DF::Decoder::iniMemory(){
    try{
        int mem = std::stoi(DecodedMemory);
        
    } catch(...){
        return {1, "SECURITY: Failed to initialize sandbox memory"};
    }
    return {0};
}