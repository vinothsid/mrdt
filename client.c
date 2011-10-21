#include "rdt.h"


void *senderFunction(char *fileName){
	rdpSend(fileName);
//	while(1){} // for testing only
	}


void *recvFunction(){
	recvThread();
//	while(1){} // for testing only

	}



int main(int argc,char *argv[]) {
	 pthread_t timerThread, senderThread, receiverThread;
	int nReceivers,size,mss;
	int iret1,iret2,iret3;

	//strcpy();	

	nReceivers = (argc -3 )/2;
	initReceivers(argv,nReceivers);
	size = atoi( argv[argc-2] );	
	mss = atoi( argv[ argc-1 ] );	
	initWindow(size,mss);
	printWindowInfo();
	printReceiverList();
	

	iret1 = pthread_create( &senderThread, NULL, senderFunction,(void *) argv[argc-3]);
        iret2 = pthread_create( &receiverThread, NULL, recvFunction, NULL);
	iret3 = pthread_create( &timerThread, NULL, timer, NULL);
	 
	     pthread_join( timerThread, NULL);
	     pthread_join( receiverThread, NULL); 
	     pthread_join( timerThread, NULL); 
	
	

}


