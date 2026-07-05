/* 
 * File:   manageForceSensor.h
 * Author: Bilal.Komati
 *
 * Created on 10 février 2015, 11:40
 */

#ifndef MANAGEFORCESENSOR_H
#define	MANAGEFORCESENSOR_H

// Limit tensions the force sensors can reach
static double maxSensorTension = 100;
static double minSensorTension = -100;

static double sensorValue = 0;              // if we have one voltage
static unsigned short essai = 0;
//static double sensorValues[4];          // if we have 4 voltages

static double samplingRate = 1000;      // 1kHz, absolutely arbitrary

#ifdef	__cplusplus
extern "C" {
#endif

void ForceSensorsADC_generalinit();
//void ForceSensorsADC_setAN4andAN5( void );
void ForceSensorsADC_readAllChannels( void );
void manageForceSensor(char** arg,int argc);
void AcqForceSensor();

void sendForce();

#ifdef	__cplusplus
}
#endif

#endif	/* MANAGEFORCESENSOR_H */

