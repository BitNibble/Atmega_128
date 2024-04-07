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
static ATMEGA128 atmega;
/**********************/
/*** File Header ***/
void ClockPrescalerSelect(volatile uint8_t prescaler);
void MoveInterruptsToBoot(void);

/*** Procedure & Function ***/
ATMEGA128 atmega128_enable(void){ 
	
	atmega.gpwr_handle = gpwr_handle();
	atmega.ac_handle = ac_handle();
	atmega.ac_misc = misc_handle();
	#ifdef _ANALOG_MODULE_
		atmega.adc_handle = adc_handle();
		atmega.adc_enable = adc_enable;
		atmega.adc = adc();
	#else
		atmega.adc_handle = adc_handle;
	#endif
	atmega.boot_load_handle = bootload_handle();
	atmega.cpu_handle = cpu_handle();
	#ifdef _EEPROM_MODULE_
		atmega.eeprom_handle = eeprom_handle();
		atmega.eeprom_enable = eeprom_enable;
		atmega.eeprom = eeprom();
	#else
		atmega.eeprom_handle = eeprom_handle;
	#endif
	#ifdef _INTERRUPT_MODULE_
		atmega.exint_handle = exint_handle();
		atmega.exint_enable = exint_enable;
		atmega.exint = exint();
	#else
		atmega.exint_handle = exint_handle();
	#endif
	atmega.porta_handle = porta_handle();
	atmega.portb_handle = portb_handle();
	atmega.portc_handle = portc_handle();
	atmega.portd_handle = portd_handle();
	atmega.porte_handle = porte_handle();
	atmega.portf_handle = portf_handle();
	atmega.portg_handle = portg_handle();
	atmega.jtag_handle = jtag_handle();
	atmega.misc_handle = misc_handle();
	#ifdef _SPI_MODULE_
		atmega.spi_handle = spi_handle();
		atmega.spi_enable = spi_enable;
		atmega.spi = spi();
	#else
		atmega.spi_handle = spi_handle();
	#endif
	#ifdef _TIMER1_MODULE_
		atmega.tc1_handle = tc1_handle();
		atmega.tc1_misc = misc_handle();
		atmega.tc1_enable = tc1_enable;
		atmega.tc1 = tc1();
	#else
		atmega.tc1_handle = tc1_handle();
		atmega.tc1_misc = misc_handle();
	#endif
	#ifdef _TIMER3_MODULE_
		atmega.tc3_handle = tc3_handle();
		atmega.tc3_misc = misc_handle();
		atmega.tc3_enable = tc3_enable;
		atmega.tc3 = tc3();
	#else
		atmega.tc3_handle = tc3_handle();
		atmega.tc3_misc = misc_handle();
	#endif
	#ifdef _TIMER2_MODULE_
		atmega.tc2_handle = tc2_handle();
		atmega.tc2_enable = tc2_enable;
		atmega.tc2 = tc2();
	#else
		atmega.tc2_handle = tc2_handle();
	#endif
	#ifdef _TIMER0_MODULE_
		atmega.tc0_handle = tc0_handle();
		atmega.tc0_misc = misc_handle();
		atmega.tc0_enable = tc0_enable;
		atmega.tc0 = tc0();
	#else
		atmega.tc0_handle = tc0_handle();
		atmega.tc0_misc = misc_handle();
	#endif
	#ifdef _TWI_MODULE_
		atmega.twi_handle = twi_handle();
		atmega.twi_enable = twi_enable;
		atmega.twi = twi();
	#else
		atmega.twi_handle = twi_handle();
	#endif
	#ifdef _USART0_MODULE_
		atmega.usart0_handle = usart0_handle();
		atmega.usart0_enable = usart0_enable;
		atmega.usart0 = usart0();
	#else
		atmega.usart0_handle = usart0_handle();
	#endif
	#ifdef _USART1_MODULE_
		atmega.usart1_handle = usart1_handle();
		atmega.usart1_enable = usart1_enable;
		atmega.usart1 = usart1();
	#else
		atmega.usart1_handle = usart1_handle();
	#endif
	atmega.wdt_handle = wdt_handle();
	/******/
	atmega.Clock_Prescaler_Select = ClockPrescalerSelect;
	atmega.Move_Interrupts_To_Boot = MoveInterruptsToBoot;
	
	return atmega;
}

ATMEGA128* atmega128(void){ return &atmega; }

/****** System ******/
void ClockPrescalerSelect(volatile uint8_t prescaler)
{
	volatile uint8_t sreg;
	volatile uint8_t* clkpr = &XDIV;
	prescaler &= 0x7F;
	sreg = atmega.cpu_handle->sreg.reg;
	atmega.cpu_handle->sreg.reg &= ~(1 << 7);
	
	*clkpr = prescaler;
	*clkpr = (1 << XDIVEN) | prescaler;
	
	atmega.cpu_handle->sreg.reg = sreg;
}
void MoveInterruptsToBoot(void)
{
	volatile uint8_t sreg;
	sreg = atmega.cpu_handle->sreg.reg;
	atmega.cpu_handle->sreg.reg &= ~(1 << 7);
	
	MCUCR = (1<<IVCE);
	MCUCR = (1<<IVSEL);
	
	atmega.cpu_handle->sreg.reg = sreg;
}

/***EOF***/

