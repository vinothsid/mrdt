#include "rdt.h"

int main(int argc,char *argv[]) {
	if(argc!=5){
		printf("argc is %d\n",argc);
		printf("usage: ./server [filename] [winSize] [mss] [loss probability]\n");
		exit(1);
	}
	printf("Server starting ....\n");
	int loss;
	int size;
	int mss;
	loss= atoi(argv[4]);
	mss= atoi(argv[3]);
	size= atoi(argv[2]);
	
	lossProbability=loss;
	int port=5000;
	char *fileName;
	fileName=argv[1];
	int windowSize=size;
	int maxSegSize=mss;
	numServers=1;
	
	initWindow(windowSize,maxSegSize);

	printWindowInfo();
	rdtRecv(port,fileName);
}
