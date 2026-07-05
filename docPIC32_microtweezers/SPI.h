/* 
 * File:   SPI.h
 * Author: Benoit Bauin
 *
 * http://docs-europe.electrocomponents.com/webdocs/10ad/0900766b810ada90.pdf
 *
 * Created on 3 mai 2013, 11:34
 */

/** \file SPI.h
 * Header file containing the function prototypes needed to initialise SPI.
 * \n <b>Source file</b>: \n @ref SPI.c
 * \n <b>Author:</b> \n Benoit Bauin
 * \n <b>Date:</b> \n 3 mai 2013, 11:34
 */

#ifndef SPI_H
#define	SPI_H


/** @brief General function initializing SPI channel. */
void	SpiInitDevice(SpiChannel chn, int isMaster, int frmEn, int frmMaster);

/** @brief Might do the same thing as SpiInitDevice but directly uses registers instead of friendly functions.
 *
 * hasn't been tested as SpiInitDevice, simpler, works.
 */
void SpiInitAnalogDeviceViaRegisters(void);

#ifdef	__cplusplus
extern "C" {
#endif




#ifdef	__cplusplus
}
#endif

#endif	/* SPI_H */

