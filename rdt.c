#include "rdt.h"
#include<stdio.h>
#include<time.h>
#include<sys/types.h>


/************************Timer Functions*************************/

void endTimer(){
	runTimer=0;
}

void resetTimer(){
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
	int i;
	i=0;
	
	restartTimer=0;
	while(1){
	while(runTimer==1)
	{

		printf("Timer Started for %d consecutive time\n",i);
		i++;
		timeout(500000);
		

		if(restartTimer==0&&runTimer==1){
			timeoutHandler();
		}

	}
	}
	
}

/*************************************************/
