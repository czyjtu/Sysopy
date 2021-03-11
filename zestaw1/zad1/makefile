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

clean:
	rm -f *.o
	rm -f *.a
	rm -f *.so
