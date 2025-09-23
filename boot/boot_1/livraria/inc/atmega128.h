/**********************************************************************
	ATMEGA 128
Author:   <sergio.salazar.santos@gmail.com>
License:  GNU General Public License
Hardware: Atmega 128
Date:	  22092025
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
#include <avr/boot.h>
#include <avr/fuse.h>
#include <avr/wdt.h>
#include <avr/eeprom.h>
#include <avr/pgmspace.h>
#include <avr/sleep.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "atmega128_instance.h"

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
// GPWR
//Atmega128GPWR_TypeDef* gpwr_reg(void);

// Analog Comparator (AC)
Atmega128AnalogComparator_TypeDef* ac_reg(void);

// Analog to Digital Converter (ADC)
Atmega128AnalogToDigitalConverter_TypeDef* adc_reg(void);

// Boot loader (BOOT_LOAD)
Atmega128BootLoader_TypeDef* bootload_reg(void);

// CPU Register (CPU)
Atmega128CPURegister_TypeDef* cpu_reg(void);

// EEPROM (EEPROM)
Atmega128Eeprom_TypeDef* eeprom_reg(void);

// External Interrupts (EXINT)
Atmega128ExternalInterrupts_TypeDef* exint_reg(void);

// I/O Port (PORTA)
Atmega128PORTA_TypeDef* gpioa_reg(void);

// I/O Port (PORTB)
Atmega128PORTB_TypeDef* gpiob_reg(void);

// I/O Port (PORTC)
Atmega128PORTC_TypeDef* gpioc_reg(void);

// I/O Port (PORTD)
Atmega128PORTD_TypeDef* gpiod_reg(void);

// I/O Port (PORTE)
Atmega128PORTE_TypeDef* gpioe_reg(void);

// I/O Port (PORTF)
Atmega128PORTF_TypeDef* gpiof_reg(void);

// I/O Port (PORTG)
Atmega128PORTG_TypeDef* gpiog_reg(void);

// JTAG Interface (JTAG)
Atmega128JtagInterface_TypeDef* jtag_reg(void);

// JTAG Interface (JTAG)
Atmega128JtagInterfaceControlStatus_TypeDef* jtag_cs_reg(void);

// Other Registers (MISC)
Atmega128OtherRegisters_TypeDef* misc_reg(void);

// Serial Peripheral Interface (SPI)
Atmega128SerialPeripheralInterface_TypeDef* spi_reg(void);

// Timer/Counter, 16-bit (TC1)
Atmega128TimerCounter1_TypeDef* tc1_reg(void);

// Timer/Counter, 16-bit (TC3)
Atmega128TimerCounter3_TypeDef* tc3_reg(void);

// Timer/Counter 1 and 3
Atmega128TimerExternalInterruptFlag_TypeDef* tc_exif_reg(void);

// Timer/Counter 1 and 3
Atmega128TimerExternalInterruptMask_TypeDef* tc_exim_reg(void);

// Timer/Counter, 8-bit (TC2)
Atmega128TimerCounter2_TypeDef* tc2_reg(void);

// Timer/Counter, 8-bit A sync (TC0)
Atmega128TimerCounter0_TypeDef* tc0_reg(void);

// Timer/Counter 0, 1 and 2
Atmega128TimerInterruptFlag_TypeDef* tc_if_reg(void);

// Timer/Counter 0, 1 and 2
Atmega128TimerInterruptMask_TypeDef* tc_im_reg(void);

// Two Wire Serial Interface (TWI)
Atmega128TwoWireSerialInterface_TypeDef* twi_reg(void);

// USART (USART0)
Atmega128Usart0_TypeDef* usart0_reg(void);

// USART (USART1)
Atmega128Usart1_TypeDef* usart1_reg(void);

// Watchdog Timer (WDT)
Atmega128WatchdogTimer_TypeDef* wdt_reg(void);

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

