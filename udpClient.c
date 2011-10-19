#include "rdt.h"
/*#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

//#define SERVERPORT "4950"    // the port users will be connecting to
*/
struct node {
	char ip[INET6_ADDRSTRLEN];
        char* port;
};
/*
struct winElement {
        int seqNo;
        int *Ack;
        char *data;
} *window;

struct server {
        char ip[INET6_ADDRSTRLEN];
        int port;
        int highSeqAcked;
} *receiver;

char *itoa(int num) {
        char *str;
        str = (char *)malloc(5);
        sprintf(str,"%d",num);
        return str;
}


int udpSend(int indexWindow,int indexRcvr)
{
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

    printf("sent %d bytes to %s\n", numbytes,(receiver+indexRcvr)->ip);
    close(sockfd);

    return 0;
}*/

int main() {
	
	window=(struct winElement*)malloc(2*sizeof(struct winElement));
	receiver=(struct server*)malloc(2*sizeof(struct server));
	char* buf=(char*)malloc(100*sizeof(char));
	//memset(buf,'a',1023*sizeof(char));
	//buf[1023]='\0';
	//printf(" msg : %s ",buf);
	strcpy(buf,"many happy returns of");
	//int len=strlen(buf);
	//buf[len]='\0';
	//printf("\nbuflen:%d\n",len);
	(window+1)->data=(char*)malloc(sizeof(char)*1024);	
	//strcpy((window+1)->data,buf);
	memset((window+1)->data,'a',1023);
	printf("\n(window+1)->data:%s\n",(window+1)->data);
	printf("\nstrlen((window+1)->data):%d\n",strlen((window+1)->data));
	struct node n;
	strcpy(n.ip,"127.0.0.1");
	strcpy((receiver+2)->ip,n.ip);
	n.port=(char*)malloc(10*sizeof(char));
	strcpy(n.port,"4950");
	(receiver+2)->port=atoi(n.port);
	mss = 100;
	udpSend(1,2);
	free((window+1)->data);
	free(window);
}



