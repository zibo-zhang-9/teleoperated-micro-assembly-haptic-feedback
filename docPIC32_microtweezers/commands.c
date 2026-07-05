#include "TCPIP Stack/TCPIP.h"
#include <string.h>
#include <p32xxxx.h>
#include "main.h"
#include "commands.h"
#include  <math.h>

//correspondance DAC/register value for the AD5724R
const short DAC_A = 0b000;
const short DAC_B = 0b001;
const short DAC_C = 0b010;
const short DAC_D = 0b011;
const short ALL_DAC = 0b100;

//------------------------------------------------------------------------------
//managing state of leds (actually, only LED1)
//------------------------------------------------------------------------------
void manageLed(unsigned led, char** arg)
{
    printf("%d, %d\n", led, LED1_IO);
        if (!strcmp((char*)"ON",arg[0]))
            LED1_IO = 1;
        else if (!strcmp((char*)"OFF",arg[0]))
            LED1_IO = 0;
        else if (!strcmp((char*)"SWITCH",arg[0]))
            LED1_IO ^= 1;
 //       else if (!strcmp((char*)"STATE",arg[0]))
 //           if (LED1_IO)
//                ServerSend("LED1 IS ON\n");
//            else
            //    ServerSend("LED1 IS OFF\n");
}


//------------------------------------------------------------------------------
//controlling V1/V2 from Vy/Vz, staying in a VY_MAX/10-VY_MAX sized rectangle
//------------------------------------------------------------------------------
void ControlV12_rectangle(double Vy,double Vz,double* V1,double* V2)
{
    //réinjection dans le rectangle
    if(Vy>VY_MAX){	Vy= VY_MAX;}
    if(Vy<-VY_MAX){	Vy=-VY_MAX;}
    if(Vz>VZ_MAX){	Vz= VZ_MAX;}
    if(Vz<-VZ_MAX){	Vz=-VZ_MAX;}
    //calcul des V1 et V2
    *V1 = Vz-Vy;
    *V2 = Vz+Vy;
}

//------------------------------------------------------------------------------
//controlling V1/V2 from Vy/Vz, with Vy dominating Vz
//------------------------------------------------------------------------------
void ControlV12_Ydominant(double Vy,double Vz,double* V1,double* V2)
{
    if((fabs(Vy)+fabs(Vz))>10)
    {
        if(Vy>10){Vy= 10;}
        if(Vy<10){Vy=-10;}
        if(Vz>0){ Vz= 10-fabs(Vy);}
        if(Vz<0){ Vz=-10+fabs(Vy);}
    }
    //calcul des V1 et V2
    *V1 = Vz-Vy;
    *V2 = Vz+Vy;
}

//------------------------------------------------------------------------------
//sleep, doesn't seem to work
//------------------------------------------------------------------------------
void sleep(char** arg,int argc)
{
    if (argc == 1)
    {
        int delay = atoi(arg[1]);
        int i=0;
        while(i<1000*delay){i++;}
    }
}


//------------------------------------------------------------------------------
//Managing one finger, right or left with 'ControlV12_rectangle' function
//------------------------------------------------------------------------------
/*
void manageXFinger(char* finger, char** arg,int argc)
{
    if( (!strcmp("r",finger))||(!strcmp("right",finger))||(!strcmp("RIGHT",finger)) )
    {
        //printf("argc=%d \n",argc);
        if (argc == 3)
        {
            if (!strcmp("POS",arg[0]))
            {
                //printf("in manageRFinger:POS \n");
                RFinger_Vy = atof (arg[1]);
                RFinger_Vz = atof (arg[2]);

                //reinjection
                ControlV12_rectangle(RFinger_Vy,RFinger_Vz,&RFinger_V1,&RFinger_V2);

                //PWM
                PWM_RFINGER_V1=(int)(( (MAX_PWM) /10)*RFinger_V1); //0-10V vers 0-FPB/FPWM
                PWM_RFINGER_V2=(int)(( (MAX_PWM) /10)*RFinger_V2); //0-10V vers 0-FPB/FPWM

                //SPI not usable for the moment, need DAC
                //SpiDoDataTxAD(SPI_CHANNEL1, RFinger_V1, DAC_A);
                //SpiDoDataTxAD(SPI_CHANNEL1, RFinger_V2, DAC_B);
            }
        }
        else if (argc == 1)
        {
            if (!strcmp("STATE",arg[0]))
            {
                //printf("in manageRFinger:STATE \n");
                char msg[100]="test";
                sprintf(msg,"RF V1=%2.1f V2=%2.1f \n",RFinger_V1,RFinger_V2);
                //printf("RF V1=%2.1f V2=%2.1f \n",RFinger_V1,RFinger_V2);
                ServerSend(msg);
            }
        }
    }
    else if( (!strcmp("l",finger))||(!strcmp("left",finger))||(!strcmp("LEFT",finger)) )
    {
        if (argc == 3)
        {
            if (!strcmp("POS",arg[0]))
            {
                //printf("in manageLFinger:POS \n");
                LFinger_Vy = atof (arg[1]);
                LFinger_Vz = atof (arg[2]);

                ControlV12_rectangle(LFinger_Vy,LFinger_Vz,&LFinger_V1,&LFinger_V2);

                //PWM
                PWM_LFINGER_V1=(int)(( (MAX_PWM) /10)*LFinger_V1); //0-10V vers 0-FPB/FPWM
                PWM_LFINGER_V2=(int)(( (MAX_PWM) /10)*LFinger_V2); //0-10V vers 0-FPB/FPWM

                //SPI not usable for the moment, need DAC
                //SpiDoDataTxAD(SPI_CHANNEL1, LFinger_V1, DAC_C);
                //SpiDoDataTxAD(SPI_CHANNEL1, LFinger_V2, DAC_D);

            }
        }
        else if (argc == 1)
        {
            if (!strcmp("STATE",arg[0]))
            {
                //printf("in manageLFinger:STATE \n");
                char msg[100]="";
                sprintf(msg,"LF V1=%2.1f V2=%2.1f \n",LFinger_V1,LFinger_V2);
                ServerSend(msg);
            }
        }
    }
}

//------------------------------------------------------------------------------
//controlling both fingers with 'ControlV12_rectangle' functions
//------------------------------------------------------------------------------
void manageGripper(char** arg,int argc)
{
    if (argc == 3)
    {
        if (!strcmp("SPACING",arg[0]))
        {
            //printf("in manageGripper:SPACING \n");
            RFinger_Vy= ( atof (arg[1]) )/2;
            RFinger_Vz= ( atof (arg[2]) )/2;

            //reinjection
            ControlV12_rectangle(RFinger_Vy,RFinger_Vz,&RFinger_V1,&RFinger_V2);
            ControlV12_rectangle(-RFinger_Vy,-RFinger_Vz,&LFinger_V1,&LFinger_V2);

            //PWM
            PWM_RFINGER_V1=(int)(( (MAX_PWM) /10)*RFinger_V1); //0-10V vers 0-FPB/FPWM
            PWM_RFINGER_V2=(int)(( (MAX_PWM) /10)*RFinger_V2); //0-10V vers 0-FPB/FPWM
            PWM_LFINGER_V1=(int)(( (MAX_PWM) /10)*LFinger_V1); //0-10V vers 0-FPB/FPWM
            PWM_LFINGER_V2=(int)(( (MAX_PWM) /10)*LFinger_V2); //0-10V vers 0-FPB/FPWM

            //SPI not usable for the moment, need DAC
            //SpiDoDataTxAD(SPI_CHANNEL1, RFinger_V1, DAC_A);
            //SpiDoDataTxAD(SPI_CHANNEL1, RFinger_V2, DAC_B);
            //SpiDoDataTxAD(SPI_CHANNEL1, LFinger_V1, DAC_C);
            //SpiDoDataTxAD(SPI_CHANNEL1, LFinger_V2, DAC_D);

        }
    }
    else if (argc == 1)
    {
        if (!strcmp("STATE",arg[0]))
        {
            //printf("in manageGripper:STATE \n");
            float   RVy= (RFinger_V2-RFinger_V1)/2;     //recalcul des tensions
            float   RVz= (RFinger_V2+RFinger_V1)/2;
            float   LVy= (LFinger_V2-LFinger_V1)/2;
            float   LVz= (LFinger_V2+LFinger_V1)/2;
            char msg[100]="test";
            sprintf(msg,"DVy=%2.1f DVz=%2.1f \n",RVy-LVy,RVz-LVz);
            ServerSend(msg);
        }
    }
}*/