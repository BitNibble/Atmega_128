/*********************************************************
	ATMEGA 128 Handler
Author: Sergio Manuel Santos
	<sergio.salazar.santos@gmail.com>
License: GNU General Public License
Hardware: Atmega 128
Update:	07/04/2024
Comment: 
	Simple tools
**********************************************************/
#ifndef _ATMEGA128HANDLER_H_
	#define _ATMEGA128HANDLER_H_
	
/*** Working Frequency ***/
// User May change to conform to use
#ifndef F_CPU
	#define F_CPU 16000000UL
#endif
	
#include <avr/io.h>
#include <avr/fuse.h>
#include <avr/eeprom.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "atmega128.h"

/***************************/
/*** HARDWARE HANDLER ***/
/***************************/
// GPWR
Atmega128GPWR_TypeDef* gpwr_handle(void);
// Analog Comparator (AC)
Atmega128AnalogComparator_TypeDef* ac_handle(void);
// Analog to Digital Converter (ADC)
Atmega128AnalogToDigitalConverter_TypeDef* adc_handle(void);
// Boot loader (BOOT_LOAD)
Atmega128BootLoader_TypeDef* bootload_handle(void);
// CPU Register (CPU)
Atmega128CPURegister_TypeDef* cpu_handle(void);
// EEPROM (EEPROM)
Atmega128Eeprom_TypeDef* eeprom_handle(void);
// External Interrupts (EXINT)
Atmega128ExternalInterrupts_TypeDef* exint_handle(void);
// I/O Port (PORTA)
Atmega128PORTA_TypeDef* porta_handle(void);
// I/O Port (PORTB)
Atmega128PORTB_TypeDef* portb_handle(void);
// I/O Port (PORTC)
Atmega128PORTC_TypeDef* portc_handle(void);
// I/O Port (PORTD)
Atmega128PORTD_TypeDef* portd_handle(void);
// I/O Port (PORTE)
Atmega128PORTE_TypeDef* porte_handle(void);
// I/O Port (PORTF)
Atmega128PORTF_TypeDef* portf_handle(void);
// I/O Port (PORTG)
Atmega128PORTG_TypeDef* portg_handle(void);
// JTAG Interface (JTAG)
Atmega128JtagInterface_TypeDef* jtag_handle(void);
// JTAG Interface (JTAG)
Atmega128JtagInterfaceControlStatus_TypeDef* jtag_cs_handle(void);
// Other Registers (MISC)
Atmega128OtherRegisters_TypeDef* misc_handle(void);
// Serial Peripheral Interface (SPI)
Atmega128SerialPeripherialInterface_TypeDef* spi_handle(void);
// Timer/Counter, 16-bit (TC1)
Atmega128TimerCounter1_TypeDef* tc1_handle(void);
// Timer/Counter, 16-bit (TC3)
Atmega128TimerCounter3_TypeDef* tc3_handle(void);
// Timer/Counter 1 and 3
Atmega128TimerExternalInterruptFlag_TypeDef* tc_exif_handle(void);
// Timer/Counter 1 and 3
Atmega128TimerExternalInterruptMask_TypeDef* tc_exim_handle(void);
// Timer/Counter, 8-bit (TC2)
Atmega128TimerCounter2_TypeDef* tc2_handle(void);
// Timer/Counter, 8-bit A sync (TC0)
Atmega128TimerCounter0_TypeDef* tc0_handle(void);
// Timer/Counter 0, 1 and 2
Atmega128TimerInterruptFlag_TypeDef* tc_if_handle(void);
// Timer/Counter 0, 1 and 2
Atmega128TimerInterruptMask_TypeDef* tc_im_handle(void);
// Two Wire Serial Interface (TWI)
Atmega128TwoWireSerialInterface_TypeDef* twi_handle(void);
// USART (USART0)
Atmega128Usart0_TypeDef* usart0_handle(void);
// USART (USART1)
Atmega128Usart1_TypeDef* usart1_handle(void);
// Watchdog Timer (WDT)
Atmega128WatchdogTimer_TypeDef* wdt_handle(void);

/*** Atmega 128 Procedure and Function ***/
uint16_t readhlbyte(HighLowByte reg);
uint16_t readlhbyte(HighLowByte reg);
HighLowByte writehlbyte(uint16_t val);
HighLowByte writelhbyte(uint16_t val);
uint16_t swapbyte(uint16_t num);
uint16_t BAUDRATEnormal(uint32_t BAUD);
uint16_t BAUDRATEdouble(uint32_t BAUD);
uint16_t BAUDRATEsynchronous(uint32_t BAUD);
void ClockPrescalerSelect(volatile uint8_t prescaler);
void MoveInterruptsToBoot(void);

#endif

/*** Interrupt List ***/
// ISR(RESET_vect){}
// ISR(EXT_INT0_vect){}
// ISR(EXT_INT1_vect){}
// ISR(EXT_INT2_vect){}
// ISR(EXT_INT3_vect){}
// ISR(EXT_INT4_vect){}
// ISR(EXT_INT5_vect){}
// ISR(EXT_INT6_vect){}
// ISR(EXT_INT7_vect){}
// ISR(SPI_STC_vect){}
// ISR(USART0_RXC_vect){}
// ISR(USART0_DRE_vect){}
// ISR(USART0_TXC_vect){}
// ISR(ADC_vect){}
// ISR(EE_RDY_vect){}
// ISR(ANA_COMP_vect){}
// ISR(USART1_RXC_vect){}
// ISR(USART1_DRE_vect){}
// ISR(USART1_TXC_vect){}
// ISR(TWI_vect){}
// ISR(SPM_RDY_vect){}
// ISR(INT0_vect){ }
// ISR(INT1_vect){ }
// ISR(INT2_vect){ }
// ISR(INT3_vect){ }
// ISR(INT4_vect){ }
// ISR(INT5_vect){ }
// ISR(INT6_vect){ }
// ISR(INT7_vect){ }
// ISR(TIMER0_COMP_vect){};
// ISR(TIMER0_OVF_vect){};
// ISR(TIMER1_CAPT_vect){};
// ISR(TIMER1_COMPA_vect){};
// ISR(TIMER1_COMPB_vect){};
// ISR(TIMER1_COMPC_vect){};
// ISR(TIMER1_OVF_vect){};
// ISR(TIMER2_COMP_vect){};
// ISR(TIMER2_OVF_vect){};
// ISR(TIMER3_CAPT_vect){};
// ISR(TIMER3_COMPA_vect){};
// ISR(TIMER3_COMPB_vect){};
// ISR(TIMER3_COMPC_vect){};
// ISR(TIMER3_OVF_vect){};

/*** EOF ***/

