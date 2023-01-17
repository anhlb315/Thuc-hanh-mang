main: server.o app.o function.o
	gcc -o server server.o app.o function.o

server.o:
	gcc -c server.c

app.o:
	gcc -c app/app.c

function.o:
	gcc -c function/function.c

clean:
	rm -f *.o *~ server