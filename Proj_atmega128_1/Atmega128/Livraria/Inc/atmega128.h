/***********************************************************************
	ATMEGA 128
Author:   <sergio.salazar.santos@gmail.com>
License:  GNU General Public License
Hardware: Atmega128 by ETT ET-BASE
Date:	  07/04/2024
***********************************************************************/
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

/*** Global Library ***/	
#include <avr/io.h>
#include <avr/boot.h>
#include <avr/fuse.h>
#include <avr/eeprom.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#include "atmega128_registers.h"

/*** Global Constant & Macro ***/
#define TWO 2
#define NIBBLE_BITS 4
#define BYTE_BITS 8
#define WORD_BITS 16
#define DWORD_BITS 32
#define QWORD_BITS 64
#define SRAMSTART 0x0100
#define SRAMEND 0x10FF
// Macros for common operations
#define SET_REG(REG, HBITS)		(REG |= HBITS)
#define CLEAR_REG(REG, HBITS)	(REG &= ~HBITS)
#define READ_BIT(REG, BIT)		((REG >> BIT) & 1)
#define TOGGLE_REG(REG, HBITS)	(REG ^= HBITS)

/***************************/
/*** HARDWARE HANDLER ***/
/***************************/
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
	U_word X; // 0x1A 0x1B
	U_word Y; // 0x1C 0x1D
	U_word Z; // 0x1E 0x1F
} Atmega128GPWR_TypeDef;

Atmega128GPWR_TypeDef* gpwr_instance(void);

// Analog Comparator (AC)
typedef volatile struct {
	union{
		struct{
			uint8_t acis:2;
			uint8_t acic:1;
			uint8_t acie:1;
			uint8_t aci:1;
			uint8_t aco:1;
			uint8_t acbg:1;
			uint8_t acd:1;
		}par;
		uint8_t reg;
	}acsr; // 0x28
} Atmega128AnalogComparator_TypeDef;

Atmega128AnalogComparator_TypeDef* ac_instance(void);

// Analog to Digital Converter (ADC)
typedef volatile struct {
	volatile U_word adc; // 0x24 0x25
	union{
		struct{
			uint8_t adps:3;
			uint8_t adie:1;
			uint8_t adif:1;
			uint8_t adfr:1;
			uint8_t adsc:1;
			uint8_t aden:1;
		}par;
		uint8_t reg;
	}adcsra; // 0x26
	union{
		struct{
			uint8_t mux:5;
			uint8_t adlar:1;
			uint8_t refs:2;
		}par;
		uint8_t reg;
	}admux; // 0x27
} Atmega128AnalogToDigitalConverter_TypeDef;

Atmega128AnalogToDigitalConverter_TypeDef* adc_instance(void);

// Boot loader (BOOT_LOAD)
typedef volatile struct {
	union{
		struct{
				uint8_t spmem:1;
				uint8_t pgers:1;
				uint8_t pgwrt:1;
				uint8_t blbset:1;
				uint8_t rwwsre:1;
				uint8_t reserved:1;
				uint8_t rwwsb:1;
				uint8_t spmie:1;
		}par;
		uint8_t reg;
	}spmcsr; // 0x68
} Atmega128BootLoader_TypeDef;

Atmega128BootLoader_TypeDef* bootload_instance(void);

// CPU Register (CPU)
typedef volatile struct {
	union{
		struct{
			uint8_t porf:1;
			uint8_t extrf:1;
			uint8_t borf:1;
			uint8_t wdrf:1;
			uint8_t jtrf:1;
			uint8_t reserved:2;
			uint8_t jtd:1;
		}par;
		uint8_t reg;
	}mcucsr; // 0x54
	union{
		struct{
			uint8_t ivce:1;
			uint8_t ivsel:1;
			uint8_t sm2:1;
			uint8_t sm0:1;
			uint8_t sm1:1;
			uint8_t se:1;
			uint8_t srw10:1;
			uint8_t sre:1;
		}par;
		uint8_t reg;
	}mcucr; // 0x55
	uint8_t fill1[5]; // (5B - 55) - 1
	union{
		struct{
			uint8_t rampz0:1;
			uint8_t reserved:7;
		}par;
		uint8_t reg;
	}rampz; // 0x5B
	union{
		struct{
			uint8_t xdiv:7;
			uint8_t xdiven:1;
		}par;
		uint8_t reg;
	}xdiv; // 0x5C
	volatile U_word sp; // 0x5D 0x5E
	union{
		struct{
			uint8_t C:1;
			uint8_t Z:1;
			uint8_t N:1;
			uint8_t V:1;
			uint8_t S:1;
			uint8_t H:1;
			uint8_t T:1;
			uint8_t I:1;
		}par;
		uint8_t reg;
	}sreg; // 0x5F
	uint8_t fill2[12]; // (6C - 5F) - 1
	union{
		struct{
			uint8_t xmm0:1;
			uint8_t xmm1:1;
			uint8_t xmm2:1;
			uint8_t reserved:4;
			uint8_t xmbk:1;
		}par;
		uint8_t reg;
	}xmcrb; // 0x6C
	union{
		struct{
			uint8_t reserved1:1;
			uint8_t srw11:1;
			uint8_t srw00:1;
			uint8_t srw01:1;
			uint8_t srl0:1;
			uint8_t srl1:1;
			uint8_t srl2:1;
			uint8_t reserved2:1;
		}par;
		uint8_t reg;
	}xmcra; // 0x6D
	uint8_t fill3; // (6F - 6D) - 1
	union{
		struct{
			uint8_t bit0:1;
			uint8_t bit1:1;
			uint8_t bit2:1;
			uint8_t bit3:1;
			uint8_t bit4:1;
			uint8_t bit5:1;
			uint8_t bit6:1;
			uint8_t bit7:1;
		}par;
		uint8_t reg;
	}osccal; // 0x6F
} Atmega128CPURegister_TypeDef;

Atmega128CPURegister_TypeDef* cpu_instance(void);

// EEPROM (EEPROM)
typedef volatile struct {
	union{
		struct{
			uint8_t eere:1;
			uint8_t eewe:1;
			uint8_t eemwe:1;
			uint8_t eerie:1;
			uint8_t reserved:4;
		}par;
		uint8_t reg;
	}eecr; // 0x3C
	union{
		struct{
			uint8_t bit0:1;
			uint8_t bit1:1;
			uint8_t bit2:1;
			uint8_t bit3:1;
			uint8_t bit4:1;
			uint8_t bit5:1;
			uint8_t bit6:1;
			uint8_t bit7:1;
		}par;
		uint8_t reg;
	}eedr; // 0x3D
	volatile U_word eear; // 0x3E 0x3F
} Atmega128Eeprom_TypeDef;

Atmega128Eeprom_TypeDef* eeprom_instance(void);

// External Interrupts (EXINT)
typedef volatile struct {
	union{
		struct{
			uint8_t intf0:1;
			uint8_t intf1:1;
			uint8_t intf2:1;
			uint8_t intf3:1;
			uint8_t intf4:1;
			uint8_t intf5:1;
			uint8_t intf6:1;
			uint8_t intf7:1;
		}par;
		uint8_t reg;
	}eifr; // 0x58
	union{
		struct{
			uint8_t int0:1;
			uint8_t int1:1;
			uint8_t int2:1;
			uint8_t int3:1;
			uint8_t int4:1;
			uint8_t int5:1;
			uint8_t int6:1;
			uint8_t int7:1;
		}par;
		uint8_t reg;
	}eimsk; // 0x59
	union{
		struct{
			uint8_t isc40:1;
			uint8_t isc41:1;
			uint8_t isc50:1;
			uint8_t isc51:1;
			uint8_t isc60:1;
			uint8_t isc61:1;
			uint8_t isc70:1;
			uint8_t isc71:1;
		}par;
		uint8_t reg;
	}eicrb; // 0x5A
	uint8_t fill[15]; // (6A - 5A) - 1
	union{
		struct{
			uint8_t isc00:1;
			uint8_t isc01:1;
			uint8_t isc10:1;
			uint8_t isc11:1;
			uint8_t isc20:1;
			uint8_t isc21:1;
			uint8_t isc30:1;
			uint8_t isc31:1;
		}par;
		uint8_t reg;
	}eicra; // 0x6A
} Atmega128ExternalInterrupts_TypeDef;

Atmega128ExternalInterrupts_TypeDef* exint_instance(void);

// I/O Port (PORTA)
typedef volatile struct {
	union{
		struct{
			uint8_t a0:1;
			uint8_t a1:1;
			uint8_t a2:1;
			uint8_t a3:1;
			uint8_t a4:1;
			uint8_t a5:1;
			uint8_t a6:1;
			uint8_t a7:1;
		}par;
		uint8_t reg;
	}pin; // 0x39
	union{
		struct{
			uint8_t a0:1;
			uint8_t a1:1;
			uint8_t a2:1;
			uint8_t a3:1;
			uint8_t a4:1;
			uint8_t a5:1;
			uint8_t a6:1;
			uint8_t a7:1;
		}par;
		uint8_t reg;
	}ddr; // 0x3A
	union{
		struct{
			uint8_t a0:1;
			uint8_t a1:1;
			uint8_t a2:1;
			uint8_t a3:1;
			uint8_t a4:1;
			uint8_t a5:1;
			uint8_t a6:1;
			uint8_t a7:1;
		}par;
		uint8_t reg;
	}port; // 0x3B
} Atmega128PORTA_TypeDef;

Atmega128PORTA_TypeDef* porta_instance(void);

// I/O Port (PORTB)
typedef volatile struct {
	union{
		struct{
			uint8_t b0:1;
			uint8_t b1:1;
			uint8_t b2:1;
			uint8_t b3:1;
			uint8_t b4:1;
			uint8_t b5:1;
			uint8_t b6:1;
			uint8_t b7:1;
		}par;
		uint8_t reg;
	}pin; // 0x36
	union{
		struct{
			uint8_t b0:1;
			uint8_t b1:1;
			uint8_t b2:1;
			uint8_t b3:1;
			uint8_t b4:1;
			uint8_t b5:1;
			uint8_t b6:1;
			uint8_t b7:1;
		}par;
		uint8_t reg;
	}ddr; // 0x37
	union{
		struct{
			uint8_t b0:1;
			uint8_t b1:1;
			uint8_t b2:1;
			uint8_t b3:1;
			uint8_t b4:1;
			uint8_t b5:1;
			uint8_t b6:1;
			uint8_t b7:1;
		}par;
		uint8_t reg;
	}port; // 0x38
} Atmega128PORTB_TypeDef;

Atmega128PORTB_TypeDef* portb_instance(void);

// I/O Port (PORTC)
typedef volatile struct {
	union{
		struct{
			uint8_t c0:1;
			uint8_t c1:1;
			uint8_t c2:1;
			uint8_t c3:1;
			uint8_t c4:1;
			uint8_t c5:1;
			uint8_t c6:1;
			uint8_t c7:1;
		}par;
		uint8_t reg;
	}pin; // 0x33
	union{
		struct{
			uint8_t c0:1;
			uint8_t c1:1;
			uint8_t c2:1;
			uint8_t c3:1;
			uint8_t c4:1;
			uint8_t c5:1;
			uint8_t c6:1;
			uint8_t c7:1;
		}par;
		uint8_t reg;
	}ddr; // 0x34
	union{
		struct{
			uint8_t c0:1;
			uint8_t c1:1;
			uint8_t c2:1;
			uint8_t c3:1;
			uint8_t c4:1;
			uint8_t c5:1;
			uint8_t c6:1;
			uint8_t c7:1;
		}par;
		uint8_t reg;
	}port; // 0x35
} Atmega128PORTC_TypeDef;

Atmega128PORTC_TypeDef* portc_instance(void);

// I/O Port (PORTD)
typedef volatile struct {
	union{
		struct{
			uint8_t d0:1;
			uint8_t d1:1;
			uint8_t d2:1;
			uint8_t d3:1;
			uint8_t d4:1;
			uint8_t d5:1;
			uint8_t d6:1;
			uint8_t d7:1;
		}par;
		uint8_t reg;
	}pin; // 0x30
	union{
		struct{
			uint8_t d0:1;
			uint8_t d1:1;
			uint8_t d2:1;
			uint8_t d3:1;
			uint8_t d4:1;
			uint8_t d5:1;
			uint8_t d6:1;
			uint8_t d7:1;
		}par;
		uint8_t reg;
	}ddr; // 0x31
	union{
		struct{
			uint8_t d0:1;
			uint8_t d1:1;
			uint8_t d2:1;
			uint8_t d3:1;
			uint8_t d4:1;
			uint8_t d5:1;
			uint8_t d6:1;
			uint8_t d7:1;
		}par;
		uint8_t reg;
	}port; // 0x32
} Atmega128PORTD_TypeDef;

Atmega128PORTD_TypeDef* portd_instance(void);

// I/O Port (PORTE)
typedef volatile struct {
	union{
		struct{
			uint8_t e0:1;
			uint8_t e1:1;
			uint8_t e2:1;
			uint8_t e3:1;
			uint8_t e4:1;
			uint8_t e5:1;
			uint8_t e6:1;
			uint8_t e7:1;
		}par;
		uint8_t reg;
	}pin; // 0x21
	union{
		struct{
			uint8_t e0:1;
			uint8_t e1:1;
			uint8_t e2:1;
			uint8_t e3:1;
			uint8_t e4:1;
			uint8_t e5:1;
			uint8_t e6:1;
			uint8_t e7:1;
		}par;
		uint8_t reg;
	}ddr; // 0x22
	union{
		struct{
			uint8_t e0:1;
			uint8_t e1:1;
			uint8_t e2:1;
			uint8_t e3:1;
			uint8_t e4:1;
			uint8_t e5:1;
			uint8_t e6:1;
			uint8_t e7:1;
		}par;
		uint8_t reg;
	}port; // 0x23
} Atmega128PORTE_TypeDef;

Atmega128PORTE_TypeDef* porte_instance(void);

// I/O Port (PORTF)
typedef volatile struct {
	union{
		struct{
			uint8_t f0:1;
			uint8_t f1:1;
			uint8_t f2:1;
			uint8_t f3:1;
			uint8_t f4:1;
			uint8_t f5:1;
			uint8_t f6:1;
			uint8_t f7:1;
		}par;
		uint8_t reg;
	}pin; // 0x20
	uint8_t fill[64]; // (0x61 - 0x20) - 1
	union{
		struct{
			uint8_t f0:1;
			uint8_t f1:1;
			uint8_t f2:1;
			uint8_t f3:1;
			uint8_t f4:1;
			uint8_t f5:1;
			uint8_t f6:1;
			uint8_t f7:1;
		}par;
		uint8_t reg;
	}ddr; // 0x61
	union{
		struct{
			uint8_t f0:1;
			uint8_t f1:1;
			uint8_t f2:1;
			uint8_t f3:1;
			uint8_t f4:1;
			uint8_t f5:1;
			uint8_t f6:1;
			uint8_t f7:1;
		}par;
		uint8_t reg;
	}port; // 0x62
} Atmega128PORTF_TypeDef;

Atmega128PORTF_TypeDef* portf_instance(void);

// I/O Port (PORTG)
typedef volatile struct {
	union{
		struct{
			uint8_t g0:1;
			uint8_t g1:1;
			uint8_t g2:1;
			uint8_t g3:1;
			uint8_t g4:1;
			uint8_t g5:1;
			uint8_t g6:1;
			uint8_t g7:1;
		}par;
		uint8_t reg;
	}pin; // 0x63
	union{
		struct{
			uint8_t g0:1;
			uint8_t g1:1;
			uint8_t g2:1;
			uint8_t g3:1;
			uint8_t g4:1;
			uint8_t g5:1;
			uint8_t g6:1;
			uint8_t g7:1;
		}par;
		uint8_t reg;
	}ddr; // 0x64
	union{
		struct{
			uint8_t g0:1;
			uint8_t g1:1;
			uint8_t g2:1;
			uint8_t g3:1;
			uint8_t g4:1;
			uint8_t g5:1;
			uint8_t g6:1;
			uint8_t g7:1;
		}par;
		uint8_t reg;
	}port; // 0x65
} Atmega128PORTG_TypeDef;

Atmega128PORTG_TypeDef* portg_instance(void);

// JTAG Interface (JTAG)
typedef volatile struct {
	union{
		struct{
			uint8_t bit0:1;
			uint8_t bit1:1;
			uint8_t bit2:1;
			uint8_t bit3:1;
			uint8_t bit4:1;
			uint8_t bit5:1;
			uint8_t bit6:1;
			uint8_t bit7:1;
		}par;
		uint8_t reg;
	}ocdr; // 0x42
	uint8_t fill[17]; // (54 - 42) - 1
	union{
		struct{
			uint8_t porf:1;
			uint8_t extrf:1;
			uint8_t borf:1;
			uint8_t wdrf:1;
			uint8_t jtrf:1;
			uint8_t reserved:2;
			uint8_t jtd:1;
		}par;
		uint8_t reg;
	}mcucsr; // 0x54
} Atmega128JtagInterface_TypeDef;

Atmega128JtagInterface_TypeDef* jtag_instance(void);

// JTAG Interface (JTAG)
typedef volatile struct {
	union{
		struct{
			uint8_t porf:1;
			uint8_t extrf:1;
			uint8_t borf:1;
			uint8_t wdrf:1;
			uint8_t jtrf:1;
			uint8_t reserved:2;
			uint8_t jtd:1;
		}par;
		uint8_t reg;
	}mcucsr; // 0x54
} Atmega128JtagInterfaceControlStatus_TypeDef;

Atmega128JtagInterfaceControlStatus_TypeDef* jtag_cs_instance(void);

// Other Registers (MISC)
typedef volatile struct {
	union{
		struct{
			uint8_t psr321:1;
			uint8_t psr0:1;
			uint8_t pud:1;
			uint8_t acme:1;
			uint8_t reserved:3;
			uint8_t tsm:1;
		}par;
		uint8_t reg;
	}sfior; // 0x40
} Atmega128OtherRegisters_TypeDef;

Atmega128OtherRegisters_TypeDef* misc_instance(void);

// Serial Peripheral Interface (SPI)
typedef volatile struct {
	union{
		struct{
			uint8_t spr0:1;
			uint8_t spr1:1;
			uint8_t cpha:1;
			uint8_t cpol:1;
			uint8_t mstr:1;
			uint8_t dord:1;
			uint8_t spe:1;
			uint8_t spie:1;
		}par;
		uint8_t reg;
	}spcr; // 0x2D
	union{
		struct{
			uint8_t spi2x:1;
			uint8_t reserved:5;
			uint8_t wcol:1;
			uint8_t spif:1;
		}par;
		uint8_t reg;
	}spsr; // 0x2E
	union{
		struct{
			uint8_t bit0:1;
			uint8_t bit1:1;
			uint8_t bit2:1;
			uint8_t bit3:1;
			uint8_t bit4:1;
			uint8_t bit5:1;
			uint8_t bit6:1;
			uint8_t bit7:1;
		}par;
		uint8_t reg;
	}spdr; // 0x2F
} Atmega128SerialPeripheralInterface_TypeDef;

Atmega128SerialPeripheralInterface_TypeDef* spi_instance(void);

// Timer/Counter, 16-bit (TC1)
typedef volatile struct {
	volatile U_word icr1; // 0x46 0x47
	volatile U_word ocr1b; // 0x48 0x49
	volatile U_word ocr1a; // 0x4A 0x4B
	volatile U_word tcnt1; // 0x4C 0x4D
	union{
		struct{
			uint8_t cs10:1;
			uint8_t cs11:1;
			uint8_t cs12:1;
			uint8_t wgm12:1;
			uint8_t wgm13:1;
			uint8_t reserved:1;
			uint8_t ices1:1;
			uint8_t icnc1:1;
		}par;
		uint8_t reg;
	}tccr1b; // 0x4E
	union{
		struct{
			uint8_t wgm10:1;
			uint8_t wgm11:1;
			uint8_t com1c0:1;
			uint8_t com1c1:1;
			uint8_t com1b0:1;
			uint8_t com1b1:1;
			uint8_t com1a0:1;
			uint8_t com1a1:1;
		}par;
		uint8_t reg;
	}tccr1a; // 0x4F
	uint8_t fill1[6]; // (56 - 4F) - 1
	union{
		struct{
			uint8_t tov0:1;
			uint8_t ocf0:1;
			uint8_t tov1:1;
			uint8_t ocf1b:1;
			uint8_t ocf1a:1;
			uint8_t icf1:1;
			uint8_t tov2:1;
			uint8_t ocf2:1;
		}par;
		uint8_t reg;
	}tifr; // 0x56
	union{
		struct{
			uint8_t toie0:1;
			uint8_t ocie0:1;
			uint8_t toie1:1;
			uint8_t ocie1b:1;
			uint8_t ocie1a:1;
			uint8_t ticie1:1;
			uint8_t toie2:1;
			uint8_t ocie2:1;
		}par;
		uint8_t reg;
	}timsk; // 0x57
	uint8_t fill2[32]; // (78 - 57) - 1
	volatile U_word ocr1c; // 0x78 0x79
	union{
		struct{
			uint8_t reserved:5;
			uint8_t foc1c:1;
			uint8_t foc1b:1;
			uint8_t foc1a:1;
		}par;
		uint8_t reg;
	}tccr1c; // 0x7A
	uint8_t fill3; // (7C - 7A) - 1
	union{
		struct{
			uint8_t ocf1c:1;
			uint8_t ocf3c:1;
			uint8_t tov3:1;
			uint8_t ocf3b:1;
			uint8_t ocf3a:1;
			uint8_t icf3:1;
			uint8_t reserved:2;
		}par;
		uint8_t reg;
	}etifr; // 0x7C
	union{
		struct{
			uint8_t ocie1c:1;
			uint8_t ocie3c:1;
			uint8_t toie3:1;
			uint8_t ocie3b:1;
			uint8_t ocie3a:1;
			uint8_t ticie3:1;
			uint8_t reserved:2;
		}par;
		uint8_t reg;
	}etimsk; // 0x7D
} Atmega128TimerCounter1_TypeDef;

Atmega128TimerCounter1_TypeDef* tc1_instance(void);

// Timer/Counter, 16-bit (TC3)
typedef volatile struct {
	union{
		struct{
			uint8_t ocf1c:1;
			uint8_t ocf3c:1;
			uint8_t tov3:1;
			uint8_t ocf3b:1;
			uint8_t ocf3a:1;
			uint8_t icf3:1;
			uint8_t reserved:2;
		}par;
		uint8_t reg;
	}etifr; // 0x7C
	union{
		struct{
			uint8_t ocie1c:1;
			uint8_t ocie3c:1;
			uint8_t toie3:1;
			uint8_t ocie3b:1;
			uint8_t ocie3a:1;
			uint8_t ticie3:1;
			uint8_t reserved:2;
		}par;
		uint8_t reg;
	}etimsk; // 0x7D
	uint8_t fill[2]; // (80 - 7D) - 1
	volatile U_word icr3; // 0x80 0x81
	volatile U_word ocr3c; // 0x82 0x83
	volatile U_word ocr3b; // 0x84 0x85
	volatile U_word ocr3a; // 0x86 0x87
	volatile U_word tcnt3; // 0x88 0x89
	union{
		struct{
			uint8_t cs30:1;
			uint8_t cs31:1;
			uint8_t cs32:1;
			uint8_t wgm32:1;
			uint8_t wgm33:1;
			uint8_t reserved:1;
			uint8_t ices3:1;
			uint8_t icnc3:1;
		}par;
		uint8_t reg;
	}tccr3b; // 0x8A
	union{
		struct{
			uint8_t wgm30:1;
			uint8_t wgm31:1;
			uint8_t com3c0:1;
			uint8_t com3c1:1;
			uint8_t com3b0:1;
			uint8_t com3b1:1;
			uint8_t com3a0:1;
			uint8_t com3a1:1;
		}par;
		uint8_t reg;
	}tccr3a; // 0x8B
	union{
		struct{
			uint8_t reserved:5;
			uint8_t foc3c:1;
			uint8_t foc3b:1;
			uint8_t foc3a:1;
		}par;
		uint8_t reg;
	}tccr3c; // 0x8C
} Atmega128TimerCounter3_TypeDef;

Atmega128TimerCounter3_TypeDef* tc3_instance(void);

// Timer/Counter 1 and 3
typedef volatile struct {
	union{
		struct{
			uint8_t ocf1c:1;
			uint8_t ocf3c:1;
			uint8_t tov3:1;
			uint8_t ocf3b:1;
			uint8_t ocf3a:1;
			uint8_t icf3:1;
			uint8_t reserved:2;
		}par;
		uint8_t reg;
	}etifr; // 0x7C
} Atmega128TimerExternalInterruptFlag_TypeDef;

Atmega128TimerExternalInterruptFlag_TypeDef* tc_exif_instance(void);

// Timer/Counter 1 and 3
typedef volatile struct {
	union{
		struct{
			uint8_t ocie1c:1;
			uint8_t ocie3c:1;
			uint8_t toie3:1;
			uint8_t ocie3b:1;
			uint8_t ocie3a:1;
			uint8_t ticie3:1;
			uint8_t reserved:2;
		}par;
		uint8_t reg;
	}etimsk; // 0x7D
} Atmega128TimerExternalInterruptMask_TypeDef;

Atmega128TimerExternalInterruptMask_TypeDef* tc_exim_instance(void);

// Timer/Counter, 8-bit (TC2)
typedef volatile struct {
	union{
		struct{
			uint8_t bit0:1;
			uint8_t bit1:1;
			uint8_t bit2:1;
			uint8_t bit3:1;
			uint8_t bit4:1;
			uint8_t bit5:1;
			uint8_t bit6:1;
			uint8_t bit7:1;
		}par;
		uint8_t reg;
	}ocr2; // 0x43
	union{
		struct{
			uint8_t bit0:1;
			uint8_t bit1:1;
			uint8_t bit2:1;
			uint8_t bit3:1;
			uint8_t bit4:1;
			uint8_t bit5:1;
			uint8_t bit6:1;
			uint8_t bit7:1;
		}par;
		uint8_t reg;
	}tcnt2; // 0x44
	union{
		struct{
			uint8_t cs20:1;
			uint8_t cs21:1;
			uint8_t cs22:1;
			uint8_t wgm21:1;
			uint8_t com20:1;
			uint8_t com21:1;
			uint8_t wgm20:1;
			uint8_t foc2:1;
		}par;
		uint8_t reg;
	}tccr2; // 0x45
	uint8_t fill[16]; // (56 - 45) - 1
	union{
		struct{
			uint8_t tov0:1;
			uint8_t ocf0:1;
			uint8_t tov1:1;
			uint8_t ocf1b:1;
			uint8_t ocf1a:1;
			uint8_t icf1:1;
			uint8_t tov2:1;
			uint8_t ocf2:1;
		}par;
		uint8_t reg;
	}tifr; // 0x56
	union{
		struct{
			uint8_t toie0:1;
			uint8_t ocie0:1;
			uint8_t toie1:1;
			uint8_t ocie1b:1;
			uint8_t ocie1a:1;
			uint8_t ticie1:1;
			uint8_t toie2:1;
			uint8_t ocie2:1;
		}par;
		uint8_t reg;
	}timsk; // 0x57
} Atmega128TimerCounter2_TypeDef;

Atmega128TimerCounter2_TypeDef* tc2_instance(void);

// Timer/Counter, 8-bit A sync (TC0)
typedef volatile struct {
	union{
		struct{
			uint8_t tcr0ub:1;
			uint8_t ocr0ub:1;
			uint8_t tcn0ub:1;
			uint8_t as0:1;
			uint8_t reserved:4;
		}par;
		uint8_t reg;
	}assr; // 0x50
	union{
		struct{
			uint8_t bit0:1;
			uint8_t bit1:1;
			uint8_t bit2:1;
			uint8_t bit3:1;
			uint8_t bit4:1;
			uint8_t bit5:1;
			uint8_t bit6:1;
			uint8_t bit7:1;
		}par;
		uint8_t reg;
	}ocr0; // 0x51
	union{
		struct{
			uint8_t bit0:1;
			uint8_t bit1:1;
			uint8_t bit2:1;
			uint8_t bit3:1;
			uint8_t bit4:1;
			uint8_t bit5:1;
			uint8_t bit6:1;
			uint8_t bit7:1;
		}par;
		uint8_t reg;
	}tcnt0; // 0x52
	union{
		struct{
			uint8_t cs00:1;
			uint8_t cs01:1;
			uint8_t cs02:1;
			uint8_t wgm01:1;
			uint8_t com00:1;
			uint8_t com01:1;
			uint8_t wgm00:1;
			uint8_t foc0:1;
		}par;
		uint8_t reg;
	}tccr0; // 0x53
	uint8_t fill[2]; // (56 - 53) - 1
	union{
		struct{
			uint8_t tov0:1;
			uint8_t ocf0:1;
			uint8_t tov1:1;
			uint8_t ocf1b:1;
			uint8_t ocf1a:1;
			uint8_t icf1:1;
			uint8_t tov2:1;
			uint8_t ocf2:1;
		}par;
		uint8_t reg;
	}tifr; // 0x56
	union{
		struct{
			uint8_t toie0:1;
			uint8_t ocie0:1;
			uint8_t toie1:1;
			uint8_t ocie1b:1;
			uint8_t ocie1a:1;
			uint8_t ticie1:1;
			uint8_t toie2:1;
			uint8_t ocie2:1;
		}par;
		uint8_t reg;
	}timsk; // 0x57
} Atmega128TimerCounter0_TypeDef;

Atmega128TimerCounter0_TypeDef* tc0_instance(void);

// Timer/Counter 0, 1 and 2
typedef volatile struct {
	union{
		struct{
			uint8_t tov0:1;
			uint8_t ocf0:1;
			uint8_t tov1:1;
			uint8_t ocf1b:1;
			uint8_t ocf1a:1;
			uint8_t icf1:1;
			uint8_t tov2:1;
			uint8_t ocf2:1;
		}par;
		uint8_t reg;
	}tifr; // 0x56
} Atmega128TimerInterruptFlag_TypeDef;

Atmega128TimerInterruptFlag_TypeDef* tc_if_instance(void);

// Timer/Counter 0, 1 and 2
typedef volatile struct {
	union{
		struct{
			uint8_t toie0:1;
			uint8_t ocie0:1;
			uint8_t toie1:1;
			uint8_t ocie1b:1;
			uint8_t ocie1a:1;
			uint8_t ticie1:1;
			uint8_t toie2:1;
			uint8_t ocie2:1;
		}par;
		uint8_t reg;
	}timsk; // 0x57
} Atmega128TimerInterruptMask_TypeDef;

Atmega128TimerInterruptMask_TypeDef* tc_im_instance(void);

// Two Wire Serial Interface (TWI)
typedef volatile struct {
	union{
		struct{
			uint8_t bit0:1;
			uint8_t bit1:1;
			uint8_t bit2:1;
			uint8_t bit3:1;
			uint8_t bit4:1;
			uint8_t bit5:1;
			uint8_t bit6:1;
			uint8_t bit7:1;
		}par;
		uint8_t reg;
	}twbr; // 0x70
	union{
		struct{
			uint8_t twps:2;
			uint8_t reserved:1;
			uint8_t tws:5;
		}par;
		uint8_t reg;
	}twsr; // 0x71
	union{
		struct{
			uint8_t twgce:1;
			uint8_t twa:7;
		}par;
		uint8_t reg;
	}twar; // 0x72
	union{
		struct{
			uint8_t bit0:1;
			uint8_t bit1:1;
			uint8_t bit2:1;
			uint8_t bit3:1;
			uint8_t bit4:1;
			uint8_t bit5:1;
			uint8_t bit6:1;
			uint8_t bit7:1;
		}par;
		uint8_t reg;
	}twdr; // 0x73
	union{
		struct{
			uint8_t twie:1;
			uint8_t reserved:1;
			uint8_t twen:1;
			uint8_t twwc:1;
			uint8_t twsto:1;
			uint8_t twsta:1;
			uint8_t twea:1;
			uint8_t twint:1;
		}par;
		uint8_t reg;
	}twcr; // 0x74
} Atmega128TwoWireSerialInterface_TypeDef;

Atmega128TwoWireSerialInterface_TypeDef* twi_instance(void);

// USART (USART0)
typedef volatile struct {
	union{
		struct{
			uint8_t bit0:1;
			uint8_t bit1:1;
			uint8_t bit2:1;
			uint8_t bit3:1;
			uint8_t bit4:1;
			uint8_t bit5:1;
			uint8_t bit6:1;
			uint8_t bit7:1;
		}par;
		uint8_t reg;
	}ubrr0l; // 0x29
	union{
		struct{
			uint8_t txb80:1;
			uint8_t rxb80:1;
			uint8_t ucsz02:1;
			uint8_t txen0:1;
			uint8_t rxen0:1;
			uint8_t udrie0:1;
			uint8_t txcie0:1;
			uint8_t rxcie0:1;
		}par;
		uint8_t reg;
	}ucsr0b; // 0x2A
	union{
		struct{
			uint8_t mpcm0:1;
			uint8_t u2x0:1;
			uint8_t upe0:1;
			uint8_t dor0:1;
			uint8_t fe0:1;
			uint8_t udre0:1;
			uint8_t txc0:1;
			uint8_t rxc0:1;
		}par;
		uint8_t reg;
	}ucsr0a; // 0x2B
	union{
		struct{
			uint8_t bit0:1;
			uint8_t bit1:1;
			uint8_t bit2:1;
			uint8_t bit3:1;
			uint8_t bit4:1;
			uint8_t bit5:1;
			uint8_t bit6:1;
			uint8_t bit7:1;
		}par;
		uint8_t reg;
	}udr0; // 0x2C
	uint8_t fill1[99]; // (90 - 2C) - 1
	union{
		struct{
			uint8_t bit0:1;
			uint8_t bit1:1;
			uint8_t bit2:1;
			uint8_t bit3:1;
			uint8_t reserved:4;
		}par;
		uint8_t reg;
	}ubrr0h; // 0x90
	uint8_t fill2[4]; // (95 - 90) - 1
	union{
		struct{
			uint8_t ucpol0:1;
			uint8_t ucsz00:1;
			uint8_t ucsz01:1;
			uint8_t usbs0:1;
			uint8_t upm00:1;
			uint8_t upm01:1;
			uint8_t umsel0:1;
			uint8_t reserved:1;
		}par;
		uint8_t reg;
	}ucsr0c; // 0x95
} Atmega128Usart0_TypeDef;

Atmega128Usart0_TypeDef* usart0_instance(void);

// USART (USART1)
typedef volatile struct {
	union{
		struct{
			uint8_t bit0:1;
			uint8_t bit1:1;
			uint8_t bit2:1;
			uint8_t bit3:1;
			uint8_t reserved:4;
		}par;
		uint8_t reg;
	}ubrr1h; // 0x98
	union{
		struct{
			uint8_t bit0:1;
			uint8_t bit1:1;
			uint8_t bit2:1;
			uint8_t bit3:1;
			uint8_t bit4:1;
			uint8_t bit5:1;
			uint8_t bit6:1;
			uint8_t bit7:1;
		}par;
		uint8_t reg;
	}ubrr1l; // 0x99
	union{
		struct{
			uint8_t txb81:1;
			uint8_t rxb81:1;
			uint8_t ucsz12:1;
			uint8_t txen1:1;
			uint8_t rxen1:1;
			uint8_t udrie1:1;
			uint8_t txcie1:1;
			uint8_t rxcie1:1;
		}par;
		uint8_t reg;
	}ucsr1b; // 0x9A
	union{
		struct{
			uint8_t mpcm1:1;
			uint8_t u2x1:1;
			uint8_t upe1:1;
			uint8_t dor1:1;
			uint8_t fe1:1;
			uint8_t udre1:1;
			uint8_t txc1:1;
			uint8_t rxc1:1;
		}par;
		uint8_t reg;
	}ucsr1a; // 0x9B
	union{
		struct{
			uint8_t bit0:1;
			uint8_t bit1:1;
			uint8_t bit2:1;
			uint8_t bit3:1;
			uint8_t bit4:1;
			uint8_t bit5:1;
			uint8_t bit6:1;
			uint8_t bit7:1;
		}par;
		uint8_t reg;
	}udr1; // 0x9C
	union{
		struct{
			uint8_t ucpol1:1;
			uint8_t ucsz10:1;
			uint8_t ucsz11:1;
			uint8_t usbs1:1;
			uint8_t upm10:1;
			uint8_t upm11:1;
			uint8_t umsel1:1;
			uint8_t reserved:1;
		}par;
		uint8_t reg;
	}ucsr1c; // 0x9D
} Atmega128Usart1_TypeDef;

Atmega128Usart1_TypeDef* usart1_instance(void);

// Watchdog Timer (WDT)
typedef volatile struct {
	union{
		struct{
			uint8_t wdp:3;
			uint8_t wde:1;
			uint8_t wdce:1;
			uint8_t reserved:3;
		}par;
		uint8_t reg;
	}wdtcr; // 0x41
} Atmega128WatchdogTimer_TypeDef;

Atmega128WatchdogTimer_TypeDef* wdt_instance(void);

/*** Atmega 128 Procedure and Function ***/
uint16_t readhlbyte(HighLowByte reg);
uint16_t readHLbyte(U_word reg);
uint16_t readlhbyte(HighLowByte reg);
uint16_t readLHbyte(U_word reg);
HighLowByte writehlbyte(uint16_t val);
U_word writeHLbyte(uint16_t val);
HighLowByte writelhbyte(uint16_t val);
U_word writeLHbyte(uint16_t val);
uint16_t swapbyte(uint16_t num);
uint16_t BAUDRATEnormal(uint32_t BAUD);
uint16_t BAUDRATEdouble(uint32_t BAUD);
uint16_t BAUDRATEsynchronous(uint32_t BAUD);
void ClockPrescalerSelect(volatile uint8_t prescaler);
void MoveInterruptsToBoot(void);
/*** Procedure and Function ToolSet ***/
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

/*** Interrupt Vectors FLASH ***
typedef struct { // IVSEL = 0
	volatile U_word RESET_vect; // 0x0000
	volatile U_word EXT_INT0_vect; // 0x0002
	volatile U_word EXT_INT1_vect; // 0x0004
	volatile U_word EXT_INT2_vect; // 0x0006
	volatile U_word EXT_INT3_vect; // 0x0008
	volatile U_word EXT_INT4_vect; // 0x000A
	volatile U_word EXT_INT5_vect; // 0x000C
	volatile U_word EXT_INT6_vect; // 0x000E
	volatile U_word EXT_INT7_vect; // 0x0010
	volatile U_word TIM2_COMP_vect; // 0x0012
	volatile U_word TIM2_OVF_vect; // 0x0014
	volatile U_word TIM1_CAPT_vect; // 0x0016
	volatile U_word TIM1_COMPA_vect; // 0x0018
	volatile U_word TIM1_COMPB_vect; // 0x001A
	volatile U_word TIM1_OVF_vect; // 0x001C
	volatile U_word TIM0_COMP_vect; // 0x001E
	volatile U_word TIM0_OVF_vect; // 0x0020
	volatile U_word SPI_STC_vect; // 0x0022
	volatile U_word USART0_RXC_vect; // 0x0024
	volatile U_word USART0_DRE_vect; // 0x0026
	volatile U_word USART0_TXC_vect; // 0x0028
	volatile U_word ADC_vect; // 0x002A
	volatile U_word EE_RDY_vect; // 0x002C
	volatile U_word ANA_COMP_vect; // 0x002E
	volatile U_word TIM1_COMPC_vect; // 0x0030
	volatile U_word TIM3_CAPT_vect; // 0x0032
	volatile U_word TIM3_COMPA_vect; // 0x0034
	volatile U_word TIM3_COMPB_vect; // 0x0036
	volatile U_word TIM3_COMPC_vect; // 0x0038
	volatile U_word TIM3_OVF_vect; // 0x003A
	volatile U_word USART1_RXC_vect; // 0x003C
	volatile U_word USART1_DRE_vect; // 0x003E
	volatile U_word USART1_TXC_vect; // 0x0040
	volatile U_word TWI_vect; // 0x0042
	volatile U_word SPM_RDY_vect; // 0x0044
} Atmega128InterruptVectors_TypeDef;
********************************/

/*** EOF ***/

