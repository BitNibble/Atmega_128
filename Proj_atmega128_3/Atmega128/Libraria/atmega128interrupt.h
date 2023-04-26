/************************************************************************
	ATMEGA128INTERRUPT
Author: Sergio Santos 
	<sergio.salazar.santos@gmail.com>
License: GNU General Public License
Hardware: ATmega128
Date: 24042023
Comment:
	Stable
************************************************************************/
#ifndef _ATMEGA128INTERRUPT_H_
	#define _ATMEGA128INTERRUPT_H_

/*** Compiler ***/
#if (__GNUC__ * 100 + __GNUC_MINOR__) < 304
	#error "This library requires AVR-GCC 3.4 or later, update to newer AVR-GCC compiler !"
#endif
/*** Working Frequency ***/
#ifndef F_CPU
	#define F_CPU 16000000UL
#endif

/*** Global Library ***/
#include <inttypes.h>

/*** Global Constant & Macro ***/
#ifndef GLOBAL_INTERRUPT_ENABLE
	#define GLOBAL_INTERRUPT_ENABLE 7
#endif

/*** Global Variable ***/
struct ntrrpt{
	void (*set)(uint8_t channel, uint8_t sense);
	void (*off)(uint8_t channel);
	void (*on)(uint8_t channel);
	uint8_t (*reset_status)(void);
};
typedef struct ntrrpt INTERRUPT;

/*** Global Header ***/
INTERRUPT INTERRUPTenable(void);

#endif
/***EOF***/

