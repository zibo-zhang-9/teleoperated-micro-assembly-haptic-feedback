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

