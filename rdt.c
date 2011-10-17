#include "rdt.h"

//initReceivers should be called before initWindow , since numServers is set there only.
int initWindow() {
	int i=0,j=0;
	window = (struct winElement *)malloc(sizeof(struct winElement) * winSize);
	for(i=0;i<winSize;i++) {
		(window + i)->Ack = (int *)malloc(sizeof(int) * numServers);
		assert((window + i)->Ack);
		// data size = mss + 8 bytes of header + 1 for storing \0
		(window + i)->data = (char *)malloc(sizeof(char) * (mss + 9));
		assert((window + i)->data);
		strcpy((window + i)->data , "");
		seqNo = -1;
		for( j=0;j<numServers;j++) {
			(window + i)->Ack[j] = 0;
		}
		
	}
	return 1;
}


int initReceivers(char **receivers,int numReceivers) {
	numServers = numReceivers;
	int i=1,receiverIndex;
	receiver = (struct server*)malloc(sizeof(struct server) * numServers);
	for (i=1 , receiverIndex=0 ;
	     i < 2*numReceivers && receiverIndex < numReceivers ;
	     i=i+2,receiverIndex++  ) {

		strcpy((receiver + receiverIndex)->ip,receivers[i]);
		(receiver + receiverIndex)->port = atoi(receivers[i+1]);
		(receiver + receiverIndex)->highSeqAcked = -1;
		
	}
	return 1;
}

int printWindowInfo() {
	int i;
	for (i=0;i<winSize;i++ ) {
		printf(" %d - ",(window+i)->seqNo);
		if( *((window + i )->data) == '\0' )
			printf("0;");
		else
			printf("1");
	}
	printf("\n");

}

int printReceiverList() {
	int i;
	for (i=0;i<numServers;i++) {
		printf("%d - IP: %s ; Port: %d\n",i+1,(receiver + i)->ip , (receiver + i)->port);
	}
}
