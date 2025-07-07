/*************************************************************************
	ATMEGA128INTERRUPT
Author:   <sergio.salazar.santos@gmail.com>
License:  GNU General Public License
Hardware: ATmega128
Update:   07/01/2024
Comment:
   Stable
*************************************************************************/
/*** File Library ***/
#include "atmega128interrupt.h"

/*** File Variable ***/
static EXINT0 atmega128_exint;

/*** File Header ***/
void INTERRUPT_set(uint8_t channel, uint8_t sense);
void INTERRUPT_off(uint8_t channel);
void INTERRUPT_on(uint8_t channel);
uint8_t INTERRUPT_reset_status(void);

/*** Procedure & Function ***/
EXINT0 exint_enable(void)
{
	// ATMEGA128enable();
	
	exint_reg()->eimsk.var = 0x00;
	atmega128_exint.set = INTERRUPT_set;
	atmega128_exint.off = INTERRUPT_off;
	atmega128_exint.on = INTERRUPT_on;
	atmega128_exint.reset_status = INTERRUPT_reset_status;
	return atmega128_exint;
}

EXINT0* exint(void){ return &atmega128_exint; }

uint8_t INTERRUPT_reset_status(void)
{
	uint8_t reset, ret = 0;
	reset = (cpu_reg()->mcucsr.var & 0x1F);
	switch(reset){
		case 1: // Power-On Reset Flag
			ret = 0;
			cpu_reg()->mcucsr.var &= ~(1 << PORF);
		break;
		case 2: // External Reset Flag
			cpu_reg()->mcucsr.var &= ~(1 << EXTRF);
			ret = 1;
		break;
		case 4: // Brown-out Reset Flag
			cpu_reg()->mcucsr.var &= ~(1 << BORF);
			ret=2;
		break;
		case 8: // Watchdog Reset Flag
			cpu_reg()->mcucsr.var &= ~(1 << WDRF);
			ret = 3;
		break;
		case 16: // JTAG Reset Flag
			cpu_reg()->mcucsr.var &= ~(1 << JTRF);
			ret = 4;
		break;
		default: // clear all status
			cpu_reg()->mcucsr.var &= ~(0x1F);
		break;
	}
	return ret;
}
void INTERRUPT_set(uint8_t channel, uint8_t sense)
{
	switch( channel ){
		case 0: // PD0
			exint_reg()->eimsk.var &= ~(1 << INT0);
			exint_reg()->eicra.var &= ~((1 << ISC01) | (1 << ISC00));
			switch(sense){
				case 0: // The low level of INTn generates an interrupt request.
				case 1: // The low level of INTn generates an interrupt request.
				break;
				case 2: // The falling edge of INTn generates asynchronously an interrupt request.
					exint_reg()->eicra.var |= (1 << ISC01);
				break;
				case 3: // The rising edge of INTn generates asynchronously an interrupt request.
					exint_reg()->eicra.var |= ((1 << ISC01) | (1 << ISC00));
				break;
				default: // The low level of INTn generates an interrupt request.
				break;
			}
			exint_reg()->eimsk.var |= (1 << INT0);
			cpu_reg()->sreg.var |= (1 << GLOBAL_INTERRUPT_ENABLE);
		break;
		case 1: // PD1
			exint_reg()->eimsk.var &= ~(1 << INT1);
			exint_reg()->eicra.var &= ~((1 << ISC11) | (1 << ISC10));
			switch(sense){
				case 0: // The low level of INTn generates an interrupt request.
				case 1: // The low level of INTn generates an interrupt request.
				break;
				case 2: // The falling edge of INTn generates asynchronously an interrupt request.
					exint_reg()->eicra.var |= (1 << ISC11);
				break;
				case 3: // The rising edge of INTn generates asynchronously an interrupt request.
					exint_reg()->eicra.var |= ((1 << ISC11) | (1 << ISC10));
				break;
				default: // The low level of INTn generates an interrupt request.
				break;
			}
			exint_reg()->eimsk.var |= (1 << INT1);
			cpu_reg()->sreg.var |= (1 << GLOBAL_INTERRUPT_ENABLE);
		break;
		case 2: // PD2
			exint_reg()->eimsk.var &= ~(1 << INT2);
			exint_reg()->eicra.var &= ~((1 << ISC21) | (1 << ISC20));
			switch(sense){
				case 0: // The low level of INTn generates an interrupt request.
				case 1: // The low level of INTn generates an interrupt request.
				break;
				case 2: // The falling edge of INTn generates asynchronously an interrupt request.
					exint_reg()->eicra.var |= (1 << ISC21);
				break;
				case 3: // The rising edge of INTn generates asynchronously an interrupt request.
					exint_reg()->eicra.var |= ((1 << ISC21) | (1 << ISC20));
				break;
				default: // The low level of INTn generates an interrupt request.
				break;
			}
			exint_reg()->eimsk.var |= (1 << INT2);
			cpu_reg()->sreg.var |= (1 << GLOBAL_INTERRUPT_ENABLE);
		break;
		case 3: // PD3
			exint_reg()->eimsk.var &= ~(1 << INT3);
			exint_reg()->eicra.var &= ~((1 << ISC31) | (1 << ISC30));
			switch(sense){
				case 0: // The low level of INTn generates an interrupt request.
				case 1: // The low level of INTn generates an interrupt request.
				break;
				case 2: // The falling edge of INTn generates asynchronously an interrupt request.
					exint_reg()->eicra.var |= (1 << ISC31);
				break;
				case 3: // The rising edge of INTn generates asynchronously an interrupt request.
					exint_reg()->eicra.var |= ((1 << ISC31) | (1 << ISC30));
				break;
				default: // The low level of INTn generates an interrupt request.
				break;
			}
			exint_reg()->eimsk.var |= (1 << INT3);
			cpu_reg()->sreg.var |= (1 << GLOBAL_INTERRUPT_ENABLE);
		break;
		case 4: // PE4
			exint_reg()->eimsk.var &= ~(1 << INT4);
			exint_reg()->eicrb.var &= ~((1 << ISC41) | (1 << ISC40));
			switch(sense){
				case 0: // The low level of INTn generates an interrupt request.
				break;
				case 1: // Any logical change on INTn generates an interrupt request
					exint_reg()->eicrb.var |= (1 << ISC40);
				break;
				case 2: // The falling edge between two samples of INTn generates an interrupt request.
					exint_reg()->eicrb.var |= (1 << ISC41);
				break;
				case 3: // The rising edge between two samples of INTn generates an interrupt request.
					exint_reg()->eicrb.var |= ((1 << ISC41) | (1 << ISC40));
				break;
				default: // The low level of INTn generates an interrupt request.
				break;
			}
			exint_reg()->eimsk.var |= (1 << INT4);
			cpu_reg()->sreg.var |= (1 << GLOBAL_INTERRUPT_ENABLE);
		break;
		case 5: // PE5
			exint_reg()->eimsk.var &= ~(1 << INT5);
			exint_reg()->eicrb.var &= ~((1 << ISC51) | (1 << ISC50));
			switch(sense){
				case 0: // The low level of INTn generates an interrupt request.
				break;
				case 1: // Any logical change on INTn generates an interrupt request
					exint_reg()->eicrb.var |= (1 << ISC50);
				break;
				case 2: // The falling edge between two samples of INTn generates an interrupt request.
					exint_reg()->eicrb.var |= (1 << ISC51);
				break;
				case 3: // The rising edge between two samples of INTn generates an interrupt request.
					exint_reg()->eicrb.var |= ((1 << ISC51) | (1 << ISC50));
				break;
				default: // The low level of INTn generates an interrupt request.
				break;
			}
			exint_reg()->eimsk.var |= (1 << INT5);
			cpu_reg()->sreg.var |= (1 << GLOBAL_INTERRUPT_ENABLE);
		break;
		case 6: // PE6
			exint_reg()->eimsk.var &= ~(1 << INT6);
			exint_reg()->eicrb.var &= ~((1 << ISC61) | (1 << ISC60));
			switch(sense){
				case 0: // The low level of INTn generates an interrupt request.
				break;
				case 1: // Any logical change on INTn generates an interrupt request
					exint_reg()->eicrb.var |= (1 << ISC60);
				break;
				case 2: // The falling edge between two samples of INTn generates an interrupt request.
					exint_reg()->eicrb.var |= (1 << ISC61);
				break;
				case 3: // The rising edge between two samples of INTn generates an interrupt request.
					exint_reg()->eicrb.var |= ((1 << ISC61) | (1 << ISC60));
				break;
				default: // The low level of INTn generates an interrupt request.
				break;
			}
			exint_reg()->eimsk.var |= (1 << INT6);
			cpu_reg()->sreg.var |= (1 << GLOBAL_INTERRUPT_ENABLE);
		break;
		case 7: // PE7
			exint_reg()->eimsk.var &= ~(1 << INT7);
			exint_reg()->eicrb.var &= ~((1 << ISC71) | (1 << ISC70));
			switch(sense){
				case 0: // The low level of INTn generates an interrupt request.
				break;
				case 1: // Any logical change on INTn generates an interrupt request
					exint_reg()->eicrb.var |= (1 << ISC70);
				break;
				case 2: // The falling edge between two samples of INTn generates an interrupt request.
					exint_reg()->eicrb.var |= (1 << ISC71);
				break;
				case 3: // The rising edge between two samples of INTn generates an interrupt request.
					exint_reg()->eicrb.var |= ((1 << ISC71) | (1 << ISC70));
				break;
				default: // The low level of INTn generates an interrupt request.
				break;
			}
			exint_reg()->eimsk.var |= (1 << INT7);
			cpu_reg()->sreg.var |= (1 << GLOBAL_INTERRUPT_ENABLE);
		break;
		default:
			exint_reg()->eimsk.var = 0X00;
		break;
	}
}
void INTERRUPT_off(uint8_t channel)
{
	switch( channel ){
		case 0: // disable
			exint_reg()->eimsk.var &= ~(1 << INT0);
		break;
		case 1: // disable
			exint_reg()->eimsk.var &= ~(1 << INT1);
		break;
		case 2: // disable
			exint_reg()->eimsk.var &= ~(1 << INT2);
		break;
		case 3: // disable
			exint_reg()->eimsk.var &= ~(1 << INT3);
		break;
		case 4: // disable
			exint_reg()->eimsk.var &= ~(1 <<INT4);
		break;
		case 5: // disable
			exint_reg()->eimsk.var &= ~(1 << INT5);
		break;
		case 6: // disable
			exint_reg()->eimsk.var &= ~(1 << INT6);
		break;
		case 7: // disable
			exint_reg()->eimsk.var &= ~(1 << INT7);
		break;
		default: // all disable
			exint_reg()->eimsk.var = 0X00;
		break;
	}
}
void INTERRUPT_on(uint8_t channel)
{
	switch( channel ){
		case 0:
			exint_reg()->eimsk.var |= (1<<INT0);
			cpu_reg()->sreg.var |= (1 << GLOBAL_INTERRUPT_ENABLE);
		break;
		case 1:
			exint_reg()->eimsk.var |= (1 << INT1);
			cpu_reg()->sreg.var |= (1 << GLOBAL_INTERRUPT_ENABLE);
		break;
		case 2:
			exint_reg()->eimsk.var |= (1 << INT2);
			cpu_reg()->sreg.var |= (1 << GLOBAL_INTERRUPT_ENABLE);
		break;
		case 3:
			exint_reg()->eimsk.var |= (1 << INT3);
			cpu_reg()->sreg.var |= (1 << GLOBAL_INTERRUPT_ENABLE);
		break;
		case 4:
			exint_reg()->eimsk.var |= (1 << INT4);
			cpu_reg()->sreg.var |= (1 << GLOBAL_INTERRUPT_ENABLE);
		break;
		case 5:
			exint_reg()->eimsk.var |= (1 << INT5);
			cpu_reg()->sreg.var |= (1 << GLOBAL_INTERRUPT_ENABLE);
		break;
		case 6:
			exint_reg()->eimsk.var |= (1 << INT6);
			cpu_reg()->sreg.var |= (1 << GLOBAL_INTERRUPT_ENABLE);
		break;
		case 7:
			exint_reg()->eimsk.var |= (1 << INT7);
			cpu_reg()->sreg.var |= (1 << GLOBAL_INTERRUPT_ENABLE);
		break;
		default:
		break;
	}
}

/***EOF***/

