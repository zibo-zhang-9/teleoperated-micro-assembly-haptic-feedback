#include "TCPIP Stack/TCPIP.h"
#include <string.h>
#include <plib.h>
#include "main.h"
#include "manageHeatResistors.h"
#include  <math.h>


typedef enum
{
    LEFT,
    RIGHT,
    BOTH
}Resistor ;

double maxHeat = 100;

int rightHeat_PWM = 0;
int leftHeat_PWM = 0;

void manageHeatResistors( char** arg,int argc )
{
    if (argc == 2)
    {
        if (!strcmp("RIGHT",arg[0]))
        {
            double Heat = atof(arg[1]);
            rightHeat_PWM = computeHeatPWM(Heat);;                    //see main.h for MAX_PWM, we actually use the same interrupt as the STACK
        }
        if (!strcmp("LEFT",arg[0]))
        {
            double Heat = atof(arg[1]);
            leftHeat_PWM = computeHeatPWM(Heat);
        }
        if (!strcmp("BOTH",arg[0]))
        {
            double Heat = atof(arg[1]);
            rightHeat_PWM = computeHeatPWM(Heat);
            leftHeat_PWM = computeHeatPWM(Heat);
        }
    }
    if (argc == 1)
    {
        if (!strcmp("STATE",arg[0]))
        {
            char msg[30]="hey\n";
            sprintf(msg,"%2.1f:%2.1f\n",rightHeat_PWM/MAX_PWM*maxHeat,leftHeat_PWM/MAX_PWM*maxHeat );
//            ServerSend(msg);
        }
    }
}

int computeHeatPWM( double heat )
{
    return ((MAX_PWM)/maxHeat)*heat;
}

void sendHeatResistorsPWMDC()
{
    SetDCOC4PWM(rightHeat_PWM);
    SetDCOC2PWM(leftHeat_PWM);
}

