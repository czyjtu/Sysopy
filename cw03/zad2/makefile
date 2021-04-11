CC = gcc
FLAGS = -Wall

test: library.c main.c
	$(CC) library.c main.c -o test

zad2: test
	./test 4 b.txt b.txt b.txt b.txt
	./test 10 bb.txt bb.txt bb.txt bb.txt bb.txt bb.txt bb.txt bb.txt bb.txt bb.txt
	./test 16 bbb.txt bbb.txt bbb.txt bbb.txt bbb.txt bbb.txt bbb.txt bbb.txt bbb.txt bbb.txt bbb.txt bbb.txt bbb.txt bbb.txt bbb.txt bbb.txt 

clean:
	rm -f *.o
	rm -f *.a
	rm -f *.so
	rm -f test
	ls | grep 'merged.*.txt' | xargs rm -f