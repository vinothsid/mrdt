#include <stdlib.h>
#include <stdio.h>

struct winElement {
	int seqNo;
	int *Ack;
	char *data;
} *window;

struct server {
	char *ip;
	int port;
	int highSeqAcked;
} *receiver;

int head, tail, inTransit;
int winSize, mss;

struct token{
	int seqNo;
	char chkSum[2];
};

int rdtSend(File *fp);
int initWindow();
int initReceivers();
char* framePacket(char *data,int seqNo);
char* computeChkSum(char* data);
int udpSendAll(int indexWindow);
int udpSend(int indexWindow, int indexRcvr);
int getRecvIndex(struct sockaddr_storage addr);
int timeoutHandler();
struct sockaddr_storage udp_rcv();
struct token tokenize(char* Pkt);
int chkMinOfHighSeqAcked();
int checkChkSum(char* data, char* chkSum);
int fillBuffer(char* pkt);
char* getData(char* pkt);
int writeToFile(File* fp, char* data);




