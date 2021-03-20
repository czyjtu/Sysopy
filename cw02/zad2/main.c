#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h> 
#include <sys/times.h>

const int BUFFOR_LEN = 256;

double calculate_time(clock_t start,clock_t end){
    return (double) (end-start) / sysconf(_SC_CLK_TCK);
}

void save_time(char *operation, clock_t real_start_time, clock_t real_end_time, struct tms *end_time, struct tms *start_time){
    FILE* f = fopen("pomiar_zad_2.txt", "a");

    if (f == NULL){
        perror("Error in opening raport file");
        exit(1);
    }

    double real = calculate_time(real_start_time,real_end_time);
    double user = calculate_time(start_time->tms_cutime,end_time->tms_cutime);
    double system = calculate_time(start_time->tms_cstime,end_time->tms_cstime);

    fprintf(f, "%s:\n", operation);
    fprintf(f,"\tReal\t\t\tUser\t\t\tSystem\n");
    fprintf(f, "\t%-15f\t%-15f\t%-15f\n",real, user, system);

    fclose(f);
}

void lib_fun(char key, char *file_name){
    FILE *fp = fopen(file_name, "r");
    if(fp == NULL){
      perror("Error in opening file");
      exit(1);
    }
    char line[BUFFOR_LEN];
    char c;
    int i=0;
    int contains = 0;

    fwrite("Lines with given character:\n--------------------------\n", 1, 56, stdout);
    while(fread(&c, sizeof(char), 1, fp) > 0){
        line[i++] = c;
        if(c == key){
            contains = 1;
        }
        else if(c == '\n'){
            
            if(contains == 1)
                fwrite(line, 1, i, stdout);

            // clear buffor
            while(i > 0)
                line[--i] = 0;
            
            contains = 0;
        }
    }
    fwrite("--------------------------\n", 1, 27, stdout);
    fclose(fp);
}

void sys_fun(char key, char *file_name){
    int fp = open(file_name, O_RDONLY);
    if(fp == -1){
      perror("Error while opening file");
      exit(1);
    }
    char line[BUFFOR_LEN];
    char c;
    int contains = 0;
    int i=0;

    write(1, "Lines with given character:\n--------------------------\n", 56);
    while(read(fp, &c, 1) > 0){
        line[i++] = c;

        if(c == key){
            contains = 1;
        }
        else if(c == '\n'){
            
            if(contains == 1)
                write(1, line, i);

            // clear buffor
            while(i > 0)
                line[--i] = 0;
            
            contains = 0;
        }
    }
    write(1, "--------------------------\n", 27);
    close(fp);
}

int main(int argc, char* argv[]) {
    if(argc < 3){
        printf("You have to pass character and file name!");
        exit(1);
    }

    struct tms *start_time = calloc(1, sizeof(struct tms));
    struct tms *end_time = calloc(1, sizeof(struct tms));
    clock_t real_start_time;
    clock_t real_end_time;
    
    real_start_time = times(start_time);
    #ifdef SYS
        sys_fun(*argv[1], argv[2]);
        real_end_time = times(end_time);
        save_time("System functions", real_start_time, real_end_time, start_time, end_time);
    #else
        lib_fun(*argv[1], argv[2]);
        real_end_time = times(end_time);
        save_time("Library functions", real_start_time, real_end_time, start_time, end_time);
    #endif

    return 0;
}