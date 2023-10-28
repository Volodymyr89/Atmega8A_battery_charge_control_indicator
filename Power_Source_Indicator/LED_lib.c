/*
 * LED_lib.c
 *
 * Created: 10/26/2023 12:37:42 PM
 *  Author: VolodymyrBatih
 */ 
#include <util/delay.h>
#include <avr/io.h>
#include "ADC_lib.h"


void leds_init(void){
	DDRD |= 1<<PORTD0 | 1<<PORTD1 | 1<<PORTD2 | 1<<PORTD3;  // set as outputs 
}

void leds_check_greeting_startup(void){
	
		for(uint8_t i=0; i<3; i++){
			_delay_ms(100);
			PORTD |= 1<<i;
		}
		PORTD=8U;
		_delay_ms(100);
		for(uint8_t i=3; i>0; i--){
			_delay_ms(100);
			PORTD &= ~(1<<i);
			PORTD |= 1<<i-1;
		}
	
}

void leds_show_status(adc_data_t adc_data){
	adc_data.CH1;
}