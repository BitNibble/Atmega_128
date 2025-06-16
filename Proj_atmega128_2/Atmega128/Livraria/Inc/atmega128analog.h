/************************************************************************
	ATMEGA128ANALOG
Author:   <sergio.salazar.santos@gmail.com>
License:  GNU General Public License
Hardware: ATmega128
Update:   07/01/2024
Comment:
	Stable
************************************************************************/
#ifndef _ATMEGA128ANALOG_H_
	#define _ATMEGA128ANALOG_H_

/*** Global Library ***/
#include "atmega128instance.h"

/*** Global Constant & Macro ***/
#if defined(__AVR_ATmega64__) || defined(__AVR_ATmega128__)
	#ifndef GLOBAL_INTERRUPT_ENABLE
		#define GLOBAL_INTERRUPT_ENABLE 7
	#endif
	// if using differential channels this value has to be greater than one
	#define MAX_CHANNEL 32
	// ADC_NUMBER_SAMPLE^2 gives number of samples, note values can only range from 0 to 4.
	#define ADC_NUMBER_SAMPLE 2
	#define MUX_MASK 31
#else
	#error "Not Atmega 128"
#endif

/*** Global Variable ***/
typedef struct{
	uint8_t VREFF;
	uint8_t DIVISION_FACTOR;
}analogparameter;

typedef struct{
	analogparameter par;
	Atmega128AnalogToDigitalConverter_TypeDef* instance;
	// prototype pointers
	int (*read)(int selection);
}ADC0;

/*** Global Header ***/
ADC0 adc_enable( uint8_t Vreff, uint8_t Divfactor, int n_channel, ... );
ADC0* adc(void);

#endif

/***EOF***/

