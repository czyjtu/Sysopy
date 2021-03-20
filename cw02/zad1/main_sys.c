#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h> 
#include <sys/times.h>

const int BUFFOR_LEN = 250;

double calculate_time(clock_t start,clock_t end){
    return (double) (end-start) / sysconf(_SC_CLK_TCK);
}

void save_time(char *operation, clock_t real_start_time, clock_t real_end_time, struct tms *end_time, struct tms *start_time){
    FILE* f = fopen("pomiar_zad_1.txt", "a");

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

int main(int argc, char* argv[]) {
    int fp_a;
    int fp_b;

    struct tms *start_time = calloc(1, sizeof(struct tms));
    struct tms *end_time = calloc(1, sizeof(struct tms));
    clock_t real_start_time;
    clock_t real_end_time;

    if(argc < 3){
        char c;
        int i=0;
        int j=0;
        char *a_name = calloc(BUFFOR_LEN, sizeof(char ));
        char *b_name = calloc(BUFFOR_LEN, sizeof(char ));
        if (a_name == NULL || b_name == NULL){
            printf("Something went wrong with memory allocation.\n");
            exit(1);
        }

        printf("Podaj nazwy plików!\nPlik 1:\n");
        while(read(STDIN_FILENO, &c, 1) > 0 && c != '\n')
            a_name[i++] = c;
        

        printf("Plik 2:\n");
        while(read(STDIN_FILENO, &c, 1) > 0 && c != '\n')
            b_name[j++] = c;
        
        fp_a = open(a_name,  O_RDONLY);
        fp_b = open(b_name,  O_RDONLY);
        free(a_name);
        free(b_name);
    }
    else{
        fp_a = open(argv[1], O_RDONLY);
        fp_b = open(argv[2], O_RDONLY);
    }

    if(fp_a == -1 || fp_b == -1) {
      perror("Error while opening file");
      exit(1);
   }

    char line_a[BUFFOR_LEN];
    char line_b[BUFFOR_LEN];
    char c;
    int i=0;
    int j=0;

    write(1, "Files content:\n--------------------------\n", 42);
    real_start_time = times(start_time);
    while(1){
        // read characters until end of the line is reached
        while(read(fp_a, &c, 1) > 0){
            line_a[i++] = c;
            if(c == '\n')
                break;
        }
        while(read(fp_b, &c, 1) > 0){
            line_b[j++] = c;
            if(c == '\n')
                break;
        }
        if(i == 0 && j == 0)
            break;
    
        if(i > 0)
            write(1, line_a, i);
        
        if(j > 0)
            write(1, line_b, j);

        // clear buffor
        while(i > 0)
            line_a[--i] = 0;

        while(j > 0)
            line_b[--j] = 0;   
    }
    write(1, "--------------------------\n", 27);
    real_end_time = times(end_time);
    save_time("System functions", real_start_time, real_end_time, start_time, end_time);

    close(fp_a);
    close(fp_b);

    free(start_time);
    free(end_time);

    return 0;
}