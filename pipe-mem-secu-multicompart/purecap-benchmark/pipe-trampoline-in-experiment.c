/*
 * pipe-trampoline-in-experiment.c
 * The main process that 
 * 1 creates a  pipe
 * 2 forks a child process
 * 3 the parent and the child use the pipe
 *   to communicate (the parent writes and chile reds)
 *   with each other. 
 * The parent and child code are implemented as dynamic
 * libraries.
 *
 * Alteration: 26 Dec 2024
 *
 * Carlos Molina-Jimenez  (carlos.molina@cl.cam.ac.uk)
 *
 * Compilation
 * $ cm770@morello-camb-3:$ clang-morello -march=morello+c64 -mabi=purecap-benchmark 
     pipe-trampoline-in-experiment.c -L. -llibrary_a -llibrary_b -o pipe_trampoline
     clang-14: warning: Using c64 in the arch string is deprecated. The CPU 
     mode should be inferred from the ABI. [-Wdeprecated]
    
   $ 
 */

#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <time.h>
#include <assert.h>
#include <string.h>
#include "library_a.h"
#include "library_b.h"

// STRLEN and  NUM_OF_MSG defined in the main file
#define STRLEN     1024
#define NUM_OF_MSG 100

int main() {
    int pipechan[2], child;

    FILE *log_file = fopen("pipe-in-experiment-purecap-benchmark-results.csv", "w");
    if (log_file == NULL) {
        printf("error: opening CSV file.\n");
        exit(1);
    }

    fprintf(log_file, "Test,Message Size (Bytes),Write Time (ms),Read Time (ms),Total Time (ms)\n");
    fclose(log_file);

    if (pipe(pipechan)) {
        printf("error: opening stream sockets pair\n");
        exit(10);
    }

    if ((child = fork()) == -1) {
        printf("error: fork child process failed\n");
        exit(-1);
    }

    if (child > 0) { // Parent process
        parent_process(pipechan);
    } else { // Child process
        child_process(pipechan);
    }

    return 0;
}
