CC = gcc
CFLAGS  = -o

main: main.o linked_list.o
	$(CC) $(CFLAGS) main main.o linked_list.o

main.o: main.c
	$(CC) -c main.c

linked_list.o: linked_list.c
	$(CC) -c linked_list.c

clean:
	$(RM) main *.o *~