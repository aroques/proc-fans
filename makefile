CC=gcc
CFLAGS=-I.

process_fans: process_fans.o
	$(CC) -o process_fans process_fans.o -I.

clean:
	rm *.o process_fans