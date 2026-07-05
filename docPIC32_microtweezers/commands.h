/* 
 * File:   commands.h
 * Author: Benoit Bauin
 *
 * Created on 18 avril 2013, 11:19
 */

#ifndef COMMANDS_H
#define	COMMANDS_H

//inscribed rectangle dimensions (VY_MAX , 10-VY_MAX)
#define VY_MAX          (7)
#define VZ_MAX          (10-VY_MAX)


//tensions
 //instructions
static double LFinger_Vy;
static double RFinger_Vy;
static double LFinger_Vz;
static double RFinger_Vz;

 //output values
static double LFinger_V1;
static double RFinger_V1;
static double LFinger_V2;
static double RFinger_V2;

#ifdef	__cplusplus
extern "C" {
#endif

//controlling V1/V2 from Vy/Vz, staying in a VY_MAX/10-VY_MAX sized rectangle
void ControlV12_rectangle(double Vy,double Vz,double* V1,double* V2);

//controlling V1/V2 from Vy/Vz, with Vy dominating Vz
void ControlV12_Ydominant(double Vy,double Vz,double* V1,double* V2);

//managing state of leds
void manageLed(unsigned led, char** arg);

//controlling both fingers with 'ControlV12_rectangle' functions
void manageGripper(char** arg,int argc);

//Managing one finger, right or left with 'ControlV12_rectangle' function
void manageXFinger(char* finger, char** arg,int argc);

//sleep, doesn't seem to work
void sleep(char** arg,int argc);


#ifdef	__cplusplus
}
#endif

#endif	/* COMMANDS_H */

