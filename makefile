CC = gcc
CFLAGS = -Wall
SRC = tail.c stat.c search.c listfile.c
OBJ = $(SRC:.c=.o)

all: myls mysearch mystat mytail

myls: listfile.c
	gcc -o myls listfile.c

mysearch: search.c
	gcc -o mysearch search.c

mystat: stat.c
	gcc -o mystat stat.c -std=c99

mytail: tail.c
	gcc -o mytail tail.c -std=c99

clean:
	rm -f $(OBJ) mylist mysearch mystat mytail