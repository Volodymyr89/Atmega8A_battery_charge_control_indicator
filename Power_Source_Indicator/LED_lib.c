/*
 * LED_lib.c
 *
 * Created: 10/26/2023 12:37:42 PM
 *  Author: VolodymyrBatih
 */ 


#include <avr/io.h>
#include "ADC_lib.h"
#include "stdbool.h"
#include "config.h"
#include "TIMER1_lib.h"
#include "TIMER2_lib.h"
#include "LED_lib.h"

uint16_t voltage_threshold = 0;
bool battey_is_not_full = true;

void leds_and_pins_init(void){
	DDRD |= (1<<PORTD0) | (1<<PORTD1) | (1<<PORTD2) | (1<<PORTD3) | (1<<PORTD5);  // set as outputs
	DDRB |= (1<<PORTB0) | (1<<PORTB1);
	DDRB |= (1<<PORTB3); // FAN PWM
	PORTD = 0x00; // set zeros
	PORTB = 0x00; // set zeros
}

void charger_control(charger_control_t charger_control){
	if (charger_control == CHARGER_ON){
		//CHARGER_TO_BATTERY_ON;
	}
	else if (charger_control == CHARGER_OFF){
		//CHARGER_TO_BATTERY_OFF;
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

void battery_stand_by(void)
{
	static uint8_t display_counter = 0;
	if (display_counter > DISPLAY_TIME)
	{
	   	if (voltage_threshold < BATT_LOW)
	   	{
		   	PORTD &= ~(1<<0x00) & (1<<0x01) & (1<<0x02) & (1<<0x03);
	   	}
	   	if (voltage_threshold > BATT_LOW)
	   	{
		   	PORTD &= ~(1<<0x00) & (1<<0x01) & (1<<0x02);
		   	PORTD |= (1<<0x03);
			battey_is_not_full = true;
	   	}
	   	if (voltage_threshold > BATT_MID)
	   	{
		   	PORTD &= ~(1<<0x00) & (1<<0x01);
		   	PORTD |= (1<<0x02) | (1<<0x03);
	   	}
	   	if (voltage_threshold > BATT_75_FULL)
	   	{
		   	PORTD &= ~(1<<0x00);
		   	PORTD |= (1<<0x01) | (1<<0x02) | (1<<0x03);
	   	}
	   	if (voltage_threshold >= BATT_FULL)
	   	{
		   	PORTD |= (1<<0x00) | (1<<0x01) | (1<<0x02) | (1<<0x03);
			battey_is_not_full = false;
	   	}  
		_delay_ms(100);
		PORTD &= ~(1<<0x00) & (1<<0x01) & (1<<0x02) & (1<<0x03);
		display_counter = 0;
	}
	else
	{
	   display_counter++;
	}

}

void leds_show_status(const adc_data_t *adc_data, bool charger_plugged_in_status){
	voltage_threshold = adc_data->CH0_BATT;
	uint8_t temperature_threshold = (uint8_t)adc_data->CH1_TEMPERATURE;
	static uint8_t cnt = 0;
	
	if(charger_plugged_in_status && battey_is_not_full)
	{
		if (temperature_threshold < TEMPERATURE_HIGH)
		{
			if (timer2_pwm_ctl(temperature_threshold) == PWM_OK)
			{
				pwm_on();
			}
			else
			{
				fail();
			}
		}
		else
		{
			pwm_off();
		}
		switch (cnt)
		{
			case 0:
			if (voltage_threshold > BATT_LOW)
			{
				PORTD |= 1<<0x03;
				cnt++;
				break;
			}
			case 1:
			if (voltage_threshold > BATT_MID)
			{
				PORTD |= 1<<0x02;
		        cnt++;
				break;
			}
			else
			{
				PORTD &= ~(1<<0x03) & (1<<0x02) & (1<<0x01) & (1<<0x00);
				cnt = 0;
			}
			case 2:
			if (voltage_threshold > BATT_75_FULL)
			{
				PORTD |= 1<<0x01;
				cnt++;
				break;
			}
			else
			{
				PORTD &= ~(1<<0x03) & (1<<0x02) & (1<<0x01) & (1<<0x00);
				cnt = 0;				
			}
			case 3:
			if (voltage_threshold > BATT_FULL)
			{
				PORTD |= 1<<0x00;
				cnt++;
				break;
			}
			else
			{
				PORTD &= ~(1<<0x03) & (1<<0x02) & (1<<0x01) & (1<<0x00);
				cnt = 0;				
			}
			case 4:
				PORTD &= ~(1<<0x03) & (1<<0x02) & (1<<0x01) & (1<<0x00);
				cnt = 0;
				battey_is_not_full = false;
				break;		
		}
	}
	else
	{
		battery_stand_by();
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

