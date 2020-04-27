CC=gcc
CFLAGS=-g -Wall

all: sched

%.o: %.c
	$(CC) $(CFLAGS) -c $^

sched: IO.o scheduler.o linkedlist.o useful_funcs.o main.c
	$(CC) $(CFLAGS) -o $@ $^
	rm *.o
	
run:
	./sched

scratchwork:
	$(CC) scratchwork.c
	./a.out
	rm a.out

clean:
	rm -r *.dSYM/
	rm sched
