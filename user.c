#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include "header.h"


int main(int argc, char** argv){
    

    int iterations = atoi(argv[1]);
    

    for(int i = 0; i < iterations; i++){
        printf("USER PID: %d PPID: %d Iteration: %d before sleeping\n", getpid(), getppid(), i+1);
        sleep(1);
        printf("USER PID: %d PPID: %d Iteration: %d after sleeping\n", getpid(), getppid(), i+1);
    }
    exit(1);
}


