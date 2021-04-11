#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>

void handler(int signal){
    printf("Ja cie! ale sygnał!\n");
}

int main(int argc, char* argv[]){
    if (argc != 2 && argc != 3){
        printf("wrong number of arguments");
        return -1;
    }
    if (strcmp(argv[1], "ignore") == 0){
        signal(SIGUSR1, SIG_IGN);
    }
    else if(strcmp(argv[1], "handler") == 0){
        signal(SIGUSR1, handler);
    }
    else if(strcmp(argv[1], "mask") == 0 || strcmp(argv[1], "pending") == 0){
        sigset_t newmask;
        sigset_t oldmask; 
        sigemptyset(&newmask); 
        sigaddset(&newmask, SIGUSR1); 
        if (sigprocmask(SIG_BLOCK, &newmask, &oldmask) < 0){
            perror("Nie udało się zablokować sygnału");
            return 1;
        }
    }

    raise(SIGUSR1);

    if (strcmp(argv[1], "pending") == 0){
        sigset_t waiting_mask;
        sigpending(&waiting_mask);
        printf("SIGUSR1 pending in parent: %d\n", sigismember(&waiting_mask, SIGUSR1));
    }

    if (argc == 3 && strcmp(argv[2], "exec") == 0){
        execl("./exec", "./exec", argv[1], NULL);
    }
    else{
        int id = fork();
        if (id < 0)
            return 2;

        if(id == 0){
            if(strcmp(argv[1], "pending") != 0){
                raise(SIGUSR1);
            }
            else{
                sigset_t waiting_mask;
                sigpending(&waiting_mask);
                printf("SIGUSR1 pending in child: %d\n", sigismember(&waiting_mask, SIGUSR1));
            }
        }
    }



    return 0;
}