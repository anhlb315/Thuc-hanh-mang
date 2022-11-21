main: server.c client.c exception/exception.o
	gcc -o server server.c exception/exception.o
	gcc -o client client.c exception/exception.o
exception.o: exception/exception.c
	gcc -c exception.c
clean:
	rm -f *.o *~
