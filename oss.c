#include<stdio.h>
#include"header.h"
#include<stdlib.h>
#include<unistd.h>



typedef struct{
    int proc; //Total Children to Launch (-n)
    int simul; //How many children can run simultaneously (-s)
    int iter; //User pass iterations to program (-t)
} options_t;


void print_usage(const char * app){
    fprintf(stderr, "usage: %s [-h] [-n proc] [-s simul] [-iter]\n", app); //Check if this is actually /app
    fprintf(stderr, "   proc is the total amount of children\n");
    fprintf(stderr, "   simul is how many children can run simultaneously\n");
    fprintf(stderr, "   iter is how many iterations the children will run\n");
}



int main(int argc, char *argv[]){
    
    options_t options;
    options.proc = 0;
    options.simul = 0;
    options.iter = 0;

    const char optstr[] = "hn:s:t:";

    char opt;
    while((opt = getopt(argc, argv, optstr))!= -1){
        switch(opt){
            case 'h':
                print_usage(argv[0]);
                printf("test h\n");
                return(EXIT_SUCCESS);
            case 'n':
                options.proc = atoi(optarg);
                printf("test %i\n", atoi(optarg));
                break;
            case 's':
                options.simul = atoi(optarg);
                printf("test %i\n", atoi(optarg));
                break;
            case 't':
                options.iter = atoi(optarg);
                printf("test %i\n", atoi(optarg));
                break;
            default:
                printf("Invalid options %c\n", optopt);
                print_usage(argv[0]);
                return(EXIT_FAILURE);
        
        }
    }
    exit(EXIT_SUCCESS);





}

