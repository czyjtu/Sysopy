#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <signal.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>

int num_received = 0;
int wait = true;
pid_t sender_pid;
int COUNT_SIGNAL;
int STOP_SIGNAL;

void handler(int sig, siginfo_t *info, void *ucontext)
{
    if(sig == COUNT_SIGNAL)
        num_received ++;
    else if (sig == STOP_SIGNAL){
        sender_pid = info->si_pid;
        wait = false;
    }
}

int main(int argc, char *argv[]){
    if (argc != 2) 
        return -1;
    printf("PID catchera: %d\n", getpid());

    if (strcmp(argv[1], "KILL") ==0 || strcmp(argv[1], "SIGQUEUE") ==0){
        COUNT_SIGNAL = SIGUSR1;
        STOP_SIGNAL = SIGUSR2;
    }
    else{
        COUNT_SIGNAL = SIGRTMIN + 0;
        STOP_SIGNAL = SIGRTMIN + 1;
    }

    sigset_t block_all;
    sigfillset(&block_all);
    if (sigprocmask(SIG_BLOCK, &block_all, NULL) < 0)
    {
        perror("cant block signals\n");
        return 1;
    }

    sigset_t unblock;
    sigfillset(&unblock);
    sigdelset(&unblock, SIGINT);
    sigdelset(&unblock, COUNT_SIGNAL);
    sigdelset(&unblock, STOP_SIGNAL);

    struct sigaction act;
    act.sa_sigaction = handler;
    act.sa_flags = SA_SIGINFO;
    sigfillset(&act.sa_mask);

    sigaction(COUNT_SIGNAL, &act, NULL);
    sigaction(STOP_SIGNAL, &act, NULL);

    while(wait){
        sigsuspend(&unblock);
    }
    if (strcmp(argv[1], "KILL")==0 || strcmp(argv[1], "SIGRT")==0){
        for (int i = 0; i < num_received; ++i){
            kill(sender_pid, COUNT_SIGNAL)
            // if (kill(sender_pid, COUNT_SIGNAL) == -1){
            //     perror(NULL);
            //     return 2;
            // }
        }
        kill(sender_pid, STOP_SIGNAL);
    }
    else if(strcmp(argv[1], "SIGQUEUE")==0){
        union sigval value;
        value.sival_int = 0;
        for (int i = 0; i < num_received; ++i)
        {
            value.sival_int = i+1;
            sigqueue(sender_pid, COUNT_SIGNAL, value) 
            // if(sigqueue(sender_pid, COUNT_SIGNAL, value) == -1){
            //     perror(NULL);
            //     return 3;
            // }
        }
        sigqueue(sender_pid, STOP_SIGNAL, value);
    }
    
    printf("\tCATCHER:\nreceived: %d\n",num_received);
    return 0;
}