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
int num_send;
int num_to_send;
char *mode;
pid_t catcher_pid;
union sigval value;


void send_count_signal()
{
    num_send++;
    if (strcmp("kill", mode) == 0 || strcmp("sigrt", mode) == 0)
    {
        kill(catcher_pid, COUNT_SIGNAL);
    }
    else
    {
        sigqueue(catcher_pid, COUNT_SIGNAL, value);
    }
}

void send_stop_signal()
{
    if (strcmp("kill", mode) == 0 || strcmp("sigrt", mode) == 0){
        kill(catcher_pid, STOP_SIGNAL);
    }
    else{
        sigqueue(catcher_pid, STOP_SIGNAL, value);
    }
}

void handler(int sig, siginfo_t *info, void *ucontext){
    if(sig == COUNT_SIGNAL){
        num_received ++;
        if (num_send < num_to_send)
        {
            send_count_signal();
        }
        else
        {
            send_stop_signal();
        }
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

    catcher_pid = atoi(argv[1]);
    num_to_send = atoi(argv[2]);
    mode = argv[3];
    
     
    if (strcmp(argv[3], "KILL") ==0 || strcmp(argv[3], "SIGQUEUE") ==0){
        COUNT_SIGNAL = SIGUSR1;
        STOP_SIGNAL = SIGUSR2;
    }
    else{
        COUNT_SIGNAL = SIGRTMIN + 1;
        STOP_SIGNAL = SIGRTMIN + 2;
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
    
    send_count_signal();
    while(wait){
        sigsuspend(&unblock);
    }

    if(strcmp(argv[3], "SIGQUEUE")==0){
         printf(
            "\tSENDER:\nsent: %d\nreceived: %d\ncactcher received : %d\n", 
            num_send, 
            num_received,
            catcher_received
            );   
    }
    else{
        printf("\tSENDER:\nsent: %d\nreceived: %d\n", num_send, num_received);
    }


    return 0;
}