/** \file manageGripper.h
 * Header file containing all the function prototypes needed to control the PiTweez fingers.
 * \n The 4 tensions of the 2 fingers are controlled by 4 DAC of a single Analog Device module, the present program communicates with this module via SPI1.
 * \n <b>Source file</b>: \n @ref manageGripper.c
 * \n <b>Author:</b> \n Benoit Bauin
 * \n <b>Date:</b> \n 23 mai 2013, 15:59
 * \n <b>Usefull links:</b>
 * \n Analog Device DAC datasheet http://www.analog.com/static/imported-files/data_sheets/AD5724R_5734R_5754R.pdf
 * \n PIC32 SPI datasheet http://ww1.microchip.com/downloads/en/DeviceDoc/61106G.pdf
 */

#include <plib.h>

#ifndef MANAGEGRIPPER_H
#define	MANAGEGRIPPER_H

/**********************************************************************//**
* \enum Finger
* \brief      An explicit enum type, grouping possible values of fingers to control
*
* \c BOTH means \c RIGHT and \c LEFT symetrically. For instance, moving \c BOTH by \em x means moving \c RIGHT by \em x/2 and \c LEFT by \em -x/2
**************************************************************************/
typedef enum
{
    LEFT,
    RIGHT,
    BOTH
}Finger ;

#define ISR_freq FPWM

/**********************************************************************//**
 * @name Other variables used for SPI communication.
 *
 * @{ */
/** @brief The last time a SPI sending have been done, in number of core timer clicks. */
static unsigned int lastSentTickTime=0;
/** @brief The last time the real SYNC have been turned low, in number of core timer clicks. */
static short lastSyncFallTime=0;
/** @brief The time seperating two SPI sendings, in miliseconds... I gess, should be verified. */
static double sentDelay=0;
/** @brief The actual state of the real SYNC, di not send new packets while it's still activated. */
static short realSyncActivated=0;
/** @} *********************************************************************/
//end of Other variables used for SPI communication.


//------------------------------------------------------------------------------
// data sheets
//------------------------------------------------------------------------------
//analog device DAC: http://www.analog.com/static/imported-files/data_sheets/AD5724R_5734R_5754R.pdf



#ifdef	__cplusplus
extern "C" {
#endif

//------------------------------------------------------------------------------------------------------------------------
// IN-MAIN FUNCTIONS (functions needing no clean time handling)
//------------------------------------------------------------------------------------------------------------------------

/**********************************************************************//**
*  \brief         Manages all the actions that can be performed by the Gripper.
*
* This function will call all the functions nedded to acheive the commands recieved by TCP.
* \n It can:
* \arg set tensions target depending on a position set point asked by the user (fingers individually or together)
* \arg set tensions variations depending on a speed consign asked by the user (fingers individually or together)
* \arg set a speed bridle to slow down motions
* \arg ask for the Analog Device module to tell the actual state of it's DAC
*
* @param [in] **arg The TCP command formated and stored in an array of "strings"
* @param [in] argc The number of "strings" in \c **arg
**************************************************************************/
void manageGripper(char** arg,int argc);

/**********************************************************************//**
*  \brief         Manage unreachable position consigns.
*
* Only positions in a diamond can be reached, that's due to the crossing of tensions in the fingers. The diagonals of the diamond are both equal to \c max.
* \n If a target is set out of this diamond, reinjectDiamond() will retrieve the exceeding from the Z axis, making the Y axis dominant.
* @param [in,out] *val1,*val2 The position of the target to be checked, in %.
* @param [in] max The dimention of the diamond.
**************************************************************************/
void reinjectDiamond(double *val1, double *val2, double max);

/**********************************************************************//**
*  \brief         Computes the tensions needed to reach a position.
*
* We first assume that tension and displacement are proportional, so positions and tensions are mingled in the program ( <em>  Vx  =  X </em> ).
* \n Then because of the actual repartition of the electrodes on the fingers, we cross these tensions:
* \n  <em> V1 = Vz - Vy &nbsp; &nbsp; &nbsp; V2 = Vz + Vy  </em>
* \n \em note: everthing is in %.
* @param [in,out] *val1,*val2 The position <em> (Y,Z)</em>  of the target comes in, the tensions for the finger's electodes <em> (V1,V2)</em>  come out
**************************************************************************/
void computeCrossTensions(double *val1, double *val2);

/**********************************************************************//**
*  \brief         Computes the speeds needded to reach a position set point.
*
* \n \em note: everthing is in %, or % per unit of time for speeds.
* @param [in,out] *speed1,*speed2 the variables used to store the computed speeds
* @param [in] pos1,pos2 the position target
* @param [in] finger the finger concerned by the consign
**************************************************************************/
void speedsFromPositions( double *speed1, double *speed2, double pos1, double pos2, Finger finger );

/**********************************************************************//**
*  \brief         Allocates atomically all the consigns to a finger.
*
* To insure integrity of data, we wait for all the tensions to be computed, and then we allocate them at once. It is useless is everything is done synchronously, but we keep it for now just in case.
* @param [in] pos1,pos2,speed1,speed2 the position and speed consigns to allocate
* @param [in] finger the finger concerned by the allocation
**************************************************************************/
void AllocateConsigns( double pos1, double pos2, double speed1, double speed2, Finger finger );



void updateGripper();

void invertCrossTensions(double *val1, double *val2);

//------------------------------------------------------------------------------------------------------------------------
// IN-ISR FUNCTIONS (functions needing a clean time handling)
//------------------------------------------------------------------------------------------------------------------------


/**********************************************************************//**
*  \brief         Calls the computing functions needed to control the gripper
*
* Calls functions to:
* \n Take tensions consigns, or tensions variations consigns, and compute the next tensions to move the gripper according to them (computeGripperNextTensions()).
* \n determine the SPI packet to send to the Analog Device to actally forward theses tensions to the gripper (formatGripperSpiPackets()).
* @see computeGripperNextTensions().
* @see formatGripperSpiPackets().
*
* \I can't figure out how to get rid of the following sentance with the buggy characters, don't mind it.
**************************************************************************/
void GripperSpiComputings();


/**********************************************************************//**
*  \brief           Sends formated SPI packets to the Analog Device to control the gripper
*
* This function takes already formated packets (stored in global arrays) to send them via SPI (SPI1 for the Analog Device).
* \n It only sends packets only if they're different from the last sending.
* \n It waits for the real SYNC to be reseted high before sending a new packet.
* \n <b>IMPORTANT</b> the SYNC signal of SPI1 is hacked to last 24bits instead of 32bits.
* @param [in] GripperSpiPacketsToSend[] a global array storing the formated SPI packets to be sent.
* @see formatGripperSpiPackets().
* @see SpiInitDevice().
**************************************************************************/
void sendGripperSpiPackets( void);

/**********************************************************************//**
*  \brief           Ask the Analog Device to tell the status of one of it's DAC
*
* This function sends an SPI packet to the Analog Device, asking it to respond with a 24bit message
* containing the actual state of one of it's DAC.
* \n This function only asks for an answer, it does \em NOT read it.
* \n No reading functions have been implemented as we red answers directly with an oscilloscope.
* \n <b>IMPORTANT</b> the SYNC signal of SPI1 is hacked to last 24bits instead of 32bits.
* @param [in] DAC       the DAC to check (0,1,2 or 3 for DAC A,B,C or D respectively).
**************************************************************************/
void getGripperDACiRegister( short DAC );

/**********************************************************************//**
*  \brief          Translates Bytes into arrays of 24 bits.
*
* @param [in] x    the byte to be translated.
* @return a binary number storred in an array.
**************************************************************************/
const char *byte_to_binary( int x );

/**********************************************************************//**
*  \brief          Computes the next tension to be sent to the Gripper
*
* The actual tensions are moved chunk by chunk to prevent to violent variations.
* \n These chunks are computed depending on the SPI sending frequency and the consigns asked by the user:
* \n <b>position consign:</b> \t each call moves actual tensions one chunk closer to the target set point, according to a speed bridle.
* \n <b>speed consign:</b> \t each call moves actual tensions by one chunk, depending on the speed consign, and with no objectives (never stops moving).
* @param [in] Ugoal a global array containing the targeted tensions.
* @param [in] Uspeed a global array containing the tensions variations.
* @param [in] Uspeed_bridle a global double value corresponding to the speed to follow if position control.
* @param [out] Unow a global array containing the actual tensions to send to the gripper, incremented chunk by chunk.
**************************************************************************/
void computeGripperNextTensions(void);

/**********************************************************************//**
*  \brief       Formats SPI packets to send to the Analog Device.
*
* The SPI packets are formated to be sent to the Analog Device to make it produce the tensions stored in the global array \c Unow
*
* @param [in] Unow the global array storring the tensions we wan't the Analog device to send to the gripper
**************************************************************************/
void formatGripperSpiPackets(void);

/**********************************************************************//**
*  \brief       Clear the 'i'th box in GripperSpiPacketsToSend, the global array containing the SPI packets
*
* Used in formatGripperSpiPackets() whitch does successive bitwise operations from void packets
* @param [in] i the index of the packet to be cleared
**************************************************************************/
void InitGripperDataBank( int i);

void updateGripperPWMDC(void);

void formatGripperPWM(int *PWM_RFINGER_V1, int *PWM_RFINGER_V2,int *PWM_LFINGER_V1,int*PWM_LFINGER_V2);

#ifdef	__cplusplus
}
#endif

#endif	/* MANAGEGRIPPER_H */

