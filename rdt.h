//#include <tgmath.h>
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
#include <stdint.h>
#include <inttypes.h>
#define u_short uint16_t
#define u_long uint32_t
#define MYPORT 4950
#define sizeofack 32

int restartTimer;
int runTimer;
int segmentNumber;
int totalSegments;
uint32_t sequenceNumber;

struct winElement {
	int seqNo;
	int *Ack; // pointer because it has to carry a count of ack's from each of the receivers. SHOULD WE NOT MALLOC IT FOR 'R' RCVRS ?
	char *data;
} *window;

struct server {
	char ip[46];
	int port;
	int highSeqAcked;
} *receiver;


struct token{
	int32_t seqNo;
	uint16_t chkSum;
};

int head, tail, inTransit;
int winSize, mss;
int numServers;
int nextExpectedSeqNo;

int rdpSend(char *fileName);
int initWindow(int size,int segSize);
int initReceivers(char **receivers,int numReceivers);
int framePacket(char *data,uint32_t seqNo,char *pkt,int flag);
int udpSendAll(int indexWindow);
int udpSend(int indexWindow, int indexRcvr);
struct server udpRcv(char* rcvBuf, int port,int bufSize);
int getRecvIndex(struct server addr);
int timeoutHandler();
struct sockaddr_storage udp_rcv();
struct token tokenize(char* pkt);
int chkMinOfHighSeqAcked();
int fillBuffer(char* pkt);
char* getData(char* pkt);
int writeToFile(FILE* fp, char* data);
int printWindowInfo();
int printReceiverList();
void endTimer();
void resetTimer();

int rdtRecv(int port,char *fileName);
//some auxilary functions
int get_in_port(struct sockaddr *sa);
void *get_in_addr(struct sockaddr *sa);
char *itoa(int num);

void timeout(int ticks);
void startTimer();
void *timer();
u_short computeChkSum (u_short *buf);
int checkChkSum(u_short *buf,u_short checksum); 
int printInTransitWindowInfo();


