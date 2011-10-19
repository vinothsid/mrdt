#include "rdt.h"
#define num 5
#define MYPORT 4950
#define winSize 16
#define sizeofAck 32
/*int ceil(float x) {
    if (((int)x % 2)==0){
        return x;
    }
    else {
        return x+1;
    }
}*/
//Function to obtain the minimum of an array
//might return -1 due to initial conditions calling function should remember
//to ignore -1
int minAcked(struct server* receiver) {
    int min=receiver->highSeqAcked;
    printf("(receiver+x)->highSeqAcked: %d", receiver->highSeqAcked);
    int x=0;
    while (x<num) {
        if ((receiver+x)->highSeqAcked < min) {
	    min=(receiver+x)->highSeqAcked;
	}	   
	x++; 
    }
    return min;
}

recvThread() {
    char *rcvBuf=(char *)malloc(sizeofAck*sizeof(char));
    int recvIndex;
    recvIndex=udpRcv(rcvBuf,MYPORT);
    //get the seqNo of the ack
    struct token t;
    t=tokenize(rcvBuf);
    //t.seqNo now contains the sequence number of the Ack received
    int start=((receiver+recvIndex)->highSeqAcked)%winSize;  

    if (t.seqNo < (receiver+recvIndex)->highSeqAcked) {
        //do nothing
        //ack is less than the already highest acked
    } 
    else if(t.seqNo==(receiver+recvIndex)->highSeqAcked) {
        //duplicate ack
        (window+start)->Ack[recvIndex]++;
        //put fast retransmit conditions and code here
    }
    else {
        int x;
        //expected ack received
        x=(start+1) % winSize;
        while((window+x)->seqNo!=t.seqNo){
            (window+x)->Ack[recvIndex]=1;
       	    x=(x+1)%winSize;
	}
        //need to do once outside the loop
        (window+x)->Ack[recvIndex]=1;
        (receiver+recvIndex)->highSeqAcked=(window+x)->seqNo;
    }
   //head update procedure
   int HU, HU_M;
   HU=minAcked(receiver);
   HU_M=HU%winSize;
   if ((HU_M!=-1)&&(HU_M!=head)) {
       head=HU_M;
       //put start_timer() code here
   }
}

int main(){
    receiver=(struct server*)malloc(5*sizeof(struct server));
    int a[5]={3,4,2,9,5};
    int x=0;
    while(x < num) {
        (receiver+x)->highSeqAcked=a[x];
	x++;
    }
    int minm;
    minm=minAcked(receiver);
    float f=1.6;
    printf("\nThe minimum value obtained is: %d\n",minm);
    x=ceil(3.25);
    printf("The ceil of 3.0/2.0 is:%f\n",ceil(1.4));
}
