
all:
	gcc -g rdt.c -lpthread -c -o rdt.o -lm
	gcc -g -o client client.c -lpthread rdt.o -lm  
	gcc -g -o server server.c -lpthread rdt.o -lm
	gcc -g -o udpServer udpServer.c rdt.o -lm
	gcc -g -o udpClient udpClient.c rdt.o -lm
	 
clean:
	rm *.o client server
