/**********************************************************
	ATMEGA 128 MAPPING
Author:   <sergio.salazar.santos@gmail.com>
License:  GNU General Public License
Hardware: Atmega128 by ETT ET-BASE
Date:   07/01/2024
**********************************************************/
/*** File Library ***/
#include "atmega128mapping.h"

/*** File Variables ***/
static ATMEGA128 setup_atmega128;

/*** Procedure & Function ***/
ATMEGA128 atmega128_enable(void){ 
	/***GPWR***/
	setup_atmega128.gpwr_reg = gpwr_reg();
	/***AC***/
	setup_atmega128.ac_reg = ac_reg();
	setup_atmega128.ac_misc_reg = misc_reg();
	/***ADC***/
	setup_atmega128.adc_reg = adc_reg();
	#ifdef _ANALOG_MODULE_
		setup_atmega128.adc_enable = adc_enable;
		setup_atmega128.adc = adc();
	#endif
	/***BOOTLOAD***/
	setup_atmega128.bootload_reg = bootload_reg();
	/***CPU***/
	setup_atmega128.cpu_reg = cpu_reg();
	/***EEPROM***/
	setup_atmega128.eeprom_reg = eeprom_reg();
	#ifdef _EEPROM_MODULE_
		setup_atmega128.eeprom = eeprom();
	#endif
	/***EXINT***/
	setup_atmega128.exint_reg = exint_reg();
	#ifdef _INTERRUPT_MODULE_
		setup_atmega128.exint = exint();
	#endif
	/***PORTA***/
	setup_atmega128.gpioa_reg = gpioa_reg();
	/***PORTB***/
	setup_atmega128.gpiob_reg = gpiob_reg();
	/***PORTC***/
	setup_atmega128.gpioc_reg = gpioc_reg();
	/***PORTD***/
	setup_atmega128.gpiod_reg = gpiod_reg();
	/***PORTE***/
	setup_atmega128.gpioe_reg = gpioe_reg();
	/***PORTF***/
	setup_atmega128.gpiof_reg = gpiof_reg();
	/***PORTG***/
	setup_atmega128.gpiog_reg = gpiog_reg();
	/***JTAG***/
	setup_atmega128.jtag_reg = jtag_reg();
	/***MISC***/
	setup_atmega128.misc_reg = misc_reg();
	/***SPI***/
	setup_atmega128.spi_reg = spi_reg();
	#ifdef _SPI_MODULE_
		setup_atmega128.spi_enable = spi_enable;
		setup_atmega128.spi = spi();
	#endif
	/***TC1***/
	setup_atmega128.tc1_reg = tc1_reg();
	setup_atmega128.tc1_misc_reg = misc_reg();
	#ifdef _TIMER1_MODULE_
		setup_atmega128.tc1_enable = tc1_enable;
		setup_atmega128.tc1 = tc1();
	#endif
	/***TC3***/
	setup_atmega128.tc3_reg = tc3_reg();
	setup_atmega128.tc3_misc_reg = misc_reg();
	#ifdef _TIMER3_MODULE_
		setup_atmega128.tc3_enable = tc3_enable;
		setup_atmega128.tc3 = tc3();
	#endif
	/***TC2***/
	setup_atmega128.tc2_reg = tc2_reg();
	#ifdef _TIMER2_MODULE_
		setup_atmega128.tc2_enable = tc2_enable;
		setup_atmega128.tc2 = tc2();
	#endif
	/***TC0***/
	setup_atmega128.tc0_reg = tc0_reg();
	setup_atmega128.tc0_misc_reg = misc_reg();
	#ifdef _TIMER0_MODULE_
		setup_atmega128.tc0_enable = tc0_enable;
		setup_atmega128.tc0 = tc0();
	#endif
	/***TWI***/
	setup_atmega128.twi_reg = twi_reg();
	#ifdef _TWI_MODULE_
		setup_atmega128.twi_enable = twi_enable;
		setup_atmega128.twi = twi();
	#endif
	/***USART0***/
	setup_atmega128.usart0_reg = usart0_reg();
	#ifdef _USART0_MODULE_
		setup_atmega128.usart0_enable = usart0_enable;
		setup_atmega128.usart0 = usart0();
	#endif
	/***USART1***/
	setup_atmega128.usart1_reg = usart1_reg();
	#ifdef _USART1_MODULE_
		setup_atmega128.usart1_enable = usart1_enable;
		setup_atmega128.usart1 = usart1();
	#endif
	/***WDT***/
	setup_atmega128.wdt_reg = wdt_reg();
	/***Pointer Function***/
	setup_atmega128.Clock_Prescaler_Select = ClockPrescalerSelect;
	setup_atmega128.Move_Interrupts_To_Boot = MoveInterruptsToBoot;
	
	return setup_atmega128;
}

ATMEGA128* atmega128(void){ return &setup_atmega128; }

/***EOF***/

