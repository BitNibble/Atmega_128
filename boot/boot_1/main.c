#ifndef __AVR_ATmega128__
	#define __AVR_ATmega128__
#endif

#define F_CPU 16000000UL

#include "atmega128.h"

//__attribute__((naked, section(".text")))
int main(void)
{
	DDRC |= ((1 << PC0) | (1 << PC7));

	if (pgm_read_word(0x0000) == 0xFFFF) {
		for (uint8_t i = 0; i < 6; i++)
		{
			PORTC &= ~(1 << PC0);
			_delay_ms(100);
			PORTC |= 1 << PC0;
			_delay_ms(100);
		}
		}else{
		for (uint8_t i = 0; i < 6; i++)
		{
			PORTC &= ~(1 << PC7);
			_delay_ms(500);
			PORTC |= (1 << PC7);
			_delay_ms(500);
		}
	}
	
	boot_rww_enable();
	
	asm("jmp 0");
}

