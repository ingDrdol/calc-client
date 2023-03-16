CC=gcc
CFLAGS=-std=c99 -Wall -Wextra -pedantic -lm -fcommon
.PHONY=runudp runtcp clean valgrind

runudp: ipkcpc.out
	./ipkcpc.out -p 2023 -m udp -h localhost < inputudp

runtcp: ipkcpc.out
	./ipkcpc.out -p 2023 -m tcp -h localhost

valgrind: ipkcpc.out
	valgrind ./ipkcpc.out -p 2023 -m udp -h localhost

ipkcpc.out: ipkcpc.c
	$(CC) $(CFLAGS) -o ipkcpc.out ipkcpc.c 

clean:
	rm *.out *.o