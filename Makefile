CC=gcc
CFLAGS=-g -Wall -Wuninitialized

all: schedProgram

%.o: %.c
	$(CC) $(CFLAGS) -c $^

schedProgram: IO.o scheduler.o process.o linkedlist.o useful_funcs.o main.c
	$(CC) $(CFLAGS) -o $@ $^
	rm *.o
	
run:
	@./schedProgram

runall:
	@python runall.py

scratchwork:
	$(CC) scratchwork.c
	@./a.out
	@rm a.out

clean:
	@rm schedProgram
	@rm -r *.dSYM/

cleanall:
	@python cleanall.py