/************************************************************************
	ATMEGA128TIMER0
Author:   <sergio.salazar.santos@gmail.com>
License:  GNU General Public License
Hardware: ATmega128
Update:   07/01/2024
Comment:
	Stable
************************************************************************/
/*** Preamble Inic ***/
#ifndef _ATMEGA128TIMER0_H_
	#define _ATMEGA128TIMER0_H_

/*** Global Library ***/
#include "atmega128.h"

/*** Constant & Macro ***/
#ifndef ATMEGA_128_TIMER_COUNTER
	#define ATMEGA_128_TIMER_COUNTER

#ifndef GLOBAL_INTERRUPT_ENABLE
	#define GLOBAL_INTERRUPT_ENABLE 7
#endif

#if defined(__AVR_ATmega64__) || defined(__AVR_ATmega128__)
#else
	#error "Not Atmega 128"
#endif

#endif

/*** Global Variable ***/
typedef struct{
	Atmega128TimerCounter0_TypeDef* instance;
	// prototype pointers
	void (*compoutmode)(unsigned char compoutmode);
	void (*compare)(unsigned char compare);
	uint8_t (*start)(unsigned int prescaler);
	uint8_t (*stop)(void);
}TC0;

/*** Global Header ***/
TC0* tc0(void);
TC0 tc0_enable(unsigned char wavegenmode, unsigned char interrupt);

#endif
/***EOF***/

