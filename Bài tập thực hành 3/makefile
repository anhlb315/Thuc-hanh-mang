CC = gcc
CFLAGS  = -o

main: main.o account.o resolver.o
	$(CC) $(CFLAGS) main main.o account.o resolver.o

main.o: main.c
	$(CC) -c main.c

account.o: account.c
	$(CC) -c account.c

resolver.o: resolver.c
	$(CC) -c resolver.c

clean:
	$(RM) main *.o *~