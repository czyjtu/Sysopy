CC = gcc
FLAGS = -Wall
LIB_NAME = lib_diff


static:
	$(CC) $(FLAGS) -c library.c -o library.o
	ar rcs library.a library.o
	rm -f *.o

shared:
	$(CC) -fPIC $(FLAGS) -c library.c -o library.o
	$(CC) -fPIC $(FLAGS) -shared -o library.so library.o
	rm -f *.o

zad2:
	$(CC) library.c main.c -o main
	./main merge_files 4 a.txt b.txt a.txt b.txt remove_block 1 remove_row 1 5 
	./main merge_files 10 aa.txt bb.txt aa.txt bb.txt aa.txt bb.txt aa.txt bb.txt aa.txt bb.txt remove_block 1 remove_row 1 5
	./main merge_files 16 aaa.txt bbb.txt aaa.txt bbb.txt aaa.txt bbb.txt aaa.txt bbb.txt aaa.txt bbb.txt aaa.txt bbb.txt aaa.txt bbb.txt aaa.txt bbb.txt remove_block 1 remove_row 1 5
	make clean

clean:
	rm -f *.o
	rm -f *.a
	rm -f *.so
	ls | grep 'merged.*.txt' | xargs rm -f
