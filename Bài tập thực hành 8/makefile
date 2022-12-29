main: server.o client.o
	gcc -o server server.o
	gcc -o client client.o

server.o:
	gcc -c server_side/server.c

client.o:
	gcc -c client_side/client.c

clean:
	rm -f *.o *~ server client