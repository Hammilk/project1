#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include "header.h"
#include <sys/shm.h>
#include <sys/ipc.h>

//Set up shared memory macro
#define SHMKEY 516158
#define BUFF_SZ sizeof(int)

int main(int argc, char** argv){
    
    //Set up shared memory
    int shmid = shmget(SHMKEY, BUFF_SZ, 0777);

    if(shmid == -1){
        fprintf(stderr, "Error in shmget in Exec file\n");
        exit(1);
    }
    int * eSimulCount = (int *)(shmat(shmid, 0, 0));

    //Change passed string (iterations) from string to integer
    int iterations = atoi(argv[1]);
    
    //Do work
    for(int i = 0; i < iterations; i++){
        printf("USER PID: %d PPID: %d Iteration: %d before sleeping\n", getpid(), getppid(), i+1);
        sleep(1);
        printf("USER PID: %d PPID: %d Iteration: %d after sleeping\n", getpid(), getppid(), i+1);
    }
    //Decrement simultaneous process count in shared memory
    (*eSimulCount)--;

    //Detach pointer
    shmdt(eSimulCount);


    exit(0);
}


