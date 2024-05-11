#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include "header.h"
#include <sys/ipc.h>

int main(int argc, char** argv){
    

    //Change passed string (iterations) from string to integer
    int iterations = atoi(argv[1]);
    
    //Do work
    for(int i = 0; i < iterations; i++){
        printf("USER PID: %d PPID: %d Iteration: %d before sleeping\n", getpid(), getppid(), i+1);
        sleep(1);
        printf("USER PID: %d PPID: %d Iteration: %d after sleeping\n", getpid(), getppid(), i+1);
    }
}


