/** \file manageStack.h
 * Header file containing all the function prototypes needed to control the PiTweez piezo electrical Stack.
 * \n The tension of the stack is controlled by a PWM (OC3), amplified and lowpass filtered.
 */

#include <plib.h>

#ifndef MANAGESTACK_H
#define	MANAGESTACK_H



//------------------------------------------------------------------------------
//data sheets:
//------------------------------------------------------------------------------
// amplifier:   http://www.cedrat-technologies.com/fileadmin/user_upload/cedrat_groupe/Mechatronic_products/User_s_manual/Inst_Man_CA-u10_v3.3.2.pdf
// -> older version http://www.mmech.com/~mmechc5/images/stories/Standard_Products/Cedrat/Cedrat%20Links/Inst_Man_CAu10_v3.2.3.pdf
// stack:       http://www.cedrat-technologies.com/fileadmin/user_upload/cedrat_groupe/Mechatronic_products/Piezo_actuators_electronics/APAs/APA30uXS_GB_v3.3.pdf

#ifdef	__cplusplus
extern "C" {
#endif

//------------------------------------------------------------------------------------------------------------------------
// MAIN FUNCTIONS
//------------------------------------------------------------------------------------------------------------------------

/**********************************************************************//**
*  \brief         Manages all the actions that can be performed by the Stack.
*
* This function will call all the functions nedded to acheive the commands recieved by TCP.
* \n It can:
* \arg set tension target depending on a position set point asked by the user
* \arg set tension variations depending on a speed consign asked by the user
*
* @param [in] **arg The TCP command formated and stored in an array of "strings"
* @param [in] argc The number of "strings" in \c **arg
**************************************************************************/
void manageStack(char** arg,int argc);

/**********************************************************************//**
*  \brief         Manage unreachable consigns.
*
* Reinject out of reach consigns (speed or position) within the boundaries.
* @param [in,out] *value The value to be checked.
* @param [in] minLimit,maxLimit The boundaries.
**************************************************************************/
void reinjectStackValue(double * value, double minLimit, double maxLimit);


/**********************************************************************//**
*  \brief         Computes the tension needed to reach a position.
*
* As everything is in %, it does quite nothing for the moment: just tells "we need x% of tension to achieve x% of displacement".
* \n it could become usefull when micrometers and volts will replace %.
* @param [in] position The position to be reach, in %.
* @return The tension needed to reach this position, in %.
**************************************************************************/
double computeStackTension(double position, double force);

/**********************************************************************//**
*  \brief         Computes the tension variation needed to move at a speed consign.
*
* As everything is in %, it does quite nothing for the moment: just tells "we need x% of V per s to achieve x% of um per s".
* \n it could become usefull when micrometers and volts will replace %.
* @param [in] dPosition The position variation asked, in %/s.
* @return The tension variation needed to move at this speed, in %/s.
**************************************************************************/
double computeStackdTension(double dPosition, double dForce);


//------------------------------------------------------------------------------------------------------------------------
// ISR FUNCTIONS
//------------------------------------------------------------------------------------------------------------------------

/**********************************************************************//**
* \brief         Calls all the functions needed to get correct PWM duty cycles.
*
* It first computes the next tension to send to the stack to reach a targeted position or to follow a speed (computeStackNextTension).
* \n Then determines the PWM duty cycle related to that next tension (formatStackPWM).
* @param [in] StackTension_goal,StackdTension Global variables containing consigns.
* @param [out] Stack_PWMDC The PWM duty cycle corresponding to the next tensions to send.
* @see computeStackNextTension()
* @see formatStackPWM()
**************************************************************************/
void StackPWMDCComputings();

/**********************************************************************//**
* \brief         Computes the next tension to send to the Stack.
*
* The tensions are moved chunk by chunk to avoid excessive variations.
* @param [in] StackTension_goal,StackdTension Global variables containing consigns.
* @param [out] StackTension_current Global variable containing the tension to send to the Stack.
**************************************************************************/
void computeStackNextTension();

/**********************************************************************//**
* \brief         Determines the PWM duty cycle corresponding to the tension to send to the Stack.
*
* Take a look at the @ref group2.
*
* @param [in] StackTension_current Global variable containing the tension to send to the Stack.
* @param [out] the PWM duty cycle value corresponding to that tension.
* @see computeStackNextTension(), which computes StackTension_current
**************************************************************************/
unsigned int formatStackPWM();

/**********************************************************************//**
* \brief         Updates the stack's PWM duty cycle.
*
* Is called in the PWM interrupt @ref T3Interrupt( void)
*
* @param [in] StackTension_current Global variable containing the tension to send to the Stack.
* @return the PWM duty cycle value corresponding to that tension.
**************************************************************************/
void sendStackPWMDC();


/**********************************************************************//**
* \brief         Cancel the actual position consign of the stack.
*
* Is used when a speed consign is set by the user.
* \n It actually sets the goal at the current position.
**************************************************************************/
void cancelStackPositionConsign();

/**********************************************************************//**
* \brief         Cancel the actual speed consign of the stack.
*
* Is used when a posstion consign is set by the user.
* \n It simply sets the speed to 0.
**************************************************************************/
void cancelStackSpeedConsign();

/**********************************************************************//**
* \brief         Initialise all the stack values.
*
* Set PWM duty cycle and SPI packets (not used in the actual version) to 0.
**************************************************************************/
void InitStackDataBanks();



void StackSpiComputings();
void sendStackSPIPackets(SpiChannel chn, int wordsNbr);
void formatStackSpiPacket();


#ifdef	__cplusplus
}
#endif

#endif	/* MANAGESTACK_H */

