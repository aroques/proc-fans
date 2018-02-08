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

char** get_argv(char* str);

int main (int argc, char *argv[]) {
    int proc_limit;         // Max number of concurrent children procceses
    int proc_count = 0;     // Number of concurrent children
    char cmd[MAX_CANON];    // Commmand (with arugents) read from stdin
    pid_t childpid = 0;     // Child process ID
    char** argv;            // argv to pass to execv


    if (argc != 2) { /* check for valid number of command-line arguments */
        fprintf(stderr, "Usage: %s max number of simultaneous (children) processes\n", argv[0]);
        return 1;
    }

    proc_limit = atoi(argv[1]);

    while (fgets(cmd, MAX_CANON, stdin)) {
        
        if (proc_count == proc_limit) {
            // Wait for one child to finish and decrement proc_count
            wait(NULL);
            proc_count -= 1;
        }

        if ((childpid = fork()) == 0) {
            // Child so...
            // Execute a program coressponding to the first command line argument
            strtok(cmd, "\n"); // remove \n's

            argv = get_argv(cmd);

            execvp(argv[0], argv);
            
            perror("Child failed to execvp the command");
            return 1;
        }
        
        if (childpid == -1) {
            perror("Child failed to fork!\n");
            return 1;
        }
        
        proc_count += 1; // increment because we forked

        if (waitpid(-1, NULL, WNOHANG) > 0) {
            // A child has finished executing
            proc_count -= 1;
        }

    }
    
    if (childpid > 0) {
        while (wait(NULL) > 0); // wait for all children
    }

    return 0;

}

char** get_argv(char* str) {
    // Returns an argv array

    char* substr;
    char** argv = malloc(10 * sizeof(char));

    substr = strtok(str, " ");

    int i = 0;
    while (substr != NULL)
    {
        argv[i] = malloc(20 * sizeof(char));
        argv[i] = substr;
        substr = strtok(NULL, " ");
        i++;
    }
    argv[i] = NULL; // Need NULL at end for execv

    return argv;

}