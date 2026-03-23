CC = gcc
CFLAGS = -Wall -Wextra -std=c99

CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++17

test: test.c duration.h
	$(CC) $(CFLAGS) -o test test.c
	./test

test_cpp: test.cpp duration.hpp
	$(CXX) $(CXXFLAGS) -o test_cpp test.cpp
	./test_cpp

all: test test_cpp

clean:
	rm -f test test_cpp example