#include <stdio.h>

void main(int iter){
    for(int i = 0; i < iter; i++){
        printf("USER PID: %d PPID: %d Iteration: %d before sleeping", getpid(), getppid(), iter);
        sleep(1);
        printf("USER PID: %d PPID: %d Iteration: %d after sleeping", getpid(), getppid(), iter);
    }
}


