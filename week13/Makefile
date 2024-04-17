huh:
	gcc heap.c use_heap.c -o use_heap -Wall -static # static linking

	# Dynamic linking saves space, memory
	# All libaries are loaded exactly once in memory
	# So by using static linking you are loading the libary twice
	# Disk is cheap but memory isn't

	# Build static libarries with
	ar -cvq lib<name>.a <file1>.o, ... <filen>.o

	# Compile with
	gcc use_heap.c -o use_heap -lheap -L
	# -L says to use the current directory
