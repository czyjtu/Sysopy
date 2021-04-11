#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <signal.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>

int num_received = 0;
int wait = true;
int COUNT_SIGNAL;
int STOP_SIGNAL;
union sigval value;
char *mode;

void handler(int sig, siginfo_t *info, void *ucontext)
{
    if (sig == COUNT_SIGNAL)
    {
        num_received++;

        if (strcmp("kill", mode) == 0 || strcmp("sigrt", mode) == 0)
        {
            kill(info->si_pid, COUNT_SIGNAL);
        }
        else
        {
            sigqueue(info->si_pid, COUNT_SIGNAL, value);
        }
    }
    else if (sig == STOP_SIGNAL)
    {
        if (strcmp("kill", mode) == 0 || strcmp("sigrt", mode) == 0)
        {
            kill(info->si_pid, STOP_SIGNAL);
        }
        else
        {
            value.sival_int = num_received;
            sigqueue(info->si_pid, STOP_SIGNAL, value);
        }
        wait=false;
    }
}

int main(int argc, char *argv[]){
    if (argc != 2) 
        return -1;
    printf("PID catchera: %d\n", getpid());
    mode = argv[3];

    if (strcmp(argv[1], "KILL") ==0 || strcmp(argv[1], "SIGQUEUE") ==0){
        COUNT_SIGNAL = SIGUSR1;
        STOP_SIGNAL = SIGUSR2;
    }
    else{
        COUNT_SIGNAL = SIGRTMIN + 1;
        STOP_SIGNAL = SIGRTMIN + 2;
    }

    sigset_t block_all;
    sigfillset(&block_all);
    if (sigprocmask(SIG_BLOCK, &block_all, NULL) < 0)
    {
        perror("cant block signals\n");
        return 1;
    }

    value.sival_int = 0;
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
    printf("\tCATCHER:\nreceived: %d\n",num_received);
    return 0;
}