#include<stdio.h>
#include<sys/types.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/wait.h>

//Define maximum digits of iterations
#define MAXDIGITS 2



typedef struct{
    int proc; //Total Children to Launch (-n)
    int simul; //How many children can run simultaneously (-s)
    int iter; //User pass iterations to program (-t)
} options_t;


void print_usage(const char * app){
    fprintf(stderr, "usage: %s [-h] [-n proc] [-s simul] [-t iter]\n", app);
    fprintf(stderr, "   proc is the total amount of children\n");
    fprintf(stderr, "   simul is how many children can run simultaneously\n");
    fprintf(stderr, "   iter is how many iterations the children will run\n");
}


int parent(int argc, char** argv[],int proc, int simul, int iter){
    pid_t childPid;
     
    if(proc > 0){

    if((childPid = fork()) == 0){

        char execIter[MAXDIGITS];
        sprintf(execIter, "%d", iter);
        char *test = execIter;
        char* args[] = {"./user", execIter, 0};

        
        execlp(args[0],args[0],args[1], NULL);

        fprintf(stderr,"Exec failed, terminating\n");
        exit(1);
    }
    else if (childPid>0){
        printf("I'm parent. PID is %d, and child PID is %d\n", getpid(), childPid);
        parent(argc, argv,  proc - 1, simul, iter);
        wait(0);
    }
    else{
        perror("Fork Failed\n");
    }
    return EXIT_SUCCESS;
    }
}

int main(int argc, char* argv[]){
    
    options_t options;
    options.proc = 1;
    options.simul = 1;
    options.iter = 1;

    const char optstr[] = "hn:s:t:";

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

    printf("test");
    char** args[3];

    parent(1, args,options.proc, options.simul, options.iter);

    return (EXIT_SUCCESS);
}


