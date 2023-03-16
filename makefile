CC=gcc
CFLAGS=-std=c99 -Wall -Wextra -pedantic -lm -fcommon
.PHONY=run clean valgrind

run: ipkcpc.out
	./ipkcpc.out -p 2023 -m udp -h localhost

valgrind: ipkcpc.out
	valgrind ./ipkcpc.out -p 2023 -m udp -h localhost

ipkcpc.out: ipkcpc.c
	$(CC) $(CFLAGS) -o ipkcpc.out ipkcpc.c 

clean:
	rm *.out *.o