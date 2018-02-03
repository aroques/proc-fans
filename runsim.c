/*
 * A program that creates a fan of n processes where n is passed as a command-line argument.
 */
#include <stdio.h>
#include <string.h>
#include <err.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

const int MAX_CANON = 100;

int main (int argc, char *argv[]) {
    int proc_limit;         // Max number of concurrent children procceses
    int proc_count = 0;     // Number of concurrent children
    char cmd[MAX_CANON];    // Commmand read from stdin
    pid_t childpid = 0;     // Child process ID

    if (argc != 2) { /* check for valid number of command-line arguments */
        fprintf(stderr, "Usage: %s max number of simultaneous (children) processes\n", argv[0]);
        return 1;
    }
    
    printf("parent pid = %d\n\n", getpid());

    proc_limit = atoi(argv[1]);
    
    while (fgets(cmd, MAX_CANON, stdin)) {
        
        if (proc_count == proc_limit) {
            // Wait for one child to finish and decrement proc_count
            printf("Waiting on child becuase limit was reached...\n");
            wait(NULL);
            proc_count -= 1;
            printf("We waited on a child. proc count = %d\n\n", proc_count);
        }
        
        if ((childpid = fork()) == 0) { // child so...
            // Execute a program coressponding to that command line
            strtok(cmd, "\n"); // extract token \n
            printf("I'm a child. The cmd im supposed to execute is: (pid = %d) %s\n", getpid(), cmd);
            break;
        }
        
        if (childpid == -1) {
            perror("child failed to fork!\n\n");
            return 1;
        }
        
        proc_count += 1; // increment because we forked
        printf("Forked a child. proc count = %d\n\n", proc_count);

        // if (waitpid(-1, NULL, WNOHANG) > 0) {
        //     proc_count -= 1;
        //     printf("A child finished. proc count = %d\n\n", proc_count);
        // }
    }
    
    if (childpid > 0) {
        // were parent
        printf("\nParent waiting on all the children...\n");
        while (wait(NULL) > 0); // wait for all children
        printf("Parent exiting becuse all children done...\n\n");
    }

    if (childpid == 0) {
        printf("Child %d exiting ...\n\n", getpid());
    }

    return 0;
    
}