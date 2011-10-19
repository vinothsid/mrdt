#include "rdt.h"
 
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
	    if (!(strcmp((receiver+x)->ip,s.ip))&&((receiver+x)->port==s.port)){
	        return x;		
	    } 
	    else {
		x=x+1;
		
	    }
	}
	printf("\nNo match in the ip array\n");
	return -1;
}
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
		(window + i)->data = (char *)malloc(sizeof(char) * (mss + 9));
		//assert((window + i)->data);
		strcpy((window + i)->data , "");
		(window + i)->seqNo = -1;
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
	printf("Total Window Size : %d\n",winSize);
	printf("Maximum Segment Size : %d\n",mss);
	for (i=0;i<winSize;i++ ) {
		printf(" %d -> ",(window+i)->seqNo);
		if( *((window + i )->data) == '\0' )
			printf("0;");
		else
			printf("1");
	}
	printf("\n");
	return 1;

}

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
	printf("Entered rst\n");
	restartTimer=1;
}

int timeoutHandler(){
	printf("You are in timeout handler\n");
	runTimer=0;
}

void timeout(int ticks)
{
	clock_t endwait;
	endwait = clock() + ticks ;
	while(clock()< endwait){
	if(restartTimer==1){
	printf("Entered timeout rst\n");
	printf("Timer restarted\n");
	break;
	}
	}
	printf("the wait is over %ld \n",CLOCKS_PER_SEC);


}

void startTimer(){
	runTimer=1;

}

void *timer()
{
	int n;
	n=0;
	
	restartTimer=0;
	while(1){
		while(runTimer==1)
		{

			printf("Timer Started for %d consecutive time\n",n);
			n++;
			timeout(500);
		

			if(restartTimer==0&&runTimer==1){
				timeoutHandler();
			}
	
		}
	}
	
}


/*************************************************/
/*===============================udpSend==========================*/
int udpSend(int indexWindow,int indexRcvr)
{
    assert(*(window+indexWindow)->data!='\0');
    
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

    if ((numbytes = sendto(sockfd, (window+indexWindow)->data, strlen((window+indexWindow)->data), 0,
             p->ai_addr, p->ai_addrlen)) == -1) {
        perror("sendto");
        exit(1);
    }

    freeaddrinfo(servinfo);
    //if (debug == 1 || log ==1) {
    	printf("sent %d bytes to %s\n", numbytes,(receiver+indexRcvr)->ip);
    //}
    close(sockfd);
    return 0;
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

int udpRcv(char* rcvBuf,int port)
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
        return 1;
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
        return 2;
    }

    freeaddrinfo(servinfo);

    //printf("listener: waiting to recvfrom...\n");

    addr_len = sizeof their_addr;
    	if ((numbytes = recvfrom(sockfd, buf, mss+8 , 0,
        	(struct sockaddr *)&their_addr, &addr_len)) == -1) {
        	perror("recvfrom");
        	exit(1);
    	}
	int rcvFromPort=ntohs(get_in_port((struct sockaddr*)&their_addr));
	//setting the values of source to getRcvIndex
	strcpy(source.ip,inet_ntop(their_addr.ss_family,get_in_addr((struct sockaddr *)&their_addr),s, sizeof s));
	source.port=rcvFromPort;

    	printf("listener: got packet from %s port %d\n",
        	inet_ntop(their_addr.ss_family,get_in_addr((struct sockaddr *)&their_addr),s, sizeof s),rcvFromPort);
    	printf("listener: packet is %d bytes long\n", numbytes);
    	buf[numbytes] = '\0';
    	printf("listener: packet contains \"%s\"\n", buf);
	strcpy(rcvBuf,buf);

    close(sockfd);

    return getRecvIndex(source);
}

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

int framePacket(char *data,uint32_t seqNo,char *pkt) {

//First 32 bits are sequence number
	packi32(pkt,seqNo);

	uint16_t checkSum= computeChkSum(data);

	packi16(pkt+4,checkSum);
	uint16_t dataFlag = 21845; //0101010101010101
	packi16(pkt+6,dataFlag);


	memcpy(pkt+8,data,strlen(data) );
	return 0;
}

struct token tokenize(char *pkt) {
	struct token t;
	t.seqNo = unpacki32(pkt);
	t.chkSum = unpacki16(pkt+4);
	return t; 

}

int checkChkSum(u_short *buf,u_short checksum)
{
        register u_long sum = 0;

        int count = ceil(strlen(buf)/2.0 ) ;
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

u_short computeChkSum(u_short *buf)
{
        int count = ceil(strlen(buf)/2.0 ) ;
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

