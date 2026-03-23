CC = gcc
CFLAGS = -Wall -Wextra -std=c99

test: test.c duration.h
	$(CC) $(CFLAGS) -o test test.c
	./test

clean:
	rm -f test example