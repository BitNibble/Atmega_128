#include "atmega128analog.h"

/*** File Variables ***/
static ADC0 atmega128_adc;
static volatile int ADC_VALUE[MAX_CHANNEL] = {0};
static volatile int ADC_CHANNEL_GAIN[MAX_CHANNEL] = {0};
static volatile int ADC_N_CHANNEL = 0;
static volatile int ADC_SELECTOR = 0;
static volatile int adc_sample = 0;
static volatile unsigned char adc_n_sample = 0;
static uint16_t adc_tmp = 0;

/*** File Header ***/
int ANALOG_read(int selection);

/*** Procedure & Function ***/
ADC0 adc_enable(uint8_t Vreff, uint8_t Divfactor, int n_channel, ...)
{
	// Initialize variables
	va_list list;
	int i;

	ADC_N_CHANNEL = (n_channel & MUX_MASK);
	ADC_SELECTOR = 0;
	adc_n_sample = 0;

	// Setup Voltage Reference (VREFF)
	adc_instance()->admux.par.refs = 0;
	switch (Vreff) {
		case 0:
		atmega128_adc.par.VREFF = 0;
		break;
		case 1:
		adc_instance()->admux.par.refs = 1;
		atmega128_adc.par.VREFF = 1;
		break;
		case 3:
		adc_instance()->admux.par.refs = 3;
		atmega128_adc.par.VREFF = 3;
		break;
		default:
		atmega128_adc.par.VREFF = 0;
		break;
	}

	// Clear ADLAR (Left Adjust Result)
	adc_instance()->admux.par.adlar = 0;

	// Populate channel gain values
	va_start(list, n_channel);
	for (i = 0; i < n_channel; i++) {
		ADC_CHANNEL_GAIN[i] = va_arg(list, int);
	}
	va_end(list);

	// Setup MUX for the first channel
	adc_instance()->admux.par.mux = ADC_CHANNEL_GAIN[ADC_SELECTOR] & MUX_MASK;

	// Start conversion and enable interrupt
	adc_instance()->adcsra.par.adsc = 1;
	adc_instance()->adcsra.par.adie = 1;

	// Set division factor for ADC clock
	adc_instance()->adcsra.par.adps = (Divfactor <= 128 && Divfactor >= 2) ? Divfactor : 128;
	atmega128_adc.par.DIVISION_FACTOR = Divfactor;

	// Enable ADC
	adc_instance()->adcsra.par.aden = 1;

	// Link instance and function pointers
	atmega128_adc.instance = adc_instance();
	atmega128_adc.read = ANALOG_read;

	// Enable global interrupts
	cpu_instance()->sreg.reg |= (1 << GLOBAL_INTERRUPT_ENABLE);

	return atmega128_adc;
}

ADC0* adc(void)
{
	return &atmega128_adc;
}

int ANALOG_read(int selection)
{
	if (!adc_instance()->adcsra.par.adsc) {
		adc_instance()->adcsra.par.adsc = 1;
	}
	return ADC_VALUE[selection];
}

/*** File Interrupt ***/
ISR(ADC_vect)
{
	adc_tmp = readhlbyte(adc_instance()->adc);
	
	if (adc_n_sample < (1 << ADC_NUMBER_SAMPLE)) {
		adc_n_sample++;
		adc_sample += adc_tmp;
		} else {
		ADC_VALUE[ADC_SELECTOR] = adc_sample >> ADC_NUMBER_SAMPLE;
		adc_n_sample = adc_sample = 0;

		ADC_SELECTOR = (ADC_SELECTOR < ADC_N_CHANNEL) ? ADC_SELECTOR + 1 : 0;

		// Update MUX for next channel
		adc_instance()->admux.par.mux = ADC_CHANNEL_GAIN[ADC_SELECTOR] & MUX_MASK;
	}
}
