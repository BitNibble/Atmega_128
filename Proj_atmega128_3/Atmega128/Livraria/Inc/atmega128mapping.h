/*********************************************************
	ATMEGA 128 MAPPING
Author:   <sergio.salazar.santos@gmail.com>
License:  GNU General Public License
Hardware: Atmega128 by ETT ET-BASE
Update:   07/01/2024
Comment: 
	Virtual Image Atmega 128 mapping.
	NB:
	- This Header file should be the first to be placed
**********************************************************/
#ifndef _ATMEGA128MAPPING_H_
	#define _ATMEGA128MAPPING_H_

#include "atmega128.h"

/*********************************************************/
/****************** Include Switches  ********************/
/*********************************************************/
// User may Comment out if not in use
// MODULES
#define _ANALOG_MODULE_
#define _TIMER0_MODULE_
#define _TIMER1_MODULE_
#define _TIMER2_MODULE_
#define _TIMER3_MODULE_
#define _EEPROM_MODULE_
#define _INTERRUPT_MODULE_
#define _SPI_MODULE_
#define _TWI_MODULE_
#define _USART0_MODULE_
#define _USART1_MODULE_

/*********************************************************/
/****************** Hardware Includes ********************/
/*********************************************************/
// MODULES
#ifdef _ANALOG_MODULE_
	#include "atmega128analog.h"
//#else
//	#warning "NO ANALOG MODULE"
#endif
#ifdef _TIMER0_MODULE_
	#include "atmega128timer0.h"
//#else
//	#warning "NO TIMER0 MODULE"
#endif
#ifdef _TIMER1_MODULE_
#include "atmega128timer1.h"
//#else
//	#warning "NO TIMER1 MODULE"
#endif
#ifdef _TIMER2_MODULE_
#include "atmega128timer2.h"
//#else
//	#warning "NO TIMER2 MODULE"
#endif
#ifdef _TIMER3_MODULE_
#include "atmega128timer3.h"
//#else
//	#warning "NO TIMER3 MODULE"
#endif
#ifdef _EEPROM_MODULE_
	#include "atmegaeeprom.h"
//#else
//	#warning "NO EEPROM MODULE"
#endif
#ifdef _INTERRUPT_MODULE_
	#include "atmega128interrupt.h"
//#else
//	#warning "NO INTERRUPT MODULE"
#endif
#ifdef _SPI_MODULE_
#include "atmega128spi.h"
//#else
//	#warning "NO SPI MODULE"
#endif
#ifdef _TWI_MODULE_
	#include "atmega128twi.h"
//#else
//	#warning "NO TWI MODULE"
#endif
#ifdef _USART0_MODULE_
#include "atmega128usart0.h"
//#else
//	#warning "NO UART0 MODULE"
#endif
#ifdef _USART1_MODULE_
#include "atmega128usart1.h"
//#else
//	#warning "NO UART1 MODULE"
#endif

/*******************************************************************/
/************************* ATMEGA 128 IMAGE ************************/
/*************************** Third Layer ***************************/
/*******************************************************************/
typedef struct {
	/***GPWR***/
	Atmega128GPWR_TypeDef* gpwr_instance;
	/***AC***/
	Atmega128AnalogComparator_TypeDef* ac_instance;
	Atmega128OtherRegisters_TypeDef* ac_misc_instance;
	/***ADC***/
	Atmega128AnalogToDigitalConverter_TypeDef* adc_instance;
	#ifdef _ANALOG_MODULE_
		ADC0 (*adc_enable)( uint8_t Vreff, uint8_t Divfactor, int n_channel, ... );
		ADC0* adc;
	#endif
	/***BOOTLOAD***/
	Atmega128BootLoader_TypeDef* bootload_instance;
	/***CPU***/
	Atmega128CPURegister_TypeDef* cpu_instance;
	/***EEPROM***/
	Atmega128Eeprom_TypeDef* eeprom_instance;
	#ifdef _EEPROM_MODULE_
		EEPROM0 (*eeprom_enable)( void );
		EEPROM0* eeprom;
	#endif
	/***EXINT***/
	Atmega128ExternalInterrupts_TypeDef* exint_instance;
	#ifdef _INTERRUPT_MODULE_
		EXINT0 (*exint_enable)( void );
		EXINT0* exint;
	#endif
	/***PORTA***/
	Atmega128PORTA_TypeDef* porta_instance;
	/***PORTB***/
	Atmega128PORTB_TypeDef* portb_instance;
	/***PORTC***/
	Atmega128PORTC_TypeDef* portc_instance;
	/***PORTD***/
	Atmega128PORTD_TypeDef* portd_instance;
	/***PORTE***/
	Atmega128PORTE_TypeDef* porte_instance;
	/***PORTF***/
	Atmega128PORTF_TypeDef* portf_instance;
	/***PORTG***/
	Atmega128PORTG_TypeDef* portg_instance;
	/***JTAG***/
	Atmega128JtagInterface_TypeDef* jtag_instance;
	/***MISC***/
	Atmega128OtherRegisters_TypeDef* misc_instance;
	/***SPI***/
	Atmega128SerialPeripheralInterface_TypeDef* spi_instance;
	#ifdef _SPI_MODULE_
		SPI0 (*spi_enable)( uint8_t master_slave_select, uint8_t data_order,  uint8_t data_modes, uint8_t prescaler );
		SPI0* spi;
	#endif
	/***TC1***/
	Atmega128TimerCounter1_TypeDef* tc1_instance;
	Atmega128OtherRegisters_TypeDef* tc1_misc_instance;
	#ifdef _TIMER1_MODULE_
		TC1 (*tc1_enable)( unsigned char wavegenmode, unsigned char interrupt );
		TC1* tc1;
	#endif
	/***TC3***/
	Atmega128TimerCounter3_TypeDef* tc3_instance;
	Atmega128OtherRegisters_TypeDef* tc3_misc_instance;
	#ifdef _TIMER3_MODULE_
		TC3 (*tc3_enable)( unsigned char wavegenmode, unsigned char interrupt );
		TC3* tc3;
	#endif
	/***TC2***/
	Atmega128TimerCounter2_TypeDef* tc2_instance;
	#ifdef _TIMER2_MODULE_
		TC2 (*tc2_enable)( unsigned char wavegenmode, unsigned char interrupt );
		TC2* tc2;
	#endif
	/***TC0***/
	Atmega128TimerCounter0_TypeDef* tc0_instance;
	Atmega128OtherRegisters_TypeDef* tc0_misc_instance;
	#ifdef _TIMER0_MODULE_
		TC0 (*tc0_enable)( unsigned char wavegenmode, unsigned char interrupt );
		TC0* tc0;
	#endif
	/***TWI***/
	Atmega128TwoWireSerialInterface_TypeDef* twi_instance;
	#ifdef _TWI_MODULE_
		TWI0 (*twi_enable)(uint8_t atmega_ID,  uint8_t prescaler);
		TWI0* twi;
	#endif
	/***USART0***/
	Atmega128Usart0_TypeDef* usart0_instance;
	#ifdef _USART0_MODULE_
		USART0 (*usart0_enable)( uint32_t baudrate, unsigned int FDbits, unsigned int Stopbits, unsigned int Parity );
		USART0* usart0;
	#endif
	/***USART1***/
	Atmega128Usart1_TypeDef* usart1_instance;
	#ifdef _USART1_MODULE_
		USART1 (*usart1_enable)( uint32_t baud, unsigned int FDbits, unsigned int Stopbits, unsigned int Parity );
		USART1* usart1;
	#endif
	/***WDT***/
	Atmega128WatchdogTimer_TypeDef* wdt_instance;
	/***Pointer Function***/
	void (*Clock_Prescaler_Select)( volatile uint8_t prescaler );
	void (*Move_Interrupts_To_Boot)( void );
}ATMEGA128;

/*** Global ***/
ATMEGA128* atmega128( void );
ATMEGA128 atmega128_enable( void );

#endif

/***EOF***/

