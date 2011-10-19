
all:
	gcc -g -lm -o rdt.o rdt.c -lpthread -c -w
	gcc -g -lm -o client client.c -lpthread rdt.o -w 
	gcc -g -lm -o server server.c -lpthread rdt.o -w
	gcc -g -lm -o udpServer udpServer.c -lpthread rdt.o -w
	gcc -g -lm -o udpClient udpClient.c -lpthread rdt.o -w
	 
clean:
	rm *.o client server
