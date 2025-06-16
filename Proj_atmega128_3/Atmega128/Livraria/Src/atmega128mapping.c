/**********************************************************
	ATMEGA 128 MAPPING
Author:   <sergio.salazar.santos@gmail.com>
License:  GNU General Public License
Hardware: Atmega128 by ETT ET-BASE
Update:   07/01/2024
Comment:
	Virtual Image Atmega 128 mapping.
**********************************************************/
/*** File Library ***/
#include "atmega128mapping.h"

/*** File Variables ***/
static ATMEGA128 setup_atmega128;

/*** Procedure & Function ***/
ATMEGA128 atmega128_enable(void){ 
	/***GPWR***/
	setup_atmega128.gpwr_instance = gpwr_instance();
	/***AC***/
	setup_atmega128.ac_instance = ac_instance();
	setup_atmega128.ac_misc_instance = misc_instance();
	/***ADC***/
	setup_atmega128.adc_instance = adc_instance();
	#ifdef _ANALOG_MODULE_
		setup_atmega128.adc_enable = adc_enable;
		setup_atmega128.adc = adc();
	#endif
	/***BOOTLOAD***/
	setup_atmega128.bootload_instance = bootload_instance();
	/***CPU***/
	setup_atmega128.cpu_instance = cpu_instance();
	/***EEPROM***/
	setup_atmega128.eeprom_instance = eeprom_instance();
	#ifdef _EEPROM_MODULE_
		setup_atmega128.eeprom_enable = eeprom_enable;
		setup_atmega128.eeprom = eeprom();
	#endif
	/***EXINT***/
	setup_atmega128.exint_instance = exint_instance();
	#ifdef _INTERRUPT_MODULE_
		setup_atmega128.exint_enable = exint_enable;
		setup_atmega128.exint = exint();
	#endif
	/***PORTA***/
	setup_atmega128.porta_instance = porta_instance();
	/***PORTB***/
	setup_atmega128.portb_instance = portb_instance();
	/***PORTC***/
	setup_atmega128.portc_instance = portc_instance();
	/***PORTD***/
	setup_atmega128.portd_instance = portd_instance();
	/***PORTE***/
	setup_atmega128.porte_instance = porte_instance();
	/***PORTF***/
	setup_atmega128.portf_instance = portf_instance();
	/***PORTG***/
	setup_atmega128.portg_instance = portg_instance();
	/***JTAG***/
	setup_atmega128.jtag_instance = jtag_instance();
	/***MISC***/
	setup_atmega128.misc_instance = misc_instance();
	/***SPI***/
	setup_atmega128.spi_instance = spi_instance();
	#ifdef _SPI_MODULE_
		setup_atmega128.spi_enable = spi_enable;
		setup_atmega128.spi = spi();
	#endif
	/***TC1***/
	setup_atmega128.tc1_instance = tc1_instance();
	setup_atmega128.tc1_misc_instance = misc_instance();
	#ifdef _TIMER1_MODULE_
		setup_atmega128.tc1_enable = tc1_enable;
		setup_atmega128.tc1 = tc1();
	#endif
	/***TC3***/
	setup_atmega128.tc3_instance = tc3_instance();
	setup_atmega128.tc3_misc_instance = misc_instance();
	#ifdef _TIMER3_MODULE_
		setup_atmega128.tc3_enable = tc3_enable;
		setup_atmega128.tc3 = tc3();
	#endif
	/***TC2***/
	setup_atmega128.tc2_instance = tc2_instance();
	#ifdef _TIMER2_MODULE_
		setup_atmega128.tc2_enable = tc2_enable;
		setup_atmega128.tc2 = tc2();
	#endif
	/***TC0***/
	setup_atmega128.tc0_instance = tc0_instance();
	setup_atmega128.tc0_misc_instance = misc_instance();
	#ifdef _TIMER0_MODULE_
		setup_atmega128.tc0_enable = tc0_enable;
		setup_atmega128.tc0 = tc0();
	#endif
	/***TWI***/
	setup_atmega128.twi_instance = twi_instance();
	#ifdef _TWI_MODULE_
		setup_atmega128.twi_enable = twi_enable;
		setup_atmega128.twi = twi();
	#endif
	/***USART0***/
	setup_atmega128.usart0_instance = usart0_instance();
	#ifdef _USART0_MODULE_
		setup_atmega128.usart0_enable = usart0_enable;
		setup_atmega128.usart0 = usart0();
	#endif
	/***USART1***/
	setup_atmega128.usart1_instance = usart1_instance();
	#ifdef _USART1_MODULE_
		setup_atmega128.usart1_enable = usart1_enable;
		setup_atmega128.usart1 = usart1();
	#endif
	/***WDT***/
	setup_atmega128.wdt_instance = wdt_instance();
	/***Pointer Function***/
	setup_atmega128.Clock_Prescaler_Select = ClockPrescalerSelect;
	setup_atmega128.Move_Interrupts_To_Boot = MoveInterruptsToBoot;
	
	return setup_atmega128;
}

ATMEGA128* atmega128(void){ return &setup_atmega128; }

/***EOF***/

