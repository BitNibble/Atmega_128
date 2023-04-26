/************************************************************************
	ATMEGA128ANALOG
Author: Sergio Santos 
	<sergio.salazar.santos@gmail.com>
License: GNU General Public License
Hardware: ATmega128
Date: 24042023
Comment:
	Stable
************************************************************************/
#ifndef _ATMEGA128ANALOG_H_
	#define _ATMEGA128ANALOG_H_
	
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
// ADC_NUMBER_SAMPLE^2 gives number of samples, note values can only range from 0 to 4.
#define ADC_NUMBER_SAMPLE 2

/*** Global Variable ***/
struct nlg{
	uint8_t VREFF;
	uint8_t DIVISION_FACTOR;
	int (*read)(int selection);
};
typedef struct nlg ANALOG;

/*** Global Header ***/
ANALOG ANALOGenable( uint8_t Vreff, uint8_t Divfactor, int n_channel, ... );

#endif
/***EOF***/

