CC=gcc
CFLAGS=-g -Wall

all: sched

%.o: %.c
	$(CC) $(CFLAGS) -c $^

sched: IO.o scheduler.o linkedlist.o useful_funcs.o main.c
	$(CC) $(CFLAGS) -o $@ $^
	rm *.o

clean:
	rm sched
	rm -r *.dSYM/

