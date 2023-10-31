/*
 * ADC_lib.c
 *
 * Created: 10/26/2023 12:27:28 PM
 *  Author: VolodymyrBatih
 */ 

	#include "ADC_lib.h"
	
void adc_enable(bool adc_enable){
	if (adc_enable){
		ADCSRA |= 1<<ADEN;
	}
	else{
		ADCSRA &= ~(1<<ADEN);
	}
}
void adc_init(void){
	adc_enable(false);
	ADMUX |= 1<<REFS0 | 1<<REFS1; // select ref source, and left align
	ADCSRA |= 1<<ADPS0 | 1<<ADPS1 | 1<<ADPS2; // 128 prescaler
}

void adc_select_adc0_channel(void){
	ADMUX &= ~(1<<MUX0); //sel 0 channel
}

void adc_select_adc1_channel(void){
	ADMUX |= 1<<MUX0;
}



void adc_start_conversion(void){
	ADCSRA |= 1<<ADSC; //start conversion
}
