main: server.o app.o function.o exception.o
	gcc -o server server.o app.o function.o exception.o

server.o:
	gcc -c server.c

app.o:
	gcc -c app/app.c

function.o:
	gcc -c function/function.c
	
exception.o:
	gcc -c exception/exception.c

clean:
	rm -f *.o *~ server