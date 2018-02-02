/*
 * A program that creates a fan of n processes where n is passed as a command-line argument.
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include "r_wait.h"

int main (int argc, char *argv[]) {
    pid_t childpid = 0;
    int i, n;
    int pr_limit;     // Max number of children allowed to execute at a time (initialize from command line)
    int pr_count = 0; // Number of active children

    if (argc != 2) { /* check for valid number of command-line arguments */
        fprintf(stderr, "Usage: %s processes\n", argv[0]);
        return 1;
    }
    n = atoi(argv[1]);

    // Enter in a main loop
    //while (1) { // Until enf of file is reached on std in

    //}
    for (i = 1; i < n; i++) {
        if ((childpid = fork()) <= 0) {
            break;
        }
    }

    while(r_wait(NULL) > 0);

    fprintf(stderr, "run:%d, process ID:%ld, parent ID:%ld, child ID:%ld\n",
        i, (long)getpid(), (long)getppid(), (long)childpid);
    
    return 0;
}