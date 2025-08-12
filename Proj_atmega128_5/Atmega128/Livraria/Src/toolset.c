/**********************************************************************
	TOOLSET
Author:   <sergio.salazar.santos@gmail.com>
License:  GNU General Public License
Hardware: ALL
Update:	  12082025
**********************************************************************/
#include "toolset.h"
#include <stdarg.h>

/*****************************/
/****       MACROS        ****/
/*****************************/
#define FTDELAY_SIZE 256
static unsigned int ft_Delay_Lock[FTDELAY_SIZE] = {0};
static unsigned int ftCounter[FTDELAY_SIZE] = {0};

/*********************************************************************/
/***************** Procedure and Function definition *****************/
/*********************************************************************/
inline uint8_t Msk_Pos(uint8_t Msk){
	uint8_t Pos = 0;
	if( Msk ){
		for( ; !(Msk & 1); Msk >>= 1, Pos++ );
	}
	return Pos;
}
inline void set_reg(volatile uint8_t* reg, uint8_t hbits){
	*reg |= hbits;
}
inline void clear_reg(volatile uint8_t* reg, uint8_t hbits){
	*reg &= ~hbits;
}
inline uint8_t get_reg_Msk(uint8_t reg, uint8_t Msk)
{
	reg = (reg & Msk) >> Msk_Pos(Msk);
	return reg;
}
inline void write_reg_Msk(volatile uint8_t* reg, uint8_t Msk, uint8_t data)
{
	uint8_t value = *reg;
	uint8_t Pos = Msk_Pos(Msk);
	data = (data << Pos) & Msk; value &= ~(Msk); value |= data; *reg = value;
}
inline void set_reg_Msk(volatile uint8_t* reg, uint8_t Msk, uint8_t data)
{
	uint8_t Pos = Msk_Pos(Msk);
	data = (data << Pos) & Msk; *reg &= ~(Msk); *reg |= data;
}
uint8_t get_reg_block(uint8_t reg, uint8_t size_block, uint8_t bit_n)
{
	if(bit_n < BYTE_BITS &&  size_block != 0 && bit_n + size_block <= BYTE_BITS) {
		uint8_t mask = (uint8_t)((1U << size_block) - 1);
		reg = (reg & (mask << bit_n)) >> bit_n;
	}
	return reg;
}
void write_reg_block(volatile uint8_t* reg, uint8_t size_block, uint8_t bit_n, uint8_t data)
{
	uint8_t value = *reg;
	if(bit_n < BYTE_BITS &&  size_block != 0 && bit_n + size_block <= BYTE_BITS) {
		uint8_t mask = (uint8_t)((1U << size_block) - 1);
		data &= mask; value &= ~(mask << bit_n);
		data = (data << bit_n);
		value |= data;
		*reg = value;
	}
}
void set_reg_block(volatile uint8_t* reg, uint8_t size_block, uint8_t bit_n, uint8_t data)
{
	if(bit_n < BYTE_BITS &&  size_block != 0 && bit_n + size_block <= BYTE_BITS) {
		uint8_t mask = (uint8_t)((1U << size_block) - 1);
		data &= mask;
		*reg &= ~(mask << bit_n);
		*reg |= (data << bit_n);
	}
}
uint8_t get_bit_block(volatile uint8_t* reg, uint8_t size_block, uint8_t bit_n)
{
	uint8_t value;
	uint8_t n = bit_n / BYTE_BITS; bit_n = bit_n % BYTE_BITS;
	value = *(reg + n );
	if(size_block != 0 && bit_n + size_block <= BYTE_BITS){
		uint8_t mask = (uint8_t)((1U << size_block) - 1);
		value = (value & (mask << bit_n)) >> bit_n;
	}
	return value;
}
void set_bit_block(volatile uint8_t* reg, uint8_t size_block, uint8_t bit_n, uint8_t data)
{
	uint8_t n = bit_n / BYTE_BITS; bit_n = bit_n % BYTE_BITS;
	if(size_block != 0 && bit_n + size_block <= BYTE_BITS) {
		uint8_t mask = (uint8_t)((1U << size_block) - 1);
		data &= mask;
		*(reg + n ) &= ~(mask << bit_n);
		*(reg + n ) |= (data << bit_n);
	}
}
void STM32446SetRegBits( uint8_t* reg, uint8_t n_bits, ... )
{
	uint8_t i;
	if(n_bits > 0 && n_bits <= BYTE_BITS){ // Filter input
		va_list list;
		va_start(list, n_bits);
		for(i = 0; i < n_bits; i++){
			*reg |= (uint32_t)(1 << va_arg(list, int));
		}
		va_end(list);
	}
}
void STM32446ResetRegBits( uint8_t* reg, uint8_t n_bits, ... )
{
	uint8_t i;
	if(n_bits > 0 && n_bits <= BYTE_BITS){ // Filter input
		va_list list;
		va_start(list, n_bits);
		for(i = 0; i < n_bits; i++){
			*reg &= (uint32_t)~(1 << va_arg(list, int));
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
/*** NULL Check ***/
int isPtrNull(void* ptr) {
	return ptr ? 0 : 1; // Returns 1 if NULL, 0 otherwise
}
int isCharPtrFlush(void* ptr) {
	// Cast the void pointer to a char pointer to dereference it
	return *((unsigned char*)ptr) ? 0 : 1; // Returns 1 if '\0', 0 otherwise
}
/*** Fall Threw Delay ***/
int ftdelayCycles( uint8_t lock_ID, unsigned int n_cycle ) {
	int ret = 0;
	if( ft_Delay_Lock[lock_ID] != lock_ID) {
		ft_Delay_Lock[lock_ID] = lock_ID;
		ftCounter[lock_ID] = n_cycle;
		}else{
		if( ftCounter[lock_ID]-- );else{ ft_Delay_Lock[lock_ID] = 0; ret = 1; }
	}
	return ret;
}
void ftdelayReset(uint8_t ID) {
	ft_Delay_Lock[ID] = 0;
	ftCounter[ID] = 0;
}

/******
n = bit_n/32; bit_n = bit_n - (n * 32); -> bit_n = bit_n % 32;
(a+b)/a = a/a + b/a = 1 + b/a
a/b/c/d/e = a/(b*c*d*e)
*******/

/*** EOF ***/

