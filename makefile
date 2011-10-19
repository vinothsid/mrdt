
all:
	gcc -g rdt.c -lm -lpthread -c -o rdt.o
	gcc -g -lm -o client client.c -lpthread rdt.o 
	gcc -g -lm -o server server.c -lpthread rdt.o
	 
clean:
	rm *.o client server
