/*************************************************************************
	ATMEGA128TIMER3
Author:   <sergio.salazar.santos@gmail.com>
License:  GNU General Public License
Hardware: ATmega128
Update:   07/01/2024
Comment:
	Stable
*************************************************************************/
/*** File Library ***/
#include "atmega128timer3.h"

/*** File Variable ***/
static TC3 atmega128_tc3;
uint8_t timer3_state;

/*** File Header ***/
void TIMER_COUNTER3_compoutmodeA(unsigned char compoutmode);
void TIMER_COUNTER3_compoutmodeB(unsigned char compoutmode);
void TIMER_COUNTER3_compoutmodeC(unsigned char compoutmode);
void TIMER_COUNTER3_compareA(uint16_t compare);
void TIMER_COUNTER3_compareB(uint16_t compare);
void TIMER_COUNTER3_compareC(uint16_t compare);
uint8_t TIMER_COUNTER3_start(unsigned int prescaler);
uint8_t TIMER_COUNTER3_stop(void);

/*** Procedure & Function ***/
TC3 tc3_enable(unsigned char wavegenmode, unsigned char interrupt)
// PARAMETER SETTING
// wavegen mode: Normal; PWM, Phase Correct, 8-bit; PWM, Phase Correct, 9-bit; PWM, Phase Correct, 10-bit;
// CTC; Fast PWM, 8-bit; Fast PWM, 9-bit; Fast PWM, 10-bit; PWM, Phase and Frequency Correct; PWM, Phase and Frequency Correct;
// PWM, Phase Correct; PWM, Phase Correct; CTC; (Reserved); Fast PWM; Fast PWM.
// interrupt: off; overflow; output compare; both; default - non.
// for more information read data sheet.
{
	// ATMEGA128enable(); // Dependency
	
	timer3_state = 0;
	tc3_instance()->tccr3a.reg &= ~((1 << WGM31) | (1 << WGM30));
	tc3_instance()->tccr3b.reg &= ~((1 << WGM33) | (1 << WGM32));
	switch(wavegenmode){ // TOP -- Update of OCRnX -- TOV Flag Set on
		case 0: // Normal, 0xFFFF -- Immediate -- MAX
		break;
		case 1: // PWM Phase Correct 8-bit, 0x00FF -- TOP -- BOTTOM
			tc3_instance()->tccr3a.reg |= (1 << WGM30);
		break;
		case 2:	// PWM Phase Correct 9-bit, 0x01FF -- TOP -- BOTTOM
			tc3_instance()->tccr3a.reg |= (1 << WGM31);
		break;
		case 3:	// PWM Phase Correct 10-bit, 0x03FF -- TOP -- BOTTOM
			tc3_instance()->tccr3a.reg |= (1 << WGM31) | (1 << WGM30);
		break;
		case 4:	// CTC, OCRnA Immediate MAX
			tc3_instance()->tccr3b.reg |= (1 << WGM32);
		break;
		case 5:	// Fast PWM 8-bit, 0x00FF -- BOTTOM -- TOP
			tc3_instance()->tccr3a.reg |=(1 << WGM30);
			tc3_instance()->tccr3b.reg |= (1 << WGM32);
		break;
		case 6:	// Fast PWM 9-bit, 0x01FF -- BOTTOM -- TOP
			tc3_instance()->tccr3a.reg |= (1 << WGM31);
			tc3_instance()->tccr3b.reg |= (1 << WGM32);
		break;
		case 7:	// Fast PWM 10-bit, 0x03FF -- BOTTOM -- TOP
			tc3_instance()->tccr3a.reg |=(1 << WGM31) | (1 << WGM30);
			tc3_instance()->tccr3b.reg |= (1 << WGM32);
		break;
		case 8:	// PWM Phase and Frequency Correct, ICRnA -- BOTTOM -- BOTTOM
			tc3_instance()->tccr3b.reg |= (1 << WGM33);
		break;
		case 9:	// PWM Phase and Frequency Correct, OCRnA -- BOTTOM -- BOTTOM
			tc3_instance()->tccr3a.reg |= (1 << WGM30);
			tc3_instance()->tccr3b.reg |= (1 << WGM33);
		break;
		case 10: // PWM Phase Correct, ICRn -- TOP -- BOTTOM
			tc3_instance()->tccr3a.reg |=(1 << WGM31);
			tc3_instance()->tccr3b.reg |= (1 << WGM33);
		break;
		case 11: // PWM Phase Correct, OCRnA -- TOP -- BOTTOM
			tc3_instance()->tccr3a.reg |= (1 << WGM31) | (1 << WGM30);
			tc3_instance()->tccr3b.reg |= (1 << WGM33);
		break;
		case 12: // CTC, ICRn -- Immediate -- MAX
			tc3_instance()->tccr3b.reg |= (1 << WGM33) | (1 << WGM32);
		break;
		case 13: // (Reserved), -- -- --
			tc3_instance()->tccr3a.reg |= (1 << WGM30);
			tc3_instance()->tccr3b.reg |= (1 << WGM33) | (1 << WGM32);
		break;
		case 14: // Fast PWM, ICRn -- BOTTOM -- TOP
			tc3_instance()->tccr3a.reg |= (1 << WGM31);
			tc3_instance()->tccr3b.reg |= (1 << WGM33) | (1 << WGM32);
		break;
		case 15: // Fast PWM, OCRnA -- BOTTOM -- TOP
			tc3_instance()->tccr3a.reg |= (1 << WGM31) | (1 << WGM30);
			tc3_instance()->tccr3b.reg |= (1 << WGM33) | (1 << WGM32);
		break;
		default:
		break;
	}
	tc3_instance()->tccr3a.reg &= ~((3 << COM3A0) | (3 << COM3B0)| (3 << COM3C0));
	tc3_instance()->etimsk.reg &= ~((1 << TICIE3) | (1 << OCIE3A) | (1 << OCIE3B) | (1 << TOIE3) | (1 << OCIE3C));
	switch(interrupt){ // ICP3  -->  PE7
		case 0:
		break;
		case 1:
			tc3_instance()->etimsk.reg |= (1 << TOIE3);
			cpu_instance()->sreg.reg |= 1 << GLOBAL_INTERRUPT_ENABLE;
		break;
		case 2:
			tc3_instance()->etimsk.reg |= (1 << OCIE3A);
			cpu_instance()->sreg.reg |= 1 << GLOBAL_INTERRUPT_ENABLE;
		break;
		case 3:
			tc3_instance()->etimsk.reg |= (1 << OCIE3B);
			cpu_instance()->sreg.reg |= 1 << GLOBAL_INTERRUPT_ENABLE;
		break;
		case 4:
			tc3_instance()->etimsk.reg |= (1 << OCIE3C);
			cpu_instance()->sreg.reg |= 1 << GLOBAL_INTERRUPT_ENABLE;
		break;
		case 5:
			tc3_instance()->etimsk.reg |= (1 << TICIE3);
			cpu_instance()->sreg.reg |= 1 << GLOBAL_INTERRUPT_ENABLE;
		break;
		case 6:
			tc3_instance()->etimsk.reg |= (1 << OCIE3A) | (1 << TOIE3);
			cpu_instance()->sreg.reg |= 1 << GLOBAL_INTERRUPT_ENABLE;
		break;
		case 7:
			tc3_instance()->etimsk.reg |= (1 << OCIE3B) | (1 << TOIE3);
			cpu_instance()->sreg.reg |= 1 << GLOBAL_INTERRUPT_ENABLE;
		break;
		case 8:
			tc3_instance()->etimsk.reg |= (1 << TOIE3) | (1 << OCIE3C);
			cpu_instance()->sreg.reg |= 1 << GLOBAL_INTERRUPT_ENABLE;
		break;
		case 9:
			tc3_instance()->etimsk.reg |= (1 << TICIE3) | (1 << TOIE3);
			cpu_instance()->sreg.reg |= 1 << GLOBAL_INTERRUPT_ENABLE;
		break;
		case 10:
			tc3_instance()->etimsk.reg |= (1 << OCIE3A) | (1 << OCIE3B) | (1 << TOIE3);
			cpu_instance()->sreg.reg |= 1 << GLOBAL_INTERRUPT_ENABLE;
		break;
		case 11:
			tc3_instance()->etimsk.reg |= (1 << OCIE3A) | (1 << OCIE3B) | (1 << TOIE3) | (1 << OCIE3C);
			cpu_instance()->sreg.reg |= 1 << GLOBAL_INTERRUPT_ENABLE;
		break;
		case 12:
			tc3_instance()->etimsk.reg |= (1 << OCIE3A) | (1 << OCIE3B) | (1 << OCIE3C);
			cpu_instance()->sreg.reg |= 1 << GLOBAL_INTERRUPT_ENABLE;
		break;
		default:
		break;
	}
	tc3_instance()->ocr3a = writeHLbyte(~0);
	tc3_instance()->ocr3b = writeHLbyte(~0);
	tc3_instance()->ocr3c = writeHLbyte(~0);
	
	atmega128_tc3.instance = tc3_instance();
	atmega128_tc3.compoutmodeA = TIMER_COUNTER3_compoutmodeA;
	atmega128_tc3.compoutmodeB = TIMER_COUNTER3_compoutmodeB;
	atmega128_tc3.compoutmodeC = TIMER_COUNTER3_compoutmodeC;
	atmega128_tc3.compareA = TIMER_COUNTER3_compareA;
	atmega128_tc3.compareB = TIMER_COUNTER3_compareB;
	atmega128_tc3.compareC = TIMER_COUNTER3_compareC;
	atmega128_tc3.start = TIMER_COUNTER3_start;
	atmega128_tc3.stop = TIMER_COUNTER3_stop;
	
	return atmega128_tc3;
}

TC3* tc3(void){ return &atmega128_tc3;}

uint8_t TIMER_COUNTER3_start(unsigned int prescaler)
// PARAMETER SETTING
// Frequency oscillator devision factor or prescaler.
// prescaler: clk T0S /(No prescaling); clk T0S /8 (From prescaler); clk T0S /64 (From prescaler);
// clk T0S /256 (From prescaler); clk T0S /1024 (From prescaler); External clock source on Tn pin. Clock on falling edge;
// External clock source on Tn pin. Clock on rising edge; default - clk T 0 S /1024 (From prescaler).
{
	if(!timer3_state){ // one shot
		tc3_instance()->tccr3b.reg &= ~(7 << CS30); // No clock source. (Timer/Counter stopped)
		switch(prescaler){
			//case 0: // No clock source. (Timer/Counter stopped)
			//break;
			case 1: // clkI/O/1 (No prescaler)
				tc3_instance()->tccr3b.reg |= (1 << CS30);
			break;
			case 8: // clkI/O/8 (From prescaler)
				tc3_instance()->tccr3b.reg |= (1 << CS31);
			break;
			case 64: // clkI/O/64 (From prescaler)
				tc3_instance()->tccr3b.reg |= (3 << CS30);
			break;
			case 256: // clkI/O/256 (From prescaler)
				tc3_instance()->tccr3b.reg |= (1 << CS32);
			break;
			case 1024: // clkI/O/1024 (From prescaler)
				tc3_instance()->tccr3b.reg |= (5 << CS30);
			break;
			case 6: // External clock source on Tn pin. Clock on falling edge [PE6]
				tc3_instance()->tccr3b.reg |= (6 << CS30);
			break;
			case 7: // External clock source on Tn pin. Clock on rising edge [PE6]
				tc3_instance()->tccr3b.reg |= (7 << CS30);
			break;
			default:
				tc3_instance()->tccr3b.reg |= (5 << CS30);
			break;
		}
		timer3_state = 85;
	}	
	return timer3_state;
}
void TIMER_COUNTER3_compoutmodeA(unsigned char compoutmode)
{
	tc3_instance()->tccr3a.reg &= ~(3 << COM3A0);
	switch(compoutmode){ // OC3A  -->  PE3
		case 0: // Normal port operation, OC3A disconnected.
		break;
		case 1: // Reserved
			// Toggle OC3A on compare match
			porte_instance()->ddr.reg |= 0x08;
			tc3_instance()->tccr3a.reg |= (1 << COM3A0);
		break;
		case 2: // Clear OC3A on compare match when up-counting. Set OC0 on compare
			// match when down counting.
			porte_instance()->ddr.reg |= 0x08;
			tc3_instance()->tccr3a.reg |= (1 << COM3A1);
		break;
		case 3: // Set OC3A on compare match when up-counting. Clear OC0 on compare
			// match when down counting.
			porte_instance()->ddr.reg |= 0x08;
			tc3_instance()->tccr3a.reg |= (1 << COM3A0) | (1 << COM3A1);
		break;
		default:
		break;
	}
}
void TIMER_COUNTER3_compoutmodeB(unsigned char compoutmode)
{
	tc3_instance()->tccr3a.reg &= ~(3 << COM3B0);
	switch(compoutmode){ // OC3B  -->  PE4
		case 0: // Normal port operation, OC3B disconnected.
		break;
		case 1: // Reserved
			// Toggle OC3A or OC3B on compare match
			porte_instance()->ddr.reg |= 0x10;
			tc3_instance()->tccr3a.reg |= (1 << COM3B0);
		break;
		case 2: // Clear OC3B on compare match when up-counting. Set OC3B on compare
			// match when down counting.
			porte_instance()->ddr.reg |= 0x10;
			tc3_instance()->tccr3a.reg |= (1 << COM3B1);
		break;
		case 3: // Set OC3B on compare match when up-counting. Clear OC3B on compare
			// match when down counting.
			porte_instance()->ddr.reg |= 0x10;
			tc3_instance()->tccr3a.reg |= (1 << COM3B0) | (1 << COM3B1);
		break;
		default:
		break;
	}
}
void TIMER_COUNTER3_compoutmodeC(unsigned char compoutmode)
{
	tc3_instance()->tccr3a.reg &= ~(3 << COM3C0);
	switch(compoutmode){ // OC3C  -->  PE5
		case 0: // Normal port operation, OC3C disconnected.
		break;
		case 1: // Reserved
			// Toggle OC3A or OC3C on compare match
			porte_instance()->ddr.reg |= 0x20;
			tc3_instance()->tccr3a.reg |= (1 << COM3C0);
		break;
		case 2: // Clear OC3C on compare match when up-counting. Set OC3C on compare
			// match when down counting.
			porte_instance()->ddr.reg |= 0x20;
			tc3_instance()->tccr3a.reg |= (1 << COM3C1);
		break;
		case 3: // Set OC3C on compare match when up-counting. Clear OC3C on compare
			// match when down counting.
			porte_instance()->ddr.reg |= 0x20;
			tc3_instance()->tccr3a.reg |= (1 << COM3C0) | (1 << COM3C1);
		break;
		default:
		break;
	}
}
void TIMER_COUNTER3_compareA(uint16_t compare)
{
	tc3_instance()->ocr3a = writeHLbyte(compare);
}
void TIMER_COUNTER3_compareB(uint16_t compare)
{
	tc3_instance()->ocr3b = writeHLbyte(compare);
}
void TIMER_COUNTER3_compareC(uint16_t compare)
{
	tc3_instance()->ocr3c = writeHLbyte(compare);
}
uint8_t TIMER_COUNTER3_stop(void)
// stops timer by setting prescaler to zero
{
	tc3_instance()->tccr3b.reg &= ~(7 << CS30); // No clock source. (Timer/Counter stopped)
	timer3_state = 0;
	return timer3_state;
}

/***EOF***/

