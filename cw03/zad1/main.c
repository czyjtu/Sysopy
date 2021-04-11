#include<stdio.h> 
#include<stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include<sys/wait.h>

int main(int argc, char* argv[]) { 
    if(argc != 2){
        printf("wrong number of parameters, pass number.\n");
        return(1);
    }

    int n = atoi(argv[1]);
    pid_t p;
    printf("parent pid: %d\n", getpid());

    for(int i=0; i < n; i++){ 
        p = fork();

        if(p == 0){ // jeśli jesteśmy w procesie potomnym
            printf("child pid: %d paarent pid: %d  i = %d\n",getpid(),getppid(), i); 
            exit(0); // proces potomny wejdzie do ifa i zakończy działanie
        }
    }

    while (wait(NULL) > 0); // czekam az wszystkie procesy sie zakoncza

    return 0;
}