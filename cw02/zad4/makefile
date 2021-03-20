CC = gcc
FLAGS = -Wall -Werror

all: run_lib run_sys clean

run_lib: main.c
	$(CC) $(FLAGS) main.c -D LIB -o run_lib
	./run_lib lyrics.txt sysopy.txt "drugs" "sysopy exercises"

run_sys: main.c
	$(CC) $(FLAGS) main.c -D SYS -o run_sys
	./run_sys lyrics.txt sysopy.txt "drugs" "sysopy exercises"
	
clean:
	rm -f run_lib run_sys 

clean_all:	
	rm -f run_lib run_sys 
	rm -f sysopy.txt