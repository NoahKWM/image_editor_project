CC = gcc
CFLAGS = -Wall -g -pedantic

all: prog

types.o: types.c types.h
	gcc -Wall -Wextra -c types.c

prog.o: projet.c types.h
	gcc -Wall -Wextra projet.c types.c

prog: projet.o types.o
	gcc -Wall -Wextra projet.o types.o -o prog