#include "VMTypes.hpp"
#include "Decoder/Decoder.hpp"

int main(int argc, char *argv[]){
    // execution format: ./amcVM <Asmmic directory> 
    if (argc != 2) return 1;
    int rtr = Decode_main(argv[1]);
    if (rtr > 0) return 1;
    return 0; 
}