
all:
	gcc -g -o rdt.o rdt.c -lpthread -c -w -lm
	gcc -g -o client client.c -lpthread rdt.o -w -lm 
	gcc -g -o server server.c -lpthread rdt.o -w -lm
	gcc -g -o udpServer udpServer.c -lpthread rdt.o -w -lm
	gcc -g -o udpClient udpClient.c -lpthread rdt.o -w -lm
	 
clean:
	rm *.o client server
