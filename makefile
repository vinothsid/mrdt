
all:
	gcc -g -pg rdt.c -lpthread -c -o rdt.o -lm -lc
	gcc -g -pg -o client client.c -lpthread rdt.o -lm -lc  
	gcc -g -pg -o server server.c -lpthread rdt.o -lm -lc
#	gcc -g -pg -o udpServer udpServer.c rdt.o -lm -lc
#	gcc -g -pg -o udpClient udpClient.c rdt.o -lm -lc
	 
clean:
	rm *.o client server
