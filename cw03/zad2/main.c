#include "library.h"
#include <stdlib.h>
#include <stdio.h>
#include <memory.h>
#include <unistd.h>
#include <sys/times.h>
#include <ctype.h>
#include <time.h>

#include <sys/types.h>
#include <unistd.h>
#include<sys/wait.h>


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
    struct tms *start_time = calloc(1, sizeof(struct tms));
    struct tms *end_time = calloc(1, sizeof(struct tms));
    clock_t real_start_time;
    clock_t real_end_time;

    if(argc < 3){
        printf("No enough arguments provided, you should pass number of files and file names\n");
        return -1;
    }
    if(!isdigit(*argv[1])){
        printf("num of files needed, but got: %s\n",argv[1]);
        return -1;
    }
    int num_files = atoi(argv[1]);
    if(num_files%2 != 0 || num_files != argc - 2){
        printf("Wrong number of files.\n");
        return -1;
    }
    struct table *table = create_table(1);

    char*** files_list = calloc(num_files/2, sizeof(char **));
    for(int i=0; i<num_files/2; i++)
        files_list[i] = calloc(2, sizeof(char *));

    int j = 0;
    int i = 1;
    while(j < num_files/2){
        files_list[j][0] = argv[++i];
        files_list[j++][1] = argv[++i];
    }

    printf("parent id: %d\n", getpid());

    real_start_time = times(start_time);
    pid_t id;
    for(int i=0; i < num_files/2; i++){
        id = fork();
        if(id == -1){
            printf("Fork failed.\n");
            return 3;
        }
        if(id == 0){     
            merge_files(2, files_list[i], table);
            break;
        }
    }
    real_end_time = times(end_time);

    if(id != 0){
        FILE *result_ptr;
        if(save_raport == 1)
            result_ptr = fopen("raport2.txt", "a"); 
    
        real_end_time = times(end_time);
        double real = calculate_time(real_start_time,real_end_time);
        double user = calculate_time(start_time->tms_cutime,end_time->tms_cutime);
        double system = calculate_time(start_time->tms_cstime,end_time->tms_cstime);
        if(save_raport == 1){
            save_header(result_ptr, num_files);
            save_time(result_ptr, "merge files", real, user, system);
        }

        while (wait(NULL) > 0);       
        clear(table);
        if(save_raport == 1)
            fclose(result_ptr);
        
    }
    free(files_list);
    free(start_time);
    free(end_time);
    
    return 0;
}