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
    char* substr;
    char* arr[10];


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

        fflush(stdin);

        if ((childpid = fork()) == 0) { // child so...
            // Execute a program coressponding to that command line m
            // makeargv execvp
            strtok(cmd, "\n"); // extract token \n

            int i = 0;
            substr = strtok(cmd, " ");
            while (substr != NULL)
            {
                arr[i] = substr;
                printf("%s \n", arr[i]);
                substr = strtok(NULL, " ");
                i++;
            }
            arr[i] = NULL;
            printf("%s: is the program to execute\n", arr[0]);
            execvp(arr[0], arr);
            
            perror("Child failed to execvp the command");
            return 1;
        }
        
        if (childpid == -1) {
            perror("Child failed to fork!\n\n");
            return 1;
        }
        
        proc_count += 1; // increment because we forked

        if (waitpid(-1, NULL, WNOHANG) > 0) {
            proc_count -= 1;
            printf("A child finished. proc count = %d\n\n", proc_count);
        }
    }
    
    if (childpid > 0) {
        while (wait(NULL) > 0); // wait for all children
    }

    return 0;

}