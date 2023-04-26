/*************************************************************************
	ATMEGA128SPI
Author: Sergio Santos 
	<sergio.salazar.santos@gmail.com>
License: GNU General Public License
Hardware: ATmega128
Date: 24042023
Comment:
   Stable
*************************************************************************/
/*** File Library ***/
#include "atmega128mapping.h"

/*** File Constant & Macro ***/
#if defined(__AVR_ATmega64__) || defined(__AVR_ATmega128__) 
	#define ATMEGA_128_SPI
	#define SPI_PIN_MASK 15
	#define DD_SS 0
	#define DD_SCK 1
	#define DD_MOSI 2
	#define DD_MISO 3
#else
	#error "Not Atmega 128"
#endif

/*** File Variable ***/
ATMEGA128 spimega128;

/*** File Header ***/
void spi_default(void);
void spi_transfer_sync (uint8_t * dataout, uint8_t * datain, uint8_t len);
void spi_transmit_sync (uint8_t * dataout, uint8_t len);
uint8_t spi_fast_shift (uint8_t data);

/*** Procedure & Function ***/
SPI SPIenable(uint8_t master_slave_select, uint8_t data_order,  uint8_t data_modes, uint8_t prescaler)
{
	SPI spi;
	spimega128 = ATMEGA128enable();
	spi.transfer_sync = spi_transfer_sync;
	spi.transmit_sync = spi_transmit_sync;
	spi.fast_shift = spi_fast_shift;
	
	spimega128.portb.reg->ddr &= ~((1 << DD_MOSI) | (1 << DD_MISO) | (1 << DD_SS) | (1 << DD_SCK));
	switch(master_slave_select){
		case SPI_MASTER_MODE:
			spimega128.spi.reg->spcr |= (1 << MSTR);
			spimega128.portb.reg->ddr |= (1 << DD_SS) | (1 << DD_MOSI) | (1 << DD_SCK);
			spimega128.portb.reg->port |= (1 << DD_SS);
		break;
		case SPI_SLAVE_MODE:
			spimega128.spi.reg->spcr |= (1 << MSTR);
			spimega128.portb.reg->ddr |= (1 << DD_MISO);
		break;
		default:
			spimega128.spi.reg->spcr |= (1 << MSTR);
			spimega128.portb.reg->ddr |= (1 << DD_SS) | (1 << DD_MOSI) | (1 << DD_SCK);
		break;
	}
	switch(data_order){
		case SPI_LSB_DATA_ORDER:
			spimega128.spi.reg->spcr |= (1 << DORD);
		break;
		case SPI_MSB_DATA_ORDER:
			spimega128.spi.reg->spcr &= ~(1 << DORD);
		break;
		default:
			spimega128.spi.reg->spcr &= ~(1 << DORD);
		break;
	}
	switch(data_modes){
		case 0:
			spimega128.spi.reg->spcr &= ~((1 << CPOL) | (1 << CPHA));
		break;
		case 1:
			spimega128.spi.reg->spcr |= (1 << CPHA);
		break;
		case 2:
			spimega128.spi.reg->spcr |= (1 << CPOL);
		break;
		case 3:
			spimega128.spi.reg->spcr |= (1 << CPOL) | (1 << CPHA);
		break;
		default:
			spimega128.spi.reg->spcr &= ~((1 << CPOL) | (1 << CPHA));
		break;
	}
	switch(prescaler){
		case 2:
			spimega128.spi.reg->spsr |= (1 << SPI2X);
			spimega128.spi.reg->spcr &= ~((1 << SPR1) | (1 << SPR0));
		break;
		case 4:
			spimega128.spi.reg->spsr &= ~(1 << SPI2X);
			spimega128.spi.reg->spcr &= ~((1 << SPR1) | (1 << SPR0));
		break;
		case 8:
			spimega128.spi.reg->spsr |= (1 << SPI2X);
			spimega128.spi.reg->spcr |= (1 << SPR0);
		break;
		case 16:
			spimega128.spi.reg->spsr &= ~(1 << SPI2X);
			spimega128.spi.reg->spcr |= (1 << SPR0);
		break;
		case 32:
			spimega128.spi.reg->spsr |= (1 << SPI2X);
			spimega128.spi.reg->spcr |= (1 << SPR1);
		break;
		case 64:
			spimega128.spi.reg->spsr &= ~(1 << SPI2X);
			spimega128.spi.reg->spcr |= (1 << SPR1);
		break;
		case 128:
			spimega128.spi.reg->spsr &= (1 << SPI2X);
			spimega128.spi.reg->spcr |= (1 << SPR1) | (1 << SPR0);
		break;
		default:
			spimega128.spi.reg->spsr |= (1 << SPI2X);
			spimega128.spi.reg->spcr |= (1 << SPR0);
		break;
	}
	spimega128.spi.reg->spcr |= (1 << SPE);
	return spi;
}
void spi_default()
// Initialize pins for spi communication
{
	spimega128.portb.reg->ddr &= ~((1 << DD_MOSI) | (1 << DD_MISO) | (1 << DD_SS) | (1 << DD_SCK));
	// Define the following pins as output
	spimega128.portb.reg->ddr |= ((1 << DD_MOSI) | (1 << DD_SS) | (1 << DD_SCK)); 
	spimega128.spi.reg->spcr	=	((1 << SPE) |				// SPI Enable
						(0 << SPIE) |				// SPI Interrupt Enable
						(0 << DORD) |				// Data Order (0:MSB first / 1:LSB first)
						(1 << MSTR) |				// Master/Slave select   
						(0 << SPR1) | (1 << SPR0) |	// SPI Clock Rate
						(0 << CPOL) |				// Clock Polarity (0:SCK low / 1:SCK hi when idle)
						(0 << CPHA));				// Clock Phase (0:leading / 1:trailing edge sampling)
    spimega128.spi.reg->spsr	=	(1 << SPI2X);				// Double Clock Rate  
}
void spi_transfer_sync (uint8_t * dataout, uint8_t * datain, uint8_t len)
// Shift full array through target device
{
	uint8_t i;      
	for (i = 0; i < len; i++) {
		spimega128.spi.reg->spdr = dataout[i];
		while((spimega128.spi.reg->spsr & (1 << SPIF)) == 0) ; // polling, serial transfer is complete interrupt.
		datain[i] = spimega128.spi.reg->spdr;
	}
}
void spi_transmit_sync (uint8_t * dataout, uint8_t len)
// Shift full array to target device without receiving any byte
{
	uint8_t i;
	for (i = 0; i < len; i++) {
		spimega128.spi.reg->spdr = dataout[i];
		while((spimega128.spi.reg->spsr & (1 << SPIF)) == 0) ; // polling, serial transfer is complete interrupt.
	}
}
uint8_t spi_fast_shift (uint8_t data)
// Clocks only one byte to target device and returns the received one
{
	spimega128.spi.reg->spdr = data;
	while((spimega128.spi.reg->spsr & (1 << SPIF)) == 0) ; // polling, serial transfer is complete interrupt.
	return spimega128.spi.reg->spdr;
}

/*** File Interrupt ***/
// ISR(SPI_STC_vect){ }

/***EOF***/


/**************************** Comment *******************************
// From data sheet:
void SPI_MasterInit(void)
{
// Set MOSI and SCK output, all others input
DDR_SPI = (1 << DD_MOSI) | (1 << DD_SCK);
// Enable SPI, Master, set clock rate fck/16
SPCR = (1 << SPE) | (1 << MSTR) | (1 << SPR0);
}
void SPI_MasterTransmit(char cData)
{
// Start transmission
SPDR = cData;
// Wait for transmission complete
while(!(SPSR & (1 << SPIF))) ;
}
void SPI_SlaveInit(void)
{
// Set MISO output, all others input
DDR_SPI = (1 << DD_MISO);
// Enable SPI
SPCR = (1 << SPE);
}
char SPI_SlaveReceive(void)
{
// Wait for reception complete
while(!(SPSR & (1 << SPIF))) ;
// Return data register
return SPDR;
}
********************************************************************/

