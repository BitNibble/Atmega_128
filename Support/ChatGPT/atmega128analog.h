#ifndef _ATMEGA128ANALOG_H_
#define _ATMEGA128ANALOG_H_

/*** Global Library ***/
#include "atmega128instance.h"
#include <avr/interrupt.h>
#include <stdio.h>

/*** Global Constant & Macro ***/
#if defined(__AVR_ATmega64__) || defined(__AVR_ATmega128__)
#ifndef GLOBAL_INTERRUPT_ENABLE
#define GLOBAL_INTERRUPT_ENABLE 7
#endif

#define MAX_CHANNEL 32
#define ADC_NUMBER_SAMPLE 2
#define MUX_MASK 31
#else
#error "This code is only compatible with ATmega64/128"
#endif

/*** Global Variable ***/
typedef struct {
	uint8_t VREFF;
	uint8_t DIVISION_FACTOR;
} analogparameter;

typedef struct {
	analogparameter par;
	Atmega128AnalogToDigitalConverter_TypeDef* instance;
	int (*read)(int selection);
} ADC0;

/*** Global Header ***/
ADC0 adc_enable(uint8_t Vreff, uint8_t Divfactor, int n_channel, ...);
ADC0* adc(void);

#endif  // _ATMEGA128ANALOG_H_

