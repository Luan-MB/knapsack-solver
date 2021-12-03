CC     = gcc
CFLAGS = -Wall -DDEBUG

PROG = quimica 
OBJS = quimica.o knapsack.o contador.o

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

