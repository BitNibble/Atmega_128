/*************************************************************************
	ATMEGA128TIMER0
Author: Sergio Santos 
	<sergio.salazar.santos@gmail.com>
License: GNU General Public License
Hardware: ATmega128
Update: 07/01/2024
Comment:
	Stable
*************************************************************************/
/*** File Library ***/
#include "atmega128mapping.h"
#include "atmega128timer0.h"

/*** File Variable ***/
static TC0 atmega128_tc0;
uint8_t timer0_state;

/*** File Header ***/
void TIMER_COUNTER0_compoutmode(unsigned char compoutmode);
void TIMER_COUNTER0_compare(unsigned char compare);
uint8_t TIMER_COUNTER0_start(unsigned int prescaler);
uint8_t TIMER_COUNTER0_stop(void);

/*** Procedure & Function ***/
TC0 tc0_enable(unsigned char wavegenmode, unsigned char interrupt)
// PARAMETER SETTING
// wavegen mode: Normal; PWM phase correct; Fast PWM; default-Normasl;
// interrupt: off; overflow; output compare; both; default - non.
{
	// ATMEGA128enable(); // Dependency
	
	timer0_state = 0;
	atmega128()->tc0_handle->tccr0.reg &= ~((1 << WGM00) | (1 << WGM01));
	switch(wavegenmode){ // TOP -- Update of OCR0 at -- TOV0 Flag Set on
		case 0: // Normal, 0xFF -- Immediate -- MAX
		break;
		case 1: // PWM Phase Correct, 0xFF -- TOP -- BOTTOM
			atmega128()->tc0_handle->tccr0.reg |= (1 << WGM00);
		break;
		case 2: // CTC, OCR0 -- Immediate -- MAX
			atmega128()->tc0_handle->tccr0.reg |= (1 << WGM01);
		break;
		case 3: // Fast PWM, 0xFF -- BOTTOM -- MAX
			atmega128()->tc0_handle->tccr0.reg |= (1 << WGM00) | (1 << WGM01);
		break;
		default:
		break;
	}
	atmega128()->tc0_handle->timsk.reg &= ~(1 << TOIE0);
	atmega128()->tc0_handle->timsk.reg &= ~(1 << OCIE0);
	switch(interrupt){
		case 0:
		break;
		case 1:
			atmega128()->tc0_handle->timsk.reg |= (1 << TOIE0);
			atmega128()->cpu_handle->sreg.reg |= 1 << GLOBAL_INTERRUPT_ENABLE;
		break;
		case 2:
			atmega128()->tc0_handle->timsk.reg |= (1 << OCIE0);
			atmega128()->cpu_handle->sreg.reg |= 1 << GLOBAL_INTERRUPT_ENABLE;
		break;
		case 3:
			atmega128()->tc0_handle->timsk.reg |= ((1 << TOIE0) | (1 << OCIE0));
			atmega128()->cpu_handle->sreg.reg |= 1 << GLOBAL_INTERRUPT_ENABLE;
		break;
		default:
		break;
	}
	atmega128()->tc0_handle->ocr0.reg = ~0;
	
	atmega128_tc0.compoutmode = TIMER_COUNTER0_compoutmode;
	atmega128_tc0.compare = TIMER_COUNTER0_compare;
	atmega128_tc0.start = TIMER_COUNTER0_start;
	atmega128_tc0.stop = TIMER_COUNTER0_stop;
	return atmega128_tc0;
}

TC0* tc0(void){ return &atmega128_tc0; }

uint8_t TIMER_COUNTER0_start(unsigned int prescaler)
// PARAMETER SETTING
// Frequency oscillator devision factor or prescaler.
// prescaler: clk T0S /(No prescaling); clk T0S /8 (From prescaler); clk T0S /32 (From prescaler);
// clk T0S /64 (From prescaler); clk T0S /128 (From prescaler); clk T 0 S /256 (From prescaler);
// clk T 0 S /1024 (From prescaler); default - clk T 0 S /1024 (From prescaler).
{
	if(!timer0_state){ // one shot
		atmega128()->tc0_handle->tccr0.reg &= ~(7 << CS00); // No clock source. (Timer/Counter stopped)
		switch(prescaler){
			//case 0: // No clock source. (Timer/Counter stopped)
			//break;
			case 1: // clk T0S /(No prescaler)
				atmega128()->tc0_handle->tccr0.reg |= (1 << CS00);
			break;
			case 8: // clk T0S /8 (From prescaler)
				atmega128()->tc0_handle->tccr0.reg |= (1 << CS01);
			break;
			case 32: // clk T0S /32 (From prescaler)
				atmega128()->tc0_handle->tccr0.reg |=(3 << CS00);
			break;
			case 64: // clk T0S /64 (From prescaler)
				atmega128()->tc0_handle->tccr0.reg |= (4 << CS00);
			break;
			case 128: // clk T0S /128 (From prescaler)
				atmega128()->tc0_handle->tccr0.reg |= (5 << CS00);
			break;
			case 256: // clk T 0 S /256 (From prescaler)
				atmega128()->tc0_handle->tccr0.reg |= (6 << CS00);
			break;
			case 1024: // clk T 0 S /1024 (From prescaler)
				atmega128()->tc0_handle->tccr0.reg |= (7 << CS00);
			break;
			default:
				atmega128()->tc0_handle->tccr0.reg |= (7 << CS00);
			break;
		}
		timer0_state = 85;
	}
	return timer0_state;
}
void TIMER_COUNTER0_compoutmode(unsigned char compoutmode)
// compoutmode: Normal port operation, OC0 disconnected; Toggle OC0 on compare match;
// Clear OC0 on compare match when up-counting. Set OC0 on compare match when downcounting. Clear OC0 on compare match;
// Set OC0 on compare match when up-counting. Clear OC0 on compare match when downcounting. Set OC0 on compare match ;
// default-Normal port operation, OC0 disconnected.
{
	atmega128()->tc0_handle->tccr0.reg &= ~((1 << COM00) | (1 << COM01));
	switch(compoutmode){ // OC0  -->  PB4
		case 0: // Normal port operation, OC0 disconnected.
		break;
		case 1: // Reserved
			// Toggle OC0 on compare match
			atmega128()->portb_handle->ddr.reg = 0x10;
			atmega128()->tc0_handle->tccr0.reg |= (1 << COM00);
		break;
		case 2: // Clear OC0 on compare match when up-counting. Set OC0 on compare
			// match when down counting.
			atmega128()->portb_handle->ddr.reg = 0x10;
			atmega128()->tc0_handle->tccr0.reg |= (1 << COM01);
		break;
		case 3: // Set OC0 on compare match when up-counting. Clear OC0 on compare
			// match when down counting.
			atmega128()->portb_handle->ddr.reg = 0x10;
			atmega128()->tc0_handle->tccr0.reg |= (1 << COM00) | (1 << COM01);
		break;
		default:
		break;
	}
}
void TIMER_COUNTER0_compare(unsigned char compare)
{
	atmega128()->tc0_handle->ocr0.reg = compare;
}
uint8_t TIMER_COUNTER0_stop(void)
// stops timer by setting prescaler to zero
{
	atmega128()->tc0_handle->tccr0.reg &= ~(7 << CS00); // No clock source. (Timer/Counter stopped)
	timer0_state = 0;
	return timer0_state;
}

/***EOF***/

