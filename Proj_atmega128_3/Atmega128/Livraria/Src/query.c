/*********************************************************
	ATMEGA Query
Author: Sergio Manuel Santos
	<sergio.salazar.santos@gmail.com>
License: GNU General Public License
Hardware: Atmega
Update:	14/01/2024
Comment: 
	Simple tools
**********************************************************/
#include "query.h"

/*** Procedure & Function ToolSet ***/
uint8_t getreg(uint8_t reg, uint8_t size_block, uint8_t bit_n)
{
	if(bit_n > DATA_BITS){ bit_n = 0;} if(size_block > DATA_SIZE){ size_block = DATA_SIZE;}
	uint8_t mask = (unsigned int)((1 << size_block) - 1);
	reg &= (mask << bit_n);
	reg = (reg >> bit_n);
	return reg;
}
void setreg(volatile uint8_t* reg, uint8_t size_block, uint8_t bit_n, uint8_t data)
{
	if(bit_n > DATA_BITS){ bit_n = 0;} if(size_block > DATA_SIZE){ size_block = DATA_SIZE;}
	uint8_t mask = (unsigned int)((1 << size_block) - 1);
	data &= mask;
	*reg &= ~(mask << bit_n);
	*reg |= (data << bit_n);
}
void writereg(volatile uint8_t* reg, uint8_t size_block, uint8_t bit_n, uint8_t data)
{
	if(bit_n > DATA_BITS){ bit_n = 0;} if(size_block > DATA_SIZE){ size_block = DATA_SIZE;}
	uint8_t value = *reg; uint8_t mask = (unsigned int)((1 << size_block) - 1);
	data &= mask; value &= ~(mask << bit_n);
	data = (data << bit_n);
	value |= data;
	*reg = value;
}
uint8_t getbit(volatile uint8_t* reg, uint8_t size_block, uint8_t bit_n)
{
	uint8_t n = 0;
	if(bit_n > DATA_BITS){ n = bit_n/DATA_SIZE; bit_n = bit_n - (n * DATA_SIZE); } if(size_block > DATA_SIZE){ size_block = DATA_SIZE;}
	uint8_t value = *(reg + n ); uint8_t mask = (unsigned int)((1 << size_block) - 1);
	value &= (mask << bit_n);
	value = (value >> bit_n);
	return value;
}
void setbit(volatile uint8_t* reg, uint8_t size_block, uint8_t bit_n, uint8_t data)
{
	uint8_t n = 0;
	if(bit_n > DATA_BITS){ n = bit_n/DATA_SIZE; bit_n = bit_n - (n * DATA_SIZE); } if(size_block > DATA_SIZE){ size_block = DATA_SIZE;}
	uint8_t mask = (unsigned int)((1 << size_block) - 1);
	data &= mask;
	*(reg + n ) &= ~(mask << bit_n);
	*(reg + n ) |= (data << bit_n);
}

/*** EOF ***/


