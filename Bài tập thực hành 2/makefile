CC = gcc
CFLAGS  = -o

main: resolver.o
	$(CC) $(CFLAGS) resolver resolver.o

resolver.o: resolver.c
	$(CC) -c resolver.c

clean:
	$(RM) main *.o *~