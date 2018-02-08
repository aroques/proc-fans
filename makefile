CC = gcc
CFLAGS = -I. -g -Wall

EXEC = runsim
OBJS = runsim.o
DEPS =

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

all: runsim testsim

runsim: $(OBJS)
	gcc -o $(EXEC) $^ $(CFLAGS)

clean:
	rm $(EXEC) $(OBJS) testsim testsim.o

testsim: testsim.c
	$(CC) -o testsim testsim.c