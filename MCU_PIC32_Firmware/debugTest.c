#include  <stdio.h>

//test/debug variables
static unsigned int actualTime = 0;
static unsigned int meanTime = 0;
static unsigned int minTime = 50000000;
static unsigned int maxTime = 0;
static unsigned int loopsNbr = 0; // /!\ WARNING BUG
static unsigned int lastTime = 0;


void testDebug_TimerCompute( void )
{
        actualTime = getElapsedTimeInMicroSecond(lastTime);
        loopsNbr++;
        if(actualTime<minTime){minTime=actualTime;}                    //if the current time is shorter, it's the new min
        else if(actualTime>maxTime){maxTime=actualTime;}               //if it's longer, it's the new max
        meanTime=((loopsNbr-1)*meanTime+actualTime)/(loopsNbr);             //take the sum of previous times ( (loopsNbr-1)*meanTime  )
                                                                            //add the current time to it
                                                                            //then divide by the total to get the new mean
       lastTime = ReadCoreTimer();
}

void testDebug_SendTimeFlow()
{
    char msg[100];
    //sprintf(msg,"TIME:min=%u Max=%u mean=%u ns\n", minTime, maxTime, meanTime);
    sprintf(msg,"TIME:%u:%u:%u\n", meanTime, actualTime, ReadCoreTimer());
//    ServerSend(msg);
}