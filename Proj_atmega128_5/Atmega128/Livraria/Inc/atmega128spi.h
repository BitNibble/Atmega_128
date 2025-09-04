/**********************************************************************
	ATMEGA128SPI
Author:   <sergio.salazar.santos@gmail.com>
Hardware: Atmega128 by ETT ET-BASE
Date:     04/09/2025
**********************************************************************/
#ifndef _ATMEGA128SPI_H_
	#define _ATMEGA128SPI_H_

/*** Library ***/
#include "atmega128.h"

/*** Constant & Macro ***/
#if defined(__AVR_ATmega64__) || defined(__AVR_ATmega128__)
#else
	#error "Not Atmega 128"
#endif

#define SPI_SERIAL_TRANSFER_COMPLETE SPI_STC_vect

#define SPI_LSB_DATA_ORDER 1
#define SPI_MSB_DATA_ORDER 0
#define SPI_MASTER_MODE 1
#define SPI_SLAVE_MODE 0
#define SPI_PIN_MASK 15
#define DD_SS 0
#define DD_SCK 1
#define DD_MOSI 2
#define DD_MISO 3

/*** Callback ***/
typedef struct {
	void (*stc)(void);
}SPI0_Callback;

/*** Handler ***/
typedef struct{
	SPI0_Callback callback;
	
	// V-table
	void (*transfer_sync) (uint8_t * dataout, uint8_t * datain, uint8_t len);
	void (*transmit_sync) (uint8_t * dataout, uint8_t len);
	uint8_t (*fast_shift) (uint8_t data);
}SPI0_Handler;

void spi_enable(uint8_t master_slave_select, uint8_t data_order,  uint8_t data_modes, uint8_t prescaler);
SPI0_Handler* spi(void);

#endif
/*** EOF ***/

