#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <stdbool.h>

int num_received = 0;
int catcher_received;
int wait = true;
int COUNT_SIGNAL;
int STOP_SIGNAL;

void handler(int sig, siginfo_t *info, void *ucontext){
    if(sig == COUNT_SIGNAL){
        num_received ++;
    }
    else if (sig == STOP_SIGNAL){
        wait = false;
        if (info->si_code == SI_QUEUE) {
            catcher_received = info->si_value.sival_int;
        }
    }

}

int main(int argc, char *argv[]){
    if (argc != 4) 
        return -1;

    pid_t catcher_pid = atoi(argv[1]);
    int num_sent = atoi(argv[2]);
    if (strcmp(argv[3], "KILL") ==0 || strcmp(argv[3], "SIGQUEUE") ==0){
        COUNT_SIGNAL = SIGUSR1;
        STOP_SIGNAL = SIGUSR2;
    }
    else{
        COUNT_SIGNAL = SIGRTMIN + 0;
        STOP_SIGNAL = SIGRTMIN + 1;
    }

    sigset_t block_all;
    sigfillset(&block_all);
    sigdelset(&block_all, SIGINT);
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

    if(strcmp(argv[3], "KILL")==0 || strcmp(argv[3], "SIGRT")==0 ){
        for(int i=0; i< num_sent; i++){
            kill(catcher_pid, COUNT_SIGNAL) 
            // if (kill(catcher_pid, COUNT_SIGNAL) == -1) {
            //     perror(NULL);
            //     return 2;
            // }
        }
        kill(catcher_pid, STOP_SIGNAL);


        while(wait){
            sigsuspend(&unblock);
        }
        printf("\tSENDER:\nsent: %d\nreceived: %d\n", num_sent, num_received);
    }
    else if(strcmp(argv[3], "SIGQUEUE")==0){
        union sigval value;
        value.sival_int = 0;
        for (int i = 0; i < num_sent; ++i){   
            sigqueue(catcher_pid, COUNT_SIGNAL, value)
            // if(sigqueue(catcher_pid, COUNT_SIGNAL, value) == -1){
            //     perror(NULL);
            //     return 3;
            // }
        }
        sigqueue(catcher_pid, STOP_SIGNAL, value);

        while(wait){
            sigsuspend(&unblock);
        }
        printf(
            "\tSENDER:\nsent: %d\nreceived: %d\ncactcher received : %d\n", 
            num_sent, 
            num_received,
            catcher_received
            );       

    }

    return 0;
}