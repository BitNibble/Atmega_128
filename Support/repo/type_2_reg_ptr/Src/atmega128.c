/**********************************************************
	ATMEGA 128
Author:   <sergio.salazar.santos@gmail.com>
License:  GNU General Public License
Hardware: Atmega 128
Update:	  05072025
**********************************************************/
#include "atmega128.h"
#include <stdarg.h>
/***************************/
/***       MACROS        ***/
/***************************/
#define FTDELAY_SIZE 256
unsigned int ft_Delay_Lock[FTDELAY_SIZE] = {0};
unsigned int ftCounter[FTDELAY_SIZE] = {0};
/*****************************/
/**** MAIN HARDWARE LAYER ****/
/*****************************/
// GPWR
Atmega128GPWR_TypeDef* gpwr_reg(void){return (Atmega128GPWR_TypeDef*) 0x0000;}
// Analog Comparator (AC)
static const Atmega128AnalogComparator_TypeDef ac_setup = {
	.acsr = (ACSR_TypeDef*) 0x0028
};
Atmega128AnalogComparator_TypeDef* ac_reg(void){
	return (Atmega128AnalogComparator_TypeDef*) &ac_setup;
}
// Analog to Digital Converter (ADC)
static const Atmega128AnalogToDigitalConverter_TypeDef adc_setup = {
	.adc = (U_word*) 0x0024,
	.adcsra = (ADCSRA_TypeDef*) 0x0026,
	.admux = (ADMUX_TypeDef*) 0x0027
};
Atmega128AnalogToDigitalConverter_TypeDef* adc_reg(void){return (Atmega128AnalogToDigitalConverter_TypeDef*) &adc_setup;}
// Boot loader (BOOT_LOAD)
static const Atmega128BootLoader_TypeDef bootload_setup = {
	.spmcsr = (SPMCSR_TypeDef*) 0x0068
};
Atmega128BootLoader_TypeDef* bootload_reg(void){return (Atmega128BootLoader_TypeDef*) &bootload_setup;}
// CPU Register (CPU)
static const Atmega128CPURegister_TypeDef cpu_setup = {
	.mcucsr = (MCUCSR_TypeDef*) 0x0054,
	.mcucr = (MCUCR_TypeDef*) 0x0055,
	.rampz = (RAMPZ_TypeDef*) 0x005B,
	.xdiv = (XDIV_TypeDef*) 0x005C,
	.sp = (U_word*) 0x005D,
	.sreg = (SREG_TypeDef*) 0x005F,
	.xmcrb = (XMCRB_TypeDef*) 0x006C,
	.xmcra = (XMCRA_TypeDef*) 0x006D,
	.osccal = (U_byte*) 0x006F
};
Atmega128CPURegister_TypeDef* cpu_reg(void){return (Atmega128CPURegister_TypeDef*) &cpu_setup;}
// EEPROM (EEPROM)
static const Atmega128Eeprom_TypeDef eeprom_setup = {
	.eecr = (EECR_TypeDef*) 0x003C,
	.eedr = (U_byte*) 0x003D,
	.eear = (U_word*) 0x003E
};
Atmega128Eeprom_TypeDef* eeprom_reg(void){return (Atmega128Eeprom_TypeDef*) &eeprom_setup;}
// External Interrupts (EXINT)
static const Atmega128ExternalInterrupts_TypeDef exint_setup = {
	.eifr = (EIFR_TypeDef*) 0x0058,
	.eimsk = (EIMSK_TypeDef*) 0x0059,
	.eicrb = (EICRB_TypeDef*) 0x005A,
	.eicra = (EICRA_TypeDef*) 0x006A
};
Atmega128ExternalInterrupts_TypeDef* exint_reg(void){return (Atmega128ExternalInterrupts_TypeDef*) &exint_setup;}
// I/O Port (PORTA)
static const Atmega128PORTA_TypeDef gpioa_setup = {
	.pin = (U_byte*) 0x0039,
	.ddr = (U_byte*) 0x003A,
	.port = (U_byte*) 0x003B
};
Atmega128PORTA_TypeDef* porta_reg(void){return (Atmega128PORTA_TypeDef*) &gpioa_setup;}
// I/O Port (PORTB)
static const Atmega128PORTB_TypeDef gpiob_setup = {
	.pin = (U_byte*) 0x0036,
	.ddr = (U_byte*) 0x0037,
	.port = (U_byte*) 0x0038
};
Atmega128PORTB_TypeDef* portb_reg(void){return (Atmega128PORTB_TypeDef*) &gpiob_setup;}
// I/O Port (PORTC)
static const Atmega128PORTC_TypeDef gpioc_setup = {
	.pin = (U_byte*) 0x0033,
	.ddr = (U_byte*) 0x0034,
	.port = (U_byte*) 0x0035
};
Atmega128PORTC_TypeDef* portc_reg(void){return (Atmega128PORTC_TypeDef*) &gpioc_setup;}
// I/O Port (PORTD)
static const Atmega128PORTD_TypeDef gpiod_setup = {
	.pin = (U_byte*) 0x0030,
	.ddr = (U_byte*) 0x0031,
	.port = (U_byte*) 0x0032
};
Atmega128PORTD_TypeDef* portd_reg(void){return (Atmega128PORTD_TypeDef*) &gpiod_setup;}
// I/O Port (PORTE)
static const Atmega128PORTE_TypeDef gpioe_setup = {
	.pin = (U_byte*) 0x0021,
	.ddr = (U_byte*) 0x0022,
	.port = (U_byte*) 0x0023
};
Atmega128PORTE_TypeDef* porte_reg(void){return (Atmega128PORTE_TypeDef*) &gpioe_setup;}
// I/O Port (PORTF)
static const Atmega128PORTF_TypeDef gpiof_setup = {
	.pin = (U_byte*) 0x0020,
	.ddr = (U_byte*) 0x0061,
	.port = (U_byte*) 0x0062
};
Atmega128PORTF_TypeDef* portf_reg(void){return (Atmega128PORTF_TypeDef*) &gpiof_setup;}
// I/O Port (PORTG)
static const Atmega128PORTG_TypeDef gpiog_setup = {
	.pin = (U_byte*) 0x0063,
	.ddr = (U_byte*) 0x0064,
	.port = (U_byte*) 0x0065
};
Atmega128PORTG_TypeDef* portg_reg(void){return (Atmega128PORTG_TypeDef*) &gpiog_setup;}
// JTAG Interface (JTAG)
static const Atmega128JtagInterface_TypeDef jtag_setup = {
	.ocdr = (U_byte*) 0x0042,
	.mcucsr = (MCUCSR_TypeDef*) 0x0054
};
Atmega128JtagInterface_TypeDef* jtag_reg(void){return (Atmega128JtagInterface_TypeDef*) &jtag_setup;}
// Other Registers (MISC)
static const Atmega128OtherRegisters_TypeDef misc_setup = {
	.sfior = (SFIOR_TypeDef*) 0x0040
};
Atmega128OtherRegisters_TypeDef* misc_reg(void){return (Atmega128OtherRegisters_TypeDef*) &misc_setup;}
// Serial Peripheral Interface (SPI)
static const Atmega128SerialPeripheralInterface_TypeDef spi_setup = {
	.spcr = (SPCR_TypeDef*) 0x002D,
	.spsr = (SPSR_TypeDef*) 0x002E,
	.spdr = (U_byte*) 0x002F
} ;
Atmega128SerialPeripheralInterface_TypeDef* spi_reg(void){return (Atmega128SerialPeripheralInterface_TypeDef*) &spi_setup;}
// Timer/Counter, 16-bit (TC1)
static const Atmega128TimerCounter1_TypeDef tc1_setup = {
	.icr1 = (U_word*) 0x0046,
	.ocr1b = (U_word*) 0x0048,
	.ocr1a = (U_word*) 0x004A,
	.tcnt1 = (U_word*) 0x004C,
	.tccr1b = (TCCR1B_TypeDef*) 0x004E,
	.tccr1a = (TCCR1A_TypeDef*) 0x004F,
	.tifr = (TIFR_Typedef*) 0x0056,
	.timsk = (TIMSK_TypeDef*) 0x0057,
	.ocr1c = (U_word*) 0x0078,
	.tccr1c = (TCCR1C_TypeDef*) 0x007A,
	.etifr = (ETIFR_TypeDef*) 0x007C,
	.etimsk = (ETIMSK_TypeDef*) 0x007D
};
Atmega128TimerCounter1_TypeDef* tc1_reg(void){return (Atmega128TimerCounter1_TypeDef*) &tc1_setup;}
// Timer/Counter, 16-bit (TC3)
Atmega128TimerCounter3_TypeDef tc3_setup = {
	.etifr = (ETIFR_TypeDef*) 0x007C,
	.etimsk = (ETIMSK_TypeDef*) 0x007D,
	.icr3 = (U_word*) 0x0080,
	.ocr3c = (U_word*) 0x0082,
	.ocr3b = (U_word*) 0x0084,
	.ocr3a = (U_word*) 0x0086,
	.tcnt3 = (U_word*) 0x0088,
	.tccr3b = (TCCR3B_TypeDef*) 0x008A,
	.tccr3a = (TCCR3A_TypeDef*) 0x008B,
	.tccr3c = (TCCR3C_TypeDef*) 0x008C
};
Atmega128TimerCounter3_TypeDef* tc3_reg(void){return (Atmega128TimerCounter3_TypeDef*) &tc3_setup;}
// Timer/Counter, 8-bit (TC2)
static const Atmega128TimerCounter2_TypeDef tc2_setup = {
	.ocr2 = (U_byte*) 0x0043,
	.tcnt2 = (U_byte*) 0x0044,
	.tccr2 = (TCCR2_TypeDef*) 0x0045,
	.tifr = (TIFR_Typedef*) 0x0056,
	.timsk = (TIMSK_TypeDef*) 0x0057
};
Atmega128TimerCounter2_TypeDef* tc2_reg(void){return (Atmega128TimerCounter2_TypeDef*) &tc2_setup;}
// Timer/Counter, 8-bit A sync (TC0)
Atmega128TimerCounter0_TypeDef tc0_setup = {
	.assr = (ASSR_TypeDef*) 0x0050,
	.ocr0 = (U_byte*) 0x0051,
	.tcnt0 = (U_byte*) 0x0052,
	.tccr0 = (TCCR0_TypeDef*) 0x0053,
	.tifr = (TIFR_Typedef*) 0x0056,
	.timsk = (TIMSK_TypeDef*) 0x0057
};
Atmega128TimerCounter0_TypeDef* tc0_reg(void){return (Atmega128TimerCounter0_TypeDef*) &tc0_setup;}
// Two Wire Serial Interface (TWI)
static const Atmega128TwoWireSerialInterface_TypeDef twi_setup = {
	.twbr = (U_byte*) 0x0070,
	.twsr = (TWSR_TypeDef*) 0x0071,
	.twar = (TWAR_TypeDef*) 0x0072,
	.twdr = (U_byte*) 0x0073,
	.twcr = (TWCR_TypeDef*) 0x0074
};
Atmega128TwoWireSerialInterface_TypeDef* twi_reg(void){return (Atmega128TwoWireSerialInterface_TypeDef*) &twi_setup;}
// USART (USART0)
static const Atmega128Usart0_TypeDef usart0_setup = {
	.ubrr0l = (U_byte*) 0x0029,
	.ucsr0b = (UCSR0B_TypeDef*) 0x002A,
	.ucsr0a = (UCSR0A_TypeDef*) 0x002B,
	.udr0 = (U_byte*) 0x002C,
	.ubrr0h = (UBRR0H_TypeDef*) 0x0090,
	.ucsr0c = (UCSR0C_TypeDef*) 0x0095
};
Atmega128Usart0_TypeDef* usart0_reg(void){return (Atmega128Usart0_TypeDef*) &usart0_setup;}
// USART (USART1)
static const Atmega128Usart1_TypeDef usart1_setup = {
	.ubrr1h = (UBRR1H_TypeDef*) 0x0098,
	.ubrr1l = (U_byte*) 0x0099,
	.ucsr1b = (UCSR1B_TypeDef*) 0x009A,
	.ucsr1a = (UCSR1A_TypeDef*) 0x009B,
	.udr1 = (U_byte*) 0x009C,
	.ucsr1c = (UCSR1C_TypeDef*) 0x009D
};
Atmega128Usart1_TypeDef* usart1_reg(void){return (Atmega128Usart1_TypeDef*) &usart1_setup;}
// Watchdog Timer (WDT)
static const Atmega128WatchdogTimer_TypeDef wdt_setup = {
	.wdtcr = (WDTCR_TypeDef*) 0x0041
};
Atmega128WatchdogTimer_TypeDef* wdt_reg(void){return (Atmega128WatchdogTimer_TypeDef*) &wdt_setup;}

/*** Atmega 128 Procedure and Function ***/
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
	prescaler &= 0x7F; sreg = cpu_reg()->sreg->var; cpu_reg()->sreg->var &= ~(1 << 7); *clkpr = prescaler;
	*clkpr = (1 << XDIVEN) | prescaler; cpu_reg()->sreg->var = sreg;
}
void MoveInterruptsToBoot(void){volatile uint8_t sreg; sreg = cpu_reg()->sreg->var; cpu_reg()->sreg->var &= ~(1 << 7);
	MCUCR = (1<<IVCE); MCUCR = (1<<IVSEL); cpu_reg()->sreg->var = sreg;
}

/*** Procedure and Function ToolSet ***/
inline uint8_t Msk_Pos(uint8_t Msk){
	uint8_t Pos = 0;
	if( Msk ){
		for( ; !(Msk & 1); Msk >>= 1, Pos++ );
	}
	return Pos;
}
inline void set_reg(volatile uint8_t* reg, uint8_t hbits){
	*reg |= hbits;
}
inline void clear_reg(volatile uint8_t* reg, uint8_t hbits){
	*reg &= ~hbits;
}
inline uint8_t get_reg_Msk(uint8_t reg, uint8_t Msk)
{
	reg = (reg & Msk) >> Msk_Pos(Msk);
	return reg;
}
inline void write_reg_Msk(volatile uint8_t* reg, uint8_t Msk, uint8_t data)
{
	uint8_t value = *reg;
	uint8_t Pos = Msk_Pos(Msk);
	data = (data << Pos) & Msk; value &= ~(Msk); value |= data; *reg = value;
}
inline void set_reg_Msk(volatile uint8_t* reg, uint8_t Msk, uint8_t data)
{
	uint8_t Pos = Msk_Pos(Msk);
	data = (data << Pos) & Msk; *reg &= ~(Msk); *reg |= data;
}
uint8_t get_reg_block(uint8_t reg, uint8_t size_block, uint8_t bit_n)
{
	if(bit_n < BYTE_BITS &&  size_block != 0 && bit_n + size_block <= BYTE_BITS) {
		uint8_t mask = (uint8_t)((1U << size_block) - 1);
		reg = (reg & (mask << bit_n)) >> bit_n;
	}
	return reg;
}
void write_reg_block(volatile uint8_t* reg, uint8_t size_block, uint8_t bit_n, uint8_t data)
{
	uint8_t value = *reg;
	if(bit_n < BYTE_BITS &&  size_block != 0 && bit_n + size_block <= BYTE_BITS) {
		uint8_t mask = (uint8_t)((1U << size_block) - 1);
		data &= mask; value &= ~(mask << bit_n);
		data = (data << bit_n);
		value |= data;
		*reg = value;
	}
}
void set_reg_block(volatile uint8_t* reg, uint8_t size_block, uint8_t bit_n, uint8_t data)
{
	if(bit_n < BYTE_BITS &&  size_block != 0 && bit_n + size_block <= BYTE_BITS) {
		uint8_t mask = (uint8_t)((1U << size_block) - 1);
		data &= mask;
		*reg &= ~(mask << bit_n);
		*reg |= (data << bit_n);
	}
}
uint8_t get_bit_block(volatile uint8_t* reg, uint8_t size_block, uint8_t bit_n)
{
	uint8_t value;
	uint8_t n = bit_n / BYTE_BITS; bit_n = bit_n % BYTE_BITS;
	value = *(reg + n );
	if(size_block != 0 && bit_n + size_block <= BYTE_BITS){
		uint8_t mask = (uint8_t)((1U << size_block) - 1);
		value = (value & (mask << bit_n)) >> bit_n;
	}
	return value;
}
void set_bit_block(volatile uint8_t* reg, uint8_t size_block, uint8_t bit_n, uint8_t data)
{
	uint8_t n = bit_n / BYTE_BITS; bit_n = bit_n % BYTE_BITS;
	if(size_block != 0 && bit_n + size_block <= BYTE_BITS) {
		uint8_t mask = (uint8_t)((1U << size_block) - 1);
		data &= mask;
		*(reg + n ) &= ~(mask << bit_n);
		*(reg + n ) |= (data << bit_n);
	}
}
void STM32446SetRegBits( uint8_t* reg, uint8_t n_bits, ... )
{
	uint8_t i;
	if(n_bits > 0 && n_bits <= BYTE_BITS){ // Filter input
		va_list list;
		va_start(list, n_bits);
		for(i = 0; i < n_bits; i++){
			*reg |= (uint32_t)(1 << va_arg(list, int));
		}
		va_end(list);
	}
}
void STM32446ResetRegBits( uint8_t* reg, uint8_t n_bits, ... )
{
	uint8_t i;
	if(n_bits > 0 && n_bits <= BYTE_BITS){ // Filter input
		va_list list;
		va_start(list, n_bits);
		for(i = 0; i < n_bits; i++){
			*reg &= (uint32_t)~(1 << va_arg(list, int));
		}
		va_end(list);
	}
}
void STM32446VecSetup( volatile uint8_t vec[], unsigned int size_block, unsigned int block_n, unsigned int data )
{
	const unsigned int n_bits = sizeof(data) * BYTE_BITS;
	if(size_block > n_bits){ size_block = n_bits; }
	const unsigned int mask = (uint8_t) ((1 << size_block) - 1);
	unsigned int index = (block_n * size_block) / n_bits;
	data &= mask;
	vec[index] &= ~( mask << ((block_n * size_block) - (index * n_bits)) );
	vec[index] |= ( data << ((block_n * size_block) - (index * n_bits)) );
}
/*** NULL Check ***/
int isPtrNull(void* ptr) {
	return ptr ? 0 : 1; // Returns 1 if NULL, 0 otherwise
}
int isCharPtrFlush(void* ptr) {
	// Cast the void pointer to a char pointer to dereference it
	return *((unsigned char*)ptr) ? 0 : 1; // Returns 1 if '\0', 0 otherwise
}
/*** Fall Threw Delay ***/
int ftdelayCycles( uint8_t lock_ID, unsigned int n_cycle ) {
	int ret = 0;
	if( ft_Delay_Lock[lock_ID] != lock_ID) {
		ft_Delay_Lock[lock_ID] = lock_ID;
		ftCounter[lock_ID] = n_cycle;
		}else{
		if( ftCounter[lock_ID]-- );else{ ft_Delay_Lock[lock_ID] = 0; ret = 1; }
	}
	return ret;
}
void ftdelayReset(uint8_t ID) {
	ft_Delay_Lock[ID] = 0;
	ftCounter[ID] = 0;
}

/******
n = bit_n/32; bit_n = bit_n - (n * 32); -> bit_n = bit_n % 32;
(a+b)/a = a/a + b/a = 1 + b/a
a/b/c/d/e = a/(b*c*d*e)
*******/

/*** EOF ***/

