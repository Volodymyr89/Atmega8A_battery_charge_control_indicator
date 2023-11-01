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
#include "TIMER0_lib.h"
#include "LED_lib.h"

adc_data_t adc_data;

void leds_init(void){
	DDRD |= 1<<PORTD0 | 1<<PORTD1 | 1<<PORTD2 | 1<<PORTD3;  // set as outputs 
	PORTD = 0x00; // set zeros
}

void relay_control(relay_control_t relay_control){
	if (relay_control == CHARGER_RELAY_ON){
		CHARGER_TO_BATTERY_RELAY_ON;
	}
	else if (relay_control == CHARGER_RELAY_OFF){
		CHARGER_TO_BATTERY_RELAY_OFF;
	}
	else if (relay_control == OUT_RELAY_ON){
		BATTERY_TO_OUT_RELAY_ON;
	}
	else if (relay_control == OUT_RELAY_OFF){
		BATTERY_TO_OUT_RELAY_OFF;
	}
}

void leds_check_greeting_startup(void){
	
		for(uint8_t i=0; i<=3; i++){
			_delay_ms(100);
			PORTD |= 1<<i;
		}
		for(uint8_t i=0; i<3; i++){
			_delay_ms(100);
			PORTD &= ~(1<<i);
		}
		for(uint8_t i=3; i>=0; i--){
			_delay_ms(100);
			PORTD &= ~(1<<i);
			PORTD |= 1<<(i-1);
		}
}

void leds_show_status(const adc_data_t adc_data, bool charger_plugged_in_status){
	static bool timer_set_4s = false;
	static bool timer_set_300ms = false;
	static uint8_t shift_led_blink = 0;
	
	if(charger_plugged_in_status){	
	COOLER_ON;
			if ((adc_data.CH0) && (adc_data.CH1) < BATT_LOW){
				shift_led_blink = 0;
			}
			else if ((adc_data.CH0) && (adc_data.CH1) >= BATT_LOW){
				shift_led_blink = 1;				
				PORTD |= 1<<0x00;
			}
			else if ((adc_data.CH0) && (adc_data.CH1) >= BATT_MID){
				shift_led_blink = 2;
				PORTD |= 1<<0x03;
			}
			else if ((adc_data.CH0) && (adc_data.CH1) >= BATT_ALMOST_FULL){
				shift_led_blink = 3;				
				PORTD |= 1<<0x07;
			}
			else if ((adc_data.CH0) && (adc_data.CH1) >= BATT_FULL){
				PORTD = 0x0F; //don't reset LEDs				
				_delay_ms(300);
				PORTD = 0x00; //reset LEDs
			}
			if((adc_data.CH0) && (adc_data.CH1) > BATT_FULL){
				if(!timer_set_4s){
					timer_set_4s = true;
					timer_set_300ms = false;
					if (timer1_delay(4000) == TIMER_OK){}
				}
				else{
					PORTD ^= 1<<shift_led_blink;
					if(!timer_set_300ms){
						timer_set_300ms = true;
						timer_set_4s = false;
						if (timer1_delay(4000) == TIMER_OK){}
					}
				}
			}
	}
	else{
		COOLER_OFF;
		if(!timer_set_4s){
			timer_set_4s = true;
			timer_set_300ms = false;
			if (timer1_delay(4000) == TIMER_OK){}
		}		
		if (adc_data.CH0 < BATT_LOW){
			PORTD = 0x00; //reset all LEDs
		}
		else if (adc_data.CH0 >= BATT_LOW){
			
			PORTD = 0x01; // LED0
		}
		else if (adc_data.CH0 >= BATT_MID){
			
			PORTD = 0x03; // LED0, 1
		}
		else if (adc_data.CH0 >= BATT_ALMOST_FULL){
			
			PORTD = 0x07; // LED0, 1, 2
		}
		else if (adc_data.CH0 >= BATT_FULL){
			PORTD = 0x0F; // LED0, 1, 2, 3
		}
		_delay_ms(300);
		PORTD = 0x00; //reset LEDs
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
