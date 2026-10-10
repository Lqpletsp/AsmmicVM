#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include <stdbool.h>
#include "../VMTypes.h"

typedef struct {
    int result; 
    char eval[256];
} rtrVal;

rtrVal InitByteCode(char *IR, const int IRsize){
    bool CommandMode = true; 
    int CurrentTokenCapacity = 16; 
    int CurrentTokenSize = 0; 
    char *Token = malloc(16*sizeof(char)); 
    for (size_t i = 0; i < IRsize; ++i){
        char ch = IR[i]; 
        if (ch == ' '){
            if (CommandMode){
                enum BCT BC;  
                enum Instructions INST;  
                if (!strcmp(Token, "SET")) INST = SET; 
                else if (!strcmp(Token, "OUT")) INST = OUTPUT; 
                else if (!strcmp(Token, "INP")) INST = INPUT; 
                else if (!strcmp(Token, "GOTO")) INST = GOTOLN; 
                else if (!strcmp(Token, "MLC")) INST = MLC; 
                else return (rtrVal){.result = 0, .eval = "SECURITY: Invalid IR format. Appropriate command not found"}; 
                BCToken BT; 
                BT.ByteCodeType = VMInstruction; 
                BT.Sub_Type.Instruction = INST; 
                BT.LT = ""; 
                ByteCode[BCP] = BT;  
                CommandMode = false; 
                ++BCP; 
            }
        }else{
            // realloc will reduce the token size so fix that
            // size of token will return the size of the pointer 
            if (CurrentTokenSize>=CurrentTokenCapacity){
                CurrentTokenCapacity += 2;
                Token = realloc(Token, CurrentTokenCapacity*sizeof(char)); 
            }
            Token[CurrentTokenSize++] = ch; 
        }
    }
    return (rtrVal){.result = 0}; 
}

rtrVal DecodeIR(char *fp) {
    if (ByteCode == NULL || MemoryPool == NULL) 
        return (rtrVal){.result = 0, .eval = "SECURITY: Invalid file format"};
    FILE *file = fopen(fp, "r"); 
    if (file == NULL) 
        return (rtrVal){.result = 1, .eval = "Cannot open file\n"}; 
    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    fseek(file, 0, SEEK_SET);
    char *file_contents = malloc(file_size + 1);
    if (file_contents == NULL) {
        fclose(file);
        return (rtrVal){.result = 1, .eval = "Memory allocation failed\n"};
    }
    size_t bytes_read = fread(file_contents, 1, file_size, file);
    file_contents[bytes_read] = '\0';
    fclose(file);
    rtrVal returned = InitByteCode(file_contents, bytes_read); 
    if (returned.result > 0) return returned; 
    free(file_contents);
    return (rtrVal){.result = 0}; 
}
rtrVal DecodeMemory(char *fp){
    FILE *file = fopen(fp, "r"); 
    if (file == NULL) return (rtrVal){.result = 1, .eval = "Cannot open file\n"}; 
    char buffer[8]; 
    if (fgets(buffer, sizeof(buffer), file) == NULL)
        return (rtrVal){.result = 1, .eval = "SECURITY: Memory file empty\n"}; 
    TotalMemoryDeclared = atoi(buffer); 
    MemoryPool = (int *)malloc(TotalMemoryDeclared * sizeof(int)); 
    if (MemoryPool == NULL) return  (rtrVal){.result = 1, .eval = "Memory declaration failed during sandbox initialization\n"};
    return (rtrVal){.result = 0};
}
int Decode_main(const char *dir_path) {
    DIR *dir = opendir(dir_path); 
    if (dir == NULL) {
        printf("Error opening directory \"%s\"\n", dir_path); 
        return 1; 
    }
    // CheckList at 0:=Memory, 1:=Variables, 2:=IR
    bool CheckList[3] = {false, false, false}; 
    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL) {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
            continue;
        }
        // checking whether regular file
        if (entry->d_type == DT_REG) {
            char *ext = strrchr(entry->d_name, '.');
            if (ext == NULL){
                printf("SECURITY: Directory does not conform to the rules of the VM decoder\n"
                    "\"%s\" has no extension", entry->d_name); 
                return 1; 
            }
            if (strcmp(ext, ".asm") != 0){
                printf("SECURITY: Directory does not conform to the rules of the VM decoder\n"
                    "\"%s\" has an invalid extension", entry->d_name); 
                return 1; 
            } 
            printf("File recognized: %s \n", entry->d_name);
            if (strcmp(ext, ".asm") == 0) {
                char filepath[1024];
                char filename[1024]; 
                strcpy(filename, entry->d_name); 
                snprintf(filepath, sizeof(filepath), "%s/%s", dir_path, entry->d_name);
                if (strcmp(filename, "memory.dsm") == 0){
                    rtrVal rtr = DecodeMemory(filepath); 
                    if (rtr.result > 0) {
                        printf("%s", rtr.eval); 
                        return 1;
                    CheckList[0] = true; 
                } else if (strcmp(filename, "ir.dsm") == 0){
                } else if (strcmp(filename,"variables.dsm") == 0){
                }
            } else{
                printf("SECURITY: Directory does not conform to the rules of the VM decoder\n"); 
                return 1; 
            }
        }
    }
    }

    closedir(dir);
    return 0; 
}
