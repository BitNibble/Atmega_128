/**********************************************************************
	ATMEGA 128 MAPPING
Author:   <sergio.salazar.santos@gmail.com>
License:  GNU General Public License
Hardware: Atmega128 by ETT ET-BASE
Date:     15/07/2025
**********************************************************************/
/*** Library ***/
#include "atmega128mapping.h"

/*** Handler ***/
static const ATMEGA128 setup_atmega128 = { 
	/***ADC***/
	#ifdef _ANALOG_MODULE_
		.adc_enable = adc_enable,
	#endif
	/***SPI***/
	#ifdef _SPI_MODULE_
		.spi_enable = spi_enable,
	#endif
	/***TC1***/
	#ifdef _TIMER1_MODULE_
		.tc1_enable = tc1_enable,
	#endif
	/***TC3***/
	#ifdef _TIMER3_MODULE_
		.tc3_enable = tc3_enable,
	#endif
	/***TC2***/
	#ifdef _TIMER2_MODULE_
		.tc2_enable = tc2_enable,
	#endif
	/***TC0***/
	#ifdef _TIMER0_MODULE_
		.tc0_enable = tc0_enable,
	#endif
	/***TWI***/
	#ifdef _TWI_MODULE_
		.twi_enable = twi_enable,
	#endif
	/***USART0***/
	#ifdef _USART0_MODULE_
		.usart0_enable = usart0_enable,
	#endif
	/***USART1***/
	#ifdef _USART1_MODULE_
		.usart1_enable = usart1_enable,
	#endif
	/***Pointer Function***/
	.Clock_Prescaler_Select = ClockPrescalerSelect,
	.Move_Interrupts_To_Boot = MoveInterruptsToBoot
};

ATMEGA128* mega128(void){ return (ATMEGA128*) &setup_atmega128; }

/*** EOF ***/

