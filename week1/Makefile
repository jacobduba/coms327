# Makefile - capitol M, rest lower case, no extension

#target: list of dependencies
#<literal tab>command to build target from the dependencies

# Purpose of make is automation and to speed up builds

all: hello fib timestable

hello: hello.c
	gcc hello.c -o hello

fib: fib.c
	gcc fib.c -o fib

timestable: timestable.c
	gcc timestable.c -o timestable

# These two lines are sufficient for assignment 0
# tour: tour.c
# 	gcc tour.c -o tour

clean:
	rm -f hello fib timestable

