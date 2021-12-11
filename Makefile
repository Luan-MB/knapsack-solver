CC     = gcc
CFLAGS = -Wall

PROG = knapsack_solver
OBJS = knapsack_solver.o knapsack.o contador.o

.PHONY: clean purge all

all: $(PROG)

%.o: %.c
	$(CC) -c $(CFLAGS) $<

$(PROG) : % :  $(OBJS) %.o
	$(CC) -o $@ $^

clean:
	@rm -f *.o

purge:   
	@rm -f $(PROG)

