#include "library.h"
#include <stdlib.h>
#include <stdio.h>
#include <memory.h>
#include <unistd.h>
#include <sys/times.h>
#include <dlfcn.h>
#include <ctype.h>
#include <time.h>


double calculate_time(clock_t start,clock_t end){
    return (double) (end-start) / sysconf(_SC_CLK_TCK);
}

void save_header(FILE *f, int operation_size){
    if(operation_size < 5){
        fprintf(f, "\n\t\tSmall number of files and small number of rows\n\n");
    }
    else if(operation_size > 15){
        fprintf(f, "\n\t\tLarge number of files and large number of rows\n\n");
    }
    else{
        fprintf(f, "\n\t\tMedium number of files and medium number of rows\n\n");
    }
}

void save_time(FILE *f, const char *operation, double real, double user, double system){
    fprintf(f, "%s:\n", operation);
    fprintf(f,"\tReal\t\t\tUser\t\t\tSystem\n");
    fprintf(f, "\t%-15f\t%-15f\t%-15f\n",real, user, system);
}

int main(int argc, char* argv[]) {

    int save_raport = 1;  // chenge to 1 if you want to save report

    if(argc < 5){
        printf("No enough arguments provided, you should at least pass merge_files files number and file names.\n");
        return -1;
    }

    struct tms *start_time = calloc(1, sizeof(struct tms));
    struct tms *end_time = calloc(1, sizeof(struct tms));
    clock_t real_start_time;
    clock_t real_end_time;

    FILE *result_ptr;

    if(save_raport == 1)
        result_ptr = fopen("raport2.txt", "a"); 
    

    const char *operation;
    int operation_size;
    struct table* table = NULL;

    for(int i=1; i<argc; ++i){
        real_start_time = times(start_time);
        if(strcmp(argv[i], "merge_files") == 0){
            if(!isdigit(*argv[i+1])){
                printf("num of files needed, but got: %s\n",argv[i+1]);
                return -1;
            }
            int num_files = atoi(argv[++i]);
            if(num_files%2 != 0){
                printf("Number of files has to be even.\n");
                return -1;
            }

            char** files_list[num_files];
            int j = 0;
            while(j < num_files)
                files_list[j++] = &argv[++i];
            
            printf("Trying to merge %i files\n", num_files);
            table = merge_files(num_files, *files_list);
            printf("Finished merging\n");
            operation = "merge files and save blocks";
            operation_size = num_files;

            if(save_raport == 1)
                save_header(result_ptr, operation_size);
        }
        else if(strcmp(argv[i], "remove_block") == 0){
            if(table == NULL){
                printf("Before removing block you have to merge some, son...\n");
                return -1;
            }
            if(!isdigit(*argv[i+1])){
                printf("You need to pass index of the block you want to remove.\n");
                return -1;
            }
            int block_idx = atoi(argv[++i]);
            printf("Will try to remove block of idx: %i\n", block_idx);
            remove_block(table, block_idx);
            printf("Block removed\n");
            operation = "remove block";
        }
        else if(strcmp(argv[i], "remove_row") == 0){
            if(table == NULL){
                printf("Before removing rows you have to merge some files, son...\n");
                return -1;
            }
            if(i+2 > argc || !isdigit(*argv[i+1]) || !isdigit(*argv[i+2])){
                printf("You need to pass block and row index to remove_row.\n");
                return -1;
            }
            int block_idx = atoi(argv[++i]);
            int row_idx = atoi(argv[++i]);
            printf("Will try to remove row %i from block %i\n", row_idx, block_idx);
            remove_row(table, block_idx, row_idx);
            printf("Row removed\n");
            operation = "remove row";
        }
        else if(strcmp(argv[i], "print_files") == 0){
            if(table == NULL){
                printf("No files were merged.\n");
                return -1;
            }
            print_files(table);
        }
        else if(strcmp(argv[i], "print_block") == 0){
            if(table == NULL){
                printf("No files were merged.\n");
                return -1;
            }
            if(i+1 > argc || !isdigit(*argv[i+1])){
                printf("To print the block you need to pass its index.\n");
                return -1;
            }
            int block_idx = atoi(argv[++i]);
            print_block(table, block_idx);
        }
        real_end_time = times(end_time);
        double real = calculate_time(real_start_time,real_end_time);
        double user = calculate_time(start_time->tms_cutime,end_time->tms_cutime);
        double system = calculate_time(start_time->tms_cstime,end_time->tms_cstime);
        printf("\tReal\t\tUser\t\tSystem\n");
        printf("\t%-15f\t%-15f\t%-15f\n",real, user, system);
        if(save_raport == 1)
            save_time(result_ptr, operation, real, user, system);

    }
    if(save_raport == 1)
        fclose(result_ptr);
    
    return 0;
}