#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include <stdbool.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>


bool contain_string(const char* filepath, char* str){
    FILE* fp = fopen(filepath, "r");
    if(fp == NULL){
        perror("Error in opening the file.\n");
        return false;
    }

    int len = strlen(str);
    int i = 0;
    char c;
    while(fread(&c, sizeof(char), 1, fp) > 0){
        if(c == str[i]){
            if(i == len - 1){
                return true;
            }
            i++;
        }
        else
            i = 0;
    } 
    return false;
}

bool is_txt(char *name){
    int len = strlen(name);
    if(len < 5)
        return false;
    
    if(strcmp(&name[len-4],".txt") == 0)
        return true;
    
    return false;
}

void indent(int level){
    for(int j = 0;j<level;j++) 
        printf("\t");
}

void listFiles(const char* dirname, char* str, int level) {
    DIR* dir = opendir(dirname);
    if (dir == NULL) {
        printf("Cant't open directory\n");
        return;
    }

    struct dirent* entity;
    entity = readdir(dir);
    while (entity != NULL) {
        char path[200] = { 0 }; 
        strcat(path, dirname);
        strcat(path, "/");
        strcat(path, entity->d_name);

        if(entity->d_type == DT_REG && is_txt(entity->d_name)){
            if(contain_string(path, str))
                printf("pid=%d %s\n", getpid(), path);
        }

        if (entity->d_type == DT_DIR && strcmp(entity->d_name, ".") != 0 && strcmp(entity->d_name, "..") != 0) {
            int id = fork();
            if(id == -1)
                return 1;
            if(id == 0){
                listFiles(path, str, ++level);
                return;
            }
        }
        entity = readdir(dir);
    }

    closedir(dir);
}

int main(int argc, char* argv[]) {
    if(argc!= 3){
        printf("Wrong number of arguments, pass dir path and string");
        return 1;
    }

    listFiles(argv[1], argv[2], 0);
    while(wait(NULL) > 0);
    return 0;
}