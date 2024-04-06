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
	atmega.ac_handle = (Atmega128AnalogComparator_TypeDef*) Atmega128AnalogComparator_Address;
	atmega.ac_misc = (Atmega128OtherRegisters_TypeDef*) Atmega128OtherRegisters_Address;
	#ifdef _ANALOG_MODULE_
		atmega.adc_handle = (Atmega128AnalogToDigitalConverter_TypeDef*) Atmega128AnalogToDigitalConverter_Address;
		atmega.adc_enable = adc_enable;
		atmega.adc = adc();
	#else
		atmega.adc_handle = (Atmega128AnalogToDigitalConverter_TypeDef*) Atmega128AnalogToDigitalConverter_Address;
	#endif
	atmega.boot_load_handle = (Atmega128BootLoader_TypeDef*) Atmega128BootLoader_Address;
	atmega.cpu_handle = (Atmega128CPURegister_TypeDef*) Atmega128CPURegister_Address;
	#ifdef _EEPROM_MODULE_
		atmega.eeprom_handle = (Atmega128Eeprom_TypeDef*) Atmega128Eeprom_Address;
		atmega.eeprom_enable = eeprom_enable;
		atmega.eeprom = eeprom();
	#else
		atmega.eeprom_handle = (Atmega128Eeprom_TypeDef*) Atmega128Eeprom_Address;
	#endif
	#ifdef _INTERRUPT_MODULE_
		atmega.exint_handle = (Atmega128ExternalInterrupts_TypeDef*) Atmega128ExternalInterrupts_Address;
		atmega.exint_enable = exint_enable;
		atmega.exint = exint();
	#else
		atmega.exint_handle = (Atmega128ExternalInterrupts_TypeDef*) Atmega128ExternalInterrupts_Address;
	#endif
	atmega.porta_handle = (Atmega128PORTA_TypeDef*) Atmega128PORTA_Address;
	atmega.portb_handle = (Atmega128PORTB_TypeDef*) Atmega128PORTB_Address;
	atmega.portc_handle = (Atmega128PORTC_TypeDef*) Atmega128PORTC_Address;
	atmega.portd_handle = (Atmega128PORTD_TypeDef*) Atmega128PORTD_Address;
	atmega.porte_handle = (Atmega128PORTE_TypeDef*) Atmega128PORTE_Address;
	atmega.portf_handle = (Atmega128PORTF_TypeDef*) Atmega128PORTF_Address;
	atmega.portg_handle = (Atmega128PORTG_TypeDef*) Atmega128PORTG_Address;
	atmega.jtag_handle = (Atmega128JtagInterface_TypeDef*) Atmega128JtagInterface_Address;
	atmega.misc_handle = (Atmega128OtherRegisters_TypeDef*) Atmega128OtherRegisters_Address;
	#ifdef _SPI_MODULE_
		atmega.spi_handle = (Atmega128SerialPeripherialInterface_TypeDef*) Atmega128SerialPeripherialInterface_Address;
		atmega.spi_enable = spi_enable;
		atmega.spi = spi();
	#else
		atmega.spi_handle = (Atmega128SerialPeripherialInterface_TypeDef*) Atmega128SerialPeripherialInterface_Address;
	#endif
	#ifdef _TIMER1_MODULE_
		atmega.tc1_handle = (Atmega128TimerCounter1_TypeDef*) Atmega128TimerCounter1_Address;
		atmega.tc1_misc = (Atmega128OtherRegisters_TypeDef*) Atmega128OtherRegisters_Address;
		atmega.tc1_enable = tc1_enable;
		atmega.tc1 = tc1();
	#else
		atmega.tc1_handle = (Atmega128TimerCounter1_TypeDef*) Atmega128TimerCounter1_Address;
		atmega.tc1_misc = (Atmega128OtherRegisters_TypeDef*) Atmega128OtherRegisters_Address;
	#endif
	#ifdef _TIMER3_MODULE_
		atmega.tc3_handle = (Atmega128TimerCounter3_TypeDef*) Atmega128TimerCounter3_Address;
		atmega.tc3_misc = (Atmega128OtherRegisters_TypeDef*) Atmega128OtherRegisters_Address;
		atmega.tc3_enable = tc3_enable;
		atmega.tc3 = tc3();
	#else
		atmega.tc3_handle = (Atmega128TimerCounter3_TypeDef*) Atmega128TimerCounter3_Address;
		atmega.tc3_misc = (Atmega128OtherRegisters_TypeDef*) Atmega128OtherRegisters_Address;
	#endif
	#ifdef _TIMER2_MODULE_
		atmega.tc2_handle = (Atmega128TimerCounter2_TypeDef*) Atmega128TimerCounter2_Address;
		atmega.tc2_enable = tc2_enable;
		atmega.tc2 = tc2();
	#else
		atmega.tc2_handle = (Atmega128TimerCounter2_TypeDef*) Atmega128TimerCounter2_Address;
	#endif
	#ifdef _TIMER0_MODULE_
		atmega.tc0_handle = (Atmega128TimerCounter0_TypeDef*) Atmega128TimerCounter0_Address;
		atmega.tc0_misc = (Atmega128OtherRegisters_TypeDef*) Atmega128OtherRegisters_Address;
		atmega.tc0_enable = tc0_enable;
		atmega.tc0 = tc0();
	#else
		atmega.tc0_handle = (Atmega128TimerCounter0_TypeDef*) Atmega128TimerCounter0_Address;
		atmega.tc0_misc = (Atmega128OtherRegisters_TypeDef*) Atmega128OtherRegisters_Address;
	#endif
	#ifdef _TWI_MODULE_
		atmega.twi_handle = (Atmega128TwoWireSerialInterface_TypeDef*) Atmega128TwoWireSerialInterface_Address;
		atmega.twi_enable = twi_enable;
		atmega.twi = twi();
	#else
		atmega.twi_handle = (Atmega128TwoWireSerialInterface_TypeDef*) Atmega128TwoWireSerialInterface_Address;
	#endif
	#ifdef _USART0_MODULE_
		atmega.usart0_handle = (Atmega128Usart0_TypeDef*) Atmega128Usart0_Address;
		atmega.usart0_enable = usart0_enable;
		atmega.usart0 = usart0();
	#else
		atmega.usart0_handle = (Atmega128Usart0_TypeDef*) Atmega128Usart0_Address;
	#endif
		atmega.usart0_handle = (Atmega128Usart0_TypeDef*) Atmega128Usart0_Address;
	#ifdef _USART1_MODULE_
		atmega.usart1_handle = (Atmega128Usart1_TypeDef*) Atmega128Usart1_Address;
		atmega.usart1_enable = usart1_enable;
		atmega.usart1 = usart1();
	#else
		atmega.usart1_handle = (Atmega128Usart1_TypeDef*) Atmega128Usart1_Address;
	#endif
	atmega.wdt_handle = (Atmega128WatchdogTimer_TypeDef*) Atmega128WatchdogTimer_Address;
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

