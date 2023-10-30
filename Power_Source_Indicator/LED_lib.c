/*
 * LED_lib.c
 *
 * Created: 10/26/2023 12:37:42 PM
 *  Author: VolodymyrBatih
 */ 
#define F_CPU 1000000UL

#include <util/delay.h>
#include <avr/io.h>
#include "ADC_lib.h"
#include "config.h"

adc_data_t adc_data;

void leds_init(void){
	DDRD |= 1<<PORTD0 | 1<<PORTD1 | 1<<PORTD2 | 1<<PORTD3;  // set as outputs 
	PORTD &= ~(1<<PORTD0); // set zeros
}

void leds_check_greeting_startup(void){
	
		for(uint8_t i=0; i<=3; i++){
			_delay_ms(100);
			PORTD |= 1<<i;
		}
		PORTD=8U;
		_delay_ms(100);
		for(uint8_t i=3; i>=0; i--){
			_delay_ms(100);
			PORTD &= ~(1<<i);
			PORTD |= 1<<(i-1);
		}
}

void leds_show_status(const adc_data_t adc_data, bool charger_plugged_in_status){
	for (uint8_t i=0; i<=3; i++){
		//if (adc_data.CH0 <= BATT_LOW){
		//	PORTD |= (1<<i);
		//}
	}
}

bool charger_status(void){
	
	static bool charger_plugged_in;
	
	if(CHARGER_PLUGGED_IN){
		charger_plugged_in = true;
	}
	else{
		charger_plugged_in = false;
	}
		return charger_plugged_in;
}
