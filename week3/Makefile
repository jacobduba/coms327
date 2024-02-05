# stack: main.c stack.c stack.h
# 	gcc main.c stack.c -o stack.out

stack: stack.o main.o
	gcc stack.o main.o -o stack.out

stack.o: stack.c stack.h
	gcc -Wall -Werror -g stack.c -c 

main.o: main.c stack.h
	gcc -Wall -Werror -g main.c -c

clean:
	rm -f *o stack.out
 
