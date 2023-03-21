CC=gcc
CFLAGS=-std=c99 -Wall -Wextra -pedantic -lm -fcommon
.PHONY=all clean 

all: ipkcpc

ipkcpc: ipkcpc.c
	$(CC) $(CFLAGS) -o ipkcpc ipkcpc.c 

clean:
	rm ipkcpc *.o