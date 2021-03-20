CC = gcc
FLAGS = -Wall -Werror

all: run_sys run_lib clean

main_lib: main_lib.c
	$(CC) $(FLAGS) main_lib.c -o main_lib

main_sys: main_sys.c
	$(CC) $(FLAGS) main_sys.c -o main_sys

run_lib: main_lib 
	./main_lib a.txt b.txt

run_sys: main_sys
	./main_sys a.txt b.txt

clean:
	rm -f main_lib main_sys
	