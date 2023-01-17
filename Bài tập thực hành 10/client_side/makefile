main: client.o app.o function.o
	gcc -o client client.o app.o function.o

client.o:
	gcc -c client.c

app.o:
	gcc -c app/app.c

function.o:
	gcc -c function/function.c

clean:
	rm -f *.o *~ client
