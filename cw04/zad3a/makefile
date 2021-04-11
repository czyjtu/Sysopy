CC = gcc
FLAGS = -Wall 

all:
	make catcher
	make sender

catcher:
	$(CC) $(FLAGS) catcher.c -o catcher

sender:
	$(CC) $(FLAGS) sender.c -o sender

clean:
	rm -f catcher sender