CC = gcc
FLAGS = -Wall -Werror

all: generate run_lib run_sys clean

run_lib: main.c
	$(CC) $(FLAGS) main.c -D LIB -o main_lib -lm
	./main_lib

run_sys: main.c
	$(CC) $(FLAGS) main.c -D SYS -o main_sys -lm
	./main_sys

generate: generator.c
	$(CC) $(FLAGS) generator.c -o generator
	./generator
	
clean:
	rm -f run_lib run_sys generator

clean_all:
	rm -f main_lib main_sys generator
	rm -f dane.txt a.txt b.txt c.txt