/*
 * ADC_lib.c
 *
 * Created: 10/26/2023 12:27:28 PM
 *  Author: VolodymyrBatih
 */ 

#include <stddef.h>
#include "ADC_lib.h"
#include "config.h"


	
void adc_init(void){
	ADMUX |= 1<<REFS0 | 1<<REFS1; // select internal ref source
	ADCSRA |= 1<<ADPS0 | 1<<ADPS1 | 1<<ADPS2; // 128 prescaler
}


void adc_read(adc_data_t *adcdata){
	if(adcdata != NULL){
		uint16_t adc_data = 0;
		ADCSRA |= (1<<ADEN); 
		ADMUX &= ADC_CHANNEL0_BATT; //reset first 4 channels ADC0
		ADCSRA |= 1<<ADSC; //start conversion
		while (ADCSRA&(1<<ADSC)){} //wait until conversion is complete
		adc_data = ADCL;
		adc_data |= (ADCH<<8);
		adcdata->CH0_BATT = adc_data;
		
		ADMUX |= ADC_CHANNEL1_TEMPERATURE; // ADC1
		ADCSRA |= 1<<ADSC; //start conversion
		while (ADCSRA&(1<<ADSC)){} //wait until conversion is complete
		adc_data = ADCL;
		adc_data |= (ADCH<<8);
		adcdata->CH1_TEMPERATURE = adc_data;
		//ADCSRA &= ~(1<<ADEN); // disable ADC	
	}
}

