CC = gcc
FLAGS = -Wall

library.a: library.o
	ar rcs library.a library.o

library.o: library.c
	$(CC) $(FLAGS) -c library.c -o library.o

library_shared.o: library.c
	$(CC) -fPIC $(FLAGS) -c library.c -o library_shared.o

library.so: library_shared.o
	$(CC) -fPIC $(FLAGS) -shared -o library.so library_shared.o

test: library.c main.c
	$(CC) library.c main.c -o test

zad2: test
	./test create_table 2 merge_files 4 a.txt b.txt a.txt b.txt remove_block 1 remove_row 1 5 remove_and_add 
	./test create_table 5 merge_files 10 aa.txt bb.txt aa.txt bb.txt aa.txt bb.txt aa.txt bb.txt aa.txt bb.txt remove_block 1 remove_row 1 5 remove_and_add 
	./test create_table 8 merge_files 16 aaa.txt bbb.txt aaa.txt bbb.txt aaa.txt bbb.txt aaa.txt bbb.txt aaa.txt bbb.txt aaa.txt bbb.txt aaa.txt bbb.txt aaa.txt bbb.txt remove_block 1 remove_row 1 5 remove_and_add 

clean:
	rm -f *.o
	rm -f *.a
	rm -f *.so
	rm -f test
	ls | grep 'merged.*.txt' | xargs rm -f



