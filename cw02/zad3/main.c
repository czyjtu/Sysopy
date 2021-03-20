#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h> 
#include <math.h>
#include <sys/times.h>

const int BUFFOR_LEN = 256;

double calculate_time(clock_t start,clock_t end){
    return (double) (end-start) / sysconf(_SC_CLK_TCK);
}

void save_time(char *operation, clock_t real_start_time, clock_t real_end_time, struct tms *end_time, struct tms *start_time){
    FILE* f = fopen("pomiar_zad_3.txt", "a");

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

int satisfy_condition(int num){
    if(num < 10)
        return 0;
    
    num /= 10;
    if(num%10 == 0 || num%10 == 7)
        return 1;
    
    return 0;
}

int is_perfect_square(int num){
    int int_var;
    float float_var;
 
    float_var=sqrt((double)num);
    int_var = (int)float_var;
 
    if(int_var == float_var)
        return 1;
    else
        return 0;
}

void lib_fun(){
    FILE *dane = fopen("dane.txt", "r");
    FILE *fa = fopen("a.txt", "w");
    FILE *fb = fopen("b.txt", "w");
    FILE *fc = fopen("c.txt", "w");

    if(dane == NULL || fa == NULL || fb == NULL || fc == NULL){
      perror("Error in opening file");
      exit(1);
    }
    char line[BUFFOR_LEN];
    char c;
    int i=0;
    int even = 0;

    while(fread(&c, sizeof(char), 1, dane) > 0){
        if(c == '\n'){
            int num = atoi(line);
            line[i++] = '\n';

            if(satisfy_condition(num) == 1)
                fwrite(line, 1, i, fb);
            
            if(is_perfect_square(num) == 1)
                fwrite(line, 1, i, fc);

            if(num % 2 == 0)
                even++;
            
            // clear buffor
            while(i > 0)
                line[--i] = 0;
            
            num = 0;
        }
        else
            line[i++] = c;
    }
    int len = 0;
    int tmp = even;
    while(tmp != 0){
        tmp /= 10;
        len++;
    }
    char num_str[23 + len];
    sprintf(num_str, "Liczb parzystych jest %d\n", even);
    fwrite(num_str, sizeof(char), 23 + len, fa);

    fclose(dane);
    fclose(fa);
    fclose(fb);
    fclose(fc);
}

void sys_fun(){
    int dane = open("dane.txt", O_RDONLY);
    int fa = open("a.txt", O_WRONLY|O_CREAT);
    int fb = open("b.txt", O_WRONLY|O_CREAT);
    int fc = open("c.txt", O_WRONLY|O_CREAT);

    if(dane == -1 || fa == -1 || fb == -1 || fc == -1){
      perror("Error in opening file");
      exit(1);
    }
    char line[BUFFOR_LEN];
    char c;
    int i=0;
    int even = 0;

    while(read(dane, &c, 1) > 0){
        if(c == '\n'){
            int num = atoi(line);
            line[i++] = '\n';

            if(satisfy_condition(num) == 1)
                write(fb, line, i);
            
            if(is_perfect_square(num) == 1)
                write(fc, line, i);

            if(num % 2 == 0)
                even++;
            
            // clear buffor
            while(i > 0)
                line[--i] = 0;
            
            num = 0;
        }
        else
            line[i++] = c;
    }
    int len = 0;
    int tmp = even;
    while(tmp != 0){
        tmp /= 10;
        len++;
    }
    char num_str[23 + len];
    sprintf(num_str, "Liczb parzystych jest %d\n", even);
    write(fa, num_str, 23 + len);

    close(dane);
    close(fa);
    close(fb);
    close(fc);
}

int main(int argc, char* argv[]) {
    struct tms *start_time = calloc(1, sizeof(struct tms));
    struct tms *end_time = calloc(1, sizeof(struct tms));
    clock_t real_start_time;
    clock_t real_end_time;
    
    #ifdef SYS
        real_start_time = times(start_time);

        sys_fun();

        real_end_time = times(end_time);
        save_time("System functions", real_start_time, real_end_time, start_time, end_time);
    #else
        real_start_time = times(start_time);

        lib_fun();

        real_end_time = times(end_time);
        save_time("Library functions", real_start_time, real_end_time, start_time, end_time);
    #endif

    free(start_time);
    free(end_time);

    return 0;
}