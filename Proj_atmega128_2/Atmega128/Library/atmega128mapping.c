/*********************************************************
	ATMEGA 128 MAPPING
Author: Sergio Manuel Santos
	<sergio.salazar.santos@gmail.com>
License: GNU General Public License
Hardware: Atmega128 by ETT ET-BASE
Date: 06122022
Comment:
	Virtual Image Atmega 128 mapping.
**********************************************************/
/*** File Library ***/
#include "atmega128mapping.h"

/*** File Define & Macro ***/
// FLASH
#define Atmega128InterruptVectors_Address 0x0100 // ISVEL = 0

/*** File Header ***/
uint16_t ReadHLByte(HighLowByte reg);
uint16_t ReadLHByte(HighLowByte reg);
HighLowByte WriteHLByte(uint16_t val);
HighLowByte WriteLHByte(uint16_t val);
uint16_t SwapByte(uint16_t num);

/*** Procedure & Function ***/
ATMEGA128 ATMEGA128enable(void){ 
	ATMEGA128 ret;
	// Assign
	ret.gpwr.reg = (Atmega128GPWR_TypeDef*) Atmega128GPWR_Address;
	ret.ac.reg = (Atmega128AnalogComparator_TypeDef*) Atmega128AnalogComparator_Address;
	// ADC
	ret.adc.reg = (Atmega128AnalogToDigitalConverter_TypeDef*) Atmega128AnalogToDigitalConverter_Address;
	#if defined(_ATMEGA128ANALOG_H_)
		ret.adc.enable = ANALOGenable;
	#endif
	ret.boot_load.reg = (Atmega128BootLoader_TypeDef*) Atmega128BootLoader_Address;
	ret.cpu.reg = (Atmega128CPURegister_TypeDef*) Atmega128CPURegister_Address;
	// EEPROM
	ret.eeprom.reg = (Atmega128Eeprom_TypeDef*) Atmega128Eeprom_Address;
	#if defined(_EEPROM_H_)
		ret.eeprom.enable = EEPROMenable;
	#endif
	// EXINT
	ret.exint.reg = (Atmega128ExternalInterrupts_TypeDef*) Atmega128ExternalInterrupts_Address;
	#if defined(_ATMEGA128INTERRUPT_H_)
		ret.exint.enable = INTERRUPTenable;
	#endif
	// PORTA
	ret.porta.reg = (Atmega128PORTA_TypeDef*) Atmega128PORTA_Address;
	// PORTB
	ret.portb.reg = (Atmega128PORTB_TypeDef*) Atmega128PORTB_Address;
	// PORTC
	ret.portc.reg = (Atmega128PORTC_TypeDef*) Atmega128PORTC_Address;
	// PORTD
	ret.portd.reg = (Atmega128PORTD_TypeDef*) Atmega128PORTD_Address;
	// PORTE
	ret.porte.reg = (Atmega128PORTE_TypeDef*) Atmega128PORTE_Address;
	// PORTF
	ret.portf.reg = (Atmega128PORTF_TypeDef*) Atmega128PORTF_Address;
	// PORTG
	ret.portg.reg = (Atmega128PORTG_TypeDef*) Atmega128PORTG_Address;
	// JTAG
	ret.jtag.reg = (Atmega128JtagInterface_TypeDef*) Atmega128JtagInterface_Address;
	// MISC
	ret.misc.reg = (Atmega128OtherRegisters_TypeDef*) Atmega128OtherRegisters_Address;
	// SPI
	ret.spi.reg = (Atmega128SerialPeripherialInterface_TypeDef*) Atmega128SerialPeripherialInterface_Address;
	#if defined(_ATMEGA128SPI_H_)
		ret.spi.enable = SPIenable;
	#endif
	// TC1
	ret.tc1.reg = (Atmega128TimerCounter1_TypeDef*) Atmega128TimerCounter1_Address;
	#if defined(_ATMEGA128TIMER_H_)
		ret.tc1.enable = TIMER_COUNTER1enable;
	#endif
	// TC3
	ret.tc3.reg = (Atmega128TimerCounter3_TypeDef*) Atmega128TimerCounter3_Address;
	#if defined(_ATMEGA128TIMER_H_)
		ret.tc3.enable = TIMER_COUNTER3enable;
	#endif
	// TC2
	ret.tc2.reg = (Atmega128TimerCounter2_TypeDef*) Atmega128TimerCounter2_Address;
	#if defined(_ATMEGA128TIMER_H_)
		ret.tc2.enable = TIMER_COUNTER2enable;
	#endif
	// TC0
	ret.tc0.reg = (Atmega128TimerCounter0_TypeDef*) Atmega128TimerCounter0_Address;
	#if defined(_ATMEGA128TIMER_H_)
		ret.tc0.enable = TIMER_COUNTER0enable;
	#endif
	// TWI
	ret.twi.reg = (Atmega128TwoWireSerialInterface_TypeDef*) Atmega128TwoWireSerialInterface_Address;
	#if defined(_ATMEGA128TWI_H_)
		ret.twi.enable = TWIenable;
	#endif
	// USART0
	ret.usart0.reg = (Atmega128Usart0_TypeDef*) Atmega128Usart0_Address;
	#if defined(_ATMEGA128UART_H_)
		ret.usart0.enable = UART0enable;
	#endif
	// USART1
	ret.usart1.reg = (Atmega128Usart1_TypeDef*) Atmega128Usart1_Address;
	#if defined(_ATMEGA128UART_H_)
		ret.usart1.enable = UART1enable;
	#endif
	ret.wdt.reg = (Atmega128WatchdogTimer_TypeDef*) Atmega128WatchdogTimer_Address;
	// FLASH
	// ret.isr.reg = (Atmega128InterruptVectors_TypeDef*) Atmega128InterruptVectors_Address;
	// func

	ret.readhlbyte = ReadHLByte;
	ret.readlhbyte = ReadLHByte;
	ret.writehlbyte = WriteHLByte;
	ret.writelhbyte = WriteLHByte;
	ret.swapbyte = SwapByte;
	return ret;
}

// COMMON
uint16_t ReadHLByte(HighLowByte reg)
{
	return (reg.H << 8) | reg.L;
}

uint16_t ReadLHByte(HighLowByte reg)
{
	return (reg.L << 8) | reg.H;
}

HighLowByte WriteHLByte(uint16_t val)
{
	HighLowByte reg; reg.H = (val >> 8); reg.L = val;
	return reg;
}

HighLowByte WriteLHByte(uint16_t val)
{
	HighLowByte reg; reg.L = (val >> 8); reg.H = val;
	return reg;
}

uint16_t SwapByte(uint16_t num)
{
	uint16_t tp;
	tp = (num << 8);
	return (num >> 8) | tp;
}

/*** File Interrupt ***/
// ISR(RESET_vect){}
// ISR(EXT_INT0_vect){}
// ISR(EXT_INT1_vect){}
// ISR(EXT_INT2_vect){}
// ISR(EXT_INT3_vect){}
// ISR(EXT_INT4_vect){}
// ISR(EXT_INT5_vect){}
// ISR(EXT_INT6_vect){}
// ISR(EXT_INT7_vect){}
// ISR(TIM2_COMP_vect){}
// ISR(TIM2_OVF_vect){}
// ISR(TIM1_CAPT_vect){}
// ISR(TIM1_COMPA_vect){}
// ISR(TIM1_COMPB_vect){}
// ISR(TIM1_OVF_vect){}
// ISR(TIM0_COMP_vect){}
// ISR(TIM0_OVF_vect){}
// ISR(SPI_STC_vect){}
// ISR(USART0_RXC_vect){}
// ISR(USART0_DRE_vect){}
// ISR(USART0_TXC_vect){}
// ISR(ADC_vect){}
// ISR(EE_RDY_vect){}
// ISR(ANA_COMP_vect){}
// ISR(TIM1_COMPC_vect){}
// ISR(TIM3_CAPT_vect){}
// ISR(TIM3_COMPA_vect){}
// ISR(TIM3_COMPB_vect){}
// ISR(TIM3_COMPC_vect){}
// ISR(TIM3_OVF_vect){}
// ISR(USART1_RXC_vect){}
// ISR(USART1_DRE_vect){}
// ISR(USART1_TXC_vect){}
// ISR(TWI_vect){}
// ISR(SPM_RDY_vect){}

/***EOF***/

