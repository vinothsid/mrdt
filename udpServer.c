#include "rdt.h"

/*#define MYPORT "4950"    // the port users will be connecting to

#define MAXBUFLEN 1024

// get sockaddr, IPv4 or IPv6:
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

int udpRcv(char* rcvBuf)
{
    int sockfd;
    struct addrinfo hints, *servinfo, *p;
    int rv;
    int numbytes;
    struct sockaddr_storage their_addr;
    char buf[MAXBUFLEN];
    socklen_t addr_len;
    char s[INET6_ADDRSTRLEN];

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC; // set to AF_INET to force IPv4
    hints.ai_socktype = SOCK_DGRAM;
    hints.ai_flags = AI_PASSIVE; // use my IP

    if ((rv = getaddrinfo(NULL, MYPORT, &hints, &servinfo)) != 0) {
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

    printf("listener: waiting to recvfrom...\n");

    addr_len = sizeof their_addr;
    //while(1) {
    	if ((numbytes = recvfrom(sockfd, buf, MAXBUFLEN-1 , 0,
        	(struct sockaddr *)&their_addr, &addr_len)) == -1) {
        	perror("recvfrom");
        	exit(1);
    	}
	int rcvFromPort=ntohs(get_in_port((struct sockaddr*)&their_addr));

    	printf("listener: got packet from %s port %d\n",
        	inet_ntop(their_addr.ss_family,get_in_addr((struct sockaddr *)&their_addr),s, sizeof s),rcvFromPort);
    	printf("listener: packet is %d bytes long\n", numbytes);
    	buf[numbytes] = '\0';
    	printf("listener: packet contains \"%s\"\n", buf);
	strcpy(rcvBuf,buf);
    //}

    close(sockfd);

    return 0;
}*/

int main() {
    char* rcvBuf;
    rcvBuf=(char*)malloc(1024*sizeof(char));
    mss=1024;
    //if there is a problem its likely that mss=1024 is not enf..increase it..
    //this problem will effect only this function..main function has no problem.   //as it needs to be passed rcvBuf of size mss+ headerlength..
   //so increase mss...
    //udpRcv takes rcvBuf to pass on the value of rcvdData that needs to be mall    //oced outside
   //second argument is the port number where the receiver listens
    //udpRcv will return the index of the receiver in the receiver element array
   //catch it in an int variable to know which receiver has responded...
    udpRcv(rcvBuf,4950);	
    printf("Received data of size %d\n data is\n%s\n",strlen(rcvBuf),rcvBuf);
}


