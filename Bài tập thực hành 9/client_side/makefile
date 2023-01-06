main: client.o client_app.o exception.o
	gcc -o client client.o client_app.o exception.o

client.o:
	gcc -c client.c

client_app.o:
	gcc -c client_app/client_app.c

exception.o:
	gcc -c exception/exception.c

clean:
	rm -f *.o *~ client