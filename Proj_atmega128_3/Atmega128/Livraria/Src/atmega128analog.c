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
	
	//adc_instance()->admux.reg &= ~(3 << REFS0);
	adc_instance()->admux.par.refs = 0;
	switch( Vreff ){
		case 0:
			atmega128_adc.par.VREFF = 0;
		break;
		case 1:
			//adc_instance()->admux.reg |= (1 << REFS0);
			adc_instance()->admux.par.refs = 1;
			atmega128_adc.par.VREFF = 1;
		break;
		case 3:
			//adc_instance()->admux.reg |= (3 << REFS0);
			adc_instance()->admux.par.refs = 3;
			atmega128_adc.par.VREFF = 3;
		break;
		default:
			atmega128_adc.par.VREFF = 0;
		break;
	}
		
	//adc_instance()->admux.reg &= ~(1 << ADLAR);
	adc_instance()->admux.par.adlar = 0;
	// atmega128.adc->admux |= (1 << ADLAR);
	
	va_start(list, n_channel);
	for(i = 0; i < n_channel; i++){
		ADC_CHANNEL_GAIN[i] = va_arg(list, int);
	}
	va_end(list);
	
	//adc_instance()->admux.reg &= ~MUX_MASK;
	adc_instance()->admux.par.mux = 0;
	//adc_instance()->admux.reg |= (MUX_MASK & ADC_CHANNEL_GAIN[ADC_SELECTOR]);
	adc_instance()->admux.par.mux = ADC_CHANNEL_GAIN[ADC_SELECTOR] & MUX_MASK;
	
	//adc_instance()->adcsra.reg |= (1 << ADSC);
	adc_instance()->adcsra.par.adsc = 1;
	//adc_instance()->adcsra.reg &= ~(1 << ADFR);
	adc_instance()->adcsra.par.adfr = 0;
	//adc_instance()->adcsra.reg |= (1 << ADIE);
	adc_instance()->adcsra.par.adie = 1;
	//adc_instance()->adcsra.reg &= ~(7 << ADPS0);
	adc_instance()->adcsra.par.adps = 0;
	
	switch( Divfactor ){
		case 2: // 1
			//adc_instance()->adcsra.reg |= (1 << ADPS0);
			adc_instance()->adcsra.par.adps = 1;
			atmega128_adc.par.DIVISION_FACTOR = 2;
		break;
		case 4: // 2
			//adc_instance()->adcsra.reg |= (1 << ADPS1);
			adc_instance()->adcsra.par.adps = 2;
			atmega128_adc.par.DIVISION_FACTOR = 4;
		break;
		case 8: // 3
			//adc_instance()->adcsra.reg |= (3 << ADPS0);
			adc_instance()->adcsra.par.adps = 3;
			atmega128_adc.par.DIVISION_FACTOR = 8;
		break;
		case 16: // 4
			//adc_instance()->adcsra.reg |= (1 << ADPS2);
			adc_instance()->adcsra.par.adps = 4;
			atmega128_adc.par.DIVISION_FACTOR = 16;
		break;
		case 32: // 5
			//adc_instance()->adcsra.reg |= (5 << ADPS0);
			adc_instance()->adcsra.par.adps = 5;
			atmega128_adc.par.DIVISION_FACTOR = 32;
		break;
		case 64: // 6
			//adc_instance()->adcsra.reg |= (6 << ADPS0);
			adc_instance()->adcsra.par.adps = 6;
			atmega128_adc.par.DIVISION_FACTOR = 64;
		break;
		case 128: // 7
			//adc_instance()->adcsra.reg |= (7 << ADPS0);
			adc_instance()->adcsra.par.adps = 7;
			atmega128_adc.par.DIVISION_FACTOR = 128;
		break;
		default:
			//adc_instance()->adcsra.reg |= (7 << ADPS0);
			adc_instance()->adcsra.par.adps = 7;
			atmega128_adc.par.DIVISION_FACTOR = 128;
		break;
	}
	
	//adc_instance()->adcsra.reg |= (1 << ADEN);
	adc_instance()->adcsra.par.aden = 1;
	
	atmega128_adc.instance = adc_instance();
	atmega128_adc.read = ANALOG_read;
	cpu_instance()->sreg.reg |= (1 << GLOBAL_INTERRUPT_ENABLE);
	
	return atmega128_adc;
}

ADC0* adc(void){ return &atmega128_adc; }

int ANALOG_read(int selection)
// Returns selected Channel ADC_VALUE
{
	//uint8_t ADSC_FLAG;
	//ADSC_FLAG = (1 << ADSC);
	//if( !(adc_instance()->adcsra.reg & ADSC_FLAG) ){
	if( !adc_instance()->adcsra.par.adsc ){
		// ADC_SELECT
		//adc_instance()->adcsra.reg |= (1 << ADSC);
		adc_instance()->adcsra.par.adsc = 1;
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
	adc_tmp = readhlbyte(adc_instance()->adc);
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
		//adc_instance()->admux.reg &= ~MUX_MASK;
		adc_instance()->admux.par.mux = 0;
		//adc_instance()->admux.reg |= (ADC_CHANNEL_GAIN[ADC_SELECTOR] & MUX_MASK);
		adc_instance()->admux.par.mux = ADC_CHANNEL_GAIN[ADC_SELECTOR] & MUX_MASK;
	}
}

/***EOF***/

