CC=gcc
CFLAGS=-O0 -g -pthread
LDFLAGS=-pthread

all:	tabort.o
	$(CC) $(LDFLAGS) -o tabort tabort.o

clean:	
	rm -fr *.o tabort
