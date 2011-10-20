#include "rdt.h"

int main(char *argv[],int argc) {
	int port=5000;
	char *fileName="test";
	int windowSize=10;
	int maxSegSize=1500;
	numServers=1;
	
	initWindow(windowSize,maxSegSize);

	printWindowInfo();
	rdtRecv(port,fileName);
}
