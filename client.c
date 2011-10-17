#include "rdt.h"

int main(char *argv[],int argc) {
	int nReceivers;
	nReceivers = argc/2 -2;
	initReceivers(argv,nReceivers);
	initWindow();
	printWindowInfo();
	printReceiverList();
}
