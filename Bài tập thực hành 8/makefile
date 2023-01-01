main: server.o client.o client_app.o exception.o
	gcc -o server server.o
	gcc -o client client.o client_app.o exception.o

server.o:
	gcc -c server_side/server.c

client.o:
	gcc -c client_side/client.c

client_app.o:
	gcc -c client_side/client_app/client_app.c

exception.o:
	gcc -c client_side/exception/exception.c

clean:
	rm -f *.o *~ server client