/*************************************************************************
	74HC595
Author: Sergio Santos
	<sergio.salazar.santos@gmail.com>
License: GNU General Public License     
Hardware: 74HC595
Date: 25/10/2020
Update: 05/01/2024
Comment:
	Tested Atemga88 8Mhz and Atmega328 8Mhz and STM32F446RE
************************************************************************/
/*** File Library ***/
#include "74hc595.h"

/*** File Header ***/
void HC595_shift_bit(hc595_parameter* par, uint8_t bool);
void HC595_shift_ibyte(hc595_parameter* par, uint8_t byte);
void HC595_shift_byte(hc595_parameter* par, uint8_t byte);
void HC595_shift_out(hc595_parameter* par);
hc595_parameter hc595_par_inic(volatile IO_var *ddr, volatile IO_var *port, uint8_t datapin, uint8_t clkpin, uint8_t outpin);

/*** 74HC595 Auxiliar ***/
hc595_parameter hc595_par_inic(volatile IO_var *ddr, volatile IO_var *port, uint8_t datapin, uint8_t clkpin, uint8_t outpin)
{
	hc595_parameter setup_hc595_par;

	setup_hc595_par.hc595_DDR = ddr;
	setup_hc595_par.hc595_PORT = port;
	setup_hc595_par.HC595_datapin = datapin;
	setup_hc595_par.HC595_clkpin = clkpin;
	setup_hc595_par.HC595_outpin = outpin;
	
	#if defined (STM32F446xx)
		*setup_hc595_par.hc595_DDR &= (IO_var) ~((3 << (datapin * 2)) | (3 << (clkpin * 2)) | (3 << (outpin * 2)));
		*setup_hc595_par.hc595_DDR |= ((1 << (datapin * 2)) | (1 << (clkpin * 2)) | (1 << (outpin * 2)));
	#else
		*setup_hc595_par.hc595_DDR |= (1 << datapin) | (1 << clkpin) | (1 << outpin);
	#endif
		*setup_hc595_par.hc595_PORT &= ~((1 << datapin) | (1 << clkpin) | (1 << outpin));

	return setup_hc595_par;
}

/*** 74HC595 Procedure & Function Definition ***/
HC595 hc595_enable(volatile IO_var *ddr, volatile IO_var *port, uint8_t datapin, uint8_t clkpin, uint8_t outpin)
{
	HC595 setup_hc595;

	setup_hc595.par = hc595_par_inic(ddr, port, datapin, clkpin, outpin);
	// Direccionar apontadores para PROTOTIPOS
	setup_hc595.bit = HC595_shift_bit;
	setup_hc595.ibyte = HC595_shift_ibyte;
	setup_hc595.byte = HC595_shift_byte;
	setup_hc595.out = HC595_shift_out;
	
	return setup_hc595;
}

void HC595_shift_bit(hc595_parameter* par, uint8_t bool)
{
	if (bool)
		*par->hc595_PORT |= (1 << par->HC595_datapin); // Data bit HIGH
	else
		*par->hc595_PORT &= ~(1 << par->HC595_datapin); // Data bit LOW
	*par->hc595_PORT |= (1 << par->HC595_clkpin); // Shift bit
	*par->hc595_PORT &= ~(1 << par->HC595_clkpin); //Shift disable
}

void HC595_shift_ibyte(hc595_parameter* par, uint8_t byte)
{
	uint8_t i;
	for(i = 0; i < 8; i++)
		HC595_shift_bit(par, byte & (1 << i));
	HC595_shift_out(par);
}

void HC595_shift_byte(hc595_parameter* par, uint8_t byte)
{
	uint8_t i;
	for(i = 0; i < 8; i++)
		HC595_shift_bit(par, byte & (1 << (7 - i)));
	HC595_shift_out(par);
}

void HC595_shift_out(hc595_parameter* par)
{
	*par->hc595_PORT |= (1 << par->HC595_outpin); // Output enable
	*par->hc595_PORT &= ~(1 << par->HC595_outpin); // Output disable
}

/***EOF***/

