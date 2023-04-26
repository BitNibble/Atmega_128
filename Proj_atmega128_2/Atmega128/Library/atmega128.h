/************************************************************************
	ATMEGA 128 REGISTERS
Author: Sergio Manuel Santos
	<sergio.salazar.santos@gmail.com>
License: GNU General Public License
Hardware: Atmega128 by ETT ET-BASE
Date: 24/04/2023
Comment:
	Virtual Image Atmega 128.
************************************************************************/
#ifndef _ATMEGA128_H_
	#define _ATMEGA128_H_
	
/*** Compiler ***/
#if (__GNUC__ * 100 + __GNUC_MINOR__) < 304
	#error "This library requires AVR-GCC 3.4 or later, update to newer AVR-GCC compiler !"
#endif

/*** Global Library ***/
#include <inttypes.h>

/*** Global Variable ***/
// Low Byte High Byte
typedef struct {
	uint8_t L; // Lower Address
	uint8_t H; // Higher Address
} HighLowByte;

/*** MAIN HARDWARE LAYER ***/
// GPWR
typedef struct {
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
	uint8_t xl; // 0x1A
	uint8_t xh; // 0x1B
	uint8_t yl; // 0x1C
	uint8_t yh; // 0x1D
	uint8_t zl; // 0x1E
	uint8_t zh; // 0x1F
}Atmega128GPWR_TypeDef;

// Analog Comparator (AC)
typedef struct {
	uint8_t acsr; // 0x28
	uint8_t fill[23]; // (0x40 - 0x28) - 1
	uint8_t sfior; // 0x40
}Atmega128AnalogComparator_TypeDef;

// Analog to Digital Converter (ADC)
typedef struct {
	HighLowByte adc; // 0x24 0x25
	uint8_t adcsra; // 0x26
	uint8_t admux; // 0x27
} Atmega128AnalogToDigitalConverter_TypeDef;

// Boot loader (BOOT_LOAD)
typedef struct {
	uint8_t spmcsr; // 0x68
} Atmega128BootLoader_TypeDef;

// CPU Register (CPU)
typedef struct {
	uint8_t mcucsr; // 0x54
	uint8_t mcucr; // 0x55
	uint8_t fill1[5]; // (5B - 55) - 1
	uint8_t rampz; // 0x5B
	uint8_t xdiv; // 0x5C
	HighLowByte sp; // 0x5D 0x5E
	uint8_t sreg; // 0x5F
	uint8_t fill2[12]; // (6C - 5F) - 1
	uint8_t xmcrb; // 0x6C
	uint8_t xmcra; // 0x6D
	uint8_t fill3[1]; // (6F - 6D) - 1
	uint8_t osccal; // 0x6F
} Atmega128CPURegister_TypeDef;

// EEPROM (EEPROM)
typedef struct {
	uint8_t eecr; // 0x3C
	uint8_t eedr; // 0x3D
	HighLowByte eear; // 0x3E 0x3F
} Atmega128Eeprom_TypeDef;

// External Interrupts (EXINT)
typedef struct {
	uint8_t eifr; // 0x58
	uint8_t eimsk; // 0x59
	uint8_t eicrb; // 0x5A
	uint8_t fill[15]; // (6A - 5A) - 1
	uint8_t eicra; // 0x6A
} Atmega128ExternalInterrupts_TypeDef;

// I/O Port (PORTA)
typedef struct {
	uint8_t pin; // 0x39
	uint8_t ddr; // 0x3A
	uint8_t port; // 0x3B
}Atmega128PORTA_TypeDef;

// I/O Port (PORTB)
typedef struct {
	uint8_t pin; // 0x36
	uint8_t ddr; // 0x37
	uint8_t port; // 0x38
}Atmega128PORTB_TypeDef;

// I/O Port (PORTC)
typedef struct {
	uint8_t pin; // 0x33
	uint8_t ddr; // 0x34
	uint8_t port; // 0x35
}Atmega128PORTC_TypeDef;

// I/O Port (PORTD)
typedef struct {
	uint8_t pin; // 0x30
	uint8_t ddr; // 0x31
	uint8_t port; // 0x32
}Atmega128PORTD_TypeDef;

// I/O Port (PORTE)
typedef struct {
	uint8_t pin; // 0x21
	uint8_t ddr; // 0x22
	uint8_t port; // 0x23
}Atmega128PORTE_TypeDef;

// I/O Port (PORTF)
typedef struct {
	uint8_t pin; // 0x20
	uint8_t fill[64]; // (0x61 - 0x20) - 1
	uint8_t ddr; // 0x61
	uint8_t port; // 0x62
}Atmega128PORTF_TypeDef;

// I/O Port (PORTG)
typedef struct {
	uint8_t pin; // 0x63
	uint8_t ddr; // 0x64
	uint8_t port; // 0x65
} Atmega128PORTG_TypeDef;

// JTAG Interface (JTAG)
typedef struct {
	uint8_t ocdr; // 0x42
	uint8_t fill[17]; // (54 - 42) - 1
	uint8_t mcucsr; // 0x54
} Atmega128JtagInterface_TypeDef;

// Other Registers (MISC)
typedef struct {
	uint8_t sfior; // 0x40
} Atmega128OtherRegisters_TypeDef;

// Serial Peripheral Interface (SPI)
typedef struct {
	uint8_t spcr; // 0x2D
	uint8_t spsr; // 0x2E
	uint8_t spdr; // 0x2F
} Atmega128SerialPeripherialInterface_TypeDef;

// Timer/Counter, 16-bit (TC1)
typedef struct {
	uint8_t sfior; // 0x40
	uint8_t fill1[5]; // (46 - 40) - 1
	HighLowByte icr1; // 0x46 0x47
	HighLowByte ocr1b; // 0x48 0x49
	HighLowByte ocr1a; // 0x4A 0x4B
	HighLowByte tcnt1; // 0x4C 0x4D
	uint8_t tccr1b; // 0x4E
	uint8_t tccr1a; // 0x4F
	uint8_t fill2[6]; // (56 - 4F) - 1
	uint8_t tifr; // 0x56
	uint8_t timsk; // 0x57
	uint8_t fill3[32]; // (78 - 57) - 1
	HighLowByte ocr1c; // 0x78 0x79
	uint8_t tccr1c; // 0x7A
	uint8_t fill4; // (7C - 7A) - 1
	uint8_t etifr; // 0x7C
	uint8_t etimsk; // 0x7D
} Atmega128TimerCounter1_TypeDef;

// Timer/Counter, 16-bit (TC3)
typedef struct {
	uint8_t sfior; // 0x40
	uint8_t fill1[59]; // (7C - 40) - 1
	uint8_t etifr; // 0x7C
	uint8_t etimsk; // 0x7D
	uint8_t fill2[2]; // (80 - 7D) - 1
	HighLowByte icr3; // 0x80 0x81
	HighLowByte ocr3c; // 0x82 0x83
	HighLowByte ocr3b; // 0x84 0x85
	HighLowByte ocr3a; // 0x86 0x87
	HighLowByte tcnt3; // 0x88 0x89
	uint8_t tccr3b; // 0x8A
	uint8_t tccr3a; // 0x8B
	uint8_t tccr3c; // 0x8C
} Atmega128TimerCounter3_TypeDef;

// Timer/Counter, 8-bit (TC2)
typedef struct {
	uint8_t ocr2; // 0x43
	uint8_t tcnt2; // 0x44
	uint8_t tccr2; // 0x45
	uint8_t fill[16]; // (56 - 45) - 1
	uint8_t tifr; // 0x56
	uint8_t timsk; // 0x57
} Atmega128TimerCounter2_TypeDef;

// Timer/Counter, 8-bit A sync (TC0)
typedef struct {
	uint8_t sfior; // 0x40
	uint8_t fill1[15]; // (50 - 40) - 1
	uint8_t assr; // 0x50
	uint8_t ocr0; // 0x51
	uint8_t tcnt0; // 0x52
	uint8_t tccr0; // 0x53
	uint8_t fill2[2]; // (56 - 53) - 1
	uint8_t tifr; // 0x56
	uint8_t timsk; // 0x57
} Atmega128TimerCounter0_TypeDef;

// Two Wire Serial Interface (TWI)
typedef struct {
	uint8_t twbr; // 0x70
	uint8_t twsr; // 0x71
	uint8_t twar; // 0x72
	uint8_t twdr; // 0x73
	uint8_t twcr; // 0x74
} Atmega128TwoWireSerialInterface_TypeDef;

// USART (USART0)
typedef struct {
	uint8_t ubrr0l; // 0x29
	uint8_t ucsr0b; // 0x2A
	uint8_t ucsr0a; // 0x2B
	uint8_t udr0; // 0x2C
	uint8_t fill1[99]; // (90 - 2C) - 1
	uint8_t ubrr0h; // 0x90
	uint8_t fill2[4]; // (95 - 90) - 1
	uint8_t ucsr0c; // 0x95
} Atmega128Usart0_TypeDef;

// USART (USART1)
typedef struct {
	uint8_t ubrr1h; // 0x98
	uint8_t ubrr1l; // 0x99
	uint8_t ucsr1b; // 0x9A
	uint8_t ucsr1a; // 0x9B
	uint8_t udr1; // 0x9C
	uint8_t ucsr1c; // 0x9D
} Atmega128Usart1_TypeDef;

// Watchdog Timer (WDT)
typedef struct {
	uint8_t wdtcr; // 0x41
} Atmega128WatchdogTimer_TypeDef;

/*** Interrupt Vectors FLASH ***/
typedef struct { // IVSEL = 0
	HighLowByte RESET_vect; // 0x0000
	HighLowByte EXT_INT0_vect; // 0x0002
	HighLowByte EXT_INT1_vect; // 0x0004
	HighLowByte EXT_INT2_vect; // 0x0006
	HighLowByte EXT_INT3_vect; // 0x0008
	HighLowByte EXT_INT4_vect; // 0x000A
	HighLowByte EXT_INT5_vect; // 0x000C
	HighLowByte EXT_INT6_vect; // 0x000E
	HighLowByte EXT_INT7_vect; // 0x0010
	HighLowByte TIM2_COMP_vect; // 0x0012
	HighLowByte TIM2_OVF_vect; // 0x0014
	HighLowByte TIM1_CAPT_vect; // 0x0016
	HighLowByte TIM1_COMPA_vect; // 0x0018
	HighLowByte TIM1_COMPB_vect; // 0x001A
	HighLowByte TIM1_OVF_vect; // 0x001C
	HighLowByte TIM0_COMP_vect; // 0x001E
	HighLowByte TIM0_OVF_vect; // 0x0020
	HighLowByte SPI_STC_vect; // 0x0022
	HighLowByte USART0_RXC_vect; // 0x0024
	HighLowByte USART0_DRE_vect; // 0x0026
	HighLowByte USART0_TXC_vect; // 0x0028
	HighLowByte ADC_vect; // 0x002A
	HighLowByte EE_RDY_vect; // 0x002C
	HighLowByte ANA_COMP_vect; // 0x002E
	HighLowByte TIM1_COMPC_vect; // 0x0030
	HighLowByte TIM3_CAPT_vect; // 0x0032
	HighLowByte TIM3_COMPA_vect; // 0x0034
	HighLowByte TIM3_COMPB_vect; // 0x0036
	HighLowByte TIM3_COMPC_vect; // 0x0038
	HighLowByte TIM3_OVF_vect; // 0x003A
	HighLowByte USART1_RXC_vect; // 0x003C
	HighLowByte USART1_DRE_vect; // 0x003E
	HighLowByte USART1_TXC_vect; // 0x0040
	HighLowByte TWI_vect; // 0x0042
	HighLowByte SPM_RDY_vect; // 0x0044
}Atmega128InterruptVectors_TypeDef;

#endif
/***EOF***/

