/************************************************************************
	ATMEGA128SPI
Author: Sergio Santos 
	<sergio.salazar.santos@gmail.com>
Hardware: ATmega128
Date: 24042023
Comment:
	Stable
************************************************************************/
#ifndef _ATMEGA128SPI_H_
	#define _ATMEGA128SPI_H_

/*** Global Library ***/
#include <inttypes.h>

/*** Global Constant & Macro ***/
#define SPI_LSB_DATA_ORDER 1
#define SPI_MSB_DATA_ORDER 0
#define SPI_MASTER_MODE 1
#define SPI_SLAVE_MODE 0

/*** Global Variable ***/
struct sp{
	void (*transfer_sync) (uint8_t * dataout, uint8_t * datain, uint8_t len);
	void (*transmit_sync) (uint8_t * dataout, uint8_t len);
	uint8_t (*fast_shift) (uint8_t data);
};
typedef struct sp SPI;

/*** Global Header ***/
SPI SPIenable(uint8_t master_slave_select, uint8_t data_order,  uint8_t data_modes, uint8_t prescaler);

#endif
/***EOF***/

