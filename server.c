#include "rdt.h"

int main(int argc,char *argv[]) {
	if(argc!=5){
		printf("argc is %d\n",argc);
		printf("usage: ./server [filename] [winSize] [mss] [loss probability]\n");
		exit(1);
	}
	sizeExtracted=0;
	printf("Server starting ....\n");
	float loss;
	int size;
	int mss;
	loss= atof(argv[4]);
	printf("Loss : %f\n",loss);
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
	rdtRecv(port,fileName,mss);
}
