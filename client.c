#include "rdt.h"


int main(int argc,char *argv[]) {

	int nReceivers,size,mss;

	nReceivers = (argc -3 )/2;
	initReceivers(argv,nReceivers);
	size = atoi( argv[argc-2] );	
	mss = atoi( argv[ argc-1 ] );	
	initWindow(size,mss);
	printWindowInfo();
	printReceiverList();
	

}
