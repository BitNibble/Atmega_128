/**********************************************************************
	ATMEGA 128
Author:   <sergio.salazar.santos@gmail.com>
License:  GNU General Public License
Hardware: Atmega 128
Date:	  27092025
**********************************************************************/
#ifndef _ATMEGA128_H_
	#define _ATMEGA128_H_

 /*** Compiler ***/
#if (__GNUC__ * 100 + __GNUC_MINOR__) < 304
	#error "This library requires AVR-GCC 3.4 or later, update to newer AVR-GCC compiler !"
#endif

/*** Working Frequency ***/
#ifndef F_CPU
	#define F_CPU 16000000UL
#endif

/*** Library ***/
#include <avr/io.h>
#include "atmega128_instance.h"
#include <avr/boot.h>
#include <avr/fuse.h>
#include <avr/wdt.h>
#include <avr/eeprom.h>
#include <avr/pgmspace.h>
#include <avr/sleep.h>
#include <avr/interrupt.h>
#include <util/delay.h>

/*** Constant & Macro ***/
#define SRAMSTART 0x0100
#define SRAMEND 0x10FF
#define ONE 1
#define TWO 2
#define NIBBLE_BITS 4
#define BYTE_BITS 8
#define WORD_BITS 16
#define DWORD_BITS 32
#define QWORD_BITS 64
// Macros for common operations
#define SET_REG(REG, HBITS)		(REG |= HBITS)
#define CLEAR_REG(REG, HBITS)	(REG &= ~HBITS)
#define READ_BIT(REG, BIT)		((REG >> BIT) & 1)
#define TOGGLE_REG(REG, HBITS)	(REG ^= HBITS)

/*****************************/
/**** MAIN HARDWARE LAYER ****/
/*****************************/
dev_atmega128* atmega128(void);

/*********************************************************************/
/**************** Procedure and Function declaration *****************/
/*********************************************************************/
uint16_t readHLbyte(U_word reg);
uint16_t readLHbyte(U_word reg);
U_word writeHLbyte(uint16_t val);
U_word writeLHbyte(uint16_t val);
uint16_t swapbyte(uint16_t num);
uint16_t BAUDRATEnormal(uint32_t BAUD);
uint16_t BAUDRATEdouble(uint32_t BAUD);
uint16_t BAUDRATEsynchronous(uint32_t BAUD);
void ClockPrescalerSelect(volatile uint8_t prescaler);
void MoveInterruptsToBoot(void);

/*********************************************************************/
/**************** Procedure and Function declaration *****************/
/*********************************************************************/
void set_reg(volatile uint8_t* reg, uint8_t hbits);
void clear_reg(volatile uint8_t* reg, uint8_t hbits);
uint8_t get_reg_block(uint8_t reg, uint8_t size_block, uint8_t bit_n);
uint8_t get_reg_Msk(uint8_t reg, uint8_t Msk);
void write_reg_block(volatile uint8_t* reg, uint8_t size_block, uint8_t bit_n, uint8_t data);
void write_reg_Msk(volatile uint8_t* reg, uint8_t Msk, uint8_t data);
void set_reg_block(volatile uint8_t* reg, uint8_t size_block, uint8_t bit_n, uint8_t data);
void set_reg_Msk(volatile uint8_t* reg, uint8_t Msk, uint8_t data);
uint8_t get_bit_block(volatile uint8_t* reg, uint8_t size_block, uint8_t bit_n);
void set_bit_block(volatile uint8_t* reg, uint8_t size_block, uint8_t bit_n, uint8_t data);
/*** NULL Check ***/
int isPtrNull(void* ptr);
int isCharPtrFlush(void* ptr);
/*** Fall Threw Delay ***/
int ftdelayCycles(uint8_t lock_ID, unsigned int n_cycle);
void ftdelayReset(uint8_t ID);

#endif
/*** EOF ***/

