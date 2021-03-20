CC = gcc
FLAGS = -Wall

.PHONY: clean

library.a: library.o
	ar rcs library.a library.o

library.o: library.c
	$(CC) $(FLAGS) -c library.c -o library.o

library_shared.o: library.c
	$(CC) -fPIC $(FLAGS) -c library.c -o library_shared.o

library.so: library_shared.o
	$(CC) -fPIC $(FLAGS) -shared -o library.so library_shared.o

clean:
	rm -f *.o
	rm -f *.a
	rm -f *.so
	ls | grep 'merged.*.txt' | xargs rm -f