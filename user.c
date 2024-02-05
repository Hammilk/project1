#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include "header.h"


void user(int iter){
    for(int i = 0; i < iter; i++){
        printf("USER PID: %d PPID: %d Iteration: %d before sleeping\n", getpid(), getppid(), i+1);
        sleep(1);
        printf("USER PID: %d PPID: %d Iteration: %d after sleeping\n", getpid(), getppid(), i+1);
    }
}


