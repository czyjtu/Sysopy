CC = gcc
FLAGS = -Wall -Werror

all: run_lib run_sys clean

run_lib: main.c
	$(CC) $(FLAGS) main.c -D LIB -o run_lib
	./run_lib in.txt out.txt 

run_sys: main.c
	$(CC) $(FLAGS) main.c -D SYS -o run_sys
	./run_sys in.txt out.txt 
	
clean:
	rm -f run_lib run_sys 

clean_all:	
	rm -f run_lib run_sys 
	rm -f out.txt