#include "TCPIP Stack/TCPIP.h"
#include <string.h>
#include <plib.h>
#include "main.h"
#include "manageGripper.h"
#include  <math.h>


static BOOL gripperEnable = TRUE;
//------------------------------------------------------------------------------
//arrays containing tensions to be sent to the piezo beams (right and left) electrodes
//------------------------------------------------------------------------------
/* U[0] = right V1;
 * U[1] = right V2;
 * U[2] = left V1;
 * U[3] = left V2;
 *
 * all tensions stored in two arrays, to insure permanent integrity of at least one of them
 */
//---   actual tensions     ---
static double Unow[4];

//---   tension to reach    ---
static double Ugoal[4];

// 0 1 Right finger
// 2 3 Left finger

//---   last tensions     ---
static int Ulast[4];

//---   tension variations  ---
static double Uspeed[4];
static int speed_flag;

//---       limitations     ---         //everything in % for now
static const double U_max = 100;            //max tension
static const double Uspeed_max = 0.1;       //absolute maximum variation, 100% in 1 mili second
static double Uspeed_bridleL = 0.000001;     //bridle set if we want to slow down motions (Uspeed_bridle always < Uspeed_max), % in 1 second (seems to be %/ns XD)
static double Uspeed_bridleR = 0.000001;     //bridle set if we want to slow down motions (Uspeed_bridle always < Uspeed_max), % in 1 second (seems to be %/ns XD)

//--  sinusoidal shape
static int sinus_flag = 0;
static double frequency;
static double amplitude;
static double sinusTime;    // Time value for sinus generation in seconds

//--        actual working mode of on board buttons (speed or pos)     ---
static int buttonOperatingMode;



//------------------------------------------------------------------------------
//gripper SPI packets, formated from one ISR for the next one
//------------------------------------------------------------------------------
static int GripperSpiPacketsToSend[4];
static int GripperSpiDATAPacketsRecieved[4];
static int GripperSpiCONFIGPacketsRecieved[4];

//------------------------------------------------------------------------------
//friendly names of certain input register bit values of the analog device AD5724R
//------------------------------------------------------------------------------
//REG names, bits 21 20 19
static const short DAC_reg = 0b000;
static const short OutputRangeSel_reg = 0b001;
static const short PowerContr_reg = 0b010;
static const short Control_reg = 0b011;
//DAC names, bits 18 17 16
static const short DAC_A = 0b000;
static const short DAC_B = 0b001;
static const short DAC_C = 0b010;
static const short DAC_D = 0b011;
static const short ALL_DAC = 0b100;


static unsigned int lastSentTickTime;
static double sentDelay;

static int leftResistorValue = 0;
static int rightResistorValue = 0;


//------------------------------------------------------------------------------
//friendly names for the analog device AD5724R pins (\LDAC and \CLR)
//------------------------------------------------------------------------------
#define AnalogDevice_nLDAC_TRIS			(TRISBbits.TRISB11)	//config port, set to 0 for output
#define AnalogDevice_nLDAC_IO                   (LATBbits.LATB11)       //when set low for at least 20ns, updates the DAC outputs
#define AnalogDevice_nCLR_TRIS                  (TRISBbits.TRISB12)     //config port, set to 0 for output
#define AnalogDevice_nCLR_IO                    (LATBbits.LATB12)       //active low, set DAC registers to default code (zero or mid, user selectable)

// PATCH 19/08 for new card
#define AnalogDevice_nSS_TRIS                   (TRISDbits.TRISD6)       //used as slave select
#define AnalogDevice_nSS_IO                     (LATDbits.LATD6)
//#define AnalogDevice_nSS_TRIS                   (TRISEbits.TRISE0)       //used as slave select, pin 12 J10
//#define AnalogDevice_nSS_IO                     (LATEbits.LATE0)

#define AnalogDevice_readCLK_TRIS               (TRISEbits.TRISE7)
#define AnalogDevice_readCLK_IO                 (LATEbits.LATE7)

// PATCH 19/08 for new card
#define AnalogDevice_CN_TRIS                    (TRISDbits.TRISD12)            //used to do an interrupt on real SYNC change
#define AnalogDevice_CN_IO                      (LATDbits.LATD12)              //corresponds
//#define AnalogDevice_CN_TRIS                    (TRISDbits.TRISD7)            //used to do an interrupt on real SYNC change
//#define AnalogDevice_CN_IO                      (LATDbits.LATD7)              //corresponds to J11 pin 11

#define LeftHeatingResistor_TRIS                (TRISGbits.TRISG14)
#define LeftHeatingResistor_IO                  (LATGbits.LATG14)

#define RightHeatingResistor_TRIS               (TRISGbits.TRISG13)
#define RightHeatingResistor_IO                 (LATGbits.LATG13)

//------------------------------------------------------------------------------------------------------------------------
// IN-MAIN FUNCTIONS (functions needing no clean time handling)
//------------------------------------------------------------------------------------------------------------------------
void manageGripper(char** arg,int argc)
{
    if (argc == 3)
    {
        if (!strcmp("RPOS",arg[0]))         //right finger
        {
            double Y = atof(arg[1]);
            double Z = atof(arg[2]);
            double Vy;
            double Vz;
            reinjectDiamond(&Y,&Z,U_max);
            computeCrossTensions(&Y,&Z);

            // Finger inversion DH 07/10/2014
            //speedsFromPositions(&Vy,&Vz,Y,Z,RIGHT);
            //AllocateConsigns(Y,Z,Vy,Vz,RIGHT);
            speedsFromPositions(&Vy,&Vz,Y,Z,LEFT);
            AllocateConsigns(Y,Z,Vy,Vz,LEFT);

            speed_flag = 0;
            sinus_flag = 0;
        }
        else if (!strcmp("LPOS",arg[0]))        //left finger
        {
            // Direction inversion DH 07/10/2014
            double Y = -atof(arg[1]);
            double Z = -atof(arg[2]);
            double Vy;
            double Vz;
            reinjectDiamond(&Y,&Z,U_max);
            computeCrossTensions(&Y,&Z);

            // Finger inversion DH 07/10/2014
            //speedsFromPositions(&Vy,&Vz,Y,Z,LEFT);
            //AllocateConsigns(Y,Z,Vy,Vz,LEFT);
            speedsFromPositions(&Vy,&Vz,Y,Z,RIGHT);
            AllocateConsigns(Y,Z,Vy,Vz,RIGHT);
            
            speed_flag = 0;
            sinus_flag = 0;
        }
    }
    else if (argc == 2)
    {
        if (!strcmp("LSPEED",arg[0]))           //change the speed bridle value
        {
            double bridle = atof(arg[1]);
            if(bridle > Uspeed_max){ bridle = Uspeed_max; }
			else if(bridle < 0){ bridle = 0; }
            Uspeed_bridleL = bridle;
            sinus_flag = 0;

            double Vy, Vz;
            speedsFromPositions(&Vy,&Vz,Ugoal[2],Ugoal[3],LEFT);
            AllocateConsigns(Ugoal[2],Ugoal[3],Vy,Vz,LEFT);
        }
        else if (!strcmp("RSPEED",arg[0]))           //change the speed bridle value
        {
            double bridle = atof(arg[1]);
            if(bridle > Uspeed_max){ bridle = Uspeed_max; }
            else if(bridle < 0){ bridle = 0; }
            Uspeed_bridleR = bridle;
            sinus_flag = 0;

            double Vy, Vz;
            speedsFromPositions(&Vy,&Vz,Ugoal[0],Ugoal[1],RIGHT);
            AllocateConsigns(Ugoal[0],Ugoal[1],Vy,Vz,RIGHT);
        }
        else if (!strcmp("SPEED",arg[0]))           //change the speed bridle value
        {
            double bridle = atof(arg[1]);
            if(bridle > Uspeed_max){ bridle = Uspeed_max; }
            else if(bridle < 0){ bridle = 0; }
            Uspeed_bridleL = bridle;
            Uspeed_bridleR = bridle;
            sinus_flag = 0;

            double Vy, Vz;
            speedsFromPositions(&Vy,&Vz,Ugoal[2],Ugoal[3],LEFT);
            AllocateConsigns(Ugoal[2],Ugoal[3],Vy,Vz,LEFT);

            double Vy2, Vz2;
            speedsFromPositions(&Vy2,&Vz2,Ugoal[0],Ugoal[1],RIGHT);
            AllocateConsigns(Ugoal[0],Ugoal[1],Vy2,Vz2,RIGHT);
        }
        if (!strcmp("LHRES",arg[0]))           //left heating resistor
        {
            leftResistorValue = atoi(arg[1]);
            SetDCOC2PWM((MAX_PWM*leftResistorValue)/100);
        }
        if (!strcmp("RHRES",arg[0]))           //right heating resistor
        {
            rightResistorValue = atoi(arg[1]);
            SetDCOC4PWM((MAX_PWM*rightResistorValue)/100);
        }
    }
    else if (argc == 1)
    {
        if (!strcmp("STATE",arg[0]))
        {
            char msg[30]="hey\n";
            sprintf(msg,"G:%2.1f:%2.1f:%2.1f:%2.1f:%d:%d\n",  (Unow[3] + Unow[2])/2, (Unow[3] - Unow[2])/2,     //LV1, LV2
                                                              -(Unow[1] + Unow[0])/2, -(Unow[1] - Unow[0])/2,     //RV1, RV2
                                                      leftResistorValue, rightResistorValue);        //Heating resistors
//            ServerSend(msg);
        }
        if (!strcmp("STATE2",arg[0]))
        {
            char msg[30]="hey\n";
            /*double currentSpeed[4];
            int i;
            for (i = 0; i < 4; i++) currentSpeed[i] = Uspeed[i];
            invertCrossTensions(&currentSpeed[0], &currentSpeed[1]);
            invertCrossTensions(&currentSpeed[2], &currentSpeed[3]);*/
            sprintf(msg,"G:%2.1f:%2.1f:%2.1f:%2.1f:%d:%d:%2.1f:%2.1f:%2.1f:%2.1f\n",  (Unow[3] + Unow[2])/2, (Unow[3] - Unow[2])/2,     //LV1, LV2
                                                              -(Unow[1] + Unow[0])/2, -(Unow[1] - Unow[0])/2,     //RV1, RV2
                                                      leftResistorValue, rightResistorValue, //Heating resistors
                                                      //currentSpeed[2] * 100000000, currentSpeed[3]* 100000000, currentSpeed[0]* 100000000, currentSpeed[1]* 100000000); // current speed * 100000000 to convert in % per second
                                                        (-(Uspeed[3]+Uspeed[2]))/2 * 100000000, (-(Uspeed[3]-Uspeed[2]))/2* 100000000, (-(Uspeed[1]+Uspeed[0]))/2* 100000000, (-(Uspeed[1]-Uspeed[0])/2)* 100000000); // current speed * 100000000 to convert in % per second
//            ServerSend(msg);
        }
        else if (!strcmp("ENABLE", arg[0])) gripperEnable = TRUE;
        else if (!strcmp("DISABLE", arg[0])) gripperEnable = FALSE;
    }
}



void resetGripper()
{
    int resetWord = 0b000110010000000000001110<<8;
    SpiChnPutS(SPI_CHANNEL1, &resetWord, 1);        //can't work with 3*8 bits words
}

//to-be-sent data banks need to be first voiden, or the first sent packet will be garbage.
//moreover, packets formating uses a bitwise trick which works by changing void arrays
void InitGripperDataBank( int i)
{
    GripperSpiPacketsToSend[i]=0x000000;
}

void InitGripperPINS( void )
{
    //OUTPUTS
    AnalogDevice_nLDAC_TRIS = 0;
    AnalogDevice_nCLR_TRIS = 0;
    AnalogDevice_nSS_TRIS = 0;

    //INPUTS
    AnalogDevice_readCLK_TRIS = 1;      //iput, we read clk from PORTE 07 to synchronise spi
    AnalogDevice_CN_TRIS = 1;           //input, to read real SYNC and to trigger an interrupt

    //OUTPUTS DEFAULT VALUES
    AnalogDevice_nLDAC_IO = 0;          //all active low
    AnalogDevice_nCLR_IO = 1;
    AnalogDevice_nSS_IO = 1;


    LeftHeatingResistor_TRIS = 0;
    RightHeatingResistor_TRIS = 0;

    LeftHeatingResistor_IO = 0;
    RightHeatingResistor_IO = 0;
}

void sendGripperInitSpiPackets(void)
{
    unsigned int initializingOutPutRange =  0b000011000000000000000100<<8;     //24bits message
    //00 (write) 001 (output range register) 100 (All DAC) 0000000000000 (don't care) 100 (+-10V) see p.27 pf datasheet
    unsigned int initializingPowerControl = 0b000100000000000000001111<<8;
    //00 (write) 010 (Power control register) 00000000000000 (don't care) 11111 (all 4 DAC and reference powered up) see p.29 pf datasheet
    unsigned int initializingControlReg = 0b000110010000000000001100<<8;

    SpiChnPutS(SPI_CHANNEL1, &initializingOutPutRange, 1);
       Delay10us(10);

    SpiChnPutS(SPI_CHANNEL1, &initializingPowerControl, 1);     //to initialise the ANALOG DEVICE DACs with 24bits messages
       Delay10us(10);

    SpiChnPutS(SPI_CHANNEL1, &initializingControlReg, 1);
       Delay10us(10);

    // Seems that the first init packet is not sent: repeated to be sure (DH 01/10/2014)
    SpiChnPutS(SPI_CHANNEL1, &initializingOutPutRange, 1);
       Delay10us(10);

    SpiChnPutS(SPI_CHANNEL1, &initializingPowerControl, 1);     //to initialise the ANALOG DEVICE DACs with 24bits messages
       Delay10us(10);

    SpiChnPutS(SPI_CHANNEL1, &initializingControlReg, 1);
       Delay10us(10);

}

void reinjectDiamond(double *val1, double *val2, double max)
{
    if((fabs(*val1)+fabs(*val2))> max )     //diamond max * max
    {
        if(*val2 > max)
            *val2 = max;
        else if(*val2 < - max)
            *val2 = -max;

        if(*val1 > 0)
            *val1 = max - fabs(*val2);
        else if(*val1 < 0)
            *val1 = - max + fabs(*val2);
    }
}

void computeCrossTensions(double *val1, double *val2)
{
    double temp = *val1;
    // Change of cross, due to different electronic behavior: Y and Z inverted (DH 01/10/2014)
    //*val1 = *val2 - *val1;      //V1 = Vz - Vy
    //*val2 = *val2 + temp;       //V2 = Vz + Vy
    *val1 = *val1 - *val2;      //V1 = Vy - Vz
    *val2 = temp + *val2;       //V2 = Vy + Vz
}

void invertCrossTensions(double *val1, double *val2)
{
    double z = -(*val1 - *val2)/2;
    double y = *val2 - z;

    *val1 = y;
    *val2 = z;
}

void speedsFromPositions( double *speed1, double *speed2, double pos1, double pos2, Finger finger )
{
    switch(finger)
    {
        case RIGHT:
        {
            double s1 = pos1-Unow[0];                      //speed vector not normed, component 1
            double s2 = pos2-Unow[1];                    //speed vector not normed, component 2
            double speedNorm = sqrt(s1*s1+s2*s2);               //speed vector norm

            if(speedNorm)   //normelizing speed vectors
            {
                *speed1 = Uspeed_bridleR * (s1/speedNorm);           //f <=> V1
                *speed2 = Uspeed_bridleR * (s2/speedNorm);           //f+1 <=> V2
            }
            else
            {
                *speed1 = 0;           //f <=> V1
                *speed2 = 0;           //f+1 <=> V2
            }
            break;
        }
        case LEFT:
        {

            double s1 = pos1-Unow[2];                      //speed vector not normed, component 1
            double s2 = pos2-Unow[3];                    //speed vector not normed, component 2
            double speedNorm = sqrt(s1*s1+s2*s2);               //speed vector norm

            if(speedNorm)   //normelizing speed vectors
            {
                *speed1 = Uspeed_bridleL * (s1/speedNorm);           //f <=> V1
                *speed2 = Uspeed_bridleL * (s2/speedNorm);           //f+1 <=> V2
            }
            else
            {
                *speed1 = 0;           //f <=> V1
                *speed2 = 0;           //f+1 <=> V2
            }
            break;      //f=2 (resp f+1=3) <=> left finger V1 (resp V2)
        }
    }
}

void AllocateConsigns( double pos1, double pos2, double speed1, double speed2, Finger finger )
{
    switch(finger)
    {
        case RIGHT:
        {
            //writte if the corresponding array boxes
            Ugoal[0] = pos1;
            Ugoal[1] = pos2;
            Uspeed[0] = speed1;
            Uspeed[1] = speed2;

            break;
        }
        case LEFT:
        {
            //writte if the corresponding array boxes
            Ugoal[2] = pos1;
            Ugoal[3] = pos2;
            Uspeed[2] = speed1;
            Uspeed[3] = speed2;

            break;      //f=2 (resp f+1=3) <=> left finger V1 (resp V2)
        }
        case BOTH:
        {
            //right finger
            Ugoal[0] = pos1;
            Ugoal[1] = pos2;
            Uspeed[0] = speed1;
            Uspeed[1] = speed2;
            //left finger
            Ugoal[2] = -pos1;
            Ugoal[3] = -pos2;
            Uspeed[2] = -speed1;
            Uspeed[3] = -speed2;

            break;
        }
    }
}

void updateGripper()
{
    if ( gripperEnable )
    {
        GripperSpiComputings();
        sendGripperSpiPackets();
    }
}

//------------------------------------------------------------------------------------------------------------------------
// IN-ISR FUNCTIONS (functions needing a clean time handling)
//------------------------------------------------------------------------------------------------------------------------
void GripperSpiComputings()
{    
    computeGripperNextTensions();              //move current tensions one step closer to the goal tensions
    formatGripperSpiPackets();                  //take the current tensions and create related SPI packets for the analog device DAC
}

void computeGripperNextTensions()
{
    //run through both fingers with 'f' index
    int f;
    for (f = 0; f < 4; f+=2)
    {
        double speed1 = Uspeed[f] * sentDelay;
        double speed2 = Uspeed[f+1] * sentDelay;

        //----------------------------------------------------------------------
        //successive if/else "drawers" to manage all the cases of speeds leads positions out of reach
        //run through all the drawers only if one of the tensions is going out of reach
        if(speed_flag)  //position alone can't go out of reach
        {
            if( fabs(Unow[f] +  speed1) > U_max     //if V1+dV1/dt out of reach
            ||  fabs(Unow[f+1] + speed2) > U_max  )  //or if V2+dV2/dt out of reach
            {                                                                //(which means we're out of the diamond in (Y,Z))
                //~~~~~~~~~~~~~~~~~~~~~~~~~~~~
                //if we're going beyond the top left border of the diamond
                if(Unow[f] + speed1 >= U_max)
                {
                    Unow[f] = U_max;
                    //and beyond the top right border of the diamond as the same time
                    if(Unow[f+1] + speed2 > U_max)
                    {
                        Unow[f+1] = U_max;
                    }
                    //and beyond the botom left border of the diamond as the same time
                    else if(Unow[f+1] + speed2 < -U_max)
                    {
                        Unow[f+1] = -U_max;
                    }
                    //but not beyond any other border
                    else
                    {
                        Unow[f+1] += speed2;
                    }
                }
                //~~~~~~~~~~~~~~~~~~~~~~~~~~~~
                //if we're going beyond the botom right border of the diamond
                else if(Unow[f] + speed1 <= -U_max)
                {
                    Unow[f] = -U_max;
                    //and beyond the top right border of the diamond as the same time
                    if(Unow[f+1] + speed2 >= U_max)
                    {
                        Unow[f+1] = U_max;
                    }
                    //and beyond the botom left border of the diamond as the same time
                    else if(Unow[f+1] + speed2 <= -U_max)
                    {
                        Unow[f+1] = -U_max;
                    }
                    //but not beyond any other border
                    else
                    {
                        Unow[f+1] += speed2;
                    }
                }
                //~~~~~~~~~~~~~~~~~~~~~~~~~~~~
                //if we're going beyond the botom left border of the diamond
                else if(Unow[f+1] + speed2 <=- U_max)
                {
                    Unow[f] += speed1;
                    Unow[f+1] = -U_max;
                }
                //~~~~~~~~~~~~~~~~~~~~~~~~~~~~
                //if we're going beyond the top right border of the diamond
                else if(Unow[f+1] + speed2 >= U_max)
                {
                    Unow[f] += speed1;
                    Unow[f+1] = U_max;
                }
            }
            //----------------------------------------------------------------------
            //if we're still in the diamond and driving in speed
            else
            {
                Unow[f] += speed1;
                Unow[f+1] += speed2;
            }
        }
        //----------------------------------------------------------------------
        //if we're still in the diamond and driving in position
        else if(speed_flag == 0 && sinus_flag == 0)
        {
            int i=0;
            for (i = 0; i < 2; i++)         //i=0 <=> V1 and i=1 <=> V2
            {
                if(fabs(Unow[f+i] - Ugoal[f+i]) <= fabs(Uspeed[f+i]*sentDelay) )
                {
                    Unow[f+i] = Ugoal[f+i];                    //we're close enougth to the goal, set now = goal
                    Uspeed[f+i] = 0;
                }
                else
                {
                    Unow[f+i] += Uspeed[f+i]*sentDelay;        //we're still too far away from the goal
                }
            }
        }
        //----------------------------------------------------------------------
        //if sinus mode is activated
        else if(sinus_flag == 1)
        {
            if(f<2) // Warning : only one finger right now, to be upgraded
            {
                double Vy = amplitude * sin(2*3.1415*frequency*sinusTime); // Tension on Vy with the sinus shape
                
                Unow[f] = Vy;
                Unow[f+1] = Vy;
                sinusTime += sentDelay / 1000 / 1000 / 1000 / 2; // Time elapsed since the last loop !! Please don't ask !! DH20150723
                //Ugoal[f] = Unow[f];
                //Ugoal[f+1] = Unow[f+1];
            }
        }
    }

}

void formatGripperSpiPackets( void )
{
    
    //from (-10V to 10V) to (0 to 65535))
    //Table 8. Bipolar Output, Offset Binary Coding p.23 of http://docs-europe.electrocomponents.com/webdocs/10ad/0900766b810ada90.pdf
    //SPI data = ( (65535-0)/(100-(-100)) )*tension + 65535/2

    int i;
    for( i=0 ; i<4 ; i++)
    {
        if(Ulast[i]!=Unow[i])
        {
            Ulast[i]=Unow[i];
            InitGripperDataBank(i);
            GripperSpiPacketsToSend[i]|= ((i)<<24);         //i <=> DAC denomination
            GripperSpiPacketsToSend[i]|= ((unsigned int)( (65535/200)*Unow[i]+(65535/2) ))<<8;
        }
    }
}

const char *byte_to_binary(int x)
{
    static char b[25];
    b[0] = '\0';

    int z;
    for (z = 16777216; z > 0; z >>= 1)
    {
        strcat(b, ((x & z) == z) ? "1" : "0");
    }

    return b;
}

void sendGripperSpiPackets( void )
{
    static int i = 0;
    static int lastSentPacket[4];
    
    if(lastSentPacket[i] != GripperSpiPacketsToSend[i])     // do sendings only if new packets have been formated
    {
        if ( SpiChnTxBuffEmpty(SPI_CHANNEL1) && ( getElapsedTimeInMicroSecond(lastSyncFallTime)/1000 > (SPIxBRG*35)*0.025  ) )   // SPIxBRG*33 timer cycles at 40MHz
        {
            lastSentPacket[i]=GripperSpiPacketsToSend[i];                       // save previous sent packet

            SpiChnPutS(SPI_CHANNEL1, &GripperSpiPacketsToSend[i], 1);           // send
            //AnalogDevice_nLDAC_IO ^= 1;                     // say hello through pin B11
        }
    }
    i = (i+1)%4;
    sentDelay = getElapsedTimeInMicroSecond(lastSentTickTime) / 1000;           //we wan't to use miliseconds, closer to the PiTweez time constants
    lastSentTickTime = ReadCoreTimer();
}

void getGripperDACiRegister( short DAC )
{
    int NOPpacket = 0x1800000;
    unsigned int requestPacket = 0b0;
    requestPacket |= (1<<31);           // 1 means ask for a response
    requestPacket |= ((DAC)<<24);       // choose the DAC to check

    SpiChnPutS(SPI_CHANNEL1, &requestPacket, 1);

    return;
}

void printfGripperReceivedPackets(void)
{
    int i=0;
    for(i=0;i<4;i++)
    {
        printf("%s\n", byte_to_binary(GripperSpiDATAPacketsRecieved[i]));
    }
}

void updateGripperPWMDC(void)
{    
    computeGripperNextTensions();
    int PWM_RFINGER_V1, PWM_RFINGER_V2, PWM_LFINGER_V1,PWM_LFINGER_V2;
    formatGripperPWM(&PWM_RFINGER_V1, &PWM_RFINGER_V2, &PWM_LFINGER_V1,&PWM_LFINGER_V2);    

    //SetDCOC1PWM(PWM_RFINGER_V1);
    //SetDCOC2PWM(PWM_RFINGER_V2);
    //SetDCOC3PWM(PWM_LFINGER_V1);
    //SetDCOC4PWM(PWM_LFINGER_V2);
}



void formatGripperPWM(int *PWM_RFINGER_V1, int *PWM_RFINGER_V2,int *PWM_LFINGER_V1,int*PWM_LFINGER_V2)
{
    *PWM_RFINGER_V1=(unsigned int)(( (MAX_PWM) / U_max)*fabs(Unow[0])); //0-100% vers 0-FPB/FPWM
    *PWM_RFINGER_V2=(unsigned int)(( (MAX_PWM) / U_max)*fabs(Unow[1]));
    *PWM_LFINGER_V1=(unsigned int)(( (MAX_PWM) / U_max)*fabs(Unow[2]));
    *PWM_LFINGER_V2=(unsigned int)(( (MAX_PWM) / U_max)*fabs(Unow[3]));
  
}


/** @defgroup spi24Hack 24bit SPI hack
 *  \brief  This group of interrupts is used to hack the SYNC SPI signal to make it last for 24bits instead of 32bits.
 *
 * The original functions of pic could generate only 8/16/32 bits SYNC.
 * \n The input capture IC5 generates an interrupt at the first falling edge of real SYNC, and sets low virtual SYNC along with it.
 * \n Timer 4 waits SPIxBRG*24.5 cycles before resetting virtual sync high.
 * \n
 * \n <b>Wirings: </b>
 * \n - pin 0 of port E is the virtual SYNC (named \c AnalogDevice_nSS_IO in the code).
 * \n - pin 9 of port D is the real SYNC, it is pluged into IC5.
 *  @{
 */
/** @brief Input Capture checking the state of real SYNC and setting low the virtual SYNC along with it. */
void __ISR( _INPUT_CAPTURE_5_VECTOR, ipl5) IC5Handler( void )        //pin 21 on J11
{
    AnalogDevice_nSS_IO = 0;                //set virtual SYNC with real SYNC
    int nbrOfCaptures = IC5BUF;             //"To clear an interrupt source, read the Buffer Result (ICxBUF)", DS61156F-page 239

    lastSyncFallTime = ReadCoreTimer();     //remember when setting SYNC

    OpenTimer4(T4_ON, SPIxBRG*24.5);        //open TIMER4 to count 24 SPI bit sendings (+ a little margin)
                                            //note: SPI clock at 12,75µs, corresponding to 'SPIxBRG' PBCLK cycles
    mIC5ClearIntFlag();
}


/** @brief Timer 4 reseting virtual SYNC high after 24 SPI sending bits. */
void __ISR( _TIMER_4_VECTOR, ipl6) T4Interrupt( void )
{
    AnalogDevice_nSS_IO = 1;                //when the time for 24-SPI-bit-sendings have been counted, reset virtual SYNC

    OpenTimer4( T4_OFF, 0);                 //shut TM4 down
    mT4ClearIntFlag();
}

/** @} */ //end of Hack group

