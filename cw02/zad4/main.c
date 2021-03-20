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
    FILE* f = fopen("pomiar_zad_4.txt", "a");

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

void lib_fun(char *in, char *out, char *n1, char *n2){
    FILE *fin = fopen(in, "r");
    FILE *fout = fopen(out, "w");

    if(fin == NULL || fout == NULL){
        perror("Error in opening file");
        exit(1);
    }
    char c;
    int i=0;
    int n1_len = strlen(n1);
    int n2_len = strlen(n2);
    char buffor[n1_len]; 
    for( int j=0; j<n1_len;j++)
        buffor[j] = 0;

    while(fread(&c, sizeof(char), 1, fin) > 0){
        if(c == n1[i]){
            // entire n1 match
            if(i == n1_len - 1){
                fwrite(n2, 1, n2_len, fout);

                // cear buffor
                while(i > 0)
                    buffor[--i] = 0;
            }
            else
                buffor[i++] = c;       
        }
        else if(c != n1[i] && i > 0){
            buffor[i++] = c; 
            fwrite(buffor, 1, i, fout);
            // cear buffor
            while(i > 0)
                buffor[--i] = 0;
        }
        else
           fwrite(&c, 1, 1, fout); 
    } 

    fclose(fin);
    fclose(fout);
}

void sys_fun(char *in, char *out, char *n1, char *n2){
    int fin = open(in, O_RDONLY);
    int fout = open(out, O_WRONLY|O_CREAT);

    if(fin == -1 || fout == -1){
        perror("Error in opening file");
        exit(1);
    }
    char c;
    int i=0;
    int n1_len = strlen(n1);
    int n2_len = strlen(n2);
    char buffor[n1_len]; 
    for( int j=0; j<n1_len;j++)
        buffor[j] = 0;

    while(read(fin, &c, 1) > 0){
        if(c == n1[i]){
            // entire n1 match
            if(i == n1_len - 1){
                write(fout, n2, n2_len);

                // cear buffor
                while(i > 0)
                    buffor[--i] = 0;
            }
            else
                buffor[i++] = c;       
        }
        else if(c != n1[i] && i > 0){
            buffor[i++] = c; 
            write(fout, buffor, i);
            // cear buffor
            while(i > 0)
                buffor[--i] = 0;
        }
        else
           write(fout, &c, 1); 
    } 

    close(fin);
    close(fout);
}

int main(int argc, char* argv[]) {

    if(argc < 5){
        printf("Podaj nazwe pliku do odczytu, nazwe pliku do zapisu oraz 2 napisy,n1 oraz n2.\n");
        exit(1);
    }

    struct tms *start_time = calloc(1, sizeof(struct tms));
    struct tms *end_time = calloc(1, sizeof(struct tms));
    clock_t real_start_time;
    clock_t real_end_time;
    
    #ifdef SYS
        real_start_time = times(start_time);

        sys_fun(argv[1], argv[2], argv[3], argv[4]);

        real_end_time = times(end_time);
        save_time("System functions", real_start_time, real_end_time, start_time, end_time);    
    #else
        real_start_time = times(start_time);

        lib_fun(argv[1], argv[2], argv[3], argv[4]);

        real_end_time = times(end_time);
        save_time("Library functions", real_start_time, real_end_time, start_time, end_time);
    #endif
    
    free(start_time);
    free(end_time);
    
    return 0;
}