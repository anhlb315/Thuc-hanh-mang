main: server.o client.o exception.o
	gcc -o server server.o exception.o
	gcc -o client client.o exception.o

server.o:
	gcc -c server.c

client.o:
	gcc -c client.c

exception.o:
	gcc -c exception.c

clean:
	rm -f *.o *~ server client
