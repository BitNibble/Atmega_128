/*********************************************************
	ATMEGA 128 MAPPING
Author: Sergio Manuel Santos
	<sergio.salazar.santos@gmail.com>
License: GNU General Public License
Hardware: Atmega128 by ETT ET-BASE
Update: 07/01/2024
Comment:
	Virtual Image Atmega 128 mapping.
**********************************************************/
/*** File Library ***/
#include "atmega128mapping.h"

/*** File Variables ***/
static ATMEGA128 setup_atmega128;

/*** Procedure & Function ***/
ATMEGA128 atmega128_enable(void){ 
	
	setup_atmega128.gpwr_handle = gpwr_handle();
	setup_atmega128.ac_handle = ac_handle();
	setup_atmega128.ac_misc = misc_handle();
	#ifdef _ANALOG_MODULE_
		setup_atmega128.adc_handle = adc_handle();
		setup_atmega128.adc_enable = adc_enable;
		setup_atmega128.adc = adc();
	#else
		setup_atmega128.adc_handle = adc_handle;
	#endif
	setup_atmega128.boot_load_handle = bootload_handle();
	setup_atmega128.cpu_handle = cpu_handle();
	#ifdef _EEPROM_MODULE_
		setup_atmega128.eeprom_handle = eeprom_handle();
		setup_atmega128.eeprom_enable = eeprom_enable;
		setup_atmega128.eeprom = eeprom();
	#else
		setup_atmega128.eeprom_handle = eeprom_handle;
	#endif
	#ifdef _INTERRUPT_MODULE_
		setup_atmega128.exint_handle = exint_handle();
		setup_atmega128.exint_enable = exint_enable;
		setup_atmega128.exint = exint();
	#else
		setup_atmega128.exint_handle = exint_handle();
	#endif
	setup_atmega128.porta_handle = porta_handle();
	setup_atmega128.portb_handle = portb_handle();
	setup_atmega128.portc_handle = portc_handle();
	setup_atmega128.portd_handle = portd_handle();
	setup_atmega128.porte_handle = porte_handle();
	setup_atmega128.portf_handle = portf_handle();
	setup_atmega128.portg_handle = portg_handle();
	setup_atmega128.jtag_handle = jtag_handle();
	setup_atmega128.misc_handle = misc_handle();
	#ifdef _SPI_MODULE_
		setup_atmega128.spi_handle = spi_handle();
		setup_atmega128.spi_enable = spi_enable;
		setup_atmega128.spi = spi();
	#else
		setup_atmega128.spi_handle = spi_handle();
	#endif
	#ifdef _TIMER1_MODULE_
		setup_atmega128.tc1_handle = tc1_handle();
		setup_atmega128.tc1_misc_handle = misc_handle();
		setup_atmega128.tc1_enable = tc1_enable;
		setup_atmega128.tc1 = tc1();
	#else
		setup_atmega128.tc1_handle = tc1_handle();
		setup_atmega128.tc1_misc_handle = misc_handle();
	#endif
	#ifdef _TIMER3_MODULE_
		setup_atmega128.tc3_handle = tc3_handle();
		setup_atmega128.tc3_misc_handle = misc_handle();
		setup_atmega128.tc3_enable = tc3_enable;
		setup_atmega128.tc3 = tc3();
	#else
		setup_atmega128.tc3_handle = tc3_handle();
		setup_atmega128.tc3_misc_handle = misc_handle();
	#endif
	#ifdef _TIMER2_MODULE_
		setup_atmega128.tc2_handle = tc2_handle();
		setup_atmega128.tc2_enable = tc2_enable;
		setup_atmega128.tc2 = tc2();
	#else
		setup_atmega128.tc2_handle = tc2_handle();
	#endif
	#ifdef _TIMER0_MODULE_
		setup_atmega128.tc0_handle = tc0_handle();
		setup_atmega128.tc0_misc_handle = misc_handle();
		setup_atmega128.tc0_enable = tc0_enable;
		setup_atmega128.tc0 = tc0();
	#else
		setup_atmega128.tc0_handle = tc0_handle();
		setup_atmega128.tc0_misc_handle = misc_handle();
	#endif
	#ifdef _TWI_MODULE_
		setup_atmega128.twi_handle = twi_handle();
		setup_atmega128.twi_enable = twi_enable;
		setup_atmega128.twi = twi();
	#else
		setup_atmega128.twi_handle = twi_handle();
	#endif
	#ifdef _USART0_MODULE_
		setup_atmega128.usart0_handle = usart0_handle();
		setup_atmega128.usart0_enable = usart0_enable;
		setup_atmega128.usart0 = usart0();
	#else
		setup_atmega128.usart0_handle = usart0_handle();
	#endif
	#ifdef _USART1_MODULE_
		setup_atmega128.usart1_handle = usart1_handle();
		setup_atmega128.usart1_enable = usart1_enable;
		setup_atmega128.usart1 = usart1();
	#else
		setup_atmega128.usart1_handle = usart1_handle();
	#endif
	setup_atmega128.wdt_handle = wdt_handle();
	/******/
	setup_atmega128.Clock_Prescaler_Select = ClockPrescalerSelect;
	setup_atmega128.Move_Interrupts_To_Boot = MoveInterruptsToBoot;
	
	return setup_atmega128;
}

ATMEGA128* atmega128(void){ return &setup_atmega128; }

/***EOF***/

