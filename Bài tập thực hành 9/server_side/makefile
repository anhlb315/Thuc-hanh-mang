main: server.o server_app.o exception.o account.o
	gcc -o server server.o server_app.o account.o exception.o

server.o:
	gcc -c server.c

server_app.o:
	gcc -c server_app/server_app.c

account.o:
	gcc -c account/account.c

exception.o:
	gcc -c exception/exception.c

clean:
	rm -f *.o *~ server