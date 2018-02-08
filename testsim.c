/*
 * A program that sleeps for sleep_factor seconds then prints it's pid repeat_factor times 
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main (int argc, char *argv[]) {

    int i;
    int repeat_factor; 
    int sleep_time;      

    if (argc != 3) { /* check for valid number of command-line arguments */
        fprintf(stderr, "Usage: %s repeat_factor sleep_time NULL\n", argv[0]);
        return 1;
    }
    
    repeat_factor = atoi(argv[1]);
    sleep_time = atoi(argv[2]);

    for (i = 0; i < repeat_factor; i++) {
        sleep(sleep_time);
        fprintf(stderr, "pid: %d _ repeat factor: %d _ sleep time: %d\n", getpid(), repeat_factor, sleep_time);
    }

    return 0;
}