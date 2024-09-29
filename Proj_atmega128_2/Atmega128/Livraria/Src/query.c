/*********************************************************
	ATMEGA Query
Author: Sergio Manuel Santos
	<sergio.salazar.santos@gmail.com>
License: GNU General Public License
Hardware: Atmega 128
Update:	14/01/2024
Comment: 
	Simple tools
**********************************************************/
#include "query.h"

#define BYTE_BITS 8
#define WORD_BITS 16
#define N_BITS 8
#define N_LIMBITS 9
#define H_BIT 7
#define L_BIT 0

/*** Procedure & Function ToolSet ***/
inline void set_reg(volatile uint8_t* reg, uint8_t hbits){
	*reg |= hbits;
}
inline void clear_reg(volatile uint8_t* reg, uint8_t hbits){
	*reg &= ~hbits;
}
inline uint8_t get_reg_Msk(uint8_t reg, uint8_t Msk, uint8_t Pos)
{
	if(Pos > H_BIT){ Pos = L_BIT; reg = 0; }
	else{ reg &= Msk; reg = (reg >> Pos); }
	return reg;
}
inline void write_reg_Msk(volatile uint8_t* reg, uint8_t Msk, uint8_t Pos, uint8_t data)
{
	uint8_t value = *reg;
	if(Pos > H_BIT){ Pos = L_BIT; }
	else{ data = (data << Pos); data &= Msk; value &= ~(Msk); value |= data; *reg = value; }
}
inline void set_reg_Msk(volatile uint8_t* reg, uint8_t Msk, uint8_t Pos, uint8_t data)
{
	if(Pos > H_BIT){ Pos = L_BIT; }
	else{ data = (data << Pos); data &= Msk; *reg &= ~(Msk); *reg |= data; }
}
uint8_t get_reg_block(uint8_t reg, uint8_t size_block, uint8_t bit_n)
{
	if(size_block > N_BITS){ size_block = N_BITS; }
	if(bit_n > H_BIT){ bit_n = L_BIT; reg = 0; }
	else{
		uint8_t mask = (uint8_t)((1 << size_block) - 1);
		reg &= (mask << bit_n);
		reg = (reg >> bit_n);
	}
	return reg;
}
void write_reg_block(volatile uint8_t* reg, uint8_t size_block, uint8_t bit_n, uint8_t data)
{
	if(size_block > N_BITS){ size_block = N_BITS; }
	if(bit_n > H_BIT){ bit_n = H_BIT; }
	else{
		uint8_t value = *reg;
		uint8_t mask = (uint8_t)((1 << size_block) - 1);
		data &= mask; value &= ~(mask << bit_n);
		data = (data << bit_n);
		value |= data;
		*reg = value;
	}
}
void set_reg_block(volatile uint8_t* reg, uint8_t size_block, uint8_t bit_n, uint8_t data)
{
	if(size_block > N_BITS){ size_block = N_BITS; }
	if(bit_n > H_BIT){ bit_n = H_BIT; }
	else{
		uint8_t mask = (uint8_t)((1 << size_block) - 1);
		data &= mask;
		*reg &= ~(mask << bit_n);
		*reg |= (data << bit_n);
	}
}
uint8_t get_bit_block(volatile uint8_t* reg, uint8_t size_block, uint8_t bit_n)
{
	uint8_t value;
	if(size_block > N_BITS){ size_block = N_BITS; }
	uint8_t n = bit_n / N_BITS; bit_n = bit_n % N_BITS;
	value = *(reg + n );
	uint8_t mask = (uint8_t)((1 << size_block) - 1);
	value &= (mask << bit_n);
	value = (value >> bit_n);
	return value;
}
void set_bit_block(volatile uint8_t* reg, uint8_t size_block, uint8_t bit_n, uint8_t data)
{
	if(size_block > N_BITS){ size_block = N_BITS; }
	uint8_t n = bit_n / N_BITS; bit_n = bit_n % N_BITS;
	uint8_t mask = (uint8_t)((1 << size_block) - 1);
	data &= mask;
	*(reg + n ) &= ~(mask << bit_n);
	*(reg + n ) |= (data << bit_n);
}
void STM32446SetRegBits( uint8_t* reg, uint8_t n_bits, ... )
{
	uint8_t i;
	if(n_bits > L_BIT && n_bits < N_LIMBITS){ // Filter input
		va_list list;
		va_start(list, n_bits);
		for(i = 0; i < n_bits; i++){
			*reg |= (uint8_t)(1 << va_arg(list, uint32_t));
		}
		va_end(list);
	}
}
void STM32446ResetRegBits( uint8_t* reg, uint8_t n_bits, ... )
{
	uint8_t i;
	if(n_bits > L_BIT && n_bits < N_LIMBITS){ // Filter input
		va_list list;
		va_start(list, n_bits);
		for(i = 0; i < n_bits; i++){
			*reg &= (uint8_t)~((1 << va_arg(list, uint8_t)) << WORD_BITS);
		}
		va_end(list);
	}
}
void STM32446VecSetup( volatile uint8_t vec[], unsigned int size_block, unsigned int block_n, unsigned int data )
{
	const unsigned int n_bits = sizeof(data) * BYTE_BITS;
	if(size_block > n_bits){ size_block = n_bits; }
	const unsigned int mask = (uint8_t) ((1 << size_block) - 1);
	unsigned int index = (block_n * size_block) / n_bits;
	data &= mask;
	vec[index] &= ~( mask << ((block_n * size_block) - (index * n_bits)) );
	vec[index] |= ( data << ((block_n * size_block) - (index * n_bits)) );
}

/******
n = bit_n/32; bit_n = bit_n - (n * 32); -> bit_n = bit_n % 32;
(a+b)/a = a/a + b/a = 1 + b/a
a/b/c/d/e = a/(b*c*d*e)
*******/

/*** EOF ***/

