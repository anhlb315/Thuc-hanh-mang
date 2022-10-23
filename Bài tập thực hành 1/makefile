CC = gcc
CFLAGS  = -o

main: main.o account.o
	$(CC) $(CFLAGS) main main.o account.o

main.o: main.c
	$(CC) -c main.c

account.o: account.c
	$(CC) -c account.c

clean:
	$(RM) main *.o *~