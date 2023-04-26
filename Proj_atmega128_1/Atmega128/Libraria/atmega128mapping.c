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
#define GPWRaddr 0x0000
#define AnalogComparator 0x0028
#define AnalogToDigitalConverter 0x0024
#define BootLoader 0x0068;
#define CPURegister 0x0054
#define Eeprom 0x003C
#define ExternalInterrupts 0x0058
#define PORTAaddr 0x39
#define PORTBaddr 0x36
#define PORTCaddr 0x33
#define PORTDaddr 0x30
#define PORTEaddr 0x21
#define PORTFaddr 0x20
#define PORTGaddr 0x63
#define JtagInterface 0x0042
#define OtherRegisters 0x0040
#define SerialPeripherialInterface 0x002D
#define TimerCounter1 0x0040
#define TimerCounter3 0x0040
#define TimerCounter2 0x0043
#define TimerCounter0 0X0040
#define TwoWireSerialInterface 0x0070
#define Usart0 0x0029
#define Usart1 0x0098
#define WatchdogTimer 0x0041
// FLASH
#define InterruptVectors 0x0000 // ISVEL = 0

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
	ret.gpwr.reg = (Atmega128GPWR_TypeDef*) GPWRaddr;
	ret.ac.reg = (Atmega128AnalogComparator_TypeDef*) AnalogComparator;
	// ADC
	ret.adc.reg = (Atmega128AnalogToDigitalConverter_TypeDef*) AnalogToDigitalConverter;
	#if defined(_ATMEGA128ANALOG_H_)
		ret.adc.enable = ANALOGenable;
	#endif
	ret.boot_load.reg = (Atmega128BootLoader_TypeDef*) BootLoader;
	ret.cpu.reg = (Atmega128CPURegister_TypeDef*) CPURegister;
	// EEPROM
	ret.eeprom.reg = (Atmega128Eeprom_TypeDef*) Eeprom;
	#if defined(_EEPROM_H_)
		ret.eeprom.enable = EEPROMenable;
	#endif
	// EXINT
	ret.exint.reg = (Atmega128ExternalInterrupts_TypeDef*) ExternalInterrupts;
	#if defined(_ATMEGA128INTERRUPT_H_)
		ret.exint.enable = INTERRUPTenable;
	#endif
	ret.porta.reg = (Atmega128PORTA_TypeDef*) PORTAaddr;
	ret.portb.reg = (Atmega128PORTB_TypeDef*) PORTBaddr;
	ret.portc.reg = (Atmega128PORTC_TypeDef*) PORTCaddr;
	ret.portd.reg = (Atmega128PORTD_TypeDef*) PORTDaddr;
	ret.porte.reg = (Atmega128PORTE_TypeDef*) PORTEaddr;
	ret.portf.reg = (Atmega128PORTF_TypeDef*) PORTFaddr;
	ret.portg.reg = (Atmega128PORTG_TypeDef*) PORTGaddr;
	ret.jtag.reg = (Atmega128JtagInterface_TypeDef*) JtagInterface;
	ret.misc.reg = (Atmega128OtherRegisters_TypeDef*) OtherRegisters;
	// SPI
	ret.spi.reg = (Atmega128SerialPeripherialInterface_TypeDef*) SerialPeripherialInterface;
	#if defined(_ATMEGA128SPI_H_)
		ret.spi.enable = SPIenable;
	#endif
	// TC1
	ret.tc1.reg = (Atmega128TimerCounter1_TypeDef*) TimerCounter1;
	#if defined(_ATMEGA128TIMER_H_)
		ret.tc1.enable = TIMER_COUNTER1enable;
	#endif
	// TC3
	ret.tc3.reg = (Atmega128TimerCounter3_TypeDef*) TimerCounter3;
	#if defined(_ATMEGA128TIMER_H_)
		ret.tc3.enable = TIMER_COUNTER3enable;
	#endif
	// TC2
	ret.tc2.reg = (Atmega128TimerCounter2_TypeDef*) TimerCounter2;
	#if defined(_ATMEGA128TIMER_H_)
		ret.tc2.enable = TIMER_COUNTER2enable;
	#endif
	// TC0
	ret.tc0.reg = (Atmega128TimerCounter0_TypeDef*) TimerCounter0;
	#if defined(_ATMEGA128TIMER_H_)
		ret.tc0.enable = TIMER_COUNTER0enable;
	#endif
	// TWI
	ret.twi.reg = (Atmega128TwoWireSerialInterface_TypeDef*) TwoWireSerialInterface;
	#if defined(_ATMEGA128TWI_H_)
		ret.twi.enable = TWIenable;
	#endif
	// USART0
	ret.usart0.reg = (Atmega128Usart0_TypeDef*) Usart0;
	#if defined(_ATMEGA128UART_H_)
		ret.usart0.enable = UART0enable;
	#endif
	// USART1
	ret.usart1.reg = (Atmega128Usart1_TypeDef*) Usart1;
	#if defined(_ATMEGA128UART_H_)
		ret.usart1.enable = UART1enable;
	#endif
	ret.wdt.reg = (Atmega128WatchdogTimer_TypeDef*) WatchdogTimer;
	// FLASH
	// ret.isr.reg = (Atmega128InterruptVectors_TypeDef*) InterruptVectors;
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

