all: poke327

OBJS = sc_heap.o csv.o poke327.o

poke327: $(OBJS)
	echo Linking $@
	g++ $^ -g3 -lncurses -o poke327.out

%.o: %.c
	@$(ECHO) Compiling $<
	@$(CC) $(CFLAGS) -MMD -MF $*.d -c $<

clean:
	rm poke327.out
