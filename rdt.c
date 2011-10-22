#include "rdt.h"
 
int ceil(float x) {
    
}




/******************************RDP SEND**************************************/

int rdpSend(char *fileName){

	FILE *fp;
	fp = fopen(fileName,"r");
	if(fp==NULL){fputs("File error",stderr);exit(1);}
		
	long length;
	char *toSend; 
	size_t sizeExtracted; 
	char *packet;
	int segmentsToSend;	

	fseek(fp,0,SEEK_END);
	
	length=ftell(fp);
	printf("file size in int is %d",length);
	rewind(fp);
	
	if(length%mss==0){
	totalSegments = length/mss;
	}else{
	totalSegments = (length -(length%mss))/mss;
	totalSegments++;
	}

	segmentsToSend = totalSegments;	
	
	inTransit=0;
	sequenceNumber = 0;
//Vinoth why below stat is needed
//	(window+winSize-1)->seqNo = 0;

	toSend = (char *) malloc((sizeof(char))* mss );
	packet = (char *) malloc((sizeof(char))* (mss+9) );
	startTimer();
	while( segmentsToSend !=0){
		//consider making all bytes of toSend NULL coz for the last segment, overlapping of data may occur
		//if u get arbid end data at the receiver...I will suggest doing it
		memset(toSend,'\0',mss);
		sizeExtracted = fread(toSend,1,mss,fp);
		if(sizeExtracted ==0 ) {
			printf("either not working or finshed\n");
			break;
		}
		printf("%d\t",sizeExtracted);
		printf("%s\t",toSend);
		//commenting following lines //don't understand why?!
		//if(sequenceNumber!=0){
		//	sequenceNumber++;
		//}
		//inTransit++;	
    		memset(packet,'\0',mss+9);
		framePacket(toSend,sequenceNumber,packet,0);
		while(inTransit==winSize){ }

		tail= (tail+1) % winSize;
		memcpy((window+tail)->data,packet,mss+9);
		//printf("\npacket is %s\n",packet+8);
		printf("\ndata is %s\n",((window+tail)->data)+8);
		/*if(tail==0){
			
			(window+tail)->seqNo=(window+winSize-1)->seqNo + 1;
		}else{
			(window+tail)->seqNo=((window+((tail-1)%winSize))->seqNo )+1;
			}*/
		(window+tail)->seqNo = sequenceNumber;
		if(udpSendAll(tail)!=1)
			printf("Some error while sending to all\n");		

		//sleep(1); 
		// PART pending......
		// Will resume after udpSendAll
		inTransit++;//incrementing only after sending packet
		sequenceNumber++;
		if (inTransit==1) {
			resetTimer(); //timer has to be restarted...has to be done
			printf("Timer Started\n");
		}
		printf("\nfrom the rdpSend\t");
		printInTransitWindowInfo();	
		segmentsToSend--;
		sleep(1);
	}

	fclose(fp);
	return 1;
//	while(1) {}

}





/***************************************************** I to A********************************/

char *itoa(int num) {
        char *str;
        str = (char *)malloc(5);
        sprintf(str,"%d",num);
        return str;
}

//getRecvIndex takes struct server s argument and returns the index of this receiver
//in the receiver array
//needs numServers to be correct for proper functioning
int getRecvIndex (struct server s) {
	int x=0;
	while((x<numServers)) {
//	    if (!(strcmp((receiver+x)->ip,s.ip))&&((receiver+x)->port==s.port)){
	    if (!(strcmp((receiver+x)->ip,s.ip))) {
	        return x;		
	    } 
	    else {
		x=x+1;
		
	    }
	}
	printf("\nNo match in the ip array\n");
	return -1;
}

/****************************INIT WINDOW*********************************************/

//initReceivers should be called before initWindow , since numServers is set there only.
int initWindow(int size,int segSize) {
	int i=0,j=0;
	winSize=size;
	mss = segSize;
	window = (struct winElement *)malloc(sizeof(struct winElement) * winSize);
	for(i=0;i<winSize;i++) {
		(window + i)->Ack = (int *)malloc(sizeof(int) * numServers);
		//assert((window + i)->Ack);
		// data size = mss + 8 bytes of header + 1 for storing \0
		(window + i)->data = (char *)malloc(sizeof(char) * (mss + 9));  // WHY MSS  '+9'  ????
		//assert((window + i)->data);
		strcpy((window + i)->data , ""); // WHY START WITH EMPTY DATA ????
		(window + i)->seqNo = -1;  
		for( j=0;j<numServers;j++) {
			(window + i)->Ack[j] = 0;
		}
		
	}
	head=0;  // I have added this as intial condition
	tail=-1;  // I have added this as intial condition
	return 1;
}

/*********************************INIT RECEIVERS**********************************/

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

/************************Print window*********************************************/

int printWindowInfo() {
	int i;
	printf("Total Window Size : %d\n",winSize);
	printf("Maximum Segment Size : %d\n",mss);
	for (i=0;i<winSize;i++ ) {
		printf(" %d -> ",(window+i)->seqNo);
		if( *((window + i )->data) == '\0' )
			printf("0;");
		else
			printf("1");  // CAN WE PRINT DATA ?? can it happen that the window ele that we are printing here might be wiped off b4 ?
	}
	printf("\n");
	return 1;

}

/************************Print InTransit window******************************************/

int printInTransitWindowInfo() {
        int i;
        printf("Total Window Size : %d\n",winSize);
        printf("Maximum Segment Size : %d\n",mss);
        printf("In Transit window size : %d\n",inTransit);
        for (i=head;i<=tail;i++ ) {
                printf(" %d -> ",(window+i)->seqNo);
                if( *((window + i )->data) == '\0' )
                        printf("0;");
                else
                        printf("1");  // CAN WE PRINT DATA ?? can it happen that the window ele that we are printing here might be wiped off b4 ?
        }
        printf("\n");
        return 1;

}

/**************************Print Receiver List***************************/

int printReceiverList() {
	int i;
	printf("Receivers:\n");
	for (i=0;i<numServers;i++) {
		printf("%d - IP: %s ; Port: %d\n",i+1,(receiver + i)->ip , (receiver + i)->port);
	}
	printf("====================");
	return 1;
}

/************************Timer Functions*************************/


void endTimer(){
	runTimer=0;
}

void resetTimer(){
	//printf("Entered rst\n");
	restartTimer=1;
}

int timeoutHandler(){
	printf("You are in timeout handler\n");
	//runTimer=0;
	int i;
	int check;
	for(i=0;i<numServers;i++){
		printf("window+head -> Ack[i] : %d\n",(window+head)->Ack[i]);
		printf("window+head -> seqNo : %d \n", (window+head)->seqNo);
		if(((window+head)->Ack[i]==0) && ((window+head)->seqNo==HP+1)){
		// send to the i th receiver only not to all
				
			check = udpSend(head,i);
			resetTimer();
			//sleep(1);
			if(check!=1){
				printf("\nUnable to send packet %d to receiver%d from timeout handler\n",head,i);}
		}
	}
	resetTimer();
}

void timeout(int ticks)
{
	clock_t endwait;
	endwait = clock() + ticks ;
	while(clock()< endwait){
		if(restartTimer==1){
			//printf("Entered timeout rst\n");
			//printf("Timer restarted\n");
		break;
		}
	}
	//printf("the wait is over %ld \n",CLOCKS_PER_SEC);
}

void startTimer(){
	runTimer=1;
	printf("Started TIMER again\n");
	resetTimer();	

}

void *timer()
{
	int n;
	n=0;
	
	//restartTimer=0;
	while(1){
		//restartTimer=0;
		while(runTimer==1)
		{       
			/*if ((totalSegments==0)&&(minAcked(receiver))) {
				endTimer();
			}*/
			restartTimer=0;
			//printf("Timer Started for %d consecutive time\n",n);
			n++;
			timeout(5000000);
		
			if((runTimer==1)&&(restartTimer==0)) {
			//if(restartTimer==0&&runTimer==1){
				timeoutHandler();
			}
	
		}
	}
	
}


/*************************************************/
/**************UDP SEND ALL**************************/
int udpSendAll(int indexWindow){
	int i;
	int check;
	for(i=0;i<numServers;i++){
		check = udpSend(indexWindow,i);
		if(check!=1){
		printf("\nUnable to send packet %d to receiver%d\n",indexWindow,i);
		}
	}
	return 1;
}

/***************************************************/
/*===============================udpSend==========================*/
int udpSend(int indexWindow,int indexRcvr)
{
    //assert(*(window+indexWindow)->data!='\0');
    
    int sockfd;
    struct addrinfo hints, *servinfo, *p;
    int rv;
    int numbytes;

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_DGRAM;

    if ((rv = getaddrinfo((receiver+indexRcvr)->ip, itoa((receiver+indexRcvr)->port), &hints, &servinfo)) != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
        return 1;
    }

    // loop through all the results and make a socket
    for(p = servinfo; p != NULL; p = p->ai_next) {
        if ((sockfd = socket(p->ai_family, p->ai_socktype,
                p->ai_protocol)) == -1) {
          perror("socket");
            continue;
        }

        break;
    }

    if (p == NULL) {
        fprintf(stderr, "failed to bind socket\n");
        return 2;
    }

    if ((numbytes = sendto(sockfd, (window+indexWindow)->data, mss+9, 0,
             p->ai_addr, p->ai_addrlen)) == -1) {
        perror("sendto");
        exit(1);
    }

    freeaddrinfo(servinfo);
    //if (debug == 1 || log ==1) {
    	printf("sent %d bytes to %s\n", numbytes,(receiver+indexRcvr)->ip);
    //}
    close(sockfd);
    return 1;
}

/*================================udpRcv==================================*/
void *get_in_addr(struct sockaddr *sa)
{
    if (sa->sa_family == AF_INET) {
        return &(((struct sockaddr_in*)sa)->sin_addr);
    }

    return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

int get_in_port(struct sockaddr *sa)
{
    if (sa->sa_family == AF_INET) {
        return ((struct sockaddr_in*)sa)->sin_port;
    }
}

struct server udpRcv(char* rcvBuf,int port,int bufSize)
{
    struct server source;
    int sockfd;
    struct addrinfo hints, *servinfo, *p;
    int rv;
    int numbytes;
    struct sockaddr_storage their_addr;
    char buf[mss];
    socklen_t addr_len;
    char s[INET6_ADDRSTRLEN];

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC; // set to AF_INET to force IPv4
    hints.ai_socktype = SOCK_DGRAM;
    hints.ai_flags = AI_PASSIVE; // use my IP

    if ((rv = getaddrinfo(NULL, itoa(port), &hints, &servinfo)) != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
        exit(1);
    }

    // loop through all the results and bind to the first we can
    for(p = servinfo; p != NULL; p = p->ai_next) {
        if ((sockfd = socket(p->ai_family, p->ai_socktype,
                p->ai_protocol)) == -1) {
            perror("listener: socket");
            continue;
        }

        if (bind(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
            close(sockfd);
            perror("listener: bind");
            continue;
        }

        break;
    }

    if (p == NULL) {
        fprintf(stderr, "listener: failed to bind socket\n");
        //return NULL;
    }

    freeaddrinfo(servinfo);

    //printf("listener: waiting to recvfrom...\n");

    addr_len = sizeof their_addr;
    	if ((numbytes = recvfrom(sockfd, buf, mss+8 , 0,
        	(struct sockaddr *)&their_addr, &addr_len)) == -1) {
        	perror("recvfrom");
        	//exit(1);
 		//return NULL;
    	}
	int rcvFromPort=ntohs(get_in_port((struct sockaddr*)&their_addr));
	//setting the values of source to getRcvIndex
	strcpy(source.ip,inet_ntop(their_addr.ss_family,get_in_addr((struct sockaddr *)&their_addr),s, sizeof s));
	source.port=rcvFromPort;

    	printf("listener: got packet from %s port %d\n",
        	inet_ntop(their_addr.ss_family,get_in_addr((struct sockaddr *)&their_addr),s, sizeof s),rcvFromPort);
    	printf("listener: packet is %d bytes long\n", numbytes);
    	buf[numbytes] = '\0';
    	printf("listener: packet contains \"%s\"\n", buf+8);
	memcpy(rcvBuf,buf,bufSize);

    close(sockfd);

    return source;
}

/**************************************************************************************/

void packi16(unsigned char *buf, uint16_t i)
{
    *buf++ = i>>8; *buf++ = i;
}

/*
** packi32() -- store a 32-bit int into a char buffer (like htonl())
*/ 
void packi32(unsigned char *buf, uint32_t i)
{
    *buf++ = i>>24; *buf++ = i>>16;
    *buf++ = i>>8;  *buf++ = i;
}

/*
** unpacki16() -- unpack a 16-bit int from a char buffer (like ntohs())
*/ 
unsigned int unpacki16(unsigned char *buf)
{
    return (buf[0]<<8) | buf[1];
}

/*
** unpacki32() -- unpack a 32-bit int from a char buffer (like ntohl())
*/ 
unsigned long unpacki32(unsigned char *buf)
{
    return (buf[0]<<24) | (buf[1]<<16) | (buf[2]<<8) | buf[3];
}

/******************************FRAME PACKET****************************************/

// flag = 0 for data ; flag = 1 for ack
int framePacket(char *data,uint32_t seqNo,char *pkt,int flag) {

//First 32 bits are sequence number
	packi32(pkt,seqNo);


	uint16_t dataFlag ;
	if(flag == 0 ) {

		uint16_t checkSum= computeChkSum(data);

		packi16(pkt+4,checkSum);
		dataFlag = 21845; //0101010101010101
	} else {
		dataFlag= 43690; //1010101010101010
		//Setting checksum = 0000000000000000 for ack
		*(pkt+4)=0;
		*(pkt+5)=0;	
	}

	packi16(pkt+6,dataFlag);


	memcpy(pkt+8,data,strlen(data) );
	
	return 0;
}

/****************************TOKENIZE********************************/

struct token tokenize(char *pkt) {
	struct token t;
	t.seqNo = unpacki32(pkt);
	t.chkSum = unpacki16(pkt+4);
	return t; 

}

/****************************CHECK SUM***********************************/

int checkChkSum(u_short *buf,u_short checksum)
{
        register u_long sum = 0;

        int count;
 	count = ceil(strlen(buf)/2.0 ) ;
        while (count--)
        {
                sum += *buf++;
                //printf("\nthis time sum was %x\n",sum);
                if (sum & 0xFFFF0000)
                {

                        /* carry occurred, so wrap around */
                        sum &= 0xFFFF;
                        sum++;
                }
        }
	sum +=checksum;
   	if(sum & 0xFFFF0000)
                {

                        /* carry occurred, so wrap around */
                        sum &= 0xFFFF;
                        sum++;
                }

	 if((short int)~sum ==0) 
		return 1;

	return 0;
}

/***************************COMPUTE CHECK SUM************************************/

u_short computeChkSum(u_short *buf)
{
        int count;
	count = ceil(strlen(buf)/2.0 ) ;
        printf("count : %d",count);
        register u_long sum = 0;

        while (count--)
        {
                sum += *buf++;
                //printf("\nthis time sum was %x\n",sum);
                if (sum & 0xFFFF0000)
                {
                        //printf("I am inside\n");
                        /* carry occurred, so wrap around */
                        sum &= 0xFFFF;
                        sum++;
                }
        }

	return ~(sum & 0xFFFF);
}
/*************************************************************************************/

int rdtRecv( int port  , char *fileName) {
	int nE = 0,x=0,prev=0;
	int lastInSequenceNo=-1;
	int index;
	window->seqNo = 0;
	struct winElement *curWindow;
	char ackPkt[9];
	receiver = (struct server*)malloc(sizeof(struct server));
	struct server sender;
//	FILE *fp = fopen(fileName,"wa");
	char *temp = (char *)malloc(sizeof(char)*mss + 9);
	FILE *fp = fopen(fileName,"w");
	fclose(fp);
	int i=0;
	int test=1;
	curWindow = window+nE;
	while(1) {
		sender = udpRcv(temp,port,mss+9);
		printf("Received : %s\n",temp+8);
		strcpy(receiver->ip,sender.ip);
		receiver->port = MYPORT; //sender.port;
		struct token t;
		t = tokenize(temp);
	
		printf("Received SeqNo : %d , expectedSeqNo : %d\n",t.seqNo, curWindow->seqNo);	
		if ( (curWindow->seqNo - winSize ) <= t.seqNo && t.seqNo < curWindow->seqNo) {
			framePacket("",lastInSequenceNo,ackPkt,1);
			udpServerSend(ackPkt);
		} else if (t.seqNo == curWindow->seqNo )	{
			printf("11111111\n");
			//if ( checkChkSum(temp+8,t.chkSum) ) {
			if (lossFunction()) {
				printf("\nProbabilistic drop: packet seqNo: %d\n",t.seqNo);
				continue;
			}
			if ((test==1)&&(t.seqNo==20)) {
				test=0;
				printf("\n\n\n\nIntended los no 20\n\n\n\n");
				continue;
			}
			if(1) {
				x = nE;
				memcpy(curWindow->data,temp+8,mss );
				curWindow->seqNo = t.seqNo;
				
				printf("2222222222 curWindow->data : %s\n",curWindow->data);
				printf("are you here\n");
				while(*(curWindow->data)!='\0') {
				//while(i < 2) {
					//i++;
					//printf("while\n");
					printf("inside while : %s\n",(curWindow->data));
					fp = fopen(fileName,"a");
					fputs(curWindow->data,fp);
					fclose(fp);
					x = (x+1)%winSize;
					memset(curWindow->data,0,mss);
					lastInSequenceNo = curWindow->seqNo;
					curWindow = window+x;

				}
				nE = x;
				printf("Next expected : %d\n",nE);	
				printf("LastSequence Num : %d\n",lastInSequenceNo);
				curWindow->seqNo = lastInSequenceNo + 1;
				framePacket("",lastInSequenceNo,ackPkt,1);
				udpServerSend(ackPkt);
							
						
			} else {
				memset(curWindow->data,0,mss); // if checksum failed ,reset the data and continue the loop
				continue;
			}
			 
		} else if ( t.seqNo < curWindow->seqNo + winSize  ) {
			index = t.seqNo%winSize	;
			memcpy((window + index)->data,temp+8,mss);
			(window + index)->seqNo = t.seqNo;
		}

	}


}

int udpServerSend (char *pkt)
{
    //assert(*(window+indexWindow)->data!='\0');

    int sockfd;
    struct addrinfo hints, *servinfo, *p;
    int rv;
    int numbytes;
    int indexRcvr = 0;

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_DGRAM;

    if ((rv = getaddrinfo((receiver+indexRcvr)->ip, itoa((receiver+indexRcvr)->port), &hints, &servinfo)) != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
        return 1;
    }

    // loop through all the results and make a socket
    for(p = servinfo; p != NULL; p = p->ai_next) {
        if ((sockfd = socket(p->ai_family, p->ai_socktype,
                p->ai_protocol)) == -1) {
          perror("socket");
            continue;
        }

        break;
    }

    if (p == NULL) {
        fprintf(stderr, "failed to bind socket\n");
        return 2;
    }

    if ((numbytes = sendto(sockfd, pkt , 9, 0,
             p->ai_addr, p->ai_addrlen)) == -1) {
        perror("sendto");
        exit(1);
    }

    freeaddrinfo(servinfo);

    //if (debug == 1 || log ==1) {
        printf("sent %d bytes to %s\n", numbytes,(receiver+indexRcvr)->ip);
    //}
    close(sockfd);
    return 1;

}
int minAcked(struct server* receiver) {
    int min=receiver->highSeqAcked;
    printf("(receiver+x)->highSeqAcked: %d", receiver->highSeqAcked);
    int x=0;
    while (x<numServers) {
        if ((receiver+x)->highSeqAcked < min) {
            min=(receiver+x)->highSeqAcked;
        }
        x++;
    }
    return min;
}

recvThread() {
    HP=-1;
    //int counter=0;
    while(1) {
    	headIncrement=0;
    	char *rcvBuf=(char *)malloc(sizeofack*sizeof(char));
    	int recvIndex;
    	//store previous head
    	recvIndex=getRecvIndex(udpRcv(rcvBuf,MYPORT,sizeofack));
	printf("Receiver index : %d\n",recvIndex);
    	//get the seqNo of the ack
    	struct token t;
    	t=tokenize(rcvBuf);

	printf("recvThread : Received ack for seqNo : %d\n",t.seqNo);
    	//t.seqNo now contains the sequence number of the Ack received
    	int start=((receiver+recvIndex)->highSeqAcked)%winSize;

    	if ( t.seqNo <  (receiver+recvIndex)->highSeqAcked) {
        	//do nothing
        	//ack is less than the already highest acked
        	headIncrement=0;
		
		printf(" 1st if\n");
    	}
    	else if(t.seqNo== (receiver+recvIndex)->highSeqAcked) {
		printf(" 2nd if\n");
        	//duplicate ack
        	(window+start)->Ack[recvIndex]++;
        	headIncrement=0;
		//headIncrement=t.seqNo;
		//while(1) {}
        	//put fast retransmit conditions and code here
        	if ((window+start)->Ack[recvIndex]>=3) {
            		int y;
            		y=(start+1)%winSize;
			printf("3 Duplicate ack Received.Trigger fast retrans for seqNo: %d ", (window+y)->seqNo );
            		udpSend(y,recvIndex);
			(window+start)->Ack[recvIndex]=0;

        	}
    	}
    	else {
        	int x;
        	//expected ack received
        	x=(start+1) % winSize;
		printf("Value of x : %d\n",x);
        	while((window+x)->seqNo!=t.seqNo){
            		(window+x)->Ack[recvIndex]=1;
            		x=(x+1)%winSize;
        	}
        	//need to do once outside the loop
        	(window+x)->Ack[recvIndex]=1;
        	(receiver+recvIndex)->highSeqAcked=(window+x)->seqNo;
        	headIncrement=1;
		printf("Received ack : %d\n",t.seqNo);
		//printInTransitWindow();
    	}
   	//head update procedure
	//HU head update
	//HU_M head update modulo
	//HP Head Previous seqNo //previous heads sequence no.
   	//int HU, HU_M;
   	HU=minAcked(receiver);
   	HU_M=(HU+1)%winSize;
   	if ((HU_M!=-1)&&((HU_M!=head)||(headIncrement==1))) {
		//code to set seqNo's to zero
		int n=((HP+1) % winSize);
		while((window+n)->seqNo != HU) {
			int i=0;
			if (n != -1) {	
				while(i < numServers) {
					(window+n)->Ack[i]=0;	
					i++;
				}
			}
			n=(n+1)% winSize;
		}
		//once outsied the loop
		int i=0;
                if (n != -1) {
                        while(i < numServers) {
                                (window+n)->Ack[i]=0;
                                i++;
                        }
                }
       		head=HU_M;
		resetTimer();
       		//inTransit=inTransit-((window+head)->seqNo -HP);
       		inTransit=inTransit-((HU-HP));
		//need to change below line //not correct
		//HP=(receiver+recvIndex)->highSeqAcked;
		HP=HU; //this maybe the correct line
		//if any problem try de-initializing Ack of winElement here
       		//put start_timer() code here
		printf("\nrecvThread: moving head : %d\n",HU_M);
		printInTransitWindowInfo();
		//resetTimer();
		//below is the place where timer should end
		//nead to replace 20 with the initial value of totalSegments
		if (HP==totalSegments - 1) { //Since seqNo starts from 0
			printf("All segments sent and acked.Stopping the timer.");
			endTimer();
			exit(1);
		}
   	} 
	free(rcvBuf);
    }
}

int lossFunction() {
	float x;
	srand(time(NULL));
	x=rand() % 1024;
	x=x/1024.0;
	printf("\nlossProbability number: %f\n",x);
	if (x < lossProbability) {
		return 1;
	} else {
	        return 0;
 	}
}
