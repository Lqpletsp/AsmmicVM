#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include <stdbool.h>
#include "../VMTypes.h"

typedef struct {
    int result; 
    char eval[256];
} rtrVal;

rtrVal InitByteCode(char *IR){
    return (rtrVal){.result = 0}; 
}

rtrVal DecodeIR(char *fp) {
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
    rtrVal returned = InitByteCode(file_contents); 
    if (returned.result > 0) return returned; 
    free(file_contents);

    return (rtrVal){.result = 0}; 
}
rtrVal DecodeMemory(char *fp){
    FILE *file = fopen(fp, "r"); 
    if (file == NULL) return (rtrVal){.result = 1, .eval = "Cannot open file\n"}; 
    char buffer[8]; 
    if (fgets(buffer, sizeof(buffer), file) != NULL)
        return (rtrVal){.result = 1, .eval = "SECURITY: Memory file empty\n"}; 
    TotalMemoryDeclared = atoi(buffer); 
    return (rtrVal){.result = 0};
}
int Decode_main(const char *dir_path) {
    DIR *dir = opendir(dir_path); 
    if (dir == NULL) {
        printf("Error opening directory \"%s\"\n", dir_path); 
        return 1; 
    }
    // CheckList at 0:=Memory, 1:=Variables, 2:=IR
    bool CheckList[4] = {false, false, false}; 
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