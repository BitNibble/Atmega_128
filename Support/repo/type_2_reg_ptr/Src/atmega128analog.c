/*************************************************************************
	ATMEGA128ANALOG
Author:   <sergio.salazar.santos@gmail.com>
License:  GNU General Public License
Hardware: ATmega128
Update:   07/01/2024
Comment:
	Stable
*************************************************************************/
/*** File Library ***/
#include "atmega128analog.h"
#include <avr/interrupt.h>
#include <stdio.h>

/*** File Variable ***/
static ADC0 atmega128_adc;

static volatile int ADC_VALUE[MAX_CHANNEL];
static volatile int ADC_CHANNEL_GAIN[MAX_CHANNEL];
int ADC_N_CHANNEL;
static volatile int ADC_SELECTOR;
static volatile int adc_sample;
uint16_t adc_tmp;
static volatile unsigned char adc_n_sample;

/*** File Header ***/
int ANALOG_read(int selection);

/*** Procedure & Function ***/
ADC0 adc_enable( uint8_t Vreff, uint8_t Divfactor, int n_channel, ... )
// Interrupt running mode setup, and list of channels to be probed
{
	// ATMEGA128enable();

	// LOCAL VARIABLES
	va_list list;
	int i;
	// Initialize variables
	ADC_N_CHANNEL = (n_channel & MUX_MASK);
	ADC_SELECTOR = 0;
	adc_n_sample = 0;
	
	//adc_reg()->admux.var &= ~(3 << REFS0);
	adc_reg()->admux->par.refs = 0;
	switch( Vreff ){
		case 1:
			//adc_reg()->admux.var |= (1 << REFS0);
			adc_reg()->admux->par.refs = 1;
		break;
		case 3:
			//adc_reg()->admux.var |= (3 << REFS0);
			adc_reg()->admux->par.refs = 3;
		break;
		default:
		break;
	}
		
	//adc_reg()->admux.var &= ~(1 << ADLAR);
	adc_reg()->admux->par.adlar = 0;
	// atmega128.adc->admux |= (1 << ADLAR);
	
	va_start(list, n_channel);
	for(i = 0; i < n_channel; i++){
		ADC_CHANNEL_GAIN[i] = va_arg(list, int);
	}
	va_end(list);
	
	//adc_reg()->admux.var &= ~MUX_MASK;
	adc_reg()->admux->par.mux = 0;
	//adc_reg()->admux.var |= (MUX_MASK & ADC_CHANNEL_GAIN[ADC_SELECTOR]);
	adc_reg()->admux->par.mux = ADC_CHANNEL_GAIN[ADC_SELECTOR] & MUX_MASK;
	
	//adc_reg()->adcsra.var |= (1 << ADSC);
	adc_reg()->adcsra->par.adsc = 1;
	//adc_reg()->adcsra.var &= ~(1 << ADFR);
	adc_reg()->adcsra->par.adfr = 0;
	//adc_reg()->adcsra.var |= (1 << ADIE);
	adc_reg()->adcsra->par.adie = 1;
	//adc_reg()->adcsra.var &= ~(7 << ADPS0);
	adc_reg()->adcsra->par.adps = 0;
	
	switch( Divfactor ){
		case 2: // 1
			//adc_reg()->adcsra.var |= (1 << ADPS0);
			adc_reg()->adcsra->par.adps = 1;
		break;
		case 4: // 2
			//adc_reg()->adcsra.var |= (1 << ADPS1);
			adc_reg()->adcsra->par.adps = 2;
		break;
		case 8: // 3
			//adc_reg()->adcsra.var |= (3 << ADPS0);
			adc_reg()->adcsra->par.adps = 3;
		break;
		case 16: // 4
			//adc_reg()->adcsra.var |= (1 << ADPS2);
			adc_reg()->adcsra->par.adps = 4;
		break;
		case 32: // 5
			//adc_reg()->adcsra.var |= (5 << ADPS0);
			adc_reg()->adcsra->par.adps = 5;
		break;
		case 64: // 6
			//adc_reg()->adcsra.var |= (6 << ADPS0);
			adc_reg()->adcsra->par.adps = 6;
		break;
		case 128: // 7
			//adc_reg()->adcsra.var |= (7 << ADPS0);
			adc_reg()->adcsra->par.adps = 7;
		break;
		default:
			//adc_reg()->adcsra.var |= (7 << ADPS0);
			adc_reg()->adcsra->par.adps = 7;
		break;
	}
	
	//adc_reg()->adcsra.var |= (1 << ADEN);
	adc_reg()->adcsra->par.aden = 1;
	
	atmega128_adc.instance = adc_reg();
	atmega128_adc.read = ANALOG_read;
	cpu_reg()->sreg->var |= (1 << GLOBAL_INTERRUPT_ENABLE);
	
	return atmega128_adc;
}

ADC0* adc(void){ return &atmega128_adc; }

int ANALOG_read(int selection)
// Returns selected Channel ADC_VALUE
{
	//uint8_t ADSC_FLAG;
	//ADSC_FLAG = (1 << ADSC);
	//if( !(adc_reg()->adcsra.var & ADSC_FLAG) ){
	if( !adc_reg()->adcsra->par.adsc ){
		// ADC_SELECT
		//adc_reg()->adcsra.var |= (1 << ADSC);
		adc_reg()->adcsra->par.adsc = 1;
	}	
	return ADC_VALUE[selection];
}

/*** File Interrupt ***/
ISR(ADC_vect)
// Function: ANALOG interrupt
// Purpose:  Read Analog Input
{
	// adc_tmp = atmega128.adc->adc.L; // ADCL
	// adc_tmp |= (atmega128.adc->adc.H << 8); // (ADCH << 8);
	// adc_tmp = *((uint16_t*)&m.adc->adc); // more then one way to skin a rabbit.
	adc_tmp = adc_reg()->adc->var;
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
		//adc_reg()->admux.var &= ~MUX_MASK;
		adc_reg()->admux->par.mux = 0;
		//adc_reg()->admux.var |= (ADC_CHANNEL_GAIN[ADC_SELECTOR] & MUX_MASK);
		adc_reg()->admux->par.mux = ADC_CHANNEL_GAIN[ADC_SELECTOR] & MUX_MASK;
	}
}

/***EOF***/

