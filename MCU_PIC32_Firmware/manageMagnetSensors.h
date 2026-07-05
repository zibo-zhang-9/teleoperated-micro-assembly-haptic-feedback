/** \file manageMagnetSensors.h
 * Header file containing all the function prototypes needed to control the PiTweez magnet position sensors.
 * \n AN0, AN1, AN2 and AN3 are used for AD conversion.
 * \n AN0 and AN1 are first both converted, then AN2 and AN3, using the multiplexer A for AN0/AN2 and B for AN1/AN3
 * \n The resultant values are stored in a static array accessible from external files via getSensorsValue( int i );
 */

#ifndef MANAGEMAGNETSENSORS_H
#define	MANAGEMAGNETSENSORS_H

// Limit tensions the magnet sensors can reach
static double maxSensorTension = 100;
static double minSensorTension = -100;

static double sensorValues[4];

static double samplingRate = 1000;      // 1kHz, absolutely arbitrary

#ifdef	__cplusplus
extern "C" {
#endif


/**********************************************************************//**
*  \brief         Does general initialisations for ADC.
**************************************************************************/
void magnetSensorsADC_generalinit(void);

/**********************************************************************//**
*  \brief         links AN0 and AN1 to MUXA and MUXB
**************************************************************************/
void magnetSensorsADC_setANOandAN1(void);

/**********************************************************************//**
*  \brief         links AN2 and AN3 to MUXA and MUXB
**************************************************************************/
void magnetSensorsADC_setAN2andAN3(void);

/**********************************************************************//**
*  \brief         Does AD cnversions AN0 to AN,3 and stores results in the array passed as argument
*
* \n Should be called in an interrupt to have a nice sampling periode.
**************************************************************************/
void magnetSensorsADC_readAllChannels( void );

/**********************************************************************//**
*  \brief         turns 0/1024 ADC values, to -100/100 values
**************************************************************************/
void magnetSensorsADC_intToTensions(int* readValues);

/**********************************************************************//**
*  \brief         function to allow external functions to read sensor values
**************************************************************************/
double getSensorsValue( int i );


#ifdef	__cplusplus
}
#endif

#endif	/* MANAGEMAGNETSENSORS_H */

