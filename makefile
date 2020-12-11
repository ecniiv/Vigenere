CC = gcc
CFLAGS = -std=c11 -Wall -Wconversion -Wextra -Wpedantic
LDFLAGS =
objects = main.o group.o
executable = main

all: $(executable)

clean:
	$(RM) $(objects) $(executable)

$(executable): $(objects)
	$(CC) $(objects) $(LDFLAGS) -o $(executable)

tar: clean
	tar -zcf "$(CURDIR).tar.gz" *.c *.h makefile

main.o: main.c group.o
group.o: group.c group.h
