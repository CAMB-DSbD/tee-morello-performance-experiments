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
 * Carlos Molina-Jimenez  (carlos.molina@cl.cam.ac.uk)
 *
 * Alterations: 26 Dec 2024   Carlos.Molina@cl.cam.ac.uk
 *            : 28 Dec 2024   Carlos.Molina@cl.cam.ac.uk
 *
 * Compilation
   1- Create library_a from library_a.c and library_a.h
   2- Create library_b from library_b.c and library_b.h
      Intructions are in the comment lines of 
      library_a.c and library_b.c
    
   3- $ clang-morello -march=morello -mabi=purecap-benchmark pipe-trampoline-in-experiment.c 
        -o pipe_trampoline -L. -llibrary_a -llibrary_b -Wl,-rpath,$(pwd)

   Execution
   4- $ proccontrol -m cheric18n -s enable ./pipe_trampoline
        cm770@morello-camb-3:$ proccontrol -m cheric18n -s enable ./pipe_trampoline
        I'm the PARENT process!
        I'm the CHILD process!
        i= 0
        !!!!!!!!msg received from child process 
        ...
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
