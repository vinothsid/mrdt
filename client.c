#include "rdt.h"


int main(char *argv[],int argc) {
pthread_t timerThread;
printf("The RDT sender is now starting for the first time...\n");
pthread_create(&timerThread,NULL,&timer,NULL);
startTimer();
pthread_join(timerThread,NULL);

}
