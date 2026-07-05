#define THIS_IS_STACK_APPLICATION

//#include <p32xxxx.h> already included in plib.h
#include  <plib.h>
#include  <stdio.h>
#include  <math.h>
#include "TCPIP Stack/TCPIP.h"
#include "main.h"
#include "server.h"
#include "manageGripper.h"
#include "manageForceSensor.h"
#include "manageStack.h"
//#include "commands.h"

#define isTestingTime 1
#define PWM_withOnBoardButtons 0



// Declare AppConfig structure and some other supporting stack variables
APP_CONFIG AppConfig;
static unsigned short wOriginalAppConfigChecksum;    // Checksum of the ROM defaults for AppConfig

UDP_SOCKET MySocket ;

// Private helper functions.
// These may or may not be present in all applications.
static void InitAppConfig(void);
static void InitializeBoard(void);
static void InitServerConfig(void);
static void AppCompute(void);


typedef struct list_cmd list_cmd;
struct list_cmd
{
    Cmd cmd;
    list_cmd *pNext;
};

static list_cmd *cmds = NULL;




// return elapsed time in nanoseconds
unsigned int getElapsedTimeInNanoSecond(unsigned int lastTime)
{
    unsigned int currentTime = ReadCoreTimer();
    if (currentTime < lastTime)
    {
        return (currentTime + 0xFFFFFFFF - lastTime) * 25;
    }
    else
    {
        return (currentTime - lastTime) * 25;
    }
}

double getElapsedTimeInMicroSecond(unsigned int lastTime)
{
    unsigned int currentTime = ReadCoreTimer();
    if (currentTime < lastTime)
    {
        return (currentTime + 0xFFFF - lastTime) * 0.025;
    }
    else
    {
        return (currentTime - lastTime) * 0.025;
    }
}

double getElapsedTimeInMilliSecond(unsigned int lastTime)
{
    unsigned int currentTime = ReadCoreTimer();
    if (currentTime < lastTime)
    {
        return (currentTime + 0xFFFF - lastTime) * 0.000025;
    }
    else
    {
        return (currentTime - lastTime) * 0.000025;
    }
}

double getElapsedTimeInSecond(unsigned int lastTime)
{
    unsigned int currentTime = ReadCoreTimer();
    if (currentTime < lastTime)
    {
        return (currentTime + 0xFFFF - lastTime) * 0.000000025;
    }
    else
    {
        return (currentTime - lastTime) * 0.000000025;
    }
}



//------------------------------------------------------------------------------
//main function
//------------------------------------------------------------------------------
int main(void)
{
    // Initialize application specific hardware
    InitializeBoard();
    //InitServerConfig();
    TickInit();
    InitAppConfig();
    StackInit();
    DBINIT();

    ForceSensorsADC_generalinit();
    WriteCoreTimer(0);

    int i = 1;

    //OpenPort_UDP(MySocket);
    //mT2IntEnable(1);
    T2CONbits.ON = 1 ;
    IEC0bits.T2IE = 0 ;
    IPC2bits.T2IP = 1 ;
    IFS0bits.T2IF = 0 ;
    IEC0bits.T2IE = 1 ;
    
    //main loop ----------------------------------
    while( 1 )
    {
        // This task performs normal stack task including checking
        // for incoming packet, type of packet and calling
        // appropriate stack entity to process it.
        StackTask();

        // This tasks invokes each of the core stack application tasks
        StackApplications();



//        ServerCompute();	//cuts and puts words from TCP messages into a structure (struct RegisteredCmd)
        AppCompute();		//calling commands related to the words in the structure
        AcqForceSensor();
        updateGripper();
        StackPWMDCComputings();

        if(isTestingTime)
            testDebug_TimerCompute();

    }
}

static void InitializeBoard(void)
{
    // LEDs, set as outputs (of course)
    LED0_TRIS = 0;
    LED1_TRIS = 0;
    LED2_TRIS = 0;
    LED3_TRIS = 0;
    LED4_TRIS = 0;
    LED5_TRIS = 0;
    LED6_TRIS = 0;
    LED7_TRIS = 0;
    LED_PUT(0x00);

    #if defined(__PIC32MX__)
    {
        // Enable multi-vectored interrupts
        INTEnableSystemMultiVectoredInt();

        // Enable optimal performance
        //SYSTEMConfigPerformance(GetSystemClock());
        //mOSCSetPBDIV(OSC_PB_DIV_1);                // Use 1:1 CPU Core:Peripheral clocks

        // Disable JTAG port so we get our I/O pins back, but first
        // wait 50ms so if you want to reprogram the part with
        // JTAG, you'll still have a tiny window before JTAG goes away.
        // The PIC32 Starter Kit debuggers use JTAG and therefore must not
        // disable JTAG.
        DelayMs(50);
        #if !defined(__MPLAB_DEBUGGER_PIC32MXSK) && !defined(__MPLAB_DEBUGGER_FS2)
            DDPCONbits.JTAGEN = 0;
        #endif
        LED_PUT(0x00);                // Turn the LEDs off

        CNPUESET = 0x00098000;        // Turn on weak pull ups on CN15, CN16, CN19 (RD5, RD7, RD13), which is connected to buttons on PIC32 Starter Kit boards
    }
    #endif




    // PWM initialisations
    //OpenOC1( OC_ON | OC_TIMER3_SRC | OC_PWM_FAULT_PIN_DISABLE, 0, 0);     //should be used for heating resistors, but is multyplexed sith SDO1
    OpenOC2( OC_ON | OC_TIMER3_SRC | OC_PWM_FAULT_PIN_DISABLE, 0, 0);       //should be used for heating resistors
    OpenOC3( OC_ON | OC_TIMER3_SRC | OC_PWM_FAULT_PIN_DISABLE, 0, 0);       //should be used for piezo Stack
    OpenOC4( OC_ON | OC_TIMER3_SRC | OC_PWM_FAULT_PIN_DISABLE, 0, 0);       //should replace OC1 to control heating resistor


    // init Timer2 for real time processing with UDP
    //OpenTimer2( T2_OFF | T2_PS_1_1 | T2_SOURCE_INT | T2_32BIT_MODE_OFF, SYNC_per);
    //OpenTimer2( T2_ON | T2_PS_1_256 | T2_SOURCE_INT, 65535);
    //mT2SetIntPriority( 5);      // set Timer2 Interrupt Priority
    //mT2ClearIntFlag(); 		// clear interrupt flag
    //mT2IntEnable(0);		// disable timer2 interrupts
    T2CONbits.ON = 0;
    T2CONbits.T32 = 0 ;
    T2CONbits.TCKPS = 0b111 ;
    T2CONbits.TCS = 0 ;
    PR2 = 0x88B8;
    TMR2 = 0 ;

    // init Timer3 mode and period (PR3=MAX_PWM-1)
    OpenTimer3( T3_ON | T3_PS_1_1 | T3_SOURCE_INT, MAX_PWM - 1);
    mT3SetIntPriority( 4);      // set Timer3 Interrupt Priority
    mT3ClearIntFlag(); 		// clear interrupt flag
    mT3IntEnable( 1);		// enable timer3 interrupts

   // init Timer4, used to reset virtual SYNC high
    OpenTimer4( T4_OFF | T4_PS_1_1 | T4_SOURCE_INT, SPIxBRG*24.5);
    mT4SetIntPriority( 6 );      // set Timer4 Interrupt Priority
    mT4ClearIntFlag(); 		// clear interrupt flag
    mT4IntEnable( 1);		// enable timer4 interrupts

    // Timer 5, initialised to be used for manageMagnetSensors A/D interrupt
/*    OpenTimer5( T5_OFF | T5_PS_1_8 | T5_SOURCE_INT, 40000);      //1kHz
    mT5SetIntPriority( 5 );      // set Timer5 Interrupt Priority
    mT5ClearIntFlag(); 		// clear interrupt flag
    mT5IntEnable( 1);		// enable timer5 interrupts
*/
    // Timer 5, used for manageForceSensors A/D interrupt
/*    OpenTimer5( T5_OFF | T5_PS_1_8 | T5_SOURCE_INT, 40000);      //1kHz
    mT5SetIntPriority( 5 );      // set Timer5 Interrupt Priority
    mT5ClearIntFlag(); 		// clear interrupt flag
    mT5IntEnable( 1);		// enable timer5 interrupts
*/
    
    // init input Capture 5 interupt on each falling edge of real SYNC
    OpenCapture5( IC_EVERY_FALL_EDGE | IC_INT_1CAPTURE | IC_TIMER3_SRC | IC_FEDGE_FALL | IC_ON | IC_CAP_16BIT);
    mIC5IntEnable(1);
    mIC5SetIntPriority(5);
    mIC5ClearIntFlag();

    //SPI initialization ------------------------
    InitGripperPINS();
    SpiChannel spiChn=SPI_CHANNEL1;		// the SPI channel to use
    SpiInitDevice(spiChn, 1, 1, 1);             //master, framed mode enabled!
    sendGripperInitSpiPackets();                //send packets to init output range and power control
    InitGripperDataBank(0);
    InitGripperDataBank(1);
    InitGripperDataBank(2);
    InitGripperDataBank(3);
    InitStackDataBanks();
}

/*********************************************************************
 * Function:        void InitAppConfig(void)
 *
 * PreCondition:    MPFSInit() is already called.
 *
 * Input:           None
 *
 * Output:          Write/Read non-volatile config variables.
 *
 * Side Effects:    None
 *
 * Overview:        None
 *
 * Note:            None
 ********************************************************************/
// MAC Address Serialization using a MPLAB PM3 Programmer and
// Serialized Quick Turn Programming (SQTP).
// The advantage of using SQTP for programming the MAC Address is it
// allows you to auto-increment the MAC address without recompiling
// the code for each unit.  To use SQTP, the MAC address must be fixed
// at a specific location in program memory.  Uncomment these two pragmas
// that locate the MAC address at 0x1FFF0.  Syntax below is for MPLAB C
// Compiler for PIC18 MCUs. Syntax will vary for other compilers.
//#pragma romdata MACROM=0x1FFF0
static ROM BYTE SerializedMACAddress[6] = {MY_DEFAULT_MAC_BYTE1, MY_DEFAULT_MAC_BYTE2, MY_DEFAULT_MAC_BYTE3, MY_DEFAULT_MAC_BYTE4, MY_DEFAULT_MAC_BYTE5, MY_DEFAULT_MAC_BYTE6};
//#pragma romdata

static void InitAppConfig(void)
{
    while(1)
    {
        // Start out zeroing all AppConfig bytes to ensure all fields are
        // deterministic for checksum generation
        memset((void*)&AppConfig, 0x00, sizeof(AppConfig));

        AppConfig.Flags.bIsDHCPEnabled = TRUE;
        AppConfig.Flags.bInConfigMode = TRUE;
        memcpypgm2ram((void*)&AppConfig.MyMACAddr, (ROM void*)SerializedMACAddress, sizeof(AppConfig.MyMACAddr));

        AppConfig.MyIPAddr.Val = MY_DEFAULT_IP_ADDR_BYTE1 | MY_DEFAULT_IP_ADDR_BYTE2<<8ul | MY_DEFAULT_IP_ADDR_BYTE3<<16ul | MY_DEFAULT_IP_ADDR_BYTE4<<24ul;
        AppConfig.DefaultIPAddr.Val = AppConfig.MyIPAddr.Val;
        AppConfig.MyMask.Val = MY_DEFAULT_MASK_BYTE1 | MY_DEFAULT_MASK_BYTE2<<8ul | MY_DEFAULT_MASK_BYTE3<<16ul | MY_DEFAULT_MASK_BYTE4<<24ul;
        AppConfig.DefaultMask.Val = AppConfig.MyMask.Val;
        AppConfig.MyGateway.Val = MY_DEFAULT_GATE_BYTE1 | MY_DEFAULT_GATE_BYTE2<<8ul | MY_DEFAULT_GATE_BYTE3<<16ul | MY_DEFAULT_GATE_BYTE4<<24ul;
        AppConfig.PrimaryDNSServer.Val = MY_DEFAULT_PRIMARY_DNS_BYTE1 | MY_DEFAULT_PRIMARY_DNS_BYTE2<<8ul  | MY_DEFAULT_PRIMARY_DNS_BYTE3<<16ul  | MY_DEFAULT_PRIMARY_DNS_BYTE4<<24ul;
        AppConfig.SecondaryDNSServer.Val = MY_DEFAULT_SECONDARY_DNS_BYTE1 | MY_DEFAULT_SECONDARY_DNS_BYTE2<<8ul  | MY_DEFAULT_SECONDARY_DNS_BYTE3<<16ul  | MY_DEFAULT_SECONDARY_DNS_BYTE4<<24ul;

        // Load the default NetBIOS Host Name
        memcpypgm2ram(AppConfig.NetBIOSName, (ROM void*)MY_DEFAULT_HOST_NAME, 16);
        FormatNetBIOSName(AppConfig.NetBIOSName);

        // Compute the checksum of the AppConfig defaults as loaded from ROM
        wOriginalAppConfigChecksum = CalcIPChecksum((BYTE*)&AppConfig, sizeof(AppConfig));

        break;
    }
}

//------------------------------------------------------------------------------
//commands list and their respective TCP buffer (char*)
//------------------------------------------------------------------------------
static void InitServerConfig(void)
{
   /* registerCmd(LED1, "LED1");
    registerCmd(LED2, "LED2");
    registerCmd(STACK, "STACK");
    registerCmd(GRIPPER,"GRIPPER");
    registerCmd(FORCESENSOR,"FORCESENSOR");
    registerCmd(TIME,"TIME");
    registerCmd(VERSION,"VERSION?");
    registerCmd(INIT,"INIT?");*/
}

//------------------------------------------------------------------------------
//Function managing commands actions
//------------------------------------------------------------------------------
static void AppCompute(void)
{    
    while ( haveCmd() )
    {
        Cmd cmd = popCmd();
        switch (cmd.cmd_typ)
        {
            case STACK:
                manageStack(cmd.args,cmd.argc);
                break;
            case LED1:
                manageLed(LED1_IO, cmd.args);
                break;
            case LED2:
                manageLed(LED2_IO, cmd.args);
                break;
            case GRIPPER:
            {
                manageGripper(cmd.args,cmd.argc);
                break;
            }
            case FORCESENSOR:
            {
                manageForceSensor(cmd.args,cmd.argc);
                break;
            }
            /*
            case SLEEP: BUGGY
            {
                sleep(cmd.args,cmd.argc);
                break;
            }*/
            case TIME:
            {
                testDebug_SendTimeFlow();
                break;
            }
            case VERSION:
            {
                char msg[30] = "\n";
                char version[16] = DRIVERVERSION;
                sprintf(msg, "VERSION:%s\n", version);
                //ServerSend(msg);
                break;
            }
            case INIT:
            {
                char initMsg[30] = "Init\n";
               // ServerSend(initMsg);
                break;
            }
        }
        releaseCmd(&cmd);
    }
}

//------------------------------------------------------------------------------
//several functions to deal with the raw commands
//------------------------------------------------------------------------------
void addCmd(Cmd cmd)
{
    list_cmd* newElement = (list_cmd*)malloc(sizeof(list_cmd));
    newElement->cmd = cmd;
    newElement->pNext = NULL;

    if (cmds)
    {
        list_cmd* pElement = cmds;
        while (pElement->pNext)
            pElement = pElement->pNext;

        pElement->pNext = newElement;
    }
    else
    {
        cmds = newElement;
    }
}

void releaseCmd(Cmd* cmd)
{
    if (cmd->argc)
    {
        int i;
        for (i = 0; i < cmd->argc; i++)
            free(cmd->args[i]);
            free(cmd->args);
    }
}
Cmd popCmd()
{
    if (cmds)
    {
        Cmd cmd = cmds->cmd;
        list_cmd* newFirst = cmds->pNext;
        free(cmds);
        cmds = newFirst;
        return cmd;
    }
    else
    {
        Cmd cmd;
        cmd.cmd_typ = NO;
        return cmd;
    }
}
int haveCmd()
{
    if (cmds)
        return 1;
    else
        return 0;
}


/** @defgroup stackPwmInt Stack PWM interrupt
 *  \brief  The interrupt updating stack PWM duty cycle.
 *
 * It calls \ref sendStackPWMDC(); to do the updating.
 *  @{
 */
void __ISR( _TIMER_3_VECTOR, ipl4) T3Interrupt( void)
{
    mT3ClearIntFlag();
    sendStackPWMDC();
    //sendHeatResistorsPWMDC(); to be tested
    //AnnounceIP_UDP();

}
/** @} */ //end of PWM group

void __ISR( _TIMER_2_VECTOR, ipl4) T2Interrupt( void)
{
    //mT2ClearIntFlag();
    IFS0bits.T2IF = 0 ;
    //SendData_UDP(MySocket);
    WriteTime_UDP();
    
}
/** @} */ //end of PWM group

