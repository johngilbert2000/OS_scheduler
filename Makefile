CC=gcc
CFLAGS=-g -Wall

all: schedProgram

%.o: %.c
	$(CC) $(CFLAGS) -c $^

schedProgram: IO.o scheduler.o proc_step.o linkedlist.o useful_funcs.o main.c
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