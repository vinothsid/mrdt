all:
	gcc -g rdt.c  -lpthread -c -o rdt.o
	gcc -g  -o client client.c -lpthread rdt.o 
	gcc -g  -o server server.c -lpthread rdt.o
         
clean:
	rm *.o client server

