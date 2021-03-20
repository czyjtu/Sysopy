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

    FILE* fp_a;
    FILE* fp_b;

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

        printf("Podaj nazwy plików!\nPlik 1: ");
        while(1){
            fread(&c, sizeof(char), 1, stdin);
            if(c == '\n')
                break;

            a_name[i++] = c;
        }

        printf("Plik 2: ");
        while(1){
            fread(&c, sizeof(char), 1, stdin);
            if(c == '\n')
                break;

            b_name[j++] = c;
        }
        fp_a = fopen(a_name, "r");
        fp_b = fopen(b_name, "r");
    }
    else{
        fp_a = fopen(argv[1], "r");
        fp_b = fopen(argv[2], "r");
    }

    if(fp_a == NULL || fp_b == NULL) {
      perror("Error in opening file");
      exit(1);
   }

    char line_a[BUFFOR_LEN];
    char line_b[BUFFOR_LEN];
    char c;
    int i=0;
    int j=0;

    real_start_time = times(start_time);
    printf("Files content:\n--------------------------\n");
    while(!feof(fp_a) || !feof(fp_b)){
        // read characters until end of the line is reached
        while(fread(&c, sizeof(char), 1, fp_a) > 0){
            line_a[i++] = c;
            if(c == '\n')
                break;
        }
        while(fread(&c, sizeof(char), 1, fp_b) > 0){
            line_b[j++] = c;
            if(c == '\n')
                break;
        }
    
        if(i > 0)
            fwrite(line_a, 1, i, stdout);
        
        if(j > 0)
            fwrite(line_b, 1, j, stdout);

        // clean buffor
        while(i > 0)
            line_a[--i] = 0;

        while(j > 0)
            line_b[--j] = 0;
    }
    printf("--------------------------\n");
    real_end_time = times(end_time);
    save_time("Library functions", real_start_time, real_end_time, start_time, end_time);
    
    fclose(fp_a);
    fclose(fp_b);

    free(start_time);
    free(end_time);

    return 0;
}