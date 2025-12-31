/********************************************************************
	ATMEGA 128
Author:   <sergio.salazar.santos@gmail.com>
License:  GNU General Public License
Hardware: Atmega 128
Update:	  27092025
********************************************************************/
#include "atmega128.h"
#include <stdarg.h>

/*****************************/
/****       MACROS        ****/
/*****************************/
#define FTDELAY_SIZE 256
static unsigned int ft_Delay_Lock[FTDELAY_SIZE] = {0};
static unsigned int ftCounter[FTDELAY_SIZE] = {0};
	
/*** Local ***/
uint8_t _get_pos(uint8_t size_block, uint8_t block_n);
uint8_t _var_mask(uint8_t var, uint8_t Msk);
uint8_t _var_imask(uint8_t var, uint8_t Msk);
uint8_t _size_to_block(uint8_t size_block);
uint8_t _block_to_size(uint8_t block);
uint8_t _get_mask(uint8_t size_block, uint8_t Pos);
uint8_t _mask_pos(uint8_t Msk);
uint8_t _mask_data(uint8_t Msk, uint8_t data);

/*******************************************************************/
/********************** MAIN HARDWARE LAYER ************************/
/*******************************************************************/
#if defined(_ATMEGA128_INSTANCE_H_)

static dev_atmega128 atmega128_setup = {
	// Indirect Address Register
	.gpiar = (Atmega128_GPIAR*) 0x001A,
	// Analog Comparator (AC)
	.ac = (Atmega128_AnalogComparator*) 0x0028,
	// Analog to Digital Converter (ADC)
	.adc = (Atmega128_AnalogToDigitalConverter*) 0x0024,
	// Boot loader (BOOT_LOAD)
	.bootload = (Atmega128_BootLoader*) 0x0068,
	// CPU Register (CPU)
	.cpu = (Atmega128_CPURegister*) 0x0054,
	// EEPROM (EEPROM)
	.eeprom = (Atmega128_Eeprom*) 0x003C,
	// External Interrupts (EXINT)
	.exint = (Atmega128_ExternalInterrupt*) 0x0058,
	// I/O Port (PORTA)
	.gpioa = (Atmega128_PORTA*) 0x0039,
	// I/O Port (PORTB)
	.gpiob = (Atmega128_PORTB*) 0x0036,
	// I/O Port (PORTC)
	.gpioc = (Atmega128_PORTC*) 0x0033,
	// I/O Port (PORTD)
	.gpiod = (Atmega128_PORTD*) 0x0030,
	// I/O Port (PORTE)
	.gpioe = (Atmega128_PORTE*) 0x0021,
	// I/O Port (PORTF)
	.gpiof = (Atmega128_PORTF*) 0x0020,
	// I/O Port (PORTG)
	.gpiog = (Atmega128_PORTG*) 0x0063,
	// JTAG Interface (JTAG)
	.jtag = (Atmega128_JtagInterface*) 0x0042,
	// Other Registers (MISC)
	.misc = (Atmega128_OtherRegister*) 0x0040,
	// Serial Peripheral Interface (SPI)
	.spi = (Atmega128_SerialPeripheralInterface*) 0x002D,
	// Timer/Counter, 16-bit (TC1)
	.tc1 = (Atmega128_TimerCounter1*) 0x0040,
	// Timer/Counter, 16-bit (TC3)
	.tc3 = (Atmega128_TimerCounter3*) 0x0040,
	// Timer/Counter, 8-bit (TC2)
	.tc2 = (Atmega128_TimerCounter2*) 0x0043,
	// Timer/Counter, 8-bit A sync (TC0)
	.tc0 = (Atmega128_TimerCounter0*) 0x0040,
	// Two Wire Serial Interface (TWI)
	.twi = (Atmega128_TwoWireSerialInterface*) 0x0070,
	// USART (USART0)
	.usart0 = (Atmega128_Usart0*) 0x0029,
	// USART (USART1)
	.usart1 = (Atmega128_Usart1*) 0x0098,
	// Watchdog Timer (WDT)
	.wdt = (Atmega128_WatchdogTimer*) 0x0041
};

dev_atmega128* dev(void){ return (dev_atmega128*) &atmega128_setup; }

#endif

/*******************************************************************/
/***************** Procedure and Function definition ***************/
/*******************************************************************/
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
uint16_t swapbyte(uint16_t num){uint16_t tp; tp = (num << 8); return (num >> 8) | tp;}


/*******************************************************************/
/***************** Procedure and Function definition ***************/
/*******************************************************************/
uint16_t BAUDRATEnormal(uint32_t BAUD){uint32_t baudrate = F_CPU/16; baudrate /= BAUD; baudrate -= 1; return (uint16_t) baudrate;}
uint16_t BAUDRATEdouble(uint32_t BAUD){uint32_t baudrate = F_CPU/8; baudrate /= BAUD; baudrate -= 1; return (uint16_t) baudrate;}
uint16_t BAUDRATEsynchronous(uint32_t BAUD){uint32_t baudrate = F_CPU/2; baudrate /= BAUD; baudrate -= 1; return (uint16_t) baudrate;}
#if defined(_ATMEGA128_INSTANCE_H_)

void ClockPrescalerSelect(volatile uint8_t prescaler){ volatile uint8_t sreg; prescaler &= 0x7F;
	sreg = atmega128_setup.cpu->sreg.var; atmega128_setup.cpu->sreg.var &= ~(1 << 7); 
	atmega128_setup.cpu->xdiv.var = prescaler; atmega128_setup.cpu->xdiv.par.xdiven = 1; 
	atmega128_setup.cpu->sreg.var = sreg;
}
void MoveInterruptsToBoot(void){volatile uint8_t sreg; sreg = atmega128_setup.cpu->sreg.var; atmega128_setup.cpu->sreg.var &= ~(1 << 7);
	atmega128_setup.cpu->mcucr.par.ivce = 1; atmega128_setup.cpu->mcucr.par.ivsel = 1; atmega128_setup.cpu->sreg.var = sreg;
}

#endif

/*******************************************************************/
/**************** Procedure and Function definition ****************/
/*******************************************************************/
inline uint8_t Msk_Pos(uint8_t Msk){
	uint8_t Pos = 0;
	if( Msk ){
		for( ; !(Msk & 1); Msk >>= 1, Pos++ );
	}
	return Pos;
}
/*** SUB Tools ***/
inline uint8_t _get_pos(uint8_t size_block, uint8_t block_n){
	return size_block * block_n;
}
inline uint8_t _var_mask(uint8_t var, uint8_t Msk){
	return (var & Msk);
}
inline uint8_t _var_imask(uint8_t var, uint8_t Msk){
	return (var & ~Msk);
}
inline uint8_t _size_to_block(uint8_t size_block){
	return (size_block >= BYTE_BITS) ? 0xFFU : ((1U << size_block) - 1);
}
inline uint8_t _block_to_size(uint8_t block) {
	return block ? ((unsigned int)BYTE_BITS - __builtin_clz(block)) : 0U;
}
inline uint8_t _get_mask(uint8_t size_block, uint8_t Pos){
	return _size_to_block(size_block) << Pos;
}
inline uint8_t _mask_pos(uint8_t Msk){
	return Msk ? (unsigned int)__builtin_ctz(Msk) : 0U;
}
inline uint8_t _mask_data(uint8_t Msk, uint8_t data){
	return _var_mask(data << _mask_pos(Msk), Msk);
}
// --- Generic helpers ---
inline uint8_t reg_get(uint8_t reg, uint8_t Msk){
	return _var_mask(reg, Msk) >> _mask_pos(Msk);
}

inline void reg_set(volatile uint8_t *reg, uint8_t Msk, uint8_t data){
	*reg = _var_imask(*reg, Msk) | _mask_data(Msk, data);
}
/*** Tools ***/
void set_reg(volatile uint8_t* reg, uint8_t hbits){
	*reg |= hbits;
}
void clear_reg(volatile uint8_t* reg, uint8_t hbits){
	*reg &= ~hbits;
}
inline uint8_t get_reg_Msk_Pos(uint8_t reg, uint8_t Msk, uint8_t Pos)
{
	return _var_mask(reg, Msk) >> Pos;
}
inline void write_reg_Msk_Pos(volatile uint8_t* reg, uint8_t Msk, uint8_t Pos, uint8_t data)
{
	uint8_t value = _var_imask(*reg, Msk);
	data = _var_mask((data << Pos), Msk); value |= data; *reg = value;
}
inline void set_reg_Msk_Pos(volatile uint8_t* reg, uint8_t Msk, uint8_t Pos, uint8_t data)
{
	data = _var_mask((data << Pos), Msk); clear_reg(reg, Msk); set_reg(reg, data);
}
uint8_t get_reg_Msk(uint8_t reg, uint8_t Msk)
{
	return get_reg_Msk_Pos(reg, Msk, _mask_pos(Msk));
}
void write_reg_Msk(volatile uint8_t* reg, uint8_t Msk, uint8_t data)
{
	write_reg_Msk_Pos(reg, Msk, _mask_pos(Msk), data);
}
void set_reg_Msk(volatile uint8_t* reg, uint8_t Msk, uint8_t data)
{
	set_reg_Msk_Pos(reg, Msk, _mask_pos(Msk), data);
}
uint8_t get_reg_block(uint8_t reg, uint8_t size_block, uint8_t Pos)
{
	return get_reg_Msk_Pos(reg, _get_mask(size_block, Pos), Pos);
}
void write_reg_block(volatile uint8_t* reg, uint8_t size_block, uint8_t Pos, uint8_t data)
{
	write_reg_Msk_Pos(reg, _get_mask(size_block, Pos), Pos, data);
}
void set_reg_block(volatile uint8_t* reg, uint8_t size_block, uint8_t Pos, uint8_t data)
{
	set_reg_Msk_Pos(reg, _get_mask(size_block, Pos), Pos, data);
}
uint8_t get_bit_block(volatile uint8_t* reg, uint8_t size_block, uint8_t Pos)
{
	uint16_t n = Pos / BYTE_BITS; Pos = Pos % BYTE_BITS;
	return get_reg_Msk_Pos((uint8_t)*(reg + n), _get_mask(size_block, Pos), Pos);
}
void set_bit_block(volatile uint8_t* reg, uint8_t size_block, uint8_t Pos, uint8_t data)
{
	uint16_t n = Pos / BYTE_BITS; Pos = Pos % BYTE_BITS;
	set_reg_Msk_Pos((reg + n), _get_mask(size_block, Pos), Pos, data);
}

/****************************************/
// UNUSED
void Atmega128SetRegBits( uint8_t* reg, uint8_t n_bits, ... )
{
	uint8_t i;
	if(n_bits > 0 && n_bits <= BYTE_BITS){ // Filter input
		va_list list;
		va_start(list, n_bits);
		for(i = 0; i < n_bits; i++){
			*reg |= (uint8_t)(1 << va_arg(list, int));
		}
		va_end(list);
	}
}
void Atmega128ResetRegBits( uint8_t* reg, uint8_t n_bits, ... )
{
	uint8_t i;
	if(n_bits > 0 && n_bits <= BYTE_BITS){ // Filter input
		va_list list;
		va_start(list, n_bits);
		for(i = 0; i < n_bits; i++){
			*reg &= (uint8_t)~(1 << va_arg(list, int));
		}
		va_end(list);
	}
}
/*** NULL Check ***/
int isPtrNull(void* ptr) {
	return ptr ? 0 : 1; // Returns 1 if NULL, 0 otherwise
}
int isCharPtrFlush(void* ptr) {
	if (ptr == NULL) return 1;
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

