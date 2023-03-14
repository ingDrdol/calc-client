CC=gcc
CFLAGS=-std=c99 -Wall -Wextra -pedantic -lm -fcommon
.PHONY=run

run: ipkcpc
	./ipkcpc -p port -m mode -h host
ipkcpc: ipkcpc.c
	$(CC) $(CFLAGS) -o ipkcpc ipkcpc.c 