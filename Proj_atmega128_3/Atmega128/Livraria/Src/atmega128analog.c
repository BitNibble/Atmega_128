/**********************************************************************
	ATMEGA128ANALOG
Author:   <sergio.salazar.santos@gmail.com>
License:  GNU General Public License
Hardware: Atmega128 by ETT ET-BASE
Date:     03/09/2025
**********************************************************************/
/*** Library ***/
#include "atmega128analog.h"
#include <avr/interrupt.h>
#include <stdio.h>

static volatile int ADC_VALUE[MAX_CHANNEL];
static volatile int ADC_CHANNEL_GAIN[MAX_CHANNEL];
int ADC_N_CHANNEL;
static volatile int ADC_SELECTOR;
static volatile int adc_sample;
uint16_t adc_tmp;
static volatile unsigned char adc_n_sample;

/*** Procedure and Function declaration ***/
int ANALOG_read(int selection);

/*** Default Callback declaration ***/
static void adc_callback(void);

/*** Internal State ***/
static ADC0_Handler atmega128_adc = {
	// Parameter
	.par = {
		.VREFF = 0,
		.DIVISION_FACTOR = ADC_NUMBER_SAMPLE
	},
	// Callback
	.callback = {
		.adc_vect = adc_callback
	},
	// V-table
	.read = ANALOG_read	
};

/*** Handler ***/
void adc_enable( uint8_t Vreff, uint8_t Divfactor, uint8_t n_channel, ... )
// Interrupt running mode setup, and list of channels to be probed
{
	// LOCAL VARIABLES
	va_list list;
	int i;
	// Initialize variables
	if (n_channel > 0 && n_channel < MAX_CHANNEL) ADC_N_CHANNEL = n_channel;
	else ADC_N_CHANNEL = 1;
	
	ADC_SELECTOR = 0;
	adc_n_sample = 0;
	
	atmega128()->adc->admux.par.refs = 0;
	switch( Vreff ){
		case 0:
			atmega128_adc.par.VREFF = 0;
		break;
		case 1:
			//atmega128()->adc->admux.var |= (1 << REFS0);
			atmega128()->adc->admux.par.refs = 1;
			atmega128_adc.par.VREFF = 1;
		break;
		case 3:
			//atmega128()->adc->admux.var |= (3 << REFS0);
			atmega128()->adc->admux.par.refs = 3;
			atmega128_adc.par.VREFF = 3;
		break;
		default:
			atmega128_adc.par.VREFF = 0;
		break;
	}
		
	//atmega128()->adc->admux.var &= ~(1 << ADLAR);
	atmega128()->adc->admux.par.adlar = 0;
	// atmega128.adc->admux |= (1 << ADLAR);
	
	va_start(list, n_channel);
	for(i = 0; i < n_channel; i++){
		ADC_CHANNEL_GAIN[i] = va_arg(list, int);
	}
	va_end(list);
	
	//atmega128()->adc->admux.var &= ~MUX_MASK;
	atmega128()->adc->admux.par.mux = 0;
	//atmega128()->adc->admux.var |= (MUX_MASK & ADC_CHANNEL_GAIN[ADC_SELECTOR]);
	atmega128()->adc->admux.par.mux = ADC_CHANNEL_GAIN[ADC_SELECTOR] & MUX_MASK;
	
	//atmega128()->adc->adcsra.var |= (1 << ADSC);
	atmega128()->adc->adcsra.par.adsc = 1;
	//atmega128()->adc->adcsra.var &= ~(1 << ADFR);
	atmega128()->adc->adcsra.par.adfr = 0;
	//atmega128()->adc->adcsra.var |= (1 << ADIE);
	atmega128()->adc->adcsra.par.adie = 1;
	//atmega128()->adc->adcsra.var &= ~(7 << ADPS0);
	atmega128()->adc->adcsra.par.adps = 0;
	
	switch( Divfactor ){
		case 2: // 1
			//atmega128()->adc->adcsra.var |= (1 << ADPS0);
			atmega128()->adc->adcsra.par.adps = 1;
			atmega128_adc.par.DIVISION_FACTOR = 2;
		break;
		case 4: // 2
			//atmega128()->adc->adcsra.var |= (1 << ADPS1);
			atmega128()->adc->adcsra.par.adps = 2;
			atmega128_adc.par.DIVISION_FACTOR = 4;
		break;
		case 8: // 3
			//atmega128()->adc->adcsra.var |= (3 << ADPS0);
			atmega128()->adc->adcsra.par.adps = 3;
			atmega128_adc.par.DIVISION_FACTOR = 8;
		break;
		case 16: // 4
			//atmega128()->adc->adcsra.var |= (1 << ADPS2);
			atmega128()->adc->adcsra.par.adps = 4;
			atmega128_adc.par.DIVISION_FACTOR = 16;
		break;
		case 32: // 5
			//atmega128()->adc->adcsra.var |= (5 << ADPS0);
			atmega128()->adc->adcsra.par.adps = 5;
			atmega128_adc.par.DIVISION_FACTOR = 32;
		break;
		case 64: // 6
			//atmega128()->adc->adcsra.var |= (6 << ADPS0);
			atmega128()->adc->adcsra.par.adps = 6;
			atmega128_adc.par.DIVISION_FACTOR = 64;
		break;
		case 128: // 7
			//atmega128()->adc->adcsra.var |= (7 << ADPS0);
			atmega128()->adc->adcsra.par.adps = 7;
			atmega128_adc.par.DIVISION_FACTOR = 128;
		break;
		default:
			//atmega128()->adc->adcsra.var |= (7 << ADPS0);
			atmega128()->adc->adcsra.par.adps = 7;
			atmega128_adc.par.DIVISION_FACTOR = 128;
		break;
	}
	
	//atmega128()->adc->adcsra.var |= (1 << ADEN);
	atmega128()->adc->adcsra.par.aden = 1;
	
	atmega128()->cpu->sreg.var |= (1 << 7);
}

ADC0_Handler* adc(void){ return &atmega128_adc; }

/*** Procedure and Function definition ***/
int ANALOG_read(int selection)
// Returns selected Channel ADC_VALUE
{
	//uint8_t ADSC_FLAG;
	//ADSC_FLAG = (1 << ADSC);
	//if( !(atmega128()->adc->adcsra.var & ADSC_FLAG) ){
	if( !atmega128()->adc->adcsra.par.adsc ){
		// ADC_SELECT
		//atmega128()->adc->adcsra.var |= (1 << ADSC);
		atmega128()->adc->adcsra.par.adsc = 1;
	}	
	return ADC_VALUE[selection];
}

/*** Default Callback definition ***/
static void adc_callback(void)
{
	// adc_tmp = atmega128.adc->adc.L; // ADCL
	// adc_tmp |= (atmega128.adc->adc.H << 8); // (ADCH << 8);
	// adc_tmp = *((uint16_t*)&m.adc->adc); // more then one way to skin a rabbit.
	adc_tmp = readHLbyte(atmega128()->adc->adc);
	if(adc_n_sample < (1 << ADC_NUMBER_SAMPLE)){
		adc_n_sample++;
		adc_sample += adc_tmp;
	}else{
		ADC_VALUE[ADC_SELECTOR] = adc_sample >> ADC_NUMBER_SAMPLE;
		adc_n_sample = adc_sample = 0;
	
		if(ADC_SELECTOR < ADC_N_CHANNEL)
			ADC_SELECTOR++;
		else
			ADC_SELECTOR = 0;
		//atmega128()->adc->admux.var &= ~MUX_MASK;
		atmega128()->adc->admux.par.mux = 0;
		//atmega128()->adc->admux.var |= (ADC_CHANNEL_GAIN[ADC_SELECTOR] & MUX_MASK);
		atmega128()->adc->admux.par.mux = ADC_CHANNEL_GAIN[ADC_SELECTOR] & MUX_MASK;
	}
}

/*** Interrupt ***/
ISR(ADC_vect)
{
	if( atmega128_adc.callback.adc_vect ){ atmega128_adc.callback.adc_vect(); }
}

/*** EOF ***/

