#include "TCPIP Stack/TCPIP.h"
#include <string.h>
#include <plib.h>
#include "main.h"
#include "PWMbyButtons.h"
#include  <math.h>

static int PWMDC_OC2;
static int PWMDC_OC3;
static int PWMDC_OC4;

static int SW1isOn;
static int SW2isOn;
static int SW3isOn;

void computePWMDCbyButtons(void)
{
    if(!BUTTON0_IO)
    {
        if(!SW1isOn)
        {
            SW1isOn = 1;
        }
        if(SW1isOn == 1)
        {
            if(PWMDC_OC2>0)         //if OC2 is active, turn it off
            {
                PWMDC_OC2=0;
            }
            else                    //if it's off, turn it to max duty
            {
                PWMDC_OC2=MAX_PWM;
            }
        SW1isOn++;
        }
    }
    else
    {
        SW1isOn = 0;
    }

    if(!BUTTON1_IO)
    {
        if(!SW2isOn)            //if the button is ON for the first time after it was OFF, do a PWMDC allocation
        {
            SW2isOn = 1;
            }
            if(SW2isOn == 1)        //if the button is ON for more than one time after it was OFF, do nothing
            {
                if(PWMDC_OC3>0)         //if OC3 is active, turn it off
                {
                    PWMDC_OC3=0;
                }
                else                    //if it's off, turn it to max duty
                {
                    PWMDC_OC3=MAX_PWM;
                }
            SW2isOn++;
        }
    }
    else
    {
        SW2isOn = 0;
    }

    if(!BUTTON2_IO)
    {
        if(!SW3isOn)
        {
            SW3isOn = 1;
        }
        if(SW3isOn == 1)
        {
            if(PWMDC_OC4>0)         //if OC2 is active, turn it off
            {
                PWMDC_OC4=0;
            }
        else                    //if it's switched off, turn it to max duty
        {
            PWMDC_OC4=MAX_PWM;
        }
        SW3isOn++;
        }
    }
    else
    {
        SW3isOn = 0;
    }
}

void updatePWMDCbyButtons(void)
{
    SetDCOC2PWM(PWMDC_OC2);
    SetDCOC3PWM(PWMDC_OC3);
    SetDCOC4PWM(PWMDC_OC4);
}