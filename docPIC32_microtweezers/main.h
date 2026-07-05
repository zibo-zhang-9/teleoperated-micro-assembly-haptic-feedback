/** \file main.h
 *
 * \n
 * \n <b>Author:</b> \n Florent Perrocheau
 * \n <b>Date:</b> \n 13 mars 2013, 16:05
 * \n <b>Usefull links:</b>
 *
 */

#ifndef MAIN_H
#define	MAIN_H

#include "TCPIP Stack/HardwareProfile.h"


#define DRIVERVERSION "1.1.0"

//------------------------------------------------------------------------------
//PWM
//------------------------------------------------------------------------------
#define FPB      GetSystemClock()/2               //cf hardwareProfile.h, FPB is a misnomer, cause in fact its SRCLK:
                                                    //TIMER 3, interrupt source, is sown by INTERN CLOCK, so 80 MHz
#define FPWM     15000                           //resolution = FPB/FPWM = a lot
#define MAX_PWM  (FPB/FPWM)                     //max duty cycle <=> resolution
                                                //only 2kHz because there's a lot of computings done in the ISR

#define SYNC_per 200
#define SPIxBRG 511

//PWM values
int PWM_RFINGER_V1;                 //should not be negative when passed to SetDCOCxPWM
int PWM_RFINGER_V2;
int PWM_LFINGER_V1;
int PWM_LFINGER_V2;



#ifdef	__cplusplus
extern "C" {
#endif

/**********************************************************************//**
* \enum typCmd
* \brief      An enum type, grouping all possible devices that can be controlled by the program.
*
* Actually the working devices are GRIPPER and STACK. The others are for testing purpose or not yet deleted unused devices.
**************************************************************************/
typedef enum {
    NO,
    LED1,
    LED2,
    STACK,
    GRIPPER,
    FORCESENSOR,
    //SLEEP,
    TIME,
    STOP,
    VERSION,
    INIT
} typCmd;

/**********************************************************************//**
 * \brief  A complete command.
 *
 * When the caracter \\n is encountered the construction is over and the command is put in the structure @ref Cmd.
**************************************************************************/
typedef struct
{
    typCmd cmd_typ;
    char** args;
    int argc;
} Cmd;


/**********************************************************************//**
 *
 * \brief  A command under construction, as array of characters.
 *
 * When the caracter \\n is encountered the construction is over and the command is put in the structure @ref Cmd.
**************************************************************************/
typedef struct
{
    char* cmdToBuild;
    int cmdToBuildc;
} CmdBuffer;

/** @brief Adds the passed cmd to the list of cmd (cmds) to be treated by main.c functions. */
void addCmd(Cmd cmd);

/** @brief Delete the passed cmd from the list of cmd (cmds) to be treated by main.c functions. */
void releaseCmd(Cmd* cmd);

    Cmd popCmd();
    int haveCmd();

/**********************************************************************//**
 * @brief  Call the function managing the device asked by TCP
 *
 * @see typCmd for the list of know devices.
**************************************************************************/
static void AppCompute(void);


#ifdef	__cplusplus
}
#endif

#endif	/* MAIN_H */

