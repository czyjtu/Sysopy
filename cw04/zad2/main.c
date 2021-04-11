#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>

void handler_SIGINFO(int sig, siginfo_t *info, void *ucontext)
{
    printf("Signal number %d\n", info->si_signo);
    printf("Sending process ID %d\n", info->si_pid);
    printf("Child exit value %d\n", info->si_status);
}

int main(int argc, char* argv[]){
    struct sigaction act;
    // stop the child didnt receive the signal
    if(strcmp(argv[1], "child_no_response")==0){
        act.sa_sigaction = &handler_SIGINFO;
        act.sa_flags = SA_SIGINFO | SA_NOCLDSTOP;

        sigaction(SIGCHLD, &act, NULL);
        pid_t id = fork();

        if (id == 0){
            while(1){}

            exit(123);
        }
        else{
            kill(id, SIGSTOP);
        }
        printf("Stopped the child but didnt receive the signal.\n");
    }
    // stopped the child and receive the signal
    if (strcmp(argv[1], "child_stop")==0){
        act.sa_sigaction = &handler_SIGINFO;
        act.sa_flags = SA_SIGINFO;

        sigaction(SIGCHLD, &act, NULL);
        pid_t id = fork();

        if (id == 0){
            while(1){}

            exit(123);
        }
        else{
            kill(id, SIGSTOP);
        }
        wait(NULL);
    }
    // exit from child and receive the signal
    if (strcmp(argv[1], "child_exit")==0){
        act.sa_sigaction = &handler_SIGINFO;
        act.sa_flags = SA_SIGINFO;

        sigaction(SIGCHLD, &act, NULL);
        pid_t id = fork();

        if (id == 0){
            exit(123);
        }
        wait(NULL);
    }
    return 0;
}