#include <stdio.h> 
#include <stdlib.h> 
#include <time.h> 

int main(int argc, char* argv[]) {
    FILE* dane = fopen("dane.txt", "w");
    int count = 10000;
    int lower = 10;
    int upper = 10000;
    int even = 0;
    for (int i = 0; i < count; i++) { 
        int num = (rand() % 
           (upper - lower + 1)) + lower; 

        if (num%2 == 0) 
            even ++;

        fprintf(dane, "%d\n", num); 
    } 
    fclose(dane);
    return 0;
}