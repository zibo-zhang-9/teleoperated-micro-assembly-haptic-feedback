#ifndef MANAGEHEATRESISTORS_H
#define	MANAGEHEATRESISTORS_H

#ifdef	__cplusplus
extern "C" {
#endif


/**********************************************************************//**
*  \brief         Calls the actions linked to messages received via TCP.
*
* \n Heat one resistor, both, or sendback the actual value of heat consign for both resistors
**************************************************************************/
void manageHeatResistors( char** arg,int argc );

/**********************************************************************//**
*  \brief         Computes the PWM dutycycle from the "heat" value.
*
* \n Uses the same interrupt as the STACK
**************************************************************************/
int computeHeatPWM( double heat );

/**********************************************************************//**
*  \brief         Loads PWM modules with the computed dutycycles.
*
* \n Loads OC2 and OC4;
**************************************************************************/
void sendHeatResistorsPWMDC();

#ifdef	__cplusplus
}
#endif

#endif	/* MANAGEHEATRESISTORS_H */

