/*
 *
 * library_a.c
 * Implements a parent process that writes a 
 * 1024 byte string to a pipe to be read by
 * a child process. 
 *
 *
 * Alterations
 * 26 Dec 2024:  Carlos Molina-Jimenez (carlos.molina@cl.cam.ac.uk)
 *
 *
 * Compilation
   cm770@morello-camb-3:$ date
   Thu Dec 26 22:24:47 UTC 2024
   cm770@morello-camb-3:$ 

   1) To create object file
   $ cm770@morello-camb-3: clang-morello -march=morello+c64 -mabi=purecap-benchmark 
     -fPIC -c library_a.c -o library_a.o
     warning: Using c64 in the arch string is deprecated. The CPU 
     mode should be inferred from the ABI. [-Wdeprecated]

   2) To create the dynamic library
   $ cm770@morello-camb-3: clang-morello -march=morello+c64 -mabi=purecap-benchmark 
     -shared -o liblibrary_a.so library_a.o 
 */

#define _POSIX_C_SOURCE 199309L // Defined for export CLOCK_MONOTONIC

#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
#include "library_a.h"

#define STRLEN     1024
#define NUM_OF_MSG 100

void parent_process(int pipechan[]) {
    printf("I'm the PARENT process!\n");
    close(pipechan[1]);
    char buf1[STRLEN];
    struct timespec start_read, end_read;

    FILE *log_file = fopen("pipe-in-experiment-result.csv", "a");
    if (log_file == NULL) {
        printf("error: opening CSV file.\n");
        exit(1);
    }

    for (int i = 0; i < NUM_OF_MSG; i++) {
        if (clock_gettime(CLOCK_MONOTONIC, &start_read) == -1) {
            perror("clock_gettime failed");
            exit(-1);
        }

        if (read(pipechan[0], buf1, STRLEN) < 0) {
            printf("error: reading from pipe failed!!!\n");
            exit(-1);
        }

        if (clock_gettime(CLOCK_MONOTONIC, &end_read) == -1) {
            perror("clock_gettime failed");
            exit(-1);
        }

        double read_time = ((end_read.tv_sec - start_read.tv_sec) * 1000.0) +
                           ((end_read.tv_nsec - start_read.tv_nsec) / 1e6);

        double write_time;
        if (read(pipechan[0], &write_time, sizeof(double)) < 0) {
            printf("error: reading write time from pipe failed!!!\n");
            exit(-1);
        }

        double total_time = write_time + read_time;
        fprintf(log_file, "%d,%d,%.3f,%.3f,%.3f\n", i + 1, STRLEN, write_time, read_time, total_time);

        printf("i= %d\n!!!!!!!!msg received from child process %s : \n", i, buf1);
    }

    fclose(log_file);

    printf("Press Enter to terminate the PARENT process...\n");
    getchar();
}
