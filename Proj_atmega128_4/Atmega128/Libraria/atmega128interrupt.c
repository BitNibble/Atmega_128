/*************************************************************************
	ATMEGA128INTERRUPT
Author: Sergio Santos 
	<sergio.salazar.santos@gmail.com>
License: GNU General Public License
Hardware: ATmega128
Date: 24042023
Comment:
   Stable
*************************************************************************/
/*** File Library ***/
#include "atmega128mapping.h"
//#include <stdarg.h>

/*** File Constant & Macro ***/
#if defined(__AVR_ATmega64__) || defined(__AVR_ATmega128__)	
	#define ATMEGA_128_INTERRUPT
#else
 	#error "Not Atmega 128"
#endif

/*** File Variable ***/
ATMEGA128 interruptmega128;

/*** File Header ***/
void INTERRUPT_set(uint8_t channel, uint8_t sense);
void INTERRUPT_off(uint8_t channel);
void INTERRUPT_on(uint8_t channel);
uint8_t INTERRUPT_reset_status(void);

/*** Procedure & Function ***/
INTERRUPT INTERRUPTenable(void)
// Setup blank

{
	INTERRUPT interrupt;
	interruptmega128 = ATMEGA128enable();
	
	interruptmega128.exint.reg->eimsk = 0x00;
	
	interrupt.set = INTERRUPT_set;
	interrupt.off = INTERRUPT_off;
	interrupt.on = INTERRUPT_on;
	interrupt.reset_status = INTERRUPT_reset_status;
	return interrupt;
}
uint8_t INTERRUPT_reset_status(void)
{
	uint8_t reset, ret = 0;
	reset = (interruptmega128.cpu.reg->mcucsr & 0x1F);
	switch(reset){
		case 1: // Power-On Reset Flag
			ret = 0;
			interruptmega128.cpu.reg->mcucsr &= ~(1 << PORF);
		break;
		case 2: // External Reset Flag
			interruptmega128.cpu.reg->mcucsr &= ~(1 << EXTRF);
			ret = 1;
		break;
		case 4: // Brown-out Reset Flag
			interruptmega128.cpu.reg->mcucsr &= ~(1 << BORF);
			ret=2;
		break;
		case 8: // Watchdog Reset Flag
			interruptmega128.cpu.reg->mcucsr &= ~(1 << WDRF);
			ret = 3;
		break;
		case 16: // JTAG Reset Flag
			interruptmega128.cpu.reg->mcucsr &= ~(1 << JTRF);
			ret = 4;
		break;
		default: // clear all status
			interruptmega128.cpu.reg->mcucsr &= ~(0x1F);
		break;
	}
	return ret;
}
void INTERRUPT_set(uint8_t channel, uint8_t sense)
{
	switch( channel ){
		case 0: // PD0
			interruptmega128.exint.reg->eimsk &= ~(1 << INT0);
			interruptmega128.exint.reg->eicra &= ~((1 << ISC01) | (1 << ISC00));
			switch(sense){
				case 0: // The low level of INTn generates an interrupt request.
				case 1: // The low level of INTn generates an interrupt request.
				break;
				case 2: // The falling edge of INTn generates asynchronously an interrupt request.
					interruptmega128.exint.reg->eicra |= (1 << ISC01);
				break;
				case 3: // The rising edge of INTn generates asynchronously an interrupt request.
					interruptmega128.exint.reg->eicra |= ((1 << ISC01) | (1 << ISC00));
				break;
				default: // The low level of INTn generates an interrupt request.
				break;
			}
			interruptmega128.exint.reg->eimsk |= (1 << INT0);
			interruptmega128.cpu.reg->sreg |= (1 << GLOBAL_INTERRUPT_ENABLE);
		break;
		case 1: // PD1
			interruptmega128.exint.reg->eimsk &= ~(1 << INT1);
			interruptmega128.exint.reg->eicra &= ~((1 << ISC11) | (1 << ISC10));
			switch(sense){
				case 0: // The low level of INTn generates an interrupt request.
				case 1: // The low level of INTn generates an interrupt request.
				break;
				case 2: // The falling edge of INTn generates asynchronously an interrupt request.
					interruptmega128.exint.reg->eicra |= (1 << ISC11);
				break;
				case 3: // The rising edge of INTn generates asynchronously an interrupt request.
					interruptmega128.exint.reg->eicra |= ((1 << ISC11) | (1 << ISC10));
				break;
				default: // The low level of INTn generates an interrupt request.
				break;
			}
			interruptmega128.exint.reg->eimsk |= (1 << INT1);
			interruptmega128.cpu.reg->sreg |= (1 << GLOBAL_INTERRUPT_ENABLE);
		break;
		case 2: // PD2
			interruptmega128.exint.reg->eimsk &= ~(1 << INT2);
			interruptmega128.exint.reg->eicra &= ~((1 << ISC21) | (1 << ISC20));
			switch(sense){
				case 0: // The low level of INTn generates an interrupt request.
				case 1: // The low level of INTn generates an interrupt request.
				break;
				case 2: // The falling edge of INTn generates asynchronously an interrupt request.
					interruptmega128.exint.reg->eicra |= (1 << ISC21);
				break;
				case 3: // The rising edge of INTn generates asynchronously an interrupt request.
					interruptmega128.exint.reg->eicra |= ((1 << ISC21) | (1 << ISC20));
				break;
				default: // The low level of INTn generates an interrupt request.
				break;
			}
			interruptmega128.exint.reg->eimsk |= (1 << INT2);
			interruptmega128.cpu.reg->sreg |= (1 << GLOBAL_INTERRUPT_ENABLE);
		break;
		case 3: // PD3
			interruptmega128.exint.reg->eimsk &= ~(1 << INT3);
			interruptmega128.exint.reg->eicra &= ~((1 << ISC31) | (1 << ISC30));
			switch(sense){
				case 0: // The low level of INTn generates an interrupt request.
				case 1: // The low level of INTn generates an interrupt request.
				break;
				case 2: // The falling edge of INTn generates asynchronously an interrupt request.
					interruptmega128.exint.reg->eicra |= (1 << ISC31);
				break;
				case 3: // The rising edge of INTn generates asynchronously an interrupt request.
					interruptmega128.exint.reg->eicra |= ((1 << ISC31) | (1 << ISC30));
				break;
				default: // The low level of INTn generates an interrupt request.
				break;
			}
			interruptmega128.exint.reg->eimsk |= (1 << INT3);
			interruptmega128.cpu.reg->sreg |= (1 << GLOBAL_INTERRUPT_ENABLE);
		break;
		case 4: // PE4
			interruptmega128.exint.reg->eimsk &= ~(1 << INT4);
			interruptmega128.exint.reg->eicrb &= ~((1 << ISC41) | (1 << ISC40));
			switch(sense){
				case 0: // The low level of INTn generates an interrupt request.
				break;
				case 1: // Any logical change on INTn generates an interrupt request
					interruptmega128.exint.reg->eicrb |= (1 << ISC40);
				break;
				case 2: // The falling edge between two samples of INTn generates an interrupt request.
					interruptmega128.exint.reg->eicrb |= (1 << ISC41);
				break;
				case 3: // The rising edge between two samples of INTn generates an interrupt request.
					interruptmega128.exint.reg->eicrb |= ((1 << ISC41) | (1 << ISC40));
				break;
				default: // The low level of INTn generates an interrupt request.
				break;
			}
			interruptmega128.exint.reg->eimsk |= (1 << INT4);
			interruptmega128.cpu.reg->sreg |= (1 << GLOBAL_INTERRUPT_ENABLE);
		break;
		case 5: // PE5
			interruptmega128.exint.reg->eimsk &= ~(1 << INT5);
			interruptmega128.exint.reg->eicrb &= ~((1 << ISC51) | (1 << ISC50));
			switch(sense){
				case 0: // The low level of INTn generates an interrupt request.
				break;
				case 1: // Any logical change on INTn generates an interrupt request
					interruptmega128.exint.reg->eicrb |= (1 << ISC50);
				break;
				case 2: // The falling edge between two samples of INTn generates an interrupt request.
					interruptmega128.exint.reg->eicrb |= (1 << ISC51);
				break;
				case 3: // The rising edge between two samples of INTn generates an interrupt request.
					interruptmega128.exint.reg->eicrb |= ((1 << ISC51) | (1 << ISC50));
				break;
				default: // The low level of INTn generates an interrupt request.
				break;
			}
			interruptmega128.exint.reg->eimsk |= (1 << INT5);
			interruptmega128.cpu.reg->sreg |= (1 << GLOBAL_INTERRUPT_ENABLE);
		break;
		case 6: // PE6
			interruptmega128.exint.reg->eimsk &= ~(1 << INT6);
			interruptmega128.exint.reg->eicrb &= ~((1 << ISC61) | (1 << ISC60));
			switch(sense){
				case 0: // The low level of INTn generates an interrupt request.
				break;
				case 1: // Any logical change on INTn generates an interrupt request
					interruptmega128.exint.reg->eicrb |= (1 << ISC60);
				break;
				case 2: // The falling edge between two samples of INTn generates an interrupt request.
					interruptmega128.exint.reg->eicrb |= (1 << ISC61);
				break;
				case 3: // The rising edge between two samples of INTn generates an interrupt request.
					interruptmega128.exint.reg->eicrb |= ((1 << ISC61) | (1 << ISC60));
				break;
				default: // The low level of INTn generates an interrupt request.
				break;
			}
			interruptmega128.exint.reg->eimsk |= (1 << INT6);
			interruptmega128.cpu.reg->sreg |= (1 << GLOBAL_INTERRUPT_ENABLE);
		break;
		case 7: // PE7
			interruptmega128.exint.reg->eimsk &= ~(1 << INT7);
			interruptmega128.exint.reg->eicrb &= ~((1 << ISC71) | (1 << ISC70));
			switch(sense){
				case 0: // The low level of INTn generates an interrupt request.
				break;
				case 1: // Any logical change on INTn generates an interrupt request
					interruptmega128.exint.reg->eicrb |= (1 << ISC70);
				break;
				case 2: // The falling edge between two samples of INTn generates an interrupt request.
					interruptmega128.exint.reg->eicrb |= (1 << ISC71);
				break;
				case 3: // The rising edge between two samples of INTn generates an interrupt request.
					interruptmega128.exint.reg->eicrb |= ((1 << ISC71) | (1 << ISC70));
				break;
				default: // The low level of INTn generates an interrupt request.
				break;
			}
			interruptmega128.exint.reg->eimsk |= (1 << INT7);
			interruptmega128.cpu.reg->sreg |= (1 << GLOBAL_INTERRUPT_ENABLE);
		break;
		default:
			interruptmega128.exint.reg->eimsk = 0X00;
		break;
	}
}
void INTERRUPT_off(uint8_t channel)
{
	switch( channel ){
		case 0: // disable
			interruptmega128.exint.reg->eimsk &= ~(1 << INT0);
		break;
		case 1: // disable
			interruptmega128.exint.reg->eimsk &= ~(1 << INT1);
		break;
		case 2: // disable
			interruptmega128.exint.reg->eimsk &= ~(1 << INT2);
		break;
		case 3: // disable
			interruptmega128.exint.reg->eimsk &= ~(1 << INT3);
		break;
		case 4: // disable
			interruptmega128.exint.reg->eimsk &= ~(1 <<INT4);
		break;
		case 5: // disable
			interruptmega128.exint.reg->eimsk &= ~(1 << INT5);
		break;
		case 6: // disable
			interruptmega128.exint.reg->eimsk &= ~(1 << INT6);
		break;
		case 7: // disable
			interruptmega128.exint.reg->eimsk &= ~(1 << INT7);
		break;
		default: // all disable
			interruptmega128.exint.reg->eimsk = 0X00;
		break;
	}
}
void INTERRUPT_on(uint8_t channel)
{
	switch( channel ){
		case 0:
			interruptmega128.exint.reg->eimsk |= (1<<INT0);
			interruptmega128.cpu.reg->sreg |= (1 << GLOBAL_INTERRUPT_ENABLE);
		break;
		case 1:
			interruptmega128.exint.reg->eimsk |= (1 << INT1);
			interruptmega128.cpu.reg->sreg |= (1 << GLOBAL_INTERRUPT_ENABLE);
		break;
		case 2:
			interruptmega128.exint.reg->eimsk |= (1 << INT2);
			interruptmega128.cpu.reg->sreg |= (1 << GLOBAL_INTERRUPT_ENABLE);
		break;
		case 3:
			interruptmega128.exint.reg->eimsk |= (1 << INT3);
			interruptmega128.cpu.reg->sreg |= (1 << GLOBAL_INTERRUPT_ENABLE);
		break;
		case 4:
			interruptmega128.exint.reg->eimsk |= (1 << INT4);
			interruptmega128.cpu.reg->sreg |= (1 << GLOBAL_INTERRUPT_ENABLE);
		break;
		case 5:
			interruptmega128.exint.reg->eimsk |= (1 << INT5);
			interruptmega128.cpu.reg->sreg |= (1 << GLOBAL_INTERRUPT_ENABLE);
		break;
		case 6:
			interruptmega128.exint.reg->eimsk |= (1 << INT6);
			interruptmega128.cpu.reg->sreg |= (1 << GLOBAL_INTERRUPT_ENABLE);
		break;
		case 7:
			interruptmega128.exint.reg->eimsk |= (1 << INT7);
			interruptmega128.cpu.reg->sreg |= (1 << GLOBAL_INTERRUPT_ENABLE);
		break;
		default:
		break;
	}
}

/*** File Interrupt ***/
// ISR(INT0_vect){ }
// ISR(INT1_vect){ }
// ISR(INT2_vect){ }
// ISR(INT3_vect){ }
// ISR(INT4_vect){ }
// ISR(INT5_vect){ }
// ISR(INT6_vect){ }
// ISR(INT7_vect){ }

/***EOF***/

