/** \file PWMbyButtons.h
 * Header file containing all the function prototypes needed to control WPM modules OC2, OC3 and OC4 via on board buttons.
 * \n This module has been done for easy tests on the PiTweez electronic
 * \n <b>Source file</b>: \n @ref PWMbyButtons.c
 * \n <b>Author:</b> \n Benoit Bauin
 * \n <b>Date:</b> \n 27 juin 2013, 18:31
 */

#ifndef PWMBYBUTTONS_H
#define	PWMBYBUTTONS_H

#ifdef	__cplusplus
extern "C" {
#endif

/**********************************************************************//**
*  \brief         Computes PWM values for OC2, OC3 and OC4 for electronical tests.
*
* \n SW1 <=> OC2    SW2 <=> OC3     SW3 <=> OC4
**************************************************************************/
void computePWMDCbyButtons(void);

/**********************************************************************//**
*  \brief         Sends PWM values to OC2, OC3 and OC4.
**************************************************************************/
void updatePWMDCbyButtons(void);

#ifdef	__cplusplus
}
#endif

#endif	/* PWMBYBUTTONS_H */

