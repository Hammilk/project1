#include<stdio.h>
#include<sys/types.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/wait.h>
#include<sys/ipc.h>
#include<sys/shm.h>

//Define maximum digits of iterations
#define MAXDIGITS 2
#define SHMKEY 516158
#define BUFF_SZ sizeof (int)

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


int main(int argc, char* argv[]){
    //Set up defaults for system calls    
    options_t options;
    options.proc = 1;
    options.simul = 1;
    options.iter = 1;

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

    int processCount = 0;
    //int simulCount = 0;
    int status;
    int terminatedChild;
    pid_t p;

    //Set up Shared Memory
    int shmid = shmget(SHMKEY, BUFF_SZ, 0777 | IPC_CREAT);
    if(shmid == -1){
        fprintf(stderr, "Error in shmget\n");
        exit(1);
    }

    char * paddr = (char *)(shmat(shmid, 0, 0));
    int * pSimulCount = (int *)(paddr);
    *pSimulCount = 0;

    //Note on control on simultaneous processes//
    //This function uses shared memory to track simultaneous processes
    //The function increments simultaneous process counter at the beginning of fork process on line 99 
    //Then the function decrements the counter in child executable file (user.c)
    //This design was chosen so that the counter updates precisely when the child begins and ends

    //Start fork loop
    while(processCount<options.proc){
        //Debug Statements
       // printf("Parent ID: %d\n", getpid());
       // printf("Count: %d\n", processCount);
       // printf("Simul: %d\n", *pSimulCount);
       // printf("Child ID: %d\n", p);
        
        //Start of child processes
        if((p = fork()) == 0 ){

            //Set up child shared memory pointer
            int * cSimulCount = (int *)(shmat(shmid, 0, 0));
            (*cSimulCount)++;
            
            //printf("Point check: %d\n", *cSimulCount);
            //printf("Child launched ID: %d\n", getpid());
            
            //Set integer from options.iter into string to pass
            char execIter[MAXDIGITS];
            sprintf(execIter, "%d", 2);
            char * test = execIter;
            char * args[] = {"./user", execIter, 0};
            shmdt(cSimulCount);

            //Execute 
            execlp(args[0], args[0], args[1], NULL);
            
            //Error checking
            fprintf(stderr, "Exec failed, terminating\n");
            exit(1);
        }
        //This statement ends loop when total processes are finished
        else if(p > 0 && processCount > (options.proc - 2)){
            terminatedChild = waitpid(p, &status, 0);
            //printf("Loop done\n");

            //Free up shared memory pointer
            shmdt(pSimulCount);
            shmctl(shmid, IPC_RMID, NULL);

            break;
        }
        //This loop halts parent execution while concurrent process capacity is reached
        else if(p > 0 && ((*pSimulCount) > (options.simul-3))){
            //printf("In Termination Loop with %d simultaneous processes\n", *pSimulCount);
            processCount++;
            terminatedChild = waitpid(-1, &status, 0);
            //printf("Child PID: %d has been terminated with status code: %d\n", terminatedChild, WEXITSTATUS(status));
            
        }
        //Increments process counter if parent has not reach concurrency limit
        else if(p > 0){
            /*
            if((waitpid(-1, &status, WNOHANG))>0){
                printf("testDecrement\n");
                simulCount--;
            }
            simulCount++;
            */
            processCount++;
            //printf("Normal Parent Loop\n");
        }
        else{
            perror("Fork failed\n");
        }
    }
        return (EXIT_SUCCESS);
}


