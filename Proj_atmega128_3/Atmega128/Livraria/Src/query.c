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

/*** Procedure & Function Handler***/
Atmega128GPWR_TypeDef* gpwr_handle(void)
{
	return (Atmega128GPWR_TypeDef*) Atmega128GPWR_Address;
}
Atmega128CPURegister_TypeDef* cpu_handle(void)
{
	return (Atmega128CPURegister_TypeDef*) Atmega128CPURegister_Address;
}
Atmega128Usart0_TypeDef* usart0_handle(void)
{
	return (Atmega128Usart0_TypeDef*) Atmega128Usart0_Address;
}
Atmega128Usart1_TypeDef* usart1_handle(void)
{
	return (Atmega128Usart1_TypeDef*) Atmega128Usart1_Address;
}
Atmega128AnalogToDigitalConverter_TypeDef* adc_handle(){
	return (Atmega128AnalogToDigitalConverter_TypeDef*) Atmega128AnalogToDigitalConverter_Address;
}
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
uint16_t readhlbyte(HighLowByte reg)
{
	return (reg.par.H << 8) | reg.par.L;
}
uint16_t readlhbyte(HighLowByte reg)
{
	return (reg.par.L << 8) | reg.par.H;
}
HighLowByte writehlbyte(uint16_t val)
{
	HighLowByte reg; reg.par.H = (val >> 8); reg.par.L = val;
	return reg;
}
HighLowByte writelhbyte(uint16_t val)
{
	HighLowByte reg; reg.par.L = (val >> 8); reg.par.H = val;
	return reg;
}
uint16_t SwapByte(uint16_t num)
{
	uint16_t tp;
	tp = (num << 8);
	return (num >> 8) | tp;
}
uint16_t BAUDRATEnormal(uint32_t BAUD)
{
	uint32_t baudrate = F_CPU/16;
	baudrate /= BAUD; baudrate -= 1;
	return (uint16_t) baudrate;
}
uint16_t BAUDRATEdouble(uint32_t BAUD)
{
	uint32_t baudrate = F_CPU/8;
	baudrate /= BAUD; baudrate -= 1;
	return (uint16_t) baudrate;
}
uint16_t BAUDRATEsynchronous(uint32_t BAUD)
{
	uint32_t baudrate = F_CPU/2;
	baudrate /= BAUD; baudrate -= 1;
	return (uint16_t) baudrate;
}

/*** EOF ***/

