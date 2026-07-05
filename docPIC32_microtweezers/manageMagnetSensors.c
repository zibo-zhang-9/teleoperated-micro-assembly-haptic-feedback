#include "TCPIP Stack/TCPIP.h"
#include <string.h>
#include <plib.h>
#include "main.h"
#include "manageMagnetSensors.h"
#include  <math.h>

// define setup parameters for OpenADC10
//            Turn module on | ouput in integer | trigger mode auto | enable autosample
#define PARAM1 ADC_MODULE_ON | ADC_FORMAT_INTG  | ADC_CLK_AUTO      | ADC_AUTO_SAMPLING_ON

// define setup parameters for OpenADC10
//             ADC ref are VDD/VSS | disable offset test    | disable scan mode | perform 2 samples     | use dual buffers | use alternate mode
#define PARAM2 ADC_VREF_AVDD_AVSS  | ADC_OFFSET_CAL_DISABLE | ADC_SCAN_OFF      | ADC_SAMPLES_PER_INT_2 | ADC_ALT_BUF_ON   | ADC_ALT_INPUT_ON

// define setup parameters for OpenADC10
//             use PB clock    | set sample time
#define PARAM3 ADC_CONV_CLK_PB | ADC_SAMPLE_TIME_15        // sample time to be checked

// define setup parameters for OpenADC10
//              set AN0, AN1, AN2 and AN3 as analog inputs for the 4 tensions
#define PARAM4 ENABLE_AN0_ANA | ENABLE_AN1_ANA | ENABLE_AN2_ANA | ENABLE_AN3_ANA

// define setup parameters for OpenADC10
// do not assign channels to scan
#define PARAM5 SKIP_SCAN_ALL


void magnetSensorsADC_generalinit()
{
    CloseADC10();	// ensure the ADC is off before setting the configuration

    // Open ADC10 with parameters 1 to 5
    #if defined (__32MX460F512L__) || defined (__32MX360F512L__) || defined (__32MX795F512L__)                  //<-- ours is __32MX795F512L__
    OpenADC10( PARAM1, PARAM2, PARAM3, PARAM4, PARAM5 );    // configure ADC using the parameters defined above
    #elif defined (__32MX220F032D__) || defined (__32MX250F128D__)
    AD1CHS = 0x05040000;
    AD1CON1 = 0xE4;
    AD1CON2 = 0x7;
    AD1CON3 = 0x8F00;
    ANSELBbits.ANSB3 = 1; //Set AN5 to analog, so not the good one for our case
    ANSELBbits.ANSB2 = 1; //Set AN4 to analog, neither, not our problem now
    #endif

    EnableADC10(); // Enable the ADC
}

void magnetSensorsADC_setANOandAN1( void )
{ 
    // configure to sample AN0 on MUXA and AN1 on MUXB
    //            use ground as neg ref for A | use AN0 for input A     | use ground as neg ref for B | use AN1 for input B
    SetChanADC10( ADC_CH0_NEG_SAMPLEA_NVREF   | ADC_CH0_POS_SAMPLEA_AN0 | ADC_CH0_NEG_SAMPLEB_NVREF   | ADC_CH0_POS_SAMPLEB_AN1); // configure to sample AN0 & AN1
}

void magnetSensorsADC_setAN2andAN3( void )
{
    // configure to sample AN2 on MUXA and AN3 on MUXB
    //            use ground as neg ref for A | use AN2 for input A     | use ground as neg ref for B | use AN3 for input B
    SetChanADC10( ADC_CH0_NEG_SAMPLEA_NVREF   | ADC_CH0_POS_SAMPLEA_AN2 | ADC_CH0_NEG_SAMPLEB_NVREF   | ADC_CH0_POS_SAMPLEB_AN3); // configure to sample AN2 & AN3
}

void magnetSensorsADC_readAllChannels( void )      //values read as integers, might be called in an interrupt to have a nice sampling periode
{
    //starts with AN0 and AN1
    magnetSensorsADC_setANOandAN1();
    while ( ! mAD1GetIntFlag() ){/*wait for the first conversion to complete so there will be vaild data in ADC result registers*/}
    sensorValues[0] = ReadADC10(0);       // read the result of channel 0 conversion
    sensorValues[1] = ReadADC10(1);       // read the result of channel 1 conversion
    #if defined (__32MX460F512L__) || defined (__32MX360F512L__) || defined (__32MX795F512L__)
    mAD1ClearIntFlag();
    #elif defined (__32MX220F032D__) || defined (__32MX250F128D__)
    IFS0bits.AD1IF =0;
    #endif

    // then AN2 and AN3
    magnetSensorsADC_setAN2andAN3();
    while ( ! mAD1GetIntFlag() ){/*wait for the first conversion to complete so there will be vaild data in ADC result registers*/}
    sensorValues[2] = ReadADC10(2);       // read the result of channel 2 conversion
    sensorValues[3] = ReadADC10(3);       // read the result of channel 3 conversion
    #if defined (__32MX460F512L__) || defined (__32MX360F512L__) || defined (__32MX795F512L__)
    mAD1ClearIntFlag();
    #elif defined (__32MX220F032D__) || defined (__32MX250F128D__)
    IFS0bits.AD1IF =0;
    #endif
}


void magnetSensorsADC_intToTensions(int* readValues)         // only turns 0/1024 into -100/100, and puts it in global array
{
    int i;
    for(i=0 ; i<4 ; i++)
    {
         sensorValues[i] = readValues[i]*0.1953125-100;
         //                readValues[i]*(maxSensorTension-minSensorTension)/(1024-0)-(maxSensorTension-minSensorTension)/2;
    }
}

double getSensorsValue( int i )
{
    return sensorValues[i];
}

void __ISR( _TIMER_5_VECTOR ) magnetSensorsADhandler( void )
{
    magnetSensorsADC_readAllChannels();
    mT5ClearIntFlag();
}