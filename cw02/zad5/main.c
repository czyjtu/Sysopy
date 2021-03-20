#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h> 
#include <string.h>
#include <sys/times.h>



double calculate_time(clock_t start,clock_t end){
    return (double) (end-start) / sysconf(_SC_CLK_TCK);
}

void save_time(char *operation, clock_t real_start_time, clock_t real_end_time, struct tms *end_time, struct tms *start_time){
    FILE* f = fopen("pomiar_zad_5.txt", "a");

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

void lib_fun(char *in, char *out){
    FILE *fin = fopen(in, "r");
    FILE *fout = fopen(out, "w");

    if(fin == NULL || fout == NULL){
        perror("Error in opening file");
        exit(1);
    }
    char c;
    char newline = '\n';
    int i=0;

    while(fread(&c, sizeof(char), 1, fin) > 0){
        if(c == '\n'){
            i = 0;
            fwrite(&c, 1, 1, fout);
        }
        else if(i == 50){
            fwrite(&newline, 1, 1, fout);
            fwrite(&c, 1, 1, fout);
            i = 1;
        }
        else{
            fwrite(&c, 1, 1, fout);
            i++;
        }
    } 

    fclose(fin);
    fclose(fout);
}

void sys_fun(char *in, char *out){
    int fin = open(in, O_RDONLY);
    int fout = open(out, O_WRONLY|O_CREAT);

    if(fin == -1 || fout == -1){
        perror("Error in opening file");
        exit(1);
    }
    char c;
    char newline = '\n';
    int i=0;

    while(read(fin, &c, 1) > 0){
        if(c == '\n'){
            i = 0;
            write(fout, &newline, 1);
        }
        else if(i == 50){
            write(fout, &newline, 1);
            write(fout, &c, 1);
            i = 1;
        }
        else{
            write(fout, &c, 1);
            i++;
        }
    } 

    close(fin);
    close(fout);
}

int main(int argc, char* argv[]) {
    if(argc < 3){
        printf("Podaj nazwe pliku do odczytu, oraz nazwe pliku do zapisu.\n");
        exit(1);
    }

    struct tms *start_time = calloc(1, sizeof(struct tms));
    struct tms *end_time = calloc(1, sizeof(struct tms));
    clock_t real_start_time;
    clock_t real_end_time;
    
    #ifdef SYS
        real_start_time = times(start_time);

        sys_fun(argv[1], argv[2]);

        real_end_time = times(end_time);
        save_time("System functions", real_start_time, real_end_time, start_time, end_time);
    #else
        real_start_time = times(start_time);

        lib_fun(argv[1], argv[2]);

        real_end_time = times(end_time);
        save_time("Library functions", real_start_time, real_end_time, start_time, end_time);
    
    #endif

    free(start_time);
    free(end_time);

    return 0;
}