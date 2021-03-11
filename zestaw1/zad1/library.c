#include "library.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


struct table* create_table(int num_blocks){
    if(num_blocks <= 0){
        return NULL;
    }

    struct table *table = (struct table *)calloc(1, sizeof(struct table));

    table->size = num_blocks;
    table->num_blocks = num_blocks;
    struct block **blocks = (struct block **) calloc(num_blocks, sizeof(struct block *));
    table->blocks = blocks;

    return table;
}

struct block* create_block(int num_lines, int idx){
    if(num_lines <= 0 || idx < 0){
        return NULL;
    }

    struct block *block = (struct block *)calloc(1, sizeof(struct block));

    block->size = num_lines;
    block->num_lines = num_lines;
    block->idx = idx;
    char **lines = (char **) calloc(num_lines, sizeof(char *));
    block->lines = lines;

    return block;
}


struct table* merge_files(int num_files, char** files_list){
    if(num_files % 2 != 0 || num_files < 2){
        printf("Uneven number of files.\n");
        exit(EXIT_FAILURE);
    }
    
    int bufferLen = 255;
    struct table *table = create_table(num_files/2);
    // for(int i=0; i < table->size; i++){
    //     table->blocks[i] = (struct block* )malloc(sizeof(struct block));
    // }
    

// iteracja po wszystkich plikach
    for (int i = 0; i < num_files; i+= 2){
        char *fileA = files_list[i];
        char *fileB = files_list[i+1];
        char destName[80];
        sprintf(destName, "merged_%i.txt", i/2);

        FILE *aPtr = fopen(fileA, "r");
        FILE *bPtr = fopen(fileB, "r");
        FILE *destPtr = fopen(destName, "w");
        char bufferA[bufferLen];
        char bufferB[bufferLen];

        if(aPtr == NULL || bPtr == NULL){
            perror("Error ");
            exit(EXIT_FAILURE);
        }

// przepisywanie wierszy do pliku tymczasowego
        int num_lines = 0;
        while(fgets(bufferA, bufferLen, aPtr) && fgets(bufferB, bufferLen, bPtr)){
            fputs(bufferA, destPtr);
            fputs(bufferB, destPtr);
            num_lines += 2;
        }

        fclose(aPtr);
        fclose(bPtr);
        fclose(destPtr);

//  Zapisanie wierszy pliku tymczasowego do bloku
        FILE *merged = fopen(destName, "r");
        struct block *block = create_block(num_lines, i/2);
        for (int j  = 0; j < block->size; ++j) {
            block->lines[j] = (char *)calloc(1, bufferLen);
        }

        int line_idx = 0;
        while(fgets(block->lines[line_idx], bufferLen, merged)){
            block->lines[line_idx][strlen(block->lines[line_idx]) - 1] = '\0';
            line_idx++;
        }

        fclose(merged);
       
        table->blocks[i/2] = block;
    }
    return table;
}

void remove_block(struct table* table, int idx){
    if(table->size <= idx){
        perror("Error ");
        printf("Block idx out of bounds.\n");
        exit(EXIT_FAILURE);
    }

    free(table->blocks[idx]);
    table->blocks[idx] = NULL;
    table->num_blocks -= 1;
}

void remove_row(struct table* table, int block_idx, int row_idx){
    if(table->size <= block_idx){
        perror("Error ");
        printf("Block idx out of bounds.\n");
        exit(EXIT_FAILURE);
    }
    else if(table->blocks[block_idx] == NULL){
        return;
    }
    else if(table->blocks[block_idx]->size <= row_idx){
        perror("Error ");
        printf("Row idx out of bounds.\n");
        exit(EXIT_FAILURE);
    }
    else if(table->blocks[block_idx]->lines[row_idx] == NULL){
        return;
    }

    free(table->blocks[block_idx]->lines[row_idx]);
    table->blocks[block_idx]->lines[row_idx] = NULL;
    table->blocks[block_idx]->num_lines -= 1;
}

void print_block(struct table* table, int block_idx){
    if(table->size <= block_idx){
        perror("Error ");
        printf("Block idx out of bounds.\n");
        exit(EXIT_FAILURE);
    }
    printf("Merged file of index %i\nnumber of lines: %i\n", block_idx, table->blocks[block_idx]->num_lines);
    for(int i=0; i < table->blocks[block_idx]->size; i++){
        if(table->blocks[block_idx]->lines[i] != NULL)
             printf("%s\n", table->blocks[block_idx]->lines[i]);
    }
    printf("\n");
}

void print_files(struct table* table){
    for(int i=0;  i < table->size; i++){
        if(table->blocks[i] != NULL)
            print_block(table, i);
    }
}