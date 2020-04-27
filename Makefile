CC=gcc
CFLAGS=-g -Wall

all: yourProgram

%.o: %.c
	$(CC) $(CFLAGS) -c $^

yourProgram: IO.o scheduler.o linkedlist.o useful_funcs.o main.c
	$(CC) $(CFLAGS) -o $@ $^
	rm *.o
	
run:
	@./yourProgram

scratchwork:
	$(CC) scratchwork.c
	@./a.out
	@rm a.out

clean:
	@rm yourProgram
	@rm -r *.dSYM/
