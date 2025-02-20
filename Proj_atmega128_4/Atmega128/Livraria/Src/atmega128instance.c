/**********************************************************
	ATMEGA 128 Instance
Author: Sergio Manuel Santos
	<sergio.salazar.santos@gmail.com>
License: GNU General Public License
Hardware: Atmega 128
Update:	07/04/2024
Comment: 

**********************************************************/
#include "atmega128instance.h"
#include <stdarg.h>
/***************************/
/***       MACROS        ***/
/***************************/
#define FTDELAY_SIZE 256
unsigned int ft_Delay_Lock[FTDELAY_SIZE] = {0};
unsigned int ftCounter[FTDELAY_SIZE] = {0};
/***************************/
/***  HARDWARE HANDLER   ***/
/***************************/
// GPWR
Atmega128GPWR_TypeDef* gpwr_instance(void){return (Atmega128GPWR_TypeDef*) Atmega128GPWR_Address;}
// Analog Comparator (AC)
Atmega128AnalogComparator_TypeDef* ac_instance(void){return (Atmega128AnalogComparator_TypeDef*) Atmega128AnalogComparator_Address;}
// Analog to Digital Converter (ADC)
Atmega128AnalogToDigitalConverter_TypeDef* adc_instance(void){return (Atmega128AnalogToDigitalConverter_TypeDef*) Atmega128AnalogToDigitalConverter_Address;}
// Boot loader (BOOT_LOAD)
Atmega128BootLoader_TypeDef* bootload_instance(void){return (Atmega128BootLoader_TypeDef*) Atmega128BootLoader_Address;}
// CPU Register (CPU)
Atmega128CPURegister_TypeDef* cpu_instance(void){return (Atmega128CPURegister_TypeDef*) Atmega128CPURegister_Address;}
// EEPROM (EEPROM)
Atmega128Eeprom_TypeDef* eeprom_instance(void){return (Atmega128Eeprom_TypeDef*) Atmega128Eeprom_Address;}
// External Interrupts (EXINT)
Atmega128ExternalInterrupts_TypeDef* exint_instance(void){return (Atmega128ExternalInterrupts_TypeDef*) Atmega128ExternalInterrupts_Address;}
// I/O Port (PORTA)
Atmega128PORTA_TypeDef* porta_instance(void){return (Atmega128PORTA_TypeDef*) Atmega128PORTA_Address;}
// I/O Port (PORTB)
Atmega128PORTB_TypeDef* portb_instance(void){return (Atmega128PORTB_TypeDef*) Atmega128PORTB_Address;}
// I/O Port (PORTC)
Atmega128PORTC_TypeDef* portc_instance(void){return (Atmega128PORTC_TypeDef*) Atmega128PORTC_Address;}
// I/O Port (PORTD)
Atmega128PORTD_TypeDef* portd_instance(void){return (Atmega128PORTD_TypeDef*) Atmega128PORTD_Address;}
// I/O Port (PORTE)
Atmega128PORTE_TypeDef* porte_instance(void){return (Atmega128PORTE_TypeDef*) Atmega128PORTE_Address;}
// I/O Port (PORTF)
Atmega128PORTF_TypeDef* portf_instance(void){return (Atmega128PORTF_TypeDef*) Atmega128PORTF_Address;}
// I/O Port (PORTG)
Atmega128PORTG_TypeDef* portg_instance(void){return (Atmega128PORTG_TypeDef*) Atmega128PORTG_Address;}
// JTAG Interface (JTAG)
Atmega128JtagInterface_TypeDef* jtag_instance(void){return (Atmega128JtagInterface_TypeDef*) Atmega128JtagInterface_Address;}
// JTAG Interface (JTAG)
Atmega128JtagInterfaceControlStatus_TypeDef* jtag_cs_instance(void){return (Atmega128JtagInterfaceControlStatus_TypeDef*) Atmega128JtagInterfaceControlStatus_Address;}
// Other Registers (MISC)
Atmega128OtherRegisters_TypeDef* misc_instance(void){return (Atmega128OtherRegisters_TypeDef*) Atmega128OtherRegisters_Address;}
// Serial Peripheral Interface (SPI)
Atmega128SerialPeripheralInterface_TypeDef* spi_instance(void){return (Atmega128SerialPeripheralInterface_TypeDef*) Atmega128SerialPeripheralInterface_Address;}
// Timer/Counter, 16-bit (TC1)
Atmega128TimerCounter1_TypeDef* tc1_instance(void){return (Atmega128TimerCounter1_TypeDef*) Atmega128TimerCounter1_Address;}
// Timer/Counter, 16-bit (TC3)
Atmega128TimerCounter3_TypeDef* tc3_instance(void){return (Atmega128TimerCounter3_TypeDef*) Atmega128TimerCounter3_Address;}
// Timer/Counter 1 and 3
Atmega128TimerExternalInterruptFlag_TypeDef* tc_exif_instance(void){return (Atmega128TimerExternalInterruptFlag_TypeDef*) Atmega128TimerExternalInterruptFlag_Address;}
// Timer/Counter 1 and 3
Atmega128TimerExternalInterruptMask_TypeDef* tc_exim_instance(void){return (Atmega128TimerExternalInterruptMask_TypeDef*) Atmega128TimerExternalInterruptMask_Address;}
// Timer/Counter, 8-bit (TC2)
Atmega128TimerCounter2_TypeDef* tc2_instance(void){return (Atmega128TimerCounter2_TypeDef*) Atmega128TimerCounter2_Address;}
// Timer/Counter, 8-bit A sync (TC0)
Atmega128TimerCounter0_TypeDef* tc0_instance(void){return (Atmega128TimerCounter0_TypeDef*) Atmega128TimerCounter0_Address;}
// Timer/Counter 0, 1 and 2
Atmega128TimerInterruptFlag_TypeDef* tc_if_instance(void){return (Atmega128TimerInterruptFlag_TypeDef*) Atmega128TimerInterruptFlag_Address;}
// Timer/Counter 0, 1 and 2
Atmega128TimerInterruptMask_TypeDef* tc_im_instance(void){return (Atmega128TimerInterruptMask_TypeDef*) Atmega128TimerInterruptMask_Address;}
// Two Wire Serial Interface (TWI)
Atmega128TwoWireSerialInterface_TypeDef* twi_instance(void){return (Atmega128TwoWireSerialInterface_TypeDef*) Atmega128TwoWireSerialInterface_Address;}
// USART (USART0)
Atmega128Usart0_TypeDef* usart0_instance(void){return (Atmega128Usart0_TypeDef*) Atmega128Usart0_Address;}
// USART (USART1)
Atmega128Usart1_TypeDef* usart1_instance(void){return (Atmega128Usart1_TypeDef*) Atmega128Usart1_Address;}
// Watchdog Timer (WDT)
Atmega128WatchdogTimer_TypeDef* wdt_instance(void){return (Atmega128WatchdogTimer_TypeDef*) Atmega128WatchdogTimer_Address;}

/*** Atmega 128 Procedure and Function ***/
uint16_t readhlbyte(HighLowByte reg){return (reg.par.H << 8) | reg.par.L;}
uint16_t readlhbyte(HighLowByte reg){return (reg.par.L << 8) | reg.par.H;}
HighLowByte writehlbyte(uint16_t val){HighLowByte reg; reg.par.H = (val >> 8); reg.par.L = val; return reg;}
HighLowByte writelhbyte(uint16_t val){HighLowByte reg; reg.par.L = (val >> 8); reg.par.H = val; return reg;}
uint16_t SwapByte(uint16_t num){uint16_t tp; tp = (num << 8); return (num >> 8) | tp;}
uint16_t BAUDRATEnormal(uint32_t BAUD){uint32_t baudrate = F_CPU/16; baudrate /= BAUD; baudrate -= 1; return (uint16_t) baudrate;}
uint16_t BAUDRATEdouble(uint32_t BAUD){uint32_t baudrate = F_CPU/8; baudrate /= BAUD; baudrate -= 1; return (uint16_t) baudrate;}
uint16_t BAUDRATEsynchronous(uint32_t BAUD){uint32_t baudrate = F_CPU/2; baudrate /= BAUD; baudrate -= 1; return (uint16_t) baudrate;}
void ClockPrescalerSelect(volatile uint8_t prescaler){ volatile uint8_t sreg; volatile uint8_t* clkpr = &XDIV; 
	prescaler &= 0x7F; sreg = cpu_instance()->sreg.reg; cpu_instance()->sreg.reg &= ~(1 << 7); *clkpr = prescaler;
	*clkpr = (1 << XDIVEN) | prescaler; cpu_instance()->sreg.reg = sreg;
}
void MoveInterruptsToBoot(void){volatile uint8_t sreg; sreg = cpu_instance()->sreg.reg; cpu_instance()->sreg.reg &= ~(1 << 7);
	MCUCR = (1<<IVCE); MCUCR = (1<<IVSEL); cpu_instance()->sreg.reg = sreg;
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

