CC = gcc
CFLAGS = -Wall
SRC = tail.c stat.c search.c listfile.c
OBJ = $(SRC:.c=.o)

all: mylist mysearch mystat mytail

mylist: listfile.o
	$(CC) -o $@ $< $(CFLAGS)

mysearch: search.o
	$(CC) -o $@ $< $(CFLAGS)

mystat: stat.o
	$(CC) -o $@ $< $(CFLAGS)

mytail: tail.o
	$(CC) -o $@ $< $(CFLAGS)

%.o: %.c
	$(CC) -c -o $@ $< $(CFLAGS)

clean:
	rm -f $(OBJ) mylist mysearch mystat mytail