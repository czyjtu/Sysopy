#include <stdio.h>
#include <unistd.h>
#include <stdbool.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>

int main(int argc, char **argv)
{

    if(strcmp(argv[1], "pending") != 0){
        raise(SIGUSR1);
    }
    else{
        sigset_t waiting_mask;
        sigpending(&waiting_mask);
        printf("SIGUSR1 pending in exec: %d\n", sigismember(&waiting_mask, SIGUSR1));
    }
    return 0;
}