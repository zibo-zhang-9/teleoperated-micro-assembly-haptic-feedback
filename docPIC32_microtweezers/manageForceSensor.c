#include "TCPIP Stack/TCPIP.h"
#include <string.h>
#include <plib.h>
#include "main.h"
#include "manageForceSensor.h"
#include  <math.h>

static BOOL forceSensorEnable = TRUE;

// define setup parameters for OpenADC10
//            Turn module on | ouput in integer | trigger mode auto | enable autosample
#define PARAM1 ADC_MODULE_ON | ADC_FORMAT_INTG  | ADC_CLK_AUTO      | ADC_AUTO_SAMPLING_ON
//             ADC ref are VDD/VSS | disable offset test    | disable scan mode | perform 2 samples     | use dual buffers | use alternate mode
#define PARAM2 ADC_VREF_AVDD_AVSS  | ADC_OFFSET_CAL_DISABLE | ADC_SCAN_OFF      | ADC_SAMPLES_PER_INT_2 | ADC_ALT_BUF_ON   | ADC_ALT_INPUT_ON
//             use PB clock    | set sample time
#define PARAM3 ADC_CONV_CLK_INTERNAL_RC | ADC_SAMPLE_TIME_15        // sample time to be checked
//              set AN4, AN.., AN.. and AN.. as analog inputs for the 4 tensions
#define configport ENABLE_AN4_ANA //| ENABLE_AN5_ANA | ENABLE_AN2_ANA | ENABLE_AN3_ANA
// do not assign channels to scan
#define configscan SKIP_SCAN_ALL

void ForceSensorsADC_generalinit()
{
    CloseADC10();	// ensure the ADC is off before setting the configuration
    SetChanADC10( ADC_CH0_NEG_SAMPLEA_NVREF   | ADC_CH0_POS_SAMPLEA_AN4);// | ADC_CH0_NEG_SAMPLEB_NVREF   | ADC_CH0_POS_SAMPLEB_AN5); // configure to sample AN4 & AN5
    // Open ADC10 with parameters 1 to 5
    #if defined (__32MX460F512L__) || defined (__32MX360F512L__) || defined (__32MX795F512L__)                  //<-- ours is __32MX795F512L__
    //OpenADC10( PARAM1, PARAM2, PARAM3, configport, configscan );    // configure ADC using the parameters defined above
    #elif defined (__32MX220F032D__) || defined (__32MX250F128D__)
    AD1CHS = 0x05040000;
    AD1CON1 = 0xE4;
    AD1CON2 = 0x7;
    AD1CON3 = 0x8F00;
    ANSELBbits.ANSB3 = 1; //Set AN5 to analog, so not the good one for our case
    ANSELBbits.ANSB2 = 1; //Set AN4 to analog, neither, not our problem now
    #endif
    OpenADC10( PARAM1, PARAM2, PARAM3, configport, configscan );    // configure ADC using the parameters defined above
    EnableADC10(); // Enable the ADC


    PORTAbits.RA2=0;
    PORTAbits.RA3=0;
    OpenI2C2( I2C_EN, (80000000/2/50000)-2 );
}

/*
void ForceSensorsADC_setAN4andAN5( void )
{
    // configure to sample AN4 on MUXA and AN5 on MUXB
    //            use ground as neg ref for A | use AN4 for input A    // | use ground as neg ref for B | use AN5 for input B
    SetChanADC10( ADC_CH0_NEG_SAMPLEA_NVREF   | ADC_CH0_POS_SAMPLEA_AN4);// | ADC_CH0_NEG_SAMPLEB_NVREF   | ADC_CH0_POS_SAMPLEB_AN5); // configure to sample AN4 & AN5
}
*/

void ForceSensorsADC_readAllChannels( void )      //values read as integers, might be called in an interrupt to have a nice sampling periode
{
    //starts with AN4 and AN5
    //ForceSensorsADC_setAN4andAN5();
    while ( ! mAD1GetIntFlag() ){/*wait for the first conversion to complete so there will be vaild data in ADC result registers*/}
    unsigned int offset = 8 * ((~ReadActiveBufferADC10() & 0x01));
    
    // sliding mean of 100 values
    essai = ReadADC10(offset);
    sensorValue = (sensorValue * 99 + essai)/100;             // read the result of channel 0 conversion
    
    //sensorValues[0] = ReadADC10(0);       // read the result of channel 0 conversion
    //sensorValues[1] = ReadADC10(1);       // read the result of channel 1 conversion
    #if defined (__32MX460F512L__) || defined (__32MX360F512L__) || defined (__32MX795F512L__)
    mAD1ClearIntFlag();
    #elif defined (__32MX220F032D__) || defined (__32MX250F128D__)
    IFS0bits.AD1IF =0;
    #endif
}

void manageForceSensor(char** arg,int argc)
{

    if (argc == 1)
    {
        if (!strcmp("gFORCE",arg[0]))
        {
            sendForce();
        }
        else if (!strcmp("ENABLE", arg[0])) forceSensorEnable = TRUE;
        else if (!strcmp("DISABLE", arg[0])) forceSensorEnable = FALSE;
    }

    if ( argc == 3 )
    {
        if ( !strcmp("SETDP", arg[0]) )
        {
            int num = atoi(arg[1]);
            int value = atoi(arg[2]);
            if (value > 255) value = 255;

            unsigned int address = ( num <=2 ) ? 0x2D : 0x2E;
            //equivalent a
            //if ( num <= 2) address = 0x2D;
            //else 0x2E;

            int instruction = ( num%2 ) ? 0x03 : 0x01;

            SendData(value, instruction, address);
        }
    }
}

void AcqForceSensor()
{
    if ( forceSensorEnable )
        ForceSensorsADC_readAllChannels();
}

/*
// Used to get a measure each specific period of time
void __ISR( _TIMER_5_VECTOR ) ForceSensorsHandler( void )
{
    ForceSensorsADC_readAllChannels();
    mT5ClearIntFlag();
}*/



/*****************************************************
 * RcvData(unsigned int address)		     *
 *					  	     *
 * Gets a byte of data from I2C slave device at      *
 *  ADDRESS.					     *
 *						     *
 * Returns: Received data			     *
 ****************************************************/
int RcvData(unsigned int address) {
	StartI2C2();				//Send line start condition
	IdleI2C2();			        //Wait to complete
	MasterWriteI2C2((address << 1) | 1);	//Write out slave address OR 1 (read command)
	IdleI2C2();				//Wait to complete
	int rcv = MasterReadI2C2();		//Read in a value
	StopI2C2();				//Send line stop condition
	IdleI2C2();				//Wait to complete
	return rcv;				//Return read value
}



/***************************************************
 * SendData(int data, unsigned int address)        *
 *						    *
 * Sends a byte of data (DATA) over the I2C line   *
 *	to I2C address ADDRESS			    *
 *						    *
 * Returns: nothing				    *
 ***************************************************/
void SendData (int value, int instruction, unsigned int address){
	StartI2C2();	        //Send the Start Bit
	IdleI2C2();		//Wait to complete

	MasterWriteI2C2((address << 1) | 0);  //Sends the slave address over the I2C line.  This must happen first so the
                                             //proper slave is selected to receive data.
	IdleI2C2();	        //Wait to complete

        MasterWriteI2C2(instruction);  //Sends data byte over I2C line
	IdleI2C2();		//Wait to complete


	MasterWriteI2C2(value);  //Sends data byte over I2C line
	IdleI2C2();		//Wait to complete

	StopI2C2();	        //Send the Stop condition
	IdleI2C2();	        //Wait to complete

} //end function


void sendForce()
{
    char msg[15]="Force\n";
    //sprintf(msg,"F:%u\n", essai);
    sprintf(msg,"F:%f\n", sensorValue);
//    ServerSend(msg);
}