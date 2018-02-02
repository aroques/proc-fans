CC=gcc
CFLAGS=-I. -g -Wall # show all warnings, preserve info for GDB

proc_fan: proc_fan.o r_wait.o
	$(CC) -o proc_fan proc_fan.o r_wait.o -I.

clean:
	rm *.o proc_fan