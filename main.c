#include "VMTypes.h"

void initMemory(){
    int TotalMemoryDeclared = 0; 
    int *MemoryPool = NULL; 
    BCToken *ByteCode = NULL;
    int BCP = 0;  
}
int main(int argc, char *argv[]){
    // execution format: ./amcVM <Asmmic directory> 
    initMemory(); 
    CleanupPointers(); 
    return 0; 
}