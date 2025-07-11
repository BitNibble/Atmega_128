/*************************************************************************
	ATMEGA128TIMER2
Author:   <sergio.salazar.santos@gmail.com>
License:  GNU General Public License
Hardware: ATmega128
Update:   07/01/2024
Comment:
	Stable
*************************************************************************/
/*** File Library ***/
#include "atmega128timer2.h"

/*** File Variable ***/
static TC2 atmega128_tc2;
uint8_t timer2_state;

/*** File Header ***/
void TIMER_COUNTER2_compoutmode(unsigned char compoutmode);
void TIMER_COUNTER2_compare(unsigned char compare);
uint8_t TIMER_COUNTER2_start(unsigned int prescaler);
uint8_t TIMER_COUNTER2_stop(void);

/*** Procedure & Function ***/
TC2 tc2_enable(unsigned char wavegenmode, unsigned char interrupt)
// PARAMETER SETTING
// wavegen mode: Normal; PWM phase correct; Fast PWM; default-Normasl;
// interrupt: off; overflow; output compare; both; default - non.
{
	// ATMEGA128enable(); // Dependency
	
	timer2_state = 0;
	tc2_reg()->tccr2->var &= ~((1 << WGM20) | (1 << WGM21));
	switch(wavegenmode){ // TOP -- Update of OCR2 at -- TOV0 Flag Set on
		case 0: // Normal, 0xFF -- Immediate -- MAX
		break;
		case 1: // PWM Phase Correct, 0xFF -- TOP -- BOTTOM
			tc2_reg()->tccr2->var |= (1 << WGM20);
		break;
		case 2: // CTC, OCR2 -- Immediate -- MAX
			tc2_reg()->tccr2->var |= (1 << WGM21);
		break;
		case 3: // Fast PWM, 0xFF -- BOTTOM -- MAX
			tc2_reg()->tccr2->var |= (1 << WGM20) | (1 << WGM21);
		break;
		default:
		break;
	}
	tc2_reg()->timsk->var &= ~((1 << TOIE2) | (1 << OCIE2));
	switch(interrupt){
		case 0:
		break;
		case 1:
			tc2_reg()->timsk->var |= (1 << TOIE2);
			cpu_reg()->sreg->var |= 1 << GLOBAL_INTERRUPT_ENABLE;
		break;
		case 2:
			tc2_reg()->timsk->var |= (1 << OCIE2);
			cpu_reg()->sreg->var |= 1 << GLOBAL_INTERRUPT_ENABLE;
		break;
		case 3:
			tc2_reg()->timsk->var |= (1 << TOIE2);
			tc2_reg()->timsk->var |= (1 << OCIE2);
			cpu_reg()->sreg->var |= 1 << GLOBAL_INTERRUPT_ENABLE;
		break;
		default:
		break;
	}
	tc2_reg()->ocr2->var = ~0;
	
	atmega128_tc2.instance = tc2_reg();
	atmega128_tc2.compoutmode = TIMER_COUNTER2_compoutmode;
	atmega128_tc2.compare = TIMER_COUNTER2_compare;
	atmega128_tc2.start = TIMER_COUNTER2_start;
	atmega128_tc2.stop = TIMER_COUNTER2_stop;
	
	return atmega128_tc2;
}

TC2* tc2(void){ return &atmega128_tc2; }

uint8_t TIMER_COUNTER2_start(unsigned int prescaler)
// PARAMETER SETTING
// Frequency oscillator devision factor or prescaler.
// prescaler: clk T0S /(No prescaling); clk T0S /8 (From prescaler); clk T0S /64 (From prescaler);
// clk T0S /256 (From prescaler); clk T0S /1024 (From prescaler); External clock source on Tn pin. Clock on falling edge;
// External clock source on Tn pin. Clock on rising edge; default - clk T 0 S /1024 (From prescaler).
{
	if(!timer2_state){ // one shot
		tc2_reg()->tccr2->var &= ~(7 << CS20); // No clock source. (Timer/Counter stopped)
		switch(prescaler){
			//case 0: // No clock source. (Timer/Counter stopped)
			//break;
			case 1: // clkI/O/(No prescaler)
				tc2_reg()->tccr2->var |= (1 << CS20);
			break;
			case 8: // clkI/O/8 (From prescaler)
				tc2_reg()->tccr2->var |= (1 << CS21);
			break;
			case 64: // clkI/O/64 (From prescaler)
				tc2_reg()->tccr2->var |= (3 << CS20);
			break;
			case 256: // clkI/O/256 (From prescaler)
				tc2_reg()->tccr2->var |= (1 << CS22);
			break;
			case 1024: // clkI/O/1024 (From prescaler)
				tc2_reg()->tccr2->var |= (5 << CS20);
			break;
			case 6: // External clock source on T2 pin. Clock on falling edge [PD7]
				tc2_reg()->tccr2->var |= (6 << CS20);
			break;
			case 7: // External clock source on T2 pin. Clock on rising edge [PD7]
				tc2_reg()->tccr2->var |= (7 << CS20);
			break;
			default:
				tc2_reg()->tccr2->var |= (5 << CS20);
			break;
		}
		timer2_state = 85;
	}
	return timer2_state;
}
void TIMER_COUNTER2_compoutmode(unsigned char compoutmode)
// compoutmode: Normal port operation, OC0 disconnected; Toggle OC0 on compare match; 
// Clear OC0 on compare match when up-counting. Set OC0 on compare match when downcounting. Clear OC0 on compare match;
// Set OC0 on compare match when up-counting. Clear OC0 on compare match when downcounting. Set OC0 on compare match ;
// default-Normal port operation, OC0 disconnected.
{
	tc2_reg()->tccr2->var &= ~((1 << COM20) | (1 << COM21));
	switch(compoutmode){ // OC2  -->  PB7
		case 0: // Normal port operation, OC2 disconnected.
		break;
		case 1: // Reserved
			// Toggle OC2 on compare match
			portb_reg()->ddr->var |= 0x80;
			tc2_reg()->tccr2->var |= (1 << COM20);
		break;
		case 2: // Clear OC2 on compare match when up-counting. Set OC0 on compare
			// match when down counting.
			portb_reg()->ddr->var |= 0x80;
			tc2_reg()->tccr2->var |= (1 << COM21);
		break;
		case 3: // Set OC2 on compare match when up-counting. Clear OC0 on compare
			// match when down counting.
			portb_reg()->ddr->var |= 0x80;
			tc2_reg()->tccr2->var |= (1 << COM20) | (1 << COM21);
		break;
		default:
		break;
	}
}
void TIMER_COUNTER2_compare(unsigned char compare)
{
	tc2_reg()->ocr2->var = compare;
}
uint8_t TIMER_COUNTER2_stop(void)
// stops timer by setting prescaler to zero
{
	tc2_reg()->tccr2->var &= ~(7 << CS20); // No clock source. (Timer/Counter stopped)
	timer2_state = 0;
	return timer2_state;
}

/***EOF***/

