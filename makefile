all: shell.o main.o
	gcc -o shell main.o shell.o

shell.o: shell.c shell.h
	gcc -c parse.c

main.o: main.c
	gcc -c main.c

clean:
	rm -rf *.o

run:
	./shell
