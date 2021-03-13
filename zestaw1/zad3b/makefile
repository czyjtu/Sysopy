CC = gcc
FLAGS = -Wall 
LIB_NAME = lib_diff
LVL = 3

library.a: library.o
	ar rcs library.a library.o

library.o: library.c
	$(CC) $(FLAGS) -c library.c -o library.o

library_shared.o: library.c
	$(CC) -fPIC $(FLAGS) -c library.c -o library_shared.o

library.so: library_shared.o
	$(CC) -fPIC $(FLAGS) -shared -o library.so library_shared.o

main.o: main.c
	gcc -c main.c -o main.o

main_dynamic.o: main_dynamic.c
	gcc -c main_dynamic.c -o main_dynamic.o

test_static: library.a main.o
	$(CC) -O$(LVL) main.o library.a -o test_static

test_shared: main.o library.so
	$(CC) -L. -Wall -Wl,-rpath=. -O$(LVL) -o test_shared main.o -lrary

test_dynamic: main_dynamic.o library.so
	$(CC) -O$(LVL) -ldl main_dynamic.o ./library.so -ldl -o test_dynamic

zad3b: test_static test_shared test_dynamic
	echo '\t\t############ OPTIMIZING: -O$(LVL)#################\n\t\t#############################################\n' >> raport3b.txt
	echo '\t\t\t-----STATIC LIBRARY-----\n' >> raport3b.txt
	./test_static create_table 2 merge_files 4 a.txt b.txt a.txt b.txt remove_block 1 remove_row 1 5 remove_and_add 
	./test_static create_table 5 merge_files 10 aa.txt bb.txt aa.txt bb.txt aa.txt bb.txt aa.txt bb.txt aa.txt bb.txt remove_block 1 remove_row 1 5 remove_and_add 
	./test_static create_table 8 merge_files 16 aaa.txt bbb.txt aaa.txt bbb.txt aaa.txt bbb.txt aaa.txt bbb.txt aaa.txt bbb.txt aaa.txt bbb.txt aaa.txt bbb.txt aaa.txt bbb.txt remove_block 1 remove_row 1 5 remove_and_add 
	
	echo '\n\t\t\t-----SHARED LIBRARY-----\n' >> raport3b.txt
	./test_shared create_table 2 merge_files 4 a.txt b.txt a.txt b.txt remove_block 1 remove_row 1 5 remove_and_add 
	./test_shared create_table 5 merge_files 10 aa.txt bb.txt aa.txt bb.txt aa.txt bb.txt aa.txt bb.txt aa.txt bb.txt remove_block 1 remove_row 1 5 remove_and_add 
	./test_shared create_table 8 merge_files 16 aaa.txt bbb.txt aaa.txt bbb.txt aaa.txt bbb.txt aaa.txt bbb.txt aaa.txt bbb.txt aaa.txt bbb.txt aaa.txt bbb.txt aaa.txt bbb.txt remove_block 1 remove_row 1 5 remove_and_add 
	
	echo '\n\t\t\t-----DYNAMIC LIBRARY-----\n' >> raport3b.txt
	./test_dynamic create_table 2 merge_files 4 a.txt b.txt a.txt b.txt remove_block 1 remove_row 1 5 remove_and_add 
	./test_dynamic create_table 5 merge_files 10 aa.txt bb.txt aa.txt bb.txt aa.txt bb.txt aa.txt bb.txt aa.txt bb.txt remove_block 1 remove_row 1 5 remove_and_add 
	./test_dynamic create_table 8 merge_files 16 aaa.txt bbb.txt aaa.txt bbb.txt aaa.txt bbb.txt aaa.txt bbb.txt aaa.txt bbb.txt aaa.txt bbb.txt aaa.txt bbb.txt aaa.txt bbb.txt remove_block 1 remove_row 1 5 remove_and_add 
	make clean
clean:
	rm -f *.o
	rm -f *.a
	rm -f *.so
	rm -f test_shared test_static test_dynamic
	ls | grep 'merged.*.txt' | xargs rm -f
