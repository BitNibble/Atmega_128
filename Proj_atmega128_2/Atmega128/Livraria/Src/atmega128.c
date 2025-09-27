/**********************************************************************
	ATMEGA 128
Author:   <sergio.salazar.santos@gmail.com>
License:  GNU General Public License
Hardware: Atmega 128
Update:	  27092025
**********************************************************************/
#include "atmega128.h"
#include <stdarg.h>

/*****************************/
/****       MACROS        ****/
/*****************************/
#define FTDELAY_SIZE 256
static unsigned int ft_Delay_Lock[FTDELAY_SIZE] = {0};
static unsigned int ftCounter[FTDELAY_SIZE] = {0};
	
/*****************************/
/**** MAIN HARDWARE LAYER ****/
/*****************************/
// Indirect Address Register
Atmega128_GPIAR* gpiar_reg(void){return (Atmega128_GPIAR*) 0x001A;}
// Analog Comparator (AC)
Atmega128_AnalogComparator* ac_reg(void){return (Atmega128_AnalogComparator*) 0x0028;}
// Analog to Digital Converter (ADC)
Atmega128_AnalogToDigitalConverter* adc_reg(void){return (Atmega128_AnalogToDigitalConverter*) 0x0024;}
// Boot loader (BOOT_LOAD)
Atmega128_BootLoader* bootload_reg(void){return (Atmega128_BootLoader*) 0x0068;}
// CPU Register (CPU)
Atmega128_CPURegister* cpu_reg(void){return (Atmega128_CPURegister*) 0x0054;}
// EEPROM (EEPROM)
Atmega128_Eeprom* eeprom_reg(void){return (Atmega128_Eeprom*) 0x003C;}
// External Interrupts (EXINT)
Atmega128_ExternalInterrupt* exint_reg(void){return (Atmega128_ExternalInterrupt*) 0x0058;}
// I/O Port (PORTA)
Atmega128_PORTA* gpioa_reg(void){return (Atmega128_PORTA*) 0x0039;}
// I/O Port (PORTB)
Atmega128_PORTB* gpiob_reg(void){return (Atmega128_PORTB*) 0x0036;}
// I/O Port (PORTC)
Atmega128_PORTC* gpioc_reg(void){return (Atmega128_PORTC*) 0x0033;}
// I/O Port (PORTD)
Atmega128_PORTD* gpiod_reg(void){return (Atmega128_PORTD*) 0x0030;}
// I/O Port (PORTE)
Atmega128_PORTE* gpioe_reg(void){return (Atmega128_PORTE*) 0x0021;}
// I/O Port (PORTF)
Atmega128_PORTF* gpiof_reg(void){return (Atmega128_PORTF*) 0x0020;}
// I/O Port (PORTG)
Atmega128_PORTG* gpiog_reg(void){return (Atmega128_PORTG*) 0x0063;}
// JTAG Interface (JTAG)
Atmega128_JtagInterface* jtag_reg(void){return (Atmega128_JtagInterface*) 0x0042;}
// Other Registers (MISC)
Atmega128_OtherRegister* misc_reg(void){return (Atmega128_OtherRegister*) 0x0040;}
// Serial Peripheral Interface (SPI)
Atmega128_SerialPeripheralInterface* spi_reg(void){return (Atmega128_SerialPeripheralInterface*) 0x002D;}
// Timer/Counter, 16-bit (TC1)
Atmega128_TimerCounter1* tc1_reg(void){return (Atmega128_TimerCounter1*) 0x0040;}
// Timer/Counter, 16-bit (TC3)
Atmega128_TimerCounter3* tc3_reg(void){return (Atmega128_TimerCounter3*) 0x0040;}
// Timer/Counter, 8-bit (TC2)
Atmega128_TimerCounter2* tc2_reg(void){return (Atmega128_TimerCounter2*) 0x0043;}
// Timer/Counter, 8-bit A sync (TC0)
Atmega128_TimerCounter0* tc0_reg(void){return (Atmega128_TimerCounter0*) 0x0040;}
// Two Wire Serial Interface (TWI)
Atmega128_TwoWireSerialInterface* twi_reg(void){return (Atmega128_TwoWireSerialInterface*) 0x0070;}
// USART (USART0)
Atmega128_Usart0* usart0_reg(void){return (Atmega128_Usart0*) 0x0029;}
// USART (USART1)
Atmega128_Usart1* usart1_reg(void){return (Atmega128_Usart1*) 0x0098;}
// Watchdog Timer (WDT)
Atmega128_WatchdogTimer* wdt_reg(void){return (Atmega128_WatchdogTimer*) 0x0041;}

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

/*********************************************************************/
/***************** Procedure and Function definition *****************/
/*********************************************************************/
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

