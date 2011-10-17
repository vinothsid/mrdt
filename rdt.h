#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <string.h>
#include <sys/wait.h>
#include <signal.h>
#include <netinet/in.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>
#include <sys/time.h>
#include <assert.h>

int restartTimer;
int runTimer;


struct winElement {
	int seqNo;
	int *Ack;
	char *data;
} *window;

struct server {
	char ip[46];
	int port;
	int highSeqAcked;
} *receiver;

int head, tail, inTransit;
int winSize, mss;
int numServers;

struct token{
	int seqNo;
	char chkSum[2];
};

int rdtSend(FILE *fp);
int initWindow(int size,int segSize);
int initReceivers(char **receivers,int numReceivers);
char* framePacket(char *data,int seqNo);
char* computeChkSum(char* data);
int udpSendAll(int indexWindow);
int udpSend(int indexWindow, int indexRcvr);
int udpRcv(char* rcvBuf, int port);
int getRecvIndex(struct server addr);
int timeoutHandler();
struct sockaddr_storage udp_rcv();
struct token tokenize(char* Pkt);
int chkMinOfHighSeqAcked();
int checkChkSum(char* data, char* chkSum);
int fillBuffer(char* pkt);
char* getData(char* pkt);
int writeToFile(FILE* fp, char* data);
int printWindowInfo();
int printReceiverList();
int writeToFile(FILE *fp, char* data);
void endTimer();
void resetTimer();

//some auxilary functions
int get_in_port(struct sockaddr *sa);
void *get_in_addr(struct sockaddr *sa);
char *itoa(int num);

void timeout(int ticks);
void startTimer();
void *timer();
 



