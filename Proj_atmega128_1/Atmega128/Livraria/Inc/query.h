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
#ifndef _QUERY_H_
	#define _QUERY_H_

#include <stdarg.h>
#include <inttypes.h>
#include <string.h>
#include <math.h>

#ifndef DATA_SIZE
	#define DATA_SIZE 8
#endif
#ifndef DATA_BITS
	#define DATA_BITS 7
#endif

/*** Procedure and Function ToolSet ***/
void set_reg(volatile uint8_t* reg, uint8_t hbits);
void reset_reg(volatile uint8_t* reg, uint8_t hbits);
uint8_t get_reg_block(uint8_t reg, uint8_t size_block, uint8_t bit_n);
void set_reg_block(volatile uint8_t* reg, uint8_t size_block, uint8_t bit_n, uint8_t data);
void write_reg_block(volatile uint8_t* reg, uint8_t size_block, uint8_t bit_n, uint8_t data);
uint8_t get_bit_block(volatile uint8_t* reg, uint8_t size_block, uint8_t bit_n);
void set_bit_block(volatile uint8_t* reg, uint8_t size_block, uint8_t bit_n, uint8_t data);

#endif

/*** EOF ***/

