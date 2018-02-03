CC = gcc
CFLAGS = -I. -g -Wall

EXEC = runsim
OBJS = runsim.o
DEPS =

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

all: $(OBJS)
	gcc -o $(EXEC) $^ $(CFLAGS)

clean:
	rm $(EXEC) $(OBJS)