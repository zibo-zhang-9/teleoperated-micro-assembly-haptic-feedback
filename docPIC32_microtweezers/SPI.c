#include "TCPIP Stack/TCPIP.h"
#include <string.h>
#include <p32xxxx.h>
#include "main.h"
#include "commands.h"
#include  <math.h>



/*********************************************************************
 * Function:        void	SpiInitDevice(SpiChannel chn, int isMaster, int frmEn, int frmMaster)
 *
 * PreCondition:    None
 *
 * Input:           chn			- the SPI channel to use, 1 or 2
 * 					isMaster	-	1: the device is to act as a bus master
 * 								0: the device is an SPI slave
 * 					frmEn		-	1: frame mode is enabled
 * 								0: frame mode is disabled
 * 					frmMaster	-	0: if frame mode is enabled, the device is a frame slave (FRMSYNC is input)
 *                                                              1: if frame mode is enabled, the device is a frame master (FRMSYNC is output)
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        Inits the SPI channel 1 to use 32 bit words
 * 					Performs the device initialization in both master/slave modes.
 *
 * Note:            None
 ********************************************************************/
void SpiInitDevice(SpiChannel chn, int isMaster, int frmEn, int frmMaster)
{
    SpiOpenFlags    oFlags=SPI_OPEN_MODE32|SPI_OPEN_SMP_END|SPI_OPEN_ON|SPI_OPEN_FSP_WIDE;	//8bits per word,  SPI open mode
    if(isMaster)
    {
        oFlags|=SPI_OPEN_MSTEN;
        oFlags|=SPI_OPEN_MSSEN;
    }
    if(frmEn)
    {
        oFlags|=SPI_OPEN_FRMEN;
        if(!frmMaster)
        {
            oFlags|=SPI_OPEN_FSP_IN;
        }
    }
    SpiChnOpen(chn, oFlags, SPIxBRG);	// divide fpb by a lot, configure the I/O ports.

}

void SpiInitViaRegisters(SpiChannel chn, int isMaster, int frmEn, int frmMaster)
{
    //datasheet DS61106G-page 23-9
    unsigned int ConfigReg1 = 0x00000000;       //to be loaded in SPIxCON1
    unsigned int ConfigReg2 = 0x00000000;       //to be loaded in SPIxCON2

    //--------------------------------------------------------------------------
    // SPI1CON register settings [BEGENNING]
    ConfigReg1 |= (0b1<<31);                    //bit 31=1 framed mode enabled, otherwise disabled
                                                //bit 30=0 Frame sync pulse output (Master mode), otherwise input
                                                //bit 29=0 Frame pulse is active-low, otherwise active high
    ConfigReg1 |= (0b1<<28);                    //bit 28=1 The SS pin is automatically driven during transmission in Master mode, otherwise SS driving disabled
    ConfigReg1 |= (0b1<<27);                    //bit 27=1 Frame sync pulse is one word length wide, as defined by MODE<32,16> bits (SPIxCON<11:10>), otherwise 1 clock wide
    ConfigReg1 |= (0b101<<24);                  //bits 26-24=0b101 Generate a frame sync pulse on every 32 data characters
                                                //bit 23=0 PBCLK is used by the Baud Rate Generator
                                                //bits 22-18 Unimplemented: Write ?0?; ignore read
    ConfigReg1 |= (0b1<<17);                    //bit 17=1 Frame synchronization pulse coincides with the first bit clock
                                                //bit 16=0 Enhanced Buffer mode is disabled
    ConfigReg1 |= (0b1<<15);                    //bit 15=1 SPI Peripheral On bit
                                                //bit 14=0 Unimplemented: Write ?0?; ignore read
                                                //bit 13=0 Continue operation in Idle mode, why not ?
                                                //bit 12=0 SDOx pin is controlled by the module
    ConfigReg1 |= (0b11<<10);                   //bit 11-10=0b11 24-bit Data, 32-bit FIFO, 32-bit Channel/64-bit Frame
                                                //bit 9=0 Input data sampled at middle of data output time
    ConfigReg1 |= (0b1<<8);                     //bit 8=1 Serial output data changes on transition from active clock state to idle clock state (see CKP bit)
    ConfigReg1 |= (0b1<<7);                     //bit 7=1 SSx pin used for Slave mode
                                                //bit 6=0 Idle state for clock is a low level; active state is a high level
    ConfigReg1 |= (0b1<<5);                     //bit 5=1 Master Mode
                                                //bit 4=0 SDIx pin is controlled by the SPI module
                                                //bits 3-2=0b00 SPIxTXIF is set when the last transfer is shifted out of SPISR and transmit operations are complete
                                                //bits 1-0=0b00 SPIxRXIF is set when the last word in the receive buffer is read (i.e., buffer is empty)
    // SPI1CON register settings [ENDING]
    //--------------------------------------------------------------------------

    //--------------------------------------------------------------------------
    // SPI1CON2 register settings [BEGENNING]
                                                //bit 31-16 Unimplemented: Write ?0?; ignore read
                                                //bit 15=0 Data from RX FIFO is not sign extened
                                                //bit 14-13 Unimplemented: Write ?0?; ignore read
                                                //bit 12=0 Frame Error does not generate error interrupts
                                                //bit 11=0 Receive overflow does not generate error interrupts
                                                //bit 10=0 Transmit Underrun does not generate error interrupts
    ConfigReg2 |= (0b1<<9);                     //bit 9=1 A ROV is not a critical error; during ROV data in the FIFO is not overwritten by receive data
    ConfigReg2 |= (0b1<<8);                     //bit 8=1 A TUR is not a critical error and zeros are transmitted until the SPIxTXB is not empty
    ConfigReg2 |= (0b1<<7);                     //bit 7=1 Audio protocol enabled
                                                //bits 6-5 Unimplemented: Write ?0?; ignore read
    ConfigReg2 |= (0b1<<3);                     //bit 3=1 Audio data is mono (Each data word is transmitted on both left and right channels)
                                                //bit 2=0 Unimplemented: Write ?0?; ignore read
    ConfigReg2 |= (0b11);                       //bits 1-0=11 PCM/DSP mode
    // SPI1CON2 register settings [ENDING]
    //--------------------------------------------------------------------------

    //SPI1CON2 = ConfigReg2;
    SPI1CON = ConfigReg1;
}
