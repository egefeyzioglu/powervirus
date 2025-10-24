#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

void print_usage(char *argv0){
    printf("Usage: %s numjobs\n", argv0);
}

void yeet(){
    volatile unsigned int a = 0;
    while (1) {
        ++a;
    }
}

int main(int argc, char **argv) {
    if(argc != 2){
        print_usage(argv[0]);
        return 1;
    }

    int numjobs = atoi(argv[1]);
    if(numjobs == 0){
        print_usage(argv[0]);
        return 1;
    }

    while(numjobs){
        --numjobs;
        if(fork() == 0){
            yeet();
        }
    }
    while(wait(NULL) > 0);
}
