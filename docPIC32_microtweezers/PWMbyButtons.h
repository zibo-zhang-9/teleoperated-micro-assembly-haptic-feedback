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

