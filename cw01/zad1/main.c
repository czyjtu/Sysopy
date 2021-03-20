#include<stdio.h>
#include<stdlib.h>


int main()
{
    FILE* fptr1=fopen("a.txt", "r");
    FILE* fptr2=fopen("b.txt", "r");
    FILE* fptr3=fopen("ab.txt", "w");
    int bufferLEN = 255;
    char line1[bufferLEN];
    char line2[bufferLEN];
    if(aPtr == NULL || bPtr == NULL){
            perror("Error ");
            exit(EXIT_FAILURE);
        }

    int num_lines = 0;
    while(fgets(line1, bufferLEN, fptr1) && fgets(line2, bufferLEN, fptr1)){
        fputs(line1, fptr3);
        fputs(line2, fptr3);
        num_lines ++;
    }
    fclose(fptr1);
    fclose(fptr2);
    fclose(fptr3);

    return 0;
}