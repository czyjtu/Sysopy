CC = gcc
FLAGS = -Wall -Werror

all: run_lib run_sys clean

run_lib: main.c
	$(CC) $(FLAGS) main.c -D LIB -o main_lib
	./main_lib d a.txt

run_sys: main.c
	$(CC) $(FLAGS) main.c -D SYS -o main_sys
	./main_sys d a.txt
	
clean:
	rm -f main_lib main_sys