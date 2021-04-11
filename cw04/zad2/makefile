CC = gcc
FLAGS = -Wall

all: test clean

main: main.c
	$(CC) $(FLAGS) main.c -o main

test: main 
	./main child_exit
	./main child_stop
	./main child_no_response

clean:
	rm -f main 