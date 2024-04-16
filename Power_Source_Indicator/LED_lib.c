/*
 * LED_lib.c
 *
 * Created: 10/26/2023 12:37:42 PM
 *  Author: VolodymyrBatih
 */ 


#include <avr/io.h>
#include "ADC_lib.h"
#include "config.h"
#include "TIMER1_lib.h"
#include "LED_lib.h"

typedef enum 
{
  state_0 = 0,
  state_1,
  state_2,
  state_3
}VOLTAGE_STATE_enum;

uint8_t  display_counter = 11;


void leds_and_pins_init(void){
	DDRD |= (1<<PORTD0) | (1<<PORTD1) | (1<<PORTD2) | (1<<PORTD3) | (1<<PORTD5);  // set as outputs
	DDRB |= (1<<PORTB0) | (1<<PORTB1);
	PORTD = 0x00; // set zeros
	PORTB = 0x00; // set zeros
}

void relay_control(relay_control_t relay_control){
	if (relay_control == CHARGER_ON){
		CHARGER_TO_BATTERY_ON;
	}
	else if (relay_control == CHARGER_OFF){
		CHARGER_TO_BATTERY_OFF;
	}
}

void leds_check_greeting_startup(void){
	
		for(int8_t i=3; i>=0; i--){
			_delay_ms(150);
			PORTD |= 1<<i;
		}
		for(int8_t i=3; i>0; i--){
			_delay_ms(150);
			PORTD &= ~(1<<i);
		}
		for(uint8_t i=0; i<=3; i++){
			_delay_ms(150);
			PORTD &= ~(1<<i);
			PORTD |= 1<<(i+1);
		}
		_delay_ms(150);
		PORTD &= ~(0x0F);
}

void leds_show_status(const adc_data_t adc_data, bool charger_plugged_in_status){
    
	VOLTAGE_STATE_enum  voltage_state_t;
	uint16_t voltage_threshold = adc_data.ADC_CH0;
	uint16_t temperature_threshold = adc_data.ADC_CH1;
    
	
	if(charger_plugged_in_status)
	{
		if (temperature_threshold >= TEMPERATURE_LOW)
		{
		  COOLER_OFF;
		}
		else
		{
		  COOLER_ON;
		}
			
		if (voltage_threshold < BATT_LOW)
		{
		  PORTD &= ~(1<<0x00) & (1<<0x01) & (1<<0x02) & (1<<0x03);
		}
		if (voltage_threshold > BATT_LOW)
		{
	      PORTD &= ~(1<<0x00) & (1<<0x01) & (1<<0x02);
		  PORTD |= (1<<0x03);
		}
		if (voltage_threshold > BATT_MID)
		{
		  PORTD &= ~(1<<0x00) & (1<<0x01);
	      PORTD |= (1<<0x02) | (1<<0x03);
		}
		if (voltage_threshold > BATT_ALMOST_FULL)
		{
		  PORTD &= ~(1<<0x00);
		  PORTD |= (1<<0x01) | (1<<0x02) | (1<<0x03);
		}
		if (voltage_threshold > BATT_FULL)
		{
		  relay_control(CHARGER_OFF);
		  PORTD |= (1<<0x00) | (1<<0x01) | (1<<0x02) | (1<<0x03);
		}
		else
		{
		  relay_control(CHARGER_ON);
		}			
	 }
	 else 
	 { 
		COOLER_OFF;
		relay_control(CHARGER_OFF);
		display_counter--;
		if (display_counter <= 1)
		{
			if (voltage_threshold < BATT_LOW)
			{
				PORTD &= ~(1<<0x00) & (1<<0x01) & (1<<0x02) & (1<<0x03);
			}
			if (voltage_threshold > BATT_LOW)
			{
				PORTD &= ~(1<<0x00) & (1<<0x01) & (1<<0x02);
				PORTD |= (1<<0x03);
			}
			if (voltage_threshold > BATT_MID)
			{
				PORTD &= ~(1<<0x00) & (1<<0x01);
				PORTD |= (1<<0x02) | (1<<0x03);
			}
			if (voltage_threshold > BATT_ALMOST_FULL)
			{
				PORTD &= ~(1<<0x00);
				PORTD |= (1<<0x01) | (1<<0x02) | (1<<0x03);
			}
			if (voltage_threshold > BATT_FULL)
			{
				PORTD |= (1<<0x00) | (1<<0x01) | (1<<0x02) | (1<<0x03);
			}
			_delay_ms(150);
			display_counter = 11;	
		}
		else
		{
			PORTD &= ~(1<<0x00) & (1<<0x01) & (1<<0x02) & (1<<0x03);
		}
		 
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

