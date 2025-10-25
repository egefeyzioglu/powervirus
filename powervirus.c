#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <signal.h>
#include <pthread.h>

volatile bool should_exit = false;

void print_usage(char *argv0){
    printf("Usage: %s num_jobs\n", argv0);
}

void *yeet(void *args){
    // Housekeeping stuff
    // Args unused
    (void) args;
    // We want `a` to be a memory location, so it is not optimized into a
    // register. Also we want to avoid it being zero, because otherwise
    // the actual FP operation could be optimized away in the hardware
    // level, so we initialize it instead of working with whatever the
    // memory happened to be set to.
    volatile double *a;
    a = malloc(sizeof(double));
    if (!a) {
        perror("Cannot reserve space for double");
        return NULL;
    }
    *a = 1.01;
    // Busy loop with conditional and FP ops
    while (!should_exit) {
        *a *= 1.01;
        if(*a > 1000) *a = 1.01;
    }
    // Cleanup
    free((double*) a);
    return NULL;
}

void handle_sigint(){
    fprintf(stderr, "\nCaught SIGINT, exiting...\n");
    should_exit = true;
}

int main(int argc, char **argv) {
    if(argc != 2){
        print_usage(argv[0]);
        return 1;
    }

    int num_jobs = atoi(argv[1]);
    if(num_jobs == 0){
        print_usage(argv[0]);
        return 1;
    }

    signal(SIGINT, handle_sigint);

    pthread_t threads[num_jobs];

    while(num_jobs){
        --num_jobs;
        pthread_create(&threads[num_jobs], NULL, &yeet, (void*) NULL);
    }
    for (int i = 0; i < sizeof(threads)/sizeof(threads[0]); i++) {
        pthread_join(threads[i], NULL);
    }
}

