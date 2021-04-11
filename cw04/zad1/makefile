CC = gcc
FLAGS = -Wall

all: test_fork test_exec clean

main: main.c
	$(CC) $(FLAGS) main.c -o main

exec: exec.c
	$(CC) $(FLAGS) exec.c -o exec

test_exec: main exec
	./main ignore exec
	./main mask exec
	./main pending exec

test_fork: main 
	./main ignore 
	./main handler 
	./main mask 
	./main pending 

clean:
	rm -f main exec