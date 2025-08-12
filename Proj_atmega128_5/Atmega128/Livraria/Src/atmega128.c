/**********************************************************
	ATMEGA 128
Author:   <sergio.salazar.santos@gmail.com>
License:  GNU General Public License
Hardware: Atmega 128
Update:	  12082025
**********************************************************/
#include "atmega128.h"

/*****************************/
/**** MAIN HARDWARE LAYER ****/
/*****************************/
// GPWR
//Atmega128GPWR_TypeDef* gpwr_reg(void){return (Atmega128GPWR_TypeDef*) 0x0000;}
// Analog Comparator (AC)
Atmega128AnalogComparator_TypeDef* ac_reg(void){return (Atmega128AnalogComparator_TypeDef*) 0x0028;}
// Analog to Digital Converter (ADC)
Atmega128AnalogToDigitalConverter_TypeDef* adc_reg(void){return (Atmega128AnalogToDigitalConverter_TypeDef*) 0x0024;}
// Boot loader (BOOT_LOAD)
Atmega128BootLoader_TypeDef* bootload_reg(void){return (Atmega128BootLoader_TypeDef*) 0x0068;}
// CPU Register (CPU)
Atmega128CPURegister_TypeDef* cpu_reg(void){return (Atmega128CPURegister_TypeDef*) 0x0054;}
// EEPROM (EEPROM)
Atmega128Eeprom_TypeDef* eeprom_reg(void){return (Atmega128Eeprom_TypeDef*) 0x003C;}
// External Interrupts (EXINT)
Atmega128ExternalInterrupts_TypeDef* exint_reg(void){return (Atmega128ExternalInterrupts_TypeDef*) 0x0058;}
// I/O Port (PORTA)
Atmega128PORTA_TypeDef* gpioa_reg(void){return (Atmega128PORTA_TypeDef*) 0x0039;}
// I/O Port (PORTB)
Atmega128PORTB_TypeDef* gpiob_reg(void){return (Atmega128PORTB_TypeDef*) 0x0036;}
// I/O Port (PORTC)
Atmega128PORTC_TypeDef* gpioc_reg(void){return (Atmega128PORTC_TypeDef*) 0x0033;}
// I/O Port (PORTD)
Atmega128PORTD_TypeDef* gpiod_reg(void){return (Atmega128PORTD_TypeDef*) 0x0030;}
// I/O Port (PORTE)
Atmega128PORTE_TypeDef* gpioe_reg(void){return (Atmega128PORTE_TypeDef*) 0x0021;}
// I/O Port (PORTF)
Atmega128PORTF_TypeDef* gpiof_reg(void){return (Atmega128PORTF_TypeDef*) 0x0020;}
// I/O Port (PORTG)
Atmega128PORTG_TypeDef* gpiog_reg(void){return (Atmega128PORTG_TypeDef*) 0x0063;}
// JTAG Interface (JTAG)
Atmega128JtagInterface_TypeDef* jtag_reg(void){return (Atmega128JtagInterface_TypeDef*) 0x0042;}
// JTAG Interface (JTAG)
Atmega128JtagInterfaceControlStatus_TypeDef* jtag_cs_reg(void){return (Atmega128JtagInterfaceControlStatus_TypeDef*) 0x0054;}
// Other Registers (MISC)
Atmega128OtherRegisters_TypeDef* misc_reg(void){return (Atmega128OtherRegisters_TypeDef*) 0x0040;}
// Serial Peripheral Interface (SPI)
Atmega128SerialPeripheralInterface_TypeDef* spi_reg(void){return (Atmega128SerialPeripheralInterface_TypeDef*) 0x002D;}
// Timer/Counter, 16-bit (TC1)
Atmega128TimerCounter1_TypeDef* tc1_reg(void){return (Atmega128TimerCounter1_TypeDef*) 0x0040;}
// Timer/Counter, 16-bit (TC3)
Atmega128TimerCounter3_TypeDef* tc3_reg(void){return (Atmega128TimerCounter3_TypeDef*) 0x0040;}
// Timer/Counter 1 and 3
Atmega128TimerExternalInterruptFlag_TypeDef* tc_exif_reg(void){return (Atmega128TimerExternalInterruptFlag_TypeDef*) 0x007C;}
// Timer/Counter 1 and 3
Atmega128TimerExternalInterruptMask_TypeDef* tc_exim_reg(void){return (Atmega128TimerExternalInterruptMask_TypeDef*) 0x007D;}
// Timer/Counter, 8-bit (TC2)
Atmega128TimerCounter2_TypeDef* tc2_reg(void){return (Atmega128TimerCounter2_TypeDef*) 0x0043;}
// Timer/Counter, 8-bit A sync (TC0)
Atmega128TimerCounter0_TypeDef* tc0_reg(void){return (Atmega128TimerCounter0_TypeDef*) 0x0040;}
// Timer/Counter 0, 1 and 2
Atmega128TimerInterruptFlag_TypeDef* tc_if_reg(void){return (Atmega128TimerInterruptFlag_TypeDef*) 0x0056;}
// Timer/Counter 0, 1 and 2
Atmega128TimerInterruptMask_TypeDef* tc_im_reg(void){return (Atmega128TimerInterruptMask_TypeDef*) 0x0057;}
// Two Wire Serial Interface (TWI)
Atmega128TwoWireSerialInterface_TypeDef* twi_reg(void){return (Atmega128TwoWireSerialInterface_TypeDef*) 0x0070;}
// USART (USART0)
Atmega128Usart0_TypeDef* usart0_reg(void){return (Atmega128Usart0_TypeDef*) 0x0029;}
// USART (USART1)
Atmega128Usart1_TypeDef* usart1_reg(void){return (Atmega128Usart1_TypeDef*) 0x0098;}
// Watchdog Timer (WDT)
Atmega128WatchdogTimer_TypeDef* wdt_reg(void){return (Atmega128WatchdogTimer_TypeDef*) 0x0041;}

/*********************************************************************/
/***************** Procedure and Function definition *****************/
/*********************************************************************/
uint16_t readHLbyte(U_word reg){
	return (reg.par.h.var << 8) | reg.par.l.var;
}
uint16_t readLHbyte(U_word reg){
	return (reg.par.l.var << 8) | reg.par.h.var;
}
U_word writeHLbyte(uint16_t val){
	U_word reg; reg.par.h.var = (val >> 8) & 0xFF; reg.par.l.var = val & 0xFF; return reg;
}
U_word writeLHbyte(uint16_t val){
	U_word reg; reg.par.l.var = (val >> 8) & 0xFF; reg.par.h.var = val & 0xFF; return reg;
}
uint16_t SwapByte(uint16_t num){uint16_t tp; tp = (num << 8); return (num >> 8) | tp;}
uint16_t BAUDRATEnormal(uint32_t BAUD){uint32_t baudrate = F_CPU/16; baudrate /= BAUD; baudrate -= 1; return (uint16_t) baudrate;}
uint16_t BAUDRATEdouble(uint32_t BAUD){uint32_t baudrate = F_CPU/8; baudrate /= BAUD; baudrate -= 1; return (uint16_t) baudrate;}
uint16_t BAUDRATEsynchronous(uint32_t BAUD){uint32_t baudrate = F_CPU/2; baudrate /= BAUD; baudrate -= 1; return (uint16_t) baudrate;}
void ClockPrescalerSelect(volatile uint8_t prescaler){ volatile uint8_t sreg; volatile uint8_t* clkpr = &XDIV; 
	prescaler &= 0x7F; sreg = cpu_reg()->sreg.var; cpu_reg()->sreg.var &= ~(1 << 7); *clkpr = prescaler;
	*clkpr = (1 << XDIVEN) | prescaler; cpu_reg()->sreg.var = sreg;
}
void MoveInterruptsToBoot(void){volatile uint8_t sreg; sreg = cpu_reg()->sreg.var; cpu_reg()->sreg.var &= ~(1 << 7);
	MCUCR = (1<<IVCE); MCUCR = (1<<IVSEL); cpu_reg()->sreg.var = sreg;
}

/*** EOF ***/

