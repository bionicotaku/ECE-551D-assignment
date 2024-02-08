# Makefile for compiling power.c and test-power.c

CC = gcc
CFLAGS = -Wall -Werror -std=c99

all: test-power

power.o: power.c
	$(CC) $(CFLAGS) -c power.c -o power.o

test-power: power.o test-power.c
	$(CC) $(CFLAGS) power.o test-power.c -o test-power

clean:
	rm -f power.o test-power
