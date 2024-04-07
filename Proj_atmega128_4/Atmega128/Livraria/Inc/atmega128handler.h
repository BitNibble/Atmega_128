/*********************************************************
	ATMEGA 128 Handler
Author: Sergio Manuel Santos
	<sergio.salazar.santos@gmail.com>
License: GNU General Public License
Hardware: Atmega 128
Update:	14/01/2024
Comment: 
	Simple tools
**********************************************************/
#ifndef _ATMEGA128HANDLER_H_
	#define _ATMEGA128HANDLER_H_

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

#endif

/*** EOF ***/

