all: poke327

CFLAGS = -Wall -ggdb -funroll-loops -DTERM=$(TERM)
OBJS = sc_heap.o csv.o poke327.o

poke327: $(OBJS)
	echo Linking $@
	g++ $^ $(CFLAGS) -lncurses -o poke327.out

%.o: %.cpp
	echo Compiling $<
	g++ $(CFLAGS) -g3 -c $<

clean:
	rm $(OBJS) poke327.out
