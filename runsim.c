/*
 * A program that reads commands (with arguments) from stdin and then forks and execs those commands
 */
#include <stdio.h>
#include <string.h>
#include <err.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <ctype.h>
#include <getopt.h>

const int MAX_CANON = 100;

char** get_exec_argv(char* str);
void print_usage();
int* parse_cmd_line_args(int argc, char *argv[]);

int main (int argc, char *argv[]) {
    int* proc_limit;        // Max number of concurrent children procceses
    int proc_count = 0;     // Number of concurrent children
    
    char cmd[MAX_CANON];    // Commmand (with arugents) read from stdin
    pid_t childpid = 0;     // Child process ID
    char** exec_argv;       // argv to pass to execv

    proc_limit = parse_cmd_line_args(argc, argv);

    while (fgets(cmd, MAX_CANON, stdin)) {
        
        if (proc_count == *proc_limit) {
            // Wait for one child to finish and decrement proc_count
            wait(NULL);
            proc_count -= 1;
        }

        if ((childpid = fork()) == 0) {
            // Child so...
            // Execute a program coressponding to the first command line argument
            strtok(cmd, "\n"); // remove \n's

            exec_argv = get_exec_argv(cmd);

            execvp(exec_argv[0], exec_argv);
            
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

    free(proc_limit);

    return 0;

}

char** get_exec_argv(char* str) {
    // Returns an argv array

    char* substr;
    char** exec_argv = malloc(10 * sizeof(char));

    substr = strtok(str, " ");

    int i = 0;
    while (substr != NULL)
    {
        exec_argv[i] = malloc(20 * sizeof(char));
        exec_argv[i] = substr;
        substr = strtok(NULL, " ");
        i++;
    }
    exec_argv[i] = NULL; // Need NULL at end for execv

    return exec_argv;

}

int* parse_cmd_line_args(int argc, char *argv[]) {
    int* proc_limit = malloc(sizeof(int));

    if (argc < 2) {
        print_usage();
    }

    int option;
    while ((option = getopt (argc, argv, "n:h")) != -1) // colon = flag needs an argument
    switch (option) {
        case 'h':
            print_usage();
        case 'n':
            *proc_limit = atoi(optarg);
            break;
        default:
            print_usage();
    }

    return proc_limit;

}

void print_usage() {
    fprintf(stderr, "Usage: runsim -n max number of concurrent children\n");
    exit(2);
}