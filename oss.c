#include<stdio.h>
#include<sys/types.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/wait.h>
#include<sys/ipc.h>
#include<sys/shm.h>

#define MAXDIGITS 2

//Set up system command options
typedef struct{
    int proc; //Total Children to Launch (-n)
    int simul; //How many children can run simultaneously (-s)
    int iter; //User pass iterations to program (-t)
} options_t;

//Set up Help command
void print_usage(const char * app){
    fprintf(stderr, "usage: %s [-h] [-n proc] [-s simul] [-t iter]\n", app);
    fprintf(stderr, "   proc is the total amount of children\n");
    fprintf(stderr, "   simul is how many children can run simultaneously\n");
    fprintf(stderr, "   iter is how many iterations the children will run\n");
}

int terminateCheck(){
    int status = 0;
    pid_t terminatedChild = waitpid(0, &status, WNOHANG);
    if(terminatedChild > 0){
        return terminatedChild;
    }
    else if(terminatedChild == 0){
        return 0;
    }
    else{
        return -1;
    }
}


int main(int argc, char* argv[]){
    //Set up defaults for system calls    
    options_t options;
    options.proc = 7;
    options.simul = 3;
    options.iter = 2;

    const char optstr[] = "hn:s:t:";

    //Parse system call
    char opt;
    while((opt = getopt(argc, argv, optstr))!= -1){
        switch(opt){
            case 'h':
                print_usage(argv[0]);
                return(EXIT_SUCCESS);
            case 'n':
                options.proc = atoi(optarg);
                break;
            case 's':
                options.simul = atoi(optarg);
                break;
            case 't':
                options.iter = atoi(optarg);
                break;
            default:
                printf("Invalid options %c\n", optopt);
                print_usage(argv[0]);
                return(EXIT_FAILURE);
        }
    }
    //Set up counters and variables 

    int terminatedChildren = 0;
    int simulCount = 0;
    int processCount = 0;

    //Start fork loop
    while(terminatedChildren<options.proc){
        //Debug Statements
        int terminatedChild = 0;
        if(simulCount > 0 && (terminatedChild = terminateCheck()) < 0){
            perror("Wait for PID failed\n");
        }
        else if(terminatedChild > 0){
            terminatedChildren++;
            simulCount--;
        }
        pid_t child = 0;
        //Start of child processes
        if(
            simulCount < options.simul &&
            processCount < options.proc &&
            (child = fork()) == 0){
            
            //Set integer from options.iter into string to pass
            char execIter[MAXDIGITS];
            sprintf(execIter, "%d", options.iter);
            char * test = execIter;
            char * args[] = {"./user", execIter, 0};

            //Execute 
            execlp(args[0], args[0], args[1], NULL);
            
            //Error checking
            fprintf(stderr, "Exec failed, terminating\n");
            exit(1);
        }
        //This statement ends loop when total processes are finished
        if(child > 0){
            simulCount++;
            processCount++;
        }
        
   }
        return (EXIT_SUCCESS);
}


