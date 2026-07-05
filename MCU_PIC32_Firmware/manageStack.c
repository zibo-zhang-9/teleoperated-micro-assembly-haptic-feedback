#include "TCPIP Stack/TCPIP.h"
#include <string.h>
#include <plib.h>
#include "main.h"
#include "manageStack.h"
#include  <math.h>


static BOOL stackEnable = TRUE;

//------------------------------------------------------------------------------
//StackTension values, read by PWM ISR and turned into duty cycle before updating PWM module
//------------------------------------------------------------------------------
static double StackTension_goal;        //where we want to go
static double StackTension_current;     //where we are
static double StackdTension;            //the speed at wich one we want the tension to vary in V/s

//------------------------------------------------------------------------------
//Stack limitations
//------------------------------------------------------------------------------
const static double maxStackTension = 100;        //100% = theorically 150V (see data sheet)
const static double maxStackPosition = 100;   //100% = theorically 40µm
const static double maxStackForce = 100;          //100% = theorically 3000mN if blocked-blocked
const static double maxStackdTension = 100;        //100%V in 1 s, to be easily seen (slew rate at about 65ms)
const static double maxStackdPosition = 100;       //100%µm in 1 s
const static double maxStackdForce = 100;          //100%mN in 1 s

static double StackSpeed = 100;

//------------------------------------------------------------------------------
//Stack PWM duty cycle to be computed from one ISR to the next one
//------------------------------------------------------------------------------
static int Stack_PWMDC;
static int nbrOfPWMInterrupts;
static int StackPWMComputingsDone;

//------------------------------------------------------------------------------
//stack SPI packet, formated from one ISR for the nextone
//------------------------------------------------------------------------------
static int StackSpiPacketToSend;

//------------------------------------------------------------------------------------------------------------------------
// MAIN FUNCTIONS
//------------------------------------------------------------------------------------------------------------------------
void manageStack(char** arg,int argc)
{
    if (argc == 2)
    {
        if (!strcmp("POS",arg[0]))
        {
            cancelStackSpeedConsign();
            double position = atof(arg[1]);                         //retrieve order value (% here)
            reinjectStackValue(&position,0,maxStackPosition);       //modify it if it's unreachable
            computeStackTension(position, 0);                       //determine the related tension, stored in StackTension_goal (see manageStack.h)
        }
        else if (!strcmp("dPOS",arg[0]))
        {
            cancelStackPositionConsign();
            double dPosition = atof(arg[1]);        //%/s
            reinjectStackValue(&dPosition,-maxStackdPosition,maxStackdPosition);
            computeStackdTension(dPosition, 0);            
        }
    }
	else if (argc == 2)
    {
        if (!strcmp("SPEED",arg[0]))           //change the speed bridle value
        {
            double tempsSpeed = atof(arg[1]);
            if(tempsSpeed > maxStackdTension){ tempsSpeed = maxStackdTension; }
			else if(tempsSpeed < 0){ tempsSpeed = 0; }
            StackSpeed = tempsSpeed;
        }        
    }
    else if (argc == 1)
    {
        if (!strcmp("STATE",arg[0]))
        {
            char msg[30]="";
            sprintf(msg,"S:%2.1f\n", StackTension_current);
//            ServerSend(msg);
        }
        else if (!strcmp("ENABLE", arg[0])) stackEnable = TRUE;
        else if (!strcmp("DISABLE", arg[0])) stackEnable = FALSE;
    }
}

void reinjectStackValue(double * value, double minLimit, double maxLimit)           //useless hence the ampli is fully adapted to the stack so no damage can occure
{                                                       //can become usefull if we change for a weaker stack with the same ampli
    if (*value > maxLimit){*value = maxLimit;}
    if (*value < minLimit){*value = minLimit;}
}


//if both displacement and force asked, only force considered (initial main purpose of the stack)
double computeStackTension(double position, double force)
{
    double a = maxStackTension*position/maxStackPosition;
    if(!force)
    {
        StackTension_goal = maxStackTension*position/maxStackPosition;          //computing tension (0-150V) from wanted displacement (0-40µm) (40µm reachable only if free-free)
    }
    else
    {
        StackTension_goal = maxStackTension*force/maxStackForce;                //computing tension (0-150V) from wanted force (0-3000mN) (2.6N min 4N max from datasheet)
    }
}

//if both displacement and force asked, only force considered (initial main purpose of the stack)
double computeStackdTension(double dPosition, double dForce)
{
    double b = maxStackdTension*dPosition/maxStackdPosition;
    if(!dForce)
        StackdTension = maxStackdTension*dPosition/maxStackdPosition;
    else
        StackdTension = maxStackdTension*dForce/maxStackdForce;
}

//------------------------------------------------------------------------------------------------------------------------
// ISR FUNCTIONS
//------------------------------------------------------------------------------------------------------------------------
void StackPWMDCComputings()      //using OC3
{
    if ( stackEnable )
    {
        computeStackNextTension();              //updates the current tension consign, moving it one step closer to the goal (from position command and/or speed command)
        Stack_PWMDC = formatStackPWM();         //determine the PWM duty cycle
    }
}

void StackSpiComputings()
{
    computeStackNextTension();              //move current tension one step closer to the goal tension
    formatStackSpiPacket();                 //take the current tension and create related SPI packet
}

void sendStackPWMDC()
{
    if(!StackPWMComputingsDone)     //if computings are not done, increment the number of interrupts occured without computings
    {
        nbrOfPWMInterrupts++;
    }
    else                            //if computings are done, rest the number of interrupt to 1, and call for new computings by clearing StackPWMComputingsDone
    {
        nbrOfPWMInterrupts=1;
        StackPWMComputingsDone=0;
    }

    SetDCOC3PWM(Stack_PWMDC);               //updates module3 dutyCycle
}

void sendStackSPIPackets(SpiChannel chn, int wordsNbr)
{
    //SpiChnChangeMode(chn, 1, 0, 0);                 // set master mode, seems to be bugging in framed mode ?
                                                      // why is this function even exists ?
    //LATBbits.LATB11 = 0;
    //force channel to be channel2, to protect actually working SPI on channel1
    SpiChnPutS(SPI_CHANNEL2, &StackSpiPacketToSend, wordsNbr);           // write 8bits words through channel chn
    //LATBbits.LATB11 = 1;    
}


void computeStackNextTension()            //adjust the current stack tension one step closer to the tension goal
{
    //if severals PWM interrupts are done before an entire computing process.
    double TensionCurrent_chunk = 0;
    double TensionGoal_chunk = 0;    

    if(!StackPWMComputingsDone)     //do not compute more than one time per PWM interrupt
    {
        //do stuff only if the goal has not been already reached
        if( StackTension_current != StackTension_goal )
        {
            //determine it's sign
            if(StackTension_current < StackTension_goal)        //current < goal
            {
                TensionCurrent_chunk = StackSpeed/FPWM;
            }
			else												//current > goal
			{
				TensionCurrent_chunk = - StackSpeed/FPWM;
			}

            //compute chunk of the number of steps to do
            int multFactor = nbrOfPWMInterrupts;
            TensionCurrent_chunk*=multFactor;
            
            //if going > max
            if(StackTension_current + TensionCurrent_chunk > maxStackTension )
            {
                StackTension_current = maxStackTension;
            }
            //if going < min
            else if(StackTension_current + TensionCurrent_chunk < 0 )
            {
                StackTension_current = 0;
            }
            //if close ehough to the goal
            else if (fabs(StackTension_current - StackTension_goal) < TensionCurrent_chunk )
            {
                StackTension_current = StackTension_goal;   //if less than one max step is available, move the entire remaining distance
            }
            //else, move tension
            else
            {
                StackTension_current += TensionCurrent_chunk;
            }
        }
        //move the goal one step (speed control), only if speed is not 0 (not sure if a negative value is TRUE...)
        else if(StackdTension!=0)
        {
            int multFactor = nbrOfPWMInterrupts;
            TensionGoal_chunk = StackdTension/FPWM*multFactor;
            if(StackTension_goal + TensionGoal_chunk > maxStackTension )
            {
                StackTension_goal = maxStackTension;
            }
            else if(StackTension_goal + TensionGoal_chunk < 0 )
            {
                StackTension_goal = 0;
            }
            else                      //if current tension + multFactor steps don't go out of reach
            {
                StackTension_goal += TensionGoal_chunk;
            }
        }
        StackPWMComputingsDone = 1;
    }
}

unsigned int formatStackPWM()
{
    return (int)(( (MAX_PWM)/maxStackTension)*StackTension_current);
}

void formatStackSpiPacket()     ///not used, and not wired in the actual (07/04/2013) version of the electronic
{
    InitStackDataBanks();
    StackSpiPacketToSend |= ((0b0100)<<11);         //control bits 12,13,14,15 left shifted (<<11) to be MSB
                                                    //sending to DAC A, reference buffered ? mode 00 ?
    StackSpiPacketToSend |= ((int)(4095*StackTension_current/maxStackTension));      //bits 0 to 11: 1111 1111 1111 (4095) <=> 100%V
}

void cancelStackPositionConsign()        //called if a speed consign is set
{
    StackTension_goal = StackTension_current;
}

void cancelStackSpeedConsign()           //called if a position consign is set
{
    StackdTension = 0;
}

void InitStackDataBanks()
{
    StackSpiPacketToSend=0x00;
    Stack_PWMDC=0;
}

