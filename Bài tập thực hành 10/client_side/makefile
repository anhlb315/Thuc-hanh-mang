main: client.o app.o
	gcc -o client client.o app.o

client.o:
	gcc -c client.c

app.o:
	gcc -c app/app.c

clean:
	rm -f *.o *~ client
