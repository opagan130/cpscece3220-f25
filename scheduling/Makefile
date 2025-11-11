
CC=clang
CFLAGS=-Wall -g

SCHEDULERS=fcfs.o sjf.o wyw.o rr.o priority.o
BINS=nice_example sim affinity nice_example
all: $(BINS)

%.o: %.c %.h
	$(CC) $(CFLAGS) -c $<

nice_example: nice_example.c
	$(CC) $(CFLAGS) -o $@ $<

sim: simmain.c schedsim.o $(SCHEDULERS)
	$(CC) $(CFLAGS) -o sim simmain.c *.o

clean:
	rm -rf $(BINS) $(SCHEDULERS) *.o *.dSYM

