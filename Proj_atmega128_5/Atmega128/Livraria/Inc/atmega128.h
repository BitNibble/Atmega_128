/********************************************************************
	ATMEGA 128
Author:   <sergio.salazar.santos@gmail.com>
License:  GNU General Public License
Hardware: Atmega 128
Date:	  27092025
********************************************************************/
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
#include <util/atomic.h>
#include <util/delay.h>

/*** Constant & Macro ***/
#define SRAMSTART 0x0100
#define SRAMEND 0x10FF
#define ZERO 0
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

/*******************************************************************/
/********************** MAIN HARDWARE LAYER ************************/
/*******************************************************************/
typedef struct {
	// Indirect Address Register
	Atmega128_GPIAR* const gpiar;
	// Analog Comparator (AC)
	Atmega128_AnalogComparator* const ac;
	// Analog to Digital Converter (ADC)
	Atmega128_AnalogToDigitalConverter* const adc;
	// Boot loader (BOOT_LOAD)
	Atmega128_BootLoader* const bootload;
	// CPU Register (CPU)
	Atmega128_CPURegister* const cpu;
	// EEPROM (EEPROM)
	Atmega128_Eeprom* const eeprom;
	// External Interrupts (EXINT)
	Atmega128_ExternalInterrupt* const exint;
	// I/O Port (PORTA)
	Atmega128_PORTA* const gpioa;
	// I/O Port (PORTB)
	Atmega128_PORTB* const gpiob;
	// I/O Port (PORTC)
	Atmega128_PORTC* const gpioc;
	// I/O Port (PORTD)
	Atmega128_PORTD* const gpiod;
	// I/O Port (PORTE)
	Atmega128_PORTE* const gpioe;
	// I/O Port (PORTF)
	Atmega128_PORTF* const gpiof;
	// I/O Port (PORTG)
	Atmega128_PORTG* const gpiog;
	// JTAG Interface (JTAG)
	Atmega128_JtagInterface* const jtag;
	// Other Registers (MISC)
	Atmega128_OtherRegister* const misc;
	// Serial Peripheral Interface (SPI)
	Atmega128_SerialPeripheralInterface* const spi;
	// Timer/Counter, 16-bit (TC1)
	Atmega128_TimerCounter1* const tc1;
	// Timer/Counter, 16-bit (TC3)
	Atmega128_TimerCounter3* const tc3;
	// Timer/Counter, 8-bit (TC2)
	Atmega128_TimerCounter2* const tc2;
	// Timer/Counter, 8-bit A sync (TC0)
	Atmega128_TimerCounter0* const tc0;
	// Two Wire Serial Interface (TWI)
	Atmega128_TwoWireSerialInterface* const twi;
	// USART (USART0)
	Atmega128_Usart0* const usart0;
	// USART (USART1)
	Atmega128_Usart1* const usart1;
	// Watchdog Timer (WDT)
	Atmega128_WatchdogTimer* const wdt;
} dev_atmega128;

dev_atmega128* dev(void);

/*******************************************************************/
/**************** Procedure and Function declaration ***************/
/*******************************************************************/
uint16_t readHLbyte(U_word reg);
uint16_t readLHbyte(U_word reg);
U_word writeHLbyte(uint16_t val);
U_word writeLHbyte(uint16_t val);
uint16_t swapbyte(uint16_t num);

/*******************************************************************/
/**************** Procedure and Function declaration ***************/
/*******************************************************************/
uint16_t BAUDRATEnormal(uint32_t BAUD);
uint16_t BAUDRATEdouble(uint32_t BAUD);
uint16_t BAUDRATEsynchronous(uint32_t BAUD);
void ClockPrescalerSelect(volatile uint8_t prescaler);
void MoveInterruptsToBoot(void);

/*******************************************************************/
/**************** Procedure and Function declaration ***************/
/*******************************************************************/
void set_reg(volatile uint8_t* reg, uint8_t hbits);
void clear_reg(volatile uint8_t* reg, uint8_t hbits);
uint8_t get_reg_Msk_Pos(uint8_t reg, uint8_t Msk, uint8_t Pos);
void write_reg_Msk_Pos(volatile uint8_t* reg, uint8_t Msk, uint8_t Pos, uint8_t data);
void set_reg_Msk_Pos(volatile uint8_t* reg, uint8_t Msk, uint8_t Pos, uint8_t data);
uint8_t get_reg_Msk(uint8_t reg, uint8_t Msk);
void write_reg_Msk(volatile uint8_t* reg, uint8_t Msk, uint8_t data);
void set_reg_Msk(volatile uint8_t* reg, uint8_t Msk, uint8_t data);
uint8_t get_reg_block(uint8_t reg, uint8_t size_block, uint8_t Pos);
void write_reg_block(volatile uint8_t* reg, uint8_t size_block, uint8_t Pos, uint8_t data);
void set_reg_block(volatile uint8_t* reg, uint8_t size_block, uint8_t Pos, uint8_t data);
uint8_t get_bit_block(volatile uint8_t* reg, uint8_t size_block, uint8_t Pos);
void set_bit_block(volatile uint8_t* reg, uint8_t size_block, uint8_t Pos, uint8_t data);
// --- Generic helpers ---
uint8_t reg_get(uint8_t reg, uint8_t mask);
void reg_set(volatile uint8_t *reg, uint8_t mask, uint8_t data);
/*** NULL Check ***/
int isPtrNull(void* ptr);
int isCharPtrFlush(void* ptr);
/*** Fall Threw Delay ***/
int ftdelayCycles(uint8_t lock_ID, unsigned int n_cycle);
void ftdelayReset(uint8_t ID);

#endif
/*** EOF ***/

