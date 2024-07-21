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

uint16_t voltage_threshold = BATT_FULL;
uint16_t current_voltage_threshold = 0;
bool battey_is_not_full = true;
uint16_t temperature_threshold = TEMPERATURE_HIGH;

void leds_and_pins_init(void){
	DDRD |= (1<<PORTD0) | (1<<PORTD1) | (1<<PORTD2) | (1<<PORTD3);  // set as outputs
	//DDRB |= (1<<PORTB0); // old FAN Control
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

void idle_mode(void){
	MCUCR &= ~(1<<SE) & (1<<SM0) & (1<<SM1) & (1<<SM2);
	MCUCR |= 1<<SE;
}

void operational_mode(void){
	MCUCR &= ~(1<<SE) & (1<<SM0) & (1<<SM1) & (1<<SM2);
}

void battery_stand_by(void)
{
	   	if (current_voltage_threshold < BATT_LOW)
	   	{
		   	PORTD &= ~(1<<0x00) & (1<<0x01) & (1<<0x02) & (1<<0x03);
			battey_is_not_full = true;
	   	}
	   	if (current_voltage_threshold > BATT_LOW)
	   	{
		   	PORTD &= ~(1<<0x00) & (1<<0x01) & (1<<0x02);
		   	PORTD |= (1<<0x03);
			battey_is_not_full = true;
	   	}
	   	if (current_voltage_threshold > BATT_MID)
	   	{
		   	PORTD &= ~(1<<0x00) & (1<<0x01);
		   	PORTD |= (1<<0x02) | (1<<0x03);
			battey_is_not_full = true;
	   	}
	   	if (current_voltage_threshold > BATT_75_FULL)
	   	{
		   	PORTD &= ~(1<<0x00);
		   	PORTD |= (1<<0x01) | (1<<0x02) | (1<<0x03);
			battey_is_not_full = true;
	   	}
	   	if (current_voltage_threshold >= voltage_threshold)
	   	{
		   	PORTD |= (1<<0x00) | (1<<0x01) | (1<<0x02) | (1<<0x03);
			battey_is_not_full = false;
	   	}  
		_delay_ms(100);
		PORTD &= ~(1<<0x00) & (1<<0x01) & (1<<0x02) & (1<<0x03);
}


void check_temperature(const adc_data_t *adc_data){
	static bool set_hysteresis_temperature = false;	
	uint16_t current_temperature_threshold = adc_data->CH1_TEMPERATURE;
	
	if (current_temperature_threshold < temperature_threshold) // Voltage is less than 2.5V
    {
	 if (!set_hysteresis_temperature)
	 {
	   set_hysteresis_temperature = true;
	   temperature_threshold += 20;
	 }	
	 if (timer2_pwm_ctl(current_temperature_threshold) == PWM_OK)
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
		if (set_hysteresis_temperature)
		{
			set_hysteresis_temperature = false;
			temperature_threshold -= 20;
		}
	}
}

void leds_show_status(bool charger_plugged_in_status, const adc_data_t *adc_data){
	
	current_voltage_threshold = adc_data->CH0_BATT;
	static uint8_t cnt = 0;
	static bool set_hysteresis_voltage = false;
	static bool operational_mode_set = false;
	static bool idle_mode_set = false;
	check_temperature(adc_data);
	
	if(charger_plugged_in_status && battey_is_not_full)
	{
		if (operational_mode_set == false){
			operational_mode_set = true;
			idle_mode_set = false;
			//operational_mode();
			timer1_delay_scan(TIMER_FOR_SCAN);
			
		}

		switch (cnt)
		{
			case 0:
			if (current_voltage_threshold > BATT_LOW)
			{
				PORTD |= 1<<0x03;
				cnt++;
			}
			break;
			case 1:
			if (current_voltage_threshold > BATT_MID)
			{
				PORTD |= 1<<0x02;
		        cnt++;
			}
			else
			{
				PORTD &= ~(1<<0x03) & (1<<0x02) & (1<<0x01) & (1<<0x00);
				cnt = 0;
			}
			break;
			case 2:
			if (current_voltage_threshold > BATT_75_FULL)
			{
				PORTD |= 1<<0x01;
				cnt++;
			}
			else
			{
				PORTD &= ~(1<<0x03) & (1<<0x02) & (1<<0x01) & (1<<0x00);
				cnt = 0;				
			}
			break;
			case 3:
			if (current_voltage_threshold > voltage_threshold)
			{
				if(!set_hysteresis_voltage)
				{
					set_hysteresis_voltage = true;
					voltage_threshold -= 20;
				}
				PORTD |= 1<<0x00;
				cnt++;
			}
			else
			{
				if(set_hysteresis_voltage)
				{
					set_hysteresis_voltage = false;
					voltage_threshold += 20;
				}
				PORTD &= ~(1<<0x03) & (1<<0x02) & (1<<0x01) & (1<<0x00);
				cnt = 0;				
			}
			break;
			case 4:
				PORTD &= ~(1<<0x03) & (1<<0x02) & (1<<0x01) & (1<<0x00);
				battey_is_not_full = false;
				cnt = 0;
				break;		
		}
	}
	else
	{
		cnt = 0;
		if (idle_mode_set == false){
			idle_mode_set = true;
			operational_mode_set = false;
			timer1_delay_stby(TIMER_FOR_STBY);
			//idle_mode();
		}
		else{
			battery_stand_by();
			//idle_mode();	
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

