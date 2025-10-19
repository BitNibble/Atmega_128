/********************************************************************
	ATMEGA 128 INSTANCE
Author:   <sergio.salazar.santos@gmail.com>
License:  GNU General Public License
Hardware: Atmega 128
Date:	  27092025
********************************************************************/
#ifndef _ATMEGA128_INSTANCE_H_
	#define _ATMEGA128_INSTANCE_H_

/*** Library ***/
#include "atmega128_register.h"
#include <avr/sfr_defs.h>

/*** Define & Macro ***/
#define REG_GAP_NAME2(x, y) x##y
#define REG_GAP_NAME(x, y) REG_GAP_NAME2(x, y)
#define REG_GAP(start, end) uint8_t REG_GAP_NAME(_r, __LINE__)[(end - start) - 1]

/*****************************/
/**** MAIN HARDWARE LAYER ****/
/*****************************/
// GPWR
typedef volatile struct {
	uint8_t r0; // 0x00
	uint8_t r1; // 0x01
	uint8_t r2; // 0x02
	uint8_t r3; // 0x03
	uint8_t r4; // 0x04
	uint8_t r5; // 0x05
	uint8_t r6; // 0x06
	uint8_t r7; // 0x07
	uint8_t r8; // 0x08
	uint8_t r9; // 0x09
	uint8_t r10; // 0x0A
	uint8_t r11; // 0x0B
	uint8_t r12; // 0x0C
	uint8_t r13; // 0x0D
	uint8_t r14; // 0x0E
	uint8_t r15; // 0x0F
	uint8_t r16; // 0x10
	uint8_t r17; // 0x11
	uint8_t r18; // 0x12
	uint8_t r19; // 0x13
	uint8_t r20; // 0x14
	uint8_t r21; // 0x15
	uint8_t r22; // 0x16
	uint8_t r23; // 0x17
	uint8_t r24; // 0x18
	uint8_t r25; // 0x19
	uint16_t X; // 0x1A 0x1B
	uint16_t Y; // 0x1C 0x1D
	uint16_t Z; // 0x1E 0x1F
} Atmega128_GPWR;

// Indirect Address Register
typedef volatile struct {
	uint16_t X; // 0x1A 0x1B
	uint16_t Y; // 0x1C 0x1D
	uint16_t Z; // 0x1E 0x1F
} Atmega128_GPIAR;

// Analog Comparator (AC)
typedef volatile struct {
	ACSR_TypeDef acsr; // 0x28
	uint8_t fill[23]; // (40 - 28) - 1
	SFIOR_TypeDef sfior; // 0x40
} Atmega128_AnalogComparator;

// Analog to Digital Converter (ADC)
typedef volatile struct {
	U_word adc; // 0x24 0x25
	ADCSRA_TypeDef adcsra; // 0x26
	ADMUX_TypeDef admux; // 0x27
} Atmega128_AnalogToDigitalConverter;

// Boot loader (BOOT_LOAD)
typedef volatile struct {
	SPMCSR_TypeDef spmcsr; // 0x68
} Atmega128_BootLoader;

// CPU Register (CPU)
typedef volatile struct {
	MCUCSR_TypeDef mcucsr; // 0x54
	MCUCR_TypeDef mcucr; // 0x55
	uint8_t fill1[5]; // (5B - 55) - 1
	RAMPZ_TypeDef rampz; // 0x5B
	XDIV_TypeDef xdiv; // 0x5C
	U_word sp; // 0x5D 0x5E
	SREG_TypeDef sreg; // 0x5F
	uint8_t fill2[12]; // (6C - 5F) - 1
	XMCRB_TypeDef xmcrb; // 0x6C
	XMCRA_TypeDef xmcra; // 0x6D
	uint8_t fill3; // (6F - 6D) - 1
	U_byte osccal; // 0x6F
} Atmega128_CPURegister;

// EEPROM (EEPROM)
typedef volatile struct {
	EECR_TypeDef eecr; // 0x3C
	U_byte eedr; // 0x3D
	U_word eear; // 0x3E 0x3F
} Atmega128_Eeprom;

// External Interrupts (EXINT)
typedef volatile struct {
	EIFR_TypeDef eifr; // 0x58
	EIMSK_TypeDef eimsk; // 0x59
	EICRB_TypeDef eicrb; // 0x5A
	uint8_t fill[15]; // (6A - 5A) - 1
	EICRA_TypeDef eicra; // 0x6A
} Atmega128_ExternalInterrupt;

// I/O Port (PORTA)
typedef volatile struct {
	U_byte pin; // 0x39
	U_byte ddr; // 0x3A
	U_byte port; // 0x3B
} Atmega128_PORTA;

// I/O Port (PORTB)
typedef volatile struct {
	U_byte pin; // 0x36
	U_byte ddr; // 0x37
	U_byte port; // 0x38
} Atmega128_PORTB;

// I/O Port (PORTC)
typedef volatile struct {
	U_byte pin; // 0x33
	U_byte ddr; // 0x34
	U_byte port; // 0x35
} Atmega128_PORTC;

// I/O Port (PORTD)
typedef volatile struct {
	U_byte pin; // 0x30
	U_byte ddr; // 0x31
	U_byte port; // 0x32
} Atmega128_PORTD;

// I/O Port (PORTE)
typedef volatile struct {
	U_byte pin; // 0x21
	U_byte ddr; // 0x22
	U_byte port; // 0x23
} Atmega128_PORTE;

// I/O Port (PORTF)
typedef volatile struct {
	U_byte  pin; // 0x20
	uint8_t fill[64]; // (0x61 - 0x20) - 1
	U_byte ddr; // 0x61
	U_byte port; // 0x62
} Atmega128_PORTF;

// I/O Port (PORTG)
typedef volatile struct {
	U_byte pin; // 0x63
	U_byte ddr; // 0x64
	U_byte port; // 0x65
} Atmega128_PORTG;

// JTAG Interface (JTAG)
typedef volatile struct {
	U_byte ocdr; // 0x42
	uint8_t fill[17]; // (54 - 42) - 1
	MCUCSR_TypeDef mcucsr; // 0x54
} Atmega128_JtagInterface;

// Other Registers (MISC)
typedef volatile struct {
	SFIOR_TypeDef sfior; // 0x40
} Atmega128_OtherRegister;

// Serial Peripheral Interface (SPI)
typedef volatile struct {
	SPCR_TypeDef spcr; // 0x2D
	SPSR_TypeDef spsr; // 0x2E
	U_byte spdr; // 0x2F
} Atmega128_SerialPeripheralInterface;

// Timer/Counter, 16-bit (TC1)
typedef volatile struct {
	SFIOR_TypeDef sfior; // 0x40
	uint8_t fill1[5]; // (46 - 40) - 1
	U_word icr1; // 0x46 0x47
	U_word ocr1b; // 0x48 0x49
	U_word ocr1a; // 0x4A 0x4B
	U_word tcnt1; // 0x4C 0x4D
	TCCR1B_TypeDef tccr1b; // 0x4E
	TCCR1A_TypeDef tccr1a; // 0x4F
	uint8_t fill2[6]; // (56 - 4F) - 1
	TIFR_TypeDef tifr; // 0x56
	TIMSK_TypeDef timsk; // 0x57
	uint8_t fill3[32]; // (78 - 57) - 1
	U_word ocr1c; // 0x78 0x79
	TCCR1C_TypeDef tccr1c; // 0x7A
	uint8_t fill4; // (7C - 7A) - 1
	ETIFR_TypeDef etifr; // 0x7C
	ETIMSK_TypeDef etimsk; // 0x7D
} Atmega128_TimerCounter1;

// Timer/Counter, 16-bit (TC3)
typedef volatile struct {
	SFIOR_TypeDef sfior; // 0x40
	uint8_t fill1[59]; // (7c - 40) - 1
	ETIFR_TypeDef etifr; // 0x7C
	ETIMSK_TypeDef etimsk; // 0x7D
	uint8_t fill2[2]; // (80 - 7D) - 1
	U_word icr3; // 0x80 0x81
	U_word ocr3c; // 0x82 0x83
	U_word ocr3b; // 0x84 0x85
	U_word ocr3a; // 0x86 0x87
	U_word tcnt3; // 0x88 0x89
	TCCR3B_TypeDef tccr3b; // 0x8A
	TCCR3A_TypeDef tccr3a; // 0x8B
	TCCR3C_TypeDef tccr3c; // 0x8C
} Atmega128_TimerCounter3;

// Timer/Counter, 8-bit (TC2)
typedef volatile struct {
	U_byte ocr2; // 0x43
	U_byte tcnt2; // 0x44
	TCCR2_TypeDef tccr2; // 0x45
	uint8_t fill[16]; // (56 - 45) - 1
	TIFR_TypeDef tifr; // 0x56
	TIMSK_TypeDef timsk; // 0x57
} Atmega128_TimerCounter2;

// Timer/Counter, 8-bit A sync (TC0)
typedef volatile struct {
	SFIOR_TypeDef sfior; // 0x40
	uint8_t fill1[15]; // (50 - 40) - 1
	ASSR_TypeDef assr; // 0x50
	U_byte ocr0; // 0x51
	U_byte tcnt0; // 0x52
	TCCR0_TypeDef tccr0; // 0x53
	uint8_t fill2[2]; // (56 - 53) - 1
	TIFR_TypeDef tifr; // 0x56
	TIMSK_TypeDef timsk; // 0x57
} Atmega128_TimerCounter0;

// Two Wire Serial Interface (TWI)
typedef volatile struct {
	U_byte twbr; // 0x70
	TWSR_TypeDef twsr; // 0x71
	TWAR_TypeDef twar; // 0x72
	U_byte twdr; // 0x73
	TWCR_TypeDef twcr; // 0x74
} Atmega128_TwoWireSerialInterface;

// USART (USART0)
typedef volatile struct {
	U_byte ubrr0l; // 0x29
	UCSR0B_TypeDef ucsr0b; // 0x2A
	UCSR0A_TypeDef ucsr0a; // 0x2B
	U_byte udr0; // 0x2C
	uint8_t fill1[99]; // (90 - 2C) - 1
	UBRR0H_TypeDef ubrr0h; // 0x90
	uint8_t fill2[4]; // (95 - 90) - 1
	UCSR0C_TypeDef ucsr0c; // 0x95
} Atmega128_Usart0;

// USART (USART1)
typedef volatile struct {
	UBRR1H_TypeDef ubrr1h; // 0x98
	U_byte ubrr1l; // 0x99
	UCSR1B_TypeDef ucsr1b; // 0x9A
	UCSR1A_TypeDef ucsr1a; // 0x9B
	U_byte udr1; // 0x9C
	UCSR1C_TypeDef ucsr1c; // 0x9D
} Atmega128_Usart1;

// Watchdog Timer (WDT)
typedef volatile struct {
	WDTCR_TypeDef wdtcr; // 0x41
} Atmega128_WatchdogTimer;

#endif
/*** EOF ***/

